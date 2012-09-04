#ifndef BRIG_RUNTIME_TEST_INTERNAL_H
#define BRIG_RUNTIME_TEST_INTERNAL_H

#include "brig_runtime.h"
#include "brig_runtime_internal.h"

#include <vector>
#include <cfloat>
#include <cmath>

#define MakeTest(INST,LOGIC)                    \
  TEST(BrigRuntimeTest, INST) {                 \
    TestInst(INST, LOGIC);                      \
  }

#define MakeVectorTest(INST,LOGIC)              \
  TEST(BrigRuntimeTest, INST) {                 \
    TestVectorInst(INST, LOGIC);                \
  }

#define TestAll(TYPE,INST,NARY)                 \
  TYPE(declare, INST, NARY)                     \
  Test ## TYPE(INST, NARY)

#define TestSignedInst(INST,NARY)               \
  MakeTest(INST ## _s32, INST ## Logic)         \
  MakeTest(INST ## _s64, INST ## Logic)

#define TestUnsignedInst(INST,NARY)             \
  MakeTest(INST ## _u32, INST ## Logic)         \
  MakeTest(INST ## _u64, INST ## Logic)

#define TestFloatInst(INST,NARY)                \
  /* MakeTest(INST ## _f16, INST ## Logic) */   \
  MakeTest(INST ## _f32, INST ## Logic)         \
  MakeTest(INST ## _f64, INST ## Logic)

#define TestSignedVectorInst(INST,NARY)         \
  Test ## NARY ## SignedVectorInst(INST)

#define TestUnsignedVectorInst(INST,NARY)       \
  Test ## NARY ## Unsigned ## VectorInst(INST)

#define TestFloatVectorInst(INST,NARY)          \
  Test ## NARY ## FloatVectorInst(INST)

#define TestUnarySignedVectorInst(INST)                 \
  MakeVectorTest(INST ## _P_s8x4,  INST ## VectorLogic) \
  MakeVectorTest(INST ## _P_s16x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _P_s32x2, INST ## VectorLogic)


#define TestUnaryUnsignedVectorInst(INST)               \
  MakeVectorTest(INST ## _P_u8x4,  INST ## VectorLogic) \
  MakeVectorTest(INST ## _P_u16x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _P_u32x2, INST ## VectorLogic)


#define TestUnaryFloatVectorInst(INST)                  \
  /* MakeVectorTest(INST ## _P_f16x2, INST ## Logic) */ \
  MakeVectorTest(INST ## _P_f32x2, INST ## VectorLogic)


#define TestBinarySignedVectorInst(INST)                  \
  MakeVectorTest(INST ## _PP_s8x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_s16x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_s32x2, INST ## VectorLogic)


#define TestBinaryUnsignedVectorInst(INST)                \
  MakeVectorTest(INST ## _PP_u8x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_u16x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_u32x2, INST ## VectorLogic)


#define TestBinaryFloatVectorInst(INST)                   \
  /* MakeVectorTest(INST ## _PP_f16x2, INST ## Logic) */  \
  MakeVectorTest(INST ## _PP_f32x2, INST ## VectorLogic)


template<class T> static void initTestVector(std::vector<T> &testVector) {
  for(unsigned i = 0; i < 255; ++i)
    testVector.push_back(T(i));

  if(sizeof(T) == 1) return;

  for(int i = 1; i < 255; ++i)
    testVector.push_back(-T(i));

  for(unsigned i = 8; i < sizeof(T) * 8; ++i) {
    testVector.push_back(T(1LL) << i);
    testVector.push_back(T(-1LL) << i);
  }

  testVector.push_back(hsa::brig::Int<T>::Max);
}

template<> void initTestVector(std::vector<float> &testVector) {
  testVector.push_back(0.0f);
  testVector.push_back(FLT_MAX);
  testVector.push_back(FLT_EPSILON);
  testVector.push_back(FLT_MIN);
  testVector.push_back((float) M_E);
  testVector.push_back((float) M_LOG2E);
  testVector.push_back((float) M_LN2);
  testVector.push_back((float) M_LN10);
  testVector.push_back((float) M_PI);
  testVector.push_back((float) M_2_SQRTPI);
  testVector.push_back((float) M_SQRT2);
  testVector.push_back((float) M_SQRT1_2);
  testVector.push_back(INFINITY);
  testVector.push_back(NAN);

  for(float f = 1.0f; f != INFINITY; f *= 2)
    testVector.push_back(f);

  for(float f = 1.0f; f != 0; f /= 2)
    testVector.push_back(f);

  for(unsigned i = 0, E = testVector.size(); i != E; ++i)
    testVector.push_back(copysignf(testVector[i], -1.0f));
}

template<> void initTestVector(std::vector<double> &testVector) {
  testVector.push_back(0.0f);
  testVector.push_back(DBL_MAX);
  testVector.push_back(DBL_EPSILON);
  testVector.push_back(DBL_MIN);
  testVector.push_back(M_E);
  testVector.push_back(M_LOG2E);
  testVector.push_back(M_LN2);
  testVector.push_back(M_LN10);
  testVector.push_back(M_PI);
  testVector.push_back(M_2_SQRTPI);
  testVector.push_back(M_SQRT2);
  testVector.push_back(M_SQRT1_2);
  testVector.push_back(INFINITY);
  testVector.push_back(NAN);

  for(double d = 1.0; d != INFINITY; d *= 2)
    testVector.push_back(d);

  for(float d = 1.0; d != 0; d /= 2)
    testVector.push_back(d);

  for(unsigned i = 0, E = testVector.size(); i != E; ++i)
    testVector.push_back(copysign(testVector[i], -1.0));
}

template<class T> static const std::vector<T> &getTestVector() {
  static std::vector<T> testVector;
  if(!testVector.size()) initTestVector(testVector);
  return testVector;
}

template<class T>
static void TestInst(T (*Impl)(T), void (*Logic)(T, T)) {
  const std::vector<T> &testVector = getTestVector<T>();
  for(unsigned i = 0; i < testVector.size(); ++i)
    Logic(Impl(testVector[i]), testVector[i]);
}

template<class T>
static void TestInst(T (*Impl)(T, T), void (*Logic)(T, T, T)) {
  const std::vector<T> &testVector = getTestVector<T>();
  for(unsigned i = 0; i < testVector.size(); ++i)
    for(unsigned j = 0; j < testVector.size(); ++j)
      Logic(Impl(testVector[i], testVector[j]), testVector[i], testVector[j]);
}

template<class T>
static void TestInst(T (*Impl)(T, T, T), void (*Logic)(T, T, T, T)) {
  const std::vector<T> &testVector = getTestVector<T>();
  for(unsigned i = 0; i < testVector.size(); ++i)
    for(unsigned j = 0; j < testVector.size(); ++j)
      for(unsigned k = 0; k < testVector.size(); ++k)
        Logic(Impl(testVector[i], testVector[j], testVector[k]),
              testVector[i], testVector[j], testVector[k]);
}

template<class T>
static void TestVectorInst(T (*Impl)(T), void (*Logic)(T, T)) {
  typedef typename hsa::brig::Vec<T>::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = { testVector[i] };
    Logic(Impl(a), a);
  }
}

template<class T>
static void TestVectorInst(T (*Impl)(T, T), void (*Logic)(T, T, T)) {
  typedef typename hsa::brig::Vec<T>::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = { testVector[i] };
    for(unsigned j = 0; j < testVector.size(); ++j) {
      T b = { testVector[j] };
      Logic(Impl(a, b), a, b);
    }
  }
}

template<class T> static bool isNegZero(T t) { return false; }
template<> bool isNegZero(float  f) {
  return f == 0.0 && copysignf(1.0, f) < 0.0;
}
template<> bool isNegZero(double d) {
  return d == 0.0 && copysign(1.0, d) < 0.0;
}

template<class T> static bool isPosZero(T t) { return !isNegZero(t); }

template<class T> static bool Not(bool (*Fn)(T), T t) { return !Fn(t); }

template<class T> static bool isInf(T t) { return false; }
template<> bool isInf(float f)  { return isinf(f); }
template<> bool isInf(double d) { return isinf(d); }

template<class T> static bool isPosInf(T t) { return false; }
template<> bool isPosInf(float f)  { return isinf(f) && f > 0.0; }
template<> bool isPosInf(double d) { return isinf(d) && d > 0.0; }

template<class T> static bool isNegInf(T t) { return false; }
template<> bool isNegInf(float f)  { return isinf(f) && f < 0.0; }
template<> bool isNegInf(double d) { return isinf(d) && d < 0.0; }

#endif // BRIG_RUNTIME_TEST_INTERNAL_H
