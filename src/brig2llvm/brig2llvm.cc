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
#include "brig_inst_helper.h"
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
    llvm::Type::getInt1Ty(*C_), "c_regs", 8);
  llvm::StructType *s_reg_type = create_soa_type(
    llvm::Type::getInt32Ty(*C_), "s_regs", 32);
  llvm::StructType *d_reg_type = create_soa_type(
    llvm::Type::getInt64Ty(*C_), "d_regs", 32);
  llvm::StructType *q_reg_type = create_soa_type(
    llvm::Type::getIntNTy(*C_, 128), "q_regs", 8);
  llvm::StructType *pc_reg_type = create_soa_type(
    llvm::Type::getIntNTy(*C_, 32), "pc_regs", 3);
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

static llvm::Type *getElementTy(llvm::LLVMContext &C, BrigDataType type) {

  assert(BrigInstHelper::isVectorTy(type) &&
         "Cannot get element of non-vector types");

  const bool isFloat = BrigInstHelper::isFloatTy(type);

  if(type >= Brigu8x4 && type <= Brigs8x16) {
    return llvm::Type::getInt8Ty(C);
  } else if(type >= Brigu16x2 && type <= Brigf16x8) {
    if(isFloat) return llvm::Type::getHalfTy(C);
    else return llvm::Type::getInt16Ty(C);
  } else if(type >= Brigu32x2 && type <= Brigf32x4) {
    if(isFloat) return llvm::Type::getFloatTy(C);
    else return llvm::Type::getInt32Ty(C);
  } else if(type >= Brigu64x2 && type <= Brigf64x2) {
    if(isFloat) return llvm::Type::getDoubleTy(C);
    else return llvm::Type::getInt64Ty(C);
  } else {
    assert(false && "Unknown type");
  }
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
  } else if(type == Brigb128) {
    return llvm::Type::getIntNTy(C, 128);
  } else if(BrigInstHelper::isVectorTy(type)) {
    unsigned length = BrigInstHelper::getVectorLength(type);
    return llvm::VectorType::get(getElementTy(C, type), length);
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

static bool isSimpleBinop(const inst_iterator inst) {
  BrigOpcode opcode = BrigOpcode(inst->opcode);
  return
    opcode == BrigAdd || opcode == BrigDiv ||
    opcode == BrigMul || opcode == BrigRem ||
    opcode == BrigSub || opcode == BrigAnd ||
    opcode == BrigOr  || opcode == BrigXor;
}

static llvm::Instruction::BinaryOps getBinop(const inst_iterator inst) {
  BrigOpcode opcode = BrigOpcode(inst->opcode);
  bool isFloat = BrigInstHelper::isFloatTy(BrigDataType(inst->type));
  bool isSigned = BrigInstHelper::isSignedTy(BrigDataType(inst->type));

  if(opcode == BrigAdd) {
    if(isFloat) return llvm::Instruction::FAdd;
    else return llvm::Instruction::Add;

  } else if(opcode == BrigDiv) {
    if(isFloat) return llvm::Instruction::FDiv;
    else if(isSigned) return llvm::Instruction::SDiv;
    else return llvm::Instruction::UDiv;

  } else if(opcode == BrigMul) {
    if(isFloat) return llvm::Instruction::FMul;
    else return llvm::Instruction::Mul;

  } else if(opcode == BrigRem) {
    if(isFloat) return llvm::Instruction::FRem;
    else if(isSigned) return llvm::Instruction::SRem;
    else return llvm::Instruction::URem;

  } else if(opcode == BrigSub) {
    if(isFloat) return llvm::Instruction::FSub;
    else return llvm::Instruction::Sub;

  } else if(opcode == BrigAnd) {
    return llvm::Instruction::And;

  } else if(opcode == BrigOr) {
    return llvm::Instruction::Or;

  } else if(opcode == BrigXor) {
    return llvm::Instruction::Xor;
  }

  assert(false && "Unknown binop");
}

static unsigned getRegField(const char *name) {
  if(name[1] == 'c') return 0;
  if(name[1] == 's') return 1;
  if(name[1] == 'd') return 2;
  if(name[1] == 'q') return 3;

  assert(false && "Unknown reg type");
}

static unsigned getRegOffset(const char *name) {
  char *endptr;
  unsigned offset = (unsigned) strtol(name + 2, &endptr, 10);
  assert(!*endptr && "Invalid field");
  assert(offset < 16 && "Invalid offset");
  return offset;
}

typedef std::map<uint32_t, llvm::Function *> FunMap;

struct FunState {
  typedef std::map<uint32_t, llvm::BasicBlock *> CBMap ;
  typedef CBMap::iterator CBIt;

  const FunMap &funMap;
  llvm::Value *regs;
  CBMap cbMap;

  FunState(const FunMap &funMap, const BrigFunction &brigFun,
           llvm::Function *llvmFun) : funMap(funMap) {

    llvm::LLVMContext &C = llvmFun->getContext();
    const BrigControlBlock E = brigFun.end();
    for(BrigControlBlock cb = brigFun.begin(); cb != E; ++cb) {

      const char *name = cb.getName();
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(C, name, llvmFun);

      typedef std::pair<uint32_t, llvm::BasicBlock *> Pair;
      bool unique = cbMap.insert(Pair(cb.getOffset(), bb)).second;
      assert(unique && "Labels should be unique in functions");

      if(cbMap.size() == 1) {
        llvm::Module *M = llvmFun->getParent();
        llvm::Type *regsType = M->getTypeByName("struct.regs");
        regs = new llvm::AllocaInst(regsType, "gpu_reg_p", bb);
      }
    }
  }
};

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   FunState &state) {

  llvm::LLVMContext &C = B.getContext();

  if(const BrigOperandReg *regOp = dyn_cast<BrigOperandReg>(op)) {

    const char *name = helper.getName(regOp);
    unsigned field = getRegField(name);
    unsigned offset = getRegOffset(name);

    llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
    llvm::Value *array[] = { llvm::ConstantInt::get(int32Ty, 0),
                             llvm::ConstantInt::get(int32Ty, field),
                             llvm::ConstantInt::get(int32Ty, 0),
                             llvm::ConstantInt::get(int32Ty, offset) };
    llvm::ArrayRef<llvm::Value *> arrayRef(array, 4);

    return llvm::GetElementPtrInst::Create(state.regs, arrayRef, "", &B);

  } else {
    assert(false && "Unimplemented operand");
  }
}

