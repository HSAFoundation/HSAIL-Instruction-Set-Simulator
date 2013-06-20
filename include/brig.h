//===- brig.h -------------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BRIG_H
#define BRIG_H

#include <stdint.h>

typedef uint32_t BrigDirectiveOffset32_t;
typedef uint32_t BrigCodeOffset32_t;
typedef uint32_t BrigOperandOffset32_t;
typedef uint32_t BrigStringOffset32_t;

typedef uint16_t BrigDirectiveKinds16_t;
enum BrigDirectiveKinds {
  BRIG_DIRECTIVE_ARG_SCOPE_END = 0,
  BRIG_DIRECTIVE_ARG_SCOPE_START = 1,
  BRIG_DIRECTIVE_BLOCK_END = 2,
  BRIG_DIRECTIVE_BLOCK_NUMERIC = 3,
  BRIG_DIRECTIVE_BLOCK_START = 4,
  BRIG_DIRECTIVE_BLOCK_STRING = 5,
  BRIG_DIRECTIVE_COMMENT = 6,
  BRIG_DIRECTIVE_CONTROL = 7,
  BRIG_DIRECTIVE_EXTENSION = 8,
  BRIG_DIRECTIVE_FBARRIER = 9,
  BRIG_DIRECTIVE_FILE = 10,
  BRIG_DIRECTIVE_FUNCTION = 11,
  BRIG_DIRECTIVE_IMAGE = 12,
  BRIG_DIRECTIVE_IMAGE_INIT = 13,
  BRIG_DIRECTIVE_KERNEL = 14,
  BRIG_DIRECTIVE_LABEL = 15,
  BRIG_DIRECTIVE_LABEL_INIT = 16,
  BRIG_DIRECTIVE_LABEL_TARGETS = 17,
  BRIG_DIRECTIVE_LOC = 18,
  BRIG_DIRECTIVE_PRAGMA = 19,
  BRIG_DIRECTIVE_SAMPLER = 20,
  BRIG_DIRECTIVE_SAMPLER_INIT = 22,
  BRIG_DIRECTIVE_SIGNATURE = 23,
  BRIG_DIRECTIVE_VARIABLE = 24,
  BRIG_DIRECTIVE_VARIABLE_INIT = 25,
  BRIG_DIRECTIVE_VERSION = 26
};

typedef uint16_t BrigInstKinds16_t;
enum BrigInstKinds {
  BRIG_INST_NONE = 0,
  BRIG_INST_BASIC = 1,
  BRIG_INST_ATOMIC = 2,
  BRIG_INST_ATOMIC_IMAGE = 3,
  BRIG_INST_BAR = 4,
  BRIG_INST_BR = 5,
  BRIG_INST_CMP = 6,
  BRIG_INST_CVT = 7,
  BRIG_INST_FBAR = 8,
  BRIG_INST_IMAGE = 9,
  BRIG_INST_MEM = 10,
  BRIG_INST_ADDR = 11,
  BRIG_INST_MOD = 12,
  BRIG_INST_SEG = 13,
  BRIG_INST_SOURCE_TYPE = 14
};

typedef uint16_t BrigOperandKinds16_t;
enum BrigOperandKinds {
  BRIG_OPERAND_IMMED         = 0,
  BRIG_OPERAND_WAVESIZE      = 1,
  BRIG_OPERAND_REG           = 2,
  BRIG_OPERAND_REG_VECTOR    = 3,
  BRIG_OPERAND_ADDRESS       = 4,
  BRIG_OPERAND_LABEL_REF     = 5,
  BRIG_OPERAND_ARGUMENT_REF  = 6,
  BRIG_OPERAND_ARGUMENT_LIST = 7,
  BRIG_OPERAND_FUNCTION_REF  = 8,
  BRIG_OPERAND_FUNCTION_LIST = 9,
  BRIG_OPERAND_SIGNATURE_REF = 10,
  BRIG_OPERAND_FBARRIER_REF  = 11
};

typedef uint16_t BrigAluModifier16_t;
enum BrigAluModifierMask {
  BRIG_ALU_ROUND = 15,
  BRIG_ALU_FTZ = 16
};

typedef uint8_t BrigAtomicOperation8_t;
enum BrigAtomicOperation {
  BRIG_ATOMIC_AND = 0,
  BRIG_ATOMIC_OR = 1,
  BRIG_ATOMIC_XOR = 2,
  BRIG_ATOMIC_CAS = 3,
  BRIG_ATOMIC_EXCH = 4,
  BRIG_ATOMIC_ADD = 5,
  BRIG_ATOMIC_INC = 6,
  BRIG_ATOMIC_DEC = 7,
  BRIG_ATOMIC_MIN = 8,
  BRIG_ATOMIC_MAX = 9,
  BRIG_ATOMIC_SUB = 10
};

