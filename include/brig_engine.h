#ifndef BRIG_ENGINE_H
#define BRIG_ENGINE_H

#include "llvm/ADT/ArrayRef.h"

#include <stdbool.h>

namespace llvm {
class Module;
class Function;
}

namespace hsa {
namespace brig {

void launchBrig(llvm::Module *Mod,
                llvm::Function *EntryFn,
                llvm::ArrayRef<void *> args,
                bool forceInterpreter = false,
                char optLevel = ' ');

} // namespace brig
} // namespace hsa

#endif // BRIG_ENGINE_H
