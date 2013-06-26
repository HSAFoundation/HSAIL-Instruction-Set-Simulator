//===- brig_runtime_test.cc -----------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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
  if (!isNan(result) && !isNan(a))  {
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
  if (isNan(a)) {
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

template<class T> static void CeilLogic(T result, T a) {
  if (isInf(a)) {
    EXPECT_PRED1(isInf<T>, result);
    return;
  }
  if (isNan<T>(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }
  EXPECT_GE(result, a);
  EXPECT_LE(result, a+1);
  EXPECT_FLOAT_EQ(trunc(result), result);
}
TestAll(FloatInst, Ceil, Unary)

template<class T> static void FloorLogic(T result, T a) {
  if (isInf(a)) {
    EXPECT_PRED1(isInf<T>, result);
    return;
  }

  if (isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }

  EXPECT_GE(result, a-1);
  EXPECT_LE(result, a);
  EXPECT_FLOAT_EQ(trunc(result), result);
}
TestAll(FloatInst, Floor, Unary)

template<class T> static void TruncLogic(T result, T a) {
  if (isInf(a)) {
    EXPECT_PRED1(isInf<T>, result);
    return;
  }

  if (isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }
  EXPECT_FLOAT_EQ(trunc(a), result);
}
TestAll(FloatInst, Trunc, Unary)

template<class T> static void RintLogic(T result, T a) {
  if (isInf(a)) {
    EXPECT_PRED1(isInf<T>, result);
    return;
  }
  if (isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }

  EXPECT_FLOAT_EQ(rint(a), result);
}
TestAll(FloatInst, Rint, Unary)

template<class T> static void AddLogic(T result, T a, T b) {
  if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if ((isPosInf(a) && isNegInf(b))) {
    EXPECT_PRED1(isNan<T>, result);
  } else if ((isNegInf(a) && isPosInf(b))) {
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

template<class T> static void Add_SatLogic(T result, T a, T b) {
  s64 result64 = s64(a) + s64(b);
  if (result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if (result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void Add_SatVectorLogic(T result, T a, T b) {
  ForEach(Add_SatLogic, result, a, b);
}
TestAll(SignedVectorInst,   Add_Sat, Binary)
TestAll(UnsignedVectorInst, Add_Sat, Binary)

template<class T> static void DivLogic(T result, T a, T b) {
  if (hsa::brig::isDivisionError(a, b)) {
    EXPECT_EQ(result, 0);
  } else if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (a == 0 && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a) && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a / b), result);
  }
}
TestAll(SignedInst,         Div, Binary)
TestAll(UnsignedInst,       Div, Binary)
TestAll(FloatInst,          Div, Binary)

template<class T> static void MulLogic(T result, T a, T b) {
  if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (a == 0 && isInf(b)) {
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

template<class T> static void Mul_SatLogic(T result, T a, T b) {
  // Handle signed and unsigned ints differently to avoid overflow into the sign
  // bit.
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty result64 = Int64Ty(a) * Int64Ty(b);
  if (result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if (result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void Mul_SatVectorLogic(T result, T a, T b) {
  ForEach(Mul_SatLogic, result, a, b);
}
TestAll(SignedVectorInst,   Mul_Sat, Binary)
TestAll(UnsignedVectorInst, Mul_Sat, Binary)

template<class T> static void Sub_SatLogic(T result, T a, T b) {
  s64 result64 = s64(a) - s64(b);
  if (result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if (result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template<class T> static void Sub_SatVectorLogic(T result, T a, T b) {
  ForEach(Sub_SatLogic, result, a, b);
}
TestAll(SignedVectorInst,   Sub_Sat, Binary)
TestAll(UnsignedVectorInst, Sub_Sat, Binary)

template<class T> static void MaxLogic(T result, T a, T b) {
  if (!isNegZero(a) && !isNegZero(b))
    EXPECT_PRED2(Not<T>, isNegZero<T>, result);

  if (isNan(a) && isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNan(a) || isNan(b)) {
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
  if (!isPosZero(a) && !isPosZero(b))
    EXPECT_PRED2(Not<T>, isPosZero<T>, result);

  if (isNan(a) && isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNan(a) || isNan(b)) {
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
  if (hsa::brig::isDivisionError(a, b)) {
    EXPECT_EQ(result, 0);
  } else if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (a == 0 && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a) && isInf(b)) {
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
  if (Int<T>::isSigned) {
    bool isNegA = Int<T>::isNeg(a);
    bool isNegB = Int<T>::isNeg(b);
    bool isNegResult = Int<T>::isNeg(a - b);
    if (isNegA == isNegB) EXPECT_EQ(isNegResult, result);
    else EXPECT_EQ(!isNegA && isNegB, result);
  } else {
    EXPECT_EQ(a < a - b, result);
  }
}
TestAll(SignedInst,         Borrow, Binary)
TestAll(UnsignedInst,       Borrow, Binary)

template<class T> static void MadLogic(T result, T a, T b, T c) {
  if (isNan(a) || isNan(b) || isNan(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (a == 0 && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNegInf(a * b) && isPosInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isPosInf(a * b) && isNegInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(a * b + c, result);
  }
}
TestAll(SignedInst,         Mad, Ternary)
TestAll(UnsignedInst,       Mad, Ternary)

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
  for (unsigned i = 0; i < T::Len / 2; ++i) {
    if (!isNan(a[i]) &&
       !isNan(result[i * 2]))
    {
      EXPECT_EQ(a[i],
                      result[i * 2]);
    }
    if (!isNan(b[i]) &&
       !isNan(result[i * 2 + 1]))
    {
      EXPECT_EQ(b[i],
                      result[i * 2 + 1]);
    }
  }
}
TestAll(UnpackInst, UnpackLo, Binary)

template<class T> static void UnpackHiLogic(T result, T a, T b) {
  for (unsigned i = 0; i < T::Len / 2; ++i) {
    if (!isNan(a[i + T::Len /2]) &&
         !isNan(result[i * 2]))
    {
      EXPECT_EQ(a[i + T::Len / 2],
                      result[i * 2]);
    }
    if (!isNan(b[i + T::Len / 2]) &&
       !isNan(result[i * 2 + 1]))
    {
      EXPECT_EQ(b[i + T::Len / 2],
                      result[i * 2 + 1]);
    }
  }
}
TestAll(UnpackInst, UnpackHi, Binary)

template<class R, class T> static void PackLogic(R result, R src0,
                                                 T src1,   u32 src2) {
                                                 
  if (isNan(src1) || isNan(src2) ||
      isInf(src1) || isInf(src2)) 
    return;
    
  unsigned Len = R::Len;
  if (src2 >= Len)
    return;
  
  for (unsigned idx = 0; idx < Len; idx++) {
    if (isNan(src0[idx]) ||
        isInf(src0[idx]))
      return;
      
    if (idx == src2)
      EXPECT_EQ(typename R::Base(src1), result[src2]);
    else
      EXPECT_EQ(src0[idx], result[idx]);        
  }        
}
PackInst(declare)
TestPackInst(Pack)

template<class R, class T> static void UnpackLogic(R result, T src0, u32 src1) {
  unsigned Len = T::Len;
  if (isNan(src1) || isInf(src1))
    return;
  if (src1 >= Len)
    return;
  
  if (isNan(src0[src1]) ||
      isInf(src0[src1]))
    return;

  EXPECT_EQ(R(src0[src1]), result);    
}
UnpackInst2(declare)
TestUnpackInst2(Unpack)

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
  while (bits != 0) {
    if (bits & 1) ++d;
    bits >>= 1;
  }
  EXPECT_EQ(d, result);
}
declareUnary(PopCount_u32, b32)
declareUnary(PopCount_u32, b64)
MakeTest(PopCount_u32_b32, PopCountLogic)
MakeTest(PopCount_u32_b64, PopCountLogic)

// Bit reverse implementation loosely adapted from Sean Eron Anderson's article
// at: http://graphics.stanford.edu/~seander/bithacks.html
template<class T> static void BitRevLogic(T result, T a) {
  union Helper {
    u8 a[sizeof(T)];
    T  t;
  };

  Helper orig, rev;
  orig.t = a;
  for (unsigned i = 0; i < sizeof(T); ++i) {
    rev.a[i] = orig.a[sizeof(T) - i - 1];
  }

  for (unsigned i = 0; i < sizeof(T); ++i) {
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

template<class T> static void BitExtractLogic(T result, T a, T b, T c) {

  unsigned offset = Int<T>::ShiftMask & b;
  unsigned width  = Int<T>::ShiftMask & c;

  if (width && offset + width <= Int<T>::Bits) {
    T mask = ~(~T(0) << width);
    T noSign = (a >> offset) & mask;
    T signEx = noSign << (Int<T>::Bits - width) >> (Int<T>::Bits - width);
    EXPECT_EQ(signEx, result);
  }
}
TestAll(SignedInst, BitExtract, Ternary)
TestAll(UnsignedInst, BitExtract, Ternary)

template<class T> static void BitInsertLogic(T result, T a, T b, b32 c, b32 d) {
  b32 width  = c & b32(Int<T>::Bits - 1);
  b32 offset = d & b32(Int<T>::Bits - 1);
  T resultNe = a;
  for (unsigned i = 0; i < width && offset + i < Int<T>::Bits; ++i) {
    resultNe &= ~(T(1) << (offset + i));
    resultNe |= (b << offset) & (T(1) << (offset + i));
  }
  resultNe |= (b & (~T(0) << width)) << offset;
  EXPECT_EQ(resultNe, result);
}
declareQuaternary(BitInsert, b32)
declareQuaternary(BitInsert, b64)
MakeTest(BitInsert_b32, BitInsertLogic)
MakeTest(BitInsert_b64, BitInsertLogic)

template<class T> static void BitSelectLogic(T result, T a, T b, T c) {
  EXPECT_EQ(b &  a, result &  a);
  EXPECT_EQ(c & ~a, result & ~a);
}
declareTernary(BitSelect, b32)
declareTernary(BitSelect, b64)
MakeTest(BitSelect_b32, BitSelectLogic)
MakeTest(BitSelect_b64, BitSelectLogic)

template<class T> static void FirstBit_u32Logic(T result, T a) {

  if (Int<T>::isNeg(a)) a = ~a;

  if (a == 0) {
    EXPECT_EQ(~T(0), result);
    return;
  }

  typedef typename Int<T>::Unsigned Unsigned;
  Unsigned mask = Int<Unsigned>::HighBit;
  while (!(a & mask)) mask >>= 1;
  EXPECT_EQ(mask, T(1) << (Int<T>::Bits - result - 1));
}
TestAll(SignedInst, FirstBit_u32, Unary)
TestAll(UnsignedInst, FirstBit_u32, Unary)

template<class T> static void LastBit_u32Logic(T result, T a) {

  if (!a) {
    EXPECT_EQ(~T(0), result);
    return;
  }

  typedef typename Int<T>::Unsigned Unsigned;
  T mask = 1;
  while (!(a & mask)) mask <<= 1;
  EXPECT_EQ(mask, T(1) << result);
}
TestAll(SignedInst, LastBit_u32, Unary)
TestAll(UnsignedInst, LastBit_u32, Unary)

template<class T> static void ShuffleLogic(T result, T a, T b, b32 c) {

  typedef typename T::Base Base;

  unsigned len   = T::Len;
  unsigned mask  = len - 1;
  unsigned shift = T::LogLen;
  b32 shuffle = c;

  for (unsigned i = 0; i < len / 2; ++i) {
    unsigned offset = shuffle & mask;
    if (isNan(a[offset])) {
      EXPECT_PRED1(isNan<Base>, result[i]);
    } else {
      EXPECT_EQ(a[offset], result[i]);
    }
    shuffle >>= shift;
  }

  for (unsigned i = len / 2; i < len; ++i) {
    unsigned offset = shuffle & mask;
    if (isNan(b[offset])) {
      EXPECT_PRED1(isNan<Base>, result[i]);
    } else {
      EXPECT_EQ(b[offset], result[i]);
    }
    shuffle >>= shift;
  }
}
TestAll(ShuffleVectorInst, Shuffle, Ternary)

template<class T> static void CmovLogic(T result, T a, T b, T c) {
  if (a == 0) {
    if (isNan(c)) {
      EXPECT_PRED1(isNan<T>, result);
    } else {
      EXPECT_EQ(c, result);
    }
  } else {
    if (isNan(b)) {
      EXPECT_PRED1(isNan<T>, result);
    } else {
      EXPECT_EQ(b, result);
    }
  }
}
TestAll(BitInst, Cmov, Ternary)

template<class T> static void CmovVectorLogic(T result, T a, T b, T c) {
  ForEach(CmovLogic, result, a, b, c);
}
TestAll(SignedVectorInst, Cmov, Ternary)
TestAll(UnsignedVectorInst, Cmov, Ternary)
TestAll(FloatVectorInst, Cmov, Ternary)

template<class T> static void FractLogic(T result, T a) {

  if (isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
    return;
  }

  EXPECT_GT(1, result);
  EXPECT_LE(0, result);
  if (a >= 0 || a <= -FLT_EPSILON)
    EXPECT_DOUBLE_EQ(a, std::floor(a) + result);
  else
    EXPECT_FLOAT_EQ(1.0f, result);
}
TestAll(FloatInst, Fract, Unary)

template<class T> static void SqrtLogic(T result, T a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<T>, a);
  } else if (a >= 0.0) {
    EXPECT_FLOAT_EQ(a, result * result);
  }
}
TestAll(FloatInst, Sqrt, Unary)

template<class T> static void FmaLogic(T result, T a, T b, T c) {
  if (isNan(a) || isNan(b) || isNan(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a) && b == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (a == 0 && isInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (!isInf(a) && !isInf(b) && isInf(c)) {
    EXPECT_EQ(c, result);
  } else if (isNegInf(a * b) && isPosInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isPosInf(a * b) && isNegInf(c)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isInf(a * b) && !isInf(c)) {
    EXPECT_FLOAT_EQ(a * b, result - c);
  } else if (result >= FLT_EPSILON || result <= -FLT_EPSILON) {
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
  if (b & hsa::brig::SNan && isSNan(a))
    EXPECT_TRUE(result);
  if (b & hsa::brig::QNan && isQNan(a))
    EXPECT_TRUE(result);
  if (b & hsa::brig::NegInf && isNegInf(a))
    EXPECT_TRUE(result);
  if (b & hsa::brig::NegNorm && fpclass == FP_NORMAL && a < 0)
    EXPECT_TRUE(result);
  if (b & hsa::brig::NegSubnorm && fpclass == FP_SUBNORMAL && a < 0)
    EXPECT_TRUE(result);
  if (b & hsa::brig::NegZero && isNegZero(a))
    EXPECT_TRUE(result);
  if (b & hsa::brig::PosZero && isPosZero(a))
    EXPECT_TRUE(result);
  if (b & hsa::brig::PosSubnorm && fpclass == FP_SUBNORMAL && a > 0)
    EXPECT_TRUE(result);
  if (b & hsa::brig::PosNorm && fpclass == FP_NORMAL && a > 0)
    EXPECT_TRUE(result);
  if (b & hsa::brig::PosInf && isPosInf(a))
    EXPECT_TRUE(result);
}
extern "C" b1 Class_f32(f32, unsigned);
extern "C" b1 Class_f64(f64, unsigned);
MakeTest(Class_f32, ClassLogic)
MakeTest(Class_f64, ClassLogic)

static void Ncos_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if (fpclass == FP_NORMAL && (-512 * M_PI > a || a > 512 * M_PI)) {
    EXPECT_FLOAT_EQ(cos(1.0), result);
  } else if (fpclass == FP_SUBNORMAL) {
    EXPECT_EQ(1.0, result);
  } else if (isNegZero(a)) {
    EXPECT_EQ(1.0, result);
  } else if (isPosZero(a)) {
    EXPECT_EQ(1.0, result);
  } else {
    EXPECT_FLOAT_EQ(cos(a), result);
  }
}
extern "C" f32 Ncos_f32(f32);
MakeTest(Ncos_f32, Ncos_f32_Logic)

static void Nsin_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a) || isInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if (fpclass == FP_NORMAL && (-512 * M_PI > a || a > 512 * M_PI)) {
    EXPECT_FLOAT_EQ(sin(1.0), result);
  } else if (fpclass == FP_SUBNORMAL && a > 0.0) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else if (fpclass == FP_SUBNORMAL && a < 0.0) {
    EXPECT_PRED1(isNegZero<f32>, result);
  } else if (isNegZero(a)) {
    EXPECT_PRED1(isNegZero<f32>, result);
  } else if (isPosZero(a)) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else {
    EXPECT_FLOAT_EQ(sin(a), result);
  }
}
extern "C" f32 Nsin_f32(f32);
MakeTest(Nsin_f32, Nsin_f32_Logic)

static void Nlog2_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a) || isNegInf(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<f32>, result);
  } else if (fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if (fpclass == FP_SUBNORMAL) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if (isNegZero(a)) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else if (isPosZero(a)) {
    EXPECT_PRED1(isNegInf<f32>, result);
  } else {
    EXPECT_FLOAT_EQ(log2(a), result);
  }
}
extern "C" f32 Nlog2_f32(f32);
MakeTest(Nlog2_f32, Nlog2_f32_Logic)

static void Nexp2_f32_Logic(f32 result, f32 a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a)) {
    EXPECT_PRED1(isNan<f32>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<f32>, result);
  } else if (fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isPosZero<f32>, result);
  } else if (fpclass == FP_SUBNORMAL) {
    EXPECT_EQ(1.0, result);
  } else if (isNegZero(a)) {
    EXPECT_EQ(1.0, result);
  } else if (isPosZero(a)) {
    EXPECT_EQ(1.0, result);
  } else {
    EXPECT_FLOAT_EQ(exp2(a), result);
  }
}
extern "C" f32 Nexp2_f32(f32);
MakeTest(Nexp2_f32, Nexp2_f32_Logic)

template<class T> static void NrsqrtLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a) || isNegInf(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (fpclass == FP_NORMAL && a < 0.0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (fpclass == FP_SUBNORMAL && a < 0.0) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if (isNegZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if (isPosZero(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if (fpclass == FP_SUBNORMAL && a > 0.0) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(0.0, result);
  } else {
    EXPECT_FLOAT_EQ(a, (1.0 / result) * (1.0 / result));
  }
}
TestAll(FloatInst, Nrsqrt, Unary)

template<class T> static void NrcpLogic(T result, T a) {
  int fpclass = std::fpclassify(a);
  if (isNan(a)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegZero<T>, result);
  } else if (fpclass == FP_SUBNORMAL && a < 0) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if (isNegZero(a)) {
    EXPECT_PRED1(isNegInf<T>, result);
  } else if (isPosZero(a)) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if (fpclass == FP_SUBNORMAL && a > 0) {
    EXPECT_PRED1(isPosInf<T>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosZero<T>, result);
  } else {
    EXPECT_FLOAT_EQ(a, 1.0 / result);
  }
}
TestAll(FloatInst, Nrcp, Unary)

static void BitAlign_b32_Logic(b32 result, b32 a, b32 b, b32 c ) {
  if (c < 32) {
    for (unsigned i=0; i < 32; ++i) {
      if (i + c > 31)
        EXPECT_EQ((b >> (i + c - 32)) & 0x1,
                  (result >> i) & 0x1);
      else
        EXPECT_EQ((a >> (i + c)) & 0x1,
                  (result >> i) & 0x1);
    }
  }
}
extern "C" b32 BitAlign_b32(b32, b32, b32);
MakeTest(BitAlign_b32, BitAlign_b32_Logic)

static void ByteAlign_b32_Logic(b32 result, b32 a, b32 b, b32 c ) {
  if (c < 4) {
    for (unsigned i=0; i < 3; ++i) {
      if (i + c > 3)
        EXPECT_EQ((b >> (i+c-4)*8) & 0xFF,
                  (result >> i*8)  & 0xFF);
      else 
        EXPECT_EQ((a >> (i+c)*8)  & 0xFF,
                  (result >> i*8) & 0xFF);  
    }
  }
}
extern "C" b32 ByteAlign_b32(b32, b32, b32);
MakeTest(ByteAlign_b32, ByteAlign_b32_Logic)

static void Lerp_u8x4_Logic(u8x4 result, u8x4 a, u8x4 b, u8x4 c) {
  for (unsigned i = 0; i < 4; ++i) {
    EXPECT_EQ(result[i],
              (a[i] + b[i] + (c[i] & 0x1))>>1);
  }
}
extern "C" u8x4 Lerp_u8x4(u8x4, u8x4, u8x4);
MakeVectorTest(Lerp_u8x4, Lerp_u8x4_Logic)

static void Sad_u32_u32_Logic(u32 result, u32 a, u32 b, u32 c) {
  EXPECT_EQ(abs(a - b) + c, result);
}
extern "C" u32 Sad_u32_u32(u32, u32, u32);
MakeTest(Sad_u32_u32, Sad_u32_u32_Logic)

static void Sad_u32_u16x2_Logic(u32 result, u16x2 a, u16x2 b, u32 c) {
  EXPECT_EQ(abs(a[0] - b[0]) +
            abs(a[1] - b[1]) +
            c,
            result);
}
extern "C" u32 Sad_u32_u16x2(u16x2, u16x2, u32);
MakeVectorTest(Sad_u32_u16x2, Sad_u32_u16x2_Logic)

static void Sad_u32_u8x4_Logic(u32 result, u8x4 a, u8x4 b, u32 c) {
  EXPECT_EQ(abs(a[0] - b[0]) +
            abs(a[1] - b[1]) +
            abs(a[2] - b[2]) +
            abs(a[3] - b[3]) +
            c,
            result);
}
extern "C" u32 Sad_u32_u8x4(u8x4, u8x4, u32);
MakeVectorTest(Sad_u32_u8x4, Sad_u32_u8x4_Logic)

static void SadHi_u16x2_u8x4_Logic(u16x2 result, u8x4 a, u8x4 b, u16x2 c) {
  EXPECT_EQ((abs(a[0] - b[0]) +
             abs(a[1] - b[1]) +
             abs(a[2] - b[2]) +
             abs(a[3] - b[3]) +
             c[1]) & 0xFFFF,
             result[1]);
  EXPECT_EQ( c[0],
             result[0]);            
}
extern "C" u16x2 Sadhi_u16x2_u8x4(u8x4, u8x4, u16x2);
MakeSadhiVectorTest()

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
  if (isBool<R>()) {
    EXPECT_EQ(result, a != T(0));
  } else if (isNan(a)) {
    EXPECT_EQ(0, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if (isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_EQ(R(a), result);
  }
}

// int to float or float to smaller float
template<class R, class T> static void Cvt_up_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<R>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<R>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegInf<R>, result);
  } else {
    EXPECT_LE(R(a), result);
  }
}

template<class R, class T> static void Cvt_down_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<R>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<R>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegInf<R>, result);
  } else {
    EXPECT_GE(R(a), result);
  }
}

template<class R, class T> static void Cvt_zero_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<R>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<R>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegInf<R>, result);
  } else {
    EXPECT_GE(std::abs(R(a)), std::abs(result));
  }
}

template<class R, class T> static void Cvt_near_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<R>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<R>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegInf<R>, result);
  } else {
    EXPECT_GE(0.5, std::abs(result - R(a)));
  }
}

