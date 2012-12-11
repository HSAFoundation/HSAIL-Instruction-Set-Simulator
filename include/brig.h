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

// PRM 19.5.21
// BrigSymbolModifier
enum BrigSymbolModifier {
  BrigConst = 1,  // set means constant; not set means read/write
  BrigArray = 2,  // set means vector (size in dim); not set means scalar
  BrigFlex = 4    // set means flexible array
};

// 20.5.2
// BrigAtomicOperation is used to specify the type of atomic operation.
enum BrigAtomicOperation {
  BrigAtomicAnd = 0,
  BrigAtomicOr = 1,
  BrigAtomicXor = 2,
  BrigAtomicCas = 3,
  BrigAtomicExch = 4,
  BrigAtomicAdd = 5,
  BrigAtomicInc = 6,
  BrigAtomicDec = 7,
  BrigAtomicMin = 8,
  BrigAtomicMax = 9,
  BrigAtomicSub = 10
};

// 20.5.3
enum BrigAttribute {
    BrigExtern = 0,
    BrigStatic = 1,
    BrigNone = 2
};

// 19.5.6
enum BrigControlType {
  BrigEMaxWIperG = 0, // work-items per work-group
  BrigEMaxGperC = 1, // groups per compute unit
  BrigEMemOpt = 2 // values[0] = 1 if on
};

// 19.5.7
enum BrigDataType {
    Brigs8 = 0,            // signed integer 8 bits
    Brigs16 = 1,           // signed integer 16 bits
    Brigs32 = 2,           // signed integer 32 bits
    Brigs64 = 3,           // signed integer 64 bits
    Brigu8 = 4,            // unsigned integer 8 bits
    Brigu16 = 5,           // unsigned integer 16 bits
    Brigu32 = 6,           // unsigned integer 32 bits
    Brigu64 = 7,           // unsigned integer 64 bits
    Brigf16 = 8,           // floating-point 16 bits
    Brigf32 = 9,           // floating-point 32 bits
    Brigf64 = 10,          // floating-point 64 bits
    Brigb1 = 11,           // uninterpreted bit string of length 1 bit
    Brigb8 = 12,           // uninterpreted bit string of length 8 bits
    Brigb16 = 13,          // uninterpreted bit string of length 16 bits
    Brigb32 = 14,          // uninterpreted bit string of length 32 bits
    Brigb64 = 15,          // uninterpreted bit string of length 64 bits
    Brigb128 = 16,         // uninterpreted bit string of length 128 bits
    BrigROImg = 17,        // read-only image object
    BrigRWImg = 18,        // read/write image object
    BrigSamp = 19,         // sampler object
    Brigu8x4 = 20,         // four bytes unsigned
    Brigs8x4 = 21,         // four bytes signed
    Brigu8x8 = 22,         // eight bytes unsigned
    Brigs8x8 = 23,         // eight bytes signed
    Brigu8x16 = 24,        // 16 bytes unsigned
    Brigs8x16 = 25,        // 16 bytes signed
    Brigu16x2 = 26,         // two short unsigned integers
    Brigs16x2 = 27,         // two short signed integer
    Brigf16x2 = 28,         // two half-floats
    Brigu16x4 = 29,         // four short unsigned integers
    Brigs16x4 = 30,         // four short signed integers
    Brigf16x4 = 31,         // four half-floats
    Brigu16x8 = 32,         // eight short unsigned integers
    Brigs16x8 = 33,         // eight short signed integers
    Brigf16x8 = 34,         // eight half-floats
    Brigu32x2 = 35,         // two unsigned integers
    Brigs32x2 = 36,         // two signed integers
    Brigf32x2 = 37,         // two floats
    Brigu32x4 = 38,         // four unsigned integers
    Brigs32x4 = 39,         // four signed integers
    Brigf32x4 = 40,         // four floats
    Brigu64x2 = 41,         // two 64-bit unsigned integers
    Brigs64x2 = 42,         // two 64-bit signed integers
    Brigf64x2 = 43          // two doubles
};
// PRM 19.5.8
// BrigDirectiveKinds
// BrigDirectiveKinds is used to specify the kind of directive.
enum BrigDirectiveKinds {
  BrigEDirectivePad = 0,
  BrigEDirectiveFunction = 1,
  BrigEDirectiveKernel = 2,
  BrigEDirectiveSymbol = 3,
  BrigEDirectiveImage = 4,
  BrigEDirectiveSampler = 5,
  BrigEDirectiveLabel = 6,
  BrigEDirectiveLabelList = 7,
  BrigEDirectiveVersion = 8,
  BrigEDirectiveSignature = 9,
  BrigEDirectiveFile = 10,
  BrigEDirectiveComment = 11,
  BrigEDirectiveLoc = 12,
  BrigEDirectiveInit = 13,
  BrigEDirectiveLabelInit = 14,
  BrigEDirectiveControl = 15,
  BrigEDirectivePragma = 16,
  BrigEDirectiveExtension = 17,
  BrigEDirectiveArgStart = 18,
  BrigEDirectiveArgEnd = 19,
  BrigEDirectiveBlockStart = 20,
  BrigEDirectiveBlockNumeric = 21,
  BrigEDirectiveBlockString = 22,
  BrigEDirectiveBlockEnd = 23
};

