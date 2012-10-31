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
              llvm::ArrayRef<void *> args);


  ~BrigEngine();

 private:
  llvm::ExecutionEngine *EE_;
  llvm::Module *M_;

  void init(bool forceInterpreter = false,
            char optLevel = ' ');
};

} // namespace brig
} // namespace hsa

#endif // BRIG_ENGINE_H
