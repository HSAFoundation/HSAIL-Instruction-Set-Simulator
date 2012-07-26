#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IRBuilder.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include <iostream>
namespace hsa{
namespace brig{
struct directive_header {
  uint16_t size;
  uint16_t kind;
};
static llvm::StructType *create_soa_type(
  llvm::Type *t, std::string name, int nr) {
  // [nr x t]
  llvm::ArrayType *tx8 = llvm::ArrayType::get(t, nr);
  std::vector<llvm::Type *> tv(1, tx8);
  // name = {[nr x t]}
  llvm::StructType *soa_type = llvm::StructType::create(
    llvm::getGlobalContext(), tv, name, false);
  return soa_type;
}

void GenLLVM::gen_GPU_states(void) {
  llvm::StructType *c_reg_type = create_soa_type(
    llvm::Type::getInt1Ty(llvm::getGlobalContext()), "c_regs",8);
  llvm::StructType *s_reg_type = create_soa_type(
    llvm::Type::getInt32Ty(llvm::getGlobalContext()), "s_regs",8);
  llvm::StructType *d_reg_type = create_soa_type(
    llvm::Type::getInt64Ty(llvm::getGlobalContext()), "d_regs",8);
  llvm::StructType *q_reg_type = create_soa_type(
    llvm::Type::getIntNTy(llvm::getGlobalContext(), 128), "q_regs",8);
  std::vector<llvm::Type *> tv1;
  tv1.push_back(c_reg_type);
  tv1.push_back(s_reg_type);
  tv1.push_back(d_reg_type);
  tv1.push_back(q_reg_type);
  llvm::StructType *gpu_states_ty = llvm::StructType::create(
    llvm::getGlobalContext(), tv1, std::string("struct.regs"), false);
  gpu_states_type_ = gpu_states_ty;
}

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
    // Create alloca of local GPU state
    llvm::IRBuilder<> TmpB(bb, bb->begin());
    TmpB.CreateAlloca(gpu_states_type_, 0, "gpu_reg_p");
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
  gen_GPU_states();
  size_t index = 0;
  while(index < directives_.size()) {
    index = gen_directive(index);
  }
  llvm::raw_string_ostream ros(output_);
  brig_frontend_->print(ros, NULL);
  brig_frontend_->dump();
}

}
}
