#include "brig_module.h"
#include "brig_inst_helper.h"
#include "llvm/Support/raw_ostream.h"
#include <cstring>
#include <set>

namespace hsa {
namespace brig {

#define check(X,Y) check(X, Y, __FILE__, __LINE__, #X)

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
  if(valid) valid &= validateInstructions();
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
  valid &= check(!dir->reserved, "Reserved not zero");

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
  valid &= check(dir->order < BrigImageOrderInvalid, "Invalid image type");
  valid &= check(dir->format < BrigImageFormatInvalid, "Invalid format type");
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
    valid &= check(dir->reserved == 0 ,
                   "The value of reserved must be zero");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabel *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelList *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  const dir_iterator label(S_.directives + dir->label);
  if(!validate(label)) return false;
  if(!check(dyn_cast<BrigDirectiveLabel>(label),
            "label of a label list is not a label"))
    return false;
  for (unsigned i = 0; i < dir->elementCount; i++) {
    const dir_iterator init(S_.directives + dir->d_labels[i]);
    if(!validate(init)) return false;
    const BrigDirectiveLabel *bcl = dyn_cast<BrigDirectiveLabel>(init);
    if(!check(bcl, "d_labels offset is wrong, not a BrigDirectiveLabel"))
      return false;
  }
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
  BrigDataType type = BrigDataType(dir->type);
  size_t size = sizeof(BrigDirectiveInit) - sizeof(uint64_t) +
    dir->elementCount * BrigInstHelper::getTypeSize(type) / 8;
  valid &= check(size <= dir->size,
                 "Directive size too small for elementCount");

  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelInit *dir) const {
  bool valid = true;
  if(!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  for (unsigned i = 0; i < dir->elementCount; i++) {
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
  BrigDataType type = BrigDataType(dir->type);
  size_t size = sizeof(BrigBlockNumeric) - sizeof(uint64_t) +
    dir->elementCount * BrigInstHelper::getTypeSize(type) / 8;
  valid &= check(size <= dir->size,
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
  if(!c->valid)
    return true;

  if(c->approx == 1)
    valid &= check(c->floatOrInt == 1, "Invalid floatOrInt");

  if(c->floatOrInt == 0)
    valid &= check(c->ftz == 0, "Invalid ftz");

  valid &= check(c->reserved == 0, "Invalid reserved");
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
  BrigDataType type = BrigDataType(operand->type);
  size_t immediateSize = sizeof(BrigOperandImmed) - 2 * sizeof(uint64_t) +
    BrigInstHelper::getTypeSize(type) / 8;
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
  dir_iterator nameDir(S_.directives + operand->directive);
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
  if(!validateSName(operand->s_name))
    return false;

  const char *name = S_.strings + operand->s_name;

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

static unsigned getNumOperands(const inst_iterator inst) {
  for(unsigned i = 0; i < 5; ++i) {
    if(!inst->o_operands[i])
      return i;
  }
  return 5;
}

static const BrigDataType *getType(const oper_iterator oper) {

  if(const BrigOperandAddress *address = dyn_cast<BrigOperandAddress>(oper))
    return (const BrigDataType *) &address->type;

  if(const BrigOperandCompound *compound = dyn_cast<BrigOperandCompound>(oper))
    return (const BrigDataType *) &compound->type;

  if(const BrigOperandImmed *immed = dyn_cast<BrigOperandImmed>(oper))
    return (const BrigDataType *) &immed->type;

  if(const BrigOperandIndirect *indirect = dyn_cast<BrigOperandIndirect>(oper))
    return (const BrigDataType *) &indirect->type;

  if(const BrigOperandReg *reg = dyn_cast<BrigOperandReg>(oper))
    return (const BrigDataType *) &reg->type;

  if(const BrigOperandRegV2 *regV2 = dyn_cast<BrigOperandRegV2>(oper))
    return (const BrigDataType *) &regV2->type;

  if(const BrigOperandRegV4 *regV4 = dyn_cast<BrigOperandRegV4>(oper))
    return (const BrigDataType *) &regV4->type;

  return NULL;
}

static bool isCompatibleSrc(BrigDataType type, const oper_iterator oper) {
  if(isa<BrigOperandWaveSz>(oper))
    return
      BrigInstHelper::isSignedTy(type)   ||
      BrigInstHelper::isUnsignedTy(type) ||
      BrigInstHelper::isBitTy(type);

  const BrigDataType *srcTy = getType(oper);
  if(!srcTy) return false;

  const size_t instSize = BrigInstHelper::getTypeSize(type);
  const size_t srcSize = BrigInstHelper::getTypeSize(*srcTy);
  return instSize == srcSize;
}

bool BrigModule::validateArithmeticInst(const inst_iterator inst,
                                        unsigned nary) const {

  bool valid = true;

  // Unary arithmetic requires signed types
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(BrigInstHelper::isSignedTy(type) ||
                 BrigInstHelper::isUnsignedTy(type) ||
                 BrigInstHelper::isFloatTy(type) ||
                 BrigInstHelper::isBitTy(type),
                 "Invalid type");

  valid &= check(isa<BrigInstBase>(inst) || isa<BrigInstMod>(inst),
                 "Incorrect instruction kind");

  if(!check(getNumOperands(inst) == nary + 1, "Incorrect number of operands"))
    return false;

  oper_iterator dest(S_.operands + inst->o_operands[0]);
  const BrigOperandReg *destReg = dyn_cast<BrigOperandReg>(dest);
  valid &= check(destReg, "Destination must be a register");

  for(unsigned i = 0; i < nary; ++i) {
    oper_iterator src(S_.operands + inst->o_operands[i + 1]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWaveSz>(src),
                   "Source must be a register, immediate, or wave size");

    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  }

  BrigPacking packing = BrigPacking(inst->packing);
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(BrigInstHelper::isFloatTy(type),
                   "BrigInstMod is only valid for floating point");
    if(mod->aluModifier.valid) {
      valid &= check(!mod->aluModifier.approx,
                     "Incompatible ALU modifier");
      valid &= check(!mod->aluModifier.fbar,
                     "Incompatible ALU modifier");
      valid &= check(packing == BrigNoPacking,
                     "Packed operations cannot accept ALU modifiers");
    }
  }

  if(BrigInstHelper::isFloatTy(type))
    valid &= check(!BrigInstHelper::isSaturated(packing),
                   "Floating point arithmetic cannot saturate");

  if(BrigInstHelper::isVectorTy(type)) {
    valid &= check(BrigInstHelper::isValidPacking(packing, nary),
                   "Vectors must have a packing");
  } else {
    valid &= check(packing == BrigNoPacking,
                   "Non-vectors must not have a packing");
  }

  valid &= check(BrigInstHelper::getTypeSize(type) <= 64, "Illegal data type");

  valid &= check(BrigInstHelper::getTypeSize(type) <=
                 BrigInstHelper::getTypeSize(BrigDataType(destReg->type)),
                 "Destination register is too small");

  return valid;
}

bool BrigModule::validateShiftInst(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(BrigInstHelper::isSignedTy(type) ||
                 BrigInstHelper::isUnsignedTy(type),
                 "Type is only valid for signed and unsigned point types");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");

  oper_iterator src0(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0) ||
                 isa<BrigOperandWaveSz>(src0),
                 "Source must be a register, immediate, or wave size");
  if(isa<BrigOperandReg>(src0) || isa<BrigOperandImmed>(src0))
    valid &= check(isCompatibleSrc(type, src0), "Incompatible source operand");

  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1) ||
                 isa<BrigOperandWaveSz>(src1),
                 "Source must be a register, immediate, or wave size");
  if(isa<BrigOperandReg>(src1) || isa<BrigOperandImmed>(src1))
    valid &= check(*getType(src1) == Brigb32, "Type of src1 should be Brigb32");

  if(BrigInstHelper::isVectorTy(type)) {
    valid &= check(inst->type == Brigu8x4 || inst->type == Brigu16x2 ||
                   inst->type == Brigs8x4 || inst->type == Brigs16x2 ||
                   inst->type == Brigu8x8 || inst->type == Brigu16x4 ||
                   inst->type == Brigs8x8 || inst->type == Brigs16x4 ||
                   inst->type == Brigs32x2 || inst->type == Brigu32x2,
                   "Length should be 8x4, 8x8, 16x2, 16x4 or 32x2");

    valid &= check(inst->packing == BrigPackPS,
                   "Packing should be BrigPackPS");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(type) == 32 ||
                   BrigInstHelper::getTypeSize(type) == 64,
                   "If regular form, length should be 32 or 64");

    valid &= check(inst->packing == BrigNoPacking,
                   "Packing should be BrigNoPacking");
  }
  return valid;
}