// PRM 19.5.13
// BrigMachine
// BrigMachine is used to specify the type of machine model.
enum BrigMachine {
  BrigESmall = 0,  // 32-bit model (all addresses are 32 bits;
                   // a pointer fits into an s register)
  BrigELarge = 1   // 64-bit model (all addresses are 64 bits;
                   // a pointer fits into a d register)
};

// PRM 19.5.18
// BrigProfile
// BrigProfile is used to specify the kind of profile.
enum BrigProfile {
  BrigEFull = 0,
  BrigEReduced = 1
};

// PRM 19.5.19
// BrigSftz
// BrigSftz is used to specify the use (or non-use) of single-precision
// flush to zero.
enum BrigSftz {
  BrigESftz = 0,
  BrigENosftz = 1
};

// PRM 19.5.12
enum BrigInstKinds {
  BrigEInstBase = 0,
  BrigEInstMod = 1,
  BrigEInstCvt = 2,
  BrigEInstImage = 3,
  BrigEInstBar = 4,
  BrigEInstLdSt = 5,
  BrigEInstCmp = 6,
  BrigEInstMem = 7,
  BrigEInstAtomic = 8,
  BrigEInstAtomicImage = 9,
  BrigEInstSegp = 10
};

// 19.5.14
// BrigMemorySemantic is used to specify the semantics for a memory operation.
enum BrigMemorySemantic {
  BrigRegular = 0,
  BrigAcquire = 1,
  BrigRelease = 2,
  BrigAcquireRelease = 3,
  BrigDep = 4,
  BrigParAcquire = 5,
  BrigParRelease = 6,
  BrigParAcquireRelease = 7
};

// 19.5.20
enum BrigStorageClass {
  BrigGlobalSpace = 0,
  BrigGroupSpace = 1,
  BrigPrivateSpace = 2,
  BrigKernargSpace = 3,
  BrigReadonlySpace = 4,
  BrigSpillSpace = 5,
  BrigArgSpace = 6,
  BrigFlatSpace = 7,
  BrigInvalidSpace
};
// 8-16 reserved for extensions

// BrigOperandKinds
// BrigOperandKinds is used to specify the kind of operand.
enum BrigOperandKinds {
  BrigEOperandPad = 0,
  BrigEOperandBase = 1,
  BrigEOperandReg = 2,
  BrigEOperandImmed = 3,
  BrigEOperandRegV2 = 4,
  BrigEOperandRegV4 = 5,
  BrigEOperandAddress = 6,
  BrigEOperandLabelRef = 7,
  BrigEOperandIndirect = 8,
  BrigEOperandCompound = 9,
  BrigEOperandArgumentList = 10,
  BrigEOperandFunctionList = 11,
  BrigEOperandArgumentRef = 12,
  BrigEOperandWaveSz = 13,
  BrigEOperandFunctionRef = 14,
  BrigEOperandOpaque = 15
};

