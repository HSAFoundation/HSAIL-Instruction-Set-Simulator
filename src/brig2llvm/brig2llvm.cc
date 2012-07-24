#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
namespace hsa{
namespace brig{
GenLLVM::GenLLVM(const Buffer &directives):directives_(directives),
  brig_frontend_(NULL) {
}
void GenLLVM::operator()(void) {
  llvm::LLVMContext &context = llvm::getGlobalContext();
}

}
}
