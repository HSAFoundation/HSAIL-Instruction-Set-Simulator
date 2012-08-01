/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_H_
#define INCLUDE_BRIG_H_
#include <stdint.h>

// PRM 20.4
// typedef

typedef uint16_t BrigPacking16_t;
typedef uint16_t BrigDataType16_t;
typedef uint32_t BrigsOffset32_t;
typedef uint32_t BrigcOffset32_t;
typedef uint32_t BrigdOffset32_t;
typedef uint32_t BrigoOffset32_t;
typedef uint16_t BrigMachine16_t;
typedef uint16_t BrigProfile16_t;
typedef uint16_t BrigSftz16_t;
typedef uint16_t BrigAttribute16_t;
typedef uint32_t BrigStorageClass32_t;
typedef uint32_t BrigOpcode32_t;
typedef uint32_t BrigMemorySemantic32_t;

// Enums

// Custom enum to check for alignment of structs
enum BrigAlignment {
    BrigEAlignment_4,
    BrigEAlignment_8
};

// 20.5.3
enum BrigAttribute {
    BrigExtern,
    BrigStatic,
    BrigNone
};
// 20.5.7
enum BrigDataType {
    Brigs8,            // signed integer 8 bits
    Brigs16,           // signed integer 16 bits
    Brigs32,           // signed integer 32 bits
    Brigs64,           // signed integer 64 bits
    Brigu8,            // unsigned integer 8 bits
    Brigu16,           // unsigned integer 16 bits
    Brigu32,           // unsigned integer 32 bits
    Brigu64,           // unsigned integer 64 bits
    Brigf16,           // floating-point 16 bits
    Brigf32,           // floating-point 32 bits
    Brigf64,           // floating-point 64 bits
    Brigb1,            // uninterpreted bit string of length 1 bit
    Brigb8,            // uninterpreted bit string of length 8 bits
    Brigb16,           // uninterpreted bit string of length 16 bits
    Brigb32,           // uninterpreted bit string of length 32 bits
    Brigb64,           // uninterpreted bit string of length 64 bits
    Brigb128,          // uninterpreted bit string of length 128 bits
    BrigROImg,         // read-only image object
    BrigRWImg,         // read/write image object
    BrigSamp,          // sampler object
    Brigu8x4,          // four bytes unsigned
    Brigs8x4,          // four bytes signed
    Brigu8x8,          // eight bytes unsigned
    Brigs8x8,          // eight bytes signed
    Brigu8x16,         // 16 bytes unsigned
    Brigs8x16,         // 16 bytes signed
    Brigu16x2,         // two short unsigned integers
    Brigs16x2,         // two short signed integer
    Brigf16x2,         // two half-floats
    Brigu16x4,         // four short unsigned integers
    Brigs16x4,         // four short signed integers
    Brigf16x4,         // four half-floats
    Brigu16x8,         // eight short unsigned integers
    Brigs16x8,         // eight short signed integers
    Brigf16x8,         // eight half-floats
    Brigu32x2,         // two unsigned integers
    Brigs32x2,         // two signed integers
    Brigf32x2,         // two floats
    Brigu32x4,         // four unsigned integers
    Brigs32x4,         // four signed integers
    Brigf32x4,         // four floats
    Brigu64x2,         // two 64-bit unsigned integers
    Brigs64x2,         // two 64-bit signed integers
    Brigf64x2          // two doubles
};
// PRM 20.5.8
// BrigDirectiveKinds
// BrigDirectiveKinds is used to specify the kind of directive.
enum BrigDirectiveKinds {
  BrigEDirectiveFunction,
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
// PRM 20.3.2
struct BrigSymbolCommon {
  BrigcOffset32_t c_code;
  BrigStorageClass32_t storageClass;
  BrigAttribute16_t attribute;
  uint16_t reserved;
  uint32_t symbolModifier;
  uint32_t dim;
  BrigsOffset32_t s_name;
  BrigDataType16_t type;
  uint16_t align;
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

// PRM 20.5.12
enum BrigInstKinds {
  BrigEInstBase,
  BrigEInstMod,
  BrigEInstCvt,
  BrigEInstRead,
  BrigEInstBar,
  BrigEInstLdSt,
  BrigEInstCmp,
  BrigEInstMem,
  BrigEInstAtomic,
  BrigEInstAtomicImage,
  BrigEInstImage
};
// 20.5.20
enum BrigStorageClass {
    BrigGlobalSpace,
    BrigGroupSpace,
    BrigPrivateSpace,
    BrigKernargSpace,
    BrigReadonlySpace,
    BrigSpillSpace,
    BrigArgSpace,
    BrigFlatSpace
};
// 8-16 reserved for extensions

// BrigOperandKinds
// BrigOperandKinds is used to specify the kind of operand.
enum BrigOperandKinds {
  BrigEOperandBase,
  BrigEOperandReg,
  BrigEOperandImmed,
  BrigEOperandRegV2,
  BrigEOperandRegV4,
  BrigEOperandAddress,
  BrigEOperandLabelRef,
  BrigEOperandIndirect,
  BrigEOperandCompound,
  BrigEOperandArgumentList,
  BrigEOperandFunctionList,
  BrigEOperandArgumentRef,
  BrigEOperandWaveSz,
  BrigEOperandFunctionRef,
  BrigEOperandPad,
  BrigEOperandOpaque
};

// BrigPacking
// BrigPacking is used to specify the packing type of HSAIL operation.
enum BrigPacking {
  BrigNoPacking,
  BrigPackPP,
  BrigPackPS,
  BrigPackSP,
  BrigPackSS,
  BrigPackS,
  BrigPackP,
  BrigPackPPsat,
  BrigPackPSsat,
  BrigPackSPsat,
  BrigPackSSsat,
  BrigPackSsat,
  BrigPackPsat
};

// BrigOpcode
// BrigOpcode is used to specify the opcode for the HSAIL operation.
enum BrigOpcode {
  BrigAbs = 0,
  BrigAdd = 1,
  BrigCarry = 2,
  BrigBorrow = 3,
  BrigDiv = 4,
  BrigMax = 5,
  BrigMin = 6,
  BrigMul = 7,
  BrigNeg = 8,
  BrigRem = 9,
  BrigSub = 10,
  BrigMad = 11,
  BrigMul24 = 12,
  BrigMad24 = 13,
  BrigShl = 14,
  BrigShr = 15,
  BrigAnd = 16,
  BrigOr = 17,
  BrigXor = 18,
  BrigNot = 19,
  BrigPopcount = 20,
  BrigFirstbit = 21,
  BrigLastbit = 22,
  BrigBitRev = 23,
  BrigExtract = 24,
  BrigInsert = 25,
  BrigMov = 26,
  BrigUnpacklo = 27,
  BrigUnpackhi = 28,
  BrigShuffle = 29,
  Brigmovslo = 30,
  Brigmovshi = 31,
  Brigmovdlo = 32,
  Brigmovdhi = 33,
  BrigLda = 34,
  BrigLdc = 36,
  BrigCmov = 37,
  BrigPackedCmp = 38,
  BrigCopySign = 39,
  BrigFma = 40,
  BrigClass = 41,
  BrigSqrt = 42,
  BrigFract = 43,
  BrigFcos = 44,
  BrigFsin = 45,
  BrigFlog2 = 46,
  BrigFexp2 = 47,
  BrigFrsqrt = 48,
  BrigFrcp = 49,
  BrigCmp = 50,
  BrigSegmentp = 51,
  BrigFtoS = 52,
  BrigStoF = 53,
  BrigCvt = 54,
  BrigF2u4 = 55,
  BrigUnpack3 = 56,
  BrigUnpack2 = 57,
  BrigUnpack1 = 58,
  BrigUnpack0 = 59,
  BrigBitAlign = 60,
  BrigByteAlign = 61,
  BrigLerp = 62,
  BrigSad = 63,
  BrigSad2 = 64,
  BrigSad4 = 65,
  BrigSad4hi = 66,
  BrigLd = 67,
  BrigSt = 68,
  BrigAtomic = 69,
  BrigAtomicNoRet = 70,
  BrigRdImage = 71,
  BrigLdImage = 72,
  BrigStImage = 73,
  BrigAtomicNoRetImage = 74,
  BrigAtomicImage = 75,
  BrigQueryWidth = 76,
  BrigQueryHeight = 77,
  BrigQueryDepth = 78,
  BrigQueryOrder = 79,
  BrigQueryFiltering = 80,
  BrigQueryData = 81,
  BrigQueryArray = 82,
  BrigQueryNormalized = 83,
  BrigCbr = 84,
  BrigBrn = 85,
  BrigSync = 86,
  BrigBarrier = 87,
  BrigFbarInitSize = 88,
  BrigFbarInitSizeWg = 89,
  BrigFbarInit = 90,
  BrigFbarWait = 91,
  BrigFbarArrive = 92,
  BrigFbarSkip = 93,
  BrigFbarRelease = 94,
  BrigFbarReleaseCF = 95,
  BrigCount = 96,
  BrigCountup = 97,
  BrigMask = 98,
  BrigSend = 99,
  BrigReceive = 100,
  BrigRet = 101,
  BrigSyscall = 102,
  BrigAlloca = 103,
  BrigCall = 104,
  BrigWorkItemId = 105,
  BrigWorkItemAId = 106,
  BrigWorkGroupId = 107,
  BrigWorkGroupSize = 108,
  BrigNDRangesize = 109,
  BrigNDRangegroups = 110,
  BrigLaneId = 111,
  BrigDynWaveId = 112,
  BrigMaxDynWaveId = 113,
  BrigDispatchId = 114,
  BrigCU = 115,
  BrigWorkDim = 116,
  BrigClock = 117,
  Brigworkitemidflat = 118,
  Brigworkitemaidflat = 119,
  BrigNop = 120,
  BrigDebugtrap = 121,
  BrigBitselect = 122
};
// Directive structures

// 20.8.8
struct BrigDirectiveFunction {
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint32_t inParamCount;
  BrigdOffset32_t d_firstScopedDirective;
  uint32_t operationCount;
  BrigdOffset32_t d_nextDirective;
  BrigAttribute16_t attribute;
  uint16_t fbarCount;
  uint32_t outParamCount;
  BrigdOffset32_t d_firstInParam;
};

// 20.8.21
struct BrigDirectiveSymbol {
  BrigSymbolCommon s;
  BrigdOffset32_t d_init;
  uint32_t reserved;
};

// 20.8.22
struct BrigDirectiveVersion {
    uint16_t size;
    uint16_t kind;
    BrigcOffset32_t c_code;
    uint16_t major;
    uint16_t minor;
    BrigMachine16_t machine;
    BrigProfile16_t profile;
    BrigSftz16_t ftz;
    uint16_t reserved;
};

// 20.6.3
// BrigBlockNumeric
// BrigBlockNumeric is a variable-size list of numeric values.
// All the values should have
// the same type.
// More than one BrigBlockNumeric can be
// in a single block section.
// This structure must be aligned to an 8-byte boundary
// (because of the uint64_t field).
typedef struct BrigBlockNumeric {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t elementCount;
  union {
    uint8_t u8[8];
    uint16_t u16[4];
    uint32_t u32[2];
    uint64_t u64[1];
  };
}BrigBlockNumeric;

// 20.6.4
// BrigBlockStart
// BrigBlockStart starts a block section.
// It provides a name that can be used to separate information used by different
// debuggers or runtimes.
// More than one BrigBlockStart can have the same name.
typedef struct BrigBlockStart {
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
} BrigBlockStart;

// 20.8.16
// BrigDirectivePad
// BrigDirectivePad is used to pad out the
// .directives stream to ensure alignment.
typedef struct BrigDirectivePad {
  uint16_t size;
  uint16_t kind;
} BrigDirectivePad;
// Code structures

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

// Operand structures
// BrigOperandReg
// BrigOperandReg is used for a register (c, s, or d).
typedef struct BrigOperandReg {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  uint32_t name;
}BrigOperandReg;

// BrigOperandImmed
// BrigOperandImmed is used for a numeric value.
typedef struct BrigOperandImmed {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  union {
    uint32_t u;
    float f;
    double d;
    uint64_t l[2];
    uint16_t h;
    uint8_t c;
  } bits;
}BrigOperandImmed;

// BrigOperandAddress
// BrigOperandAddress is used for [name].
// In the .operands section, BrigOperandAddress must start
// on an offset divisible by 4.
typedef struct BrigOperandAddress {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigdOffset32_t directive;
  int32_t offset;
}BrigOperandAddress;

typedef struct BrigInstBase {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
}BrigInstBase;

#endif  // INCLUDE_BRIG_H_