// PRM 19.5.17
// BrigPacking
// BrigPacking is used to specify the packing type of HSAIL operation.
enum BrigPacking {
  BrigNoPacking = 0,
  BrigPackPP = 1,
  BrigPackPS = 2,
  BrigPackSP = 3,
  BrigPackSS = 4,
  BrigPackS = 5,
  BrigPackP = 6,
  BrigPackPPsat = 7,
  BrigPackPSsat = 8,
  BrigPackSPsat = 9,
  BrigPackSSsat = 10,
  BrigPackSsat = 11,
  BrigPackPsat = 12
};

// BrigOpcode
// BrigOpcode is used to specify the opcode for the HSAIL operation.
enum BrigOpcode {
  BrigAbs = 0,
  BrigAdd = 1,
  BrigBorrow = 2,
  BrigCarry = 3,
  BrigCopySign = 4,
  BrigDiv = 5,
  BrigFma = 6,
  BrigFract = 7,
  BrigMad = 8,
  BrigMax = 9,
  BrigMin = 10,
  BrigMul = 11,
  BrigMulHi = 12,
  BrigNeg = 13,
  BrigRem = 14,
  BrigSqrt = 15,
  BrigSub = 16,
  BrigMad24 = 17,
  BrigMad24Hi = 18,
  BrigMul24 = 19,
  BrigMul24Hi = 20,
  BrigShl = 21,
  BrigShr = 22,
  BrigAnd = 23,
  BrigNot = 24,
  BrigOr = 25,
  BrigPopCount = 26,
  BrigXor = 27,
  BrigBitRev = 28,
  BrigBitSelect = 29,
  BrigExtract = 30,
  BrigFirstBit = 31,
  BrigInsert = 32,
  BrigLastBit = 33,
  BrigLda = 34,
  BrigLdc = 35,
  BrigMov = 36,
  BrigMovdHi = 37,
  BrigMovdLo = 38,
  BrigMovsHi = 39,
  BrigMovsLo = 40,
  BrigShuffle = 41,
  BrigUnpackHi = 42,
  BrigUnpackLo = 43,
  BrigCmov = 44,
  BrigClass = 45,
  BrigFcos = 46,
  BrigFexp2 = 47,
  BrigFlog2 = 48,
  BrigFrcp = 49,
  BrigFsqrt = 50,
  BrigFrsqrt = 51,
  BrigFsin = 52,
  BrigBitAlign = 53,
  BrigByteAlign = 54,
  BrigF2u4 = 55,
  BrigLerp = 56,
  BrigSad = 57,
  BrigSad2 = 58,
  BrigSad4 = 59,
  BrigSad4Hi = 60,
  BrigUnpack0 = 61,
  BrigUnpack1 = 62,
  BrigUnpack2 = 63,
  BrigUnpack3 = 64,
  BrigSegmentp = 65,
  BrigFtoS = 66,
  BrigStoF = 67,
  BrigCmp = 68,
  BrigPackedCmp = 69,
  BrigCvt = 70,
  BrigLd = 71,
  BrigSt = 72,
  BrigAtomic = 73,
  BrigAtomicNoRet = 74,
  BrigRdImage = 75,
  BrigLdImage = 76,
  BrigStImage = 77,
  BrigAtomicImage = 78,
  BrigAtomicNoRetImage = 79,
  BrigQueryArray = 80,
  BrigQueryData = 81,
  BrigQueryDepth = 82,
  BrigQueryFiltering = 83,
  BrigQueryHeight = 84,
  BrigQueryNormalized = 85,
  BrigQueryOrder = 86,
  BrigQueryWidth = 87,
  BrigCbr = 88,
  BrigBrn = 89,
  BrigBarrier = 90,
  BrigFbarArrive = 91,
  BrigFbarInit = 92,
  BrigFbarRelease = 93,
  BrigFbarSkip = 94,
  BrigFbarWait = 95,
  BrigSync = 96,
  BrigCount = 97,
  BrigCountUp = 98,
  BrigMask = 99,
  BrigSend = 100,
  BrigReceive = 101,
  BrigCall = 102,
  BrigRet = 103,
  BrigSysCall = 104,
  BrigAlloca = 105,
  BrigClock = 106,
  BrigCU = 107,
  BrigCurrentWorkGroupSize = 108,
  BrigDebugTrap = 109,
  BrigDispatchId = 110,
  BrigDynWaveId = 111,
  BrigLaneId = 112,
  BrigMaxDynWaveId = 113,
  BrigNDRangeGroups = 114,
  BrigNDRangeSize = 115,
  BrigNop = 116,
  BrigNullPtr = 117,
  BrigWorkDim = 118,
  BrigWorkGroupId = 119,
  BrigWorkGroupSize = 120,
  BrigWorkItemAbsId = 121,
  BrigWorkItemAbsIdFlat = 122,
  BrigWorkItemId = 123,
  BrigWorkItemIdFlat = 124,
  BrigInvalidOpcode
};

