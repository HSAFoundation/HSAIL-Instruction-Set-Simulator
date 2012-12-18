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
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
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
                       char optLevel) : EE_(NULL), M_(BP.M.get()) {
  init(forceInterpreter, optLevel);
}

BrigEngine::BrigEngine(llvm::Module *Mod,
                       bool forceInterpreter,
                       char optLevel) : EE_(NULL), M_(Mod) {
  init(forceInterpreter, optLevel);
}

void BrigEngine::init(bool forceInterpreter, char optLevel) {

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
  if(!forceInterpreter) {
    JMM = new llvm::SectionMemoryManager();
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

  if(JMM)
    JMM->invalidateInstructionCache();
}

void BrigEngine::launch(llvm::Function *EntryFn,
                        llvm::ArrayRef<void *> args,
                        uint32_t blockNum,
                        uint32_t threadNum) {

  assert(blockNum && threadNum && "Thread count too low");

  typedef void *(*EntryFunPtrTy)(void*);
  EntryFunPtrTy EntryFunPtr =
    (EntryFunPtrTy)(intptr_t) EE_->getPointerToFunction(EntryFn);

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  ThreadInfo **threads = new ThreadInfo *[blockNum * threadNum];

  uint32_t NDRangeSize = blockNum * threadNum;
  uint32_t workdim = 1;
  uint32_t workGroupSize[] = { threadNum, 1, 1 };

  for(uint32_t i = 0; i < blockNum; ++i) {
    for(uint32_t j = 0; j < threadNum; ++j) {

      uint32_t tid = i * threadNum + j;
      uint32_t workItemAbsId[] = { tid, 0, 0 };
      threads[tid] = new ThreadInfo(NDRangeSize, workdim,
                                    workGroupSize, workItemAbsId,
                                    args.data(), args.size());
      pthread_create(&threads[tid]->tid, &attr, EntryFunPtr,
                     threads[tid]->argsArray);
    }
  }

  for(uint32_t i = 0; i < blockNum * threadNum; ++i) {
    void *retVal;
    pthread_join(threads[i]->tid, &retVal);
    delete threads[i];

  }

  pthread_attr_destroy(&attr);

  delete[] threads;
}

BrigEngine::~BrigEngine() {
  EE_->removeModule(M_);
  delete EE_;
}

} // namespace brig
} // namespace hsa
