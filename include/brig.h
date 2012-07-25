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

//BrigDataType
//BrigDataType is used to specify the data type of the destination of the operation.
enum BrigDataType {
	Brigs8, // signed integer 8 bits
	Brigs16, // signed integer 16 bits
	Brigs32, // signed integer 32 bits
	Brigs64, // signed integer 64 bits
	Brigu8, // unsigned integer 8 bits
	Brigu16, // unsigned integer 16 bits
	Brigu32, // unsigned integer 32 bits
	Brigu64, // unsigned integer 64 bits
	Brigf16, // floating-point 16 bits
	Brigf32, // floating-point 32 bits
	Brigf64, // floating-point 64 bits
	Brigb1, // uninterpreted bit string of length 1 bit
	Brigb8, // uninterpreted bit string of length 8 bits
	Brigb16, // uninterpreted bit string of length 16 bits
	Brigb32, // uninterpreted bit string of length 32 bits
	Brigb64, // uninterpreted bit string of length 64 bits
	Brigb128, // uninterpreted bit string of length 128 bits
	BrigROImg, // read-only image object
	BrigRWImg, // read/write image object
	BrigSamp, // sampler object
	Brigu8x4, // four bytes unsigned
	Brigs8x4, // four bytes signed
	Brigu8x8, // eight bytes unsigned
	Brigs8x8, // eight bytes signed
	Brigu8x16, // 16 bytes unsigned
	Brigs8x16, // 16 bytes signed
	Brigu16x2, // two short unsigned integers
	Brigs16x2, // two short signed integers
	Brigf16x2, // two half-floats
	Brigu16x4, // four short unsigned integers
	Brigs16x4, // four short signed integers
	Brigf16x4, // four half-floats
	Brigu16x8, // eight short unsigned integers
	Brigs16x8, // eight short signed integers
	Brigf16x8, // eight half-floats
	Brigu32x2, // two unsigned integers
	Brigs32x2, // two signed integers
	Brigf32x2, // two floats
	Brigu32x4, // four unsigned integers
	Brigs32x4, // four signed integers
	Brigf32x4, // four floats
	Brigu64x2, // two 64-bit unsigned integers
	Brigs64x2, // two 64-bit signed integers
	Brigf64x2 // two doubles
};
//BrigOperandKinds
//BrigOperandKinds is used to specify the kind of operand.
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
//BrigOperandReg
//BrigOperandReg is used for a register (c, s, or d).
typedef struct BrigOperandReg {
	uint16_t size;
	uint16_t kind;
	BrigDataType16_t type;
	uint16_t reserved;
	uint32_t name;
}BrigOperandReg;
#endif //INCLUDE_BRIG_H_
