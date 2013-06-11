//===- brig_runtime_internal.h --------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BRIG_RUNTIME_INTERNAL_H
#define BRIG_RUNTIME_INTERNAL_H

#include "brig_runtime.h"
#include <cmath>
#include <fenv.h>

namespace hsa {
namespace brig {

template<class T>
inline T map(typename T::UMapFn MapFn, T t) {
  for(unsigned i = 0; i < T::Len; ++i) {
    t[i] = MapFn(t[i]);
  }
  return t;
}

template<class T>
inline T map(typename T::BMapFn MapFn, T x, T y) {
  for(unsigned i = 0; i < T::Len; ++i) {
    x[i] = MapFn(x[i], y[i]);
  }
  return x;
}

template<class T>
inline T map(typename T::TMapFn MapFn, T x, T y, T z) {
  for(unsigned i = 0; i < T::Len; ++i) {
    x[i] = MapFn(x[i], y[i], z[i]);
  }
  return x;
}

template<class T>
inline T map(typename T::SMapFn MapFn, T x, typename T::Base y) {
  for(unsigned i = 0; i < T::Len; ++i) {
    x[i] = MapFn(x[i], y);
  }
  return x;
}

template<class T>
inline void ForEach(typename T::UForEachFn MapFn, T t) {
  for(unsigned i = 0; i < T::Len; ++i) {
    MapFn(t[i]);
  }
}

template<class T>
inline void ForEach(typename T::BForEachFn MapFn, T x, T y) {
  for(unsigned i = 0; i < T::Len; ++i) {
    MapFn(x[i], y[i]);
  }
}

template<class T>
inline void ForEach(typename T::TForEachFn MapFn, T x, T y, T z) {
  for(unsigned i = 0; i < T::Len; ++i) {
    MapFn(x[i], y[i], z[i]);
  }
}

template<class T>
inline void ForEach(typename T::QForEachFn MapFn, T x, T y, T z, T w) {
  for(unsigned i = 0; i < T::Len; ++i) {
    MapFn(x[i], y[i], z[i], w[i]);
  }
}

template<class T>
inline void ForEach(typename T::SForEachFn MapFn, T x, T y, unsigned z) {
  for(unsigned i = 0; i < T::Len; ++i) {
    MapFn(x[i], y[i], z);
  }
}

#define BitInst(D,INST,NARY)                    \
  D ## NARY(INST, b1)                           \
  D ## NARY(INST, b32)                          \
  D ## NARY(INST, b64)

#define SignedInst(D,INST,NARY)                 \
  D ## NARY(INST, s32)                          \
  D ## NARY(INST, s64)

#define UnsignedInst(D,INST,NARY)               \
  D ## NARY(INST, u32)                          \
  D ## NARY(INST, u64)

#define FloatInst(D,INST,NARY)                  \
  /* D ## NARY(INST, f16) */                    \
  D ## NARY(INST, f32)                          \
  D ## NARY(INST, f64)

#define SignedVectorInst(D,INST,NARY)           \
  NARY ## Vector(D, INST, s8x4)                 \
  NARY ## Vector(D, INST, s8x8)                 \
  NARY ## Vector(D, INST, s16x2)                \
  NARY ## Vector(D, INST, s16x4)                \
  NARY ## Vector(D, INST, s32x2)

#define UnsignedVectorInst(D,INST,NARY)         \
  NARY ## Vector(D, INST, u8x4)                 \
  NARY ## Vector(D, INST, u8x8)                 \
  NARY ## Vector(D, INST, u16x2)                \
  NARY ## Vector(D, INST, u16x4)                \
  NARY ## Vector(D, INST, u32x2)

#define ShiftInst(D,INST,NARY)                  \
  D ## Shift(INST, s32)                         \
  D ## Shift(INST, u32)                         \
  D ## Shift(INST, s64)                         \
  D ## Shift(INST, u64)                         \
  D ## ShiftVector(INST, s8x4)                  \
  D ## ShiftVector(INST, s8x8)                  \
  D ## ShiftVector(INST, s16x2)                 \
  D ## ShiftVector(INST, s16x4)                 \
  D ## ShiftVector(INST, s32x2)                 \
  D ## ShiftVector(INST, u8x4)                  \
  D ## ShiftVector(INST, u8x8)                  \
  D ## ShiftVector(INST, u16x2)                 \
  D ## ShiftVector(INST, u16x4)                 \
  D ## ShiftVector(INST, u32x2)

#define UnpackInst(D,INST,NARY)                 \
  D ## NARY(INST, s8x4)                         \
  D ## NARY(INST, s8x8)                         \
  D ## NARY(INST, s16x2)                        \
  D ## NARY(INST, s16x4)                        \
  D ## NARY(INST, s32x2)                        \
  D ## NARY(INST, u8x4)                         \
  D ## NARY(INST, u8x8)                         \
  D ## NARY(INST, u16x2)                        \
  D ## NARY(INST, u16x4)                        \
  D ## NARY(INST, u32x2)                        \
  /* D ## NARY(INST, f16x2) */                  \
  /* D ## NARY(INST, f16x4) */                  \
  D ## NARY(INST, f32x2)

#define FloatVectorInst(D,INST,NARY)            \
  /* NARY ## Vector(D, INST, f16x2) */          \
  /* NARY ## Vector(D, INST, f16x4) */          \
  NARY ## Vector(D, INST, f32x2)

#define UnaryVector(D,FUNC,TYPE)                \
  D ## UnaryVectorPacking(FUNC,TYPE,P)          \
  D ## UnaryVectorPacking(FUNC,TYPE,S)

#define BinaryVector(D,FUNC,TYPE)               \
  D ## BinaryVectorPacking(FUNC, TYPE, P, P)    \
  D ## BinaryVectorPacking(FUNC, TYPE, P, S)    \
  D ## BinaryVectorPacking(FUNC, TYPE, S, P)    \
  D ## BinaryVectorPacking(FUNC, TYPE, S, S)

#define TernaryVector(D,FUNC,TYPE)              \
  D ## TernaryVectorPacking(FUNC, TYPE)

#define ShuffleVectorInst(D,INST,NARY)          \
  D ## ShuffleVector(INST, s8x4)                \
  D ## ShuffleVector(INST, s8x8)                \
  D ## ShuffleVector(INST, s16x2)               \
  D ## ShuffleVector(INST, s16x4)               \
  D ## ShuffleVector(INST, s32x2)               \
  D ## ShuffleVector(INST, u8x4)                \
  D ## ShuffleVector(INST, u8x8)                \
  D ## ShuffleVector(INST, u16x2)               \
  D ## ShuffleVector(INST, u16x4)               \
  D ## ShuffleVector(INST, u32x2)               \
  /* D ## ShuffleVector(INST, f16x2) */         \
  /* D ## ShuffleVector(INST, f16x4) */         \
  D ## ShuffleVector(INST, f32x2)

#define AtomicInst(D,INST,NARY)                 \
  D ## Atomic ## NARY(INST, b32)                \
  D ## Atomic ## NARY(INST, b64)                \
  D ## Atomic ## NARY(INST, s32)                \
  D ## Atomic ## NARY(INST, s64)                \
  D ## Atomic ## NARY(INST, u32)                \
  D ## Atomic ## NARY(INST, u64)

#define CmpImpl(FUNC,PRED)                                    \
  template<class T> static T Cmp_ ## FUNC (T x, T y) {        \
    return cmpResult<T>(PRED);                                \
  }                                                           \
  template<class T> static T PackedCmp_ ## FUNC (T x, T y) {  \
    return map(Cmp_ ## FUNC, x, y);                           \
  }

#define CmpInst(FUNC,PRED)                              \
  CmpImpl(FUNC, (PRED))                                 \
  CmpImpl(s ## FUNC, (PRED))                            \
  CmpImpl(FUNC ## u, isUnordered(x, y) || (PRED))       \
  CmpImpl(s ## FUNC ## u, isUnordered(x, y) || (PRED))  \
  Cmp(define, FUNC, b32)                                \
  Cmp(define, FUNC, b64)                                \
  Cmp(define, FUNC, s32)                                \
  Cmp(define, FUNC, s64)                                \
  Cmp(define, FUNC, u32)                                \
  Cmp(define, FUNC, u64)                                \
  /* Cmp(define, Cmp_ ## FUNC, f16) */                  \
  FCmp(define, FUNC, f32)                               \
  FCmp(define, FUNC, f64)                               \
  PackedCmp(define, FUNC, u8x4)                         \
  PackedCmp(define, FUNC, u8x8)                         \
  PackedCmp(define, FUNC, u8x16)                        \
  PackedCmp(define, FUNC, u16x2)                        \
  PackedCmp(define, FUNC, u16x4)                        \
  PackedCmp(define, FUNC, u16x8)                        \
  PackedCmp(define, FUNC, u32x2)                        \
  PackedCmp(define, FUNC, u32x4)                        \
  PackedCmp(define, FUNC, u64x2)                        \
  PackedCmp(define, FUNC, s8x4)                         \
  PackedCmp(define, FUNC, s8x8)                         \
  PackedCmp(define, FUNC, s8x16)                        \
  PackedCmp(define, FUNC, s16x2)                        \
  PackedCmp(define, FUNC, s16x4)                        \
  PackedCmp(define, FUNC, s16x8)                        \
  PackedCmp(define, FUNC, s32x2)                        \
  PackedCmp(define, FUNC, s32x4)                        \
  PackedCmp(define, FUNC, s64x2)                        \
  /*FPackedCmp(define, FUNC, f16x2) */                  \
  /*FPackedCmp(define, FUNC, f16x4) */                  \
  /*FPackedCmp(define, FUNC, f16x8) */                  \
  FPackedCmp(define, FUNC, f32x2)                       \
  FPackedCmp(define, FUNC, f32x4)                       \
  FPackedCmp(define, FUNC, f64x2)

#define FCmp(D,FUNC,TYPE)                       \
  Cmp(D, FUNC, TYPE)                            \
  Cmp(D, s ## FUNC, TYPE)                       \
  Cmp(D, FUNC ## u, TYPE)                       \
  Cmp(D, s ## FUNC ## u, TYPE)

#define Cmp(D,FUNC,TYPE)                        \
  D ## CmpRet(Cmp_ ## FUNC, b1, TYPE)           \
  D ## CmpRet(Cmp_ ## FUNC, b32, TYPE)          \
  D ## CmpRet(Cmp_ ## FUNC, s32, TYPE)          \
  D ## CmpRet(Cmp_ ## FUNC, u32, TYPE)          \
  /* D ## CmpRet(Cmp_ ## FUNC, f16, TYPE) */    \
  D ## CmpRet(Cmp_ ## FUNC, f32, TYPE)

#define FPackedCmp(D,FUNC,TYPE)                          \
  PackedCmp(D, FUNC, TYPE)                               \
  PackedCmp(D, s ## FUNC, TYPE)                          \
  PackedCmp(D, FUNC ## u, TYPE)                          \
  PackedCmp(D, s ## FUNC ## u, TYPE)

#define PackedCmp(D,FUNC,TYPE)                           \
  D ## PackedCmp(PackedCmp_ ## FUNC, TYPE)

#define definePackedCmp(FUNC,TYPE)                       \
  extern "C" TYPE FUNC ## _ ## TYPE(TYPE t, TYPE u) {    \
    return FUNC(t, u);                                   \
  }

#define declarePackedCmp(FUNC,TYPE)                      \
  extern "C" TYPE FUNC ## _ ## TYPE(TYPE t, TYPE u);

#define defineCmpRet(FUNC,RET,TYPE)                                 \
  extern "C" bool FUNC ## _ ## RET ## _ ## TYPE (TYPE t, TYPE u) {  \
    return FUNC(t, u);                                              \
  }

#define declareCmpRet(FUNC,RET,TYPE)                              \
  extern "C" bool FUNC ## _ ## RET ## _ ## TYPE (TYPE t, TYPE u);

#define RIICvt(D)                               \
  IBCvt(I, D, Cvt, ~0)

#define RFICvt(D)                               \
  FICvt(D, Cvt,       ~0)                       \
  FICvt(D, Cvt_upi,   FE_UPWARD)                \
  FICvt(D, Cvt_downi, FE_DOWNWARD)              \
  FICvt(D, Cvt_zeroi, FE_TOWARDZERO)            \
  FICvt(D, Cvt_neari, FE_TONEAREST)

#define RIFCvt(D)                               \
  IBCvt(F, D, Cvt,      ~0)                     \
  IBCvt(F, D, Cvt_up,   FE_UPWARD)              \
  IBCvt(F, D, Cvt_down, FE_DOWNWARD)            \
  IBCvt(F, D, Cvt_zero, FE_TOWARDZERO)          \
  IBCvt(F, D, Cvt_near, FE_TONEAREST)

#define IBCvt(B,D,FUNC,ROUND)                   \
  B ## Cvt(D, FUNC, ROUND, b1)                  \
  B ## Cvt(D, FUNC, ROUND, u8)                  \
  B ## Cvt(D, FUNC, ROUND, s8)                  \
  B ## Cvt(D, FUNC, ROUND, u16)                 \
  B ## Cvt(D, FUNC, ROUND, s16)                 \
  B ## Cvt(D, FUNC, ROUND, u32)                 \
  B ## Cvt(D, FUNC, ROUND, s32)                 \
  B ## Cvt(D, FUNC, ROUND, u64)                 \
  B ## Cvt(D, FUNC, ROUND, s64)

#define FICvt(D,FUNC,ROUND)                     \
  /* ICvt(D, FUNC, ROUND, f16) */               \
  ICvt(D, FUNC, ROUND, f32)                     \
  ICvt(D, FUNC, ROUND, f64)

#define ICvt(D,FUNC,ROUND,TYPE)                 \
  D ## Cvt(FUNC, ROUND, u8,  TYPE)              \
  D ## Cvt(FUNC, ROUND, s8,  TYPE)              \
  D ## Cvt(FUNC, ROUND, u16, TYPE)              \
  D ## Cvt(FUNC, ROUND, s16, TYPE)              \
  D ## Cvt(FUNC, ROUND, u32, TYPE)              \
  D ## Cvt(FUNC, ROUND, s32, TYPE)              \
  D ## Cvt(FUNC, ROUND, u64, TYPE)              \
  D ## Cvt(FUNC, ROUND, s64, TYPE)

#define FCvt(D,FUNC,ROUND,TYPE)                 \
  /* D ## Cvt(FUNC, ROUND, f16, TYPE) */        \
  D ## Cvt(FUNC, ROUND, f32, TYPE)              \
  D ## Cvt(FUNC, ROUND, f64, TYPE)

#define defineCvt(FUNC,ROUND,RET,TYPE)                    \
  extern "C" RET FUNC ## _ ## RET ## _ ## TYPE (TYPE t) { \
    return Cvt<RET>(t, ROUND);                      \
  }

#define defineUnary(FUNC,TYPE)                  \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t) {  \
    return FUNC(t);                             \
  }

#define defineBinary(FUNC,TYPE)                         \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u) {  \
    return FUNC(t, u);                                  \
  }

#define defineTernary(FUNC,TYPE)                                \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, TYPE v) {  \
    return FUNC(t, u, v);                                       \
  }

#define defineQuaternary(FUNC,TYPE)                                     \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, b32 v, b32 w) {    \
    return FUNC(t, u, v, w);                                            \
  }

#define defineShift(FUNC,TYPE)                            \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, b32 shift) { \
    return FUNC(t, shift);                                \
  }

#define defineUnaryVectorPacking(FUNC,TYPE,PACKING)             \
  extern "C" TYPE FUNC ## _ ## PACKING ## _ ## TYPE (TYPE t) {  \
    return FUNC ## Vector(t.PACKING());                         \
  }

#define defineBinaryVectorPacking(FUNC,TYPE,P1,P2)                      \
  extern "C" TYPE FUNC ## _ ## P1 ## P2 ## _ ## TYPE (TYPE t, TYPE u) { \
    return FUNC ## Vector(t.P1(), u.P2());                              \
  }

