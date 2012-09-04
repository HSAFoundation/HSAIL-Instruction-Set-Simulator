#include "brig_runtime_test_internal.h"

#include "gtest/gtest.h"

using hsa::brig::ForEach;
using hsa::brig::Int;
using hsa::brig::isNan;

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

template<class T> static void MadLogic(T result, T x, T y, T z) {
  if(isNan(x) || isNan(y) || isNan(z)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isInf(x) && y == 0) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(x == 0 && isInf(y)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isNegInf(x * y) && isPosInf(z)) {
    EXPECT_PRED1(isNan<T>, result);
  } else if(isPosInf(x * y) && isNegInf(z)) {
    EXPECT_PRED1(isNan<T>, result);
  } else {
    EXPECT_EQ(x * y +z, result);
  }
}
// These test are temporarily disabled since they are VERY slow
// TestAll(SignedInst,         Mad, Ternary)
// TestAll(UnsignedInst,       Mad, Ternary)
// TestAll(FloatInst,          Mad, Ternary)
