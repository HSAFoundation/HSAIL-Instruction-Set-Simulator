//===- brig_runtime.cc ----------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_runtime.h"
#include "brig_runtime_internal.h"

#if defined(__i386__) || defined(__x86_64__)
#include <pmmintrin.h>
#endif  // defined(__i386__) || defined(__x86_64__)

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace hsa {
namespace brig {

ForceBrigRuntimeLinkage::ForceBrigRuntimeLinkage() {}

static __thread ThreadInfo *__brigThreadInfo;

extern "C" void __setThreadInfo(ThreadInfo *info) { __brigThreadInfo = info; }

extern "C" void enableFtzMode(void) {
#if defined(__i386__) || defined(__x86_64__)
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif  // defined(__i386__) || defined(__x86_64__)

#if defined(__arm__)
  __asm__ volatile("vmrs r0, fpscr\n"
                   "orr r0, $(1 << 24)\n"
                   "vmsr fpscr, r0" : : : "r0");
#endif  // defined(__arm__)
}

extern "C" void disableFtzMode(void) {
#if defined(__i386__) || defined(__x86_64__)
  _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
  _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
#endif  // defined(__i386__) || defined(__x86_64__)

#if defined(__arm__)
  __asm__ volatile("vmrs r0, fpscr\n"
                   "bic r0, $(1 << 24)\n"
                   "vmsr fpscr, r0" : : : "r0");
#endif  // defined(__arm__)
}

extern "C" void setRoundingMode_near(void) {
  fesetround(FE_TONEAREST);
}

extern "C" void setRoundingMode_zero(void) {
  fesetround(FE_TOWARDZERO);
}

extern "C" void setRoundingMode_up(void) {
  fesetround(FE_UPWARD);
}

extern "C" void setRoundingMode_down(void) {
  fesetround(FE_DOWNWARD);
}

extern "C" unsigned getWavefrontSize(void) { return 1; }

template<class T> static T Abs(T t) { return std::abs(t); }
template<class T> static T AbsVector(T t) { return map(Abs, t); }
SignedInst(define, Abs, Unary)
FloatInst(define, Abs, Unary)
SignedVectorInst(define, Abs, Unary)
FloatVectorInst(define, Abs, Unary)

template<class T> static T Neg(T x) { return T(-x); }
template<class T> static T NegVector(T t) { return map(Neg, t); }
SignedInst(define, Neg, Unary)
FloatInst(define, Neg, Unary)
SignedVectorInst(define, Neg, Unary)
FloatVectorInst(define, Neg, Unary)

template<class T> static T Ceil(T t) { return std::ceil(t); }
template<class T> static T CeilVector(T t) { return map(Ceil, t); }
FloatInst(define, Ceil, Unary)
FloatVectorInst(define, Ceil, Unary)

template<class T> static T Floor(T t) { return std::floor(t); }
template<class T> static T FloorVector(T t) { return map(Floor, t); }
FloatInst(define, Floor, Unary)
FloatVectorInst(define, Floor, Unary)

template<class T> static T Trunc(T t) {
  if (t >= 0)
    return std::floor(t);
  else
    return std::ceil(t);
}
template<class T> static T TruncVector(T t) { return map(Trunc, t); }
FloatInst(define, Trunc, Unary)
FloatVectorInst(define, Trunc, Unary)

template<class T> static T Rint(T a) {
  if (isNan(a) || isInf(a))
    return a;

  T ceil_a = std::ceil(a);
  T floor_a = std::floor(a);

  if (ceil_a - a < a - floor_a) {
    return ceil_a;
  } else if (ceil_a - a > a - floor_a) {
    return floor_a;
  } else {
    uint64_t int_ceil = (uint64_t) ceil_a;
    if (int_ceil % 2 == 0)
      return ceil_a;
    else
      return floor_a;
  }
}
template<class T> static T RintVector(T t) { return map(Rint, t); }
FloatInst(define, Rint, Unary)
FloatVectorInst(define, Rint, Unary)

template<class T> static T Add(T x, T y) { return x + y; }
template<class T> static T AddVector(T x, T y) { return map(Add, x, y); }
SignedInst(define, Add, Binary)
UnsignedInst(define, Add, Binary)
FloatInst(define, Add, Binary)
SignedVectorInst(define, Add, Binary)
UnsignedVectorInst(define, Add, Binary)
FloatVectorInst(define, Add, Binary)

// Saturated arithmetic implementation loosely adapted from Steven Fuerest's
// article at: http://locklessinc.com/articles/sat_arithmetic/
template<class T> static T AddSat(T x, T y) {
  if (Int<T>::isSigned) {
    T res = x + y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if (isNegX == isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x + y;
    if (res < x) return Int<T>::Max;
    return res;
  }
}
template<class T> static T AddSatVector(T x, T y) { return map(AddSat, x, y); }
SignedVectorInst(define, AddSat, Binary)
UnsignedVectorInst(define, AddSat, Binary)

template<class T> static T Div(T x, T y) {
  if (isDivisionError(x, y)) return T(0);
  return x / y;
}
SignedInst(define, Div, Binary)
UnsignedInst(define, Div, Binary)
FloatInst(define, Div, Binary)

template<class T> static T Mul(T x, T y) { return x * y; }
template<class T> static T MulVector(T x, T y) { return map(Mul, x, y); }
SignedInst(define, Mul, Binary)
UnsignedInst(define, Mul, Binary)
FloatInst(define, Mul, Binary)
SignedVectorInst(define, Mul, Binary)
UnsignedVectorInst(define, Mul, Binary)
FloatVectorInst(define, Mul, Binary)

template<class T> static T MulSat(T x, T y) {
  if (Int<T>::isSigned) {
    s64 res = (s64) x * (s64) y;
    if (res > (s64) Int<T>::Max) return Int<T>::Max;
    if (res < (s64) Int<T>::Min) return Int<T>::Min;
    return res;
  } else {
    u64 res = (u64) x * (u64) y;
    if (res > (u64) Int<T>::Max) return Int<T>::Max;
    return res;
  }
}
template<class T> static T MulSatVector(T x, T y) { return map(MulSat, x, y); }
SignedVectorInst(define, MulSat, Binary)
UnsignedVectorInst(define, MulSat, Binary)

template<class T> static T MulHi(T x, T y) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int64Ty(x);
  Int64Ty y64 = Int64Ty(y);

  if (Int<T>::Bits <= 32) {
    Int64Ty res = x64 * y64;
    return T(res >> Int<T>::Bits);
  } else {  // (Int<T>::Bits == 64) {
    Int64Ty x_lo = x64 & 0xFFFFFFFF;
    Int64Ty x_hi = x64 >> 32;
    Int64Ty y_lo = y64 & 0xFFFFFFFF;
    Int64Ty y_hi = y64 >> 32;

    Int64Ty prod1 = x_lo*y_lo;
    Int64Ty prod3 = y_hi*x_hi;
    Int64Ty prod2 = (x_hi + x_lo)*(y_hi + y_lo) - prod1 - prod3;
    Int64Ty temp = prod2 + (prod1 >> 32);
    // lower part of the product:
    //  Int64Ty res_lo =  (prod1 & 0xFFFFFFFF) +
    //                    ((temp & 0xFFFFFFFF) << 32);
    Int64Ty res_hi = prod3 + (temp >> 32);
    return res_hi;
  }
}
template<class T> static T MulHiVector(T x, T y) { return map(MulHi, x, y); }
defineBinary(MulHi, s32)
defineBinary(MulHi, u32)
defineBinary(MulHi, u64)
defineBinary(MulHi, s64)
SignedVectorMulHi(define)
UnsignedVectorMulHi(define)

template<class T> static T Sub(T x, T y) { return x - y; }
template<class T> static T SubVector(T x, T y) { return map(Sub, x, y); }
SignedInst(define, Sub, Binary)
UnsignedInst(define, Sub, Binary)
FloatInst(define, Sub, Binary)
SignedVectorInst(define, Sub, Binary)
UnsignedVectorInst(define, Sub, Binary)
FloatVectorInst(define, Sub, Binary)

template<class T> static T SubSat(T x, T y) {
  if (Int<T>::isSigned) {
    T res = x - y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if (isNegX != isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x - y;
    if (res > x) return Int<T>::Min;
    return res;
  }
}
template<class T> static T SubSatVector(T x, T y) { return map(SubSat, x, y); }
SignedVectorInst(define, SubSat, Binary)
UnsignedVectorInst(define, SubSat, Binary)

template<class T> static T Max(T x, T y) {
  if (isNan(x)) return y;
  if (isNan(y)) return x;
  return std::max(x, y);
}
template<class T> static T MaxVector(T x, T y) { return map(Max, x, y); }
SignedInst(define, Max, Binary)
UnsignedInst(define, Max, Binary)
FloatInst(define, Max, Binary)
SignedVectorInst(define, Max, Binary)
UnsignedVectorInst(define, Max, Binary)
FloatVectorInst(define, Max, Binary)

template<class T> static T Min(T x, T y) {
  if (isNan(x)) return y;
  if (isNan(y)) return x;
  return std::min(x, y);
}
template<class T> static T MinVector(T x, T y) { return map(Min, x, y); }
SignedInst(define, Min, Binary)
UnsignedInst(define, Min, Binary)
FloatInst(define, Min, Binary)
SignedVectorInst(define, Min, Binary)
UnsignedVectorInst(define, Min, Binary)
FloatVectorInst(define, Min, Binary)

template<class T> static T Rem(T x, T y) {
  if (isDivisionError(x, y)) return T(0);
  return x % y;
}
SignedInst(define, Rem, Binary)
UnsignedInst(define, Rem, Binary)

template <class T> static T Carry(T x, T y) {
  typedef typename Int<T>::Unsigned Unsigned;
  if ((Unsigned) x + (Unsigned) y < (Unsigned) x)
    return T(1);
  else
    return T(0);
}
SignedInst(define, Carry, Binary)
UnsignedInst(define, Carry, Binary)

template<class T> static T Borrow(T x, T y) {
  typedef typename Int<T>::Unsigned Unsigned;
  Unsigned ux = Unsigned(x);
  Unsigned uy = Unsigned(y);
  return ux - uy > ux;
}
SignedInst(define, Borrow, Binary)
UnsignedInst(define, Borrow, Binary)

template<class T> static T Mad(T x, T y, T z) {
  return x * y + z;
}
SignedInst(define, Mad, Ternary)
UnsignedInst(define, Mad, Ternary)

template<class T> static T Mul24(T x, T y) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int24Ty(x);
  Int64Ty y64 = Int24Ty(y);
  return T(x64 * y64 & 0xFFFFFFFF);
}
SignedInst(define, Mul24, Binary)
UnsignedInst(define, Mul24, Binary)

template<class T> static T Mul24Hi(T x, T y) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int24Ty(x);
  Int64Ty y64 = Int24Ty(y);
  return T(Int12Ty((x64 * y64) >> 32));
}
SignedInst(define, Mul24Hi, Binary)
UnsignedInst(define, Mul24Hi, Binary)

template<class T> static T Mad24(T x, T y, T z) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int24Ty(x);
  Int64Ty y64 = Int24Ty(y);
  Int64Ty z64 = Int64Ty(z);
  return T((x64 * y64 + z64) & 0xFFFFFFFF);
}
SignedInst(define, Mad24, Ternary)
UnsignedInst(define, Mad24, Ternary)

