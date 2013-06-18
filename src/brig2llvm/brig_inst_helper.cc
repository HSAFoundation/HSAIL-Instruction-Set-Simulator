//===- brig_inst_helper.cc ---- -------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_inst_helper.h"
#include "brig_symbol.h"

#include "llvm/ADT/StringRef.h"

namespace hsa {
namespace brig {

const BrigSymbol getArgument(const BrigInstHelper &helper,
                             const BrigOperandArgumentList *argList,
                             unsigned argNo) {

  assert(argNo < argList->elementCount && "Index beyond argument list");
  BrigDirectiveOffset32_t operandOffset = argList->elements[argNo];
  dir_iterator symbolDirIt(helper.S_.directives + operandOffset);
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

#define caseOper(X,Y)                           \
  case BRIG_OPCODE_ ## Y:                       \
    return #X

  switch (inst->opcode) {
    caseOper(Abs, ABS);
    caseOper(Add, ADD);
    caseOper(Borrow, BORROW);
    caseOper(Carry, CARRY);
    caseOper(CopySign, COPYSIGN);
    caseOper(Div, DIV);
    caseOper(Fma, FMA);
    caseOper(Fract, FRACT);
    caseOper(Mad, MAD);
    caseOper(Max, MAX);
    caseOper(Min, MIN);
    caseOper(Mul, MUL);
    caseOper(MulHi, MULHI);
    caseOper(Neg, NEG);
    caseOper(Rem, REM);
    caseOper(Sqrt, SQRT);
    caseOper(Sub, SUB);
    caseOper(Mad24, MAD24);
    caseOper(Mad24Hi, MAD24HI);
    caseOper(Mul24, MUL24);
    caseOper(Mul24Hi, MUL24HI);
    caseOper(Shl, SHL);
    caseOper(Shr, SHR);
    caseOper(And, AND);
    caseOper(Not, NOT);
    caseOper(Or, OR);
    caseOper(PopCount, POPCOUNT);
    caseOper(Xor, XOR);
    caseOper(BitRev, BITREV);
    caseOper(BitSelect, BITSELECT);
    caseOper(FirstBit, FIRSTBIT);
    caseOper(LastBit, LASTBIT);
    caseOper(Lda, LDA);
    caseOper(Ldc, LDC);
    caseOper(Mov, MOV);
    caseOper(Shuffle, SHUFFLE);
    caseOper(UnpackHi, UNPACKHI);
    caseOper(UnpackLo, UNPACKLO);
    caseOper(Cmov, CMOV);
    caseOper(Class, CLASS);
    caseOper(Ncos, NCOS);
    caseOper(Nexp2, NEXP2);
    caseOper(Nlog2, NLOG2);
    caseOper(Nrcp, NRCP);
    caseOper(Nsqrt, NSQRT);
    caseOper(Nrsqrt, NRSQRT);
    caseOper(Nsin, NSIN);
    caseOper(BitAlign, BITALIGN);
    caseOper(ByteAlign, BYTEALIGN);
    caseOper(Lerp, LERP);
    caseOper(Segmentp, SEGMENTP);
    caseOper(FtoS, FTOS);
    caseOper(StoF, STOF);
    caseOper(Cmp, CMP);
    caseOper(Cvt, CVT);
    caseOper(Ld, LD);
    caseOper(St, ST);
    caseOper(Atomic, ATOMIC);
    caseOper(AtomicNoRet, ATOMICNORET);
    caseOper(RdImage, RDIMAGE);
    caseOper(LdImage, LDIMAGE);
    caseOper(StImage, STIMAGE);
    caseOper(AtomicImage, ATOMICIMAGE);
    caseOper(AtomicNoRetImage, ATOMICIMAGENORET);
    caseOper(Cbr, CBR);
    caseOper(Brn, BRN);
    caseOper(Barrier, BARRIER);
    caseOper(Sync, SYNC);
    caseOper(Call, CALL);
    caseOper(Ret, RET);
    caseOper(SysCall, SYSCALL);
    caseOper(Alloca, ALLOCA);
    caseOper(Clock, CLOCK);
    caseOper(CurrentWorkGroupSize, CURRENTWORKGROUPSIZE);
    caseOper(DebugTrap, DEBUGTRAP);
    caseOper(DispatchId, DISPATCHID);
    caseOper(GridGroups, GRIDGROUPS);
    caseOper(GridSize, GRIDSIZE);
    caseOper(LaneId, LANEID);
    caseOper(Nop, NOP);
    caseOper(NullPtr, NULLPTR);
    caseOper(Qid, QID);
    caseOper(WorkGroupId, WORKGROUPID);
    caseOper(WorkGroupSize, WORKGROUPSIZE);
    caseOper(WorkItemAbsId, WORKITEMABSID);
    caseOper(WorkItemId, WORKITEMID);
    caseOper(Combine, COMBINE);
    caseOper(BitExtract, BITEXTRACT);
    caseOper(Sad, SAD);
    caseOper(Ceil, CEIL);
    caseOper(Floor, FLOOR);
    caseOper(Rint, RINT);
    caseOper(Trunc, TRUNC);
    caseOper(BitMask, BITMASK);
    caseOper(BitInsert, BITINSERT);
    caseOper(Expand, EXPAND);
    caseOper(NFma, NFMA);
    caseOper(Sadhi, SADHI);
    caseOper(PackCvt, PACKCVT);
    caseOper(UnpackCvt, UNPACKCVT);
    caseOper(QueryImageArray, QUERYIMAGEARRAY);
    caseOper(QueryImageDepth, QUERYIMAGEDEPTH);
    caseOper(QueryImageFormat, QUERYIMAGEFORMAT);
    caseOper(QueryImageHeight, QUERYIMAGEHEIGHT);
    caseOper(QueryImageOrder, QUERYIMAGEORDER);
    caseOper(QueryImageWidth, QUERYIMAGEWIDTH);
    caseOper(QuerySamplerCoord, QUERYSAMPLERCOORD);
    caseOper(QuerySamplerFilter, QUERYSAMPLERFILTER);
    caseOper(ArriveFbar, ARRIVEFBAR);
    caseOper(InitFbar, INITFBAR);
    caseOper(JoinFbar, JOINFBAR);
    caseOper(LeaveFbar, LEAVEFBAR);
    caseOper(ReleaseFbar, RELEASEFBAR);
    caseOper(WaitFbar, WAITFBAR);
    caseOper(Ldf, LDF);
    caseOper(MaskLane, MASKLANE);
    caseOper(CountLane, COUNTLANE);
    caseOper(CountUpLane, COUNTUPLANE);
    caseOper(SendLane, SENDLANE);
    caseOper(ReceiveLane, RECEIVELANE);
    caseOper(CuId, CUID);
    caseOper(Dim, DIM);
    caseOper(GetDetectExcept, GETDETECTEXCEPT);
    caseOper(MaxCuId, MAXCUID);
    caseOper(MaxWaveId, MAXWAVEID);
    caseOper(WaveId, WAVEID);
    caseOper(WorkItemFlatAbsId, WORKITEMFLATABSID);
    caseOper(WorkItemFlatId, WORKITEMFLATID);
    caseOper(ClearDetectExcept, CLEARDETECTEXCEPT);
    caseOper(SetDetectExcept, SETDETECTEXCEPT);
    caseOper(DispatchPtr, DISPATCHPTR);
    caseOper(QPtr, QPTR);
    caseOper(Pack, PACK);
  default:
    assert(false && "Unknown instruction");
  }
#undef castOper
}

static const char *getPackingName(const inst_iterator inst) {

  const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst);
  const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst);
  if (!cmp && !mod) return "";