static bool hasAddr(const BrigOperandBase *op) {
  BrigOperandKinds kind = BrigOperandKinds(op->kind);
  switch(kind) {
  case BrigEOperandImmed:
  case BrigEOperandLabelRef:
  case BrigEOperandFunctionRef:
    return false;
  case BrigEOperandAddress:
  case BrigEOperandReg:
    return true;
  default:
    assert(false && "Unimplemented");
  }
}

static llvm::Value *getOperand(llvm::BasicBlock &B,
                               const BrigOperandBase *op,
                               const BrigInstHelper &helper,
                               FunState &state) {

  if(hasAddr(op)) {
    llvm::Value *valAddr = getOperandAddr(B, op, helper, state);
    llvm::Value *val = new llvm::LoadInst(valAddr, "", false, &B);
    return val;
  }

  if(const BrigOperandLabelRef *label = dyn_cast<BrigOperandLabelRef>(op)) {
    return state.cbMap[label->labeldirective];
  }

  const BrigOperandFunctionRef *func = dyn_cast<BrigOperandFunctionRef>(op);
  if(func) {
    return state.funMap.find(func->fn)->second;
  }

  assert(false && "Unimplemented");
}

static llvm::Value *decodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  unsigned opnum,
                                  const inst_iterator inst) {

  BrigPacking packing = BrigPacking(inst->packing);
  bool packed = BrigInstHelper::isPacked(packing, opnum);
  bool broadcast = BrigInstHelper::isBroadcast(packing, opnum);

  assert((!packed || !broadcast) && "Illegal packing combination!?");

  llvm::LLVMContext &C = B.getContext();

  if(packed || broadcast) {
    llvm::Type *type = runOnType(C, BrigDataType(inst->type));
    llvm::Value *bitcast = new llvm::BitCastInst(value, type, "", &B);
    if(packed) return bitcast;

    assert(false && "Unimplemented");
  }

  return value;
}