// Brig Support Structures
// BrigAluModifier specifies arithmetic logic unit controls:
struct BrigAluModifier {
  uint32_t valid: 1;
  uint32_t floatOrInt: 1;
  uint32_t rounding: 2;
  uint32_t ftz: 1;
  uint32_t approx: 1;
  uint32_t fbar: 1;
  uint32_t reserved: 25;
};

//PRM 	19.5.9
//BrigGeom
//BrigGeom is used to specify the number of coordinates needed to access an
//image.
enum BrigGeom {
  Briggeom_1d = 0,  //1D image (1 coordinate needed)
  Briggeom_2d = 1,  //2D image (2 coordinate needed)
  Briggeom_3d = 2,  //3D image (3 coordinate needed)
  Briggeom_1da = 3, //1DA image array (2 coordinates needed)
  Briggeom_1db = 4, //1DB image  buffer (1 coordinates needed)
  Briggeom_2da = 5  //2DA image array (3 coordinates needed)
};

//PRM 	19.5.10
enum BrigImageFormat {
  BrigImageFormatUnknown = 0,
  BrigSNORM_INT8 = 1,
  BrigSNORM_INT16 = 2,
  BrigUNORM_INT8 = 3,
  BrigUNORM_INT16 = 4,
  BrigUNORM_SHORT_565 = 5,
  BrigUNORM_SHORT_555 = 6,
  BrigUNORM_SHORT_101010 = 7,
  BrigSIGNED_INT8 = 8,
  BrigSIGNED_INT16 = 9,
  BrigSIGNED_INT32 = 10,
  BrigUNSIGNED_INT8 = 11,
  BrigUNSIGNED_INT16 = 12,
  BrigUNSIGNED_INT32 = 13,
  BrigHALF_FLOAT = 14,
  BrigFLOAT = 15,
  BrigImageFormatInvalid
};

enum BrigImageOrder {
  BrigImageOrderUnknown = 0, // used when no order is specified
  BrigImage_R = 1,
  BrigImage_A = 2,
  BrigImage_RX = 3,
  BrigImage_RG = 4,
  BrigImage_RGX = 5,
  BrigImage_RA = 6,
  BrigImage_RGB = 7,
  BrigImage_RGBA = 8,
  BrigImage_RGBX = 9,
  BrigImage_BGRA = 10,
  BrigImage_ARGB = 11,
  BrigImage_INTENSITY = 12,
  BrigImage_LUMINANCE = 13,
  BrigImageOrderInvalid
};

//PRM 20.5.1
enum BrigAddrFilter {
  BrigSamplerFilterLinear = 0,
  BrigSamplerFilterNearest = 1
};

//PRM 20.5.4
enum BrigBoundaryMode {
  BrigSamplerClamp = 0,
  BrigSamplerWrap = 1,
  BrigSamplerMirror = 2,
  BrigSamplerMirrorOnce = 3,
  BrigSamplerBorder = 4
};

//PRM 19.5.5
enum BrigCompareOperation {
  BrigEq = 0,
  BrigNe = 1,
  BrigLt = 2,
  BrigLe = 3,
  BrigGt = 4,
  BrigGe = 5,
  BrigEqu = 6,
  BrigNeu = 7,
  BrigLtu = 8,
  BrigLeu = 9,
  BrigGtu = 10,
  BrigGeu = 11,
  BrigNum = 12,
  BrigNan = 13,
  BrigSeq = 14,
  BrigSne = 15,
  BrigSlt = 16,
  BrigSle = 17,
  BrigSgt = 18,
  BrigSge = 19,
  BrigSgeu = 20,
  BrigSequ = 21,
  BrigSneu = 22,
  BrigSltu = 23,
  BrigSleu = 24,
  BrigSnum = 25,
  BrigSnan = 26,
  BrigSgtu = 27
};