typedef uint8_t BrigCompareOperation8_t;
enum BrigCompareOperation {
  BRIG_COMPARE_EQ = 0,
  BRIG_COMPARE_NE = 1,
  BRIG_COMPARE_LT = 2,
  BRIG_COMPARE_LE = 3,
  BRIG_COMPARE_GT = 4,
  BRIG_COMPARE_GE = 5,
  BRIG_COMPARE_EQU = 6,
  BRIG_COMPARE_NEU = 7,
  BRIG_COMPARE_LTU = 8,
  BRIG_COMPARE_LEU = 9,
  BRIG_COMPARE_GTU = 11,
  BRIG_COMPARE_GEU = 10,
  BRIG_COMPARE_NUM = 12,
  BRIG_COMPARE_NAN = 13,
  BRIG_COMPARE_SEQ = 14,
  BRIG_COMPARE_SNE = 15,
  BRIG_COMPARE_SLT = 16,
  BRIG_COMPARE_SLE = 17,
  BRIG_COMPARE_SGT = 18,
  BRIG_COMPARE_SGE = 19,
  BRIG_COMPARE_SGEU = 20,
  BRIG_COMPARE_SEQU = 21,
  BRIG_COMPARE_SNEU = 22,
  BRIG_COMPARE_SLTU = 23,
  BRIG_COMPARE_SLEU = 24,
  BRIG_COMPARE_SNUM = 25,
  BRIG_COMPARE_SNAN = 26,
  BRIG_COMPARE_SGTU = 27
};

typedef uint16_t BrigControlDirective16_t;
enum BrigControlDirective {
  BRIG_CONTROL_NONE = 0,
  BRIG_CONTROL_ENABLEBREAKEXCEPTIONS = 1,
  BRIG_CONTROL_ENABLEDETECTEXCEPTIONS = 2,
  BRIG_CONTROL_MAXDYNAMICGROUPSIZE = 3,
  BRIG_CONTROL_MAXFLATGRIDSIZE = 4,
  BRIG_CONTROL_MAXFLATWORKGROUPSIZE = 5,
  BRIG_CONTROL_REQUESTEDWORKGROUPSPERCU = 6,
  BRIG_CONTROL_REQUIREDDIM = 7,
  BRIG_CONTROL_REQUIREDGRIDSIZE = 8,
  BRIG_CONTROL_REQUIREDWORKGROUPSIZE = 9,
  BRIG_CONTROL_REQUIRENOPARTIALWORKGROUPS = 10
};

typedef uint8_t BrigExecutableModifier8_t;
enum BrigExecuteableModifierMask {
  BRIG_EXECUTABLE_LINKAGE = 3,
  BRIG_EXECUTABLE_DECLARATION = 4
};

typedef uint8_t BrigImageFormat8_t;
enum BrigImageFormat {
  BRIG_FORMAT_SNORM_INT8 = 0,
  BRIG_FORMAT_SNORM_INT16 = 1,
  BRIG_FORMAT_UNORM_INT8 = 2,
  BRIG_FORMAT_UNORM_INT16 = 3,
  BRIG_FORMAT_UNORM_SHORT_565 = 4,
  BRIG_FORMAT_UNORM_SHORT_555 = 5,
  BRIG_FORMAT_UNORM_SHORT_101010 = 6,
  BRIG_FORMAT_SIGNED_INT8 = 7,
  BRIG_FORMAT_SIGNED_INT16 = 8,
  BRIG_FORMAT_SIGNED_INT32 = 9,
  BRIG_FORMAT_UNSIGNED_INT8 = 10,
  BRIG_FORMAT_UNSIGNED_INT16 = 11,
  BRIG_FORMAT_UNSIGNED_INT32 = 12,
  BRIG_FORMAT_HALF_FLOAT = 13,
  BRIG_FORMAT_FLOAT = 14,
  BRIG_FORMAT_UNORM_INT24 = 15
};

typedef uint8_t BrigImageGeometry8_t;
enum BrigImageGeometry {
  BRIG_GEOMETRY_1D = 0,
  BRIG_GEOMETRY_2D = 1,
  BRIG_GEOMETRY_3D = 2,
  BRIG_GEOMETRY_1DA = 3,
  BRIG_GEOMETRY_1DB = 4,
  BRIG_GEOMETRY_2DA = 5
};

typedef uint8_t BrigImageOrder8_t;
enum BrigImageOrder {
  BRIG_ORDER_R = 0,
  BRIG_ORDER_A = 1,
  BRIG_ORDER_RX = 2,
  BRIG_ORDER_RG = 3,
  BRIG_ORDER_RGX = 4,
  BRIG_ORDER_RA = 5,
  BRIG_ORDER_RGB = 6,
  BRIG_ORDER_RGBA = 7,
  BRIG_ORDER_RGBX = 8,
  BRIG_ORDER_BGRA = 9,
  BRIG_ORDER_ARGB = 10,
  BRIG_ORDER_INTENSITY = 11,
  BRIG_ORDER_LUMINANCE = 12,
  BRIG_ORDER_SRGB = 13,
  BRIG_ORDER_SRGBX = 14,
  BRIG_ORDER_SRGBA = 15,
  BRIG_ORDER_SBGRA = 16
};

typedef uint8_t BrigLinkage8_t;
enum BrigLinkage {
  BRIG_LINKAGE_NONE = 0,
  BRIG_LINKAGE_STATIC = 1,
  BRIG_LINKAGE_EXTERN = 2
};

typedef uint8_t BrigMachineModel8_t;
enum BrigMachineModel {
  BRIG_MACHINE_SMALL = 0,
  BRIG_MACHINE_LARGE = 1
};

typedef uint8_t BrigMemoryFence8_t;
enum BrigMemoryFence {
  BRIG_FENCE_NONE = 0,
  BRIG_FENCE_GROUP = 1,
  BRIG_FENCE_GLOBAL = 2,
  BRIG_FENCE_BOTH = 3,
  BRIG_FENCE_PARTIAL = 4,
  BRIG_FENCE_PARTIAL_BOTH = 5
};

