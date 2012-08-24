#include "brig_inst_helper.h"
#include "brig_symbol.h"

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

std::string BrigInstHelper::getInstName(const inst_iterator inst) {
  const char *base = getBaseName(inst);
  const char *packing = getPackingName(inst);
  const char *type = getTypeName(inst);

  return std::string(base) + packing + "_" + type;
}

} // namespace brig
} // namespace hsa
