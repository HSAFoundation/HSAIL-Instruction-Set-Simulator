#include "brig_runtime.h"
#include "brig_runtime_internal.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace hsa {
namespace brig {

ForceBrigRuntimeLinkage::ForceBrigRuntimeLinkage() {}

template<class T, class U, bool S> const bool IntPolicy<T, U, S>::isSigned;
template<class T, class U, bool S> const unsigned IntPolicy<T, U, S>::Bits;
template<class T, class U, bool S> const unsigned IntPolicy<T, U, S>::ShiftMask;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::HighBit;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::Max;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::Min;

template struct IntPolicy<u8,  u8,  false>;
template struct IntPolicy<u16, u16, false>;
template struct IntPolicy<u32, u32, false>;
template struct IntPolicy<u64, u64, false>;
template struct IntPolicy<s8,  u8,  true>;
template struct IntPolicy<s16, u16, true>;
template struct IntPolicy<s32, u32, true>;
template struct IntPolicy<s64, u64, true>;

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
  if(Int<T>::isSigned) {
    T res = x + y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if(isNegX == isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x + y;
    if(res < x) return Int<T>::Max;
    return res;
  }
}
template<class T> static T AddSatVector(T x, T y) { return map(AddSat, x, y); }
SignedVectorInst(define, AddSat, Binary)
UnsignedVectorInst(define, AddSat, Binary)

template<class T> static T Div(T x, T y) {
  if(isDivisionError(x, y)) return T(0);
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
  if(Int<T>::isSigned) {
    s64 res = (s64) x * (s64) y;
    if(res > (s64) Int<T>::Max) return Int<T>::Max;
    if(res < (s64) Int<T>::Min) return Int<T>::Min;
    return res;
   } else {
    u64 res = (u64) x * (u64) y;
    if(res > (u64) Int<T>::Max) return Int<T>::Max;
    return res;
  }
}
template<class T> static T MulSatVector(T x, T y) { return map(MulSat, x, y); }
SignedVectorInst(define, MulSat, Binary)
UnsignedVectorInst(define, MulSat, Binary)

template<class T> static T MulHi(T x, T y) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = x;
  Int64Ty y64 = y;
  Int64Ty res = x64 * y64;
  return T(res >> Int<T>::Bits);
}
template<class T> static T MulHiVector(T x, T y) { return map(MulHi, x, y); }
defineBinary(MulHi, s32)
defineBinary(MulHi, u32)
SignedVectorInst(define, MulHi, Binary)
UnsignedVectorInst(define, MulHi, Binary)

template<class T> static T Sub(T x, T y) { return x - y; }
template<class T> static T SubVector(T x, T y) { return map(Sub, x, y); }
SignedInst(define, Sub, Binary)
UnsignedInst(define, Sub, Binary)
FloatInst(define, Sub, Binary)
SignedVectorInst(define, Sub, Binary)
UnsignedVectorInst(define, Sub, Binary)
FloatVectorInst(define, Sub, Binary)

