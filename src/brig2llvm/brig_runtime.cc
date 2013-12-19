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

#if defined(__clang__)
#include <dlfcn.h>
#endif // defined(__clang__)

#include <unistd.h>

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

#if defined(__i386__) || defined(__x86_64__)
static u64 rdtscp(u32 &cpuId) {
  u32 tickLow, tickHigh;
  __asm__ __volatile__("rdtscp" :
                       "=a"(tickLow), "=d"(tickHigh), "=c"(cpuId));
  return ((u64) tickHigh << 32) | tickLow;
}
#endif // defined(__i386__) || defined(__x86_64__)

static u64 rdtsc(void) {
#if defined(__i386__) || defined(__x86_64__)
  u32 cpuId;
  return rdtscp(cpuId);
#endif // defined(__i386__) || defined(__x86_64__)
}

// On RHEL 5, neither sched_getcpu nor the getcpu syscall are
// available. Consequently, on i386 and x86-64 architectures, we determine the
// CPU by invoking the rdtscp instruction directly. sched_getcpu was introduced
// in glibc 2.6. RHEL 5 uses 2.5. The getcpu syscall was introduced in Linux
// 2.6.19. RHEL 5 uses 2.6.18.
static u32 getCPU() {
#if defined(__i386__) || defined(__x86_64__)
  u32 cpuId;
  rdtscp(cpuId);
  return cpuId;
#endif // defined(__i386__) || defined(__x86_64__)
}

#if defined(__clang__)
// The glibc fenv.h header file defines an extern inline and extern version of
// feraiseexcept and the extern inline version calls the inline version using
// asm renaming. In GCC, asm renaming happens at assembly time, so everything
// works as expected, but in clang, asm renaming happens at parse time, so clang
// tries to call the extern inline version instead of the extern
// version. Getting the address of feraiseexcept from dlsym at runtime yields
// the extern version.
#define feraiseexcept(X) FERaiseExcept(X)
static inline int FERaiseExcept(int except) {
  typedef int (*FERaiseExceptTy)(int);
  static const void *symbol = dlsym(NULL, "feraiseexcept");
  FERaiseExceptTy FERaiseExcept = *(FERaiseExceptTy *) &symbol;
  return FERaiseExcept(except);
}
#endif // defined(__clang__)

// On i386, the x87 FPU does not respect the FTZ mode. We use the
// -mfpmath=sse compile flag to ensure the compiler uses SSE floating
// point instead of x87, but sometimes we call libraries that use the
// x87 FPU. The fixFTZ macro does a floating point addition using
// SSE. If the input to the operation is a denormal, it will be
// flushed to zero; otherwise, the operation will be unchanged. Even
// though (X + 0) appears to be an identity operation, correct
// compilers cannot optimize it away. See Section F.9.2 of the C11
// specification for details.

// Unfortunatey, on some x86-64 platforms (CentOS 6.5) certain
// floating point operations (trunc and ceil) are implemented in
// software as a series of bit operations rather than using the
// appropriate SSE instruction. Manipulating floating point values
// with bitwise operations bypasses the flush to zero mode. Since
// there is no easy way to check whether these operations are
// implemented in hardware or software, the FTZ workaround is enabled
// unconditionally. It might be possible to have CMAKE detect whether
// various floating point operations respect the processor's FTZ mode
// and selectively enable the workaround, but it seems like too much
// work to avoid a single floating point add on platforms with good
// libraries.

#define fixFTZ(X) (X + 0)

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

template<class T> static T Ceil(T t) { return std::ceil(fixFTZ(t)); }
template<class T> static T CeilVector(T t) { return map(Ceil, t); }
FloatInst(define, Ceil, Unary)
FloatVectorInst(define, Ceil, Unary)

template<class T> static T Floor(T t) { return std::floor(fixFTZ(t)); }
template<class T> static T FloorVector(T t) { return map(Floor, t); }
FloatInst(define, Floor, Unary)
FloatVectorInst(define, Floor, Unary)

