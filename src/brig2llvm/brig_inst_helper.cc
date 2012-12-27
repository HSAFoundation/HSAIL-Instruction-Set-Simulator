#include "brig_inst_helper.h"
#include "brig_symbol.h"

#include "llvm/ADT/StringRef.h"

namespace hsa {
namespace brig {

const BrigSymbol getArgument(const BrigInstHelper &helper,
                             const BrigOperandArgumentList *argList,
                             unsigned argNo) {

  assert(argNo < argList->elementCount && "Index beyond argument list");
  BrigoOffset32_t operandOffset = argList->o_args[argNo];
  oper_iterator argRef(helper.S_.operands + operandOffset);
  BrigdOffset32_t dirOffset = cast<BrigOperandArgumentRef>(argRef)->arg;
  dir_iterator symbolDirIt(helper.S_.directives + dirOffset);
  assert(isa<BrigDirectiveSymbol>(symbolDirIt) &&
         "Arguments must reference symbols");
  return BrigSymbol(helper.S_, symbolDirIt);
}

const BrigSymbol
BrigInstHelper::getArgument(const BrigOperandArgumentList *argList,
                            unsigned argNo) const {
  return hsa::brig::getArgument(*this, argList, argNo);
}

static const char *getBaseName(const inst_iterator inst) {

#define caseOper(X)                             \
  case Brig ## X:                               \
    return #X

  switch(inst->opcode) {
    caseOper(Abs);
    caseOper(Add);
    caseOper(Borrow);
    caseOper(Carry);
    caseOper(CopySign);
    caseOper(Div);
    caseOper(Fma);
    caseOper(Fract);
    caseOper(Mad);
    caseOper(Max);
    caseOper(Min);
    caseOper(Mul);
    caseOper(MulHi);
    caseOper(Neg);
    caseOper(Rem);
    caseOper(Sqrt);
    caseOper(Sub);
    caseOper(Mad24);
    caseOper(Mad24Hi);
    caseOper(Mul24);
    caseOper(Mul24Hi);
    caseOper(Shl);
    caseOper(Shr);
    caseOper(And);
    caseOper(Not);
    caseOper(Or);
    caseOper(PopCount);
    caseOper(Xor);
    caseOper(BitRev);
    caseOper(BitSelect);
    caseOper(Extract);
    caseOper(FirstBit);
    caseOper(Insert);
    caseOper(LastBit);
    caseOper(Lda);
    caseOper(Ldc);
    caseOper(Mov);
    caseOper(MovdHi);
    caseOper(MovdLo);
    caseOper(MovsHi);
    caseOper(MovsLo);
    caseOper(Shuffle);
    caseOper(UnpackHi);
    caseOper(UnpackLo);
    caseOper(Cmov);
    caseOper(Class);
    caseOper(Fcos);
    caseOper(Fexp2);
    caseOper(Flog2);
    caseOper(Frcp);
    caseOper(Fsqrt);
    caseOper(Frsqrt);
    caseOper(Fsin);
    caseOper(BitAlign);
    caseOper(ByteAlign);
    caseOper(F2u4);
    caseOper(Lerp);
    caseOper(Sad);
    caseOper(Sad2);
    caseOper(Sad4);
    caseOper(Sad4Hi);
    caseOper(Unpack0);
    caseOper(Unpack1);
    caseOper(Unpack2);
    caseOper(Unpack3);
    caseOper(Segmentp);
    caseOper(FtoS);
    caseOper(StoF);
    caseOper(Cmp);
    caseOper(PackedCmp);
    caseOper(Cvt);
    caseOper(Ld);
    caseOper(St);
    caseOper(Atomic);
    caseOper(AtomicNoRet);
    caseOper(RdImage);
    caseOper(LdImage);
    caseOper(StImage);
    caseOper(AtomicImage);
    caseOper(AtomicNoRetImage);
    caseOper(QueryArray);
    caseOper(QueryData);
    caseOper(QueryDepth);
    caseOper(QueryFiltering);
    caseOper(QueryHeight);
    caseOper(QueryNormalized);
    caseOper(QueryOrder);
    caseOper(QueryWidth);
    caseOper(Cbr);
    caseOper(Brn);
    caseOper(Barrier);
    caseOper(FbarArrive);
    caseOper(FbarInit);
    caseOper(FbarRelease);
    caseOper(FbarSkip);
    caseOper(FbarWait);
    caseOper(Sync);
    caseOper(Count);
    caseOper(CountUp);
    caseOper(Mask);
    caseOper(Send);
    caseOper(Receive);
    caseOper(Call);
    caseOper(Ret);
    caseOper(SysCall);
    caseOper(Alloca);
    caseOper(Clock);
    caseOper(CU);
    caseOper(CurrentWorkGroupSize);
    caseOper(DebugTrap);
    caseOper(DispatchId);
    caseOper(DynWaveId);
    caseOper(FeClearExcept);
    caseOper(FeGetExcept);
    caseOper(FeSetExcept);
    caseOper(GridGroups);
    caseOper(GridSize);
    caseOper(LaneId);
    caseOper(MaxDynWaveId);
    caseOper(Nop);
    caseOper(NullPtr);
    caseOper(Qid);
    caseOper(WorkDim);
    caseOper(WorkGroupId);
    caseOper(WorkGroupSize);
    caseOper(WorkItemAbsId);
    caseOper(WorkItemAbsIdFlat);
    caseOper(WorkItemId);
    caseOper(WorkItemIdFlat);
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
  case BrigPackPPsat: return "Sat_PP";
  case BrigPackPSsat: return "Sat_PS";
  case BrigPackSPsat: return "Sat_SP";
  case BrigPackSSsat: return "Sat_SS";
  case BrigPackSsat:  return "Sat_S";
  case BrigPackPsat:  return "Sat_P";
  case BrigNoPacking: return "";
  default:
    assert(false && "Unknown packing");
  }
#undef castPacking
}

static const char *getTypeName(BrigDataType type) {
#define caseBrig(X)                              \
  case Brig ## X:                                \
  return #X

  switch(type) {
    caseBrig(s8);
    caseBrig(s16);
    caseBrig(s32);
    caseBrig(s64);
    caseBrig(u8);
    caseBrig(u16);
    caseBrig(u32);
    caseBrig(u64);
    caseBrig(f16);
    caseBrig(f32);
    caseBrig(f64);
    caseBrig(b1);
    caseBrig(b8);
    caseBrig(b16);
    caseBrig(b32);
    caseBrig(b64);
    caseBrig(b128);
    caseBrig(ROImg);
    caseBrig(RWImg);
    caseBrig(Samp);
    caseBrig(u8x4);
    caseBrig(s8x4);
    caseBrig(u8x8);
    caseBrig(s8x8);
    caseBrig(u8x16);
    caseBrig(s8x16);
    caseBrig(u16x2);
    caseBrig(s16x2);
    caseBrig(f16x2);
    caseBrig(u16x4);
    caseBrig(s16x4);
    caseBrig(f16x4);
    caseBrig(u16x8);
    caseBrig(s16x8);
    caseBrig(f16x8);
    caseBrig(u32x2);
    caseBrig(s32x2);
    caseBrig(f32x2);
    caseBrig(u32x4);
    caseBrig(s32x4);
    caseBrig(f32x4);
    caseBrig(u64x2);
    caseBrig(s64x2);
    caseBrig(f64x2);
  default:
    assert(false && "Unknown type");
  }
}

const char *getPredName(BrigCompareOperation pred) {
  switch(pred) {
    caseBrig(Eq);
    caseBrig(Ne);
    caseBrig(Lt);
    caseBrig(Le);
    caseBrig(Gt);
    caseBrig(Ge);
    caseBrig(Equ);
    caseBrig(Neu);
    caseBrig(Ltu);
    caseBrig(Leu);
    caseBrig(Gtu);
    caseBrig(Geu);
    caseBrig(Num);
    caseBrig(Nan);
    caseBrig(Seq);
    caseBrig(Sne);
    caseBrig(Slt);
    caseBrig(Sle);
    caseBrig(Sgt);
    caseBrig(Sge);
    caseBrig(Sequ);
    caseBrig(Sgeu);
    caseBrig(Sneu);
    caseBrig(Sleu);
    caseBrig(Sltu);
    caseBrig(Snum);
    caseBrig(Snan);
    caseBrig(Sgtu);
  default:
    assert(false && "Unknown comparison operation");
  }
#undef caseBrig
}

const char *BrigInstHelper::getRoundingName(const BrigAluModifier &aluMod) {
  if(aluMod.floatOrInt) {
    switch(aluMod.rounding) {
    case 0: return "near";
    case 1: return "zero";
    case 2: return "up";
    case 3: return "down";
    default: assert(false && "Unknown rounding mode");
    }
  } else {
    switch(aluMod.rounding) {
    case 0: return "neari";
    case 1: return "zeroi";
    case 2: return "upi";
    case 3: return "downi";
    default: assert(false && "Unknown rounding mode");
    }
  }
}

const char *getAtomicOpName(BrigAtomicOperation atomicOp) {
#define caseBrigAtomic(X)                        \
  case BrigAtomic ## X:                          \
  return #X
  switch(atomicOp) {
    caseBrigAtomic(And);
    caseBrigAtomic(Or);
    caseBrigAtomic(Xor);
    caseBrigAtomic(Cas);
    caseBrigAtomic(Exch);
    caseBrigAtomic(Add);
    caseBrigAtomic(Inc);
    caseBrigAtomic(Dec);
    caseBrigAtomic(Min);
    caseBrigAtomic(Max);
    caseBrigAtomic(Sub);
  default: assert(false && "Unknown atomic operation");
  }
#undef caseBrigAtomic
}

std::string BrigInstHelper::getInstName(const inst_iterator inst) {
  const char *base = getBaseName(inst);
  const char *packing = getPackingName(inst);
  const char *type = getTypeName(BrigDataType(inst->type));

  if(const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst)) {
    const char *srcType = getTypeName(BrigDataType(cmp->sourceType));
    const char *predRaw =
      getPredName(BrigCompareOperation(cmp->comparisonOperator));
    std::string pred = llvm::StringRef(predRaw).lower();
    return std::string(base) + "_" + pred +  "_" + type + "_" + srcType;
  }

  if(const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst)) {
    const char *srcType = getTypeName(BrigDataType(cvt->stype));
    const char *roundingRaw = getRoundingName(cvt->aluModifier);
    std::string rounding = llvm::StringRef(roundingRaw).lower();
    return std::string(base) + "_" + rounding + "_" + type + "_" + srcType;
  }

  if(const BrigInstAtomic *atom = dyn_cast<BrigInstAtomic>(inst)) {
    BrigAtomicOperation atomicOp = BrigAtomicOperation(atom->atomicOperation);
    const char *atomicOpName = getAtomicOpName(atomicOp);
    return std::string(base) + atomicOpName + "_" + type;
  }

  return std::string(base) + packing + "_" + type;
}

} // namespace brig
} // namespace hsa
