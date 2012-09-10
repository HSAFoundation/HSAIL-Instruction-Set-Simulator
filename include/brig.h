/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_H_
#define INCLUDE_BRIG_H_
#include <stdint.h>

// PRM 20.4
// typedef

typedef uint16_t BrigPacking16_t;
typedef uint16_t BrigDataType16_t;
typedef uint32_t BrigGeom32_t;
typedef uint32_t BrigImageFormat32_t;
typedef uint32_t BrigDirectiveKinds32_t;
typedef uint32_t BrigsOffset32_t;
typedef uint32_t BrigcOffset32_t;
typedef uint32_t BrigdOffset32_t;
typedef uint32_t BrigoOffset32_t;
typedef uint16_t BrigMachine16_t;
typedef uint16_t BrigProfile16_t;
typedef uint16_t BrigSftz16_t;
typedef uint32_t BrigControlType32_t;
typedef uint16_t BrigAttribute16_t;
typedef uint32_t BrigStorageClass32_t;
typedef uint32_t BrigImageOrder32_t;
typedef uint32_t BrigOpcode32_t;
typedef uint32_t BrigAtomicOperation32_t;
typedef uint32_t BrigMemorySemantic32_t;
typedef uint32_t BrigCompareOperation32_t;
typedef uint8_t  BrigBoundaryMode8_t;
typedef uint8_t  BrigAddrFilter8_t;
// Enums

// Custom enum to check for alignment of structs
enum BrigAlignment {
    BrigEAlignment_4,
    BrigEAlignment_8
};
// BrigSymbolModifier
enum BrigSymbolModifier {
  BrigConst = 1,  // set means constant; not set means read/write
  BrigArray = 2,  // set means vector (size in dim); not set means scalar
  BrigFlex = 4    // set means flexible array
};
// 20.5.3
enum BrigAttribute {
    BrigExtern,
    BrigStatic,
    BrigNone
};

