/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_H_
#define INCLUDE_BRIG_H_
#include <stdint.h>
// PRM 20.4
typedef uint16_t BrigPacking16_t;
typedef uint16_t BrigDataType16_t;
typedef uint16_t BrigMachine16_t;
typedef uint16_t BrigProfile16_t;
typedef uint16_t BrigSftz16_t;
typedef uint32_t BrigoOffset32_t;
typedef uint32_t BrigcOffset32_t;
typedef uint32_t BrigStorageClass32_t;
typedef uint32_t BrigOpcode32_t;
typedef uint32_t BrigMemorySemantic32_t;

// BrigDirectiveKinds
// BrigDirectiveKinds is used to specify the kind of directive.
enum BrigDirectiveKinds {
  BrigEDirectiveFunc,
  BrigEDirectiveKernel,
  BrigEDirectiveSymbol,
  BrigEDirectiveImage,
  BrigEDirectiveSamp,
  BrigEDirectiveLabel,
  BrigEDirectiveLabelList,
  BrigEDirectiveVersion,
  BrigEDirectiveProto,
  BrigEDirectiveFile,
  BrigEDirectiveComment,
  BrigEDirectiveLoc,
  BrigEDirectiveInit,
  BrigEDirectiveLabelInit,
  BrigEDirectiveControl,
  BrigEDirectivePragma,
  BrigEDirectiveExtension,
  BrigEDirectiveArgStart,
  BrigEDirectiveArgEnd,
  BrigEDirectiveBlockStart,
  BrigEDirectiveBlockNumeric,
  BrigEDirectiveBlockString,
  BrigEDirectiveBlockEnd,
  BrigEDirectivePad
};

// BrigMachine
// BrigMachine is used to specify the type of machine model.
enum BrigMachine {
  BrigESmall,  // 32-bit model (all addresses are 32 bits;
               // a pointer fits into an s register)
  BrigELarge   // 64-bit model (all addresses are 64 bits;
               // a pointer fits into a d register)
};

// BrigProfile
// BrigProfile is used to specify the kind of profile.
enum BrigProfile {
  BrigEFull,
  BrigEReduced
};

// BrigSftz
// BrigSftz is used to specify the use (or non-use) of single-precision
// flush to zero.
enum BrigSftz {
  BrigESftz,
  BrigENosftz
};

// 20.9.9
struct BrigInstLdSt {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigStorageClass32_t storageClass;
  BrigMemorySemantic32_t memorySemantic;
  uint32_t equivClass;
};

typedef struct BrigDirectiveVersion {
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint16_t major;
  uint16_t minor;
  BrigMachine16_t machine;
  BrigProfile16_t profile;
  BrigSftz16_t ftz;
  uint16_t reserved;
}BrigDirectiveVersion;
#endif  // INCLUDE_BRIG_H_
