#include "brig_runtime_test_internal.h"

#include "gtest/gtest.h"

using hsa::brig::cmpResult;
using hsa::brig::ForEach;
using hsa::brig::Int;
using hsa::brig::Int48Ty;
using hsa::brig::Int24Ty;
using hsa::brig::Int12Ty;
using hsa::brig::isInf;
using hsa::brig::isNan;
using hsa::brig::isNegInf;
using hsa::brig::isNegZero;
using hsa::brig::isPosInf;
using hsa::brig::isPosZero;
using hsa::brig::isQNan;
using hsa::brig::isSNan;
using hsa::brig::isUnordered;
using hsa::brig::Vec;

template<class T> static void AbsLogic(T result, T a) {
  if(!isNan(result) && !isNan(a))  {
    EXPECT_TRUE(result == a || result == -a);
    // Do a cast back to T since operator- turns subword types into ints.
    EXPECT_LE((T) -a, result);
  } else {
    EXPECT_PRED1(isNan<T>, a);
    EXPECT_PRED1(isNan<T>, result);
  }
}
template<class T> static void AbsVectorLogic(T result, T a) {
  ForEach(AbsLogic, result, a);
}
TestAll(SignedInst,       Abs, Unary)
TestAll(FloatInst,        Abs, Unary)
TestAll(SignedVectorInst, Abs, Unary)
TestAll(FloatVectorInst,  Abs, Unary)

template<class T> static void NegLogic(T result, T a) {
  if(isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
    EXPECT_NE(std::signbit(result), std::signbit(a));
  } else {
    EXPECT_EQ(T(-result), a);
  }
}
template<class T> static void NegVectorLogic(T result, T a) {
  ForEach(NegLogic, result, a);
}
TestAll(SignedInst,         Neg, Unary)
TestAll(FloatInst,          Neg, Unary)
TestAll(SignedVectorInst,   Neg, Unary)
TestAll(FloatVectorInst,    Neg, Unary)

template<class T> static void AddLogic(T result, T a, T b) {
  if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if((isPosInf(a) && isNegInf(b))) {
    EXPECT_PRED1(isNan<T>, result);
  } else if((isNegInf(a) && isPosInf(b))) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(result, T(a + b));
  }
}
template<class T> static void AddVectorLogic(T result, T a, T b) {
  ForEach(AddLogic, result, a, b);
}
TestAll(SignedInst,         Add, Binary)
TestAll(UnsignedInst,       Add, Binary)
TestAll(FloatInst,          Add, Binary)
TestAll(SignedVectorInst,   Add, Binary)
TestAll(UnsignedVectorInst, Add, Binary)
TestAll(FloatVectorInst,    Add, Binary)

template<class T> static void AddSatLogic(T result, T a, T b) {
  s64 result64 = s64(a) + s64(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void AddSatVectorLogic(T result, T a, T b) {
  ForEach(AddSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   AddSat, Binary)
TestAll(UnsignedVectorInst, AddSat, Binary)

template<class T> static void DivLogic(T result, T a, T b) {
  if(hsa::brig::isDivisionError(a, b)) {
    EXPECT_EQ(result, 0);
  } else if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(a == 0 && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a) && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a / b), result);
  }
}
template<class T> static void DivVectorLogic(T result, T a, T b) {
  ForEach(DivLogic, result, a, b);
}
TestAll(SignedInst,         Div, Binary)
TestAll(UnsignedInst,       Div, Binary)
TestAll(FloatInst,          Div, Binary)

template<class T> static void MulLogic(T result, T a, T b) {
  if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(a == 0 && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a * b), result);
  }
}
template<class T> static void MulVectorLogic(T result, T a, T b) {
  ForEach(MulLogic, result, a, b);
}
TestAll(SignedInst,         Mul, Binary)
TestAll(UnsignedInst,       Mul, Binary)
TestAll(FloatInst,          Mul, Binary)
TestAll(SignedVectorInst,   Mul, Binary)
TestAll(UnsignedVectorInst, Mul, Binary)
TestAll(FloatVectorInst,    Mul, Binary)

