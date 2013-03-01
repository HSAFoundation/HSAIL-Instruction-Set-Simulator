#include "brig_runtime_test_internal.h"

#include "gtest/gtest.h"

using hsa::brig::cmpResult;
using hsa::brig::ForEach;
using hsa::brig::getMax;
using hsa::brig::getMin;
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
  for(unsigned i = 0; i < T::Len / 2; ++i) {
    if(!isNan(a[i]) &&
       !isNan(result[i * 2]))
    {
      EXPECT_EQ(a[i],
                      result[i * 2]);
    }
    if(!isNan(b[i]) &&
       !isNan(result[i * 2 + 1]))
    {
      EXPECT_EQ(b[i],
                      result[i * 2 + 1]);
    }
  }
}
TestAll(UnpackInst, UnpackLo, Binary)

template<class T> static void UnpackHiLogic(T result, T a, T b) {
  for(unsigned i = 0; i < T::Len / 2; ++i) {
    if(!isNan(a[i + T::Len /2]) &&
         !isNan(result[i * 2]))
    {
      EXPECT_EQ(a[i + T::Len / 2],
                      result[i * 2]);
    }
    if(!isNan(b[i + T::Len / 2]) &&
       !isNan(result[i * 2 + 1]))
    {
      EXPECT_EQ(b[i + T::Len / 2],
                      result[i * 2 + 1]);
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

template<class T> static void PopCountLogic(T result, T a) {
  int d = 0;
  T bits = a;
  while(bits != 0) {
    if(bits & 1) ++d;
    bits >>= 1;
  }
  EXPECT_EQ(d, result);
}
declareUnary(PopCount, b32)
declareUnary(PopCount, b64)
MakeTest(PopCount_b32, PopCountLogic)
MakeTest(PopCount_b64, PopCountLogic)

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
declareUnary(BitRev, b32)
declareUnary(BitRev, b64)
MakeTest(BitRev_b32, BitRevLogic)
MakeTest(BitRev_b64, BitRevLogic)

template<class T> static void ExtractLogic(T result, T a, T b, T c) {

  unsigned offset = Int<T>::ShiftMask & b;
  unsigned width  = Int<T>::ShiftMask & c;

  if(width && offset + width <= Int<T>::Bits) {
    T mask = ~(~T(0) << width);
    T noSign = (a >> offset) & mask;
    T signEx = noSign << (Int<T>::Bits - width) >> (Int<T>::Bits - width);
    EXPECT_EQ(signEx, result);
  }
}
declareTernary(Extract, b32)
declareTernary(Extract, b64)
MakeTest(Extract_b32, ExtractLogic)
MakeTest(Extract_b64, ExtractLogic)

template<class T> static void InsertLogic(T result, T a, T b, T c, T d) {

  unsigned width  = c & T(Int<T>::Bits - 1);
  unsigned offset = d & T(Int<T>::Bits - 1);
  T resultNe = a;
  for (unsigned i = 0; i < width && offset + i < Int<T>::Bits; ++i) {
    resultNe &= ~(T(1) << (offset + i));
    resultNe |= (b << offset) & (T(1) << (offset + i));
  }
  resultNe |= (b & (~T(0) << width)) << offset;
  EXPECT_EQ(resultNe, result);
}
declareQuaternary(Insert, b32)
declareQuaternary(Insert, b64)
MakeTest(Insert_b32, InsertLogic)
MakeTest(Insert_b64, InsertLogic)

template<class T> static void BitSelectLogic(T result, T a, T b, T c) {
  EXPECT_EQ(b &  a, result &  a);
  EXPECT_EQ(c & ~a, result & ~a);
}
declareTernary(BitSelect, b32)
declareTernary(BitSelect, b64)
MakeTest(BitSelect_b32, BitSelectLogic)
MakeTest(BitSelect_b64, BitSelectLogic)

template<class T> static void FirstBitLogic(T result, T a) {

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
declareUnary(FirstBit, b32)
declareUnary(FirstBit, b64)
MakeTest(FirstBit_b32, FirstBitLogic)
MakeTest(FirstBit_b64, FirstBitLogic)

template<class T> static void LastBitLogic(T result, T a) {

  if(!a) {
    EXPECT_EQ(~T(0), result);
    return;
  }

  typedef typename Int<T>::Unsigned Unsigned;
  T mask = 1;
  while(!(a & mask)) mask <<= 1;
  EXPECT_EQ(mask, T(1) << result);
}
declareUnary(LastBit, b32)
declareUnary(LastBit, b64)
MakeTest(LastBit_b32, LastBitLogic)
MakeTest(LastBit_b64, LastBitLogic)

static void MovsLo_b32_Logic(b32 result, b64 a) {
  EXPECT_EQ(b32(a), result);
}
extern "C" b32 MovsLo_b32(b64);
MakeTest(MovsLo_b32, MovsLo_b32_Logic)

static void MovsHi_b32_Logic(b32 result, b64 a) {
  EXPECT_EQ(a >> 32, result);
}
extern "C" b32 MovsHi_b32(b64);
MakeTest(MovsHi_b32, MovsHi_b32_Logic)

static void MovdLo_b64_Logic(b64 result, b64 a, b32 b) {
  EXPECT_EQ(a >> 32, result >> 32);
  EXPECT_EQ(b32(b), b32(result));
}
extern "C" b64 MovdLo_b64(b64, b32);
MakeTest(MovdLo_b64, MovdLo_b64_Logic)

static void MovdHi_b64_Logic(b64 result, b64 a, b32 b) {
  EXPECT_EQ(a >> 32, b32(result));
  EXPECT_EQ(b32(b), result >> 32);
}
extern "C" b64 MovdHi_b64(b64, b32);
MakeTest(MovdHi_b64, MovdHi_b64_Logic)

template<class T> static void ShuffleLogic(T result, T a, T b, b32 c) {

  typedef typename T::Base Base;

  unsigned len   = T::Len;
  unsigned mask  = len - 1;
  unsigned shift = T::LogLen;
  b32 shuffle = c;

  for(unsigned i = 0; i < len / 2; ++i) {
    unsigned offset = shuffle & mask;
    if(isNan(a[offset])) {
      EXPECT_PRED1(isNan<Base>, result[i]);
    } else {
      EXPECT_EQ(a[offset], result[i]);
    }
    shuffle >>= shift;
  }

  for(unsigned i = len / 2; i < len; ++i) {
    unsigned offset = shuffle & mask;
    if(isNan(b[offset])) {
      EXPECT_PRED1(isNan<Base>, result[i]);
    } else {
      EXPECT_EQ(b[offset], result[i]);
    }
    shuffle >>= shift;
  }
}
TestAll(ShuffleVectorInst, Shuffle, Ternary)

template<class T> static void CmovLogic(T result, T a, T b, T c) {
  if(a == 0) {
    if(isNan(c)) {
      EXPECT_PRED1(isNan<T>, result);
    } else {
      EXPECT_EQ(c, result);
    }
  } else {
    if(isNan(b)) {
      EXPECT_PRED1(isNan<T>, result);
    } else {
      EXPECT_EQ(b, result);
    }
  }
}
TestAll(BitInst, Cmov, Ternary)

template<class T> static void Cmov_PPVectorLogic(T result, T a, T b, T c) {
  ForEach(CmovLogic, result, a, b, c);
}
TestAll(SignedVectorInst, Cmov_PP, Ternary)
TestAll(UnsignedVectorInst, Cmov_PP, Ternary)
TestAll(FloatVectorInst, Cmov_PP, Ternary)

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
  b64 mask = (1ULL << 63) - 1;
  EXPECT_EQ(resultConv.b &  mask, aConv.b &  mask);
  EXPECT_EQ(resultConv.b & ~mask, bConv.b & ~mask);
}
template<> void CopySignLogic(f32 result, f32 a, f32 b) {
  union Conv { f32 d; b32 b; };
  Conv aConv = { a };
  Conv bConv = { b };
  Conv resultConv = { result };
  b32 mask = (1U << 31) - 1;
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

static void Fcos_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
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
extern "C" f32 Fcos_f32(f32);
MakeTest(Fcos_f32, Fcos_f32_Logic)

static void Fsin_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if(fpclass == FP_NORMAL && (-512 * M_PI > a || a > 512 * M_PI)) {
    EXPECT_FLOAT_EQ(sin(1.0), result);
  } else if(fpclass == FP_SUBNORMAL && a > 0.0) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else if(fpclass == FP_SUBNORMAL && a < 0.0) {
    EXPECT_PRED1(isNegZero<f32>, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegZero<f32>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else {
    EXPECT_FLOAT_EQ(sin(a), result);
  }
}
extern "C" f32 Fsin_f32(f32);
MakeTest(Fsin_f32, Fsin_f32_Logic)

static void Flog2_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a) || isNegInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if(isPosInf(a)) {
    EXPECT_PRED1(isPosInf<f32>, result);
  } else if(fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if(fpclass == FP_SUBNORMAL) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if(isNegZero(a)) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if(isPosZero(a)) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else {
    EXPECT_FLOAT_EQ(log2(a), result);
  }
}
extern "C" f32 Flog2_f32(f32);
MakeTest(Flog2_f32, Flog2_f32_Logic)

static void Fexp2_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if(isNan(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if(isNegInf(a)) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else if(isPosInf(a)) {
    EXPECT_PRED1(isPosInf<f32>, result);
  } else if(fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isPosZero<f32>, result);
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
extern "C" f32 Fexp2_f32(f32);
MakeTest(Fexp2_f32, Fexp2_f32_Logic)

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

static void F2u4_u32_Logic(u32 result, f32 a, f32 b, f32 c, f32 d) {
  if(isNan(a) || a < 0.0f || a >= 256.0f) return;
  if(isNan(b) || b < 0.0f || b >= 256.0f) return;
  if(isNan(c) || c < 0.0f || c >= 256.0f) return;
  if(isNan(d) || d < 0.0f || d >= 256.0f) return;

  EXPECT_EQ((result      ) & 0xFF, lrint(d) & 0xFF);
  EXPECT_EQ((result >>  8) & 0xFF, lrint(c) & 0xFF);
  EXPECT_EQ((result >> 16) & 0xFF, lrint(b) & 0xFF);
  EXPECT_EQ((result >> 24) & 0xFF, lrint(a) & 0xFF);
}
extern "C" u32 F2u4_u32(f32 a, f32 b, f32 c, f32 d);
MakeTest(F2u4_u32, F2u4_u32_Logic)

static void Unpack3Logic(f32 result, b32 a) {
  EXPECT_EQ(result, (a >> 24) & 0xFF);
}
extern "C" f32 Unpack3_f32(b32);
MakeTest(Unpack3_f32, Unpack3Logic)

static void Unpack2Logic(f32 result, b32 a) {
  EXPECT_EQ(result, (a >> 16) & 0xFF);
}
extern "C" f32 Unpack2_f32(b32);
MakeTest(Unpack2_f32, Unpack2Logic)

static void Unpack1Logic(f32 result, b32 a) {
  EXPECT_EQ(result, (a >> 8) & 0xFF);
}
extern "C" f32 Unpack1_f32(b32);
MakeTest(Unpack1_f32, Unpack1Logic)

static void Unpack0Logic(f32 result, b32 a) {
  EXPECT_EQ(result, (a & 0xFF));
}
extern "C" f32 Unpack0_f32(b32);
MakeTest(Unpack0_f32, Unpack0Logic)

static void BitAlign_b32_Logic(b32 result, b32 a, b32 b, b32 c ) {
  if(c == 0 || c == 8 || c == 16 || c == 24 || c == 32) {
    unsigned tag = (32 - c) / 8;
    for(unsigned i = 0; i < 4; ++i) {
      if(i + tag > 3)
        EXPECT_EQ((a >> (((i + tag) % 4) * 8)) & 0xFF,
                  (result >> (i * 8)) & 0xFF);
      else
        EXPECT_EQ((b >> ((i + tag) * 8)) & 0xFF,
                  (result >> (i * 8)) & 0xFF);
    }
  }
}
extern "C" b32 BitAlign_b32(b32, b32, b32);
MakeTest(BitAlign_b32, BitAlign_b32_Logic)

static void ByteAlign_b32_Logic(b32 result, b32 a, b32 b, b32 c ) {
  if(c <= 4) {
    unsigned tag = (4 - c);
    for(unsigned i = 0; i < 4; ++i) {
      if(i + tag > 3)
        EXPECT_EQ(((a) >> (((i + tag) % 4) * 8)) & 0xFF,
                  (result >> (i * 8)) & 0xFF);
      else
        EXPECT_EQ(((b) >> (((i + tag)) * 8)) & 0xFF,
                  (result >> (i * 8)) & 0xFF);
    }
  }
}
extern "C" b32 ByteAlign_b32(b32, b32, b32);
MakeTest(ByteAlign_b32, ByteAlign_b32_Logic)

static void Lerp_b32_Logic(b32 result, b32 a, b32 b, b32 c) {
  for(unsigned i = 0; i < 4; ++i) {
    EXPECT_EQ((result >> i * 8) & 0xFF,
                ((((a >> i * 8) & 0xFF)
                + ((b >> i * 8) & 0xFF)
                + ((c >> i * 8) & 0x1)) >> 1) & 0xFF);
  }
}
extern "C" b32 Lerp_b32(b32, b32, b32);
MakeTest(Lerp_b32, Lerp_b32_Logic)

static void Sad_b32_Logic(b32 result, b32 a, b32 b, b32 c) {
  EXPECT_EQ(abs(a - b) + c, result);
}
extern "C" b32 Sad_b32(b32, b32, b32);
MakeTest(Sad_b32, Sad_b32_Logic)

static void Sad2_b32_Logic(b32 result, b32 a, b32 b, b32 c) {
  EXPECT_EQ(abs((a & 0xFFFF) - (b & 0xFFFF)) +
            abs(((a >> 16) & 0xFFFF) - ((b >> 16) & 0xFFFF)) +
            c,
            result);
}
extern "C" b32 Sad2_b32(b32, b32, b32);
MakeTest(Sad2_b32, Sad2_b32_Logic)

static void Sad4_b32_Logic(b32 result, b32 a, b32 b, b32 c) {
  EXPECT_EQ(abs((a & 0xFF) - (b & 0xFF)) +
            abs(((a >> 8) & 0xFF)  - ((b >> 8)  & 0xFF)) +
            abs(((a >> 16) & 0xFF) - ((b >> 16) & 0xFF)) +
            abs(((a >> 24) & 0xFF) - ((b >> 24) & 0xFF)) +
            c,
            result);
}
extern "C" b32 Sad4_b32(b32, b32, b32);
MakeTest(Sad4_b32, Sad4_b32_Logic)

static void Sad4Hi_b32_Logic(b32 result, b32 a, b32 b, b32 c) {
  EXPECT_EQ(((abs((a & 0xFF) - (b & 0xFF)) +
              abs(((a >> 8) & 0xFF)  - ((b >> 8)  & 0xFF)) +
              abs(((a >> 16) & 0xFF) - ((b >> 16) & 0xFF)) +
              abs(((a >> 24) & 0xFF) - ((b >> 24) & 0xFF))) << 16) +
            c,
            result);
}
extern "C" b32 Sad4Hi_b32(b32, b32, b32);
MakeTest(Sad4Hi_b32, Sad4Hi_b32_Logic)

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

template<class R, class T> static void Cmp_numLogic(R result, T a, T b) {
  EXPECT_EQ(cmpResult<R>(!isNan(a) && !isNan(b)), result);
}
template<class R, class T> static void Cmp_nanLogic(R result, T a, T b) {
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

template<class R, class T> static void Cvt_Logic(R result, T a) {
  if(isBool<R>()) {
    EXPECT_EQ(result, a != T(0));
  } else if(isNan(a)) {
    EXPECT_EQ(0, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if(isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if(T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_EQ(R(a), result);
  }
}
template<class R, class T> static void Cvt_upi_Logic(R result, T a) {
  if(isNan(a)) {
    EXPECT_EQ(0, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if(isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if(T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_LE(a, result);
  }
}
template<class R, class T> static void Cvt_downi_Logic(R result, T a) {
  if(isNan(a)) {
    EXPECT_EQ(0, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if(isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if(T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(a, result);
  }
}
template<class R, class T> static void Cvt_zeroi_Logic(R result, T a) {
  if(isNan(a)) {
    EXPECT_EQ(0, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if(isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if(T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(llabs((int64_t) a), llabs((int64_t) result));
  }
}
template<class R, class T> static void Cvt_neari_Logic(R result, T a) {
  if(isNan(a)) {
    EXPECT_EQ(0, result);
  } else if(isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if(isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if(T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(0.5, std::abs(f64(result) - f64(a)));
  }
}
MakeCvtI2FTest(Cvt_)
MakeCvtF2ITest(f32)
MakeCvtF2ITest(f64)
MakeCvtX2ITestRet(f32, b1, Cvt_)
MakeCvtX2ITestRet(f64, b1, Cvt_)
MakeCvtF2FsTest(f32)
MakeCvtF2FsTest(f64)
MakeCvtF2FdTest(up,   a <= result)
MakeCvtF2FdTest(down, a >= result)
MakeCvtF2FdTest(zero, std::abs(a) >= std::abs(result))
MakeCvtF2FdTest(near, (f32) a == result)

template<class T> static void AtomicAndLogic(T result, T a, T b) {
  EXPECT_EQ(T(a & b), result);
}
TestAll(AtomicInst, And, Binary)

template<class T> static void AtomicOrLogic(T result, T a, T b) {
  EXPECT_EQ(T(a | b), result);
}
TestAll(AtomicInst, Or, Binary)

template<class T> static void AtomicXorLogic(T result, T a, T b) {
  EXPECT_EQ(T(a ^ b), result);
}
TestAll(AtomicInst, Xor, Binary)

template<class T> static void AtomicExchLogic(T result, T a, T b) {
  EXPECT_EQ(b, result);
}
TestAll(AtomicInst, Exch, Binary)

template<class T> static void AtomicAddLogic(T result, T a, T b) {
  if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(a) && isNegInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(a) && isPosInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a + b), result);
  }
}
TestAll(AtomicInst, Add, Binary)

template<class T> static void AtomicSubLogic(T result, T a, T b) {
  if(isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(a) && isPosInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(a) && isNegInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a - b), result);
  }
}
TestAll(AtomicInst, Sub, Binary)

template<class T> static void AtomicCasLogic(T result, T a, T b, T c) {
  if(a == b) {
    EXPECT_EQ(c, result);
  } else {
    EXPECT_EQ(a, result);
  }
}
TestAll(AtomicInst, Cas, Ternary)


