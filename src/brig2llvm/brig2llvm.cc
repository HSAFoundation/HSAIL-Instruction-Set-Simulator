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
    llvm::Type::getInt1Ty(*C_), "c_regs",8);
  llvm::StructType *s_reg_type = create_soa_type(
    llvm::Type::getInt32Ty(*C_), "s_regs",16);
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

static bool isFloatTy(BrigDataType type) {
  return
    type == Brigf16   || type == Brigf32   || type == Brigf64   ||
    type == Brigf16x2 || type == Brigf16x4 ||
    type == Brigf32x2 || type == Brigf32x4 ||
    type == Brigf64x2;
}

static bool isVectorTy(BrigDataType type) {
  return type >= Brigu8x4 && type <= Brigf64x2;
}

static bool isSignedTy(BrigDataType type) {
  return
    type == Brigs8    || type == Brigs16   || type == Brigs32   ||
    type == Brigs64   ||
    type == Brigs8x4  || type == Brigs8x8  || type == Brigs8x16 ||
    type == Brigs16x2 || type == Brigs16x4 || type == Brigs16x8 ||
    type == Brigs32x2 || type == Brigs32x4 ||
    type == Brigs64x2;
}

static llvm::Type *getElementTy(llvm::LLVMContext &C, BrigDataType type) {

  assert(isVectorTy(type) && "Cannot get element of non-vector types");

  const bool isFloat = isFloatTy(type);

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

static unsigned getVectorLength(BrigDataType type) {

  assert(isVectorTy(type) && "Cannot get element of non-vector types");

  switch(type) {
  case Brigu16x2: case Brigs16x2: case Brigf16x2:
  case Brigu32x2: case Brigs32x2: case Brigf32x2:
  case Brigu64x2: case Brigs64x2: case Brigf64x2:
    return 2;
  case Brigu8x4:  case Brigs8x4:
  case Brigu16x4: case Brigs16x4: case Brigf16x4:
  case Brigu32x4: case Brigs32x4: case Brigf32x4:
    return 4;
  case Brigu8x8:  case Brigs8x8:  case Brigu16x8:
  case Brigs16x8: case Brigf16x8:
    return 8;
  case Brigu8x16: case Brigs8x16:
    return 16;
  default:
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
  } else if(isVectorTy(type)) {
    return llvm::VectorType::get(getElementTy(C, type), getVectorLength(type));
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

static bool isSimpleBinop(const inst_iterator &inst) {
  BrigOpcode opcode = BrigOpcode(inst->opcode);
  return
    opcode == BrigAdd || opcode == BrigDiv ||
    opcode == BrigMul || opcode == BrigRem ||
    opcode == BrigSub || opcode == BrigAnd ||
    opcode == BrigOr  || opcode == BrigXor;
}

static llvm::Instruction::BinaryOps getBinop(const inst_iterator &inst) {
  BrigOpcode opcode = BrigOpcode(inst->opcode);
  bool isFloat = isFloatTy(BrigDataType(inst->type));
  bool isSigned = isSignedTy(BrigDataType(inst->type));

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

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   llvm::Value *regState) {

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

    return llvm::GetElementPtrInst::Create(regState, arrayRef, "", &B);

  } else {
    assert(false && "Unimplemented operand");
  }
}

static bool isPacked(BrigPacking packing, unsigned opnum) {
  if(opnum == 1) {
    return
      packing == BrigPackPP    || packing == BrigPackPS    ||
      packing == BrigPackP     ||
      packing == BrigPackPPsat || packing == BrigPackPSsat ||
      packing == BrigPackPsat;
  } else if(opnum == 2) {
    return
      packing == BrigPackPP    || packing == BrigPackSP    ||
      packing == BrigPackPPsat || packing == BrigPackSPsat;
  }

  assert(false && "Illegal opnum for arithmetic operation");
}

static bool isBroadcast(BrigPacking packing, unsigned opnum) {
  if(opnum == 1) {
    return
      packing == BrigPackSS    || packing == BrigPackSP    ||
      packing == BrigPackS     ||
      packing == BrigPackSSsat || packing == BrigPackSPsat ||
      packing == BrigPackSsat;
  } else if(opnum == 2) {
    return
      packing == BrigPackSS    || packing == BrigPackPS    ||
      packing == BrigPackSSsat || packing == BrigPackPSsat;
  }

  assert(false && "Illegal opnum for arithmetic operation");
}

static bool isSaturated(BrigPacking packing) {
  return
    packing == BrigPackPPsat || packing == BrigPackPSsat ||
    packing == BrigPackSPsat || packing == BrigPackSSsat ||
    packing == BrigPackPsat  || packing == BrigPackSsat;
}

static llvm::Value *decodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  unsigned opnum,
                                  const inst_iterator inst) {

  BrigPacking packing = BrigPacking(inst->packing);
  bool packed = isPacked(packing, opnum);
  bool broadcast = isBroadcast(packing, opnum);

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
  if(isVectorTy(type)) {
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
                                 llvm::Value *regState) {


  const BrigOperandBase *brigDest = helper.getOperand(inst, 0);
  const BrigOperandBase *brigSrc0 = helper.getOperand(inst, 1);
  const BrigOperandBase *brigSrc1 = helper.getOperand(inst, 2);

  llvm::Value *src0Addr = getOperandAddr(B, brigSrc0, helper, regState);
  llvm::Value *src1Addr = getOperandAddr(B, brigSrc1, helper, regState);

  llvm::Value *src0Raw = new llvm::LoadInst(src0Addr, "", false, &B);
  llvm::Value *src1Raw = new llvm::LoadInst(src1Addr, "", false, &B);

  llvm::Value *src0Val = decodePacking(B, src0Raw, 1, inst);
  llvm::Value *src1Val = decodePacking(B, src1Raw, 2, inst);

  llvm::Instruction::BinaryOps binop = getBinop(inst);
  llvm::Value *resultRaw =
    llvm::BinaryOperator::Create(binop, src0Val, src1Val, "", &B);
  llvm::Value *resultVal = encodePacking(B, resultRaw, inst);

  llvm::Value *destAddr = getOperandAddr(B, brigDest, helper, regState);
  new llvm::StoreInst(resultVal, destAddr, &B);
}

static const char *getBaseName(const inst_iterator inst) {

#define caseOper(X)                             \
  case Brig ## X:                               \
    return #X

  switch(inst->opcode) {
    caseOper(Abs);
    caseOper(Add);
    caseOper(Carry);
    caseOper(Borrow);
    caseOper(Div);
    caseOper(Max);
    caseOper(Min);
    caseOper(Mul);
    caseOper(Neg);
    caseOper(Rem);
    caseOper(Sub);
    caseOper(Mad);
    caseOper(Mul24);
    caseOper(Mad24);
    caseOper(Shl);
    caseOper(Shr);
    caseOper(And);
    caseOper(Or);
    caseOper(Xor);
    caseOper(Not);
    caseOper(Popcount);
    caseOper(Firstbit);
    caseOper(Lastbit);
    caseOper(BitRev);
    caseOper(Extract);
    caseOper(Insert);
    caseOper(Mov);
    caseOper(Unpacklo);
    caseOper(Unpackhi);
    caseOper(Shuffle);
    caseOper(movslo);
    caseOper(movshi);
    caseOper(movdlo);
    caseOper(movdhi);
    caseOper(Lda);
    caseOper(Ldc);
    caseOper(Cmov);
    caseOper(PackedCmp);
    caseOper(CopySign);
    caseOper(Fma);
    caseOper(Class);
    caseOper(Sqrt);
    caseOper(Fract);
    caseOper(Fcos);
    caseOper(Fsin);
    caseOper(Flog2);
    caseOper(Fexp2);
    caseOper(Frsqrt);
    caseOper(Frcp);
    caseOper(Cmp);
    caseOper(Segmentp);
    caseOper(FtoS);
    caseOper(StoF);
    caseOper(Cvt);
    caseOper(F2u4);
    caseOper(Unpack3);
    caseOper(Unpack2);
    caseOper(Unpack1);
    caseOper(Unpack0);
    caseOper(BitAlign);
    caseOper(ByteAlign);
    caseOper(Lerp);
    caseOper(Sad);
    caseOper(Sad2);
    caseOper(Sad4);
    caseOper(Sad4hi);
    caseOper(Ld);
    caseOper(St);
    caseOper(Atomic);
    caseOper(AtomicNoRet);
    caseOper(RdImage);
    caseOper(LdImage);
    caseOper(StImage);
    caseOper(AtomicNoRetImage);
    caseOper(AtomicImage);
    caseOper(QueryWidth);
    caseOper(QueryHeight);
    caseOper(QueryDepth);
    caseOper(QueryOrder);
    caseOper(QueryFiltering);
    caseOper(QueryData);
    caseOper(QueryArray);
    caseOper(QueryNormalized);
    caseOper(Cbr);
    caseOper(Brn);
    caseOper(Sync);
    caseOper(Barrier);
    caseOper(FbarInitSize);
    caseOper(FbarInitSizeWg);
    caseOper(FbarInit);
    caseOper(FbarWait);
    caseOper(FbarArrive);
    caseOper(FbarSkip);
    caseOper(FbarRelease);
    caseOper(FbarReleaseCF);
    caseOper(Count);
    caseOper(Countup);
    caseOper(Mask);
    caseOper(Send);
    caseOper(Receive);
    caseOper(Ret);
    caseOper(Syscall);
    caseOper(Alloca);
    caseOper(Call);
    caseOper(WorkItemId);
    caseOper(WorkItemAId);
    caseOper(WorkGroupId);
    caseOper(WorkGroupSize);
    caseOper(NDRangesize);
    caseOper(NDRangegroups);
    caseOper(LaneId);
    caseOper(DynWaveId);
    caseOper(MaxDynWaveId);
    caseOper(DispatchId);
    caseOper(CU);
    caseOper(WorkDim);
    caseOper(Clock);
    caseOper(workitemidflat);
    caseOper(workitemaidflat);
    caseOper(Nop);
    caseOper(Debugtrap);
  default:
    assert(false && "Unknown instruction");
  }
#undef castOper
}

static const char *getPackingName(const inst_iterator inst) {
#define casePacking(X)                          \
case BrigPack ## X:                             \
    return "_" #X

  switch(inst->packing) {
    casePacking(PP);
    casePacking(PS);
    casePacking(SP);
    casePacking(SS);
    casePacking(S);
    casePacking(P);
    casePacking(PPsat);
    casePacking(PSsat);
    casePacking(SPsat);
    casePacking(SSsat);
    casePacking(Ssat);
    casePacking(Psat);
  case BrigNoPacking:
    return "";
  default:
    assert(false && "Unknown packing");
  }
#undef castPacking
}

static const char *getTypeName(const inst_iterator inst) {
#define caseDataType(X)                          \
  case Brig ## X:                                \
  return #X

  switch(inst->type) {
    caseDataType(s8);
    caseDataType(s16);
    caseDataType(s32);
    caseDataType(s64);
    caseDataType(u8);
    caseDataType(u16);
    caseDataType(u32);
    caseDataType(u64);
    caseDataType(f16);
    caseDataType(f32);
    caseDataType(f64);
    caseDataType(b1);
    caseDataType(b8);
    caseDataType(b16);
    caseDataType(b32);
    caseDataType(b64);
    caseDataType(b128);
    caseDataType(ROImg);
    caseDataType(RWImg);
    caseDataType(Samp);
    caseDataType(u8x4);
    caseDataType(s8x4);
    caseDataType(u8x8);
    caseDataType(s8x8);
    caseDataType(u8x16);
    caseDataType(s8x16);
    caseDataType(u16x2);
    caseDataType(s16x2);
    caseDataType(f16x2);
    caseDataType(u16x4);
    caseDataType(s16x4);
    caseDataType(f16x4);
    caseDataType(u16x8);
    caseDataType(s16x8);
    caseDataType(f16x8);
    caseDataType(u32x2);
    caseDataType(s32x2);
    caseDataType(f32x2);
    caseDataType(u32x4);
    caseDataType(s32x4);
    caseDataType(f32x4);
    caseDataType(u64x2);
    caseDataType(s64x2);
    caseDataType(f64x2);
  default:
    assert(false && "Unknown type");
  }

#undef caseDataType
}

static std::string getName(const inst_iterator inst) {
  const char *base = getBaseName(inst);
  const char *packing = getPackingName(inst);
  const char *type = getTypeName(inst);

  return std::string(base) + packing + "_" + type;
}

static llvm::FunctionType *getInstFunType(const inst_iterator inst,
                                          llvm::LLVMContext &C) {

  llvm::Type *result = runOnType(C, BrigDataType(inst->type));
  std::vector<llvm::Type *> params;
  for(unsigned i = 1; inst->o_operands[i] && i < 5; ++i)
    params.push_back(result);

  return llvm::FunctionType::get(result, params, false);
}

static bool hasDest(const inst_iterator inst) {
  (void) inst;
  return true;
}

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             llvm::Value *regState) {

  unsigned operand = 0;
  const BrigOperandBase *brigDest = NULL;
  if(hasDest(inst))
    brigDest = helper.getOperand(inst, operand++);

  std::vector<llvm::Value *> sources;
  for(; inst->o_operands[operand] && operand < 5; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);

    llvm::Value *srcAddr = getOperandAddr(B, brigSrc, helper, regState);

    llvm::Value *srcRaw = new llvm::LoadInst(srcAddr, "", false, &B);

    llvm::Value *srcVal = decodePacking(B, srcRaw, operand, inst);

    sources.push_back(srcVal);
  }

  llvm::Module *M = B.getParent()->getParent();
  std::string name = getName(inst);
  llvm::FunctionType *funTy = getInstFunType(inst, B.getContext());
  llvm::Function *instFun =
    llvm::cast<llvm::Function>(M->getOrInsertFunction(name, funTy));
  llvm::Value *resultRaw = llvm::CallInst::Create(instFun, sources, "", &B);

  if(brigDest) {
    llvm::Value *resultVal = encodePacking(B, resultRaw, inst);

    llvm::Value *destAddr = getOperandAddr(B, brigDest, helper, regState);
    new llvm::StoreInst(resultVal, destAddr, &B);
  }
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator &inst,
                             const BrigInstHelper &helper,
                             llvm::Value *regState) {
  llvm::LLVMContext &C = B.getContext();

  if(inst->opcode == BrigAbs) {
    runOnComplexInst(B, inst, helper, regState);
  } else if(isSaturated(BrigPacking(inst->packing))) {
    runOnComplexInst(B, inst, helper, regState);
  } else if(isSimpleBinop(inst)) {
    runOnSimpleBinopInst(B, inst, helper, regState);
  } else if(inst->opcode == BrigRet) {
    llvm::ReturnInst::Create(C, &B);
  } else {
    assert(false && "Unimplemented instruction");
  }
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB,
                    llvm::Value **regState) {
  llvm::LLVMContext &C = F.getContext();
  llvm::Twine name(CB.getName());
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(C, name, &F);

  if(F.size() == 1) {
    assert(!*regState && "regState should be uninitialized");
    llvm::Module *M = F.getParent();
    *regState =
      new llvm::AllocaInst(M->getTypeByName("struct.regs"), "gpu_reg_p", bb);
  }

  BrigInstHelper helper = CB.getInstHelper();

  for(inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst, helper, *regState);
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

  llvm::Value *regState = NULL;
  for(BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, &regState);
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