template<class T> static void MulSatLogic(T result, T a, T b) {
  // Handle signed and unsigned ints differently to avoid overflow into the sign
  // bit.
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty result64 = Int64Ty(a) * Int64Ty(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void MulSatVectorLogic(T result, T a, T b) {
  ForEach(MulSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   MulSat, Binary)
TestAll(UnsignedVectorInst, MulSat, Binary)

template<class T> static void MulHiLogic(T result, T a, T b) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty a64 = Int64Ty(a);
  Int64Ty b64 = Int64Ty(b);
  EXPECT_EQ(T((a64 * b64) >> Int<T>::Bits), result);
}
template<class T> static void MulHiVectorLogic(T result, T a, T b) {
  ForEach(MulHiLogic, result, a, b);
}
declareBinary(MulHi, s32)
declareBinary(MulHi, u32)
MakeTest(MulHi_s32, MulHiLogic)
MakeTest(MulHi_u32, MulHiLogic)
TestAll(SignedVectorInst,   MulHi, Binary)
TestAll(UnsignedVectorInst, MulHi, Binary)

template<class T> static void SubSatLogic(T result, T a, T b) {
  s64 result64 = s64(a) - s64(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void SubSatVectorLogic(T result, T a, T b) {
  ForEach(SubSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   SubSat, Binary)
TestAll(UnsignedVectorInst, SubSat, Binary)

template<class T> static void MaxLogic(T result, T a, T b) {
  if(!isNegZero(a) && !isNegZero(b))
    EXPECT_PRED2(Not<T>, isNegZero<T>, result);

  if(isNan(a) && isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNan(a) || isNan(b)) {
    EXPECT_PRED2(Not<T>, isNan<T>, result);
    EXPECT_TRUE(a == result || b == result);
  } else {
    EXPECT_LE(a, result);
    EXPECT_LE(b, result);
    EXPECT_TRUE(a == result || b == result);
  }
}
template<class T> static void MaxVectorLogic(T result, T a, T b) {
  ForEach(MaxLogic, result, a, b);
}
TestAll(SignedInst,         Max, Binary)
TestAll(UnsignedInst,       Max, Binary)
TestAll(FloatInst,          Max, Binary)
TestAll(SignedVectorInst,   Max, Binary)
TestAll(UnsignedVectorInst, Max, Binary)
TestAll(FloatVectorInst,    Max, Binary)

template<class T> static void MinLogic(T result, T a, T b) {
  if(!isPosZero(a) && !isPosZero(b))
    EXPECT_PRED2(Not<T>, isPosZero<T>, result);

  if(isNan(a) && isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNan(a) || isNan(b)) {
    EXPECT_PRED2(Not<T>, isNan<T>, result);
    EXPECT_TRUE(a == result || b == result);
  } else {
    EXPECT_GE(a, result);
    EXPECT_GE(b, result);
    EXPECT_TRUE(a == result || b == result);
  }
}
template<class T> static void MinVectorLogic(T result, T a, T b) {
  ForEach(MinLogic, result, a, b);
}
TestAll(SignedInst,         Min, Binary)
TestAll(UnsignedInst,       Min, Binary)
TestAll(FloatInst,          Min, Binary)
TestAll(SignedVectorInst,   Min, Binary)
TestAll(UnsignedVectorInst, Min, Binary)
TestAll(FloatVectorInst,    Min, Binary)

template<class T> static void RemLogic(T result, T a, T b) {
  if(hsa::brig::isDivisionError(a, b)) {
    EXPECT_EQ(result, 0);
  } else if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(a == 0 && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a) && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a % b), result);
  }
}
TestAll(SignedInst,         Rem, Binary)
TestAll(UnsignedInst,       Rem, Binary)

template<class T> static void CarryLogic(T result, T a, T b) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  typedef typename Int<T>::Unsigned Unsigned;
  Int64Ty a64 = Int64Ty(a);
  Int64Ty b64 = Int64Ty(b);
  if ((Unsigned) a64 + (Unsigned) b64 < (Unsigned) a64) {
    EXPECT_EQ(T(1), result);
  } else {
    EXPECT_EQ(T(0), result);
  }
}
TestAll(SignedInst,         Carry, Binary)
TestAll(UnsignedInst,       Carry, Binary)

template<class T> static void BorrowLogic(T result, T a, T b) {
  if(Int<T>::isSigned) {
    bool isNegA = Int<T>::isNeg(a);
    bool isNegB = Int<T>::isNeg(b);
    bool isNegResult = Int<T>::isNeg(a - b);
    if(isNegA == isNegB) EXPECT_EQ(isNegResult, result);
    else EXPECT_EQ(!isNegA && isNegB, result);
  } else {
    EXPECT_EQ(a < a - b, result);
  }
}
TestAll(SignedInst,         Borrow, Binary)
TestAll(UnsignedInst,       Borrow, Binary)

template<class T> static void MadLogic(T result, T a, T b, T c) {
  if(isNan(a) || isNan(b) || isNan(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(a == 0 && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(a * b) && isPosInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(a * b) && isNegInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(a * b + c, result);
  }
}
TestAll(SignedInst,         Mad, Ternary)
TestAll(UnsignedInst,       Mad, Ternary)
TestAll(FloatInst,          Mad, Ternary)

template<class T> static void Mul24Logic(T result, T a, T b) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty a64 = Int24Ty(a);
  Int64Ty b64 = Int24Ty(b);
  EXPECT_EQ(T(a64 * b64 & 0xFFFFFFFF), result);
}
TestAll(SignedInst,         Mul24, Binary)
TestAll(UnsignedInst,       Mul24, Binary)

template<class T> static void Mul24HiLogic(T result, T a, T b) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty a64 = Int24Ty(a);
  Int64Ty b64 = Int24Ty(b);
  EXPECT_EQ(T(Int12Ty(a64 * b64 >> 32)), result);
}
TestAll(SignedInst,         Mul24Hi, Binary)
TestAll(UnsignedInst,       Mul24Hi, Binary)

template<class T> static void Mad24Logic(T result, T a, T b, T c) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty a64 = Int24Ty(a);
  Int64Ty b64 = Int24Ty(b);
  Int64Ty c64 = Int64Ty(c);
  Int64Ty ab64 = Int48Ty(a64 * b64);
  EXPECT_EQ(T((ab64 + c64) & 0xFFFFFFFF), result);
}
TestAll(SignedInst,         Mad24, Ternary)
TestAll(UnsignedInst,       Mad24, Ternary)