bool BrigModule::validateMovdInst(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == Brigb64, "Type of Movd should be Brigb64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Movd cannot accept vector types");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(*getType(dest) == Brigb64,
                 "Destination should be a d register");
  oper_iterator src0(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src0), "Src0 should be register");
  valid &= check(*getType(src0) == Brigb64,
                 "Src0 should be a d register");
  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(isa<BrigOperandReg>(src1), "Src0 should be register");
  valid &= check(*getType(src1) == Brigb32,
                 "Src1 should be a s register");
  return valid;
}

bool BrigModule::validateMovsInst(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Movs should be Brigb32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Movs cannot accept vector types");
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(*getType(dest) == Brigb32,
                 "Destination should be a s register");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src), "Src0 should be register");
  valid &= check(*getType(src) == Brigb64,
                 "Destination should be a d register");
  return valid;
}

bool BrigModule::validateUnpackInst(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;

  BrigDataType type = BrigDataType(inst->type);
  valid &= check(inst->type== Brigu16x2 || inst->type== Brigs16x2 ||
                 inst->type==  Brigf16x2 || inst->type== Brigu16x4 ||
                 inst->type== Brigs16x4 || inst->type== Brigf16x4 ||
                 inst->type==  Brigu32x2 ||  inst->type== Brigs32x2 ||
                 inst->type== Brigf32x2 || inst->type== Brigu8x8 ||
                 inst->type== Brigs8x8 || inst->type== Brigu8x4 ||
                 inst->type== Brigs8x4,
                 "Length of Unpack should be 8x4, 8x8, 16x2, 16x4 or 32x2");

  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  oper_iterator src0(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0), "Src should be reg or immed");
  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1), "Src should be reg or immed");

  for(int i = 0; i < 3; i++) {
    oper_iterator reg(S_.operands + inst->o_operands[i]);
    valid &= check(isCompatibleSrc(type, reg), "Incompatible source operand");
  }

  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Unpack should accept vector types");
  valid &= check(inst->packing == BrigNoPacking,
                 "The packing should be set to BrigNoPacking");

  return valid;
}