  BrigPack pack = BrigPack(cmp ? cmp->pack : mod->pack);
  switch (pack) {
    case BRIG_PACK_PP:    return "_PP";
    case BRIG_PACK_PS:    return "_PS";
    case BRIG_PACK_SP:    return "_SP";
    case BRIG_PACK_SS:    return "_SS";
    case BRIG_PACK_S:     return "_S";
    case BRIG_PACK_P:     return "_P";
    case BRIG_PACK_PPSAT: return "_Sat_PP";
    case BRIG_PACK_PSSAT: return "_Sat_PS";
    case BRIG_PACK_SPSAT: return "_Sat_SP";
    case BRIG_PACK_SSSAT: return "_Sat_SS";
    case BRIG_PACK_SSAT:  return "_Sat_S";
    case BRIG_PACK_PSAT:  return "_Sat_P";
    case BRIG_PACK_NONE:  return "";
  default:
    assert(false && "Unknown packing");
  }
}

static const char *getTypeName(BrigType type) {
#define caseBrig(X,Y)                           \
  case BRIG_ ## Y:                              \
    return "_" #X

  switch (type) {
    case BRIG_TYPE_NONE: return "";
    caseBrig(s8, TYPE_S8);
    caseBrig(s16, TYPE_S16);
    caseBrig(s32, TYPE_S32);
    caseBrig(s64, TYPE_S64);
    caseBrig(u8, TYPE_U8);
    caseBrig(u16, TYPE_U16);
    caseBrig(u32, TYPE_U32);
    caseBrig(u64, TYPE_U64);
    caseBrig(f16, TYPE_F16);
    caseBrig(f32, TYPE_F32);
    caseBrig(f64, TYPE_F64);
    caseBrig(b1, TYPE_B1);
    caseBrig(b8, TYPE_B8);
    caseBrig(b16, TYPE_B16);
    caseBrig(b32, TYPE_B32);
    caseBrig(b64, TYPE_B64);
    caseBrig(b128, TYPE_B128);
    caseBrig(ROImg, TYPE_ROIMG);
    caseBrig(RWImg, TYPE_RWIMG);
    caseBrig(Samp, TYPE_SAMP);
    caseBrig(u8x4, TYPE_U8X4);
    caseBrig(s8x4, TYPE_S8X4);
    caseBrig(u8x8, TYPE_U8X8);
    caseBrig(s8x8, TYPE_S8X8);
    caseBrig(u8x16, TYPE_U8X16);
    caseBrig(s8x16, TYPE_S8X16);
    caseBrig(u16x2, TYPE_U16X2);
    caseBrig(s16x2, TYPE_S16X2);
    caseBrig(f16x2, TYPE_F16X2);
    caseBrig(u16x4, TYPE_U16X4);
    caseBrig(s16x4, TYPE_S16X4);
    caseBrig(f16x4, TYPE_F16X4);
    caseBrig(u16x8, TYPE_U16X8);
    caseBrig(s16x8, TYPE_S16X8);
    caseBrig(f16x8, TYPE_F16X8);
    caseBrig(u32x2, TYPE_U32X2);
    caseBrig(s32x2, TYPE_S32X2);
    caseBrig(f32x2, TYPE_F32X2);
    caseBrig(u32x4, TYPE_U32X4);
    caseBrig(s32x4, TYPE_S32X4);
    caseBrig(f32x4, TYPE_F32X4);
    caseBrig(u64x2, TYPE_U64X2);
    caseBrig(s64x2, TYPE_S64X2);
    caseBrig(f64x2, TYPE_F64X2);
  default:
    assert(false && "Unknown type");
  }
}