template<class T> static T SubSat(T x, T y) {
  if(Int<T>::isSigned) {
    T res = x - y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if(isNegX != isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x - y;
    if(res > x) return Int<T>::Min;
    return res;
  }
}
template<class T> static T SubSatVector(T x, T y) { return map(SubSat, x, y); }
SignedVectorInst(define, SubSat, Binary)
UnsignedVectorInst(define, SubSat, Binary)

template<class T> static T Max(T x, T y) {
  if(isNan(x)) return y;
  if(isNan(y)) return x;
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
  if(isNan(x)) return y;
  if(isNan(y)) return x;
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
  if(isDivisionError(x, y)) return T(0);
  return x % y;
}
SignedInst(define, Rem, Binary)
UnsignedInst(define, Rem, Binary)

template <class T> static T Carry(T x, T y) {
  typedef typename Int<T>::Unsigned Unsigned;
  if ((Unsigned) x + (Unsigned) y < (Unsigned) x) return T(1);
  else return T(0);
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
FloatInst(define, Mad, Ternary)

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
  for(unsigned i = 0; i < Vec<T>::Len; i += 2) {
    ((Vec<T>) result)[i]     = ((Vec<T>) x)[i / 2];
    ((Vec<T>) result)[i + 1] = ((Vec<T>) y)[i / 2];
  }
  return result;
}
UnpackInst(define, UnpackLo, Binary)

template<class T> static T UnpackHi(T x, T y) {
  T result;
  unsigned Len = Vec<T>::Len;
  for(unsigned i = 0; i < Len; i += 2) {
    ((Vec<T>) result)[i]     = ((Vec<T>) x)[i / 2 + Len / 2];
    ((Vec<T>) result)[i + 1] = ((Vec<T>) y)[i / 2 + Len / 2];
  }
  return result;
}
UnpackInst(define, UnpackHi, Binary)

template<class T> static T And(T x, T y) { return x & y; }
BitInst(define, And, Binary)

template<class T> static T Or(T x, T y) { return x | y; }
BitInst(define, Or, Binary)

template<class T> static T Xor(T x, T y) { return x ^ y; }
BitInst(define, Xor, Binary)

template<class T> static T Not(T x) { return ~x; }
template<> bool Not(bool x) { return !x; }
BitInst(define, Not, Unary)

static b32 Popcount(b32 x) { return __builtin_popcount(x); }
static b64 Popcount(b64 x) { return __builtin_popcountl(x); }
defineUnary(Popcount, b32)
defineUnary(Popcount, b64)

// Bit reverse implementation loosely adapted from Sean Eron Anderson's article
// at: http://graphics.stanford.edu/~seander/bithacks.html
template<class T> static T Bitrev(T x) {
  typedef typename Int<T>::Unsigned Unsigned;
  Unsigned v = Unsigned(x); // input bits to be reversed
  Unsigned r = v; // r will be reversed bits of v; first get LSB of v
  int s = Int<T>::Bits - 1; // extra shift needed at end

  for (v >>= 1; v; v >>= 1) {
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s; // shift when v's highest bits are zero

  return T(r);
}
SignedInst(define, Bitrev, Unary)
UnsignedInst(define, Bitrev, Unary)

template<class T> static T Extract(T x, unsigned y, unsigned z) {
  unsigned offset = Int<T>::ShiftMask & y;
  unsigned width  = Int<T>::ShiftMask & z;
  return (x << offset) >> (Int<T>::Bits - width);
}
SignedInst(define, Extract, Ternary)
UnsignedInst(define, Extract, Ternary)

template<class T> static T Bitselect(T x, T y, T z) {
  return (y & x) | (z & ~x);
}
SignedInst(define, Bitselect, Ternary)
UnsignedInst(define, Bitselect, Ternary)

template<class T> static T Firstbit(T x) {
  if(Int<T>::isNeg(x)) x = ~x;
  if(!x) return ~T(0);
  T pos = T(0);
  while(!(x & Int<T>::HighBit)) {
    x <<= 1;
    ++pos;
  }
  return pos;
}
SignedInst(define, Firstbit, Unary)
UnsignedInst(define, Firstbit, Unary)

template<class T> static T Lastbit(T x) {
  if(!x) return ~T(0);
  T pos = T(0);
  while(!(x & 1)) {
    x >>= 1;
    ++pos;
  }
  return pos;
}
SignedInst(define, Lastbit, Unary)
UnsignedInst(define, Lastbit, Unary)

extern "C" b64 Mov_b64_b32(b32 x, b32 y) { return (b64(x) << 32) | b64(y); }
extern "C" b64 Mov_b64_b64(b64 x) { return x; }

// Assumes little-endian
extern "C" b128 Mov_b128_b32(b32 w, b32 x, b32 y, b32 z) {
  b128 result;
  ((Vec<b128>) result)[0] = z;
  ((Vec<b128>) result)[1] = y;
  ((Vec<b128>) result)[2] = x;
  ((Vec<b128>) result)[3] = w;
  return result;
}
extern "C" b128 Mov_b128_b128(b128 x) { return x; }

extern "C" b32 Movs_lo_b32(b64 x) { return x; }
extern "C" b32 Movs_hi_b32(b64 x) { return x >> 32; }

extern "C" b64 Movd_lo_b64(b64 x, b32 y) { return (x >> 32 << 32) | b64(y); }
extern "C" b64 Movd_hi_b64(b64 x, b32 y) {
  return (b64(y) << 32) | (x >> 32);
}

template<class T> static T ShuffleVector(T x, T y, b32 z) {

  unsigned len   = Vec<T>::Len;
  unsigned mask  = len - 1;
  unsigned shift = Vec<T>::LogLen;
  b32 shuffle = z;
  T result = { 0 };

  for(unsigned i = 0; i < len / 2; ++i) {
    unsigned offset = shuffle & mask;
    ((Vec<T>) result)[i] = ((Vec<T>) x)[offset];
    shuffle >>= shift;
  }

  for(unsigned i = len / 2; i < len; ++i) {
    unsigned offset = shuffle & mask;
    ((Vec<T>) result)[i] = ((Vec<T>) y)[offset];
    shuffle >>= shift;
  }

  return result;
}
ShuffleVectorInst(define, Shuffle, Ternary)

template<class T> static T Cmov(T x, T y, T z) {
  return (x & 1) ? y : z;
}
BitInst(define, Cmov, Ternary)

// Neither C++98 nor C++11 implement C99's floating point hexadecimal literals. :(
// 0x3F7FFFFE == 0x1.fffffep-1f
// 0x3FEFFFFFFFFFFFFF == 0x1.fffffffffffffp-1
extern "C" f32 Fract_f32(f32 f) {
  union { b32 b; f32 f; } AlmostOne = { 0x3F7FFFFE };
  return std::min(f - std::floor(f), AlmostOne.f);
}

extern "C" f64 Fract_f64(f64 d) {
  union { b64 b; f64 d; } AlmostOne = { 0x3FEFFFFFFFFFFFFF };
  return std::min(d - std::floor(d), AlmostOne.d);
}

template<class T> static T Sqrt(T x) { return std::sqrt(x); }
FloatInst(define, Sqrt, Unary)

template<class T> static T Fma(T x, T y, T z) { return fma(x, y, z); }
FloatInst(define, Fma, Ternary)

template<class T> static T Copysign(T x, T y) { return copysign(x, y); }
FloatInst(define, Copysign, Binary)

template<class T> static b1 Class(T x, b32 y) {
  int fpclass = std::fpclassify(x);
  if(y & SNan && isSNan(x)) return true;
  if(y & QNan && isQNan(x)) return true;
  if(y & NegInf && isNegInf(x)) return true;
  if(y & NegNorm && fpclass == FP_NORMAL && x < 0) return true;
  if(y & NegSubnorm && fpclass == FP_SUBNORMAL && x < 0) return true;
  if(y & NegZero && isNegZero(x)) return true;
  if(y & PosZero && isPosZero(x)) return true;
  if(y & PosSubnorm && fpclass == FP_SUBNORMAL && x > 0) return true;
  if(y & PosNorm && fpclass == FP_NORMAL && x > 0) return true;
  if(y & PosInf && isPosInf(x)) return true;
  return false;
}
extern "C" b1 Class_f32(f32 f, b32 y) { return Class(f, y); }
extern "C" b1 Class_f64(f64 f, b32 y) { return Class(f, y); }

} // namespace brig
} // namespace hsa