typedef uint16_t BrigMemoryModifier8_t;
enum BrigMemoryModifierMask {
  BRIG_MEMORY_SEMANTIC = 15,
  BRIG_MEMORY_ALIGNED = 16
};

typedef uint8_t BrigMemorySemantic8_t;
enum BrigMemorySemantic {
  BRIG_SEMANTIC_NONE = 0,
  BRIG_SEMANTIC_REGULAR = 1,
  BRIG_SEMANTIC_ACQUIRE = 2,
  BRIG_SEMANTIC_RELEASE = 3,
  BRIG_SEMANTIC_ACQUIRE_RELEASE = 4,
  BRIG_SEMANTIC_PARTIAL_ACQUIRE = 5,
  BRIG_SEMANTIC_PARTIAL_RELEASE = 6,
  BRIG_SEMANTIC_PARTIAL_ACQUIRE_RELEASE = 7
};

typedef uint16_t BrigOpcode16_t;
enum BrigOpcode {
  BRIG_OPCODE_NOP = 0,
  BRIG_OPCODE_ABS = 1,
  BRIG_OPCODE_ADD = 2,
  BRIG_OPCODE_BORROW = 3,
  BRIG_OPCODE_CARRY = 4,
  BRIG_OPCODE_CEIL = 5,
  BRIG_OPCODE_COPYSIGN = 6,
  BRIG_OPCODE_DIV = 7,
  BRIG_OPCODE_FLOOR = 8,
  BRIG_OPCODE_FMA = 9,
  BRIG_OPCODE_FRACT = 10,
  BRIG_OPCODE_MAD = 11,
  BRIG_OPCODE_MAX = 12,
  BRIG_OPCODE_MIN = 13,
  BRIG_OPCODE_MUL = 14,
  BRIG_OPCODE_MULHI = 15,
  BRIG_OPCODE_NEG = 16,
  BRIG_OPCODE_REM = 17,
  BRIG_OPCODE_RINT = 18,
  BRIG_OPCODE_SQRT = 19,
  BRIG_OPCODE_SUB = 20,
  BRIG_OPCODE_TRUNC = 21,
  BRIG_OPCODE_MAD24 = 22,
  BRIG_OPCODE_MAD24HI = 23,
  BRIG_OPCODE_MUL24 = 24,
  BRIG_OPCODE_MUL24HI = 25,
  BRIG_OPCODE_SHL = 26,
  BRIG_OPCODE_SHR = 27,
  BRIG_OPCODE_AND = 28,
  BRIG_OPCODE_NOT = 29,
  BRIG_OPCODE_OR = 30,
  BRIG_OPCODE_POPCOUNT = 31,
  BRIG_OPCODE_XOR = 32,
  BRIG_OPCODE_BITEXTRACT = 33,
  BRIG_OPCODE_BITINSERT = 34,
  BRIG_OPCODE_BITMASK = 35,
  BRIG_OPCODE_BITREV = 36,
  BRIG_OPCODE_BITSELECT = 37,
  BRIG_OPCODE_FIRSTBIT = 38,
  BRIG_OPCODE_LASTBIT = 39,
  BRIG_OPCODE_COMBINE = 40,
  BRIG_OPCODE_EXPAND = 41,
  BRIG_OPCODE_LDA = 42,
  BRIG_OPCODE_LDC = 43,
  BRIG_OPCODE_MOV = 44,
  BRIG_OPCODE_SHUFFLE = 45,
  BRIG_OPCODE_UNPACKHI = 46,
  BRIG_OPCODE_UNPACKLO = 47,
  BRIG_OPCODE_PACK = 48,
  BRIG_OPCODE_UNPACK = 49,
  BRIG_OPCODE_CMOV = 50,
  BRIG_OPCODE_CLASS = 51,
  BRIG_OPCODE_NCOS = 52,
  BRIG_OPCODE_NEXP2 = 53,
  BRIG_OPCODE_NFMA = 54,
  BRIG_OPCODE_NLOG2 = 55,
  BRIG_OPCODE_NRCP = 56,
  BRIG_OPCODE_NRSQRT = 57,
  BRIG_OPCODE_NSIN = 58,
  BRIG_OPCODE_NSQRT = 59,
  BRIG_OPCODE_BITALIGN = 60,
  BRIG_OPCODE_BYTEALIGN = 61,
  BRIG_OPCODE_PACKCVT = 62,
  BRIG_OPCODE_UNPACKCVT = 63,
  BRIG_OPCODE_LERP = 64,
  BRIG_OPCODE_SAD = 65,
  BRIG_OPCODE_SADHI = 66,
  BRIG_OPCODE_SEGMENTP = 67,
  BRIG_OPCODE_FTOS = 68,
  BRIG_OPCODE_STOF = 69,
  BRIG_OPCODE_CMP = 70,
  BRIG_OPCODE_CVT = 71,
  BRIG_OPCODE_LD = 72,
  BRIG_OPCODE_ST = 73,
  BRIG_OPCODE_ATOMIC = 74,
  BRIG_OPCODE_ATOMICNORET = 75,
  BRIG_OPCODE_RDIMAGE = 76,
  BRIG_OPCODE_LDIMAGE = 77,
  BRIG_OPCODE_STIMAGE = 78,
  BRIG_OPCODE_ATOMICIMAGE = 79,
  BRIG_OPCODE_ATOMICIMAGENORET = 80,
  BRIG_OPCODE_QUERYIMAGEARRAY = 81,
  BRIG_OPCODE_QUERYIMAGEDEPTH = 82,
  BRIG_OPCODE_QUERYIMAGEFORMAT = 83,
  BRIG_OPCODE_QUERYIMAGEHEIGHT = 84,
  BRIG_OPCODE_QUERYIMAGEORDER = 85,
  BRIG_OPCODE_QUERYIMAGEWIDTH = 86,
  BRIG_OPCODE_QUERYSAMPLERCOORD = 87,
  BRIG_OPCODE_QUERYSAMPLERFILTER = 88,
  BRIG_OPCODE_CBR = 89,
  BRIG_OPCODE_BRN = 90,
  BRIG_OPCODE_BARRIER = 91,
  BRIG_OPCODE_ARRIVEFBAR = 92,
  BRIG_OPCODE_INITFBAR = 93,
  BRIG_OPCODE_JOINFBAR = 94,
  BRIG_OPCODE_LEAVEFBAR = 95,
  BRIG_OPCODE_RELEASEFBAR = 96,
  BRIG_OPCODE_WAITFBAR = 97,
  BRIG_OPCODE_LDF = 98,
  BRIG_OPCODE_SYNC = 99,
  BRIG_OPCODE_COUNTLANE = 100,
  BRIG_OPCODE_COUNTUPLANE = 101,
  BRIG_OPCODE_MASKLANE = 102,
  BRIG_OPCODE_SENDLANE = 103,
  BRIG_OPCODE_RECEIVELANE = 104,
  BRIG_OPCODE_CALL = 105,
  BRIG_OPCODE_RET = 106,
  BRIG_OPCODE_SYSCALL = 107,
  BRIG_OPCODE_ALLOCA = 108,
  BRIG_OPCODE_CLEARDETECTEXCEPT = 109,
  BRIG_OPCODE_CLOCK = 110,
  BRIG_OPCODE_CUID = 111,
  BRIG_OPCODE_CURRENTWORKGROUPSIZE = 112,
  BRIG_OPCODE_DEBUGTRAP = 113,
  BRIG_OPCODE_DIM = 114,
  BRIG_OPCODE_DISPATCHID = 115,
  BRIG_OPCODE_DISPATCHPTR = 116,
  BRIG_OPCODE_GETDETECTEXCEPT = 117,
  BRIG_OPCODE_GRIDGROUPS = 118,
  BRIG_OPCODE_GRIDSIZE = 119,
  BRIG_OPCODE_LANEID = 120,
  BRIG_OPCODE_MAXCUID = 121,
  BRIG_OPCODE_MAXWAVEID = 122,
  BRIG_OPCODE_NULLPTR = 123,
  BRIG_OPCODE_QID = 124,
  BRIG_OPCODE_QPTR = 125,
  BRIG_OPCODE_SETDETECTEXCEPT = 126,
  BRIG_OPCODE_WAVEID = 127,
  BRIG_OPCODE_WORKGROUPID = 128,
  BRIG_OPCODE_WORKGROUPSIZE = 129,
  BRIG_OPCODE_WORKITEMABSID = 130,
  BRIG_OPCODE_WORKITEMFLATABSID = 131,
  BRIG_OPCODE_WORKITEMFLATID = 132,
  BRIG_OPCODE_WORKITEMID = 133,
  BRIG_OPCODE_INVALID
};