#define defineTernaryVectorPacking(FUNC,TYPE)                   \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, TYPE v) {  \
    return FUNC ## Vector(t, u, v);                             \
  }

#define defineShiftVector(FUNC,TYPE)                      \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, b32 shift) { \
    return FUNC ## Vector(t, shift);                      \
  }

#define defineShuffleVector(FUNC,TYPE)                            \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, b32 shift) { \
    return FUNC ## Vector(t, u, shift);                           \
  }

#define defineAtomicUnary(FUNC,TYPE)                            \
  extern "C" TYPE Atomic ## FUNC ## _ ## TYPE (TYPE *t) {       \
    return Atomic ## FUNC(t);                                   \
  }                                                             \
  extern "C" void AtomicNoRet ## FUNC ## _ ## TYPE (TYPE *t) {  \
    Atomic ## FUNC(t);                                          \
  }

#define defineAtomicBinary(FUNC,TYPE)                                   \
  extern "C" TYPE Atomic ## FUNC ## _ ## TYPE (TYPE *t, TYPE u) {       \
    return Atomic ## FUNC(t, u);                                        \
  }                                                                     \
  extern "C" void AtomicNoRet ## FUNC ## _ ## TYPE (TYPE *t, TYPE u) {  \
    Atomic ## FUNC(t, u);                                               \
  }