// 20.5.6
enum BrigControlType {
  BrigEMaxTid,
  BrigEMaxGperC,
  BrigEMemOpt
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
  BrigEDirectiveSampler,
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
// BrigMemorySemantic
// BrigMemorySemantic is used to specify the semantics for a memory operation.
enum BrigMemorySemantic {
  BrigRegular,
  BrigAcquire,
  BrigRelease,
  BrigAcquireRelease,
  BrigDep,
  BrigBin
};

// BrigAtomicOperation
// BrigAtomicOperation is used to specify the type of atomic operation.
enum BrigAtomicOperation {
  BrigAtomicAnd,
  BrigAtomicOr,
  BrigAtomicXor,
  BrigAtomicCas,
  BrigAtomicExch,
  BrigAtomicAdd,
  BrigAtomicInc,
  BrigAtomicDec,
  BrigAtomicMin,
  BrigAtomicMax,
  BrigAtomicSub
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
  BrigBitselect = 122 ,
  BrigFbarInitSizeKnown = 123
};


// Brig Support Structures
// BrigAluModifier specifies arithmetic logic unit controls:
struct BrigAluModifier {
  uint32_t floatOrInt: 1;
  uint32_t rounding: 2;
  uint32_t hi: 1;
  uint32_t ftz: 1;
  uint32_t approx: 1;
  uint32_t fbar: 1;
  uint32_t reserved: 25;
};

//PRM 	20.5.9
enum BrigGeom { 
  Briggeom_1d,  //1D image (1 coordinate needed)
  Briggeom_2d,  //2D image (2 coordinate needed)
  Briggeom_3d,  //3D image (3 coordinate needed)
  Briggeom_1da, //1DA image array (2 coordinates needed)
  Briggeom_1db, //1DB image  buffer (1 coordinates needed)
  Briggeom_2da  //2DA image array (3 coordinates needed)
};

//PRM 	20.5.10
enum BrigImageFormat {
  BrigSNORM_INT8,
  BrigSNORM_INT16,
  BrigUNORM_INT8,
  BrigUNORM_INT16,
  BrigUNORM_SHORT_565,
  BrigUNORM_SHORT_555,
  BrigUNORM_SHORT_101010,
  BrigSIGNED_INT8,
  BrigSIGNED_INT16,
  BrigSIGNED_INT32,
  BrigUNSIGNED_INT8,
  BrigUNSIGNED_INT16,
  BrigUNSIGNED_INT32,
  BrigHALF_FLOAT,
  BrigFLOAT,
  BrigImageFormatUnknown
};

enum BrigImageOrder {
  BrigImage_R,
  BrigImage_A,
  BrigImage_RX,
  BrigImage_RG,
  BrigImage_RGX,
  BrigImage_RA,
  BrigImage_RGB,
  BrigImage_RGBA,
  BrigImage_RGBX,
  BrigImage_BGRA,
  BrigImage_ARGB,
  BrigImage_INTENSITY,
  BrigImage_LUMINANCE,
  BrigImageOrderUnknown // used when no order is specified
};

//PRM 20.5.1
enum BrigAddrFilter {
  BrigSamplerFilterLinear,
  BrigSamplerFilterNearest
};

//PRM 20.5.4
enum BrigBoundaryMode {
  BrigSamplerClamp,
  BrigSamplerWrap,
  BrigSamplerMirror,
  BrigSamplerMirrorOnce,
  BrigSamplerBorder
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

enum BrigSyncFlags {
  BrigGroupLevel = 1,
  BrigGlobalLevel = 2,
  BrigPartialLevel = 4
};

// Directive structures

// PRM 20.8.3
struct BrigDirectiveBase {
  uint16_t size;
  uint16_t kind;
};

// PRM 20.8.4
struct BrigDirectiveComment {
  static const uint16_t DirKind = BrigEDirectiveComment;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 20.8.5
struct BrigDirectiveControl {
  static const uint16_t DirKind = BrigEDirectiveControl;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigControlType32_t controlType;
  uint32_t values[3];
};

// PRM 20.8.6
struct BrigDirectiveExtension {
  static const uint16_t DirKind = BrigEDirectiveExtension;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 20.8.7
struct BrigDirectiveFile {
  static const uint16_t DirKind = BrigEDirectiveFile;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t fileid;
  BrigsOffset32_t s_filename;
};


// PRM 20.8.8
struct BrigDirectiveFunction {
  static const uint16_t DirKind = BrigEDirectiveFunction;
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

// PRM 20.8.9
struct BrigDirectiveImage {
  static const uint16_t DirKind = BrigEDirectiveImage;
  uint16_t size;
  uint16_t kind;
  BrigSymbolCommon s;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t array;
  BrigImageOrder32_t order;
  BrigImageFormat32_t format;
};

// PRM 20.8.10
struct BrigDirectiveInit {
  static const uint16_t DirKind = BrigEDirectiveInit;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t elementCount;
  BrigDataType16_t type;
  uint16_t reserved;