typedef uint8_t BrigPack8_t;
enum BrigPack {
  BRIG_PACK_NONE = 0,
  BRIG_PACK_PP = 1,
  BRIG_PACK_PS = 2,
  BRIG_PACK_SP = 3,
  BRIG_PACK_SS = 4,
  BRIG_PACK_S = 5,
  BRIG_PACK_P = 6,
  BRIG_PACK_PPSAT = 7,
  BRIG_PACK_PSSAT = 8,
  BRIG_PACK_SPSAT = 9,
  BRIG_PACK_SSSAT = 10,
  BRIG_PACK_SSAT = 11,
  BRIG_PACK_PSAT = 12
};

typedef uint8_t BrigProfile8_t;
enum BrigProfile {
  BRIG_PROFILE_BASE = 0,
  BRIG_PROFILE_FULL = 1
};

typedef uint8_t BrigRound8_t;
enum BrigRound {
  BRIG_ROUND_NONE = 0,
  BRIG_ROUND_FLOAT_NEAR_EVEN = 1,
  BRIG_ROUND_FLOAT_ZERO = 2,
  BRIG_ROUND_FLOAT_PLUS_INFINITY = 3,
  BRIG_ROUND_FLOAT_MINUS_INFINITY = 4,
  BRIG_ROUND_INTEGER_NEAR_EVEN = 5,
  BRIG_ROUND_INTEGER_ZERO = 6,
  BRIG_ROUND_INTEGER_PLUS_INFINITY = 7,
  BRIG_ROUND_INTEGER_MINUS_INFINITY = 8,
  BRIG_ROUND_INTEGER_NEAR_EVEN_SAT = 9,
  BRIG_ROUND_INTEGER_ZERO_SAT = 10,
  BRIG_ROUND_INTEGER_PLUS_INFINITY_SAT = 11,
  BRIG_ROUND_INTEGER_MINUS_INFINITY_SAT = 12
};

