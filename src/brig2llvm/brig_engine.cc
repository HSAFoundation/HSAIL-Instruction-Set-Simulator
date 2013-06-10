//===- brig_engine.cc - BRIG Interpreter / Dynamic compiler ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_engine.h"
#include "brig_runtime.h"

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Type.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Support/IRReader.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/Memory.h"
#include <cerrno>

#include <dlfcn.h>

#ifdef __linux__
// These includes used by LLIMCJITMemoryManager::getPointerToNamedFunction()
// for Glibc trickery. Look comments in this function for more information.
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef __CYGWIN__
#include <cygwin/version.h>
#if defined(CYGWIN_VERSION_DLL_MAJOR) && CYGWIN_VERSION_DLL_MAJOR<1007
#define DO_NOTHING_ATEXIT 1
#endif
#endif

namespace {

void llvm_initialize(void) {
  LLVMLinkInMCJIT();
}

// Memory manager for MCJIT
class LLIMCJITMemoryManager : public llvm::JITMemoryManager {
 public:
  llvm::SmallVector<llvm::sys::MemoryBlock, 16> AllocatedDataMem;
  llvm::SmallVector<llvm::sys::MemoryBlock, 16> AllocatedCodeMem;
  llvm::SmallVector<llvm::sys::MemoryBlock, 16> FreeCodeMem;

  LLIMCJITMemoryManager() {}
  ~LLIMCJITMemoryManager();

  virtual uint8_t *allocateCodeSection(uintptr_t Size, unsigned Alignment,
                                       unsigned SectionID);

  virtual uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment,
                                       unsigned SectionID);

  virtual void *getPointerToNamedFunction(const std::string &Name,
                                          bool AbortOnFailure = true);

  // Invalidate instruction cache for code sections. Some platforms with
  // separate data cache and instruction cache require explicit cache flush,
  // otherwise JIT code manipulations (like resolved relocations) will get to
  // the data cache but not to the instruction cache.
  virtual void invalidateInstructionCache();