template<class R, class T> static void Cvt_upi_Logic(R result, T a) {
  if (isNan(a) || isPosInf(a) || isNegInf(a)) {
    EXPECT_EQ(0, result); // should have an exception
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_LE(a, result);
  } else {
    EXPECT_EQ(0, result); // should have an exception
  }
}
template<class R, class T> static void Cvt_ups_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_EQ(0, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if (isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_LE(a, result);
  }
}
template<class R, class T> static void Cvt_downi_Logic(R result, T a) {
  if (isNan(a) || isPosInf(a) || isNegInf(a)) {
    EXPECT_EQ(0, result); // should have an exception
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(a, result);
  } else {
    EXPECT_EQ(0, result); // should have an exception
  }
}
template<class R, class T> static void Cvt_downs_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_EQ(0, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if (isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(a, result);
  }
}
template<class R, class T> static void Cvt_zeroi_Logic(R result, T a) {
  if (isNan(a) || isPosInf(a) || isNegInf(a)) {
    EXPECT_EQ(0, result); // should have an exception
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(llabs((int64_t) a), llabs((int64_t) result));
  } else {
    EXPECT_EQ(0, result); // should have an exception
  }
}
template<class R, class T> static void Cvt_zeros_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_EQ(0, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if (isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(llabs((int64_t) a), llabs((int64_t) result));
  }
}
template<class R, class T> static void Cvt_neari_Logic(R result, T a) {
  if (isNan(a) || isPosInf(a) || isNegInf(a)) {
    EXPECT_EQ(0, result); // should have an exception
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(0.5, std::abs(f64(result) - f64(a)));
  } else {
    EXPECT_EQ(0, result); // should have an exception
  }
}
template<class R, class T> static void Cvt_nears_Logic(R result, T a) {
  if (isNan(a)) {
    EXPECT_EQ(0, result);
  } else if (isPosInf(a)) {
    EXPECT_EQ(getMax<R>(), result);
  } else if (isNegInf(a)) {
    EXPECT_EQ(getMin<R>(), result);
  } else if (T(getMin<R>()) < a && a < T(getMax<R>())) {
    EXPECT_GE(0.5, std::abs(f64(result) - f64(a)));
  }
}
// Integer to float
MakeCvtI2FTest(Cvt_up_)
MakeCvtI2FTest(Cvt_down_)
MakeCvtI2FTest(Cvt_zero_)
MakeCvtI2FTest(Cvt_near_)
// Float to integer
MakeCvtF2ITest(f32)
MakeCvtF2ITest(f64)
// Float to bool
MakeCvtX2ITestRet(f32, b1, Cvt_)
MakeCvtX2ITestRet(f64, b1, Cvt_)
// Float to float (same size)
MakeCvtF2FsTest(f32)
MakeCvtF2FsTest(f64)
// Float to smaller size float
MakeCvtF2FdTest(up,   a <= result)
MakeCvtF2FdTest(down, a >= result)
MakeCvtF2FdTest(zero, std::abs(a) >= std::abs(result))
MakeCvtF2FdTest(near, (f32) a == result)
// Float to larger size float
static void Cvt_f64_f32_Logic(f64 result, f32 a) {
  if (isNan(a)) {
    EXPECT_PRED1(isNan<f64>, result);
  } else if (isPosInf(a)) {
    EXPECT_PRED1(isPosInf<f64>, result);
  } else if (isNegInf(a)) {
    EXPECT_PRED1(isNegInf<f64>, result);
  } else {
    EXPECT_TRUE((f64)a == result);
  }
}
extern "C" f64 Cvt_f64_f32(f32);
MakeTest(Cvt_f64_f32, Cvt_f64_f32_Logic)