typedef uint8_t BrigSamplerBoundaryMode8_t;
enum BrigSamplerBoundaryMode {
  BRIG_BOUNDARY_CLAMP = 0,
  BRIG_BOUNDARY_WRAP = 1,
  BRIG_BOUNDARY_MIRROR = 2,
  BRIG_BOUNDARY_MIRRORONCE = 3,
  BRIG_BOUNDARY_BORDER = 4
};

enum BrigSamplerCoord {
  BRIG_COORD_NORMALIZED = 0,
  BRIG_COORD_UNNORMALIZED = 1
};

enum BrigSamplerFilter {
  BRIG_FILTER_NEAREST = 0,
  BRIG_FILTER_LINEAR = 1
};

typedef uint8_t BrigSamplerModifier8_t;
enum BrigSamplerModifierMask {
  BRIG_SAMPLER_FILTER = 63,
  BRIG_SAMPLER_COORD = 64,
  BRIG_SAMPLER_COORD_UNNORMALIZED = 64
};

typedef uint8_t BrigSegment8_t;
enum BrigSegment {
  BRIG_SEGMENT_NONE = 0,
  BRIG_SEGMENT_FLAT = 1,
  BRIG_SEGMENT_GLOBAL = 2,
  BRIG_SEGMENT_READONLY = 3,
  BRIG_SEGMENT_KERNARG = 4,
  BRIG_SEGMENT_GROUP = 5,
  BRIG_SEGMENT_PRIVATE = 6,
  BRIG_SEGMENT_SPILL = 7,
  BRIG_SEGMENT_ARG = 8,
  BRIG_SEGMENT_INVALID
};

typedef uint8_t BrigSymbolModifier8_t;
enum BrigSymbolModifierMask {
  BRIG_SYMBOL_LINKAGE = 3,
  BRIG_SYMBOL_DECLARATION = 4,
  BRIG_SYMBOL_CONST = 8,
  BRIG_SYMBOL_ARRAY = 16,
  BRIG_SYMBOL_FLEX_ARRAY = 32
};

enum {
  BRIG_TYPE_PACK_SHIFT = 5,
  BRIG_TYPE_BASE_MASK = (1 << BRIG_TYPE_PACK_SHIFT) - 1,
  BRIG_TYPE_PACK_MASK = 3 << BRIG_TYPE_PACK_SHIFT,
  BRIG_TYPE_PACK_NONE = 0 << BRIG_TYPE_PACK_SHIFT,
  BRIG_TYPE_PACK_32 = 1 << BRIG_TYPE_PACK_SHIFT,
  BRIG_TYPE_PACK_64 = 2 << BRIG_TYPE_PACK_SHIFT,
  BRIG_TYPE_PACK_128 = 3 << BRIG_TYPE_PACK_SHIFT
};

typedef uint16_t BrigType16_t;
enum BrigType {
  BRIG_TYPE_NONE = 0,
  BRIG_TYPE_U8 = 1,
  BRIG_TYPE_U16 = 2,
  BRIG_TYPE_U32 = 3,
  BRIG_TYPE_U64 = 4,
  BRIG_TYPE_S8 = 5,
  BRIG_TYPE_S16 = 6,
  BRIG_TYPE_S32 = 7,
  BRIG_TYPE_S64 = 8,
  BRIG_TYPE_F16 = 9,
  BRIG_TYPE_F32 = 10,
  BRIG_TYPE_F64 = 11,
  BRIG_TYPE_B1 = 12,
  BRIG_TYPE_B8 = 13,
  BRIG_TYPE_B16 = 14,
  BRIG_TYPE_B32 = 15,
  BRIG_TYPE_B64 = 16,
  BRIG_TYPE_B128 = 17,
  BRIG_TYPE_SAMP = 18,
  BRIG_TYPE_ROIMG = 19,
  BRIG_TYPE_RWIMG = 20,
  BRIG_TYPE_FBAR = 21,
  BRIG_TYPE_U8X4 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_32,
  BRIG_TYPE_U8X8 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_U8X16 = BRIG_TYPE_U8 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_U16X2 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_32,
  BRIG_TYPE_U16X4 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_U16X8 = BRIG_TYPE_U16 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_U32X2 = BRIG_TYPE_U32 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_U32X4 = BRIG_TYPE_U32 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_U64X2 = BRIG_TYPE_U64 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_S8X4 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_32,
  BRIG_TYPE_S8X8 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_S8X16 = BRIG_TYPE_S8 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_S16X2 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_32,
  BRIG_TYPE_S16X4 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_S16X8 = BRIG_TYPE_S16 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_S32X2 = BRIG_TYPE_S32 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_S32X4 = BRIG_TYPE_S32 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_S64X2 = BRIG_TYPE_S64 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_F16X2 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_32,
  BRIG_TYPE_F16X4 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_F16X8 = BRIG_TYPE_F16 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_F32X2 = BRIG_TYPE_F32 | BRIG_TYPE_PACK_64,
  BRIG_TYPE_F32X4 = BRIG_TYPE_F32 | BRIG_TYPE_PACK_128,
  BRIG_TYPE_F64X2 = BRIG_TYPE_F64 | BRIG_TYPE_PACK_128
};