#define defineAtomicTernary(FUNC,TYPE)                                  \
  extern "C"                                                            \
  TYPE Atomic ## FUNC ## _ ## TYPE (TYPE *t, TYPE u, TYPE v) {          \
    return Atomic ## FUNC(t, u, v);                                     \
  }                                                                     \
  extern "C"                                                            \
  void AtomicNoRet ## FUNC ## _ ## TYPE (TYPE *t, TYPE u, TYPE v) {     \
    Atomic ## FUNC(t, u, v);                                            \
  }


#define declareUnary(FUNC,TYPE)                 \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t);

#define declareBinary(FUNC,TYPE)                      \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u);

#define declareTernary(FUNC,TYPE)                             \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, TYPE v);

#define declareQuaternary(FUNC,TYPE)                                \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, b32 v, b32 w);

#define declareShift(FUNC,TYPE)                               \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, unsigned shift);

#define declareUnaryVectorPacking(FUNC,TYPE,PACKING)          \
  extern "C" TYPE FUNC ## _ ## PACKING ## _ ## TYPE (TYPE t);

#define declareBinaryVectorPacking(FUNC,TYPE,P1,P2)                     \
  extern "C" TYPE FUNC ## _ ## P1 ## P2 ## _ ## TYPE (TYPE t, TYPE u);