template<class T> static void Mad24HiLogic(T result, T a, T b, T c) {
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty a64 = Int24Ty(a);
  Int64Ty b64 = Int24Ty(b);
  Int64Ty c64 = Int64Ty(c);
  Int64Ty ab64 = Int48Ty(a64 * b64);
  EXPECT_EQ(T(Int12Ty((ab64 + c64) >> 32)), result);
}
TestAll(SignedInst,         Mad24Hi, Ternary)
TestAll(UnsignedInst,       Mad24Hi, Ternary)

template<class T> static void ShlLogic(T result, T a, unsigned b) {
  typedef typename Int<T>::Unsigned Unsigned;
  unsigned shift = b & Int<T>::ShiftMask;
  T mask = !shift ? ~T(0) : (T(1) << (Int<T>::Bits - shift)) - 1;
  EXPECT_EQ(T(a & mask), T(Unsigned(result) >> shift));
}
template<class T> static void ShlVectorLogic(T result, T a, unsigned b) {
  ForEach(ShlLogic, result, a, b);
}
TestAll(ShiftInst, Shl, Binary)

template<class T> static void ShrLogic(T result, T a, unsigned b) {
  typedef typename Int<T>::Unsigned Unsigned;
  unsigned shift = b & Int<T>::ShiftMask;
  T mask = !shift ? ~T(0) : ~((T(1) << shift) - 1);
  EXPECT_EQ(T(a & mask), T(Unsigned(result) << shift));
}
template<class T> static void ShrVectorLogic(T result, T a, unsigned b) {
  ForEach(ShrLogic, result, a, b);
}
TestAll(ShiftInst, Shr, Binary)

template<class T> static void UnpackLoLogic(T result, T a, T b) {
  for(unsigned i = 0; i < Vec<T>::Len / 2; ++i) {
    if(!isNan(((Vec<T>) a)[i]) &&
       !isNan(((Vec<T>) result)[i * 2]))
    {
      EXPECT_EQ(((Vec<T>) a)[i],
                      ((Vec<T>) result)[i * 2]);
    }
    if(!isNan(((Vec<T>) b)[i]) &&
       !isNan(((Vec<T>) result)[i * 2 + 1]))
    {
      EXPECT_EQ(((Vec<T>) b)[i],
                      ((Vec<T>) result)[i * 2 + 1]);
    }
  }
}
TestAll(UnpackInst, UnpackLo, Binary)