template<class T> static T Mad24Hi(T x, T y, T z) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int24Ty(x);
  Int64Ty y64 = Int24Ty(y);
  Int64Ty z64 = Int64Ty(z);
  return T(Int12Ty((x64 * y64 + z64) >> 32));
}
SignedInst(define, Mad24Hi, Ternary)
UnsignedInst(define, Mad24Hi, Ternary)

template<class T> static T Fma(T x, T y, T z) { return fma(x, y, z); }
FloatInst(define, Fma, Ternary)

template<class T> static T Shl(T x, unsigned y) {
  return x << (y & Int<T>::ShiftMask);
}
template<class T> static T ShlVector(T x, unsigned y) { return map(Shl, x, y); }
ShiftInst(define, Shl, Binary)

template<class T> static T Shr(T x, unsigned y) {
  return x >> (y & Int<T>::ShiftMask);
}
template<class T> static T ShrVector(T x, unsigned y) { return map(Shr, x, y); }
ShiftInst(define, Shr, Binary)

template<class T> static T UnpackLo(T x, T y) {
  T result;
  for (unsigned i = 0; i < T::Len; i += 2) {
    result[i]     = x[i / 2];
    result[i + 1] = y[i / 2];
  }
  return result;
}
UnpackInst(define, UnpackLo, Binary)

