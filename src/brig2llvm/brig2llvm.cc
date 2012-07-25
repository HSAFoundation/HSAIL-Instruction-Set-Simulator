#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig.h"
#include <iostream>
namespace hsa{
namespace brig{
struct directive_header {
  uint16_t size;
  uint16_t kind;
};
size_t GenLLVM::gen_function(size_t index,
  const struct BrigDirectiveFunction *directive) {
  assert(directive->inParamCount == 0);
  std::vector<llvm::Type *> args;
  std::vector<std::string> argnames;
  if (directive->outParamCount) {
    const struct BrigDirectiveSymbol *out_arg =
      reinterpret_cast<const struct BrigDirectiveSymbol*>(
        &(directives_.get()[directive->d_firstScopedDirective]));
    assert(out_arg->s.type == Brigf32);
    std::string symbol_name = strings_.at(out_arg->s.s_name+1);

    //std::cout << "Symbol name =  "<< symbol_name<<std::endl;

    // Make the function type:  double(double,double) etc.
    args.push_back(
      llvm::Type::getFloatPtrTy(llvm::getGlobalContext()));
    argnames.push_back(symbol_name);
    index += sizeof(*out_arg);
  } else {
    assert(0);
  }
  llvm::FunctionType *FT =
    llvm::FunctionType::get(llvm::Type::getVoidTy(
      llvm::getGlobalContext()), args, false);
  llvm::Function *func = llvm::Function::Create(FT,
    llvm::Function::ExternalLinkage,
    strings_.at(directive->s_name+1), brig_frontend_);
  
  // Set names for output arguments.
  unsigned idx = 0;
  for (llvm::Function::arg_iterator ai = func->arg_begin(); idx !=
      directive->outParamCount; ++ai, ++idx) {
    ai->setName(argnames[idx]);
  }
  if (directive->operationCount) {
    // Create a new basic block to start insertion into.
    llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry",
        func);
  }
//  std::cout << "A function\n";
  return index;
}

size_t GenLLVM::gen_directive(size_t index) {
  const directive_header *dh = 
    reinterpret_cast<const directive_header *>(&(directives_.get()[index]));
  //std::cout << "Size = "<<dh->size<<"; kind = "<<dh->kind <<std::endl;
  enum BrigDirectiveKinds directive_kind =
    static_cast<enum BrigDirectiveKinds>(dh->kind);
  switch(directive_kind) {
    case BrigEDirectiveFunction:
      index = gen_function(index,
        reinterpret_cast<const struct BrigDirectiveFunction*>(dh));
      break;
    case BrigEDirectiveVersion:
      //std::cout << "A version\n";
      break;
    default:
      assert(0&&"Unknown directive type");
  };
  return index+dh->size;
}

GenLLVM::GenLLVM(const Buffer &directives, const StringBuffer &strings):
  directives_(directives), strings_(strings),
   brig_frontend_(NULL) {
}
void GenLLVM::operator()(void) {
   llvm::LLVMContext &context = llvm::getGlobalContext();
  brig_frontend_ = new llvm::Module("BRIG", context);
  size_t index = 0;
  while(index < directives_.size()) {
    index = gen_directive(index);
  }
  llvm::raw_string_ostream ros(output_);
  brig_frontend_->print(ros, NULL);
  //brig_frontend_->dump();
}

}
}
