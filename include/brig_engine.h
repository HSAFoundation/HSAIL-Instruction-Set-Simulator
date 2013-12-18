//===- brig_engine.h ------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BRIG_ENGINE_H
#define BRIG_ENGINE_H

#include "brig_llvm.h"

#include "llvm/ADT/ArrayRef.h"

namespace llvm {
class Module;
class Function;
class ExecutionEngine;
}

namespace hsa {
namespace brig {

class BrigEngine {

 public:
  BrigEngine(BrigProgram &BP,
             bool forceInterpreter = false,
             char optLevel = ' ');

  BrigEngine(llvm::Module *Mod,
             bool forceInterpreter = false,
             char optLevel = ' ');

  void launch(llvm::Function *EntryFn,
              llvm::ArrayRef<void *> args,
              uint32_t blockNum = 1,
              uint32_t threadNum = 1,
              void *queue = NULL, uint64_t queueId = 0,
              void *dispatch = NULL, uint64_t dispatchId = 0);


  ~BrigEngine();

 private:
  llvm::ExecutionEngine *EE_;
  llvm::Module *M_;
  uint32_t numProcessors;

  void init(bool forceInterpreter = false,
            char optLevel = ' ');
};

} // namespace brig
} // namespace hsa

#endif // BRIG_ENGINE_H
