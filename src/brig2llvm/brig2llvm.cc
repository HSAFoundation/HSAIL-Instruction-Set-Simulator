#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig.h"
#include <iostream>
namespace hsa{
namespace brig{
GenLLVM::GenLLVM(const Buffer &directives, const StringBuffer &strings):
  directives_(directives), strings_(strings),
  brig_frontend_(NULL) {
}
void GenLLVM::operator()(void) {
  llvm::LLVMContext &context = llvm::getGlobalContext();
}

}
}
