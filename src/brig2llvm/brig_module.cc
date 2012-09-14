#include "brig_module.h"

#include "llvm/Support/raw_ostream.h"
#include <cstring>
#include <set>

namespace hsa {
namespace brig {

#define check(X,Y) check(X, Y, __FILE__, __LINE__, #X)

int getTypeSize(BrigDataType16_t type) {
  if (Brigb1 == type) {
    return 1;
  } else if (Brigb8 == type) {
    return 1;
  } else if (Brigb16 == type) {
    return 2;
  } else if (Brigb32 == type) {
    return 4;
  } else if (Brigb64 == type) {
    return 8;
  } else if (Brigb128 == type) {
    return 16;
  }
  return 0;
}

template<class Message>
bool (BrigModule::check)(bool test, const Message &msg,
                         const char *filename, unsigned lineno,
                         const char *cause) const {
  if(!test && out_)
     (*out_) << filename << "." << lineno << ": " << msg
            << " (" << cause << ")\n";
  return test;
}

bool BrigModule::validate(void) const {
  bool valid = true;
  valid &= validateDirectives();
  valid &= validateCode();
  valid &= validateOperands();
  valid &= validateStrings();
  return valid;
}

bool BrigModule::validateDirectives(void) const {

  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

  if(!validate(it)) return false;
  if(!check(it != E, "Empty directive section")) return false;

  // 20.8.22: The BrigDirectiveVersion directive must be the first directive
  // in the .directives section.
  const BrigDirectiveVersion *bdv = dyn_cast<BrigDirectiveVersion>(it);
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
    default: check(false, "Unrecognized directive");
    }
  }

  return true;
}

bool BrigModule::validateCode(void) const {

  inst_iterator it = S_.code_begin();
  const inst_iterator E = S_.code_end();

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
      default: check(false, "Unrecognized code");
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
      caseBrig(OperandReg);
      caseBrig(OperandRegV2);
      caseBrig(OperandRegV4);
      caseBrig(OperandWaveSz);
      default: check(false, "Unrecognized operands");
    }
  }

#undef caseBrig

  return true;
}

bool BrigModule::validateStrings(void) const {

  bool valid = true;

  std::set<std::string> stringSet;

  const char *curr = S_.strings;
  size_t maxLen = S_.stringsSize;

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

bool BrigModule::validate(const BrigDirectiveMethod *dir) const {
  bool valid = true;

  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);

  const unsigned paramCount = dir->inParamCount + dir->outParamCount;
  dir_iterator argIt = dir_iterator(dir) + 1;
  for(unsigned i = 0; i < paramCount; ++i, ++argIt) {
    if(!validate(argIt)) return false;
    const BrigDirectiveSymbol *bds = dyn_cast<BrigDirectiveSymbol>(argIt);
    valid &= check(bds, "Too few argument symbols");
    valid &= check(bds->s.storageClass == BrigArgSpace,
                   "Argument not in arg space");
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
    valid &= check(bdi || bdli, "Missing initializer");

    uint32_t elementCount = bdi ? bdi->elementCount : bdli->elementCount;
    valid &= check(elementCount == dir->s.dim,
                   "Inconsistent array dimensions");

    if(bdi)
      valid &= check(bdi->type == dir->s.type,
                     "Inconsistent array element type");
  }

  return valid;
}

