#include "brig_module.h"
#include "llvm/Support/raw_ostream.h"
#include <cstring>
#include <set>

namespace hsa {
namespace brig {

#define check(X,Y) check(X, Y, __FILE__, __LINE__, #X)

int getTypeSize(BrigDataType16_t type) {
  if(Brigb1 == type) {
    return 1;
  } else if(Brigb8 == type) {
    return 1;
  } else if(Brigb16 == type) {
    return 2;
  } else if(Brigb32 == type) {
    return 4;
  } else if(Brigb64 == type) {
    return 8;
  } else if(Brigb128 == type) {
    return 16;
  }
  return 0;
}

template<class Message>
bool (BrigModule::check)(bool test, const Message &msg,
                         const char *filename, unsigned lineno,
                         const char *cause) const {
  if(!test && out_)
     (*out_) << filename << ":" << lineno << " " << msg
            << " (" << cause << ")\n";
  return test;
}

bool BrigModule::validate(void) const {
  bool valid = true;
  valid &= validateDirectives();
  valid &= validateCode();
  valid &= validateOperands();
  valid &= validateStrings();
  valid &= validateDebug();
  return valid;
}

bool BrigModule::validateDirectives(void) const {

  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

  for(unsigned i = 0; i < std::min(size_t(8), S_.directivesSize); ++i)
    check(!S_.directives[i],
          "The first eight bytes of the directives section must be zero");

  if(!validate(it)) return false;
  if(!check(it != E, "Empty directive section")) return false;

  // 20.8.22: The BrigDirectiveVersion directive must be the first directive
  // in the .directives section.
  const BrigDirectiveVersion *bdv = dyn_cast<BrigDirectiveVersion>(it);
  if(!check(bdv, "Missing BrigDirectiveVersion")) return false;
  if(!validate(bdv)) return false;

#define caseBrig(X)                                   \
  case BrigE ## X:                                    \
    if(!validate(cast<Brig ## X>(it))) return false;  \
    break

  for(; it != E; ++it) {
    if(!validate(it)) return false;
    switch(it->kind) {
      case BrigEDirectiveFunction:
      case BrigEDirectiveKernel:
        if(!validate(cast<BrigDirectiveMethod>(it))) return false;
        break;
      caseBrig(DirectiveSymbol);
      caseBrig(DirectiveImage);
      caseBrig(DirectiveSampler);
      caseBrig(DirectiveLabel);
      caseBrig(DirectiveLabelList);
      caseBrig(DirectiveVersion);
      caseBrig(DirectiveSignature);
      caseBrig(DirectiveFile);
      caseBrig(DirectiveComment);
      caseBrig(DirectiveLoc);
      caseBrig(DirectiveInit);
      caseBrig(DirectiveLabelInit);
      caseBrig(DirectiveControl);
      caseBrig(DirectivePragma);
      caseBrig(DirectiveExtension);
      caseBrig(DirectiveArgStart);
      caseBrig(DirectiveArgEnd);
      caseBrig(DirectiveBlockStart);
      caseBrig(DirectiveBlockNumeric);
      caseBrig(DirectiveBlockString);
      caseBrig(DirectiveBlockEnd);
      caseBrig(DirectivePad);
    default:
      check(false, "Unrecognized directive");
      return false;
    }
  }

  return true;
}

bool BrigModule::validateCode(void) const {

  inst_iterator it = S_.code_begin();
  const inst_iterator E = S_.code_end();

  for(unsigned i = 0; i < std::min(size_t(8), S_.codeSize); ++i)
    check(!S_.code[i],
          "The first eight bytes of the code section must be zero");

  for(; it != E; it++) {
    if(!validate(it)) return false;
    switch(it->kind) {
      caseBrig(InstAtomic);
      caseBrig(InstAtomicImage);
      caseBrig(InstBar);
      caseBrig(InstBase);
      caseBrig(InstCmp);
      caseBrig(InstImage);
      caseBrig(InstCvt);
      caseBrig(InstLdSt);
      caseBrig(InstMem);
      caseBrig(InstMod);
      caseBrig(InstRead);
      default:
        check(false, "Unrecognized code");
        return false;
    }
  }

  return true;
}

bool BrigModule::validateOperands(void) const {
  oper_iterator it = S_.oper_begin();
  const oper_iterator E = S_.oper_end();

  for(unsigned i = 0; i < std::min(size_t(8), S_.operandsSize); ++i)
    check(!S_.operands[i],
          "The first eight bytes of the operands section must be zero");

  for(; it < E; ++it) {
    if(!validate(it)) return false;
    switch(it->kind) {
      caseBrig(OperandAddress);
      caseBrig(OperandArgumentList);
      caseBrig(OperandFunctionList);
      caseBrig(OperandArgumentRef);
      caseBrig(OperandBase);
      caseBrig(OperandCompound);
      caseBrig(OperandFunctionRef);
      caseBrig(OperandImmed);
      caseBrig(OperandIndirect);
      caseBrig(OperandLabelRef);
      caseBrig(OperandOpaque);
      caseBrig(OperandPad);
      caseBrig(OperandReg);
      caseBrig(OperandRegV2);
      caseBrig(OperandRegV4);
      caseBrig(OperandWaveSz);
      default:
        check(false, "Unrecognized operands");
        return false;
    }
  }

#undef caseBrig

  return true;
}

bool BrigModule::validateInstructions(void) const {

  inst_iterator it = S_.code_begin();
  const inst_iterator E = S_.code_end();

  for(unsigned i = 0; i < std::min(size_t(8), S_.codeSize); ++i)
    check(!S_.code[i],
          "The first eight bytes of the code section must be zero");

#define caseInst(X)                             \
  case Brig ## X:                               \
    if(!validate ## X(it)) return false;        \
    break

  for(; it != E; it++) {
    switch(it->opcode) {
      caseInst(Abs);
      caseInst(Add);
      caseInst(Borrow);
      caseInst(Carry);
      caseInst(CopySign);
      caseInst(Div);
      caseInst(Fma);
      caseInst(Fract);
      caseInst(Mad);
      caseInst(Max);
      caseInst(Min);
      caseInst(Mul);
      caseInst(MulHi);
      caseInst(Neg);
      caseInst(Rem);
      caseInst(Sqrt);
      caseInst(Sub);
      caseInst(Mad24);
      caseInst(Mad24Hi);
      caseInst(Mul24);
      caseInst(Mul24Hi);
      caseInst(Shl);
      caseInst(Shr);
      caseInst(And);
      caseInst(Not);
      caseInst(Or);
      caseInst(PopCount);
      caseInst(Xor);
      caseInst(BitRev);
      caseInst(BitSelect);
      caseInst(Extract);
      caseInst(FirstBit);
      caseInst(Insert);
      caseInst(LastBit);
      caseInst(Lda);
      caseInst(Ldc);
      caseInst(Mov);
      caseInst(MovdHi);
      caseInst(MovdLo);
      caseInst(MovsHi);
      caseInst(MovsLo);
      caseInst(Shuffle);
      caseInst(UnpackHi);
      caseInst(UnpackLo);
      caseInst(Cmov);
      caseInst(Class);
      caseInst(Fcos);
      caseInst(Fexp2);
      caseInst(Flog2);
      caseInst(Frcp);
      caseInst(Frsqrt);
      caseInst(Fsin);
      caseInst(BitAlign);
      caseInst(ByteAlign);
      caseInst(F2u4);
      caseInst(Lerp);
      caseInst(Sad);
      caseInst(Sad2);
      caseInst(Sad4);
      caseInst(Sad4Hi);
      caseInst(Unpack0);
      caseInst(Unpack1);
      caseInst(Unpack2);
      caseInst(Unpack3);
      caseInst(Segmentp);
      caseInst(FtoS);
      caseInst(StoF);
      caseInst(Cmp);
      caseInst(PackedCmp);
      caseInst(Cvt);
      caseInst(Ld);
      caseInst(St);
      caseInst(Atomic);
      caseInst(AtomicNoRet);
      caseInst(RdImage);
      caseInst(LdImage);
      caseInst(StImage);
      caseInst(AtomicImage);
      caseInst(AtomicNoRetImage);
      caseInst(QueryArray);
      caseInst(QueryData);
      caseInst(QueryDepth);
      caseInst(QueryFiltering);
      caseInst(QueryHeight);
      caseInst(QueryNormalized);
      caseInst(QueryOrder);
      caseInst(QueryWidth);
      caseInst(Cbr);
      caseInst(Brn);
      caseInst(Barrier);
      caseInst(FbarArrive);
      caseInst(FbarInit);
      caseInst(FbarRelease);
      caseInst(FbarSkip);
      caseInst(FbarWait);
      caseInst(Sync);
      caseInst(Count);
      caseInst(CountUp);
      caseInst(Mask);
      caseInst(Send);
      caseInst(Receive);
      caseInst(Call);
      caseInst(Ret);
      caseInst(SysCall);
      caseInst(Alloca);
      caseInst(Clock);
      caseInst(CU);
      caseInst(DebugTrap);
      caseInst(DispatchId);
      caseInst(DynWaveId);
      caseInst(LaneId);
      caseInst(MaxDynWaveId);
      caseInst(NDRangeGroups);
      caseInst(NDRangeSize);
      caseInst(Nop);
      caseInst(NullPtr);
      caseInst(WorkDim);
      caseInst(WorkGroupId);
      caseInst(WorkGroupSize);
      caseInst(WorkItemAId);
      caseInst(WorkItemAIdFlat);
      caseInst(WorkItemId);
      caseInst(WorkItemIdFlat);
    default:
      check(false, "Unrecognized opcode");
      return false;
    }
  }

#undef caseInst

  return true;
}

bool BrigModule::validateStrings(void) const {

  bool valid = true;

  for(unsigned i = 0; i < std::min(size_t(8), S_.stringsSize); ++i)
    check(!S_.strings[i],
          "The first eight bytes of the strings section must be zero");

  std::set<std::string> stringSet;

  const char *curr = S_.strings + 8;
  size_t maxLen = S_.stringsSize - 8;

  while(maxLen) {
    size_t len = strnlen(curr, maxLen);
    if(!check(len != maxLen, "String not null terminated"))
      return false;

    valid &= check(stringSet.insert(curr).second, "Duplicate string detected");

    // Account for the null terminator
    maxLen -= (len + 1);
    curr += (len + 1);
  }

  return valid;
}

bool BrigModule::validateDebug(void) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validate(const BrigDirectiveMethod *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);

  const unsigned paramCount = dir->inParamCount + dir->outParamCount;
  dir_iterator argIt = dir_iterator(dir) + 1;
  for(unsigned i = 0; i < paramCount; ++i, ++argIt) {
    if(!validate(argIt)) return false;
    const BrigDirectiveSymbol *bds = dyn_cast<BrigDirectiveSymbol>(argIt);
    if(!check(bds, "Too few argument symbols")) return false;
    if(dir->kind == BrigEDirectiveFunction)
      valid &= check(bds->s.storageClass == BrigArgSpace,
                     "Argument not in arg space");
    if(dir->kind == BrigEDirectiveKernel)
      valid &= check(bds->s.storageClass == BrigKernargSpace,
                     "Argument not in kernarg space");
  }

  const dir_iterator firstScopedDir(S_.directives +
                                    dir->d_firstScopedDirective);
  if(!validOrEnd(firstScopedDir)) return false;
  valid &= check(argIt <= firstScopedDir,
                 "The first scoped directive is too early");
  valid &= check(dir->d_firstScopedDirective <= dir->d_nextDirective,
                 "The next directive is before the first scoped directive");
  valid &= check(dir->attribute == BrigExtern ||
                 dir->attribute == BrigStatic ||
                 dir->attribute == BrigNone,
                 "Invalid linkage type");

  if(dir->inParamCount) {
    dir_iterator firstInParam1(dir);
    for(unsigned i = 0; i < dir->outParamCount + 1; ++i) {
      ++firstInParam1;
      if(!validate(firstInParam1)) return false;
    }

    const dir_iterator firstInParam2(S_.directives + dir->d_firstInParam);
    if(!validate(firstInParam2)) return false;
    valid &= check(firstInParam1 == firstInParam2, "d_firstInParam is wrong");
  }

  return valid;
}

bool BrigModule::validate(const BrigDirectiveSymbol *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validate(&dir->s);
  valid &= check(!dir->reserved, "Reserved not zero");

  if(dir->d_init) {

    // 4.23
    valid &= check(dir->s.storageClass == BrigReadonlySpace ||
                   dir->s.storageClass == BrigGlobalSpace,
                   "Only global and readonly spaces can be initialized");

    const dir_iterator init(S_.directives + dir->d_init);
    if(!validate(init)) return false;
    const BrigDirectiveInit *bdi =
      dyn_cast<BrigDirectiveInit>(init);
    const BrigDirectiveLabelInit *bdli =
      dyn_cast<BrigDirectiveLabelInit>(init);
    if(!check(bdi || bdli, "Missing initializer")) return false;

    uint32_t elementCount = bdi ? bdi->elementCount : bdli->elementCount;
    if(dir->s.dim)
      valid &= check(elementCount == dir->s.dim,
                     "Inconsistent array dimensions");
  }

  return valid;
}

bool BrigModule::validate(const BrigDirectiveImage *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validate(&dir->s);
  if(dir->array > 1) {
    valid &= check(dir->depth == 0,
                   "depth value is wrong for 1DA and 2DA images");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveSampler *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  if(dir->valid == 1) {
    valid &= check(dir->filter <= BrigSamplerFilterNearest,
                   "Invalid filter");
    valid &= check(dir->boundaryU <= BrigSamplerBorder,
                   "Invalid boundaryU");
    valid &= check(dir->boundaryV <= BrigSamplerBorder,
                   "Invalid boundaryV");
    valid &= check(dir->boundaryW <= BrigSamplerBorder,
                   "Invalid boundaryW");
    valid &= check(dir->reserved1 == 0 ,
                   "The value of reserved1 must be zero");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabel *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelList *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

// 20.8.22
bool BrigModule::validate(const BrigDirectiveVersion *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= check(dir->machine == BrigELarge ||
                 dir->machine == BrigESmall,
                 "Invalid machine");
  valid &= check(dir->profile == BrigEFull ||
                 dir->profile == BrigEReduced,
                 "Invalid profile");

  valid &= check(dir->ftz == BrigESftz ||
                 dir->ftz == BrigENosftz,
                 "Invalid flush to zero");
  valid &= check(!dir->reserved, "Reserved not zero");

  return valid;
}

bool BrigModule::validate(const BrigDirectiveSignature *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  valid &= check(!dir->reserved, "Reserved not zero");
  valid &= check(sizeof(BrigDirectiveSignature) +
                 sizeof(BrigDirectiveSignature::BrigProtoType) *
                 (dir->outCount + dir->inCount - 1) <= dir->size,
                 "BrigDirectiveProto size too small for outCount + inCount");
  for (unsigned i = 0; i < dir->outCount + dir->inCount; i++) {
     valid &= check(dir->types[i].type <= Brigf64x2,
                 "Invalid type");
     if(dir->types[i].hasDim == 1) {
       valid &= check(dir->types[i].dim, "dimension not set when hasDim is 1");
     }
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveFile *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_filename);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveComment *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLoc *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveInit *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 8);
  valid &= check(!dir->reserved, "Reserved not zero");
  valid &= check(Brigb1 == dir->type  || Brigb8 == dir->type  ||
                 Brigb16 == dir->type || Brigb32 == dir->type ||
                 Brigb64 == dir->type || Brigb128 == dir->type,
                 "Invalid type, must be b1, b8, b16, b32, b64, or b128");
  valid &= check(sizeof(BrigDirectiveInit) - sizeof(uint64_t) +
                 dir->elementCount * getTypeSize(dir->type) <= dir->size,
                 "Directive size too small for elementCount");

  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelInit *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  for (unsigned i = 0; i < dir->elementCount; i++) {
    valid &= check(dir->d_labels[i] < S_.directivesSize,
                   "d_labels past the directives section");

    const dir_iterator init(S_.directives + dir->d_labels[i]);
    if(!validate(init)) return false;
    const BrigDirectiveLabel *bcl = dyn_cast<BrigDirectiveLabel>(init);
    if(!check(bcl, "d_labels offset is wrong, not a BrigDirectiveLabel"))
      return false;
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveControl *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectivePragma *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveExtension *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgStart *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgEnd *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockStart *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateSName(dir->s_name);
  const char *string = S_.strings + dir->s_name;
  valid &= check(0 == strcmp(string, "debug") ||
                 0 == strcmp(string, "rti"),
                 "Invalid s_name, should be either debug or rti");
  valid &= validateCCode(dir->c_code);

  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockNumeric *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= check(0 == dir->size % 8,
                 "Invalid size, must be a multiple of 8");
  valid &= check(Brigb1 == dir->type  || Brigb8 == dir->type  ||
                 Brigb16 == dir->type || Brigb32 == dir->type ||
                 Brigb64 == dir->type,
                 "Invalid type, must be b1, b8, b16, b32, or b64");
  valid &= check(sizeof(BrigBlockNumeric) - sizeof(uint64_t) +
                 dir->elementCount * getTypeSize(dir->type) <= dir->size,
                 "Directive size too small for elementCount");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockString *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockEnd *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  return valid;
}

bool BrigModule::validate(const BrigDirectivePad *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  return valid;
}

bool BrigModule::validate(const BrigSymbolCommon *s) const {
  bool valid = true;

  // RPM 20.5.20: 8-16 reserved for extensions
  valid &= validateCCode(s->c_code);
  valid &= check(s->storageClass <= BrigFlatSpace + 8,
                 "Invalid storage class");
  valid &= check(s->attribute <= BrigNone,
                 "Invalid linkage type");
  valid &= check(!s->reserved, "Reserved not zero");
  valid &= check(s->symbolModifier < (BrigConst | BrigArray | BrigFlex),
                 "Invalid symbol modifier");
  // PRM 4.24
  if(!(s->symbolModifier & BrigArray))
    valid &= check(!s->dim, "Non-array type with non-zero dimension");
  valid &= validateSName(s->s_name);
  valid &= check(s->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(s->align == 1 || s->align == 2 ||
                 s->align == 4 || s->align == 8,
                 "Invalid alignment");

  return valid;
}

bool BrigModule::validOrEnd(const dir_iterator dir) const {

  // Exit early to avoid segmentation faults.
  dir_iterator firstValidDir(S_.directives);
  if(!check(firstValidDir <= dir, "dir before the directives section"))
    return false;

  dir_iterator E(S_.directives + S_.directivesSize);
  if(!check(dir <= E, "dir past the directives section"))
    return false;

  return true;
}

bool BrigModule::validate(const dir_iterator dir) const {

  // Exit early to avoid segmentation faults.
  if(!validOrEnd(dir)) return false;

  dir_iterator lastValidDir(S_.directives + S_.directivesSize -
                            sizeof(BrigDirectiveBase));
  if(!check(dir <= lastValidDir, "dir spans the directives section"))
    return false;

  dir_iterator E(S_.directives + S_.directivesSize);
  if(!check(dir + 1 <= E, "dir spans the directives section"))
    return false;

  return true;
}

bool BrigModule::validateCCode(BrigcOffset32_t c_code) const {
  bool valid = true;
  valid &= check(c_code + sizeof(BrigInstBase) > c_code,
                 "c_code overflows");
  valid &= check(!c_code || c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

bool BrigModule::validateSName(BrigsOffset32_t s_name) const {

  bool valid = true;

  valid &= check(s_name < S_.stringsSize,
                 "s_name past the strings section");

  // Do attempt the next test if s_name is past the end of the strings
  // section! It may cause a segmentation fault.
  if(!valid) return false;

  size_t maxlen = S_.stringsSize - s_name;
  size_t length = strnlen(S_.strings + s_name, maxlen);
  valid &= check(length != maxlen, "String not null terminated");

  return valid;
}

bool BrigModule::validateAlignment(const void *dir, uint8_t alignment) const {
  bool valid = true;
  const char *dirOffset = reinterpret_cast<const char *>(dir);
  valid &= check((S_.directives - dirOffset) % alignment == 0,
                 "Improperly aligned directive");
  return valid;
}

template<typename T> bool BrigModule::validateSize(const T *brig) const{
  return check(brig->size >= sizeof(T),"Brig structure is too small");
}

// validating the code section
bool BrigModule::validate(const BrigAluModifier *c) const {
  bool valid = true;
  if(c->approx == 1) {
    valid &= check(c->floatOrInt == 1,
                   "Invalid floatOrInt");
  }
  if(c->floatOrInt == 1) {
    valid &= check(c->hi == 0,
                   "Invalid hi");
  }
  if(c->floatOrInt == 0) {
    valid &= check(c->ftz == 0,
                   "Invalid ftz");
  }
  valid &= check(c->reserved == 0,
                 "Invalid reserved");
  return valid;
}

bool BrigModule::validate(const BrigInstAtomic *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigAtomic ||
                 code->opcode == BrigAtomicNoRet,
                 "Invalid opcode, should be either BrigAtomic or "
                 "BrigAtomicNoRet");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->atomicOperation <= BrigAtomicSub,
                 "Invalid atomicOperation");
  valid &= check(code->storageClass == BrigGlobalSpace ||
                 code->storageClass == BrigGroupSpace ||
                 code->storageClass == BrigPrivateSpace ||
                 code->storageClass == BrigKernargSpace ||
                 code->storageClass == BrigReadonlySpace ||
                 code->storageClass == BrigSpillSpace ||
                 code->storageClass == BrigArgSpace ||
                 code->storageClass == BrigFlatSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, or arg");
  valid &= check(code->memorySemantic == BrigRegular ||
                 code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigAcquireRelease ||
                 code->memorySemantic == BrigParAcquireRelease,
                 "Invalid memorySemantic, can be regular, acquire, "
                 "acquire release, or partial acquire release");
  return valid;
}

bool BrigModule::validate(const BrigInstAtomicImage *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigAtomicImage ||
                 code->opcode == BrigAtomicNoRetImage,
                 "Invalid opcode, should be either BrigAtomicImage or "
                 "BrigAtomicNoRetImage");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                     "o_operands past the operands section");
    }
  }
  valid &= check(code->atomicOperation <= BrigAtomicSub,
                 "Invalid atomicOperation");
  valid &= check(code->storageClass == BrigGlobalSpace,
                 "Invalid storage class, must be global");
  valid &= check(code->memorySemantic == BrigRegular ||
                 code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigAcquireRelease,
                 "Invalid memorySemantic, can be regular, "
                 "acquire, or acquire release");
  valid &= check(code->geom <= Briggeom_2da, "Invalid geom");
  return valid;
}

bool BrigModule::validate(const BrigInstBar *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigBarrier ||
                 code->opcode == BrigSync    ||
                 code->opcode == BrigBrn,
                 "Invalid opcode, should be either BrigBarrier, BrigSync or "
                 "BrigBrn");
  valid &= check(code->type <= Brigf64x2, "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                     "o_operands past the operands section");
    }
  }
  valid &= check(
    code->syncFlags <= (BrigGroupLevel | BrigGlobalLevel | BrigPartialLevel),
    "Invalid syncFlags, should be either BrigGroupLevel BrigGlobalLevel"
    "or BrigPartialLevel");
  return valid;
}

bool BrigModule::validate(const BrigInstBase *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode < BrigInvalidOpcode,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                     "o_operands past the operands section");
    }
  }
  return valid;
}