const char *getPredName(BrigCompareOperation pred) {
  switch (pred) {
    caseBrig(Eq, COMPARE_EQ);
    caseBrig(Ne, COMPARE_NE);
    caseBrig(Lt, COMPARE_LT);
    caseBrig(Le, COMPARE_LE);
    caseBrig(Gt, COMPARE_GT);
    caseBrig(Ge, COMPARE_GE);
    caseBrig(Equ, COMPARE_EQU);
    caseBrig(Neu, COMPARE_NEU);
    caseBrig(Ltu, COMPARE_LTU);
    caseBrig(Leu, COMPARE_LEU);
    caseBrig(Gtu, COMPARE_GTU);
    caseBrig(Geu, COMPARE_GEU);
    caseBrig(Num, COMPARE_NUM);
    caseBrig(Nan, COMPARE_NAN);
    caseBrig(Seq, COMPARE_SEQ);
    caseBrig(Sne, COMPARE_SNE);
    caseBrig(Slt, COMPARE_SLT);
    caseBrig(Sle, COMPARE_SLE);
    caseBrig(Sgt, COMPARE_SGT);
    caseBrig(Sge, COMPARE_SGE);
    caseBrig(Sequ, COMPARE_SEQU);
    caseBrig(Sgeu, COMPARE_SGEU);
    caseBrig(Sneu, COMPARE_SNEU);
    caseBrig(Sleu, COMPARE_SLEU);
    caseBrig(Sltu, COMPARE_SLTU);
    caseBrig(Snum, COMPARE_SNUM);
    caseBrig(Snan, COMPARE_SNAN);
    caseBrig(Sgtu, COMPARE_SGTU);
  default:
    assert(false && "Unknown comparison operation");
  }
#undef caseBrig
}

static bool hasRoundingMode(BrigAluModifier16_t mod) {
  return BrigRound(mod & BRIG_ALU_ROUND) != BRIG_ROUND_NONE;
}