bool BrigModule::validateLdStImageInst(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstImage *image = dyn_cast<BrigInstImage>(inst);
  if(!validate(image)) return false;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  BrigDataType destType = BrigDataType(image->type);
  valid &= check(destType == Brigu32 ||
                 destType == Brigf32 ||
                 destType == Brigs32,
                 "Type of destination should be u32, f32 or s32");
  BrigDataType srcType = BrigDataType(image->stype);
  valid &= check(srcType == Brigu32, "Type of source should be u32");
  oper_iterator dest(S_.operands + image->o_operands[0]);
  valid &= check(isa<BrigOperandRegV4>(dest), "Destination should be regV4");
  valid &= check(*getType(dest) == Brigb32,
		 "Destination should be s register");
  oper_iterator src0(S_.operands + image->o_operands[1]);
  valid &= check(isa<BrigOperandOpaque>(src0), "Source should be opaque");
  const BrigOperandOpaque *opaque = dyn_cast<BrigOperandOpaque>(src0);
  if(!validate(opaque)) return false;
  dir_iterator direc(S_.directives + opaque->directive);
  if(const BrigDirectiveImage *dir = dyn_cast<BrigDirectiveImage>(direc))
    valid &= check(dir->s.type == BrigROImg || dir->s.type == BrigRWImg,
                   "Type should be read-write or read-only image");
  if(const BrigDirectiveSampler *dir = dyn_cast<BrigDirectiveSampler>(direc))
    valid &= check(dir->s.type == BrigROImg || dir->s.type == BrigRWImg,
                   "Type should be read-write or read-only image");
  oper_iterator src1(S_.operands + image->o_operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandRegV2>(src1) ||
                 isa<BrigOperandRegV4>(src1),
                 "Source should be reg, regV2 or regV4");
  valid &= check(*getType(src1) == Brigb32, "Source should be s register");
  valid &= check(!BrigInstHelper::isVectorTy(destType),
                 "LdSt can not accept vector types");
  return valid;
}

bool BrigModule::validateImageQueryInst(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(type == Brigb32 || type == Brigu32,
                 "Type should be b32 or u32");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandOpaque>(src),
                 "Source should be BrigOperandOpaque");
  const BrigOperandOpaque *opaque = dyn_cast<BrigOperandOpaque>(src);
  if(!validate(opaque)) return false;
  dir_iterator direc(S_.directives + opaque->directive);
  if(const BrigDirectiveImage *dir = dyn_cast<BrigDirectiveImage>(direc))
    valid &= check(dir->s.type == BrigROImg ||
                   dir->s.type == BrigRWImg ||
                   dir->s.type == BrigSamp,
                   "Type should be image object or sampler object");

  if(const BrigDirectiveSampler *dir = dyn_cast<BrigDirectiveSampler>(direc))
    valid &= check(dir->s.type == BrigROImg ||
                   dir->s.type == BrigRWImg ||
                   dir->s.type == BrigSamp,
                   "Type should be image object or sampler object");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Image can not accept vector types");
  return valid;
}

bool BrigModule::validateParaSynInst(const inst_iterator inst,
				     unsigned nary) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 1 + nary, "Incorrect number of operands"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(type == Brigb64 ||
                 type == Brigu32 ||
                 type == Brigb32,
                 "Type should be b64, u32 or b32");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  for(unsigned i = 0; i < nary; ++i) {
    oper_iterator src(S_.operands + inst->o_operands[i + 1]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWaveSz>(src),
                   "Source shoule be register, immediate or waveSz");
    valid &= check(isCompatibleSrc(type, src), "Incompatible Source operand");
  }
  valid &= check(BrigInstHelper::isVectorTy(type),
                 "ParaSynInst can not accept vector types");
  return valid;
}

bool BrigModule::validateAbs(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Abs is not valid for bit types");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "Abs does not support rounding");
    valid &= check(mod->aluModifier.ftz == 0,
                   "Abs does not support ftz");
  }
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateAdd(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Add is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateBorrow(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Borrow is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Borrow cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateCarry(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Carry is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Carry cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateCopySign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigDataType(inst->type)),
                 "CopySign is only valid for floating point types");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "CopySign does not support rounding");
    valid &= check(mod->aluModifier.ftz == 0,
                   "CopySign does not support ftz");
  }
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "CopySign cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateDiv(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Div is not valid for bit types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Div cannot accept vector types");
  valid &= check(!BrigInstHelper::isSaturated(BrigPacking(inst->packing)),
                 "Div cannot saturate");
  valid &= validateArithmeticInst(inst, 2);
  return valid;;
}

bool BrigModule::validateFma(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigDataType(inst->type)),
                 "Fma is only valid for floating point type");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Fma cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateFract(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigDataType(inst->type)),
                 "Fract is only valid for floating point type");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "Fract does not support rounding");
  }
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Fract cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateMad(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Mad is not valid for bit types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mad cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMax(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Max is not valid for bit types");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "Max does not support rounding");
  }
  valid &= check(!BrigInstHelper::isSaturated(BrigPacking(inst->packing)),
                 "Max cannot saturate");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMin(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Min is not valid for bit types");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "Min does not support rounding");
  }
  valid &= check(!BrigInstHelper::isSaturated(BrigPacking(inst->packing)),
                 "Min cannot saturate");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMul(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Mul is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMulHi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "MulHi is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isSaturated(BrigPacking(inst->packing)),
                 "MulHi cannot saturate");
  if(!BrigInstHelper::isVectorTy(BrigDataType(inst->type))) {
    valid &= check(inst->type == Brigu32 || inst->type == Brigs32,
                   "MulHi should be u32 and s32");
  }
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateNeg(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isFloatTy(BrigDataType(inst->type)),
                 "Neg is only valid for signed and floating point types");
  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    valid &= check(mod->aluModifier.rounding == 0,
                   "Neg does not support rounding");
  }
  valid &= check(!BrigInstHelper::isSaturated(BrigPacking(inst->packing)),
                 "Neg cannot saturate");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateRem(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBase>(inst), "Rem must be BrigInstBase");
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Rem is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Rem cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateSqrt(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigDataType(inst->type)),
                 "Sqrt is only valid for floating point types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Sqrt cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateSub(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigDataType(inst->type)),
                 "Sub is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMad24(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Mad24 is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mad24 cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMad24Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Mad24Hi is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mad24Hi cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMul24(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Mul24 is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mul24 cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMul24Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigDataType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigDataType(inst->type)),
                 "Mul24Hi is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mul24Hi cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateShl(const inst_iterator inst) const {
  return validateShiftInst(inst);
}