  union {
    uint8_t u8[8];
    uint16_t u16[4];
    uint32_t u32[2];
    uint64_t u64[1];
  } initializationData;
};


// PRM 20.8.11
struct BrigDirectiveKernel {
  static const uint16_t DirKind = BrigEDirectiveKernel;
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

struct BrigDirectiveMethod {
  static const uint16_t DirKind = ~0;
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

// PRM 20.8.12
// BrigDirectiveLabel
// BrigDirectiveLabel declares a label.
struct BrigDirectiveLabel {
  static const uint16_t DirKind = BrigEDirectiveLabel;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 20.8.13
struct BrigDirectiveLabelInit {
  static const uint16_t DirKind = BrigEDirectiveLabelInit;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t elementCount;
  BrigdOffset32_t d_labels[1];
};

// PRM 20.8.14
struct BrigDirectiveLabelList {
  static const uint16_t DirKind = BrigEDirectiveLabelList;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t elementCount;
  BrigdOffset32_t d_labels[1];
};

// PRM 20.8.15
struct BrigDirectiveLoc {
  static const uint16_t DirKind = BrigEDirectiveLoc;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t sourceFile;
  uint32_t sourceLine;
  uint32_t sourceColumn;
};

// PRM 20.8.16
// BrigDirectivePad
// BrigDirectivePad is used to pad out the
// .directives stream to ensure alignment.
struct BrigDirectivePad {
  static const uint16_t DirKind = BrigEDirectivePad;
  uint16_t size;
  uint16_t kind;
};

// PRM 20.8.17
struct BrigDirectivePragma {
  static const uint16_t DirKind = BrigEDirectivePragma;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 20.8.18
struct BrigDirectiveProto {
  static const uint16_t DirKind = BrigEDirectiveProto;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint16_t fbarCount;
  uint16_t reserved;
  uint32_t outCount;
  uint32_t inCount;
  struct BrigProtoType {
    BrigDataType16_t type;
    uint8_t align;
    uint8_t hasDim;
    uint32_t dim;
  } types[1];
};

// PRM 20.8.19
struct BrigDirectiveSampler {
  static const uint16_t DirKind = BrigEDirectiveSampler;
  uint16_t size;
  uint16_t kind;
  BrigSymbolCommon s;
  uint8_t valid: 1;
  uint8_t normalized: 1;
  uint8_t filter:6;
  uint8_t boundaryU;
  uint8_t boundaryV;
  uint8_t boundaryW;
  uint32_t reserved1;
};

// PRM 20.8.20
// BrigDirectiveScope
// BrigDirectiveScope is used to start or end a scope.
struct BrigDirectiveScope {
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

struct BrigDirectiveArgStart {
  static const uint16_t DirKind = BrigEDirectiveArgStart;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

struct BrigDirectiveArgEnd {
  static const uint16_t DirKind = BrigEDirectiveArgEnd;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

// 20.8.21
// The documentation is in error. The BrigDirectiveSymbol needs size and kind
// fields to be parsable. Otherwise, there is no way to tell if the second field
// is a uint16_t kind or the second 16-bit word of BrigSymbolCommon's c_code
// field. This view is supported by Table 20-3 in the HSA PRM.
struct BrigDirectiveSymbol {
  static const uint16_t DirKind = BrigEDirectiveSymbol;
  uint16_t size;
  uint16_t kind;
  BrigSymbolCommon s;
  BrigdOffset32_t d_init;
  uint32_t reserved;
};

struct BrigDirectiveSymbolCommon {
  uint16_t size;
  uint16_t kind;
  BrigSymbolCommon s;
};

// 20.8.22
struct BrigDirectiveVersion {
    static const uint16_t DirKind = BrigEDirectiveVersion;
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

// PRM 20.6.2
struct BrigBlockEnd {
  static const uint16_t DirKind = BrigEDirectiveBlockEnd;
  uint16_t size;
  uint16_t kind;
};

// PRM 20.6.3
// BrigBlockNumeric
// BrigBlockNumeric is a variable-size list of numeric values.
// All the values should have
// the same type.
// More than one BrigBlockNumeric can be
// in a single block section.
// This structure must be aligned to an 8-byte boundary
// (because of the uint64_t field).
struct BrigBlockNumeric {
  static const uint16_t DirKind = BrigEDirectiveBlockNumeric;
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
};

// PRM 20.6.4
// BrigBlockStart
// BrigBlockStart starts a block section.
// It provides a name that can be used to separate information used by different
// debuggers or runtimes.
// More than one BrigBlockStart can have the same name.
struct BrigBlockStart {
  static const uint16_t DirKind = BrigEDirectiveBlockStart;
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 20.6.5
struct BrigBlockString {
  static const uint16_t DirKind = BrigEDirectiveBlockString;
  uint16_t size;
  uint16_t kind;
  BrigsOffset32_t s_name;
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

struct BrigInstBase {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
};

// BrigInstMod
// The BrigInstMod format is used for ALU operations with a modifier.
struct BrigInstMod {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigAluModifier aluModifier;
};

// BrigInstBar
// The BrigInstBar format is used for the barrier and sync operations.
struct BrigInstBar {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  uint32_t syncFlags;
};

struct BrigOperandBase {
  uint16_t size;
  uint16_t kind;
};

// Operand structures
// BrigOperandReg
// BrigOperandReg is used for a register (c, s, or d).
struct BrigOperandReg {
  static const uint16_t OperKind = BrigEOperandReg;
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  uint32_t name;
};

// BrigOperandImmed
// BrigOperandImmed is used for a numeric value.
struct BrigOperandImmed {
  static const uint16_t OperKind = BrigEOperandImmed;
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
};

// BrigOperandAddress
// BrigOperandAddress is used for [name].
// In the .operands section, BrigOperandAddress must start
// on an offset divisible by 4.
struct BrigOperandAddress {
  static const uint16_t OperKind = BrigEOperandAddress;
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigdOffset32_t directive;
  int32_t offset;
};

// BrigOperandLabelRef
// BrigOperandLabelRef is used for a label.
struct BrigOperandLabelRef {
  static const uint16_t OperKind = BrigEOperandLabelRef;
  uint16_t size;
  uint16_t kind;
  uint32_t labeldirective;
};

// BrigOperandFunctionRef
// BrigOperandFunctionRef is used for a reference
// to a function or function signature.
struct BrigOperandFunctionRef {
  static const uint16_t OperKind = BrigEOperandFunctionRef;
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t fn;
};

// BrigOperandArgumentList
// BrigOperandArgumentList is used for the list of
// arguments to a function or a list of
// function names or function signatures.
// Lists of function names or function signatures
// are needed when the call statement has a list of possible targets.
struct BrigOperandArgumentList {
  static const uint16_t OperKind = BrigEOperandArgumentList;
  uint16_t size;
  uint16_t kind;
  uint32_t elementCount;
  BrigdOffset32_t o_args[1];
};

struct BrigOperandFunctionList {
  static const uint16_t OperKind = BrigEOperandFunctionList;
  uint16_t size;
  uint16_t kind;
  uint32_t elementCount;
  BrigdOffset32_t o_args[1];
};

// BrigOperandArgumentRef
// BrigOperandArgumentRef is used for a single argument.
struct BrigOperandArgumentRef {
  static const uint16_t OperKind = BrigEOperandArgumentRef;
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t arg;
};

struct BrigOperandIndirect {
  uint16_t size;
  uint16_t kind;
  BrigoOffset32_t reg;
  BrigDataType16_t type;
  uint16_t reserved;
  int32_t offset;
};

struct BrigOperandCompound {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t name;
  BrigoOffset32_t reg;
  int32_t offset;
};

// BrigOperandRegV2
// BrigOperandRegV2 is used for certain memory operations 
// to allow vector register forms.
struct BrigOperandRegV2 {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t regs[2];
};

// BrigOperandRegV4
// BrigOperandRegV4 is used for certain memory operations 
// to allow vector register forms.
struct BrigOperandRegV4 {
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t regs[4];
};

// BrigInstMem
// The BrigInstMem format is used for operations 
// that take a space modifier. 
struct BrigInstMem {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigStorageClass32_t storageClass;
};

// BrigInstAtomic
// The BrigInstAtomic format is used for atomic operations: atomicReturn and
// atomicNoReturn.
struct BrigInstAtomic {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigAtomicOperation32_t atomicOperation;
  BrigStorageClass32_t storageClass;
  BrigMemorySemantic32_t memorySemantic;
};

// The BrigInstAtomicImage
// The BrigInstAtomicImage format is used for atomicNoReturn image operations.
struct BrigInstAtomicImage {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigAtomicOperation32_t atomicOperation;
  BrigStorageClass32_t storageClass;
  BrigMemorySemantic32_t memorySemantic;
  BrigGeom32_t geom;
};

// BrigOperandOpaque
// BrigOperandOpaque is used for addressing image and sampler objects.
struct BrigOperandOpaque {
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t name;
  BrigoOffset32_t reg;
  int32_t offset;
};

// BrigInstImage
// The BrigInstImage format is used for the load image and store image operations. It is
// similar to BrigInstRead, but has an additional field.
struct BrigInstImage {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigoOffset32_t o_operands[5];
  BrigGeom32_t geom;
  BrigDataType16_t type;
  BrigDataType16_t stype;
  BrigPacking16_t packing;
  uint16_t reserved;
};


// The BrigInstRead
// The BrigInstRead format is a special format used for the read image operation. The
// read image operation needs an associated geometry (1D, 2D, 3D, 1DA, 1DB, or 2DA) and
// the type of the coordinates.

struct BrigInstRead {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigoOffset32_t o_operands[5];
  BrigGeom32_t geom;
  BrigDataType16_t stype;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  uint16_t reserved;
};



#endif  // INCLUDE_BRIG_H_