bool BrigModule::validate(const BrigInstCmp *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigCmp,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= validate(&code->aluModifier);
  valid &= check(code->comparisonOperator <= BrigSgtu,
                 "Invalid comparisonOperator");
  valid &= check(code->sourceType <= Brigf64x2,
                 "Invalid sourceType");
  valid &= check(code->reserved == 0,
                 "Invalid reserved");
  return valid;
}
bool BrigModule::validate(const BrigInstImage *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigRdImage,
                 "Invalid opcode");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->geom <= Briggeom_2da,
                 "Invalid type of image geometry");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->stype <= Brigf64x2,
                 "Invalid stype");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  valid &= check(code->reserved == 0,
                 "reserved must be zero");
  return valid;
}
bool BrigModule::validate(const BrigInstCvt *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigCvt,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= validate(&code->aluModifier);
  valid &= check(code->stype <= Brigf64x2,
                 "Invalid stype");
  valid &= check(code->reserved == 0,
                 "reserved must be zero");
  return valid;
}
bool BrigModule::validate(const BrigInstLdSt *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode < BrigInvalidOpcode,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->storageClass <= BrigFlatSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, arg or flat");
  valid &= check(code->memorySemantic == BrigRegular ||
                 code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigRelease ||
                 code->memorySemantic == BrigDep ||
                 code->memorySemantic == BrigParAcquire ||
                 code->memorySemantic == BrigParRelease,
                 "Invalid memorySemantic, can be regular, "
                 "acquire, release, dep, partial acquire, "
                 "or partial release");
  valid &= check(code->equivClass < 64,
                 "Invalid equivClass, must less than 64");
  return valid;
}
bool BrigModule::validate(const BrigInstMem *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode < BrigInvalidOpcode,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->storageClass == BrigGlobalSpace ||
                 code->storageClass == BrigGroupSpace ||
                 code->storageClass == BrigPrivateSpace ||
                 code->storageClass == BrigKernargSpace ||
                 code->storageClass == BrigReadonlySpace ||
                 code->storageClass == BrigSpillSpace ||
                 code->storageClass == BrigArgSpace ||
                 code->storageClass == BrigFlatSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, arg, or flat");
    return valid;
}
bool BrigModule::validate(const BrigInstMod *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode < BrigInvalidOpcode,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= validate(&code->aluModifier);
  return valid;
}
bool BrigModule::validate(const BrigInstRead *code) const {
  bool valid = true;
  if(!validateSize(code)) return false;
  valid &= check(code->opcode == BrigRdImage,
                 "Invalid opcode");
  for (unsigned i = 0; i < 5; i++) {
    if(code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->geom <= Briggeom_2da,
                 "Invalid type of image geometry");
  valid &= check(code->stype <= Brigf64x2,
                 "Invalide type");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  valid &= check(code->reserved == 0,
                 "reserved must be zero");
  return valid;
}

bool BrigModule::validate(const inst_iterator inst) const {

  // Exit early to avoid segmentation faults.
  inst_iterator firstValidCode(S_.code);
  if(!check(firstValidCode <= inst, "inst before the code section"))
    return false;

  inst_iterator E(S_.code + S_.codeSize);
  if(!check(inst <= E, "inst past the code section"))
    return false;

  inst_iterator lastValidCode(S_.code + S_.codeSize -
                            sizeof(BrigInstBase));
  if(!check(inst <= lastValidCode, "inst spans the code section"))
    return false;

  if(!check(inst + 1 <= E, "inst spans the code section"))
    return false;

  return true;
}

bool BrigModule::validate(const BrigOperandAddress *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  valid &= check(operand->type == Brigb32 ||
                 operand->type == Brigb64, "Invald datatype, should be "
                 "Brigb32 and Brigb64");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  valid &= check(operand->directive < S_.directivesSize,
                 "directive past the directive section");
  dir_iterator dir(S_.directives + operand->directive);
  if(!validate(dir)) return false;
  valid &= check(isa<BrigDirectiveSymbol>(dir),
                 "Invalid directive, should point to a BrigDirectiveSymbol");
  return valid;
}

bool BrigModule::validate(const BrigOperandArgumentList *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  size_t dirSize =
    sizeof(BrigOperandArgumentList) +
    sizeof(operand->o_args[0]) * (std::max(1U, operand->elementCount) - 1);
  valid &= check(operand->size >= dirSize, "Invalid size");

  for (unsigned i = 0; i < operand->elementCount; ++i) {
    oper_iterator arg(S_.operands + operand->o_args[i]);
    if(!validate(arg)) return false;
    valid &= check(isa<BrigOperandArgumentRef>(arg),
                   "Invalid o_args, should point to BrigOperandArgumentRef");
  }
  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionList *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  unsigned funRefCount = 0;
  unsigned argRefCount = 0;

  if(operand->elementCount) {
    valid &= check(sizeof(BrigOperandArgumentList) +
                   sizeof(operand->o_args[1]) * (operand->elementCount - 1)
                   <= operand->size, "Invalid size");
  }

  for(unsigned i = 0; i < operand->elementCount; ++i) {
    oper_iterator arg(S_.operands + operand->o_args[i]);
    if(!validate(arg)) return false;
    if(const BrigOperandFunctionRef *funRef =
       dyn_cast<BrigOperandFunctionRef>(arg)) {
      dir_iterator fun(S_.directives + funRef->fn);
      if(!validate(fun)) return false;
      valid &= check(isa<BrigDirectiveFunction>(fun),
                     "should point to BrigOperandFunctionRef, "
                     "refer to BrigDirectiveFunction");
      ++funRefCount;
    }
    if(const BrigOperandArgumentRef *argRef =
        dyn_cast<BrigOperandArgumentRef>(arg)) {
      dir_iterator funSig(S_.directives + argRef->arg);
      if(!validate(funSig)) return false;
      //conflict with BrigOperandArgumentRef refer to BrigDirectiveSymbol
      valid &= check(isa<BrigDirectiveSignature>(funSig),
                     "should point to BrigOperandArgumentRef, "
                     "refer to BrigDirectiveSignature");
      ++argRefCount;
    }
  }
  valid &= check(funRefCount == operand->elementCount ||
                 argRefCount == operand->elementCount,
                 "element of o_args should be BrigOperandFunctionRef "
                 "or BrigOperandArgumentRef");
  valid &= check(argRefCount < 2, "Invalid argRefCount, should be 1 or 0");

  return valid;
}

bool BrigModule::validate(const BrigOperandArgumentRef *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  dir_iterator argDir(S_.directives + operand->arg);
  if(!validate(argDir)) return false;
  valid &= check(isa<BrigDirectiveSymbol>(argDir),
                 "Invalid reg, should be point BrigDirectiveSymbol");
  return valid;
}

bool BrigModule::validate(const BrigOperandBase *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  return valid;
}

bool BrigModule::validate(const BrigOperandCompound *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  valid &= check(operand->type == Brigb32 ||
                 operand->type == Brigb64, "Invald datatype, should be "
                 "Brigb32 and Brigb64");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  oper_iterator nameOper(S_.operands + operand->name);
  if(!validate(nameOper)) return false;
  valid &= check(isa<BrigOperandAddress>(nameOper),
                 "Invalid name, should point to BrigOperandAddress");

  if(operand->reg) {
    const oper_iterator oper(S_.operands + operand->reg);
    if(!validate(oper)) return false;
    const BrigOperandReg *bor = dyn_cast<BrigOperandReg>(oper);
    if(!check(bor, "reg offset is wrong, not a BrigOperandReg"))
      return false;
    valid &= check(bor->type == Brigb32 ||
                   bor->type == Brigb64, "Invalid register, the register "
                   "must be an s or d register");
  }

  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionRef *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  dir_iterator fnDir(S_.directives + operand->fn);
  if(!validate(fnDir)) return false;
  valid &= check(isa<BrigDirectiveFunction>(fnDir) ||
                 isa<BrigDirectiveSignature>(fnDir),
                 "Invalid directive, should point to a "
                 "BrigDirectiveFunction or BrigDirectiveSignature");
  return valid;
}

bool BrigModule::validate(const BrigOperandImmed *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  valid &= check(Brigb1 == operand->type  || Brigb8 == operand->type  ||
                 Brigb16 == operand->type || Brigb32 == operand->type ||
                 Brigb64 == operand->type,
                 "Invalid type, must be b1, b8, b16, b32 or b64");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  long int immedSize = sizeof(BrigOperandImmed) - 2 * sizeof(uint64_t);
  long int immediateSize = immedSize + getTypeSize(operand->type);
  valid &= check(immediateSize <= operand->size,
                 "Operand size too small for immediate");
  return valid;
}

bool BrigModule::validate(const BrigOperandIndirect *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  if(operand->reg) {
    oper_iterator regOper(S_.operands + operand->reg);
    if(!validate(regOper)) return false;
    valid &= check(isa<BrigOperandReg>(regOper),
                   "Invalid reg, should be point BrigOprandReg");
    valid &= check(operand->type == Brigb32 ||
                   operand->type == Brigb64, "Invald datatype, should be "
                   "Brigb32 and Brigb64");
  }

  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  return valid;
}

bool BrigModule::validate(const BrigOperandLabelRef *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  dir_iterator directiveDir(S_.directives + operand->labeldirective);
  if(!validate(directiveDir)) return false;
  valid &= check(isa<BrigDirectiveLabel>(directiveDir),
                 "Invalid directive, should point "
                 "to a BrigDirectiveLabel");
  return valid;
}
bool BrigModule::validate(const BrigOperandOpaque *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  dir_iterator nameDir(S_.directives + operand->name);
  if(!validate(nameDir)) return false;
  valid &= check(isa<BrigDirectiveImage>(nameDir) ||
                 isa<BrigDirectiveSampler>(nameDir),
                 "Invalid directive, should point to a "
                 "BrigDirectiveImage or BrigDirectiveSampler");
  const oper_iterator oper(S_.operands + operand->reg);
  if(!validate(oper)) return false;
  const BrigOperandReg *bor = dyn_cast<BrigOperandReg>(oper);
  if(!check(bor, "reg offset is wrong, not a BrigOperandReg")) return false;
  valid &= check(bor->type == Brigb32,
                 "Register type should be Brigb32");
  return valid;
}

static bool getRegType(char c, BrigDataType *type) {
  if(c == 'c') {
    *type = Brigb1;
    return true;
  } else if(c == 's') {
    *type = Brigb32;
    return true;
  } else if(c == 'd') {
    *type = Brigb64;
    return true;
  } else if(c == 'q') {
    *type = Brigb128;
    return true;
  } else {
    return false;
  }
}

bool BrigModule::validate(const BrigOperandPad *operand) const {
  if(!validateSize(operand)) return false;
  return true;
}

bool BrigModule::validate(const BrigOperandReg *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  // Exit early to prevent out-of-bounds access
  if(!validateSName(operand->name))
    return false;

  const char *name = S_.strings + operand->name;

  // Exit early to prevent out-of-bounds access
  if(!check(name[0] == '$', "Register names must begin with '$'"))
    return false;

  // Exit early to prevent out-of-bounds access
  BrigDataType type;
  if(!check(getRegType(name[1], &type), "Invalid register type"))
    return false;

  // Exit early to prevent out-of-bounds access
  if(!check(isdigit(name[2]), "Register offset not a number"))
    return false;

  char *endptr;
  long int regOffset = strtol(name + 2, &endptr, 10);
  valid &= check(!*endptr, "Garbage after register offset");
  if(type == Brigb1 || type == Brigb128)
    check(0 <= regOffset && regOffset < 8, "Register offset out-of-bounds");
  else if(type == Brigb32 || type == Brigb64)
    check(0 <= regOffset && regOffset < 32, "Register offset out-of-bounds");

  valid &= check(operand->type == type, "Register name does not match type");
  valid &= check(!operand->reserved, "reserved must be zero");

  return valid;
}

bool BrigModule::validate(const BrigOperandRegV2 *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  for(int i = 0; i < 2; i++) {
    const oper_iterator oper(S_.operands + operand->regs[i]);
    if(!validate(oper)) return false;
    const BrigOperandReg *bor = dyn_cast<BrigOperandReg>(oper);
    valid &= check(bor, "reg offset is wrong, not a BrigOperandReg");
    valid &= check(bor->type == operand->type,
                   "should be the same type with BrigOperandReg");
  }
  valid &= check(operand->type == Brigb1 ||
                 operand->type == Brigb32 ||
                 operand->type == Brigb64,
                 "Invalid date type");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  return valid;
}
bool BrigModule::validate(const BrigOperandRegV4 *operand) const {
  bool valid = true;
  if(!validateSize(operand)) return false;
  for(int i = 0; i < 4; i++) {
    const oper_iterator oper(S_.operands + operand->regs[i]);
    if(!validate(oper)) return false;
    const BrigOperandReg *bor = dyn_cast<BrigOperandReg>(oper);
    valid &= check(bor, "reg offset is wrong, not a BrigOperandReg");
    valid &= check(bor->type == operand->type,
                   "should be the same type with BrigOperandReg");
  }
  valid &= check(operand->type == Brigb1 ||
                 operand->type == Brigb32 ||
                 operand->type == Brigb64,
                 "Invalid date type");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  return valid;
}
bool BrigModule::validate(const BrigOperandWaveSz *operand) const {
  if(!validateSize(operand)) return false;
  return true;
}

bool BrigModule::validate(const oper_iterator operands) const {

  // Exit early to avoid segmentation faults.
  oper_iterator firstValidOperands(S_.operands);
  if(!check(firstValidOperands <= operands,
     "operands before the operands section"))
    return false;

  oper_iterator E(S_.operands + S_.operandsSize);
  if(!check(operands <= E, "operands past the operands section"))
    return false;

  oper_iterator lastValidOperands(S_.operands + S_.operandsSize -
                            sizeof(BrigOperandBase));
  if(!check(operands <= lastValidOperands,
     "operands spans the operands section"))
    return false;

  if(!check(operands + 1 <= E, "operands spans the operands section"))
    return false;

  return true;
}

bool BrigModule::validateAbs(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAdd(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBorrow(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCarry(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCopySign(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateDiv(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFma(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFract(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMad(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMax(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMin(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMul(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMulHi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNeg(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateRem(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSqrt(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSub(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMad24(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMad24Hi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMul24(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMul24Hi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateShl(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateShr(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAnd(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNot(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateOr(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validatePopCount(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateXor(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBitRev(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBitSelect(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateExtract(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFirstBit(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateInsert(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLastBit(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLda(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLdc(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMov(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMovdHi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMovdLo(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMovsHi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMovsLo(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateShuffle(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpackHi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpackLo(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCmov(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateClass(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFcos(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFexp2(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFlog2(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFrcp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFrsqrt(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFsin(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBitAlign(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateByteAlign(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateF2u4(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLerp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSad(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSad2(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSad4(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSad4Hi(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpack0(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpack1(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpack2(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateUnpack3(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSegmentp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFtoS(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateStoF(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCmp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validatePackedCmp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCvt(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLd(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSt(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAtomic(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAtomicNoRet(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateRdImage(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLdImage(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateStImage(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAtomicImage(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAtomicNoRetImage(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryArray(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryData(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryDepth(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryFiltering(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryHeight(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryNormalized(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryOrder(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateQueryWidth(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCbr(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBrn(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateBarrier(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFbarArrive(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFbarInit(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFbarRelease(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFbarSkip(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateFbarWait(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSync(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCount(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCountUp(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMask(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSend(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateReceive(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCall(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateRet(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateSysCall(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateAlloca(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateClock(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateCU(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateDebugTrap(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateDispatchId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateDynWaveId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateLaneId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateMaxDynWaveId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNDRangeGroups(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNDRangeSize(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNop(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateNullPtr(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkDim(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkGroupId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkGroupSize(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkItemAId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkItemAIdFlat(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkItemId(const inst_iterator inst) const{
  return true;
}

bool BrigModule::validateWorkItemIdFlat(const inst_iterator inst) const{
  return true;
}

} // namespace brig
} // namespace hsa