#define declareTernaryVectorPacking(FUNC,TYPE)                \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, TYPE v);

#define declareShiftVector(FUNC,TYPE)                         \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, unsigned shift);

#define declareShuffleVector(FUNC,TYPE)                               \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u, unsigned shift);

#define declareAtomicBinary(FUNC,TYPE)                                  \
  extern "C" TYPE Atomic ## FUNC ## _ ## TYPE (TYPE *t, TYPE u);        \
  extern "C" void AtomicNoRet ## FUNC ## _ ## TYPE (TYPE *t, TYPE u);

#define declareAtomicTernary(FUNC,TYPE)                                 \
  extern "C"                                                            \
  TYPE Atomic ## FUNC ## _ ## TYPE (TYPE *t, TYPE u, TYPE v);           \
  extern "C"                                                            \
  void AtomicNoRet ## FUNC ## _ ## TYPE (TYPE *t, TYPE u, TYPE v);

template <bool S> struct IntTypes;
template<> struct IntTypes<true> {
  typedef s64 Int64Ty;
  typedef s32 Int32Ty;
  typedef s16 Int16Ty;
  typedef s8  Int8Ty;
};

template<> struct IntTypes<false> {
  typedef u64 Int64Ty;
  typedef u32 Int32Ty;
  typedef u16 Int16Ty;
  typedef u8  Int8Ty;
};