typedef uint32_t BrigVersion32_t;
enum BrigVersion {
  BRIG_VERSION_HSAIL_MAJOR = 0,
  BRIG_VERSION_HSAIL_MINOR = 96,
  BRIG_VERSION_BRIG_MAJOR = 0,
  BRIG_VERSION_BRIG_MINOR = 1
};

typedef uint8_t BrigWidth8_t;
enum BrigWidth {
  BRIG_WIDTH_NONE = 0,
  BRIG_WIDTH_1 = 1,
  BRIG_WIDTH_2 = 2,
  BRIG_WIDTH_4 = 3,
  BRIG_WIDTH_8 = 4,
  BRIG_WIDTH_16 = 5,
  BRIG_WIDTH_32 = 6,
  BRIG_WIDTH_64 = 7,
  BRIG_WIDTH_128 = 8,
  BRIG_WIDTH_256 = 9,
  BRIG_WIDTH_512 = 10,
  BRIG_WIDTH_1024 = 11,
  BRIG_WIDTH_2048 = 12,
  BRIG_WIDTH_4096 = 13,
  BRIG_WIDTH_8192 = 14,
  BRIG_WIDTH_16364 = 15,
  BRIG_WIDTH_32768 = 16,
  BRIG_WIDTH_65536 = 17,
  BRIG_WIDTH_131072 = 18,
  BRIG_WIDTH_262144 = 19,
  BRIG_WIDTH_524288 = 20,
  BRIG_WIDTH_1048576 = 21,
  BRIG_WIDTH_2097152 = 22,
  BRIG_WIDTH_4194304 = 23,
  BRIG_WIDTH_8388608 = 24,
  BRIG_WIDTH_16777216 = 25,
  BRIG_WIDTH_33554432 = 26,
  BRIG_WIDTH_67108864 = 27,
  BRIG_WIDTH_134217728 = 28,
  BRIG_WIDTH_268435456 = 29,
  BRIG_WIDTH_536870912 = 30,
  BRIG_WIDTH_1073741824 = 31,
  BRIG_WIDTH_2147483648 = 32,
  BRIG_WIDTH_WAVESIZE = 33,
  BRIG_WIDTH_ALL = 34
};

struct BrigSectionHeader {
  uint32_t size;
};

struct BrigString {
  uint32_t byteCount;
  uint8_t bytes[1];
  enum { FlexSize = sizeof(uint8_t) };
};

struct BrigDebugBase {
  uint16_t size;
  BrigDirectiveKinds16_t kind;
};

struct BrigBlockEnd {
  enum { DirKind = BRIG_DIRECTIVE_BLOCK_END };
  enum { DebugKind = BRIG_DIRECTIVE_BLOCK_END };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
};

struct BrigBlockNumeric {
  enum { DirKind = BRIG_DIRECTIVE_BLOCK_NUMERIC };
  enum { DebugKind = BRIG_DIRECTIVE_BLOCK_NUMERIC };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigType16_t type;
  uint16_t reserved;
  uint32_t elementCount;
  BrigStringOffset32_t data;
};

