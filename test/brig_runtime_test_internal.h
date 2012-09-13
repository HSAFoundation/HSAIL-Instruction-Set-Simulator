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

#define TestBitInst(INST,NARY)                  \
  MakeTest(INST ## _b1,  INST ## Logic)         \
  MakeTest(INST ## _b32, INST ## Logic)         \
  MakeTest(INST ## _b64, INST ## Logic)

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

#define TestShiftInst(INST,NARY)                      \
  TestSignedInst(INST, NARY)                          \
  TestUnsignedInst(INST, NARY)                        \
  MakeVectorTest(INST ## _s8x4,  INST ## VectorLogic) \
  MakeVectorTest(INST ## _s16x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _s32x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _u8x4,  INST ## VectorLogic) \
  MakeVectorTest(INST ## _u16x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _u32x2, INST ## VectorLogic)

#define TestUnpackInst(INST,NARY)                       \
  MakeVectorTest(INST ## _s8x4,   INST ## Logic)        \
  MakeVectorTest(INST ## _s8x8,   INST ## Logic)        \
  MakeVectorTest(INST ## _s16x2,  INST ## Logic)        \
  MakeVectorTest(INST ## _s16x4,  INST ## Logic)        \
  MakeVectorTest(INST ## _s32x2,  INST ## Logic)        \
  MakeVectorTest(INST ## _u8x4,   INST ## Logic)        \
  MakeVectorTest(INST ## _u8x8,   INST ## Logic)        \
  MakeVectorTest(INST ## _u16x2,  INST ## Logic)        \
  MakeVectorTest(INST ## _u16x4,  INST ## Logic)        \
  MakeVectorTest(INST ## _u32x2,  INST ## Logic)        \
  /* MakeVectorTest(INST ## _f16x2,  INST ## Logic )*/  \
  /* MakeVectorTest(INST ## _f16x4,  INST ## Logic) */  \
  MakeVectorTest(INST ## _f32x2,  INST ## Logic)

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

#define TestShuffleVectorInst(INST,NARY)              \
  MakeVectorTest(INST ## _s8x4,  INST ## Logic)       \
  MakeVectorTest(INST ## _s16x2, INST ## Logic)       \
  MakeVectorTest(INST ## _s32x2, INST ## Logic)       \
  MakeVectorTest(INST ## _u8x4,  INST ## Logic)       \
  MakeVectorTest(INST ## _u16x2, INST ## Logic)       \
  MakeVectorTest(INST ## _u32x2, INST ## Logic)       \
  /* MakeVectorTest(INST ## _f16x2, INST ## Logic) */ \
  MakeVectorTest(INST ## _f32x2, INST ## Logic)

#define MakeCmpTest(FUNC,TYPE)                                          \
  MakeTest(Cmp_ ## FUNC ## _b1_  ## TYPE,   Cmp_ ## FUNC ## Logic)      \
  MakeTest(Cmp_ ## FUNC ## _b32_ ## TYPE, Cmp_ ## FUNC ## Logic)        \
  MakeTest(Cmp_ ## FUNC ## _s32_ ## TYPE, Cmp_ ## FUNC ## Logic)        \
  MakeTest(Cmp_ ## FUNC ## _u32_ ## TYPE, Cmp_ ## FUNC ## Logic)        \
  /* MakeTest(Cmp_ ## FUNC ## _f16_ ## TYPE, Cmp_ ## FUNC ## Logic) */  \
  MakeTest(Cmp_ ## FUNC ## _f32_ ## TYPE, Cmp_ ## FUNC ## Logic)

#define TestCmp(FUNC,PRED)                                  \
  template<class R, class T>                                \
  static void Cmp_ ## FUNC ## Logic(R result, T a, T b) {   \
    if(isNan(a) || isNan(b)) {                              \
      EXPECT_EQ(cmpResult<R>(false), result);               \
    } else {                                                \
      EXPECT_EQ(cmpResult<R>(PRED), result);                \
    }                                                       \
  }                                                         \
  template<class R, class T>                                \
  static void Cmp_ ## FUNC ## uLogic(R result, T a, T b) {  \
    if(isNan(a) || isNan(b)) {                              \
      EXPECT_EQ(cmpResult<R>(true), result);                \
    } else {                                                \
      EXPECT_EQ(cmpResult<R>(PRED), result);                \
    }                                                       \
  }                                                         \
  Cmp(declare, FUNC, b32)                                   \
  Cmp(declare, FUNC, b64)                                   \
  Cmp(declare, FUNC, s32)                                   \
  Cmp(declare, FUNC, s64)                                   \
  Cmp(declare, FUNC, u32)                                   \
  Cmp(declare, FUNC, u64)                                   \
  /* Cmp(declare, Cmp_ ## FUNC, f16) */                     \
  FCmp(declare, FUNC, f32)                                  \
  FCmp(declare, FUNC, f64)                                  \
  MakeCmpTest(FUNC, b32)                                    \
  MakeCmpTest(FUNC, b64)                                    \
  MakeCmpTest(FUNC, s32)                                    \
  MakeCmpTest(FUNC, s64)                                    \
  MakeCmpTest(FUNC, u32)                                    \
  MakeCmpTest(FUNC, u64)                                    \
  /* MakeCmpTest(MakeCmpTest_ ## FUNC, f16) */              \
  MakeCmpTest(FUNC, f32)                                    \
  MakeCmpTest(FUNC ## u, f32)                               \
  MakeCmpTest(FUNC, f64)                                    \
  MakeCmpTest(FUNC ## u, f64)

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

template<> void initTestVector(std::vector<bool> &testVector) {
  testVector.push_back(false);
  testVector.push_back(true);
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

  union { b32 b; f32 f; } SNAN = { 0x7FC00000 };
  testVector.push_back(SNAN.f);

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

  union { b64 b; f64 f; } SNAN = { 0x7FF8000000000000UL };
  testVector.push_back(SNAN.f);

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

template<class R, class A>
static void TestInst(R (*Impl)(A), void (*Logic)(R, A)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    A a = getTestVector<A>()[i];
    Logic(Impl(a), a);
  }
}

template<class R, class A, class B>
static void TestInst(R (*Impl)(A, B), void (*Logic)(R, A, B)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      B b = getTestVector<B>()[j];
      Logic(Impl(a, b), a, b);
    }
  }
}

template<class R, class A, class B, class C>
static void TestInst(R (*Impl)(A, B, C), void (*Logic)(R, A, B, C)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      B b = getTestVector<B>()[j];
      for(unsigned k = 0; k < getTestVector<C>().size(); ++k) {
        C c = getTestVector<C>()[k];
        Logic(Impl(a, b, c), a, b, c);
      }
    }
  }
}

template<class R, class A, class B, class C, class D>
static void TestInst(R (*Impl)(A, B, C, D), void (*Logic)(R, A, B, C, D)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      B b = getTestVector<B>()[j];
      for(unsigned k = 0; k < getTestVector<C>().size(); ++k) {
        C c = getTestVector<C>()[k];
        for(unsigned m = 0; m < getTestVector<D>().size(); ++m) {
          D d = getTestVector<D>()[m];
          Logic(Impl(a, b, c, d), a, b, c, d);
        }
      }
    }
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T), void (*Logic)(R, T)) {
  typedef typename hsa::brig::Vec<T>::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = { testVector[i] };
    Logic(Impl(a), a);
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, T), void (*Logic)(R, T, T)) {
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

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, unsigned),
                           void (*Logic)(R, T, unsigned)) {
  typedef typename hsa::brig::Vec<T>::Base Base;
  const std::vector<Base> &testVectorB = getTestVector<Base>();
  const std::vector<unsigned> &testVectorU = getTestVector<unsigned>();
  for(unsigned i = 0; i < testVectorB.size(); ++i) {
    T a = { testVectorB[i] };
    for(unsigned j = 0; j < testVectorU.size(); ++j) {
      unsigned b = testVectorU[j];
      Logic(Impl(a, b), a, b);
    }
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, T, unsigned),
                           void (*Logic)(R, T, T, unsigned)) {
  typedef typename hsa::brig::Vec<T>::Base Base;
  const std::vector<Base> &testVectorB = getTestVector<Base>();
  const std::vector<unsigned> &testVectorU = getTestVector<unsigned>();
  for(unsigned i = 0; i < testVectorB.size(); ++i) {
    T a = { testVectorB[i] };
    for(unsigned j = 0; j < testVectorB.size(); ++j) {
      T b = { testVectorB[j] };
      for(unsigned k = 0; k < testVectorB.size(); ++k) {
        unsigned c = testVectorU[k];
        Logic(Impl(a, b, c), a, b, c);
      }
    }
  }
}

template<class T> static bool Not(bool (*Fn)(T), T t) { return !Fn(t); }

#endif // BRIG_RUNTIME_TEST_INTERNAL_H