const char *BrigInstHelper::getRoundingName(BrigAluModifier16_t mod) {

  BrigRound round = BrigRound(mod & BRIG_ALU_ROUND);
  switch (round) {
    case BRIG_ROUND_NONE:                       return "";
    case BRIG_ROUND_FLOAT_NEAR_EVEN:            return "near";
    case BRIG_ROUND_FLOAT_ZERO:                 return "zero";
    case BRIG_ROUND_FLOAT_PLUS_INFINITY:        return "up";
    case BRIG_ROUND_FLOAT_MINUS_INFINITY:       return "down";
    case BRIG_ROUND_INTEGER_NEAR_EVEN:          return "neari";
    case BRIG_ROUND_INTEGER_ZERO:               return "zeroi";
    case BRIG_ROUND_INTEGER_PLUS_INFINITY:      return "upi";
    case BRIG_ROUND_INTEGER_MINUS_INFINITY:     return "downi";
    case BRIG_ROUND_INTEGER_NEAR_EVEN_SAT:      return "nears";
    case BRIG_ROUND_INTEGER_ZERO_SAT:           return "zeros";
    case BRIG_ROUND_INTEGER_PLUS_INFINITY_SAT:  return "ups";
    case BRIG_ROUND_INTEGER_MINUS_INFINITY_SAT: return "downs";
    default: assert(false && "Unknown rounding mode");
  }
}

const char *getAtomicOpName(BrigAtomicOperation atomicOp) {
#define caseBrigAtomic(X,Y)                     \
  case BRIG_ATOMIC_ ## Y:                       \
    return #X
  switch (atomicOp) {
    caseBrigAtomic(And, AND);
    caseBrigAtomic(Or, OR);
    caseBrigAtomic(Xor, XOR);
    caseBrigAtomic(Cas, CAS);
    caseBrigAtomic(Exch, EXCH);
    caseBrigAtomic(Add, ADD);
    caseBrigAtomic(Inc, INC);
    caseBrigAtomic(Dec, DEC);
    caseBrigAtomic(Min, MIN);
    caseBrigAtomic(Max, MAX);
    caseBrigAtomic(Sub, SUB);
  default: assert(false && "Unknown atomic operation");
  }
#undef caseBrigAtomic
}

std::string BrigInstHelper::getInstName(const inst_iterator inst) {
  const char *base = getBaseName(inst);
  const char *packing = getPackingName(inst);
  const char *type = getTypeName(BrigType(inst->type));

  if (const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst)) {
    const char *srcType = getTypeName(BrigType(cmp->sourceType));
    const char *predRaw =
      getPredName(BrigCompareOperation(cmp->compare));
    std::string pred = llvm::StringRef(predRaw).lower();
    return std::string(base) + pred + type + srcType;
  }

  if (const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst)) {
    const char *srcType = getTypeName(BrigType(cvt->sourceType));
    const char *roundingRaw = getRoundingName(cvt->modifier);
    std::string rounding =
      hsa::brig::hasRoundingMode(cvt->modifier) ?
      "_" + llvm::StringRef(roundingRaw).lower() : "";
    return std::string(base) + rounding + type + srcType;
  }

  if (const BrigInstAtomic *atom = dyn_cast<BrigInstAtomic>(inst)) {
    BrigAtomicOperation atomicOp = BrigAtomicOperation(atom->atomicOperation);
    const char *atomicOpName = getAtomicOpName(atomicOp);
    return std::string(base) + atomicOpName + type;
  }

  if (const BrigInstSourceType *src = dyn_cast<BrigInstSourceType>(inst)) {
    const char *srcType = getTypeName(BrigType(src->sourceType));
    return std::string(base) + type + srcType;
  }

  return std::string(base) + packing + type;
}

long int nstrtol(size_t size, const uint8_t *str,
                 const uint8_t **endptr, int base) {
  assert(base == 10 && "Unimplemented");

  long int result = 0;
  size_t i;
  for (i = 0; i < size; ++i) {
    if (str[i] < '0' || str[i] > '9') break;
    result *= 10;
    result += str[i] - '0';
  }

  if (endptr) *endptr = str + i;

  return result;
}

long int nstrtol(const BrigString *str, const uint8_t **endptr, int base) {
  return nstrtol(str->byteCount - 2, str->bytes + 2, endptr, 10);
}

}  // namespace brig
}  // namespace hsa