bool BrigModule::validate(const BrigDirectiveImage *dir) const {
  bool valid = true;
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
  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelList *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

// 20.8.22
bool BrigModule::validate(const BrigDirectiveVersion *dir) const {
  bool valid = true;

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
     if (dir->types[i].hasDim == 1) {
       valid &= check(dir->types[i].dim, "dimension not set when hasDim is 1");
     }
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveFile *dir) const {
  bool valid = true;
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_filename);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveComment *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLoc *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveInit *dir) const {
  bool valid = true;
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
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  for (unsigned i = 0; i < dir->elementCount; i++) {
    valid &= check(dir->d_labels[i] < S_.directivesSize,
                   "d_labels past the directives section");

    const dir_iterator init(S_.directives + dir->d_labels[i]);
    if(!validate(init)) return false;
    const BrigDirectiveLabel *bcl = dyn_cast<BrigDirectiveLabel>(init);
    valid &= check(bcl, "d_labels offset is wrong, not a BrigDirectiveLabel");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveControl *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectivePragma *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveExtension *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgStart *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgEnd *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockStart *dir) const {
  bool valid = true;
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
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockEnd *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  return valid;
}

bool BrigModule::validate(const BrigDirectivePad *dir) const {
  bool valid = true;
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
  const uint8_t *dirOffset = reinterpret_cast<const uint8_t *>(dir);
  valid &= check((S_.directives - dirOffset) % alignment == 0,
                 "Improperly aligned directive");
  return valid;
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
  valid &= check(code->opcode == BrigAtomic ||
                 code->opcode == BrigAtomicNoRet,
                 "Invalid opcode, should be either BrigAtomic or "
                 "BrigAtomicNoRet");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
                 code->storageClass == BrigArgSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, or arg");
  valid &= check(code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigAcquireRelease ||
                 code->memorySemantic == BrigParAcquireRelease,
                 "Invalid memorySemantic, can be BrigAcquire, "
                 "BrigAcquireRelease, BrigParAcquireRelease");
  return valid;
}

bool BrigModule::validate(const BrigInstAtomicImage *code) const {
  bool valid = true;
  valid &= check(code->opcode == BrigAtomicImage ||
                 code->opcode == BrigAtomicNoRetImage,
                 "Invalid opcode, should be either BrigAtomicImage or "
                 "BrigAtomicNoRetImage");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                     "o_operands past the operands section");
    }
  }
  valid &= check(code->atomicOperation <= BrigAtomicSub,
                 "Invalid atomicOperation");
  valid &= check(code->storageClass == BrigGlobalSpace,
                 "Invalid storage class, must be global");
  valid &= check(code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigAcquireRelease ||
                 code->memorySemantic == BrigParAcquireRelease,
                 "Invalid memorySemantic, can be BrigAcquire, "
                 "BrigAcquireRelease, BrigParAcquireRelease");
  valid &= check(code->geom <= Briggeom_2da, "Invalid geom");
  return valid;
}

