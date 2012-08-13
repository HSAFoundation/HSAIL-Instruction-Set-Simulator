#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_function.h"
#include "brig_symbol.h"
#include "brig_control_block.h"
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

static llvm::Type *runOnType(llvm::LLVMContext &C, BrigDataType type) {
  if(type == Brigb1) {
    return llvm::Type::getInt1Ty(C);
  } else if(type == Brigs8  || type == Brigu8  || type == Brigb8)  {
    return llvm::Type::getInt8Ty(C);
  } else if(type == Brigs16 || type == Brigu16 || type == Brigb16) {
    return llvm::Type::getInt16Ty(C);
  } else if(type == Brigs32 || type == Brigu32 || type == Brigb32) {
    return llvm::Type::getInt32Ty(C);
  } else if(type == Brigs64 || type == Brigu64 || type == Brigb64) {
    return llvm::Type::getInt64Ty(C);
  } else if(type == Brigf32) {
    return llvm::Type::getFloatTy(C);
  } else if(type == Brigf64) {
    return llvm::Type::getDoubleTy(C);
  } else {
    assert(false && "Unimplemented type");
  }
}

static llvm::Type *runOnType(llvm::LLVMContext &C, const BrigSymbol &S) {

  if(S.isArray()) assert(false && "Array types unimplemented");
  if(S.isFlexArray()) assert(false && "Flex array types unimplemented");

  return runOnType(C, S.getType());
}

static llvm::GlobalValue::LinkageTypes runOnLinkage(BrigAttribute link) {
  if(link == BrigExtern || link == BrigNone) {
    return llvm::GlobalValue::ExternalLinkage;
  } else if(link == BrigStatic) {
    return llvm::GlobalValue::InternalLinkage;
  }

  assert(false && "Invalid attribute");
}

static void runOnInstruction(llvm::BasicBlock &B, const inst_iterator inst) {
  llvm::LLVMContext &C = B.getContext();
  if(inst->opcode == BrigRet) {
    llvm::ReturnInst::Create(C, &B);
  } else {
    assert(false && "Unimplemented instruction");
  }
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB) {
  llvm::LLVMContext &C = F.getContext();
  llvm::Twine name(CB.getName());
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(C, name, &F);

  if(F.size() == 1) {
    llvm::Module *M = F.getParent();
    new llvm::AllocaInst(M->getTypeByName("struct.regs"), "gpu_reg_p", bb);
  }

  for(inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst);
  }
}

static void runOnFunction(llvm::Module &M, const BrigFunction &F) {

  llvm::LLVMContext &C = M.getContext();

  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  std::vector<llvm::Type *> argVec;
  for(BrigSymbol arg = F.arg_begin(), E = F.arg_end(); arg != E; ++arg) {
    argVec.push_back(runOnType(C, arg)->getPointerTo(0));
  }
  llvm::ArrayRef<llvm::Type *> args(argVec);
  llvm::FunctionType *funTy = llvm::FunctionType::get(voidTy, args, false);

  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(F.getLinkage());
  llvm::Twine name(F.getName());

  llvm::Function *fun = llvm::Function::Create(funTy, linkage, name, &M);

  BrigSymbol brigArg = F.arg_begin();
  llvm::Function::arg_iterator llvmArg = fun->arg_begin();
  llvm::Function::arg_iterator E = fun->arg_end();
  for(; llvmArg != E; ++brigArg, ++llvmArg) {
    llvmArg->setName(brigArg.getName());
  }

  if(F.isDeclaration()) return;

  for(BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb);
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

  BrigModule mod(strings_, directives_, code_, operands_, &std::cerr);
  assert(mod.isValid());

  gen_GPU_states();

  for(BrigFunction fun = mod.begin(), E = mod.end(); fun != E; ++fun) {
    runOnFunction(*brig_frontend_, fun);
  }

  llvm::raw_string_ostream ros(output_);
  brig_frontend_->print(ros, NULL);
  brig_frontend_->dump();

  llvm::verifyModule(*brig_frontend_);
}

}
}
