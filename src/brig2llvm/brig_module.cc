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
      caseBrig(DirectiveProto);
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

bool BrigModule::validate(const BrigDirectiveImage *dir) const { return true; }
bool BrigModule::validate(const BrigDirectiveSampler *dir) const {
  return true;
}
bool BrigModule::validate(const BrigDirectiveLabel *dir) const { return true; }
bool BrigModule::validate(const BrigDirectiveLabelList *dir) const {
  return true;
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

bool BrigModule::validate(const BrigDirectiveProto *dir) const {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->c_code);
  valid &= validateSName(dir->s_name);
  valid &= check(!dir->reserved, "Reserved not zero");
  valid &= check(sizeof(BrigDirectiveProto) + sizeof(BrigDirectiveProto::BrigProtoType) *
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

} // namespace brig
} // namespace hsa