  // The MCJITMemoryManager doesn't use the following functions, so we don't
  // need implement them.
  virtual void setMemoryWritable() {
    llvm_unreachable("Unexpected call!");
  }
  virtual void setMemoryExecutable() {
    llvm_unreachable("Unexpected call!");
  }
  virtual void setPoisonMemory(bool poison) {
    llvm_unreachable("Unexpected call!");
  }
  virtual void AllocateGOT() {
    llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t *getGOTBase() const {
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual uint8_t *startFunctionBody(const llvm::Function *F,
                                     uintptr_t &ActualSize){
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual uint8_t *allocateStub(const llvm::GlobalValue* F, unsigned StubSize,
                                unsigned Alignment) {
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual void endFunctionBody(const llvm::Function *F, uint8_t *FunctionStart,
                               uint8_t *FunctionEnd) {
    llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t *allocateSpace(intptr_t Size, unsigned Alignment) {
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual uint8_t *allocateGlobal(uintptr_t Size, unsigned Alignment) {
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual void deallocateFunctionBody(void *Body) {
    llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t* startExceptionTable(const llvm::Function* F,
                                       uintptr_t &ActualSize) {
    llvm_unreachable("Unexpected call!");
    return 0;
  }
  virtual void endExceptionTable(const llvm::Function *F, uint8_t *TableStart,
                                 uint8_t *TableEnd, uint8_t* FrameRegister) {
    llvm_unreachable("Unexpected call!");
  }
  virtual void deallocateExceptionTable(void *ET) {
    llvm_unreachable("Unexpected call!");
  }
};

uint8_t *LLIMCJITMemoryManager::allocateDataSection(uintptr_t Size,
                                                    unsigned Alignment,
                                                    unsigned SectionID) {
  if (!Alignment)
    Alignment = 16;
  uint8_t *Addr = (uint8_t*)calloc((Size + Alignment - 1)/Alignment, Alignment);
  AllocatedDataMem.push_back(llvm::sys::MemoryBlock(Addr, Size));
  return Addr;
}

uint8_t *LLIMCJITMemoryManager::allocateCodeSection(uintptr_t Size,
                                                    unsigned Alignment,
                                                    unsigned SectionID) {
  if (!Alignment)
    Alignment = 16;
  unsigned NeedAllocate = Alignment * ((Size + Alignment - 1)/Alignment + 1);
  uintptr_t Addr = 0;
  // Look in the list of free code memory regions and use a block there if one
  // is available.
  for (int i = 0, e = FreeCodeMem.size(); i != e; ++i) {
    llvm::sys::MemoryBlock &MB = FreeCodeMem[i];
    if (MB.size() >= NeedAllocate) {
      Addr = (uintptr_t)MB.base();
      uintptr_t EndOfBlock = Addr + MB.size();
      // Align the address.
      Addr = (Addr + Alignment - 1) & ~(uintptr_t)(Alignment - 1);
      // Store cutted free memory block.
      FreeCodeMem[i] = llvm::sys::MemoryBlock((void*)(Addr + Size),
                                              EndOfBlock - Addr - Size);
      return (uint8_t*)Addr;
    }
  }

  // No pre-allocated free block was large enough. Allocate a new memory region.
  llvm::sys::MemoryBlock MB =
    llvm::sys::Memory::AllocateRWX(NeedAllocate, 0, 0);

  AllocatedCodeMem.push_back(MB);
  Addr = (uintptr_t)MB.base();
  uintptr_t EndOfBlock = Addr + MB.size();
  // Align the address.
  Addr = (Addr + Alignment - 1) & ~(uintptr_t)(Alignment - 1);
  // The AllocateRWX may allocate much more memory than we need. In this case,
  // we store the unused memory as a free memory block.
  unsigned FreeSize = EndOfBlock-Addr-Size;
  if (FreeSize > 16)
    FreeCodeMem.push_back(llvm::sys::MemoryBlock((void*)(Addr + Size),
                                                 FreeSize));

  // Return aligned address
  return (uint8_t*)Addr;
}

void LLIMCJITMemoryManager::invalidateInstructionCache() {
  for (int i = 0, e = AllocatedCodeMem.size(); i != e; ++i)
    llvm::sys::Memory::InvalidateInstructionCache(AllocatedCodeMem[i].base(),
                                            AllocatedCodeMem[i].size());
}

static int jit_noop() {
  return 0;
}

void *LLIMCJITMemoryManager::getPointerToNamedFunction(const std::string &Name,
                                                       bool AbortOnFailure) {
#if defined(__linux__)
  //===--------------------------------------------------------------------===//
  // Function stubs that are invoked instead of certain library calls
  //
  // Force the following functions to be linked in to anything that uses the
  // JIT. This is a hack designed to work around the all-too-clever Glibc
  // strategy of making these functions work differently when inlined vs. when
  // not inlined, and hiding their real definitions in a separate archive file
  // that the dynamic linker can't see. For more info, search for
  // 'libc_nonshared.a' on Google, or read http://llvm.org/PR274.
  if (Name == "stat") return (void*)(intptr_t)&stat;
  if (Name == "fstat") return (void*)(intptr_t)&fstat;
  if (Name == "lstat") return (void*)(intptr_t)&lstat;
  if (Name == "stat64") return (void*)(intptr_t)&stat64;
  if (Name == "fstat64") return (void*)(intptr_t)&fstat64;
  if (Name == "lstat64") return (void*)(intptr_t)&lstat64;
  if (Name == "atexit") return (void*)(intptr_t)&atexit;
  if (Name == "mknod") return (void*)(intptr_t)&mknod;
#endif // __linux__

  // We should not invoke parent's ctors/dtors from generated main()!
  // On Mingw and Cygwin, the symbol __main is resolved to
  // callee's(eg. tools/lli) one, to invoke wrong duplicated ctors
  // (and register wrong callee's dtors with atexit(3)).
  // We expect ExecutionEngine::runStaticConstructorsDestructors()
  // is called before ExecutionEngine::runFunctionAsMain() is called.
  if (Name == "__main") return (void*)(intptr_t)&jit_noop;

  const char *NameStr = Name.c_str();
  void *Ptr = llvm::sys::DynamicLibrary::SearchForAddressOfSymbol(NameStr);
  if (Ptr) return Ptr;

  // If it wasn't found and if it starts with an underscore ('_') character,
  // try again without the underscore.
  if (NameStr[0] == '_') {
    Ptr = llvm::sys::DynamicLibrary::SearchForAddressOfSymbol(NameStr+1);
    if (Ptr) return Ptr;
  }

  if (AbortOnFailure)
    llvm::report_fatal_error("Program used external function '" + Name +
                             "' which could not be resolved!");
  return 0;
}

LLIMCJITMemoryManager::~LLIMCJITMemoryManager() {
  for (unsigned i = 0, e = AllocatedCodeMem.size(); i != e; ++i)
    llvm::sys::Memory::ReleaseRWX(AllocatedCodeMem[i]);
  for (unsigned i = 0, e = AllocatedDataMem.size(); i != e; ++i)
    free(AllocatedDataMem[i].base());
}

}

namespace hsa {
namespace brig {

static ForceBrigRuntimeLinkage runtime;

BrigEngine::BrigEngine(hsa::brig::BrigProgram &BP,
                       bool forceInterpreter,
                       char optLevel) : EE_(NULL), M_(BP.M.get()) {
  init(forceInterpreter, optLevel);
}

BrigEngine::BrigEngine(llvm::Module *Mod,
                       bool forceInterpreter,
                       char optLevel) : EE_(NULL), M_(Mod) {
  init(forceInterpreter, optLevel);
}

#ifdef __arm__
static bool isHardFP(int a, int b, int c) {
  assert((b == 0x314159 || b == 0x271828) && "ABI Madness");
  return b == 0x271828;
}

static bool isHardFP(void) {
  union {
    bool (*a)(int, int, int);
    bool (*b)(float, int, int);
  } foo = { isHardFP };
  return foo.b(0, 0x314159, 0x271828);
}
#endif // __arm__

static std::set<std::string> loadedLibs;

void BrigEngine::init(bool forceInterpreter, char optLevel) {

  Dl_info info;
  char *threnv = getenv("SIMTHREADS");
  if (threnv != NULL && atoi(threnv) > 0) {
    numProcessors = atoi(threnv);
  } else if(sysconf(_SC_NPROCESSORS_CONF) > 0) {
    numProcessors = sysconf(_SC_NPROCESSORS_CONF);
  } else {
    numProcessors = 1;
  }

  int err = dladdr(&runtime, &info);
  assert(err && info.dli_fname &&
         "How are we executing if we haven't even been loaded?!");
  if(loadedLibs.insert(std::string(info.dli_fname)).second)
    llvm::sys::DynamicLibrary::LoadLibraryPermanently(info.dli_fname);

  assert(!EE_ && "BrigEngine was already constructed?!");

  // If we have a native target, initialize it to ensure it is linked in and
  // usable by the JIT.
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  // load the whole bitcode file eagerly
  std::string errorMsg;
  if (M_->MaterializeAllPermanently(&errorMsg)) {
    llvm::errs() << "bitcode didn't read correctly.\n";
    llvm::errs() << "Reason: " << errorMsg << "\n";
    exit(1);
  }

  llvm::EngineBuilder builder(M_);
  builder.setErrorStr(&errorMsg);
  builder.setEngineKind(forceInterpreter
                        ? llvm::EngineKind::Interpreter
                        : llvm::EngineKind::JIT);

  LLIMCJITMemoryManager *JMM = NULL;
  if(!forceInterpreter) {
    LLIMCJITMemoryManager *JMM = new LLIMCJITMemoryManager();
    builder.setJITMemoryManager(JMM);
  }

  llvm::CodeGenOpt::Level OLvl = llvm::CodeGenOpt::Default;
  switch (optLevel) {
  default:
    llvm::errs() << "Invalid optimization level.\n";
    exit(1);
  case ' ': break;
  case '0': OLvl = llvm::CodeGenOpt::None; break;
  case '1': OLvl = llvm::CodeGenOpt::Less; break;
  case '2': OLvl = llvm::CodeGenOpt::Default; break;
  case '3': OLvl = llvm::CodeGenOpt::Aggressive; break;
  }
  builder.setOptLevel(OLvl);

  builder.setUseMCJIT(true);

  llvm::TargetOptions options;
  options.JITEmitDebugInfo = true;

#ifdef __arm__
  if(isHardFP()) options.FloatABIType = llvm::FloatABI::Hard;
#endif // __arm__

  builder.setTargetOptions(options);

  EE_ = builder.create();
  if (!EE_) {
    if (!errorMsg.empty())
      llvm::errs() << "Error creating EE: " << errorMsg << "\n";
    else
      llvm::errs() << "Unknown error creating EE!\n";
    exit(1);
  }

  EE_->DisableLazyCompilation(true);

  // Run static constructors.
  EE_->runStaticConstructorsDestructors(false);

  for (llvm::Module::iterator I = M_->begin(), E = M_->end(); I != E; ++I) {
    llvm::Function *Fn = &*I;
    if (!Fn->isDeclaration())
      EE_->getPointerToFunction(Fn);
  }

  if(JMM)
    JMM->invalidateInstructionCache();
}


typedef void *(*EntryFunPtrTy)(void*);

// a struct that adds fields used by the threads that run the WorkItemLoop
struct WorkItemLoopThreadInfo : public ThreadInfo {
  EntryFunPtrTy EntryFunPtr;
  uint32_t absidLow;
  uint32_t absidStep;
  uint32_t groupSize;
  pthread_barrier_t *barriers;

  WorkItemLoopThreadInfo(uint32_t NDRangeSize, uint32_t workdim,
                         uint32_t workGroupSize[3], uint32_t workItemAbsId[3],
                         pthread_barrier_t *barrier,
                         void *const *args, size_t size,
                         EntryFunPtrTy EntryFunPtr,
                         uint32_t absidLow, uint32_t absidStep,
                         uint32_t groupSize, pthread_barrier_t *barriers) :
    ThreadInfo(NDRangeSize, workdim, workGroupSize, workItemAbsId, barrier, args, size),
    EntryFunPtr(EntryFunPtr), absidLow(absidLow), absidStep(absidStep),
    groupSize(groupSize), barriers(barriers) {
  }
};

static uint32_t roundUp(int val, int multiple) {
  return ((val + multiple - 1) / multiple) * multiple;
}

// the workItemLoop runs a set of workItems (from different workGroups)
// all in the same pthread.  It assigns the workItems a barrier
// based on the workGroupId. (absid / workGroupSize)

static void *workItemLoop(void *vargs) {
  void **args = (void **) vargs;
  WorkItemLoopThreadInfo *thrInfo = (WorkItemLoopThreadInfo *) (args[0]);
  // compute size of the last group
  uint32_t lastGroupSize = thrInfo->NDRangeSize % thrInfo->groupSize;
  uint32_t lastGroupNum = (roundUp(thrInfo->NDRangeSize, thrInfo->groupSize) / thrInfo->groupSize) - 1;
  if (lastGroupSize == 0) lastGroupSize = thrInfo->groupSize;
  for(uint32_t absid = thrInfo->absidLow; absid < thrInfo->NDRangeSize; absid += thrInfo->absidStep ) {
    thrInfo->workItemAbsId[0] = absid;
    uint32_t workGroupNum = absid / thrInfo->groupSize;
    thrInfo->barrier = &thrInfo->barriers[workGroupNum];
    // insert correct groupsize if we are in last group
    if (workGroupNum == lastGroupNum) {
      thrInfo->workGroupSize[0] = lastGroupSize;
    }
    // all other fields such as argsArray, etc were set up when thrInfo created
    (thrInfo->EntryFunPtr)(vargs);
  }
  return NULL;
}


void BrigEngine::launch(llvm::Function *EntryFn,
                        llvm::ArrayRef<void *> args,
                        uint32_t blockNum,
                        uint32_t workGroupSize) {

  /***
   *  Note: This interface is currently built on the assumption that
   *  the actual NDRangeSize is blockNum * _workGroupSize.  In other
   *  words, blockNum is the grid dimensions.  The latest HSAIL
   *  programmer's guide supports NDRangeSizes that are not exact
   *  multiples of the workGroupSize.  So eventually the interface to
   *  this routine should take NDRangeSize and compute blockNum from
   *  that instead of the other way around (would have to round up).
   *  Right now there might be brig_reader tests that depend on the
   *  current interface that would have to change.
   ***/

  uint32_t NDRangeSize = blockNum * workGroupSize;


  assert(blockNum && workGroupSize && "Thread count too low");

  EntryFunPtrTy EntryFunPtr =
    (EntryFunPtrTy)(intptr_t) EE_->getPointerToFunction(EntryFn);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_barrierattr_t barrierAttr;
  pthread_barrierattr_init(&barrierAttr);

  /***
   * Currently we use one barrier per block (although we probably
   * could get away with a number of barriers equal to the number of
   * concurrently executing workgroups).  Another optimization would
   * be to avoid barrier initialization at all if we had detected that
   * the kernel never uses barriers (nor calls external functions)
   ***/
  pthread_barrier_t *barriers = new pthread_barrier_t[blockNum];

  // compute how many pthreads we will start we need at least as many
  // as the incoming workGroupSize but we will also try to keep all
  // the processors busy by using multiples of the workGroupSize if
  // necessary
  uint32_t numConcurrentWorkGroups = roundUp(numProcessors, workGroupSize) / workGroupSize;
  uint32_t numPthreads = numConcurrentWorkGroups * workGroupSize;

  WorkItemLoopThreadInfo **threads = new WorkItemLoopThreadInfo *[numPthreads];

  uint32_t workdim = 1;
  uint32_t workGroupSizeV3[] = { workGroupSize, 1, 1 };

  // initialize all the barriers
  for(uint32_t i = 0; i < blockNum; ++i) {
    pthread_barrier_init(barriers + i, &barrierAttr, workGroupSize);
  }

  // The final barrier might not have the full workGroupSize threads on it
  // (when we move to the input being NDRangeSize)
  if (NDRangeSize % workGroupSize != 0) {
    pthread_barrier_init(&barriers[blockNum-1], &barrierAttr, NDRangeSize % workGroupSize);
  }

  // create the workItemLoop pthreads
  for (uint32_t k=0; k<numPthreads; k++) {
    uint32_t workItemAbsId[] = { 0, 0, 0 };  // will be filled in by the workItemLoop
    pthread_barrier_t *barrier = NULL;     // will be filled in by the workItemLoop
    uint32_t absidLow = k;
    uint32_t absidStep = numPthreads;
    uint32_t groupSize = workGroupSize;

    threads[k] = new WorkItemLoopThreadInfo(NDRangeSize, workdim,
                                            workGroupSizeV3, workItemAbsId,
                                            barrier,
                                            args.data(), args.size(),
                                            EntryFunPtr,
                                            absidLow, absidStep,
                                            groupSize, barriers);


    pthread_create(&threads[k]->tid, &attr, &workItemLoop, threads[k]->argsArray);
  }

  // join all the workItemLoop pthreads
  for (uint32_t k=0; k<numPthreads; k++) {
    WorkItemLoopThreadInfo *thrInfo = threads[k];
    void *retVal;
    pthread_join(thrInfo->tid, &retVal);
    delete threads[k];
  }

  // destroy all the barriers
  for(uint32_t i = 0; i < blockNum; ++i) {
    pthread_barrier_destroy(barriers + i);
  }

  pthread_barrierattr_destroy(&barrierAttr);
  pthread_attr_destroy(&attr);

  delete[] barriers;
  delete[] threads;
}

BrigEngine::~BrigEngine() {
  EE_->removeModule(M_);
  delete EE_;
}

} // namespace brig
} // namespace hsa