bool BrigModule::validateShr(const inst_iterator inst) const {
  return validateShiftInst(inst);
}

bool BrigModule::validateAnd(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                 inst->type == Brigb64,
                 "Type of And should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "And cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateNot(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                 inst->type == Brigb64,
                 "Type of Not should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Not cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateOr(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                 inst->type == Brigb64,
                 "Type of Or should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Or cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validatePopCount(const inst_iterator inst) const {
  bool valid = true;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(inst->type == Brigb32 || inst->type ==Brigb64,
                 "Type of PopCount shoud be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "PopCount cannot accept vector types");
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;

  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(*getType(dest) == Brigb32, "Type Destination of PopCount "
                 "must be Brigb32");

  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be reg, immediate or WaveSz");

  if(isa<BrigOperandReg>(src) || isa<BrigOperandImmed>(src))
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");

  return valid;
}

bool BrigModule::validateXor(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                 inst->type == Brigb64,
                 "Type of Xor should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Xor cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateBitRev(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of BitRev should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "BitRev cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateBitSelect(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of BitSelect should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "BitSelect cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateExtract(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of Extract should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Extract cannot accept vector types");
  if(!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;

  BrigDataType type = BrigDataType(inst->type);
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");

  for(int i = 1; i < 4; i++) {
    oper_iterator src(S_.operands + inst->o_operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWaveSz>(src),
                   "Source should be reg, immediate or WaveSz");
  }

  oper_iterator src0(S_.operands + inst->o_operands[1]);
  if(isa<BrigOperandReg>(src0) || isa<BrigOperandImmed>(src0))
    valid &= check(isCompatibleSrc(type, src0), "Incompatible source operand");

  oper_iterator src1(S_.operands + inst->o_operands[2]);
  if(isa<BrigOperandReg>(src1) || isa<BrigOperandImmed>(src1))
    valid &= check(*getType(src1) == Brigb32,"Type src1 of Extract "
                   "should be b32");

  oper_iterator src2(S_.operands + inst->o_operands[3]);
  if(isa<BrigOperandReg>(src2) || isa<BrigOperandImmed>(src2))
    valid &= check(*getType(src2) == Brigb32,"Type src2 of Extract "
                   "should be b32");
  return valid;
}

bool BrigModule::validateFirstBit(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of FirstBit should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "FirstBit cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateInsert(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of Insert should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Insert cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateLastBit(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Type of LastBit should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "LastBit cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateLda(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type ==Brigu32 || inst->type == Brigu64,
                 "Length should be 32 or 64");
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Lda cannot accept vector types");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be BrigOperandReg");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandAddress>(src) ||
                 isa<BrigOperandIndirect>(src) ||
                 isa<BrigOperandCompound>(src),
                 "Src should be BrigOperandAddress, BrigOperandIndirect "
                 "and BrigOPerandCompound");
  return valid;
}

bool BrigModule::validateLdc(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == Brigb32 || inst->type == Brigb64,
                 "Length should be 32 or 64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Ldc cannot accept vector types");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be BrigOperandReg");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandLabelRef>(src) ||
                 isa<BrigOperandFunctionRef>(src),
                 "Src should be LabelRef and FunctionRef");
  if(isa<BrigOperandLabelRef>(src))
    valid &= check(*getType(dest) == Brigb32,
                   "Type of dest should be b32 if the source is label");
  if(isa<BrigOperandFunctionRef>(src)) {
    const dir_iterator version(S_.directives + 8);
    const BrigDirectiveVersion *bdv = dyn_cast<BrigDirectiveVersion>(version);
    if(!validate(bdv)) return false;
    if(bdv->machine == BrigELarge)
      valid &= check(*getType(dest) == Brigb64,
                     "Type of dest should be b64 if machine model is large");
    if(bdv->machine == BrigESmall)
      valid &= check(*getType(dest) == Brigb32,
                     "Type of dest should be b32 if machine model is small");
  }
  return valid;
}

