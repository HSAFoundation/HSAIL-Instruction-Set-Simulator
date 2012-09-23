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
    caseOper(UnpackLo);
    caseOper(UnpackHi);
    caseOper(Shuffle);
    caseOper(MovsLo);
    caseOper(MovsHi);
    caseOper(MovdLo);
    caseOper(MovdHi);
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
    caseOper(Sad4Hi);
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
    caseOper(FbarInitSizeKnown);
    caseOper(FbarInitSizeDynamic);
    caseOper(FbarWait);
    caseOper(FbarArrive);
    caseOper(FbarSkip);
    caseOper(FbarRelease);
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
    caseOper(NDRangeSize);
    caseOper(NDRangeGroups);
    caseOper(LaneId);
    caseOper(DynWaveId);
    caseOper(MaxDynWaveId);
    caseOper(DispatchId);
    caseOper(CU);
    caseOper(WorkDim);
    caseOper(Clock);
    caseOper(WorkItemIdFlat);
    caseOper(WorkItemAIdFlat);
    caseOper(Nop);
    caseOper(Debugtrap);
    caseOper(BitSelect);
    caseOper(MulHi);
    caseOper(Mul24Hi);
    caseOper(Mad24Hi);
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

  return std::string(base) + packing + "_" + type;
}

} // namespace brig
} // namespace hsa
