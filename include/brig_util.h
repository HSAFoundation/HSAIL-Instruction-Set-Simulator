//===- brig_util.h --------------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _BRIG_UTIL_H_
#define _BRIG_UTIL_H_

#include "brig.h"
#include <cassert>
#include <cstdlib>

namespace llvm {
class raw_ostream;
raw_ostream &errs();
}

namespace hsa {
namespace brig {

typedef BrigBlockStart BrigDirectiveBlockStart;
typedef BrigBlockNumeric BrigDirectiveBlockNumeric;
typedef BrigBlockString BrigDirectiveBlockString;
typedef BrigBlockEnd BrigDirectiveBlockEnd;

typedef BrigBlockStart BrigDebugBlockStart;
typedef BrigBlockNumeric BrigDebugBlockNumeric;
typedef BrigBlockString BrigDebugBlockString;
typedef BrigBlockEnd BrigDebugBlockEnd;

template<class T> inline T *dyn_cast(BrigDirectiveBase *dir) {
  return dir->kind == T::DirKind ? reinterpret_cast<T *>(dir) : NULL;
}

template<class T> inline T *dyn_cast(BrigInstBase *inst) {
  return inst->kind == T::InstKind ? reinterpret_cast<T *>(inst) : NULL;
}

template<class T> inline T *dyn_cast(BrigOperandBase *inst) {
  return inst->kind == T::OperKind ? reinterpret_cast<T *>(inst) : NULL;
}

template<class T> inline T *dyn_cast(BrigDebugBase *inst) {
  return inst->kind == T::DebugKind ? reinterpret_cast<T *>(inst) : NULL;
}

template<> inline BrigDirectiveExecutable *dyn_cast(BrigDirectiveBase *dir) {
  if(dir->kind == BRIG_DIRECTIVE_FUNCTION ||
     dir->kind == BRIG_DIRECTIVE_KERNEL)
    return reinterpret_cast<BrigDirectiveExecutable *>(dir);
  return NULL;
}

template<> inline
BrigDirectiveSymbol *dyn_cast(BrigDirectiveBase *dir) {
  if(dir->kind == BRIG_DIRECTIVE_IMAGE ||
     dir->kind == BRIG_DIRECTIVE_SAMPLER ||
     dir->kind == BRIG_DIRECTIVE_VARIABLE)
    return reinterpret_cast<BrigDirectiveSymbol *>(dir);
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

template<class Super> class brig_iterator {

  public:

  typedef typename Super::Base Base;
  typedef brig_iterator<Super> Self;

  explicit brig_iterator() : super(), curr(NULL) {}
  explicit brig_iterator(const char *curr) : super(), curr(curr) {}
  brig_iterator(const Self &other) : super(), curr(other.curr) {}

  template<class T> explicit brig_iterator(const T *t) :
    super(t), curr(reinterpret_cast<const char *>(t)) {}

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

  Self operator+(intptr_t addend) const {
    brig_iterator other = *this;
    for(intptr_t i = 0; i < addend; ++i) {
      ++other;
    }
    return other;
  }

  uintptr_t operator-(const char *start) const { return curr - start; }

  private:
  Super super;
  const char *curr;
};

struct dir_super {
  typedef BrigDirectiveBase Base;
  dir_super() {}
  template<class T> dir_super(const T *t) { (void) T::DirKind; }
};

struct inst_super {
  typedef BrigInstBase Base;
  inst_super() {}
  template<class T> inst_super(const T *t) { (void) T::InstKind; }
};

struct oper_super {
  typedef BrigOperandBase Base;
  oper_super() {}
  template<class T> oper_super(const T *t) { (void) T::OperKind; }
};

struct debug_super {
  typedef BrigDebugBase Base;
  debug_super() {}
  template<class T> debug_super(const T *t) { (void) T::DebugKind; }
};

typedef brig_iterator<dir_super> dir_iterator;
typedef brig_iterator<inst_super> inst_iterator;
typedef brig_iterator<oper_super> oper_iterator;
typedef brig_iterator<debug_super> debug_iterator;

// The dir_iterator versions of dyn_cast, cast, and isa
template<class T, class It> inline const T *dyn_cast(const It it) {
  return dyn_cast<T>(&*it);
}

template<class T, class It> inline const T *cast(const It it) {
  return cast<T>(&*it);
}

template<class T, class It> inline bool isa(const It it) {
  return isa<T>(&*it);
}

struct BrigSections {
  const char *strings;
  const char *directives;
  const char *code;
  const char *operands;
  const char *debug;
  const size_t stringsSize;
  const size_t directivesSize;
  const size_t codeSize;
  const size_t operandsSize;
  const size_t debugSize;

  enum { HeaderSize = sizeof(BrigSectionHeader) };

  BrigSections(const char *strings,
               const char *directives,
               const char *code,
               const char *operands,
               const char *debug,
               const size_t stringsSize,
               const size_t directivesSize,
               const size_t codeSize,
               const size_t operandsSize,
               const size_t debugSize) :
    strings(strings), directives(directives),
    code(code), operands(operands), debug(debug),
    stringsSize(stringsSize), directivesSize(directivesSize),
    codeSize(codeSize), operandsSize(operandsSize), debugSize(debugSize) {}

  dir_iterator begin() const { return dir_iterator(directives + HeaderSize); }
  dir_iterator end() const {
    return dir_iterator(directives + directivesSize);
  }

  inst_iterator code_begin() const { return inst_iterator(code + HeaderSize); }
  inst_iterator code_end() const { return inst_iterator(code + codeSize); }

  oper_iterator oper_begin() const {
    return oper_iterator(operands + HeaderSize);
  }
  oper_iterator oper_end() const {
    return oper_iterator(operands + operandsSize);
  }

  debug_iterator debug_begin() const {
    return debug_iterator(debug + HeaderSize);
  }
  debug_iterator debug_end() const {
    return debug_iterator(debug + debugSize);
  }
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_UTIL_H_ */
