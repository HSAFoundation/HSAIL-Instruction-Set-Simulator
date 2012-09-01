#ifndef BRIG_RUNTIME_H
#define BRIG_RUNTIME_H

#include <stdint.h>

namespace hsa {
namespace brig {

struct ForceBrigRuntimeLinkage {
  ForceBrigRuntimeLinkage();
};

} // namespace brig
} // namespace hsa

typedef bool       b1;
typedef uint8_t    b8;
typedef uint8_t    u8;
typedef  int8_t    s8;
typedef uint16_t   b16;
typedef uint16_t   u16;
typedef  int16_t   s16;
typedef uint32_t   b32;
typedef uint32_t   u32;
typedef  int32_t   s32;
typedef uint64_t   b64;
typedef uint64_t   u64;
typedef  int64_t   s64;
typedef b64        b128[2];

// typedef __half f16; // Not a real half yet.
typedef float  f32;
typedef double f64;

#define vector(X) __attribute__((vector_size(X)))

#define declareVector(TYPE,LEN)                             \
  typedef TYPE TYPE ## x ## LEN vector(sizeof(TYPE) * LEN)

declareVector(u8,  4);
declareVector(s8,  4);
declareVector(u8,  8);
declareVector(s8,  8);
declareVector(u8,  16);
declareVector(s8,  16);
declareVector(u16, 2);
declareVector(s16, 2);
// declareVector(f16, 2);
declareVector(u16, 4);
declareVector(s16, 4);
// declareVector(f16, 4);
declareVector(u16, 8);
declareVector(s16, 8);
// declareVector(f16, 8);
declareVector(u32, 2);
declareVector(s32, 2);
declareVector(f32, 2);
declareVector(u32, 4);
declareVector(s32, 4);
declareVector(f32, 4);
declareVector(u64, 2);
declareVector(s64, 2);
declareVector(f64, 2);

#undef declareVector
#undef vector

#endif // BRIG_RUNTIME_H