// PRM 19.3.2
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

// PRM 19.5.22
enum BrigSyncFlags {
  BrigGroupLevel = 1,
  BrigGlobalLevel = 2,
  BrigPartialLevel = 4
};

// Directive structures

// PRM 19.8.3
struct BrigDirectiveBase {
  uint16_t size;
  uint16_t kind;
};

// PRM 19.8.4
struct BrigDirectiveComment {
  enum { DirKind = BrigEDirectiveComment };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 19.8.5
struct BrigDirectiveControl {
  enum { DirKind = BrigEDirectiveControl };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigControlType32_t controlType;
  uint32_t values[3];
};

// PRM 19.8.6
struct BrigDirectiveExtension {
  enum { DirKind = BrigEDirectiveExtension };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 19.8.7
struct BrigDirectiveFile {
  enum { DirKind = BrigEDirectiveFile };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t fileid;
  BrigsOffset32_t s_filename;
};


// PRM 19.8.8
struct BrigDirectiveFunction {
  enum { DirKind = BrigEDirectiveFunction };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint32_t inParamCount;
  BrigdOffset32_t d_firstScopedDirective;
  uint32_t operationCount;
  BrigdOffset32_t d_nextDirective;
  BrigAttribute16_t attribute;
  uint16_t reserved;
  uint32_t outParamCount;
  BrigdOffset32_t d_firstInParam;
};

// PRM 19.8.9
struct BrigDirectiveImage {
  enum { DirKind = BrigEDirectiveImage };
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

// PRM 19.8.10
struct BrigDirectiveInit {
  enum { DirKind = BrigEDirectiveInit };
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


// PRM 19.8.11
struct BrigDirectiveKernel {
  enum { DirKind = BrigEDirectiveKernel };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint32_t inParamCount;
  BrigdOffset32_t d_firstScopedDirective;
  uint32_t operationCount;
  BrigdOffset32_t d_nextDirective;
  BrigAttribute16_t attribute;
  uint16_t reserved;
  uint32_t outParamCount;
  BrigdOffset32_t d_firstInParam;
};

struct BrigDirectiveMethod {
  enum { DirKind = ~0 };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint32_t inParamCount;
  BrigdOffset32_t d_firstScopedDirective;
  uint32_t operationCount;
  BrigdOffset32_t d_nextDirective;
  BrigAttribute16_t attribute;
  uint16_t reserved;
  uint32_t outParamCount;
  BrigdOffset32_t d_firstInParam;
};

// PRM 19.8.12
// BrigDirectiveLabel
// BrigDirectiveLabel declares a label.
struct BrigDirectiveLabel {
  enum { DirKind = BrigEDirectiveLabel };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 19.8.13
struct BrigDirectiveLabelInit {
  enum { DirKind = BrigEDirectiveLabelInit };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t elementCount;
  BrigsOffset32_t s_name;
  BrigdOffset32_t d_labels[1];
};

// PRM 19.8.14
struct BrigDirectiveLabelList {
  enum { DirKind = BrigEDirectiveLabelList };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigdOffset32_t label;
  uint32_t elementCount;
  BrigdOffset32_t d_labels[1];
};

// PRM 19.8.15
struct BrigDirectiveLoc {
  enum { DirKind = BrigEDirectiveLoc };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  uint32_t sourceFile;
  uint32_t sourceLine;
  uint32_t sourceColumn;
};

// PRM 19.8.16
// BrigDirectivePad
// BrigDirectivePad is used to pad out the
// .directives stream to ensure alignment.
struct BrigDirectivePad {
  enum { DirKind = BrigEDirectivePad };
  uint16_t size;
  uint16_t kind;
};

// PRM 19.8.17
struct BrigDirectivePragma {
  enum { DirKind = BrigEDirectivePragma };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 19.8.18
struct BrigDirectiveSignature {
  enum { DirKind = BrigEDirectiveSignature };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
  uint32_t outCount;
  uint32_t inCount;
  struct BrigProtoType {
    BrigDataType16_t type;
    uint8_t align;
    uint8_t hasDim;
    uint32_t dim;
  } types[1];
};

// PRM 19.8.19
struct BrigDirectiveSampler {
  enum { DirKind = BrigEDirectiveSampler };
  uint16_t size;
  uint16_t kind;
  BrigSymbolCommon s;
  uint8_t valid: 1;
  uint8_t normalized: 1;
  uint8_t filter:6;
  uint8_t boundaryU;
  uint8_t boundaryV;
  uint8_t boundaryW;
  uint32_t reserved;
};

// PRM 19.8.20
// BrigDirectiveScope
// BrigDirectiveScope is used to start or end a scope.
struct BrigDirectiveScope {
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

struct BrigDirectiveArgStart {
  enum { DirKind = BrigEDirectiveArgStart };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

struct BrigDirectiveArgEnd {
  enum { DirKind = BrigEDirectiveArgEnd };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
};

// 19.8.21
// The documentation is in error. The BrigDirectiveSymbol needs size and kind
// fields to be parsable. Otherwise, there is no way to tell if the second field
// is a uint16_t kind or the second 16-bit word of BrigSymbolCommon's c_code
// field. This view is supported by Table 20-3 in the HSA PRM.
struct BrigDirectiveSymbol {
  enum { DirKind = BrigEDirectiveSymbol };
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

// 19.8.22
struct BrigDirectiveVersion {
    enum { DirKind = BrigEDirectiveVersion };
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

// PRM 19.6.2
struct BrigBlockEnd {
  enum { DirKind = BrigEDirectiveBlockEnd };
  uint16_t size;
  uint16_t kind;
};

// PRM 19.6.3
// BrigBlockNumeric
// BrigBlockNumeric is a variable-size list of numeric values.
// All the values should have
// the same type.
// More than one BrigBlockNumeric can be
// in a single block section.
// This structure must be aligned to an 8-byte boundary
// (because of the uint64_t field).
struct BrigBlockNumeric {
  enum { DirKind = BrigEDirectiveBlockNumeric };
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

// PRM 19.6.4
// BrigBlockStart
// BrigBlockStart starts a block section.
// It provides a name that can be used to separate information used by different
// debuggers or runtimes.
// More than one BrigBlockStart can have the same name.
struct BrigBlockStart {
  enum { DirKind = BrigEDirectiveBlockStart };
  uint16_t size;
  uint16_t kind;
  BrigcOffset32_t c_code;
  BrigsOffset32_t s_name;
};

// PRM 19.6.5
struct BrigBlockString {
  enum { DirKind = BrigEDirectiveBlockString };
  uint16_t size;
  uint16_t kind;
  BrigsOffset32_t s_name;
};

// Code structures

// 19.9.9
struct BrigInstLdSt {
  enum { InstKind = BrigEInstLdSt };
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
  enum { InstKind = BrigEInstBase };
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
  enum { InstKind = BrigEInstMod };
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
  enum { InstKind = BrigEInstBar };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  uint32_t syncFlags;
};

// BrigInstAtomic
// The BrigInstAtomic format is used for atomic operations: atomicReturn and
// atomicNoReturn.
struct BrigInstAtomic {
  enum { InstKind = BrigEInstAtomic };
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

// BrigInstAtomicImage
// The BrigInstAtomicImage format is used for atomicNoReturn image operations.
struct BrigInstAtomicImage {
  enum { InstKind = BrigEInstAtomicImage };
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

// BrigInstCmp
// The BrigInstCmp format is used for compare operations.
struct BrigInstCmp {
  enum { InstKind = BrigEInstCmp };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigAluModifier aluModifier;
  BrigCompareOperation32_t comparisonOperator;
  BrigDataType16_t sourceType;
  uint16_t reserved;
};

// BrigInstCvt
// The BrigInstCvt format is used for convert operations.
struct BrigInstCvt {
  enum { InstKind = BrigEInstCvt };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigAluModifier aluModifier;
  BrigDataType16_t stype;
  uint16_t reserved;
};

// BrigInstImage
// The BrigInstImage format is used for the load image and store image
// operations.
struct BrigInstImage {
  enum { InstKind = BrigEInstImage };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigGeom32_t geom;
  BrigDataType16_t sourceType;
  uint16_t reserved;
};

// BrigInstMem
// The BrigInstMem format is used for operations that take a space modifier.
struct BrigInstMem {
  enum { InstKind = BrigEInstMem };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigStorageClass32_t storageClass;
};

struct BrigInstSegp {
  enum { InstKind = BrigEInstSegp };
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigStorageClass32_t storageClass;
  BrigDataType16_t sourceType;
  uint16_t reserved;
};

struct BrigOperandBase {
  enum { OperKind = BrigEOperandBase };
  uint16_t size;
  uint16_t kind;
};

// Operand structures
// BrigOperandReg
// BrigOperandReg is used for a register (c, s, d, or q).
struct BrigOperandReg {
  enum { OperKind = BrigEOperandReg };
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigsOffset32_t s_name;
};

// BrigOperandImmed
// BrigOperandImmed is used for a numeric value.
struct BrigOperandImmed {
  enum { OperKind = BrigEOperandImmed };
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
  enum { OperKind = BrigEOperandAddress };
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigdOffset32_t directive;
};

// BrigOperandLabelRef
// BrigOperandLabelRef is used for a label.
struct BrigOperandLabelRef {
  enum { OperKind = BrigEOperandLabelRef };
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t labeldirective;
};

// BrigOperandFunctionRef
// BrigOperandFunctionRef is used for a reference
// to a function or function signature.
struct BrigOperandFunctionRef {
  enum { OperKind = BrigEOperandFunctionRef };
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
  enum { OperKind = BrigEOperandArgumentList };
  uint16_t size;
  uint16_t kind;
  uint32_t elementCount;
  BrigdOffset32_t o_args[1];
};

struct BrigOperandFunctionList {
  enum { OperKind = BrigEOperandFunctionList };
  uint16_t size;
  uint16_t kind;
  uint32_t elementCount;
  BrigdOffset32_t o_args[1];
};

// BrigOperandArgumentRef
// BrigOperandArgumentRef is used for a single argument.
struct BrigOperandArgumentRef {
  enum { OperKind = BrigEOperandArgumentRef };
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t arg;
};

//BrigOperandCompound
//BrigOperandCompound is used for compound addressing modes.
struct BrigOperandCompound {
  enum { OperKind = BrigEOperandCompound };
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t name;
  BrigoOffset32_t reg;
  int32_t offset;
};

//BrigOperandIndirect
//BrigOperandIndirect is used for register plus offset addressing modes.
struct BrigOperandIndirect {
  enum { OperKind = BrigEOperandIndirect };
  uint16_t size;
  uint16_t kind;
  BrigoOffset32_t reg;
  BrigDataType16_t type;
  uint16_t reserved;
  int32_t offset;
};

struct BrigOperandPad {
  enum { OperKind = BrigEOperandPad };
  uint16_t size;
  uint16_t kind;
};

//BrigOperandOpaque
//BrigOperandOpaque is used for addressing image and sampler objects.
struct BrigOperandOpaque {
  enum { OperKind = BrigEOperandOpaque };
  uint16_t size;
  uint16_t kind;
  BrigdOffset32_t directive;
  BrigoOffset32_t reg;
  int32_t offset;
};

//BrigOperandRegV2
//BrigOperandRegV2 is used for certain memory operations.
struct BrigOperandRegV2 {
  enum { OperKind = BrigEOperandRegV2 };
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t regs[2];
};

//BrigOperandRegV4
//BrigOperandRegV4 is used for certain memory operations.
struct BrigOperandRegV4 {
  enum { OperKind = BrigEOperandRegV4 };
  uint16_t size;
  uint16_t kind;
  BrigDataType16_t type;
  uint16_t reserved;
  BrigoOffset32_t regs[4];
};

//BrigOperandWaveSz
//BrigOperandWaveSz is a compile-time value equal to the size of a wavefront.
struct BrigOperandWaveSz {
  enum { OperKind = BrigEOperandWaveSz };
  uint16_t size;
  uint16_t kind;
};

#endif  // INCLUDE_BRIG_H_
