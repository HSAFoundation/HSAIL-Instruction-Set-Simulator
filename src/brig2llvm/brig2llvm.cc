#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include <iostream>
namespace hsa{
namespace brig{

llvm::StructType *GenLLVM::create_soa_type(
  llvm::Type *t, std::string name, int nr) {
  // [nr x t]
  llvm::ArrayType *tx8 = llvm::ArrayType::get(t, nr);
  std::vector<llvm::Type *> tv(1, tx8);
  // name = {[nr x t]}
  llvm::StructType *soa_type = llvm::StructType::create(
    *C_, tv, name, false);
  return soa_type;
}

void GenLLVM::gen_GPU_states(void) {
  llvm::StructType *c_reg_type = create_soa_type(
    llvm::Type::getInt1Ty(*C_), "c_regs",8);
  llvm::StructType *s_reg_type = create_soa_type(
    llvm::Type::getInt32Ty(*C_), "s_regs",8);
  llvm::StructType *d_reg_type = create_soa_type(
    llvm::Type::getInt64Ty(*C_), "d_regs",8);
  llvm::StructType *q_reg_type = create_soa_type(
    llvm::Type::getIntNTy(*C_, 128), "q_regs",8);
  llvm::StructType *pc_reg_type = create_soa_type(
    llvm::Type::getIntNTy(*C_, 32), "pc_regs",3);
  std::vector<llvm::Type *> tv1;
  tv1.push_back(c_reg_type);
  tv1.push_back(s_reg_type);
  tv1.push_back(d_reg_type);
  tv1.push_back(q_reg_type);
  tv1.push_back(pc_reg_type);
  llvm::StructType *gpu_states_ty = llvm::StructType::create(
    *C_, tv1, std::string("struct.regs"), false);
  gpu_states_type_ = gpu_states_ty;
}

size_t GenLLVM::gen_function(
  size_t index,
  const struct BrigDirectiveFunction *directive) {
  std::vector<llvm::Type *> args;
  std::vector<std::string> argnames;

  assert(directive->outParamCount < 2 && "Multiple out params not supported");
  if(directive->outParamCount == 1) {
    const struct BrigDirectiveSymbol *out_arg =
      reinterpret_cast<const struct BrigDirectiveSymbol*>
      (&(directives_.get()[directive->d_firstScopedDirective]));
    std::string symbol_name = strings_.at(out_arg->s.s_name+1);

    if(out_arg->s.type == Brigf32) {
      args.push_back(llvm::Type::getFloatPtrTy(*C_));
    } else if(out_arg->s.type == Brigu32) {
      args.push_back(llvm::Type::getInt32PtrTy(*C_));
    }

    argnames.push_back(symbol_name);
  }

  for(unsigned i = 0; i < directive->inParamCount; ++i) {
    const struct BrigDirectiveSymbol *in_arg =
      reinterpret_cast<const struct BrigDirectiveSymbol*>
      (&(directives_.get()[directive->d_firstInParam]));
    std::string symbol_name = strings_.at(in_arg->s.s_name+1);

    if(in_arg->s.type == Brigf32) {
      args.push_back(llvm::Type::getFloatPtrTy(*C_));
    } else if(in_arg->s.type == Brigu32) {
      args.push_back(llvm::Type::getInt32PtrTy(*C_));
    }

    argnames.push_back(symbol_name);
  }

  llvm::FunctionType *FT =
    llvm::FunctionType::get(llvm::Type::getVoidTy(
      *C_), args, false);
  llvm::Function *func = llvm::Function::Create(FT,
    llvm::Function::ExternalLinkage,
    strings_.at(directive->s_name+1), brig_frontend_);

  // Set names for output arguments.
  unsigned idx = 0;
  for (llvm::Function::arg_iterator ai = func->arg_begin(); idx !=
      directive->outParamCount + directive->inParamCount; ++ai, ++idx) {
    ai->setName(argnames[idx]);
  }

  if (directive->operationCount) {
    // Create a new basic block to start insertion into.
    llvm::BasicBlock *bb =
      llvm::BasicBlock::Create(*C_, "entry",
        func);
    // Create alloca of local GPU state
    llvm::IRBuilder<> TmpB(bb, bb->begin());
    TmpB.CreateAlloca(gpu_states_type_, 0, "gpu_reg_p");

    uint32_t offset = directive->c_code;
    for(uint32_t i = 0; i < directive->operationCount; ++i) {
      const BrigInstBase *inst =
        reinterpret_cast<const BrigInstBase *>(&code_.get()[offset]);

      if(inst->kind == BrigEInstBase) {
        if(inst->opcode == BrigRet) {
          TmpB.CreateRetVoid();
        } else {
          assert(false && "Unimplemented base opcode");
        }
      } else {
        assert(false && "Unimplemented instruction kind");
      }

      offset += inst->size;
    }
  }

  return directive->d_nextDirective;
}

size_t GenLLVM::gen_directive(size_t index) {
  const BrigDirectiveBase *dh =
    reinterpret_cast<const BrigDirectiveBase *>(&(directives_.get()[index]));
  //std::cout << "Size = "<<dh->size<<"; kind = "<<dh->kind <<std::endl;
  enum BrigDirectiveKinds directive_kind =
    static_cast<enum BrigDirectiveKinds>(dh->kind);
  switch(directive_kind) {
    case BrigEDirectiveFunction:
      return gen_function(index,
        reinterpret_cast<const struct BrigDirectiveFunction*>(dh));
    case BrigEDirectiveVersion:
      return index + dh->size;
      //std::cout << "A version\n";
    default:
      assert(0&&"Unknown directive type");
  }
}

GenLLVM::GenLLVM(const StringBuffer &strings,
                 const Buffer &directives,
                 const Buffer &code,
                 const Buffer &operands) :
  strings_(strings), directives_(directives),
  code_(code), operands_(operands),
   brig_frontend_(NULL) {
}
void GenLLVM::operator()(void) {
  C_ = new llvm::LLVMContext();
  brig_frontend_ = new llvm::Module("BRIG", *C_);
  gen_GPU_states();
  size_t index = 0;
  while(index < directives_.size()) {
    index = gen_directive(index);
  }
  llvm::raw_string_ostream ros(output_);
  brig_frontend_->print(ros, NULL);
  brig_frontend_->dump();

  llvm::verifyModule(*brig_frontend_);
}

}
}
