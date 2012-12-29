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

#define MakeAtomicTest(INST,LOGIC,NARY)         \
  TEST(BrigRuntimeTest, INST) {                 \
    TestAtomic ## NARY(INST, Atomic ## LOGIC);  \
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

#define TestShiftInst(INST,NARY)                          \
  TestSignedInst(INST, NARY)                              \
  TestUnsignedInst(INST, NARY)                            \
  MakeVectorTest(INST ## _PS_s8x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PS_s16x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PS_s32x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PS_u8x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PS_u16x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PS_u32x2, INST ## VectorLogic)

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

#define TestAtomicInst(INST,NARY)                                   \
  MakeAtomicTest(Atomic ## INST ## _s32, INST ## Logic, NARY)       \
  MakeAtomicTest(Atomic ## INST ## _s64, INST ## Logic, NARY)       \
  MakeAtomicTest(Atomic ## INST ## _u32, INST ## Logic, NARY)       \
  MakeAtomicTest(Atomic ## INST ## _u64, INST ## Logic, NARY)       \
  MakeAtomicTest(AtomicNoRet ## INST ## _s32, INST ## Logic, NARY)  \
  MakeAtomicTest(AtomicNoRet ## INST ## _s64, INST ## Logic, NARY)  \
  MakeAtomicTest(AtomicNoRet ## INST ## _u32, INST ## Logic, NARY)  \
  MakeAtomicTest(AtomicNoRet ## INST ## _u64, INST ## Logic, NARY)

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

#define TestTernarySignedVectorInst(INST)               \
  MakeVectorTest(INST ## _s8x4,  INST ## VectorLogic)   \
  MakeVectorTest(INST ## _s16x2,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _s16x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _s32x2,  INST ## VectorLogic)

#define TestBinaryUnsignedVectorInst(INST)                \
  MakeVectorTest(INST ## _PP_u8x4,  INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_u16x2, INST ## VectorLogic)  \
  MakeVectorTest(INST ## _PP_u32x2, INST ## VectorLogic)

#define TestTernaryUnsignedVectorInst(INST)           \
  MakeVectorTest(INST ## _u8x4,  INST ## VectorLogic) \
  MakeVectorTest(INST ## _u16x2, INST ## VectorLogic) \
  MakeVectorTest(INST ## _u32x2, INST ## VectorLogic)

#define TestBinaryFloatVectorInst(INST)                   \
  /* MakeVectorTest(INST ## _PP_f16x2, INST ## Logic) */  \
  MakeVectorTest(INST ## _PP_f32x2, INST ## VectorLogic)

#define TestTernaryFloatVectorInst(INST)                  \
  /* MakeVectorTest(INST ## _PP_f16x2, INST ## Logic) */  \
  MakeVectorTest(INST ## _f32x2, INST ## VectorLogic)

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

#define MakePackedCmpTest(FUNC,TYPE)                                    \
  MakeVectorTest(PackedCmp_ ## FUNC ## _ ## TYPE, PackedCmp_ ## FUNC ## Logic)

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
  template<class R, class T>                                \
  static void PackedCmp_ ## FUNC ## Logic(R result, T a, T b) {    \
    ForEach(Cmp_ ## FUNC ## Logic, result, a, b);                  \
  }                                                                \
  template<class R, class T>                                       \
  static void PackedCmp_ ## FUNC ## uLogic(R result, T a, T b) {   \
    ForEach(Cmp_ ## FUNC ## uLogic, result, a, b);                 \
  }                                                                \
  Cmp(declare, FUNC, b32)                                   \
  Cmp(declare, FUNC, b64)                                   \
  Cmp(declare, FUNC, s32)                                   \
  Cmp(declare, FUNC, s64)                                   \
  Cmp(declare, FUNC, u32)                                   \
  Cmp(declare, FUNC, u64)                                   \
  /* Cmp(declare, Cmp_ ## FUNC, f16) */                     \
  FCmp(declare, FUNC, f32)                                  \
  FCmp(declare, FUNC, f64)                                  \
  PackedCmp(declare, FUNC, u8x4)                            \
  PackedCmp(declare, FUNC, u8x8)                            \
  PackedCmp(declare, FUNC, u8x16)                           \
  PackedCmp(declare, FUNC, u16x2)                           \
  PackedCmp(declare, FUNC, u16x4)                           \
  PackedCmp(declare, FUNC, u16x8)                           \
  PackedCmp(declare, FUNC, u32x2)                           \
  PackedCmp(declare, FUNC, u32x4)                           \
  PackedCmp(declare, FUNC, u64x2)                           \
  PackedCmp(declare, FUNC, s8x4)                            \
  PackedCmp(declare, FUNC, s8x8)                            \
  PackedCmp(declare, FUNC, s8x16)                           \
  PackedCmp(declare, FUNC, s16x2)                           \
  PackedCmp(declare, FUNC, s16x4)                           \
  PackedCmp(declare, FUNC, s16x8)                           \
  PackedCmp(declare, FUNC, s32x2)                           \
  PackedCmp(declare, FUNC, s32x4)                           \
  PackedCmp(declare, FUNC, s64x2)                           \
  /*FPackedCmp(declare, FUNC, f16x2) */                     \
  /*FPackedCmp(declare, FUNC, f16x4) */                     \
  /*FPackedCmp(declare, FUNC, f16x8) */                     \
  FPackedCmp(declare, FUNC, f32x2)                          \
  FPackedCmp(declare, FUNC, f32x4)                          \
  FPackedCmp(declare, FUNC, f64x2)                          \
  MakeCmpTest(FUNC, b32)                                    \
  MakeCmpTest(FUNC, b64)                                    \
  MakeCmpTest(FUNC, s32)                                    \
  MakeCmpTest(FUNC, s64)                                    \
  MakeCmpTest(FUNC, u32)                                    \
  MakeCmpTest(FUNC, u64)                                    \
  /* MakeCmpTest(FUNC, f16) */                              \
  MakeCmpTest(FUNC, f32)                                    \
  MakeCmpTest(FUNC ## u, f32)                               \
  MakeCmpTest(FUNC, f64)                                    \
  MakeCmpTest(FUNC ## u, f64)                               \
  MakePackedCmpTest(FUNC, u8x4)                             \
  MakePackedCmpTest(FUNC, u8x8)                             \
  MakePackedCmpTest(FUNC, u8x16)                            \
  MakePackedCmpTest(FUNC, u16x2)                            \
  MakePackedCmpTest(FUNC, u16x4)                            \
  MakePackedCmpTest(FUNC, u16x8)                            \
  MakePackedCmpTest(FUNC, u32x2)                            \
  MakePackedCmpTest(FUNC, u32x4)                            \
  MakePackedCmpTest(FUNC, u64x2)                            \
  MakePackedCmpTest(FUNC, s8x4)                             \
  MakePackedCmpTest(FUNC, s8x8)                             \
  MakePackedCmpTest(FUNC, s8x16)                            \
  MakePackedCmpTest(FUNC, s16x2)                            \
  MakePackedCmpTest(FUNC, s16x4)                            \
  MakePackedCmpTest(FUNC, s16x8)                            \
  MakePackedCmpTest(FUNC, s32x2)                            \
  MakePackedCmpTest(FUNC, s32x4)                            \
  MakePackedCmpTest(FUNC, s64x2)                            \
  /* MakePackedCmpTest(FUNC, f16x2) */                      \
  /* MakePackedCmpTest(FUNC, f16x4) */                      \
  /* MakePackedCmpTest(FUNC, f16x2) */                      \
  MakePackedCmpTest(FUNC, f32x2)                            \
  MakePackedCmpTest(FUNC, f32x4)                            \
  MakePackedCmpTest(FUNC, f64x2)                            \
  /* MakePackedCmpTest(FUNC ## u, f16x2) */                 \
  /* MakePackedCmpTest(FUNC ## u, f16x4) */                 \
  /* MakePackedCmpTest(FUNC ## u, f16x2) */                 \
  MakePackedCmpTest(FUNC ## u, f32x2)                       \
  MakePackedCmpTest(FUNC ## u, f32x4)                       \
  MakePackedCmpTest(FUNC ## u, f64x2)


#define MakeCvtF2FsTest(TYPE)                   \
  MakeCvtF2FsTestFun(TYPE, Cvt, TYPE)           \
  MakeCvtF2FsTestFun(TYPE, Cvt_upi, ceil)       \
  MakeCvtF2FsTestFun(TYPE, Cvt_downi, floor)    \
  MakeCvtF2FsTestFun(TYPE, Cvt_zeroi, trunc)    \
  MakeCvtF2FsTestFun(TYPE, Cvt_neari, nearbyint)

#define MakeCvtF2FsTestFun(TYPE,NAME,FUNC)                              \
  static void NAME ## _ ## TYPE ## _ ## TYPE ## _Logic(TYPE result,     \
                                                       TYPE a) {        \
    if(isNan(a)) {                                                      \
      EXPECT_PRED1(isNan<TYPE>, a);                                     \
    } else {                                                            \
      EXPECT_EQ(FUNC(a), result);                                       \
    }                                                                   \
  }                                                                     \
  extern "C" TYPE NAME ## _ ## TYPE ## _ ## TYPE (TYPE);                \
  MakeTest(NAME ## _ ## TYPE ## _ ## TYPE,                              \
           NAME ## _ ## TYPE ## _ ## TYPE ## _Logic)

#define MakeCvtF2FdTest(NAME,PRED)                                    \
  static void Cvt_ ## NAME ## _f32_f64_Logic(f32 result, f64 a) {     \
    if(isNan(a)) {                                                    \
      EXPECT_PRED1(isNan<f32>, result);                               \
    } else if(isPosInf(a)) {                                          \
      EXPECT_PRED1(isPosInf<f32>, result);                            \
    } else if(isNegInf(a)) {                                          \
      EXPECT_PRED1(isNegInf<f32>, result);                            \
    } else {                                                          \
      EXPECT_TRUE(PRED);                                              \
    }                                                                 \
  }                                                                   \
  extern "C" f32 Cvt_ ## NAME ## _f32_f64(f64);                       \
  MakeTest(Cvt_ ## NAME ## _f32_f64, Cvt_ ## NAME ## _f32_f64_Logic)

#define MakeCvtX2ITestRet(TYPE,RET,NAME)              \
  extern "C" RET NAME ## RET ## _ ## TYPE (TYPE);     \
  MakeTest(NAME ## RET ## _ ## TYPE,  NAME ## Logic)

#define MakeCvtX2ITest(TYPE,NAME)               \
  MakeCvtX2ITestRet(TYPE, u8,  NAME)            \
  MakeCvtX2ITestRet(TYPE, s8,  NAME)            \
  MakeCvtX2ITestRet(TYPE, u16, NAME)            \
  MakeCvtX2ITestRet(TYPE, s16, NAME)            \
  MakeCvtX2ITestRet(TYPE, u32, NAME)            \
  MakeCvtX2ITestRet(TYPE, s32, NAME)            \
  MakeCvtX2ITestRet(TYPE, u64, NAME)            \
  MakeCvtX2ITestRet(TYPE, s64, NAME)

#define MakeCvtF2ITest(TYPE)                 \
  MakeCvtX2ITest(TYPE, Cvt_)                 \
  MakeCvtX2ITest(TYPE, Cvt_upi_)             \
  MakeCvtX2ITest(TYPE, Cvt_downi_)           \
  MakeCvtX2ITest(TYPE, Cvt_zeroi_)           \
  MakeCvtX2ITest(TYPE, Cvt_neari_)

#define MakeCvtI2FTestRet(TYPE,RET,NAME)              \
  extern "C" RET NAME ## RET ## _ ## TYPE (TYPE);     \
  MakeTest(NAME ## RET ## _ ## TYPE,  NAME ## Logic)

#define MakeCvtI2FTestType(TYPE,NAME)           \
  /* MakeCvtI2FTestRet(TYPE, f16, NAME) */      \
  MakeCvtI2FTestRet(TYPE, f32, NAME)            \
  MakeCvtI2FTestRet(TYPE, f64, NAME)            \

#define MakeCvtI2FTest(NAME)                    \
  MakeCvtI2FTestType(u8, NAME)                  \
  MakeCvtI2FTestType(s8, NAME)                  \
  MakeCvtI2FTestType(u16, NAME)                 \
  MakeCvtI2FTestType(s16, NAME)                 \
  MakeCvtI2FTestType(u32, NAME)                 \
  MakeCvtI2FTestType(s32, NAME)                 \
  MakeCvtI2FTestType(u64, NAME)                 \
  MakeCvtI2FTestType(s64, NAME)

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

  union { b64 b; f64 f; } SNAN = { 0x7FF8000000000000ULL };
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
  typedef typename T::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = testVector[i];
    Logic(Impl(a), a);
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, T), void (*Logic)(R, T, T)) {
  typedef typename T::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = testVector[i];
    for(unsigned j = 0; j < testVector.size(); ++j) {
      T b = testVector[j];
      Logic(Impl(a, b), a, b);
    }
  }
}

template<class T, class R>
  static void TestVectorInst(R (*Impl)(T, T, T), void (*Logic)(R, T, T, T)) {
  typedef typename T::Base Base;
  const std::vector<Base> &testVector = getTestVector<Base>();
  for(unsigned i = 0; i < testVector.size(); ++i) {
    T a = testVector[i];
    for(unsigned j = 0; j < testVector.size(); ++j) {
      T b = testVector[j];
      for(unsigned k = 0; k < testVector.size(); ++k) {
        T c = testVector[k];
        Logic(Impl(a, b, c), a, b, c);
      }
    }
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, unsigned),
                           void (*Logic)(R, T, unsigned)) {
  typedef typename T::Base Base;
  const std::vector<Base> &testVectorB = getTestVector<Base>();
  const std::vector<unsigned> &testVectorU = getTestVector<unsigned>();
  for(unsigned i = 0; i < testVectorB.size(); ++i) {
    T a = testVectorB[i];
    for(unsigned j = 0; j < testVectorU.size(); ++j) {
      unsigned b = testVectorU[j];
      Logic(Impl(a, b), a, b);
    }
  }
}

template<class T, class R>
static void TestVectorInst(R (*Impl)(T, T, unsigned),
                           void (*Logic)(R, T, T, unsigned)) {
  typedef typename T::Base Base;
  const std::vector<Base> &testVectorB = getTestVector<Base>();
  const std::vector<unsigned> &testVectorU = getTestVector<unsigned>();
  for(unsigned i = 0; i < testVectorB.size(); ++i) {
    T a = testVectorB[i];
    for(unsigned j = 0; j < testVectorB.size(); ++j) {
      T b = testVectorB[j];
      for(unsigned k = 0; k < testVectorB.size(); ++k) {
        unsigned c = testVectorU[k];
        Logic(Impl(a, b, c), a, b, c);
      }
    }
  }
}

template<class R, class A, class B>
static void TestAtomicBinary(R (*Impl)(A*, B), void (*Logic)(R, A, B)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    const A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      A tempA = a;
      B b = getTestVector<B>()[j];
      Impl(&tempA, b);
      Logic(tempA, a, b);
    }
  }
}

template<class R, class A, class B, class C>
static void TestAtomicTernary(R (*Impl)(A*, B, C),
                              void (*Logic)(R, A, B, C)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    const A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      B b = getTestVector<B>()[j];
      for(unsigned k = 0; k < getTestVector<C>().size(); ++k) {
        A tempA = a;
        C c = getTestVector<C>()[k];
        Impl(&tempA, b, c);
        Logic(tempA, a, b, c);
      }
    }
  }
}

template<class A, class B>
static void TestAtomicBinary(void (*Impl)(A*, B),
                             void (*Logic)(A, A, B)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    const A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      A tempA = a;
      B b = getTestVector<B>()[j];
      Impl(&tempA, b);
      Logic(tempA, a, b);
    }
  }
}

template<class A, class B, class C>
static void TestAtomicTernary(void (*Impl)(A*, B, C),
                              void (*Logic)(A, A, B, C)) {
  for(unsigned i = 0; i < getTestVector<A>().size(); ++i) {
    const A a = getTestVector<A>()[i];
    for(unsigned j = 0; j < getTestVector<B>().size(); ++j) {
      B b = getTestVector<B>()[j];
      for(unsigned k = 0; k < getTestVector<C>().size(); ++k) {
        A tempA = a;
        C c = getTestVector<C>()[k];
        Impl(&tempA, b, c);
        Logic(tempA, a, b, c);
      }
    }
  }
}

template<class T> static bool Not(bool (*Fn)(T), T t) { return !Fn(t); }


template<class T> bool isBool()       { return false; }
template<>        bool isBool<bool>() { return true;  }

#endif // BRIG_RUNTIME_TEST_INTERNAL_H