static llvm::Value *encodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  const inst_iterator inst) {

  BrigDataType type = BrigDataType(inst->type);
  if(BrigInstHelper::isVectorTy(type)) {
    llvm::LLVMContext &C = B.getContext();
    llvm::Type *encodedType = runOnType(C, type);
    unsigned bitsize =  encodedType->getPrimitiveSizeInBits();
    llvm::Type *type = llvm::Type::getIntNTy(C, bitsize);
    llvm::Value *bitcast = new llvm::BitCastInst(value, type, "", &B);
    return bitcast;
  }

  return value;
}

static void runOnSimpleBinopInst(llvm::BasicBlock &B,
                                 const inst_iterator inst,
                                 const BrigInstHelper &helper,
                                 FunState &state) {


  const BrigOperandBase *brigDest = helper.getOperand(inst, 0);
  const BrigOperandBase *brigSrc0 = helper.getOperand(inst, 1);
  const BrigOperandBase *brigSrc1 = helper.getOperand(inst, 2);

  llvm::Value *src0Raw = getOperand(B, brigSrc0, helper, state);
  llvm::Value *src1Raw = getOperand(B, brigSrc1, helper, state);

  llvm::Value *src0Val = decodePacking(B, src0Raw, 1, inst);
  llvm::Value *src1Val = decodePacking(B, src1Raw, 2, inst);

  llvm::Instruction::BinaryOps binop = getBinop(inst);
  llvm::Value *resultRaw =
    llvm::BinaryOperator::Create(binop, src0Val, src1Val, "", &B);
  llvm::Value *resultVal = encodePacking(B, resultRaw, inst);

  llvm::Value *destAddr = getOperandAddr(B, brigDest, helper, state);
  new llvm::StoreInst(resultVal, destAddr, &B);
}

static llvm::FunctionType *getInstFunType(const inst_iterator inst,
                                          llvm::LLVMContext &C) {

  llvm::Type *result = runOnType(C, BrigDataType(inst->type));
  std::vector<llvm::Type *> params;
  for(unsigned i = 1; i < 5 && inst->o_operands[i]; ++i)
    params.push_back(result);

  return llvm::FunctionType::get(result, params, false);
}

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             FunState &state) {

  unsigned operand = 0;
  const BrigOperandBase *brigDest = NULL;
  if(BrigInstHelper::hasDest(inst))
    brigDest = helper.getOperand(inst, operand++);

  std::vector<llvm::Value *> sources;
  for(; operand < 5 && inst->o_operands[operand]; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);
    llvm::Value *srcRaw = getOperand(B, brigSrc, helper, state);
    llvm::Value *srcVal = decodePacking(B, srcRaw, operand, inst);
    sources.push_back(srcVal);
  }

  llvm::Module *M = B.getParent()->getParent();
  std::string name = BrigInstHelper::getInstName(inst);
  llvm::FunctionType *funTy = getInstFunType(inst, B.getContext());
  llvm::Function *instFun =
    llvm::cast<llvm::Function>(M->getOrInsertFunction(name, funTy));
  llvm::Value *resultRaw = llvm::CallInst::Create(instFun, sources, "", &B);

  if(brigDest) {
    llvm::Value *resultVal = encodePacking(B, resultRaw, inst);

    llvm::Value *destAddr = getOperandAddr(B, brigDest, helper, state);
    new llvm::StoreInst(resultVal, destAddr, &B);
  }
}

static bool isBranchInst(const inst_iterator inst) {
  BrigOpcode opcode = BrigOpcode(inst->opcode);
  return opcode == BrigBrn || opcode == BrigCbr;
}

static void runOnBranchInst(llvm::BasicBlock &B,
                            const inst_iterator inst,
                            const BrigInstHelper &helper,
                            FunState &state) {

  // The width of the branch is not necessary for a functional simulator.
  // Similarly, we can ignore the list of possible branch targets for an
  // indirect branch. In debug mode, we should check for and log branchs outside
  // the target width and branch divergence beyond the limit given by width.

  unsigned targetOpNum = inst->opcode == BrigBrn ? 1 : 2;

  const BrigOperandBase *target = helper.getOperand(inst, targetOpNum);
  llvm::BasicBlock *targetBB =
    llvm::cast<llvm::BasicBlock>(getOperand(B, target, helper, state));

  if(inst->opcode == BrigCbr) {
    const BrigOperandBase *pred = helper.getOperand(inst, 1);
    llvm::Value *predVal = getOperand(B, pred, helper, state);
    llvm::BranchInst::Create(targetBB, NULL, predVal, &B);
    return;
  }

  if(inst->opcode == BrigBrn) {
    llvm::BranchInst::Create(targetBB, &B);
    return;
  }

  assert(false && "Unknown branch opcode");
}