template<class T, class U, bool S> struct IntPolicy : public IntTypes<S> {
  typedef U Unsigned;
  enum { isSigned = S };
  enum {
    Bits = 8 * sizeof(T),
    ShiftMask = Bits - 1,
    HighBit = T(T(1) << (Bits - 1)),
    Min = S ? HighBit : T(0),
    Max = T(~Min)
  };
  static bool isNeg(T t) { return isSigned && (t & HighBit); }
  static bool isNegOne(T t) { return isSigned && t == T(~0); }
};

template<class T> struct Int;

template<> struct Int<u8>  : public IntPolicy<u8,  u8,  false> {};
template<> struct Int<u16> : public IntPolicy<u16, u16, false> {};
template<> struct Int<u32> : public IntPolicy<u32, u32, false> {};
template<> struct Int<u64> : public IntPolicy<u64, u64, false> {};
template<> struct Int<s8>  : public IntPolicy<s8,  u8,  true>  {};
template<> struct Int<s16> : public IntPolicy<s16, u16, true>  {};
template<> struct Int<s32> : public IntPolicy<s32, u32, true>  {};
template<> struct Int<s64> : public IntPolicy<s64, u64, true>  {};

template<class T> inline bool isNan(T t) { return false; }
template<> inline bool isNan(float f) { return std::isnan(f); }
template<> inline bool isNan(double d) { return std::isnan(d); }