extern "C" void PackCvtLogic(u8x4 result, 
                             f32 src0, 
                             f32 src1, 
                             f32 src2, 
                             f32 src3) {
  if (isNan(src0) || isNegInf(src0) || src0 <= 0.0f) {
    EXPECT_EQ(0, result[0]);
  } else if (isPosInf(src0) || src0 >= 255.0) {
    EXPECT_EQ(255, result[0]);
  } else {
    EXPECT_EQ(result[0], u8(nearbyint(src0)));
  }
    
  if (isNan(src1) || isNegInf(src1) || src1 <= 0.0f) {
    EXPECT_EQ(0, result[1]);
  } else if (isPosInf(src1) || src1 >= 255.0) {
    EXPECT_EQ(255, result[1]);
  } else {
    EXPECT_EQ(result[1], u8(nearbyint(src1))); 
  }
    
  if (isNan(src2) || isNegInf(src2) || src2 <= 0.0f) {
    EXPECT_EQ(0, result[2]);
  } else if (isPosInf(src2) || src2 >= 255.0) {
    EXPECT_EQ(255, result[2]);
  } else {
    EXPECT_EQ(result[2], u8(nearbyint(src2))); 
  }
    
  if (isNan(src3) || isNegInf(src3) || src3 <= 0.0f) {
    EXPECT_EQ(0, result[3]);
  } else if (isPosInf(src3) || src3 >= 255.0) {
    EXPECT_EQ(255, result[3]);              
  } else {
    EXPECT_EQ(result[3], u8(nearbyint(src3))); 
  }               
} 
extern "C" u8x4 PackCvt_u8x4_f32(f32, f32, f32, f32);
MakeTest(PackCvt_u8x4_f32, PackCvtLogic)

extern "C" void UnpackCvtLogic(f32 result,
                               u8x4 src0,
                               u32  src1) {
  if (src1 > 3)
    EXPECT_FLOAT_EQ(result, 0.0);
  else
    EXPECT_FLOAT_EQ(result, src0[src1]);                              
}
extern "C" f32 UnpackCvt_f32_u8x4(u8x4, u32);
MakeVectorTest(UnpackCvt_f32_u8x4, UnpackCvtLogic)

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
  if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isPosInf(a) && isNegInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNegInf(a) && isPosInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a + b), result);
  }
}
TestAll(AtomicInst, Add, Binary)

template<class T> static void AtomicSubLogic(T result, T a, T b) {
  if (isNan(a) || isNan(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isPosInf(a) && isPosInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if (isNegInf(a) && isNegInf(b)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(T(a - b), result);
  }
}
TestAll(AtomicInst, Sub, Binary)

template<class T> static void AtomicCasLogic(T result, T a, T b, T c) {
  if (a == b) {
    EXPECT_EQ(c, result);
  } else {
    EXPECT_EQ(a, result);
  }
}
TestAll(AtomicInst, Cas, Ternary)