bool BrigModule::validate(const BrigInstBar *code) const {
  bool valid = true;
  valid &= check(code->opcode == BrigBarrier ||
                 code->opcode == BrigSync    ||
                 code->opcode == BrigBrn,
                 "Invalid opcode, should be either BrigBarrier, BrigSync or "
                 "BrigBrn");
  valid &= check(code->type <= Brigf64x2, "Invalid type");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
  valid &= check(code->opcode <= BrigFbarInitSizeKnown,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  return valid;
}

bool BrigModule::validate(const BrigInstCmp *code) const {
  bool valid = true;
  valid &= check(code->opcode == BrigCmp,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
  valid &= check(code->opcode == BrigRdImage,
                 "Invalid opcode");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
  valid &= check(code->opcode == BrigCvt,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
  valid &= check(code->opcode <= BrigFbarInitSizeKnown,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= check(code->storageClass <= BrigFlatSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, arg or flat");
  valid &= check(code->memorySemantic == BrigAcquire ||
                 code->memorySemantic == BrigAcquireRelease ||
                 code->memorySemantic == BrigParAcquireRelease,
                 "Invalid memorySemantic, can be BrigAcquire, "
                 "BrigAcquireRelease, BrigParAcquireRelease");
  valid &= check(code->equivClass < 64,
                 "Invalid equivClass, must less than 64");
  return valid;
}
bool BrigModule::validate(const BrigInstMem *code) const {
  bool valid = true;
  valid &= check(code->opcode <= BrigFbarInitSizeKnown,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
                 code->storageClass == BrigArgSpace,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, or arg");
    return valid;
}
bool BrigModule::validate(const BrigInstMod *code) const {
  bool valid = true;
  valid &= check(code->opcode <= BrigFbarInitSizeKnown,
                 "Invalid opcode");
  valid &= check(code->type <= Brigf64x2,
                 "Invalid type");
  valid &= check(code->packing <= BrigPackPsat,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
      valid &= check(code->o_operands[i] < S_.operandsSize,
                   "o_operands past the operands section");
    }
  }
  valid &= validate(&code->aluModifier);
  return valid;
}
bool BrigModule::validate(const BrigInstRead *code) const {
  bool valid = true;
  valid &= check(code->opcode == BrigRdImage,
                 "Invalid opcode");
  for (unsigned i = 0; i < 5; i++) {
    if (code->o_operands[i]) {
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
  valid &= check(operand->type == Brigb32 ||
                 operand->type == Brigb64, "Invald datatype, should be "
                 "Brigb32 and Brigb64");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  valid &= check(operand->directive < S_.directivesSize,
                 "directive past the directive section");
  dir_iterator dir(S_.directives + operand->directive);
  valid &= check(isa<BrigDirectiveSymbol>(dir),
                 "Invalid directive, should point to a BrigDirectiveSymbol");
  return valid;
}

bool BrigModule::validate(const BrigOperandArgumentList *operand) const {
  bool valid = true;
  if (operand->elementCount) {
    valid &= check(sizeof(BrigOperandArgumentList) + 
                   sizeof(operand->o_args[1]) *(operand->elementCount - 1) <= 
                   operand->size, "Invalid size");
    for (unsigned i = 0; i < operand->elementCount; i++) {
      oper_iterator argsOper(S_.operands + operand->o_args[i]);
      valid &= validate(argsOper);
      valid &= check(isa<BrigOperandArgumentRef>(argsOper), 
                     "Invalid o_args, should point BrigOperandArgumentRef");
    }
  }
  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionList *operand) const {
  return true;
}

bool BrigModule::validate(const BrigOperandArgumentRef *operand) const {
  bool valid = true;
  dir_iterator argDir(S_.directives + operand->arg);
  valid &= validate(argDir);
  valid &= check(isa<BrigDirectiveSymbol>(argDir), 
                 "Invalid reg, should be point BrigDirectiveSymbol");
  return valid;
}

bool BrigModule::validate(const BrigOperandBase *operand) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validate(const BrigOperandCompound *operand) const {
  bool valid = true;
  valid &= check(operand->type == Brigb32 ||
                 operand->type == Brigb64, "Invald datatype, should be " 
                 "Brigb32 and Brigb64");
  valid &= check(operand->reserved == 0,
                 "reserved must be zero");
  oper_iterator nameOper(S_.operands + operand->name);
  valid &= validate(nameOper);
  valid &= check(isa<BrigOperandAddress>(nameOper), 
                 "Invalid name, should point to BrigOperandAddress");

  if(operand->reg) {
    const oper_iterator oper(S_.operands + operand->reg);
    if(!validate(oper)) return false;
    const BrigOperandReg *bor = dyn_cast<BrigOperandReg>(oper);
    valid &= check(bor, "reg offset is wrong, not a BrigOperandReg");  
    valid &= check(bor->type == Brigb32 ||
                   bor->type == Brigb64, "Invalid register, the register "
                   "must be an s or d register");
  }

  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionRef *operand) const {
  bool valid = true;
  dir_iterator fnDir(S_.directives + operand->fn);
  valid &= validate(fnDir);
  valid &= check(isa<BrigDirectiveFunction>(fnDir) ||
                 isa<BrigDirectiveSignature>(fnDir), 
                 "Invalid directive, should point to a "
                 "BrigDirectiveFunction or BrigDirectiveSibnature");
  return valid;
}

bool BrigModule::validate(const BrigOperandImmed *operand) const {
  return true;
}

bool BrigModule::validate(const BrigOperandIndirect *operand) const {
  bool valid = true;

  if (operand->reg) {
    oper_iterator regOper(S_.operands + operand->reg);
    valid &= validate(regOper);
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
  return true;
}
bool BrigModule::validate(const BrigOperandOpaque *operand) const {
  return true;
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

bool BrigModule::validate(const BrigOperandReg *operand) const {
  bool valid = true;

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
  return true;
}
bool BrigModule::validate(const BrigOperandRegV4 *operand) const {
  return true;
}
bool BrigModule::validate(const BrigOperandWaveSz *operand) const {
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

} // namespace brig
} // namespace hsa