template<class T> inline bool isSNan(T t) { return false; }
template<> inline bool isSNan(f32 f) {
  if(!isNan(f)) return false;
  union { f32 f; b32 b; } Conv = { f };
  b32 mask = (1U << 22);
  return Conv.b & mask;
}
template<> inline bool isSNan(f64 f) {
  if(!isNan(f)) return false;
  union { f64 f; b64 b; } Conv = { f };
  b64 mask = (1ULL << 51);
  return Conv.b & mask;
}

template<class T> inline bool isQNan(T t) { return isNan(t) && !isSNan(t); }

template<class T> inline bool isDivisionError(T x, T y) {
  if(y == 0) return true;
  if(!Int<T>::isSigned) return false;
  if(Int<T>::isNegOne(x) && y == Int<T>::Min) return true;
  if(x == Int<T>::Min && Int<T>::isNegOne(y)) return true;
  return false;
}
template<> inline bool isDivisionError(float, float) { return false; }
template<> inline bool isDivisionError(double, double) { return false; }

template<class T> inline T Int48Ty(T t) {
  struct { T x:48; } Ext;
  return Ext.x = t;
}

template<class T> inline T Int24Ty(T t) {
  struct { T x:24; } Ext;
  return Ext.x = t;
}

template<class T> inline T Int12Ty(T t) {
  struct { T x:12; } Ext;
  return Ext.x = t;
}

template<class T> inline bool isNegZero(T t) { return false; }
template<> inline bool isNegZero(float  f) {
  return f == 0.0 && copysignf(1.0, f) < 0.0;
}
template<> inline bool isNegZero(double d) {
  return d == 0.0 && copysign(1.0, d) < 0.0;
}

template<class T> inline bool isPosZero(T t) { return false; }
template<> inline bool isPosZero(f32 f) { return f == 0.0 && !isNegZero(f); }
template<> inline bool isPosZero(f64 f) { return f == 0.0 && !isNegZero(f); }

template<class T> inline bool isInf(T t) { return false; }
template<> inline bool isInf(float f)  { return std::isinf(f); }
template<> inline bool isInf(double d) { return std::isinf(d); }

template<class T> inline bool isPosInf(T t) { return false; }
template<> inline bool isPosInf(float f)  { return std::isinf(f) && f > 0.0; }
template<> inline bool isPosInf(double d) { return std::isinf(d) && d > 0.0; }

template<class T> inline bool isNegInf(T t) { return false; }
template<> inline bool isNegInf(float f)  { return std::isinf(f) && f < 0.0; }
template<> inline bool isNegInf(double d) { return std::isinf(d) && d < 0.0; }

template<class T> inline T getMax() { return Int<T>::Max; }
template<> inline b1  getMax() { return true; }
template<> inline f32 getMax() { return INFINITY; }
template<> inline f64 getMax() { return INFINITY; }

template<class T> inline T getMin() { return Int<T>::Min; }
template<> inline b1  getMin() { return false; }
template<> inline f32 getMin() { return -INFINITY; }
template<> inline f64 getMin() { return -INFINITY; }

enum BrigFPClass {
  SNan       = 0x001,
  QNan       = 0x002,
  NegInf     = 0x004,
  NegNorm    = 0x008,
  NegSubnorm = 0x010,
  NegZero    = 0x020,
  PosZero    = 0x040,
  PosSubnorm = 0x080,
  PosNorm    = 0x100,
  PosInf     = 0x200
};

template<class T> inline T cmpResult(bool result) {
  return result ? ~T(0) : T(0);
}
template<> inline f32 cmpResult(bool result) {
  return result ? 1.0f : 0.0f;
}
template<> inline f64 cmpResult(bool result) {
  return result ? 1.0 : 0.0;
}

template<class T> inline b1 isUnordered(T x, T y) {
  return isNan(x) || isNan(y);
}

} // namespace brig
} // namespace hsa

#endif // BRIG_RUNTIME_INTERNAL_H
