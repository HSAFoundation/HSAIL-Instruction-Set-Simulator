#ifndef BRIG_ENGINE_H
#define BRIG_ENGINE_H

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
  BrigEngine(llvm::Module *Mod,
             bool forceInterpreter = false,
             char optLevel = ' ');

  void launch(llvm::Function *EntryFn,
              llvm::ArrayRef<void *> args);

 private:
  llvm::ExecutionEngine *EE_;
};

} // namespace brig
} // namespace hsa

#endif // BRIG_ENGINE_H
