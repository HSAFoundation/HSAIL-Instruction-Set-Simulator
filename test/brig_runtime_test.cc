#include "brig_runtime_test_internal.h"

#include "gtest/gtest.h"

using hsa::brig::ForEach;
using hsa::brig::Int;
using hsa::brig::isNan;

template <class T> static void AbsLogic(T result, T a) {
  if(!isNan(result) && !isNan(a))  {
    EXPECT_TRUE(result == a || result == -a);
    // Do a cast back to T since operator- turns subword types into ints.
    EXPECT_LE((T) -a, result);
  } else {
    EXPECT_PRED1(isNan<T>, a);
    EXPECT_PRED1(isNan<T>, result);
  }
}
template <class T> static void AbsVectorLogic(T result, T a) {
  ForEach(AbsLogic, result, a);
}
TestAll(SignedInst,       Abs, Unary)
TestAll(FloatInst,        Abs, Unary)
TestAll(SignedVectorInst, Abs, Unary)
TestAll(FloatVectorInst,  Abs, Unary)

template <class T> static void AddSatLogic(T result, T a, T b) {
  s64 result64 = s64(a) + s64(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template <class T> static void AddSatVectorLogic(T result, T a, T b) {
  ForEach(AddSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   AddSat, Binary)
TestAll(UnsignedVectorInst, AddSat, Binary)

template<class T> static void SubSatLogic(T result, T a, T b) {
  s64 result64 = s64(a) - s64(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template <class T> static void SubSatVectorLogic(T result, T a, T b) {
  ForEach(SubSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   SubSat, Binary)
TestAll(UnsignedVectorInst, SubSat, Binary)

template<class T> static void MulSatLogic(T result, T a, T b) {
  // Handle signed and unsigned ints differently to avoid overflow into the sign
  // bit.
  typedef typename Int<T>::Int64Ty Int64Ty;
  Int64Ty result64 = Int64Ty(a) * Int64Ty(b);
  if(result64 > Int<T>::Max) EXPECT_EQ(Int<T>::Max, result);
  else if(result64 <= Int<T>::Min) EXPECT_EQ(Int<T>::Min, result);
  else EXPECT_EQ(result64, result);
}
template <class T> static void MulSatVectorLogic(T result, T a, T b) {
  ForEach(MulSatLogic, result, a, b);
}
TestAll(SignedVectorInst,   MulSat, Binary)
TestAll(UnsignedVectorInst, MulSat, Binary)

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
template <class T> static void MaxVectorLogic(T result, T a, T b) {
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
template <class T> static void MinVectorLogic(T result, T a, T b) {
  ForEach(MinLogic, result, a, b);
}
TestAll(SignedInst,         Min, Binary)
TestAll(UnsignedInst,       Min, Binary)
TestAll(FloatInst,          Min, Binary)
TestAll(SignedVectorInst,   Min, Binary)
TestAll(UnsignedVectorInst, Min, Binary)
TestAll(FloatVectorInst,    Min, Binary)