static llvm::Value *getSymbolAddr(const BrigSymbol &symbol) {
  assert(false && "Unimplemented");
}

static void runOnCallInst(llvm::BasicBlock &B,
                          const inst_iterator inst,
                          const BrigInstHelper &helper,
                          FunState &state) {

  std::vector<llvm::Value *> args;

  const BrigOperandArgumentList *oArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 1));
  for(unsigned i = 0; i < oArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(oArgList, i);
    args.push_back(getSymbolAddr(symbol));
  }

  const BrigOperandArgumentList *iArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 3));
  for(unsigned i = 0; i < iArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(iArgList, i);
    args.push_back(getSymbolAddr(symbol));
  }

  const BrigOperandBase *brigFun = helper.getOperand(inst, 2);
  llvm::Value *func = getOperand(B, brigFun, helper, state);
  llvm::CallInst::Create(func, args, "", &B);
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             FunState &state) {
  llvm::LLVMContext &C = B.getContext();

  if(inst->opcode == BrigAbs ||
     inst->opcode == BrigMax ||
     inst->opcode == BrigMin) {
    runOnComplexInst(B, inst, helper, state);
  } else if(BrigInstHelper::isSaturated(BrigPacking(inst->packing))) {
    runOnComplexInst(B, inst, helper, state);
  } else if(isSimpleBinop(inst)) {
    runOnSimpleBinopInst(B, inst, helper, state);
  } else if(inst->opcode == BrigRet) {
    llvm::ReturnInst::Create(C, &B);
  } else if(isBranchInst(inst)) {
    runOnBranchInst(B, inst, helper, state);
  } else if(inst->opcode == BrigCall) {
    runOnCallInst(B, inst, helper, state);
  } else {
    assert(false && "Unimplemented instruction");
  }
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB,
                    FunState &state) {

  llvm::LLVMContext &C = F.getContext();
  BrigInstHelper helper = CB.getInstHelper();
  FunState::CBIt it = state.cbMap.find(CB.getOffset());
  assert(it != state.cbMap.end() && "Missing CB");
  llvm::BasicBlock *bb = it->second;

  for(inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst, helper, state);

    // Create a new basic block after every conditional branch to handle the
    // fall-through path
    llvm::BranchInst *branch = llvm::dyn_cast<llvm::BranchInst>(&bb->back());
    if(branch && branch->isConditional()) {
      bb = llvm::BasicBlock::Create(C, bb->getName() + ".succ", &F);
      branch->setSuccessor(1, bb);
    }
  }

  // Fall through to the next control block
  if(!llvm::isa<llvm::TerminatorInst>(&bb->back())) {
    ++it;
    if(it != state.cbMap.end()) llvm::BranchInst::Create(it->second, bb);
    else llvm::ReturnInst::Create(C, bb);
  }
}

static void runOnFunction(llvm::Module &M, const BrigFunction &F,
                          FunMap &funMap) {

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
  funMap[F.getOffset()] = fun;

  BrigSymbol brigArg = F.arg_begin();
  llvm::Function::arg_iterator llvmArg = fun->arg_begin();
  llvm::Function::arg_iterator E = fun->arg_end();
  for(; llvmArg != E; ++brigArg, ++llvmArg) {
    llvmArg->setName(brigArg.getName());
  }

  if(F.isDeclaration()) return;

  FunState state(funMap, F, fun);
  for(BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, state);
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

  BrigModule mod(strings_, directives_, code_, operands_, &llvm::errs());
  assert(mod.isValid());

  gen_GPU_states();

  FunMap funMap;
  for(BrigFunction fun = mod.begin(), E = mod.end(); fun != E; ++fun) {
    runOnFunction(*brig_frontend_, fun, funMap);
  }

  llvm::raw_string_ostream ros(output_);
  brig_frontend_->print(ros, NULL);
  brig_frontend_->dump();

  llvm::verifyModule(*brig_frontend_);
}

}
}
