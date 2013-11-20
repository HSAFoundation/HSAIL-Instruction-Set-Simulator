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

#include "llvm/ADT/Triple.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IRReader/IRReader.h"
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
#include <fenv.h>

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

}

namespace hsa {
namespace brig {

static ForceBrigRuntimeLinkage runtime;

BrigEngine::BrigEngine(hsa::brig::BrigProgram &BP,
                       bool forceInterpreter,
                       char optLevel) : EE_(NULL), M_(BP) {
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
#endif  // __arm__

static std::set<std::string> loadedLibs;

void BrigEngine::init(bool forceInterpreter, char optLevel) {

  Dl_info info;
  char *threnv = getenv("SIMTHREADS");
  if (threnv != NULL && atoi(threnv) > 0) {
    numProcessors = atoi(threnv);
  } else if (sysconf(_SC_NPROCESSORS_CONF) > 0) {
    numProcessors = sysconf(_SC_NPROCESSORS_CONF);
  } else {
    numProcessors = 1;
  }

  int err = dladdr(&runtime, &info);
  assert(err && info.dli_fname &&
         "How are we executing if we haven't even been loaded?!");
  if (loadedLibs.insert(std::string(info.dli_fname)).second)
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

  llvm::SectionMemoryManager *JMM = NULL;
  if (!forceInterpreter) {
    JMM = new llvm::SectionMemoryManager();
    builder.setJITMemoryManager(JMM);
  }

  // If SIMNOOPT is defined, optimization will be disabled to facilitate
  // debugging
  if(getenv("SIMNOOPT")) optLevel = '0';

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
  if (isHardFP()) options.FloatABIType = llvm::FloatABI::Hard;
#endif  // __arm__

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

  // Give MCJIT a chance to apply relocations and set page permissions.
  EE_->finalizeObject();

  // Run static constructors.
  EE_->runStaticConstructorsDestructors(false);

  for (llvm::Module::iterator I = M_->begin(), E = M_->end(); I != E; ++I) {
    llvm::Function *Fn = &*I;
    if (!Fn->isDeclaration())
      EE_->getPointerToFunction(Fn);
  }

  if (JMM)
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

  WorkItemLoopThreadInfo(uint32_t NDRangeSize[3], uint32_t workdim,
                         uint32_t workGroupSize[3], uint32_t workItemAbsId[3],
                         pthread_barrier_t *barrier,
                         void *const *args, size_t size,
                         EntryFunPtrTy EntryFunPtr,
                         uint32_t absidLow, uint32_t absidStep,
                         uint32_t groupSize, pthread_barrier_t *barriers) :
    ThreadInfo(NDRangeSize, workdim, workGroupSize, workItemAbsId, barrier,
               args, size),
    EntryFunPtr(EntryFunPtr), absidLow(absidLow), absidStep(absidStep),
    groupSize(groupSize), barriers(barriers) {
  }
};

static uint32_t roundUpDiv(int val, int multiple) {
  return ((val + multiple - 1) / multiple);
}

// the workItemLoop runs a set of workItems (from different workGroups)
// all in the same pthread.  It assigns the workItems a barrier
// based on the workGroupId. (absid / workGroupSize)

static void *workItemLoop(void *vargs) {
  void **args = (void **) vargs;
  WorkItemLoopThreadInfo *thrInfo = (WorkItemLoopThreadInfo *) (args[0]);
  // compute size of the last group
  uint32_t lastGroupSize = thrInfo->NDRangeSize[0] % thrInfo->groupSize;
  uint32_t lastGroupNum =
    roundUpDiv(thrInfo->NDRangeSize[0], thrInfo->groupSize) - 1;
  if (lastGroupSize == 0) lastGroupSize = thrInfo->groupSize;
  for (uint32_t absid = thrInfo->absidLow;
       absid < thrInfo->NDRangeSize[0];
       absid += thrInfo->absidStep ) {
    thrInfo->workItemAbsId[0] = absid;
    uint32_t workGroupNum = absid / thrInfo->groupSize;
    thrInfo->barrier = &thrInfo->barriers[workGroupNum];
    // insert correct groupsize if we are in last group
    if (workGroupNum == lastGroupNum) {
      thrInfo->workGroupSize[0] = lastGroupSize;
    }

    // Reset the floating point exception flags
    feclearexcept(FE_ALL_EXCEPT);

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

  uint32_t NDRangeSize[3] = { blockNum * workGroupSize, 1, 1};


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
  uint32_t numConcurrentWorkGroups = roundUpDiv(numProcessors, workGroupSize);
  uint32_t numPthreads = numConcurrentWorkGroups * workGroupSize;

  WorkItemLoopThreadInfo **threads = new WorkItemLoopThreadInfo *[numPthreads];

  uint32_t workdim = 1;
  uint32_t workGroupSizeV3[] = { workGroupSize, 1, 1 };

  // initialize all the barriers
  for (uint32_t i = 0; i < blockNum; ++i) {
    pthread_barrier_init(barriers + i, &barrierAttr, workGroupSize);
  }

  // The final barrier might not have the full workGroupSize threads on it
  // (when we move to the input being NDRangeSize)
  if (NDRangeSize[0] % workGroupSize != 0) {
    pthread_barrier_init(&barriers[blockNum - 1], &barrierAttr,
                         NDRangeSize[0] % workGroupSize);
  }

  // create the workItemLoop pthreads
  for (uint32_t k=0; k<numPthreads; k++) {
    // will be filled in by the workItemLoop
    uint32_t workItemAbsId[] = { 0, 0, 0 };
    // will be filled in by the workItemLoop
    pthread_barrier_t *barrier = NULL;
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


    pthread_create(&threads[k]->tid, &attr, &workItemLoop,
                   threads[k]->argsArray);
  }

  // join all the workItemLoop pthreads
  for (uint32_t k=0; k<numPthreads; k++) {
    WorkItemLoopThreadInfo *thrInfo = threads[k];
    void *retVal;
    pthread_join(thrInfo->tid, &retVal);
    delete threads[k];
  }

  // destroy all the barriers
  for (uint32_t i = 0; i < blockNum; ++i) {
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

}  // namespace brig
}  // namespace hsa
