#include "brig_runtime.h"
#include "brig_runtime_internal.h"

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

template<class T> static T Abs(T t) { return abs(t); }
template<> f32 Abs<f32>(f32 f) { return fabsf(f); }
template<> f64 Abs<f64>(f64 d) { return fabs(d);  }
template<> s64 Abs<s64>(s64 s) { return labs(s);  }
template <class T> static T AbsVector(T t) { return map(Abs, t); }
declareSignedInst(Abs, Unary)
declareFloatInst(Abs, Unary)
declareSignedVectorInst(Abs, Unary)
declareFloatVectorInst(Abs, Unary)

// Saturated arithmetic implementation loosely adapted from Steven Fuerest's
// article at: http://locklessinc.com/articles/sat_arithmetic/
template<class T> static T AddSat(T x, T y) {
  if(Int<T>::isSigned) {
    T res = x + y;

    bool isNegX = Int<T>::isNeg(x);
    bool isNegY = Int<T>::isNeg(y);
    bool isNegResult = Int<T>::isNeg(res);

    if (isNegX == isNegY && isNegX != isNegResult)
      return isNegX ? Int<T>::Min : Int<T>::Max;

    return res;

  } else {
    T res = x + y;
    if(res < x) return Int<T>::Max;
    return res;
  }
}
template <class T> static T AddSatVector(T x, T y) { return map(AddSat, x, y); }
declareSignedVectorInst(AddSat, Binary)
declareUnsignedVectorInst(AddSat, Binary)

} // namespace brig
} // namespace hsa