static float  Trunc(float t)  { return ::truncf(fixFTZ(t)); }
static double Trunc(double t) { return ::trunc(fixFTZ(t)); }
template<class T> static T TruncVector(T t) { return map(Trunc, t); }
FloatInst(define, Trunc, Unary)
FloatVectorInst(define, Trunc, Unary)

template<class T> static T Rint(T t)  {
  int oldMode = fegetround();
  fesetround(FE_TONEAREST);
  volatile T result = T(nearbyint(fixFTZ(t)));
  fesetround(oldMode);
  return result;
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
template<class T> static T Add_Sat(T x, T y) {
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
template<class T> static T Add_SatVector(T x, T y) {
  return map(Add_Sat, x, y);
}
SignedVectorInst(define, Add_Sat, Binary)
UnsignedVectorInst(define, Add_Sat, Binary)

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

template<class T> static T Mul_Sat(T x, T y) {
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
template<class T> static T Mul_SatVector(T x, T y) {
  return map(Mul_Sat, x, y);
}
SignedVectorInst(define, Mul_Sat, Binary)
UnsignedVectorInst(define, Mul_Sat, Binary)

template<class T> static T MulHi(T x, T y) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty x64 = Int64Ty(x);
  Int64Ty y64 = Int64Ty(y);

  Int64Ty res = x64 * y64;
  return T(res >> Int<T>::Bits);
}
template<class T> static u64 MulHi64(T x, T y) {
  T x_lo = x & 0xFFFFFFFF;
  T x_hi = x >> 32;
  T y_lo = y & 0xFFFFFFFF;
  T y_hi = y >> 32;

  T prod1 = x_lo*y_lo;
  T prod3 = y_hi*x_hi;
  T prod2 = (x_hi + x_lo)*(y_hi + y_lo) - prod1 - prod3;
  T temp = prod2 + (prod1 >> 32);
  // lower part of the product:
  //  T res_lo =  (prod1 & 0xFFFFFFFF) +
  //              ((temp & 0xFFFFFFFF) << 32);
  T res_hi = prod3 + (temp >> 32);
  return res_hi;
}
template<> u64 MulHi(u64 x, u64 y) { return MulHi64(x, y); }
template<> s64 MulHi(s64 x, s64 y) { return MulHi64(x, y); }
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

template<class T> static T Sub_Sat(T x, T y) {
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
template<class T> static T Sub_SatVector(T x, T y) {
  return map(Sub_Sat, x, y);
}
SignedVectorInst(define, Sub_Sat, Binary)
UnsignedVectorInst(define, Sub_Sat, Binary)

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
  b32 width  = z & b32(Int<T>::Bits - 1);
  b32 offset = y & b32(Int<T>::Bits - 1);
  Unsigned mask = (1 << width) - 1;
  w &= ~(mask << offset);
  return w | ((x & mask) << offset);
}
SignedInst(define, BitInsert, Quaternary)
UnsignedInst(define, BitInsert, Quaternary)

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
// 0x3F7FFFFF == 0x1.fffffep-1f
// 0x3FEFFFFFFFFFFFFF == 0x1.fffffffffffffp-1
extern "C" f32 Fract_f32(f32 f) {
  union { b32 b; f32 f; } AlmostOne = { 0x3F7FFFFF };
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

// Pass by reference to avoid SNAN to QNAN conversion on i386.
template<class T> static b1 Class(const T &x, b32 y) {
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
extern "C" b1 Class_b1_f32(f32 f, b32 y) { return Class(f, y); }
extern "C" b1 Class_b1_f64(f64 f, b32 y) { return Class(f, y); }

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
  unsigned shift = y & 31;
  b64 temp = (b64(x) << 32) | b64(w);
  return (temp >> shift);
}

extern "C" b32 ByteAlign_b32(b32 w, b32 x, b32 y) {
  return BitAlign_b32(w, x, y * 8);
}

extern "C" u8x4 Lerp_u8x4(u8x4 w, u8x4 x, u8x4 y) {
  u8x4 result = 0;
  for (unsigned i = 0; i < 4; ++i) {
    result[i] = (w[i] + x[i] + (y[i] & 0x1)) >> 1;
  }
  return result;
}

extern "C" u32 Sad_u32_u32(u32 w, u32 x, u32 y) {
  return abs(w - x) + y;
}

extern "C" u32 Sad_u32_u16x2(u16x2 w, u16x2 x, u32 y) {
  u32 result = 0;
  for (unsigned i = 0; i < 2; ++i)
    result += Sad_u32_u32(w[i], x[i], 0);

  return result + y;
}

extern "C" u32 Sad_u32_u8x4(u8x4 w, u8x4 x, u32 y) {
  u32 result = 0;
  for (unsigned i = 0; i < 4; ++i)
    result += Sad_u32_u32(w[i], x[i], 0);

  return result + y;
}

extern "C" u16x2 Sadhi_u16x2_u8x4(u8x4 w, u8x4 x, u16x2 y) {
  u32 temp_result = 0;
  for (unsigned i = 0; i < 4; ++i)
    temp_result += Sad_u32_u32(w[i], x[i], 0);

  u16x2 result = y;
  result[1] += temp_result;

  return result;
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

// PRM 5.18.4
// Regular integer rounding: if the value is out of range, the result
// is undefined and an invalid operation exception is generated
//
// Saturating integer rounding: the value is clamped to the range of
// the destination type, with NaN converted to 0
// Integer rounding:
// f32 to Int
template<class R> static R Cvt(volatile f32 f, int mode) {
  int oldMode = fegetround();
  fesetround(mode);
  typedef typename Int<R>::Int64Ty Int64Ty;
  volatile Int64Ty result = Int64Ty(nearbyint(f));
  if (result < getMin<R>() || result > getMax<R>())
    feraiseexcept(FE_INVALID);
  if (f < 0 && !Int<R>::isSigned)
    feraiseexcept(FE_INVALID);
  fesetround(oldMode);
  return R(result);
}

template<class R> static R Cvt_sat(volatile f32 f, int mode) {
  if (isPosInf(f)) return getMax<R>();
  if (isNegInf(f)) return getMin<R>();
  if (f > getMax<R>()) return getMax<R>();
  if (f < getMin<R>()) return getMin<R>();
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
// f64 to Int
template<class R> static R Cvt(volatile f64 f, int mode) {
  int oldMode = fegetround();
  fesetround(mode);
  typedef typename Int<R>::Int64Ty Int64Ty;
  volatile Int64Ty result = Int64Ty(nearbyint(f));
  if (result < getMin<R>() || result > getMax<R>())
    feraiseexcept(FE_INVALID);
  if (f < 0 && !Int<R>::isSigned)
    feraiseexcept(FE_INVALID);
  fesetround(oldMode);
  return R(result);
}

#if defined(__clang__) && defined(__x86_64__)
// x86-64 does not have an instruction to convert from floating point to
// u64. gcc and clang both shift the floating point value to a value in the s64
// range and then shift back after conversion. In the gcc implementation, there
// is a branch to control how the u64 should be converted. The clang
// implementation computes both values and then selects between
// them. Consequently, the clang implementation will incorrectly raise an
// FE_INVALID exception when converting from a float to a u64 when the input
// value is between Int<u64>::HighBit and Int<u64>::Max. For more details, read
// LLVM's lib/Target/X86/README-X86-64.txt (search for cvttss2siq) and
// lib/CodeGen/SelectionDAG/LegalizeDAG.cpp (search for ISD::FP_TO_UINT in
// ExpandNode).
template<> u64 Cvt(volatile f32 f, int mode) {
  if (f < 0) feraiseexcept(FE_INVALID);
  if (f >= Int<u64>::HighBit)
    return u64(Cvt<s64>(f - Int<u64>::HighBit, mode)) + Int<u64>::HighBit;
  return u64(Cvt<s64>(f, mode));
}
template<> u64 Cvt(volatile f64 f, int mode) {
  if (f < 0) feraiseexcept(FE_INVALID);
  if (f >= Int<u64>::HighBit)
    return u64(Cvt<s64>(f - Int<u64>::HighBit, mode)) + Int<u64>::HighBit;
  return u64(Cvt<s64>(f, mode));
}
#endif // defined(__clang__) && defined(__x86_64__)

template<class R> static R Cvt_sat(volatile f64 f, int mode) {
  if (isPosInf(f)) return getMax<R>();
  if (isNegInf(f)) return getMin<R>();
  if (f > getMax<R>()) return getMax<R>();
  if (f < getMin<R>()) return getMin<R>();
  if (isNan(f)) return 0;
  if (!~mode) return R(f);
  int oldMode = fegetround();
  fesetround(mode);
  volatile R result = R(nearbyint(f));
  fesetround(oldMode);
  return result;
}

// Float to Float
// same size or larger size
template<> f32 Cvt(f32 f, int mode) {
  return f32(f);
}
template<> f64 Cvt(f64 f, int mode) {
  return f64(f);
}
template<> f64 Cvt(f32 f, int mode) {
  return f64(f);
}

// Floating point rounding:
// f64 to f32
template<> f32 Cvt(volatile f64 f, int mode) {
  int oldMode = fegetround();
  fesetround(mode);
  volatile f32 result = f32(f);
  fesetround(oldMode);
  return result;
}
template<> bool Cvt(f64 f, int mode) { return f != 0.0; }
// Floating point rounding:
// Int to Int, Int to f32, Int to f64
template<class R, class T> static R Cvt(volatile T t, int mode)  {
  if (!~mode) return R(t);
  int oldMode = fegetround();
  fesetround(mode);
  volatile R result = R(t);
  fesetround(oldMode);
  return result;
}

extern "C" u8x4 PackCvt_u8x4_f32(f32 src0, f32 src1, f32 src2, f32 src3) {
  u8x4 result;
  result[0] = Cvt_sat<u8>(src0, FE_TONEAREST);
  result[1] = Cvt_sat<u8>(src1, FE_TONEAREST);
  result[2] = Cvt_sat<u8>(src2, FE_TONEAREST);
  result[3] = Cvt_sat<u8>(src3, FE_TONEAREST);
  return result;
}

extern "C" f32 UnpackCvt_f32_u8x4(u8x4 src0, u32 src1) {
  if (src1 > 3)
    return 0;
  return src0[src1];
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

// Conversion from a floating point to integer requires
// an integer rounding
RFICvt(define)

// saturating float to int
RFICvtSat(define)

// Conversion from integer to floating-point requires
// a floating-point rounding
RIFCvt(define)

// Boolean conversions
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

// b1 to f must not specify rounding
defineCvt(Cvt,      ~0,            f32, b1)
defineCvt(Cvt,      ~0,            f64, b1)

// Conversion from a floating point to a floating point with
// smaller size requires a floating-point rounding
defineCvt(Cvt_up,   FE_UPWARD,      f32, f64)
defineCvt(Cvt_down, FE_DOWNWARD,    f32, f64)
defineCvt(Cvt_zero, FE_TOWARDZERO,  f32, f64)
defineCvt(Cvt_near, FE_TONEAREST,   f32, f64)

// A rounding modifier is illegal in conversion from f to f
// with the same size or larger size, and in conversion
// from f to b1 and vice-versa, and in conversion from
// b1, s or u to b1, s or u
RIICvt(define)
defineCvt(Cvt, ~0, f64, f32)
defineCvt(Cvt, ~0, f32, f32)
defineCvt(Cvt, ~0, f64, f64)

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

extern "C" u64 Clock_u64(void) {
  return rdtsc();
}

extern "C" u32 Dim_u32(void) {
  return __brigThreadInfo->workdim;
}

extern "C" u32 WorkItemAbsId_u32(u32 x) {
  if (x >= Dim_u32()) return 0;
  return __brigThreadInfo->workItemAbsId[x];
}

extern "C" u32 GridSize_u32(u32 x) {
  if (x > Dim_u32()) return 1;
  return __brigThreadInfo->NDRangeSize[x];
}

extern "C" u32 WorkGroupSize_u32(u32 x) {
  if (x >= Dim_u32()) return 1;
  return __brigThreadInfo->workGroupSize[x];
}

extern "C" u32 GridGroups_u32(u32 x) {
  return (GridSize_u32(x) + WorkGroupSize_u32(x) - 1) / WorkGroupSize_u32(x);
}

extern "C" u32 CurrentWorkGroupSize_u32(u32 x) {
  return WorkGroupSize_u32(x);
}

extern "C" u32 WorkItemId_u32(u32 x) {
  if (x >= Dim_u32()) return 0;
  return WorkItemAbsId_u32(x) % WorkGroupSize_u32(x);
}

extern "C" u32 WorkGroupId_u32(u32 x) {
  if (x >= Dim_u32()) return 0;
  return WorkItemAbsId_u32(x) / WorkGroupSize_u32(x);
}

extern "C" u32 WorkItemFlatAbsId_u32(void) {
  return
    WorkItemAbsId_u32(0) +
    WorkItemAbsId_u32(1) * GridSize_u32(0) +
    WorkItemAbsId_u32(2) * GridSize_u32(0) * GridSize_u32(1);
}

extern "C" u32 WorkItemFlatId_u32(void) {
  return
    WorkItemId_u32(0) +
    WorkItemId_u32(1) * WorkGroupSize_u32(0) +
    WorkItemId_u32(2) * WorkGroupSize_u32(0) * WorkGroupSize_u32(1);
}

extern "C" u32 LaneId_u32(void) {
  return 0;
}

extern "C" u32 MaxWaveId_u32(void) {
  return 0;
}

extern "C" u32 WaveId_u32(void) {
  return 0;
}

extern "C" u32 MaxCuId_u32(void) {
  return sysconf(_SC_NPROCESSORS_ONLN) - 1;
}

extern "C" u32 CuId_u32(void) {
  return getCPU();
}

extern "C" void Nop(void) {}

extern "C" u32 NullPtr_u32(void) { return 0; }
extern "C" u64 NullPtr_u64(void) { return 0; }

static u32 hsaToCFPE(u32 hsaFlag) {
  u32 feFlag = 0;
  if (hsaFlag & HSA_INVALID)   feFlag |= FE_INVALID;
  if (hsaFlag & HSA_DIVBYZERO) feFlag |= FE_DIVBYZERO;
  if (hsaFlag & HSA_OVERFLOW)  feFlag |= FE_OVERFLOW;
  if (hsaFlag & HSA_UNDERFLOW) feFlag |= FE_UNDERFLOW;
  if (hsaFlag & HSA_INEXACT)   feFlag |= FE_INEXACT;
  return feFlag;
}

static u32 cToHSAFPE(u32 feFlag) {
  u32 hsaFlag = 0;
  if (feFlag & FE_INVALID)   hsaFlag |= HSA_INVALID;
  if (feFlag & FE_DIVBYZERO) hsaFlag |= HSA_DIVBYZERO;
  if (feFlag & FE_OVERFLOW)  hsaFlag |= HSA_OVERFLOW;
  if (feFlag & FE_UNDERFLOW) hsaFlag |= HSA_UNDERFLOW;
  if (feFlag & FE_INEXACT)   hsaFlag |= HSA_INEXACT;
  return hsaFlag;
}

extern "C" void ClearDetectExcept_u32(u32 x) {
  feclearexcept(hsaToCFPE(x));
}

extern "C" u32 GetDetectExcept_u32(void) {
  return cToHSAFPE(fetestexcept(FE_ALL_EXCEPT));
}

extern "C" void SetDetectExcept_u32(u32 x) {
  feraiseexcept(hsaToCFPE(x));
}

extern "C" u32 Qid_u32(void) { return __brigThreadInfo->queueId; }

extern "C" u32 QPtr_u32(void) { return (u32)(size_t) __brigThreadInfo->queue; }
extern "C" u64 QPtr_u64(void) { return (u64)(size_t) __brigThreadInfo->queue; }

extern "C" u64 DispatchId_u64(void) { return __brigThreadInfo->dispatchId; }

extern "C" u32 DispatchPtr_u32(void) {
  return (u32)(size_t)__brigThreadInfo->dispatch;
}
extern "C" u64 DispatchPtr_u64(void) {
  return (u64) __brigThreadInfo->dispatch;
}

}  // namespace brig
}  // namespace hsa