template<class T> static T UnpackHi(T x, T y) {
  T result;
  unsigned Len = T::Len;
  for (unsigned i = 0; i < Len; i += 2) {
    result[i]     = x[i / 2 + Len / 2];
    result[i + 1] = y[i / 2 + Len / 2];
  }
  return result;
}
UnpackInst(define, UnpackHi, Binary)

template<class D, class S> static D Pack(D src0, S src1, u32 src2) {
  D result = src0;
  unsigned Len = D::Len;
  unsigned index = src2 & (Len-1);
  result[index] = src1;
  return result;
}
PackInst(define)

template<class D, class S> static D Unpack(S src0, u32 src1) {
  if (src1 >= S::Len)
    return 0;
  else
    return D(src0[src1]);
}
UnpackInst2(define)

template<class T> static T And(T x, T y) { return x & y; }
BitInst(define, And, Binary)

template<class T> static T Or(T x, T y) { return x | y; }
BitInst(define, Or, Binary)

template<class T> static T Xor(T x, T y) { return x ^ y; }
BitInst(define, Xor, Binary)

template<class T> static T Not(T x) { return ~x; }
template<> bool Not(bool x) { return !x; }
BitInst(define, Not, Unary)

static u32 PopCount_u32(b32 x) { return __builtin_popcount(x); }
static u32 PopCount_u32(b64 x) { return __builtin_popcountll(x); }
defineUnary(PopCount_u32, b32)
defineUnary(PopCount_u32, b64)