bool BrigModule::validateMov(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                 inst->type == Brigb64 || inst->type == Brigb128,
                 "Length should be 1, 32, 64 or 128");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Mov cannot accept vector types");
  BrigDataType type = BrigDataType(inst->type);
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest) ||
                 isa<BrigOperandRegV2>(dest) ||
                 isa<BrigOperandRegV4>(dest),
                 "Destination of Mov should be reg, regv2 or regv4");

  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandRegV2>(src) ||
                 isa<BrigOperandRegV4>(src) ||
                 isa<BrigOperandImmed>(src),
                 "Src of Mov should be reg, regv2, regv4 or immediate");

  if(isa<BrigOperandReg>(dest))
    valid &= check(isCompatibleSrc(type, dest),
                   "Incompatible destination operand");

  if(isa<BrigOperandReg>(src))
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");

  if(isa<BrigOperandRegV2>(dest))
    valid &= check(isa<BrigOperandReg>(src) &&
                   *getType(src) == Brigb64 &&
                   *getType(dest) == Brigb32,
                   "Src should point to d reg and type of dest should be b32");

  if(isa<BrigOperandRegV2>(src))
    valid &= check(isa<BrigOperandReg>(dest) &&
                   *getType(dest) == Brigb64 &&
                   *getType(src) == Brigb32,
                   "Dest should point to d reg and type of src should be b32");

  if(isa<BrigOperandRegV4>(dest))
    valid &= check(isa<BrigOperandReg>(src) &&
                   *getType(src) == Brigb128 &&
                   *getType(dest) == Brigb32,
                   "Src should point to q reg and type of dest should be b32");

  if(isa<BrigOperandRegV4>(src))
    valid &= check(isa<BrigOperandReg>(dest) &&
                   *getType(dest) == Brigb128 &&
                   *getType(src) == Brigb32,
                   "Dest should point to d reg and type of src should be b32");
  return valid;
}

bool BrigModule::validateMovdHi(const inst_iterator inst) const {
  return validateMovdInst(inst);;
}

bool BrigModule::validateMovdLo(const inst_iterator inst) const {
  return validateMovdInst(inst);;
}

bool BrigModule::validateMovsHi(const inst_iterator inst) const {
  return validateMovsInst(inst);;
}

bool BrigModule::validateMovsLo(const inst_iterator inst) const {
  return validateMovsInst(inst);;
}

bool BrigModule::validateShuffle(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(inst->type== Brigu16x2 || inst->type== Brigs16x2 ||
                 inst->type==  Brigf16x2 || inst->type== Brigu16x4 ||
                 inst->type== Brigs16x4 || inst->type== Brigf16x4 ||
                 inst->type==  Brigu32x2 ||  inst->type== Brigs32x2 ||
                 inst->type== Brigf32x2 || inst->type== Brigu8x8 ||
                 inst->type== Brigs8x8 || inst->type== Brigu8x4 ||
                 inst->type== Brigs8x4,
                 "Length of Shuffle should be 8x4, 8x8, 16x2, 16x4 or 32x2");

  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  oper_iterator src0(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0), "Src0 should be reg or immed");
  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1), "Src1 should be reg or immed");
  oper_iterator src2(S_.operands + inst->o_operands[3]);
  valid &= check(isa<BrigOperandImmed>(src2), "Src2 should be immed");

  for(int i = 0; i < 4; i++) {
    oper_iterator reg(S_.operands + inst->o_operands[i]);
    valid &= check(isCompatibleSrc(type, reg), "Incompatible operand");
  }

  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Shuffle should accept vector types");
  valid &= check(inst->packing == BrigNoPacking,
                 "The packing should be set to BrigNoPacking");
  return valid;
}

bool BrigModule::validateUnpackHi(const inst_iterator inst) const {
  return validateUnpackInst(inst);
}

bool BrigModule::validateUnpackLo(const inst_iterator inst) const {
  return validateUnpackInst(inst);
}

bool BrigModule::validateCmov(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;

  BrigDataType type = BrigDataType(inst->type);
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");

  oper_iterator src0(S_.operands + inst->o_operands[1]);

  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(isCompatibleSrc(type, src1), "Incompatible src1 operand");

  oper_iterator src2(S_.operands + inst->o_operands[3]);
  valid &= check(isCompatibleSrc(type, src2), "Incompatible src2 operand");

  for(int i = 1; i < 4; i++) {
    oper_iterator src(S_.operands + inst->o_operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src),
                   "Source should be reg or immdiate");
  }

  if(BrigInstHelper::isVectorTy(type)) {
    valid &= check(BrigInstHelper::isSignedTy(type)   ||
                   BrigInstHelper::isUnsignedTy(type) ||
                   BrigInstHelper::isFloatTy(type),
                   "Invalid type");
    valid &= check(isCompatibleSrc(type, src0), "Incompatible src0 operand");
    valid &= check(inst->packing == BrigPackPP, "Packing should be pp");
  } else {
    valid &= check(inst->type == Brigb1 || inst->type == Brigb32 ||
                   inst->type == Brigb64, "Type should be b1, b32 and b64");
    valid &= check(*getType(src0) == Brigb1, "Type of Src0 should be b1");
    valid &= check(inst->packing == BrigNoPacking,
                   "Packing should be nopacking");
  }
  return valid;
}

bool BrigModule::validateClass(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == Brigf32 || inst->type == Brigf64,
                 "Type of Class should be f32 or f64");

  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be a register");
  valid &= check(*getType(dest) == Brigb1, "Destination should be a c reg");

  for(int i = 1; i < 3; i++) {
    oper_iterator src(S_.operands + inst->o_operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(dest),
                   "Source should be register or immediate");
  }

  BrigDataType type = BrigDataType(inst->type);
  oper_iterator src0(S_.operands + inst->o_operands[1]);
  valid &= check(isCompatibleSrc(type, src0), "Incompatible src0 operand");

  oper_iterator src1(S_.operands + inst->o_operands[2]);
  valid &= check(*getType(src1) == Brigb32, "Type of src1 should be Brigb32");

  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Class can not accept the vector");
  return valid;
}