template<class T> static void UnpackHiLogic(T result, T a, T b) {
  for(unsigned i = 0; i < Vec<T>::Len / 2; ++i) {
    if(!isNan(((Vec<T>) a)[i + Vec<T>::Len /2]) &&
         !isNan(((Vec<T>) result)[i * 2]))
    {
      EXPECT_EQ(((Vec<T>) a)[i + Vec<T>::Len / 2],
                      ((Vec<T>) result)[i * 2]);
    }
    if(!isNan(((Vec<T>) b)[i + Vec<T>::Len / 2]) &&
       !isNan(((Vec<T>) result)[i * 2 + 1]))
    {
      EXPECT_EQ(((Vec<T>) b)[i + Vec<T>::Len / 2],
                      ((Vec<T>) result)[i * 2 + 1]);
    }
  }
}
TestAll(UnpackInst, UnpackHi, Binary)

template<class T> static void AndLogic(T result, T a, T b) {
  EXPECT_EQ(~(~a | ~b), result);
}
TestAll(BitInst, And, Binary)

template<class T> static void OrLogic(T result, T a, T b) {
  EXPECT_EQ(~(~a & ~b), result);
}
TestAll(BitInst, Or, Binary)

template<class T> static void XorLogic(T result, T a, T b) {
  EXPECT_EQ((a | b) & ~(a & b), result);
}
TestAll(BitInst, Xor, Binary)

template<class T> static void NotLogic(T result, T a) {
  EXPECT_EQ(T(~0), result | a);
  EXPECT_EQ(T(~0), result ^ a);
  EXPECT_EQ(0, result & a);
}
TestAll(BitInst, Not, Unary)

template<class T> static void PopcountLogic(T result, T a) {
  int d = 0;
  T bits = a;
  while(bits != 0) {
    if(bits & 1) ++d;
    bits >>= 1;
  }
  EXPECT_EQ(d, result);
}
declareUnary(Popcount, b32)
declareUnary(Popcount, b64)
MakeTest(Popcount_b32, PopcountLogic)
MakeTest(Popcount_b64, PopcountLogic)

// Bit reverse implementation loosely adapted from Sean Eron Anderson's article
// at: http://graphics.stanford.edu/~seander/bithacks.html
template<class T> static void BitRevLogic(T result, T a) {
  union Helper {
    u8 a[sizeof(T)];
    T  t;
  };

  Helper orig, rev;
  orig.t = a;
  for(unsigned i = 0; i < sizeof(T); ++i) {
    rev.a[i] = orig.a[sizeof(T) - i - 1];
  }

  for(unsigned i = 0; i < sizeof(T); ++i) {
    // swap odd and even bits
    rev.a[i] = ((rev.a[i] >> 1) & 0x55)  | ((rev.a[i] & 0x55) << 1);
    // swap consecutive pairs
    rev.a[i] = ((rev.a[i] >> 2) & 0x33)  | ((rev.a[i] & 0x33) << 2);
    // swap nibbles
    rev.a[i] = ((rev.a[i] >> 4) & 0x0F)  | ((rev.a[i] & 0x0F) << 4);
  }

  EXPECT_EQ(rev.t, result);
}
TestAll(SignedInst,   BitRev, Unary)
TestAll(UnsignedInst, BitRev, Unary)

template<class T> static void ExtractLogic(T result, T a, T b, T c) {

  unsigned offset = Int<T>::ShiftMask & b;
  unsigned width  = Int<T>::ShiftMask & c;

  if(width && offset + width <= Int<T>::Bits) {
    T mask = ~(~T(0) << width);
    T noSign = (a >> (Int<T>::Bits - width - offset)) & mask;
    T signEx = noSign << (Int<T>::Bits - width) >> (Int<T>::Bits - width);
    EXPECT_EQ(signEx, result);
  }
}
TestAll(SignedInst,   Extract, Ternary)
TestAll(UnsignedInst, Extract, Ternary)

