// Copyright 2012 MulticoreWare Inc.

#ifndef INCLUDE_BRIG_UTIL_H_
#define INCLUDE_BRIG_UTIL_H_

#include "brig.h"
#include <cassert>
#include <cstdlib>

namespace hsa {
namespace brig {

typedef BrigBlockStart BrigDirectiveBlockStart;
typedef BrigBlockNumeric BrigDirectiveBlockNumeric;
typedef BrigBlockString BrigDirectiveBlockString;
typedef BrigBlockEnd BrigDirectiveBlockEnd;

template<class T> inline T *dyn_cast(BrigDirectiveBase *dir) {
  return dir->kind == T::DIRKIND ? reinterpret_cast<T *>(dir) : NULL;
}

template<class T> inline T *dyn_cast(BrigInstBase *inst) {
  return inst->kind == T::INSTKIND ? reinterpret_cast<T *>(inst) : NULL;
}

template<> inline BrigDirectiveMethod *dyn_cast(BrigDirectiveBase *dir) {
  if (dir->kind == BrigEDirectiveFunction ||
      dir->kind == BrigEDirectiveKernel)
    return reinterpret_cast<BrigDirectiveMethod *>(dir);
  return NULL;
}

template<> inline
BrigDirectiveSymbolCommon *dyn_cast(BrigDirectiveBase *dir) {
  if (dir->kind == BrigEDirectiveImage ||
      dir->kind == BrigEDirectiveSampler ||
      dir->kind == BrigEDirectiveSymbol)
    return reinterpret_cast<BrigDirectiveSymbolCommon *>(dir);
  return NULL;
}

template<class T, class Base> inline T *cast(Base *base) {
  T *t = dyn_cast<T>(base);
  assert(t && "Invalid Cast");
  return t;
}

// The const versions of dyn_cast, cast, and isa
template<class T, class Base> inline const T *dyn_cast(const Base *base) {
  return const_cast<const T *>(dyn_cast<T>(const_cast<Base *>(base)));
}

template<class T, class Base> inline const T *cast(const Base *base) {
  return const_cast<const T *>(cast<T>(const_cast<Base *>(base)));
}

template<class T, class Base> inline bool isa(const Base *base) {
  return dyn_cast<T>(base);
}

template<class Super> class brig_iterator : private Super {
  public:
    typedef typename Super::Base Base;
    typedef brig_iterator<Super> Self;

    explicit brig_iterator(const uint8_t *curr) : Super(), curr(curr) {}
    explicit brig_iterator(const brig_iterator &other)
    : Super(), curr(other.curr) {}

    template<class T> brig_iterator(const T *t) :
      Super(t), curr(reinterpret_cast<const uint8_t *>(t)) {}

    Self operator++(int) {
      brig_iterator other = *this;
      ++(*this);
      return other;
    }

    Self &operator++() {
      curr += (*this)->size;
      return *this;
    }

    const Base &operator*() const {
      return *reinterpret_cast<const Base *>(curr);
    }

    operator const Base *() const {
      return reinterpret_cast<const Base *>(curr);
    }

    const Base *operator->() const {
      return reinterpret_cast<const Base *>(curr);
    }

    bool operator==(const brig_iterator &other) const {
      return this->curr == other.curr;
    }

    bool operator!=(const brig_iterator &other) const {
      return this->curr != other.curr;
    }

    Self operator+(uint64_t addend) const {
      brig_iterator other = *this;
      for (unsigned i = 0; i < addend; ++i) {
        ++other;
      }
      return other;
    }

  private:
    const uint8_t *curr;
};

class dir_super {
  protected:
  typedef BrigDirectiveBase Base;
  dir_super() {}
  template<class T> dir_super(const T *t) { (void) T::DIRKIND; }
};

class inst_super {
  protected:
  typedef BrigInstBase Base;
  inst_super() {}
  template<class T> inst_super(const T *t) { (void) T::INSTKIND; }
};

typedef brig_iterator<dir_super> dir_iterator;
typedef brig_iterator<inst_super> inst_iterator;

// The dir_iterator versions of dyn_cast, cast, and isa
template<class T> inline const T *dyn_cast(const dir_iterator it) {
  return dyn_cast<T>(&*it);
}

template<class T> inline const T *cast(const dir_iterator it) {
  return cast<T>(&*it);
}

template<class T> inline bool isa(const dir_iterator it) {
  return isa<T>(&*it);
}

struct BrigSections {
  const char *strings;
  const uint8_t *directives;
  const uint8_t *code;
  const uint8_t *operands;
  const size_t stringsSize;
  const size_t directivesSize;
  const size_t codeSize;
  const size_t operandsSize;

  BrigSections(const char *strings,
               const uint8_t *directives,
               const uint8_t *code,
               const uint8_t *operands,
               const size_t stringsSize,
               const size_t directivesSize,
               const size_t codeSize,
               const size_t operandsSize) :
    strings(strings), directives(directives),
    code(code), operands(operands),
    stringsSize(stringsSize), directivesSize(directivesSize),
    codeSize(codeSize), operandsSize(operandsSize) {}

  dir_iterator begin() const {
    return dir_iterator(directives);
  }

  dir_iterator end() const {
    return dir_iterator(directives + directivesSize);
  }
};

}  // namespace brig
}  // namespace hsa

#endif  // INCLUDE_BRIG_UTIL_H_