bool BrigModule::validateFcos(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Fcos can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateFexp2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Fexp2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateFlog2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Flog2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateFrcp(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32 || inst->type == Brigf64,
                 "Type should be f32 or f64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Frcp can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateFrsqrt(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32 || inst->type == Brigf64,
                 "Type should be f32 or f64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Frsqrt can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateFsin(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigf32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Fsin can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateBitAlign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of BitAlign should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "BitAlign can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateByteAlign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of ByteAlign should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "ByteAlign can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateF2u4(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigu32, "Type of F2u4 should be u32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "F2u4 can not accept vector types");
  valid &= validateArithmeticInst(inst, 4);
  return valid;
}

bool BrigModule::validateLerp(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Lerp should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Lerp can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateSad(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Sad should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Sad can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateSad2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Sad2 should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Sad2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateSad4(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Sad4 should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Sad4 can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateSad4Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of Sad4Hi should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigDataType(inst->type)),
                 "Sad4Hi can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateUnpack0(const inst_iterator inst) const {
  return validateArithmeticInst(inst, 1);
}

bool BrigModule::validateUnpack1(const inst_iterator inst) const {
  return validateArithmeticInst(inst, 1);
}

bool BrigModule::validateUnpack2(const inst_iterator inst) const {
  return validateArithmeticInst(inst, 1);
}

bool BrigModule::validateUnpack3(const inst_iterator inst) const {
  return validateArithmeticInst(inst, 1);
}

bool BrigModule::validateSegmentp(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstMem *mem = dyn_cast<BrigInstMem>(inst);
  if(!validate(mem)) return false;
  BrigDataType type = BrigDataType(mem->type);
  valid &= check(type == Brigb1, "Type of Segmentp should be b1");
  valid &= check(mem->storageClass <= 6, "StorageClass should be global, "
                "group, private, kernarg, readonly, spill, or arg");
  oper_iterator dest(S_.operands + mem->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + mem->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be reg, immediate or waveSz");
  valid &= check(inst->type == Brigb32, "Type should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Segmentp can not accept vector types");
  return valid;
}

bool BrigModule::validateFtoS(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstMem *mem = dyn_cast<BrigInstMem>(inst);
  if(!validate(mem)) return false;
  valid &= check(mem->type == Brigu32 || mem->type == Brigu64,
                 "Type of FtoS should be u32 or u64");
  BrigDataType type = BrigDataType(mem->type);
  oper_iterator dest(S_.operands + mem->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + mem->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be reg, immediate or waveSz");
  valid &= check(isCompatibleSrc(type, src), "Incompatible Source operand");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "FtoS can not accept vector types");
  return valid;
}

bool BrigModule::validateStoF(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstMem *mem = dyn_cast<BrigInstMem>(inst);
  if(!validate(mem)) return false;
  valid &= check(mem->type == Brigu32 || mem->type == Brigu64,
                 "Type of StoF should be u32 or u64");
  BrigDataType type = BrigDataType(mem->type);
  oper_iterator dest(S_.operands + mem->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + mem->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be reg, immediate or waveSz");

  valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "StoF can not accept vector types");
  return valid;
}

bool BrigModule::validateCmp(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "Cmp may not have an aluModifier");
  const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst);
  if(!validate(cmp)) return false;
  BrigDataType type = BrigDataType(cmp->type);
  valid &= check(type == Brigb1  || type == Brigb32 || type == Brigu32 ||
                 type == Brigs32 || type == Brigf32 || type == Brigf16 ||
                 type == Brigs16 || type == Brigu16 || type == Brigb16,
                 "Invalid destination type");
  BrigDataType srcTy = BrigDataType(cmp->sourceType);
  valid &= check(srcTy == Brigb1  || srcTy == Brigb32 || srcTy == Brigu32 ||
                 srcTy == Brigs32 || srcTy == Brigf32 || srcTy == Brigb64 ||
                 srcTy == Brigu64 || srcTy == Brigs64 || srcTy == Brigf64 ||
                 srcTy == Brigf16 || srcTy == Brigs16 || srcTy == Brigu16 ||
                 srcTy == Brigb16, "Invalid source type");
  oper_iterator dest(S_.operands + cmp->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  for(int i = 1; i < 3; i++) {
    oper_iterator src(S_.operands + cmp->o_operands[i]);
    if(BrigInstHelper::isFloatTy(type)) {
      valid &= check(isa<BrigOperandReg>(src) ||
                     isa<BrigOperandImmed>(src),
                     "Source should be reg or immediate");
    } else {
      valid &= check(isa<BrigOperandReg>(src)   ||
                     isa<BrigOperandImmed>(src) ||
                     isa<BrigOperandWaveSz>(src),
                     "Source should be reg, immediate or waveSz");
    }

    valid &= check(isCompatibleSrc(srcTy, src), "Incompatible source operand");
  }
  if(BrigInstHelper::isUnsignedTy(srcTy) || BrigInstHelper::isSignedTy(srcTy))
    valid &= check(cmp->comparisonOperator <= 5, "Invalid comparisonOperator");

  if(BrigInstHelper::isFloatTy(srcTy))
    valid &= check(cmp->comparisonOperator <= 27, "Invalid comparisonOperator");

  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Cmp can not accept vector types");
  return valid;
}

bool BrigModule::validatePackedCmp(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "PackedCmp may not have an aluModifier");
  const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst);
  if(!validate(cmp)) return false;
  BrigDataType type = BrigDataType(cmp->type);
  valid &= check(type == Brigu8x4  || type == Brigs8x4  ||
                 type == Brigu8x8  || type == Brigs8x8  ||
                 type == Brigu16x2 || type == Brigs16x2 || type == Brigf16x2 ||
                 type == Brigu16x4 || type == Brigs16x4 || type == Brigf16x4 ||
                 type == Brigu32x2 || type == Brigs32x2 || type == Brigf32x2,
                 "Invalid type");
  oper_iterator dest(S_.operands + cmp->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  valid &= check(isCompatibleSrc(type, dest),
		  "Incompatible destination oper;and");
  for(int i = 1; i < 3; i++) {
    oper_iterator src(S_.operands + cmp->o_operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed> (src),
                   "Source should be reg or immediate");
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  }

  if(BrigInstHelper::isUnsignedTy(type) || BrigInstHelper::isSignedTy(type))
    valid &= check(cmp->comparisonOperator <= 5, "Invalid comparisonOperator");

  if(BrigInstHelper::isFloatTy(type))
    valid &= check(cmp->comparisonOperator <= 27, "Invalid comparisonOperator");

  if(BrigInstHelper::isVectorTy(type)) {
    valid &= check(cmp->packing == BrigPackPP, "Packing should be pp");
  } else {
    valid &= check(cmp->packing == BrigNoPacking,
                   "Packing should be noPacking");
  }
  return valid;
}