template<class T> static void BitselectLogic(T result, T a, T b, T c) {
  EXPECT_EQ(b &  a, result &  a);
  EXPECT_EQ(c & ~a, result & ~a);
}
TestAll(SignedInst,   Bitselect, Ternary)
TestAll(UnsignedInst, Bitselect, Ternary)

template<class T> static void FirstbitLogic(T result, T a) {

  if(Int<T>::isNeg(a)) a = ~a;

  if(a == 0) {
    EXPECT_EQ(~T(0), result);
    return;
  }

  typedef typename Int<T>::Unsigned Unsigned;
  Unsigned mask = Int<Unsigned>::HighBit;
  while(!(a & mask)) mask >>= 1;
  EXPECT_EQ(mask, T(1) << (Int<T>::Bits - result - 1));
}
TestAll(SignedInst,   Firstbit, Unary)
TestAll(UnsignedInst, Firstbit, Unary)

template<class T> static void LastbitLogic(T result, T a) {

  if(!a) {
    EXPECT_EQ(~T(0), result);
    return;
  }

  typedef typename Int<T>::Unsigned Unsigned;
  T mask = 1;
  while(!(a & mask)) mask <<= 1;
  EXPECT_EQ(mask, T(1) << result);
}
TestAll(SignedInst,   Lastbit, Unary)
TestAll(UnsignedInst, Lastbit, Unary)

static void Mov_b64_b32_Logic(b64 result, b32 a, b32 b) {
  EXPECT_EQ(b, b32(result));
  EXPECT_EQ(a, result >> 32);
}
extern "C" b64 Mov_b64_b32(b32, b32);
MakeTest(Mov_b64_b32, Mov_b64_b32_Logic)

static void Mov_b64_b64_Logic(b64 result, b64 a) {
  EXPECT_EQ(a, result);
}
extern "C" b64 Mov_b64_b64(b64);
MakeTest(Mov_b64_b64, Mov_b64_b64_Logic)

// Assumes little-endian
static void Mov_b128_b32_Logic(b128 result, b32 a, b32 b, b32 c, b32 d) {
  EXPECT_EQ(d, ((Vec<b128>) result)[0]);
  EXPECT_EQ(c, ((Vec<b128>) result)[1]);
  EXPECT_EQ(b, ((Vec<b128>) result)[2]);
  EXPECT_EQ(a, ((Vec<b128>) result)[3]);
}
extern "C" b128 Mov_b128_b32(b32, b32, b32, b32);
MakeTest(Mov_b128_b32, Mov_b128_b32_Logic)

static void Movs_lo_b32_Logic(b32 result, b64 a) {
  EXPECT_EQ(b32(a), result);
}
extern "C" b32 Movs_lo_b32(b64);
MakeTest(Movs_lo_b32, Movs_lo_b32_Logic)

static void Movs_hi_b32_Logic(b32 result, b64 a) {
  EXPECT_EQ(a >> 32, result);
}
extern "C" b32 Movs_hi_b32(b64);
MakeTest(Movs_hi_b32, Movs_hi_b32_Logic)

static void Movd_lo_b64_Logic(b64 result, b64 a, b32 b) {
  EXPECT_EQ(a >> 32, result >> 32);
  EXPECT_EQ(b32(b), b32(result));
}
extern "C" b64 Movd_lo_b64(b64, b32);
MakeTest(Movd_lo_b64, Movd_lo_b64_Logic)

static void Movd_hi_b64_Logic(b64 result, b64 a, b32 b) {
  EXPECT_EQ(a >> 32, b32(result));
  EXPECT_EQ(b32(b), result >> 32);
}
extern "C" b64 Movd_hi_b64(b64, b32);
MakeTest(Movd_hi_b64, Movd_hi_b64_Logic)

template<class T> static void ShuffleLogic(T result, T a, T b, b32 c) {

  typedef typename Vec<T>::Base Base;

  unsigned len   = Vec<T>::Len;
  unsigned mask  = len - 1;
  unsigned shift = Vec<T>::LogLen;
  b32 shuffle = c;

  for(unsigned i = 0; i < len / 2; ++i) {
    unsigned offset = shuffle & mask;
    if(isNan(((Vec<T>) a)[offset])) {
      EXPECT_PRED1(isNan<Base>, ((Vec<T>) result)[i]);
    } else {
      EXPECT_EQ(((Vec<T>) a)[offset], ((Vec<T>) result)[i]);
    }
    shuffle >>= shift;
  }

  for(unsigned i = len / 2; i < len; ++i) {
    unsigned offset = shuffle & mask;
    if(isNan(((Vec<T>) b)[offset])) {
      EXPECT_PRED1(isNan<Base>, ((Vec<T>) result)[i]);
    } else {
      EXPECT_EQ(((Vec<T>) b)[offset], ((Vec<T>) result)[i]);
    }
    shuffle >>= shift;
  }
}
TestAll(ShuffleVectorInst, Shuffle, Ternary)

