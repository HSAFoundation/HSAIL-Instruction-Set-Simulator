#ifndef BRIG_ENGINE_H
#define BRIG_ENGINE_H

#include <stdbool.h>

namespace llvm {
class Module;
class Function;
}

namespace hsa {
namespace brig {

void launchBrig(llvm::Module *Mod,
                llvm::Function *EntryFn,
                bool forceInterpreter = true,
                char optLevel = ' ');

} // namespace brig
} // namespace hsa

#endif // BRIG_ENGINE_H