bool BrigModule::validateCvt(const inst_iterator inst) const {
   bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "Cvt may not have an aluModifier");
  const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst);
  if(!validate(cvt)) return false;

  BrigDataType type = BrigDataType(cvt->type);
  valid &= check(BrigInstHelper::isBitTy(type)      ||
                 BrigInstHelper::isUnsignedTy(type) ||
                 BrigInstHelper::isSignedTy(type)   ||
                 BrigInstHelper::isFloatTy(type),
                 "Type should be unsigned, signed, float or bit");

  if(BrigInstHelper::isBitTy(type)) {
    valid &= check(BrigInstHelper::getTypeSize(type) == 1,
                   "Illegal data length");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(type) <= 64,
                   "Illegal data length");
  }

  BrigDataType srcTy = BrigDataType(cvt->stype);
  valid &= check(BrigInstHelper::isBitTy(srcTy) ||
                 BrigInstHelper::isUnsignedTy(srcTy) ||
                 BrigInstHelper::isSignedTy(srcTy) ||
                 BrigInstHelper::isFloatTy(srcTy),
                 "Type should be unsigned, signed float or bit");
  if(BrigInstHelper::isBitTy(srcTy)) {
    valid &= check(BrigInstHelper::getTypeSize(srcTy) == 1,
                   "Illegal data length");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(srcTy) <= 64,
                   "Illegal data length");
  }

  oper_iterator dest(S_.operands + cvt->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");

  if(BrigInstHelper::getTypeSize(type) == 8 ||
     BrigInstHelper::getTypeSize(type) == 16) {
    valid &= check(*getType(dest) == Brigb32,
                   "Destination should be s register");
  } else {
    valid &= check(isCompatibleSrc(type, dest),
                   "Incompatible destination operand");
  }

  oper_iterator src(S_.operands + cvt->o_operands[1]);
  if(BrigInstHelper::isFloatTy(type)) {
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src),
                   "Source should be reg or immediate");
  } else {
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWaveSz>(src),
                   "Source should be reg, immediate or waveSz");
  }

  if(BrigInstHelper::getTypeSize(srcTy) == 8 ||
     BrigInstHelper::getTypeSize(srcTy) == 16) {
    valid &= check(*getType(src) == Brigb32,
                   "Source should be s register");
  } else {
    valid &= check(isCompatibleSrc(srcTy, src), "Incompatible source operand");
  }

  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Cvt can not accept vector types");

  return valid;
}

static bool isPowerOf2(const uint32_t immed) {
  return !(immed & (immed - 1));
}

