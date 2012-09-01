#include "brig_runtime.h"
#include "brig_runtime_internal.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace hsa {
namespace brig {

ForceBrigRuntimeLinkage::ForceBrigRuntimeLinkage() {}

template<class T, class U, bool S> const bool IntPolicy<T, U, S>::isSigned;
template<class T, class U, bool S> const unsigned IntPolicy<T, U, S>::Bits;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::HighBit;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::Max;
template<class T, class U, bool S> const T IntPolicy<T, U, S>::Min;

template struct IntPolicy<u8,  u8,  false>;
template struct IntPolicy<u16, u16, false>;
template struct IntPolicy<u32, u32, false>;
template struct IntPolicy<u64, u64, false>;
template struct IntPolicy<s8,  u8,  true>;
template struct IntPolicy<s16, u16, true>;
template struct IntPolicy<s32, u32, true>;
template struct IntPolicy<s64, u64, true>;

template<class T> static T Abs(T t) { return abs(t); }
template<> f32 Abs<f32>(f32 f) { return fabsf(f); }
template<> f64 Abs<f64>(f64 d) { return fabs(d);  }
template<> s64 Abs<s64>(s64 s) { return labs(s);  }
template <class T> static T AbsVector(T t) { return map(Abs, t); }
SignedInst(define, Abs, Unary)
FloatInst(define, Abs, Unary)
SignedVectorInst(define, Abs, Unary)
FloatVectorInst(define, Abs, Unary)

// Saturated arithmetic implementation loosely adapted from Steven Fuerest's
// article at: http://locklessinc.com/articles/sat_arithmetic/
template<class T> static T AddSat(T x, T y) {
  if(Int<T>::isSigned) {
    T res = x + y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if(isNegX == isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x + y;
    if(res < x) return Int<T>::Max;
    return res;
  }
}
template <class T> static T AddSatVector(T x, T y) { return map(AddSat, x, y); }
SignedVectorInst(define, AddSat, Binary)
UnsignedVectorInst(define, AddSat, Binary)

template<class T> static T SubSat(T x, T y) {
  if(Int<T>::isSigned) {
    T res = x - y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if(isNegX != isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x - y;
    if(res > x) return Int<T>::Min;
    return res;
  }
}
template <class T> static T SubSatVector(T x, T y) { return map(SubSat, x, y); }
SignedVectorInst(define, SubSat, Binary)
UnsignedVectorInst(define, SubSat, Binary)

template<class T> static T MulSat(T x, T y) {
  if(Int<T>::isSigned) {
    s64 res = (s64) x * (s64) y;
    if(res > (s64) Int<T>::Max) return Int<T>::Max;
    if(res < (s64) Int<T>::Min) return Int<T>::Min;
    return res;
   } else {
    u64 res = (u64) x * (u64) y;
    if(res > (u64) Int<T>::Max) return Int<T>::Max;
    return res;
  }
}
template<> u64 MulSat(u64 x, u64 y); // DO NOT IMPLEMENT
template<> s64 MulSat(s64 x, s64 y); // DO NOT IMPLEMENT
template <class T> static T MulSatVector(T x, T y) { return map(MulSat, x, y); }
SignedVectorInst(define, MulSat, Binary)
UnsignedVectorInst(define, MulSat, Binary)

template<class T> static T Max(T x, T y) {
  if(isNan(x)) return y;
  if(isNan(y)) return x;
  return std::max(x, y);
}
template<class T> static T MaxVector(T x, T y) { return map(Max, x, y); }
SignedInst(define, Max, Binary)
UnsignedInst(define, Max, Binary)
FloatInst(define, Max, Binary)
SignedVectorInst(define, Max, Binary)
UnsignedVectorInst(define, Max, Binary)
FloatVectorInst(define, Max, Binary)

template<class T> static T Min(T x, T y) {
  if(isNan(x)) return y;
  if(isNan(y)) return x;
  return std::min(x, y);
}
template<class T> static T MinVector(T x, T y) { return map(Min, x, y); }
SignedInst(define, Min, Binary)
UnsignedInst(define, Min, Binary)
FloatInst(define, Min, Binary)
SignedVectorInst(define, Min, Binary)
UnsignedVectorInst(define, Min, Binary)
FloatVectorInst(define, Min, Binary)

} // namespace brig
} // namespace hsa
