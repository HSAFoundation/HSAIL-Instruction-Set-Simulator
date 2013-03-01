#ifndef BRIG_RUNTIME_H
#define BRIG_RUNTIME_H

#include <stdint.h>
#include <pthread.h>

namespace hsa {
namespace brig {

struct ForceBrigRuntimeLinkage {
  ForceBrigRuntimeLinkage();
};

} // namespace brig
} // namespace hsa

typedef bool       b1;
typedef uint8_t    b8;
typedef uint8_t    u8;
typedef  int8_t    s8;
typedef uint16_t   b16;
typedef uint16_t   u16;
typedef  int16_t   s16;
typedef uint32_t   b32;
typedef uint32_t   u32;
typedef  int32_t   s32;
typedef uint64_t   b64;
typedef uint64_t   u64;
typedef  int64_t   s64;

// typedef __half f16; // Not a real half yet.
typedef float  f32;
typedef double f64;

template<class B, unsigned L>
class Vector {
 public:
  enum { Len = L };
  enum { LogLen =
         L == 2 ? 1 :
         L == 4 ? 2 :
         L == 8 ? 3 : 4 };

  typedef Vector<B, L> Self;
  typedef B Base;
  typedef Base Type[Len];

  typedef Base (*UMapFn)(Base);
  typedef Base (*BMapFn)(Base, Base);
  typedef Base (*TMapFn)(Base, Base, Base);
  typedef Base (*SMapFn)(Base, unsigned);
  typedef void (*UForEachFn)(Base);
  typedef void (*BForEachFn)(Base, Base);
  typedef void (*TForEachFn)(Base, Base, Base);
  typedef void (*QForEachFn)(Base, Base, Base, Base);
  typedef void (*SForEachFn)(Base, Base, unsigned);

  Vector(Base b = 0) {
    for(unsigned i = 0; i < Len; ++i)
      t_[i] = b;
  }

  Base &operator[](unsigned i) {
    return t_[i];
  }

  Self S() const {
    Self other = *this;
    for(unsigned i = 1; i < Len; ++i)
      other[i] = other[0];
    return other;
  }

  Self P() const {
    return *this;
  }

 private:
#ifdef __arm__
  __attribute__ ((aligned(sizeof(Type))))
#endif
  Type t_;
};

#define declareVector(TYPE,LEN)                 \
  typedef Vector<TYPE, LEN> TYPE ## x ## LEN

declareVector(u8,  4);
declareVector(s8,  4);
declareVector(u8,  8);
declareVector(s8,  8);
declareVector(u8,  16);
declareVector(s8,  16);
declareVector(u16, 2);
declareVector(s16, 2);
// declareVector(f16, 2);
declareVector(u16, 4);
declareVector(s16, 4);
// declareVector(f16, 4);
declareVector(u16, 8);
declareVector(s16, 8);
// declareVector(f16, 8);
declareVector(u32, 2);
declareVector(s32, 2);
declareVector(f32, 2);
declareVector(u32, 4);
declareVector(s32, 4);
declareVector(f32, 4);
declareVector(u64, 2);
declareVector(s64, 2);
declareVector(f64, 2);

typedef u64x2 b128;

#undef declareVector
#undef vector

struct ThreadInfo {
  void **argsArray;
  const uint32_t NDRangeSize; // number of work items
  const uint32_t workdim;     // number of work group dimensions
  pthread_barrier_t *barrier; // Workgroup barrier
  uint32_t workGroupSize[3];  // work group dimensions
  uint32_t workItemAbsId[3];  // absolute identifier
  pthread_t tid;

  ThreadInfo(uint32_t NDRangeSize, uint32_t workdim,
             uint32_t workGroupSize[3], uint32_t workItemAbsId[3],
             pthread_barrier_t *barrier,
             void *const *args, size_t size) :
    argsArray(new void*[size + 1]),
    NDRangeSize(NDRangeSize), workdim(workdim), barrier(barrier) {

    for(unsigned i = 0; i < 3; ++i) {
      this->workGroupSize[i] = workGroupSize[i];
      this->workItemAbsId[i] = workItemAbsId[i];
    }

    argsArray[0] = this;
    for(unsigned i = 0; i < size; ++i)
      argsArray[i + 1] = args[i];
  }

  ~ThreadInfo() { delete[] argsArray; }
};

#endif // BRIG_RUNTIME_H