bool BrigModule::validateLd(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateSt(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateAtomic(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateAtomicNoRet(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateRdImage(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateLdImage(const inst_iterator inst) const {
  return validateLdStImageInst(inst);
}

bool BrigModule::validateStImage(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstImage *image = dyn_cast<BrigInstImage>(inst);
  if(!validate(image)) return false;
  oper_iterator src0(S_.operands + image->o_operands[1]);
  const BrigOperandOpaque *opaque = dyn_cast<BrigOperandOpaque>(src0);
  if(!validate(opaque)) return false;
  dir_iterator direc(S_.directives + opaque->directive);
  if(const BrigDirectiveImage *dir = dyn_cast<BrigDirectiveImage>(direc))
    valid &= check(dir->s.type == BrigRWImg,
                   "Type should be read-write image");
  if(const BrigDirectiveSampler *dir = dyn_cast<BrigDirectiveSampler>(direc))
    valid &= check(dir->s.type == BrigRWImg,
                   "Type should be read-write image");
  valid &= validateLdStImageInst(inst);
  return valid;
}

bool BrigModule::validateAtomicImage(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateAtomicNoRetImage(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateQueryArray(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of QueryArray should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryData(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of QueryData should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryDepth(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigu32, "Type of QueryDepth should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryFiltering(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32,
                 "Type of QueryFiltering should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryHeight(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigu32, "Type of QueryHeight should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryNormalized(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32,
                 "Type of QueryNormalized should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryOrder(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type of QueryOrder should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryWidth(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigu32, "Type of QueryWidth should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateBranchInst(const inst_iterator inst, unsigned nary) const {

  bool valid = true;

  if(!check(isa<BrigInstBase>(inst) || isa<BrigInstMod>(inst),
            "Incorrect instruction kind"))
  return false;

  const unsigned numOperands = getNumOperands(inst);

  if(!check(numOperands == nary || numOperands == nary + 1, 
            "Incorrect number of operands"))
  return false;

  if(const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst)) {
    if(mod->aluModifier.valid) {
      valid &= check(!mod->aluModifier.floatOrInt,
                       "Incompatible ALU modifier");
      valid &= check(!mod->aluModifier.rounding,
                       "Incompatible ALU modifier");
      valid &= check(!mod->aluModifier.ftz,
                       "Incompatible ALU modifier");
      valid &= check(!mod->aluModifier.approx,
                       "Incompatible ALU modifier");
    }
  }

  oper_iterator number(S_.operands + inst->o_operands[0]);
  const BrigOperandImmed *widthMod = dyn_cast<BrigOperandImmed>(number);
  if(!check(widthMod, "o_operands[0] must be a BrigOperandImmed"))
    return false;
  valid &= check(BrigInstHelper::isBitTy(BrigDataType(widthMod->type)),
                   "Invalid type");
  valid &= check(32 == BrigInstHelper::getTypeSize(BrigDataType(widthMod->type)),
                   "Invalid type size");
  valid &= check(isPowerOf2(widthMod->bits.u), "Width must be a power of 2");

  if(nary == 3){
  oper_iterator dest(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(dest),
                   "Destination must be a Reg");
    valid &= check(1 == BrigInstHelper::getTypeSize(*getType(dest)),
                     "Invalid type size");
  }

  if(numOperands == nary) {
    oper_iterator labelOrReg (S_.operands + inst->o_operands[nary - 1]);
    valid &= check(isa<BrigOperandLabelRef>(labelOrReg)   ||
                     isa<BrigOperandReg>(labelOrReg),
                     "Destination must be a LabelRef or Reg");
  }

  if(numOperands == nary + 1){
    oper_iterator dest(S_.operands + inst->o_operands[nary - 1]);
    valid &= check(isa<BrigOperandReg>(dest),
                     "Destination must be a Reg");
    valid &= check(32 == BrigInstHelper::getTypeSize(*getType(dest)),
                     "Invalid type size");
    oper_iterator labelOrAddr(S_.operands + inst->o_operands[nary]);
    valid &= check(isa<BrigOperandLabelRef>(labelOrAddr)   ||
                     isa<BrigOperandAddress>(labelOrAddr),
                     "Destination must be a LabelRef or Address");
  }
  return valid;
}

bool BrigModule::validateCbr(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateBranchInst(inst, 3);
  return valid;
}

bool BrigModule::validateBrn(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateBranchInst(inst, 2);
  return valid;
}

bool BrigModule::validateBarrier(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 1, "Incorrect number of operands"))
    return false;
  const BrigInstBar *bib = dyn_cast<BrigInstBar>(inst);
  if(!validate(bib)) return false;
  BrigDataType type = BrigDataType(bib->type);
  oper_iterator dest(S_.operands + bib->o_operands[0]);
  valid &= check(isa<BrigOperandImmed>(dest),
                 "Destination should be immediate");
  valid &= check(type == Brigb32, "Type of dest should be b32");
  valid &= check(*getType(dest) == Brigb32, "Type of immediate should be b32");
  valid &= check(bib->syncFlags <= 4,
		 "SyncFlags should be global, group, or both");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Barrier can not accept vector types");
  return valid;
}

bool BrigModule::validateFbarArrive(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarInit(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb64, "Type should be b64");
  valid &= validateParaSynInst(inst, 1);
  return valid;
}

bool BrigModule::validateFbarRelease(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarSkip(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarWait(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateSync(const inst_iterator inst) const {
  if(!check(getNumOperands(inst) == 0, "Incorrect number of operands"))
    return false;
  return true;
}

bool BrigModule::validateCount(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operand"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(type == Brigu32, "Type should be u32");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be register, immediate or waveSz");
  valid &= check(*getType(src) == Brigb1 || *getType(src) == Brigb32,
                 "Source should be c or s register");
  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Count can not accept vector types");
  return valid;
}

bool BrigModule::validateCountUp(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigu32, "Type should be u32");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateMask(const inst_iterator inst) const {
  bool valid = true;
  if(!check(getNumOperands(inst) == 2, "Incorrect number of operand"))
    return false;
  BrigDataType type = BrigDataType(inst->type);
  valid &= check(type == Brigb64, "Type should be b64");
  oper_iterator dest(S_.operands + inst->o_operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + inst->o_operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWaveSz>(src),
                 "Source should be register, immediate or waveSz");
  valid &= check(*getType(src) == Brigb1 || *getType(src) == Brigb32,
                 "Source should be c or s register");
  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Mask can not accept vector types");
  return valid;
}

bool BrigModule::validateSend(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type should be b32");
  valid &= validateParaSynInst(inst, 2);
  return valid;
}

bool BrigModule::validateReceive(const inst_iterator inst) const { 
  bool valid = true;
  valid &= check(inst->type == Brigb32, "Type should be b32");
  valid &= validateParaSynInst(inst, 2);
  return valid;
}

bool BrigModule::validateCall(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateRet(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateSysCall(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateAlloca(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateClock(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateCU(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateDebugTrap(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateDispatchId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateDynWaveId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateLaneId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateMaxDynWaveId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateNDRangeGroups(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateNDRangeSize(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateNop(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateNullPtr(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkDim(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkGroupId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkGroupSize(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkItemAId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkItemAIdFlat(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkItemId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkItemIdFlat(const inst_iterator inst) const {
  return true;
}

} // namespace brig
} // namespace hsa