template<class T> static void CmovLogic(T result, T a, T b, T c) {
  if(a & 1) {
    EXPECT_EQ(b, result);
  } else {
    EXPECT_EQ(c, result);
  }
}
TestAll(BitInst, Cmov, Ternary)

template<class T> static void FractLogic(T result, T a) {

  if(isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }

  EXPECT_GT(1, result);
  EXPECT_LE(0, result);
  if(a >= 0 || a <= -FLT_EPSILON)
    EXPECT_DOUBLE_EQ(a, std::floor(a) + result);
  else
    EXPECT_FLOAT_EQ(1.0f, result);
}
TestAll(FloatInst, Fract, Unary)

template<class T> static void SqrtLogic(T result, T a) {
  if(isNan(a)) {
    EXPECT_PRED1(isNan<T>, a);
  } else if(a >= 0.0) {
    EXPECT_FLOAT_EQ(a, result * result);
  }
}
TestAll(FloatInst, Sqrt, Unary)

template<class T> static void FmaLogic(T result, T a, T b, T c) {
  if(isNan(a) || isNan(b) || isNan(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(a == 0 && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(!isInf(a) && !isInf(b) && isInf(c)) {
    EXPECT_EQ(c, result);
  } else if(isNegInf(a * b) && isPosInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(a * b) && isNegInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(a * b) && !isInf(c)) {
    EXPECT_FLOAT_EQ(a * b, result - c);
  } else if(result >= FLT_EPSILON || result <= -FLT_EPSILON) {
    EXPECT_FLOAT_EQ((double ) a * b + c, result);
  }
}
TestAll(FloatInst, Fma, Ternary)

template<class T> static void CopySignLogic(T result, T a, T b);
template<> void CopySignLogic(f64 result, f64 a, f64 b) {
  union Conv { f64 d; b64 b; };
  Conv aConv = { a };
  Conv bConv = { b };
  Conv resultConv = { result };
  b64 mask = (1UL << 63) - 1;
  EXPECT_EQ(resultConv.b &  mask, aConv.b &  mask);
  EXPECT_EQ(resultConv.b & ~mask, bConv.b & ~mask);
}
template<> void CopySignLogic(f32 result, f32 a, f32 b) {
  union Conv { f32 d; b32 b; };
  Conv aConv = { a };
  Conv bConv = { b };
  Conv resultConv = { result };
  b32 mask = (1UL << 31) - 1;
  EXPECT_EQ(resultConv.b &  mask, aConv.b &  mask);
  EXPECT_EQ(resultConv.b & ~mask, bConv.b & ~mask);
}
TestAll(FloatInst, CopySign, Binary)

template<class T> static void ClassLogic(b1 result, T a, b32 b) {
  int fpclass = std::fpclassify(a);
  if(b & hsa::brig::SNan && isSNan(a))
    EXPECT_TRUE(result);
  if(b & hsa::brig::QNan && isQNan(a))
    EXPECT_TRUE(result);
  if(b & hsa::brig::NegInf && isNegInf(a))
    EXPECT_TRUE(result);
  if(b & hsa::brig::NegNorm && fpclass == FP_NORMAL && a < 0)
    EXPECT_TRUE(result);
  if(b & hsa::brig::NegSubnorm && fpclass == FP_SUBNORMAL && a < 0)
    EXPECT_TRUE(result);
  if(b & hsa::brig::NegZero && isNegZero(a))
    EXPECT_TRUE(result);
  if(b & hsa::brig::PosZero && isPosZero(a))
    EXPECT_TRUE(result);
  if(b & hsa::brig::PosSubnorm && fpclass == FP_SUBNORMAL && a > 0)
    EXPECT_TRUE(result);
  if(b & hsa::brig::PosNorm && fpclass == FP_NORMAL && a > 0)
    EXPECT_TRUE(result);
  if(b & hsa::brig::PosInf && isPosInf(a))
    EXPECT_TRUE(result);
}
extern "C" b1 Class_f32(f32, unsigned);
extern "C" b1 Class_f64(f64, unsigned);
MakeTest(Class_f32, ClassLogic)
MakeTest(Class_f64, ClassLogic)

template<class T> static void FcosLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(fpclass == FP_NORMAL && (-512 * M_PI > a || a > 512 * M_PI)) {
    EXPECT_FLOAT_EQ(cos(1.0), result);
  } else if(fpclass == FP_SUBNORMAL) {
    EXPECT_EQ(1.0, result);
  } else if(isNegZero(a)) {
    EXPECT_EQ(1.0, result);
  } else if(isPosZero(a)) {
    EXPECT_EQ(1.0, result);
  } else {
    EXPECT_FLOAT_EQ(cos(a), result);
  }
}
TestAll(FloatInst, Fcos, Unary)

template<class T> static void FsinLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(fpclass == FP_NORMAL && (-512 * M_PI > a || a > 512 * M_PI)) {
    EXPECT_FLOAT_EQ(sin(1.0), result);
  } else if(fpclass == FP_SUBNORMAL && a > 0.0) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else if(fpclass == FP_SUBNORMAL && a < 0.0) {
    EXPECT_EQ(0.0, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegZero<T>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else {
    EXPECT_FLOAT_EQ(sin(a), result);
  }
}
TestAll(FloatInst, Fsin, Unary)

template<class T> static void Flog2Logic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isNegInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(fpclass == FP_SUBNORMAL) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else {
    EXPECT_FLOAT_EQ(log2(a), result);
  }
}
TestAll(FloatInst, Flog2, Unary)

template<class T> static void Fexp2Logic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(a)) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else if(isPosInf(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else if(fpclass == FP_SUBNORMAL) {
    EXPECT_EQ(1.0, result);
  } else if(isNegZero(a)) {
    EXPECT_EQ(1.0, result);
  } else if(isPosZero(a)) {
    EXPECT_EQ(1.0, result);
  } else {
    EXPECT_FLOAT_EQ(exp2(a), result);
  }
}
TestAll(FloatInst, Fexp2, Unary)

template<class T> static void FrsqrtLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isNegInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(fpclass == FP_SUBNORMAL && a < 0.0) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(fpclass == FP_SUBNORMAL && a > 0.0) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(0.0, result);
  } else {
    EXPECT_FLOAT_EQ(a, (1.0 / result) * (1.0 / result));
  }
}
TestAll(FloatInst, Frsqrt, Unary)