struct BrigBlockStart {
  enum { DirKind = BRIG_DIRECTIVE_BLOCK_START };
  enum { DebugKind = BRIG_DIRECTIVE_BLOCK_START };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigBlockString {
  enum { DirKind = BRIG_DIRECTIVE_BLOCK_STRING };
  enum { DebugKind = BRIG_DIRECTIVE_BLOCK_STRING };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigStringOffset32_t string;
};

struct BrigDirectiveBase {
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
};

struct BrigDirectiveCallableBase {
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  uint16_t inArgCount;
  uint16_t outArgCount;
};

struct BrigDirectiveArgScope {
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
};

struct BrigDirectiveArgScopeStart {
  enum { DirKind = BRIG_DIRECTIVE_ARG_SCOPE_START };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
};

struct BrigDirectiveArgScopeEnd {
  enum { DirKind = BRIG_DIRECTIVE_ARG_SCOPE_END };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
};

struct BrigDirectiveComment {
  enum { DirKind = BRIG_DIRECTIVE_COMMENT };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigDirectiveControl {
  enum { DirKind = BRIG_DIRECTIVE_CONTROL };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigControlDirective16_t control;
  BrigType16_t type;
  uint16_t reserved;
  uint16_t valueCount;
  BrigOperandOffset32_t values[1];
  enum { FlexSize = sizeof(BrigOperandOffset32_t) };
};

struct BrigDirectiveExecutable {
  enum { DirKind = BrigDirectiveKinds16_t(~0) };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  uint16_t inArgCount;
  uint16_t outArgCount;
  BrigDirectiveOffset32_t firstInArg;
  BrigDirectiveOffset32_t firstScopedDirective;
  BrigDirectiveOffset32_t nextTopLevelDirective;
  uint32_t instCount;
  BrigExecutableModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveKernel {
  enum { DirKind = BRIG_DIRECTIVE_KERNEL };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  uint16_t inArgCount;
  uint16_t outArgCount;
  BrigDirectiveOffset32_t firstInArg;
  BrigDirectiveOffset32_t firstScopedDirective;
  BrigDirectiveOffset32_t nextTopLevelDirective;
  uint32_t instCount;
  BrigExecutableModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveFunction {
  enum { DirKind = BRIG_DIRECTIVE_FUNCTION };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  uint16_t inArgCount;
  uint16_t outArgCount;
  BrigDirectiveOffset32_t firstInArg;
  BrigDirectiveOffset32_t firstScopedDirective;
  BrigDirectiveOffset32_t nextTopLevelDirective;
  uint32_t instCount;
  BrigExecutableModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveExtension {
  enum { DirKind = BRIG_DIRECTIVE_EXTENSION };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigDirectiveFbarrier {
  enum { DirKind = BRIG_DIRECTIVE_FBARRIER };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigDirectiveFile {
  enum { DirKind = BRIG_DIRECTIVE_FILE };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  uint32_t fileid;
  BrigStringOffset32_t filename;
};

struct BrigDirectiveImageInit {
  enum { DirKind = BRIG_DIRECTIVE_IMAGE_INIT };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t array;
  BrigImageOrder8_t order;
  BrigImageFormat8_t format;
  uint16_t reserved;
};

struct BrigDirectiveLabel {
  enum { DirKind = BRIG_DIRECTIVE_LABEL };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigDirectiveLabelTargets {
  enum { DirKind = BRIG_DIRECTIVE_LABEL_TARGETS };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigDirectiveOffset32_t label;
  uint16_t labelCount;
  uint16_t reserved;
  BrigDirectiveOffset32_t labels[1];
};

struct BrigDirectiveLabelInit {
  enum { DirKind = BRIG_DIRECTIVE_LABEL_INIT };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigDirectiveOffset32_t label;
  uint16_t labelCount;
  uint16_t reserved;
  BrigDirectiveOffset32_t labels[1];
  enum { FlexSize = sizeof(BrigDirectiveOffset32_t) };
};

struct BrigDirectiveLoc {
  enum { DirKind = BRIG_DIRECTIVE_LOC };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  uint32_t fileid;
  uint32_t line;
  uint32_t column;
};

struct BrigDirectivePragma {
  enum { DirKind = BRIG_DIRECTIVE_PRAGMA };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
};

struct BrigDirectiveSamplerInit {
  enum { DirKind = BRIG_DIRECTIVE_SAMPLER_INIT };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigSamplerModifier8_t modifier;
  BrigSamplerBoundaryMode8_t boundaryU;
  BrigSamplerBoundaryMode8_t boundaryV;
  BrigSamplerBoundaryMode8_t boundaryW;
};

struct BrigDirectiveSignature {
  enum { DirKind = BRIG_DIRECTIVE_SIGNATURE };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  uint16_t inArgCount;
  uint16_t outArgCount;
  struct BrigSigType {
    BrigType16_t type;
    uint8_t align;
    BrigSymbolModifier8_t modifier;
    uint32_t dimLo;
    uint32_t dimHi;
  } args[1];
  enum { FlexSize = sizeof(BrigSigType) };
};

struct BrigDirectiveSymbol {
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  BrigDirectiveOffset32_t init;
  BrigType16_t type;
  BrigSegment8_t segment;
  uint8_t align;
  uint32_t dimLo;
  uint32_t dimHi;
  BrigSymbolModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveVariable {
  enum { DirKind = BRIG_DIRECTIVE_VARIABLE };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  BrigDirectiveOffset32_t init;
  BrigType16_t type;
  BrigSegment8_t segment;
  uint8_t align;
  uint32_t dimLo;
  uint32_t dimHi;
  BrigSymbolModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveImage {
  enum { DirKind = BRIG_DIRECTIVE_IMAGE };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  BrigDirectiveOffset32_t init;
  BrigType16_t type;
  BrigSegment8_t segment;
  uint8_t align;
  uint32_t dimLo;
  uint32_t dimHi;
  BrigSymbolModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveSampler {
  enum { DirKind = BRIG_DIRECTIVE_SAMPLER };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t name;
  BrigDirectiveOffset32_t init;
  BrigType16_t type;
  BrigSegment8_t segment;
  uint8_t align;
  uint32_t dimLo;
  uint32_t dimHi;
  BrigSymbolModifier8_t modifier;
  uint8_t reserved[3];
};

struct BrigDirectiveVariableInit {
  enum { DirKind = BRIG_DIRECTIVE_VARIABLE_INIT };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigStringOffset32_t data;
  uint32_t elementCount;
  BrigType16_t type;
  uint16_t reserved;
};

struct BrigDirectiveVersion {
  enum { DirKind = BRIG_DIRECTIVE_VERSION };
  uint16_t size;
  BrigDirectiveKinds16_t kind;
  BrigCodeOffset32_t code;
  BrigVersion32_t hsailMajor;
  BrigVersion32_t hsailMinor;
  BrigVersion32_t brigMajor;
  BrigVersion32_t brigMinor;
  BrigProfile8_t profile;
  BrigMachineModel8_t machineModel;
  uint16_t reserved;
};

struct BrigInstBase {
  enum { InstKind = BrigInstKinds16_t(~0) };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
};

struct BrigInstBasic {
  enum { InstKind = BRIG_INST_BASIC };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
};

struct BrigInstAddr {
  enum { InstKind = BRIG_INST_ADDR };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigSegment8_t segment;
  uint8_t reserved[3];
};

struct BrigInstAtomic {
  enum { InstKind = BRIG_INST_ATOMIC };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigSegment8_t segment;
  BrigMemorySemantic8_t memorySemantic;
  BrigAtomicOperation8_t atomicOperation;
  uint8_t reserved;
};

struct BrigInstAtomicImage {
  enum { InstKind = BRIG_INST_ATOMIC_IMAGE };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t imageType;
  BrigType16_t coordType;
  BrigImageGeometry8_t geometry;
  BrigAtomicOperation8_t atomicOperation;
  uint16_t reserved;
};

struct BrigInstBar {
  enum { InstKind = BRIG_INST_BAR };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigMemoryFence8_t memoryFence;
  BrigWidth8_t width;
  uint16_t reserved;
};

struct BrigInstBr {
  enum { InstKind = BRIG_INST_BR };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigAluModifier16_t modifier;
  BrigWidth8_t width;
  uint8_t reserved;
};

struct BrigInstCmp {
  enum { InstKind = BRIG_INST_CMP };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t sourceType;
  BrigAluModifier16_t modifier;
  BrigCompareOperation8_t compare;
  BrigPack8_t pack;
  uint16_t reserved;
};

struct BrigInstCvt {
  enum { InstKind = BRIG_INST_CVT };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t sourceType;
  BrigAluModifier16_t modifier;
};

struct BrigInstFbar {
  enum { InstKind = BRIG_INST_FBAR };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigMemoryFence8_t memoryFence;
  BrigWidth8_t width;
  uint16_t reserved;
};

struct BrigInstImage {
  enum { InstKind = BRIG_INST_IMAGE };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t imageType;
  BrigType16_t coordType;
  BrigImageGeometry8_t geometry;
  uint8_t reserved[3];
};

struct BrigInstMem {
  enum { InstKind = BRIG_INST_MEM };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigSegment8_t segment;
  BrigMemoryModifier8_t modifier;
  uint8_t equivClass;
  BrigWidth8_t width;
};

struct BrigInstMod {
  enum { InstKind = BRIG_INST_MOD };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigAluModifier16_t modifier;
  BrigPack8_t pack;
  uint8_t reserved;
};

struct BrigInstNone {
  enum { InstKind = BRIG_INST_NONE };
  uint16_t size;
  BrigInstKinds16_t kind;
};

struct BrigInstSeg {
  enum { InstKind = BRIG_INST_SEG };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t sourceType;
  BrigSegment8_t segment;
  uint8_t reserved;
};

struct BrigInstSourceType {
  enum { InstKind = BRIG_INST_SOURCE_TYPE };
  uint16_t size;
  BrigInstKinds16_t kind;
  BrigOpcode16_t opcode;
  BrigType16_t type;
  BrigOperandOffset32_t operands[5];
  BrigType16_t sourceType;
  uint16_t reserved;
};

struct BrigOperandBase {
  uint16_t size;
  BrigOperandKinds16_t kind;
};

struct BrigOperandAddress {
  enum { OperKind = BRIG_OPERAND_ADDRESS };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t symbol;
  BrigStringOffset32_t reg;
  uint32_t offsetLo;
  uint32_t offsetHi;
  BrigType16_t type;
  uint16_t reserved;
};

struct BrigOperandImmed {
  enum { OperKind = BRIG_OPERAND_IMMED };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigType16_t type;
  uint16_t byteCount;
  uint8_t bytes[1];
  enum { FlexSize = sizeof(uint8_t) };
};

struct BrigOperandList {
  uint16_t size;
  BrigOperandKinds16_t kind;
  uint16_t elementCount;
  uint16_t reserved;
  BrigDirectiveOffset32_t elements[1];
};

struct BrigOperandArgumentList {
  enum { OperKind = BRIG_OPERAND_ARGUMENT_LIST };
  uint16_t size;
  BrigOperandKinds16_t kind;
  uint16_t elementCount;
  uint16_t reserved;
  BrigDirectiveOffset32_t elements[1];
  enum { FlexSize = sizeof(BrigDirectiveOffset32_t) };
};

struct BrigOperandFunctionList {
  enum { OperKind = BRIG_OPERAND_FUNCTION_LIST };
  uint16_t size;
  BrigOperandKinds16_t kind;
  uint16_t elementCount;
  uint16_t reserved;
  BrigDirectiveOffset32_t elements[1];
  enum { FlexSize = sizeof(BrigDirectiveOffset32_t) };
};

struct BrigOperandRef {
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandArgumentRef {
  enum { OperKind = BRIG_OPERAND_ARGUMENT_REF };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandLabelRef {
  enum { OperKind = BRIG_OPERAND_LABEL_REF };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandFunctionRef {
  enum { OperKind = BRIG_OPERAND_FUNCTION_REF };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandSignatureRef {
  enum { OperKind = BRIG_OPERAND_SIGNATURE_REF };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandFbarrierRef {
  enum { OperKind = BRIG_OPERAND_FBARRIER_REF };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigDirectiveOffset32_t ref;
};

struct BrigOperandReg {
  enum { OperKind = BRIG_OPERAND_REG };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigStringOffset32_t reg;
  BrigType16_t type;
  uint16_t reserved;
};

struct BrigOperandRegVector {
  enum { OperKind = BRIG_OPERAND_REG_VECTOR };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigType16_t type;
  uint16_t regCount;
  BrigStringOffset32_t regs[1];
  enum { FlexSize = sizeof(BrigStringOffset32_t) };
};

struct BrigOperandWavesize {
  enum { OperKind = BRIG_OPERAND_WAVESIZE };
  uint16_t size;
  BrigOperandKinds16_t kind;
  BrigType16_t type;
  uint16_t reserved;
};

#endif /* BRIG_H */
