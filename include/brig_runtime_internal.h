#ifndef BRIG_RUNTIME_INTERNAL_H
#define BRIG_RUNTIME_INTERNAL_H

#include "brig_runtime.h"

namespace hsa {
namespace brig {

template<class T, class B, unsigned L>
struct VecPolicy {
  typedef VecPolicy<T, B, L> Self;
  typedef T Type;
  typedef B Base;
  static const unsigned Len = L;
  typedef Base (*UMapFn)(Base);
  typedef Base (*BMapFn)(Base, Base);

  VecPolicy(T t) : t_(t) {}

  Base &operator[](unsigned i) {
    return reinterpret_cast<Base *>(&t_)[i];
  }

  static Type S(Type t) {
    for(unsigned i = 1; i < Len; ++i) {
      ((Self) t)[i] = ((Self) t)[0];
    }
    return t;
  }

  static Type P(Type t) {
    return t;
  }

 private:
  Type t_;
};

template<class T> struct Vec;

template<class T>
static T map(typename Vec<T>::UMapFn MapFn, T t) {
  for(unsigned i = 0; i < Vec<T>::Len; ++i) {
    ((Vec<T>) t)[i] = MapFn(((Vec<T>) t)[i]);
  }
  return t;
}

template<class T>
static T map(typename Vec<T>::BMapFn MapFn, T x, T y) {
  for(unsigned i = 0; i < Vec<T>::Len; ++i) {
    ((Vec<T>) x)[i] = MapFn(((Vec<T>) x)[i], ((Vec<T>) y)[i]);
  }
  return x;
}

#define defineVec(T,LEN)                                          \
  template<> struct Vec<T ## x ## LEN> :                          \
    public VecPolicy<T ## x ## LEN, T, LEN> {                     \
    Vec(T ## x ## LEN t) : VecPolicy<T ## x ## LEN, T, LEN>(t) {} \
  };

defineVec(u8, 4)
defineVec(s8, 4)
defineVec(u8, 8)
defineVec(s8, 8)
defineVec(u8, 16)
defineVec(s8, 16)
defineVec(u16, 2)
defineVec(s16, 2)
// defineVec(f16, 2)
defineVec(u16, 4)
defineVec(s16, 4)
// defineVec(f16, 4)
defineVec(u16, 8)
defineVec(s16, 8)
// defineVec(f16, 8)
defineVec(u32, 2)
defineVec(s32, 2)
defineVec(f32, 2)
defineVec(u32, 4)
defineVec(s32, 4)
defineVec(f32, 4)
defineVec(u64, 2)
defineVec(s64, 2)
defineVec(f64, 2)

#define declareBitInst(INST,NARY)               \
  declare ## NARY(INST, b32)                    \
  declare ## NARY(INST, b64)

#define declareSignedInst(INST,NARY)            \
  declare ## NARY(INST, s32)                    \
  declare ## NARY(INST, s64)

#define declareUnsignedInst(INST,NARY)          \
  declare ## NARY(INST, u32)                    \
  declare ## NARY(INST, u64)

#define declareFloatInst(INST,NARY)             \
  /* declare ## NARY(INST, f16) */              \
  declare ## NARY(INST, f32)                    \
  declare ## NARY(INST, f64)

#define declareSignedVectorInst(INST,NARY)      \
  declare ## NARY ## Vector(INST, s8x4)         \
  declare ## NARY ## Vector(INST, s8x8)         \
  declare ## NARY ## Vector(INST, s16x2)        \
  declare ## NARY ## Vector(INST, s16x4)        \
  declare ## NARY ## Vector(INST, s32x2)

#define declareUnsignedVectorInst(INST,NARY)    \
  declare ## NARY ## Vector(INST, u8x4)         \
  declare ## NARY ## Vector(INST, u8x8)         \
  declare ## NARY ## Vector(INST, u16x2)        \
  declare ## NARY ## Vector(INST, u16x4)        \
  declare ## NARY ## Vector(INST, u32x2)

#define declareFloatVectorInst(INST,NARY)       \
  /* declare ## NARY ## Vector(INST, f16x2) */  \
  /* declare ## NARY ## Vector(INST, f16x4) */  \
  declare ## NARY ## Vector(INST, f32x2)

#define declareUnary(FUNC,TYPE)                 \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t) {  \
    return FUNC(t);                             \
  }

#define declareUnaryVector(FUNC,TYPE)           \
  declareUnaryVectorPacking(FUNC,TYPE,P)        \
  declareUnaryVectorPacking(FUNC,TYPE,S)

#define declareUnaryVectorPacking(FUNC,TYPE,PACKING)            \
  extern "C" TYPE FUNC ## _ ## PACKING ## _ ## TYPE (TYPE t) {  \
    return FUNC ## Vector(Vec<TYPE>::PACKING(t));               \
  }

#define declareBinary(FUNC,TYPE)                        \
  extern "C" TYPE FUNC ## _ ## TYPE (TYPE t, TYPE u) {  \
    return FUNC(t, u);                                  \
  }

#define declareBinaryVector(FUNC,TYPE)          \
  declareBinaryVectorPacking(FUNC, TYPE, P, P)  \
  declareBinaryVectorPacking(FUNC, TYPE, P, S)  \
  declareBinaryVectorPacking(FUNC, TYPE, S, P)  \
  declareBinaryVectorPacking(FUNC, TYPE, S, S)

#define declareBinaryVectorPacking(FUNC,TYPE,P1,P2)                     \
  extern "C" TYPE FUNC ## _ ## P1 ## P2 ## _ ## TYPE (TYPE t, TYPE u) { \
    return FUNC ## Vector(Vec<TYPE>::P1(t), Vec<TYPE>::P2(t));          \
  }

template<class T, class U, bool S> struct IntPolicy {
  typedef U Unsigned;
  static const bool isSigned = S;
  static const unsigned Bits = 8 * sizeof(T);
  static const T HighBit = T(T(1) << (Bits - 1));
  static const T Min = isSigned ? HighBit :  T(0);
  static const T Max = ~Min;
  static bool isNeg(T t) { return isSigned && (t & HighBit); }
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

} // namespace brig
} // namespace hsa

#endif // BRIG_RUNTIME_INTERNAL_H
