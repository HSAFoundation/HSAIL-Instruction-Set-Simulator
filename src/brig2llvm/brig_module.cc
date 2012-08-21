#include "brig_module.h"

#include "llvm/Support/raw_ostream.h"
#include <cstring>

namespace hsa {
namespace brig {

#define check(X,Y) check(X, Y, __FILE__, __LINE__, #X)

template<class Message>
bool (BrigModule::check)(bool test, const Message &msg,
                         const char *filename, unsigned lineno,
                         const char *cause) {
  if(!test && out_)
     (*out_) << filename << "." << lineno << ": " << msg
            << " (" << cause << ")\n";
  return test;
}

bool BrigModule::validate(void) {

  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

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

bool BrigModule::validate(const BrigDirectiveMethod *dir) {
  bool valid = true;

  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);

  const unsigned paramCount = dir->inParamCount + dir->outParamCount;
  dir_iterator argIt = dir_iterator(dir) + 1;
  for(unsigned i = 0; i < paramCount; ++i) {
    const BrigDirectiveSymbol *bds = dyn_cast<BrigDirectiveSymbol>(argIt);
    valid &= check(bds, "Too few argument symbols");
    valid &= check(bds->s.storageClass == BrigArgSpace,
                   "Argument not in arg space");
  }

  const dir_iterator firstScopedDir(S_.directives +
                                    dir->d_firstScopedDirective);
  valid &= check(argIt <= firstScopedDir,
                 "The first scoped directive is too early");
  valid &= check(dir->d_firstScopedDirective <= dir->d_nextDirective,
                 "The next directive is before the first scoped directive");
  valid &= check(dir->attribute == BrigExtern ||
                 dir->attribute == BrigStatic ||
                 dir->attribute == BrigNone,
                 "Invalid linkage type");

  if(dir->inParamCount) {
    const dir_iterator firstInParam1 =
      dir_iterator(dir) + dir->outParamCount + 1;
    const dir_iterator firstInParam2(S_.directives + dir->d_firstInParam);
    valid &= check(firstInParam1 == firstInParam2,
                   "d_firstInParam is wrong");
  }

  return valid;
}

bool BrigModule::validate(const BrigDirectiveSymbol *dir) {
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

bool BrigModule::validate(const BrigDirectiveImage *dir) { return true; }
bool BrigModule::validate(const BrigDirectiveSampler *dir) { return true; }
bool BrigModule::validate(const BrigDirectiveLabel *dir) { return true; }
bool BrigModule::validate(const BrigDirectiveLabelList *dir) {
  return true;
}

// 20.8.22
bool BrigModule::validate(const BrigDirectiveVersion *dir) {
  bool valid = true;

  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
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

bool BrigModule::validate(const BrigDirectiveProto *dir) { return true; }

bool BrigModule::validate(const BrigDirectiveFile *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_filename);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveComment *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLoc *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveInit *dir) { return true; }

bool BrigModule::validate(const BrigDirectiveLabelInit *dir) {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  for (unsigned i = 0; i < dir->elementCount; i++) {
    valid &= check(dir->d_labels[i] <= S_.directivesSize,
                   "d_labels past the directives section");
    const dir_iterator init(S_.directives + dir->d_labels[i]);
    const BrigDirectiveLabel *bcl = dyn_cast<BrigDirectiveLabel>(init);
    valid &= check(bcl, "d_labels offset is wrong, not a BrigDirectiveLabel");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveControl *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                   "c_code past the code section");
  return valid;
}

bool BrigModule::validate(const BrigDirectivePragma *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveExtension *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgStart *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgEnd *dir) {
  bool valid = true;
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockStart *dir) {
  bool valid = true;
  valid &= validateSName(dir->s_name);
  const char *string = S_.strings + dir->s_name;
  valid &= check(0 == strcmp(string, "debug") ||
                 0 == strcmp(string, "rti"),
                 "Invalid s_name, should be either debug or rti");
  valid &= check(dir->c_code <= S_.codeSize,
                 "c_code past the code section");

  return valid;
}

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
  }
  return 0;
}

bool BrigModule::validate(const BrigDirectiveBlockNumeric *dir) {
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

bool BrigModule::validate(const BrigDirectiveBlockString *dir) {
  bool valid = true;
  valid &= validateSName(dir->s_name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockEnd *dir) {
  bool valid = true;
  valid &= validateAlignment(dir, 4);
  return valid;
}

bool BrigModule::validate(const BrigDirectivePad *dir) {
  bool valid = true;
  return valid;
}

bool BrigModule::validate(const BrigSymbolCommon *s) {
  bool valid = true;

  // RPM 20.5.20: 8-16 reserved for extensions
  valid &= check(s->c_code <= S_.codeSize, "c_code past the code section");
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

bool BrigModule::validateSName(BrigsOffset32_t s_name) {

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

bool BrigModule::validateAlignment(const void *dir, uint8_t alignment) {
  bool valid = true;
  const uint8_t *dirOffset = reinterpret_cast<const uint8_t *>(dir);
  valid &= check((S_.directives - dirOffset) % alignment == 0,
                 "Improperly aligned directive");
  return valid;
}

} // namespace brig
} // namespace hsa