// Bit reverse implementation loosely adapted from Sean Eron Anderson's article
// at: http://graphics.stanford.edu/~seander/bithacks.html
template<class T> static T BitRev(T x) {
  typedef typename Int<T>::Unsigned Unsigned;
  Unsigned v = Unsigned(x);  // input bits to be reversed
  Unsigned r = v;  // r will be reversed bits of v; first get LSB of v
  int s = Int<T>::Bits - 1;  // extra shift needed at end

  for (v >>= 1; v; v >>= 1) {
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;  // shift when v's highest bits are zero

  return T(r);
}
defineUnary(BitRev, b32)
defineUnary(BitRev, b64)

template<class T> static T BitExtract(T x, b32 y, b32 z) {
  unsigned offset = Int<T>::ShiftMask & y;
  unsigned width  = Int<T>::ShiftMask & z;
  if (!width) return 0;
  return (x << (Int<T>::Bits - width - offset)) >> (Int<T>::Bits - width);
}
SignedInst(define, BitExtract, Ternary)
UnsignedInst(define, BitExtract, Ternary)

template<class T> static T BitInsert(T w, T x, b32 y, b32 z) {
  typedef typename Int<T>::Unsigned Unsigned;
  b32 width  = y & b32(Int<T>::Bits - 1);
  b32 offset = z & b32(Int<T>::Bits - 1);
  Unsigned mask = ~(~T(0) << width) << offset;
  mask = ~mask;
  w &= mask;
  return w | (x << offset);
}
defineQuaternary(BitInsert, b32)
defineQuaternary(BitInsert, b64)

template<class T> static T BitMask(b32 y, b32 z) {
  unsigned offset = Int<T>::ShiftMask & y;
  unsigned width  = Int<T>::ShiftMask & z;
  if (!width) return 0;
  T mask = (1 << width) - 1;
  return mask << offset;
}
defineBitMask(b32)
defineBitMask(b64)

template<class T> static T BitSelect(T x, T y, T z) {
  return (y & x) | (z & ~x);
}
defineTernary(BitSelect, b32)
defineTernary(BitSelect, b64)

template<class T> static T FirstBit_u32(T x) {
  if (Int<T>::isNeg(x)) x = ~x;
  if (!x) return ~T(0);
  T pos = T(0);
  while (!(x & Int<T>::HighBit)) {
    x <<= 1;
    ++pos;
  }
  return pos;
}
SignedInst(define, FirstBit_u32, Unary)
UnsignedInst(define, FirstBit_u32, Unary)

template<class T> static T LastBit_u32(T x) {
  if (!x) return ~T(0);
  T pos = T(0);
  while (!(x & 1)) {
    x >>= 1;
    ++pos;
  }
  return pos;
}
SignedInst(define, LastBit_u32, Unary)
UnsignedInst(define, LastBit_u32, Unary)

template<class T> static T Mov(T x) { return x; }
BitInst(define, Mov, Unary)
extern "C" b128 Mov_b128(b128 x) { return x; }

extern "C" b64 Combine_b64_b32(b64 x) { return x; }
extern "C" b128 Combine_b128_b32(b128 x) { return x; }
extern "C" b128 Combine_b128_b64(b128 x) { return x; }

template<class T> static T Lda(T x) { return x; }
UnsignedInst(define, Lda, Unary)

template<class T> static T Ldc(T x) { return x; }
UnsignedInst(define, Ldc, Unary)

template<class T> static T ShuffleVector(T x, T y, b32 z) {
  unsigned len   = T::Len;
  unsigned mask  = len - 1;
  unsigned shift = T::LogLen;
  b32 shuffle = z;
  T result;

  for (unsigned i = 0; i < len / 2; ++i) {
    unsigned offset = shuffle & mask;
    result[i] = x[offset];
    shuffle >>= shift;
  }

  for (unsigned i = len / 2; i < len; ++i) {
    unsigned offset = shuffle & mask;
    result[i] = y[offset];
    shuffle >>= shift;
  }

  return result;
}
ShuffleVectorInst(define, Shuffle, Ternary)

template<class T> static T Cmov(T x, T y, T z) {
  return (x == 0) ? z : y;
}
BitInst(define, Cmov, Ternary)

template<class T> static T CmovVector(T x, T y, T z) {
  return map(Cmov, x, y, z);
}
SignedVectorInst(define, Cmov, Ternary)
UnsignedVectorInst(define, Cmov, Ternary)
FloatVectorInst(define, Cmov, Ternary)

// Neither C++98 nor C++11 implement C99's floating point hexadecimal
// literals. :(
// 0x3F7FFFFE == 0x1.fffffep-1f
// 0x3FEFFFFFFFFFFFFF == 0x1.fffffffffffffp-1
extern "C" f32 Fract_f32(f32 f) {
  union { b32 b; f32 f; } AlmostOne = { 0x3F7FFFFE };
  return std::min(f - std::floor(f), AlmostOne.f);
}

extern "C" f64 Fract_f64(f64 d) {
  union { b64 b; f64 d; } AlmostOne = { 0x3FEFFFFFFFFFFFFFULL };
  return std::min(d - std::floor(d), AlmostOne.d);
}

template<class T> static T Sqrt(T x) { return std::sqrt(x); }
FloatInst(define, Sqrt, Unary)

template<class T> static T Nsqrt(T x) { return std::sqrt(x); }
FloatInst(define, Nsqrt, Unary)

template<class T> static T NFma(T x, T y, T z) { return fma(x, y, z); }
FloatInst(define, NFma, Ternary)

template<class T> static T CopySign(T x, T y) { return copysign(x, y); }
FloatInst(define, CopySign, Binary)

template<class T> static b1 Class(T x, b32 y) {
  int fpclass = std::fpclassify(x);
  if (y & SNan && isSNan(x)) return true;
  if (y & QNan && isQNan(x)) return true;
  if (y & NegInf && isNegInf(x)) return true;
  if (y & NegNorm && fpclass == FP_NORMAL && x < 0) return true;
  if (y & NegSubnorm && fpclass == FP_SUBNORMAL && x < 0) return true;
  if (y & NegZero && isNegZero(x)) return true;
  if (y & PosZero && isPosZero(x)) return true;
  if (y & PosSubnorm && fpclass == FP_SUBNORMAL && x > 0) return true;
  if (y & PosNorm && fpclass == FP_NORMAL && x > 0) return true;
  if (y & PosInf && isPosInf(x)) return true;
  return false;
}
extern "C" b1 Class_f32(f32 f, b32 y) { return Class(f, y); }
extern "C" b1 Class_f64(f64 f, b32 y) { return Class(f, y); }

extern "C" f32 Ncos_f32(f32 x) {
  if (isNan(x)) return x;
  if (isInf(x)) return NAN;
  if (-512 * M_PI <= x && x <= 512 * M_PI) {
    return std::cos(x);
  } else {
    return std::cos(1.0);
  }
}

extern "C" f32 Nsin_f32(f32 x) {
  if (isNan(x)) return x;
  if (isInf(x)) return NAN;
  if (std::fpclassify(x) == FP_SUBNORMAL) {
    return copysign(0.0, x);
  }
  if (-512 * M_PI <= x && x <= 512 * M_PI) {
    return std::sin(x);
  } else {
    return std::sin(1.0);
  }
}

extern "C" f32 Nlog2_f32(f32 x) {
  if (std::fpclassify(x) == FP_SUBNORMAL) {
    return -INFINITY;
  } else if (std::fpclassify(x) == FP_NORMAL && x < 0) {
    return -INFINITY;
  } else {
    return log2(x);
  }
}

extern "C" f32 Nexp2_f32(f32 x) {
  if (std::fpclassify(x) == FP_NORMAL && x < 0) {
    return 0.0;
  } else {
    return exp2(x);
  }
}

template<class T> static T Nrsqrt(T x) {
  if (std::fpclassify(x) == FP_SUBNORMAL) {
    return x > 0 ? INFINITY : -INFINITY;
  } else {
    return  T(1.0) / std::sqrt(x);
  }
}
FloatInst(define, Nrsqrt, Unary)

template<class T> static T Nrcp(T x) {
  if (std::fpclassify(x) == FP_SUBNORMAL) {
    return x > 0 ? INFINITY : -INFINITY;
  } else {
    return T(1.0) / x;
  }
}
FloatInst(define, Nrcp, Unary)

extern "C" b32 BitAlign_b32(b32 w, b32 x, b32 y) {
  return (b64(w) << y) | (b64(x) >> (32 - y));
}

extern "C" b32 ByteAlign_b32(b32 w, b32 x, b32 y) {
  return BitAlign_b32(w, x, y * 8);
}

extern "C" b32 Lerp_b32(b32 w, b32 x, b32 y) {
  b32 result = 0;
  for (unsigned i = 0; i < 4; ++i) {
    result |= (((((w >> 8 * i) & 0xFF)
                 + ((x >> 8 * i) & 0xFF)
                 + ((y >> 8 * i) & 0x1)) >> 1) & 0xFF) << 8 * i;
  }
  return result;
}

extern "C" u32 Sad_u32_u32(u32 w, u32 x, u32 y) {
  return abs(w - x) + y;
}

extern "C" u32 Sad_u32_u16x2(u32 w, u32 x, u32 y) {
  u32 result = 0;
  for (unsigned i = 0; i < 2; ++i) {
    result += Sad_u32_u32((w >> i * 16) & 0xFFFF,
                          (x >> i * 16) & 0xFFFF, 0);
  }
  return result + y;
}

extern "C" u32 Sad_u32_u8x4(u32 w, u32 x, u32 y) {
  u32 result = 0;
  for (unsigned i = 0; i < 4; ++i) {
    result += Sad_u32_u32((w >> i * 8) & 0xFF,
                          (x >> i * 8) & 0xFF, 0);
  }
  return result + y;
}

extern "C" u32 Sadhi_u16x2_u8x4(u32 w, u32 x, u32 y) {
  u32 result = 0;
  for (unsigned i = 0; i < 4; ++i) {
    result += Sad_u32_u32((w >> i * 8) & 0xFF,
                          (x >> i * 8) & 0xFF, 0);
  }
  return (result << 16) + y;
}

CmpInst(eq, x == y)
CmpInst(ne, !isUnordered(x, y) && x != y)
CmpInst(lt, x <  y)
CmpInst(le, x <= y)
CmpInst(gt, x >  y)
CmpInst(ge, x >= y)

Cmp(define, eq, b1)
Cmp(define, ne, b1)

CmpImpl(num,  !isNan(x) && !isNan(y))
CmpImpl(snum, !isNan(x) && !isNan(y))
CmpImpl(nan,   isNan(x) ||  isNan(y))
CmpImpl(snan,  isNan(x) ||  isNan(y))

// Cmp(define, num, f16)
Cmp(define, num,  f32)
Cmp(define, num,  f64)
// Cmp(define, snum, f16)
Cmp(define, snum, f32)
Cmp(define, snum, f64)
// Cmp(define, nan, f16)
Cmp(define, nan,  f32)
Cmp(define, nan,  f64)
// Cmp(define, snan, f16)
Cmp(define, snan, f32)
Cmp(define, snan, f64)

// Integer rounding:
// f32 to Int, f32 to f32, f32 to f64
template<class R> static R Cvt(f32 f, int mode) {
  if (isPosInf(f)) return getMax<R>();
  if (isNegInf(f)) return getMin<R>();
  if (isNan(f)) return 0;
  if (!~mode) return R(f);
  int oldMode = fegetround();
  fesetround(mode);
  volatile R result = R(nearbyint(f));
  fesetround(oldMode);
  return result;
}
template<> bool Cvt(f32 f, int mode) { return f != 0.0f; }
// Integer rounding:
// f64 to Int, f64 to f64
template<class R> static R Cvt(f64 f, int mode) {
  if (isPosInf(f)) return getMax<R>();
  if (isNegInf(f)) return getMin<R>();
  if (isNan(f)) return 0;
  if (!~mode) return R(f);
  int oldMode = fegetround();
  fesetround(mode);
  volatile R result = R(nearbyint(f));
  fesetround(oldMode);
  return result;
}
// Floating point rounding:
// f64 to f32
template<> f32 Cvt(f64 f, int mode) {
  int oldMode = fegetround();
  fesetround(mode);
  volatile f32 result = f32(f);
  fesetround(oldMode);
  return result;
}
template<> bool Cvt(f64 f, int mode) { return f != 0.0; }
// Floating point rounding:
// Int to Int, Int to f32, Int to f64
template<class R, class T> static R Cvt(T t, int mode)  {
  if (!~mode) return R(t);
  int oldMode = fegetround();
  fesetround(mode);
  volatile R result = R(t);
  fesetround(oldMode);
  return result;
}

#if defined(__arm__)
// Handle long to double conversions on platforms without hardware support
// (ARM).
template<class T> static f64 l2d(T x, int mode) {
  double d = x;
  T y = d;
  switch (mode) {
  case FE_UPWARD: return y >= x ? d : nexttoward(d, INFINITY);
  case FE_DOWNWARD: return y <= x ? d : nexttoward(d, -INFINITY);
  case FE_TOWARDZERO: return llabs(y) <= llabs(x) ? d : nexttoward(d, 0);
  default: return d;
  }
}
template<> f64 Cvt(u64 x, int mode) { return l2d(x, mode); }
template<> f64 Cvt(s64 x, int mode) { return l2d(x, mode); }
#endif  // defined(__arm__)

RIICvt(define)
RFICvt(define)
RIFCvt(define)
// Boolean conversions
defineCvt(Cvt,       ~0,            b1,  b1)
defineCvt(Cvt,       ~0,            b1,  s8)
defineCvt(Cvt,       ~0,            b1,  u8)
defineCvt(Cvt,       ~0,            b1,  s16)
defineCvt(Cvt,       ~0,            b1,  u16)
defineCvt(Cvt,       ~0,            b1,  s32)
defineCvt(Cvt,       ~0,            b1,  u32)
defineCvt(Cvt,       ~0,            b1,  f32)
defineCvt(Cvt,       ~0,            b1,  s64)
defineCvt(Cvt,       ~0,            b1,  u64)
defineCvt(Cvt,       ~0,            b1,  f64)
// Converting from a floating point to a floating point of the same type takes
// an integer rounding modifier
defineCvt(Cvt,       ~0,            f32, f32)
defineCvt(Cvt,       ~0,            f64, f64)
defineCvt(Cvt_upi,   FE_UPWARD,     f32, f32)
defineCvt(Cvt_upi,   FE_UPWARD,     f64, f64)
defineCvt(Cvt_downi, FE_DOWNWARD,   f32, f32)
defineCvt(Cvt_downi, FE_DOWNWARD,   f64, f64)
defineCvt(Cvt_zeroi, FE_TOWARDZERO, f32, f32)
defineCvt(Cvt_zeroi, FE_TOWARDZERO, f64, f64)
defineCvt(Cvt_neari, FE_TONEAREST,  f32, f32)
defineCvt(Cvt_neari, FE_TONEAREST,  f64, f64)
// A rounding modifier is required where precision may be lost
defineCvt(Cvt_up,   FE_UPWARD,     f32, f64)
defineCvt(Cvt_down, FE_DOWNWARD,   f32, f64)
defineCvt(Cvt_zero, FE_TOWARDZERO, f32, f64)
defineCvt(Cvt_near, FE_TONEAREST,  f32, f64)
// A rounding modifier is illegal in all other cases
defineCvt(Cvt, ~0, f64, f32)

template<class T> static T Ld(T *x) { return *x; }
#define defineLd(X)                             \
  extern "C" X Ld_ ## X(X *x) { return Ld(x); }
defineLd(u8)
defineLd(u16)
defineLd(u32)
defineLd(u64)
defineLd(s8)
defineLd(s16)
defineLd(s32)
defineLd(s64)
// defineLd(f16)
defineLd(f32)
defineLd(f64)

template<class T> static void St(T x, T *y) { *y = x; }
#define defineSt(X)                                         \
  extern "C" void St_ ## X(X x, X *y) { return St(x, y); }
defineSt(u8)
defineSt(u16)
defineSt(u32)
defineSt(u64)
defineSt(s8)
defineSt(s16)
defineSt(s32)
defineSt(s64)
// defineSt(f16)
defineSt(f32)
defineSt(f64)
defineSt(b128)

template<class T> static T AtomicAnd(T *x, T y) {
  return __sync_fetch_and_and(x, y);
}
AtomicInst(define, And, Binary)

template<class T> static T AtomicOr(T *x, T y) {
  return __sync_fetch_and_or(x, y);
}
AtomicInst(define, Or, Binary)

template<class T> static T AtomicXor(T *x, T y) {
  return __sync_fetch_and_xor(x, y);
}
AtomicInst(define, Xor, Binary)

template<class T> static T AtomicCas(T *x, T y, T z) {
  return __sync_val_compare_and_swap(x, y, z);
}
AtomicInst(define, Cas, Ternary)

template<class T> static T AtomicExch(T *x, T y) {
  return __sync_val_compare_and_swap(x, *x, y);
}
AtomicInst(define, Exch, Binary)

template<class T> static T AtomicAdd(T *x, T y) {
  return __sync_fetch_and_add (x, y);
}
AtomicInst(define, Add, Binary)

template<class T> static T AtomicSub(T *x, T y) {
  return __sync_fetch_and_sub (x, y);
}
AtomicInst(define, Sub, Binary)

template<class T> static T AtomicInc(T *x) {
  return __sync_fetch_and_add (x, 1);
}
AtomicInst(define, Inc, Unary)

template<class T> static T AtomicDec(T *x) {
  return __sync_fetch_and_sub (x, 1);
}
AtomicInst(define, Dec, Unary)

template<class T> static T AtomicMax(volatile T *x, T y) {
  T oldVal;
  T max;
  do {
    oldVal = *x;
    max = std::max(oldVal, y);
  } while (!__sync_bool_compare_and_swap(x, oldVal, max));
  return max;
}
AtomicInst(define, Max, Binary)

template<class T> static T AtomicMin(volatile T *x, T y) {
  T oldVal;
  T min;
  do {
    oldVal = *x;
    min = std::min(oldVal, y);
  } while (!__sync_bool_compare_and_swap(x, oldVal, min));
  return min;
}
AtomicInst(define, Min, Binary)

extern "C" void Barrier(void) {
  pthread_barrier_wait(__brigThreadInfo->barrier);
}

extern "C" void Sync(void) {
  __sync_synchronize();
}

extern "C" u32 WorkItemAbsId_u32(u32 x) {
  return __brigThreadInfo->workItemAbsId[x];
}

extern "C" u32 WorkGroupSize_u32(u32 x) {
  return __brigThreadInfo->workGroupSize[x];
}

}  // namespace brig
}  // namespace hsa