template<class T> static void FrcpLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(a)) {
    EXPECT_PRED1(isNegZero<T>, result);
  } else if(fpclass == FP_SUBNORMAL && a < 0) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(fpclass == FP_SUBNORMAL && a > 0) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if(isPosInf(a)) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else {
    EXPECT_FLOAT_EQ(a, 1.0 / result);
  }
}
TestAll(FloatInst, Frcp, Unary)

TestCmp(eq, a == b)
TestCmp(ne, a != b)
TestCmp(lt, a <  b)
TestCmp(le, a <= b)
TestCmp(gt, a >  b)
TestCmp(ge, a >= b)

Cmp(declare, eq, b1)
Cmp(declare, ne, b1)
MakeCmpTest(eq, b1)
MakeCmpTest(ne, b1)

template<class R, class T> void Cmp_numLogic(R result, T a, T b) {
  EXPECT_EQ(cmpResult<R>(!isNan(a) && !isNan(b)), result);
}
template<class R, class T> void Cmp_nanLogic(R result, T a, T b) {
  EXPECT_EQ(cmpResult<R>(isNan(a) || isNan(b)), result);
}
// Cmp(declare, num, f16)
Cmp(declare, num,  f32)
Cmp(declare, num,  f64)
// Cmp(declare, nan, f16)
Cmp(declare, nan,  f32)
Cmp(declare, nan,  f64)
// MakeCmpTest(num, f16)
MakeCmpTest(num, f32)
MakeCmpTest(num, f64)
// MakeCmpTest(nan, f16)
MakeCmpTest(nan, f32)
MakeCmpTest(nan, f64)
