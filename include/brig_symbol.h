//===- brig_symbol.h ------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _BRIG_SYMBOL_H
#define _BRIG_SYMBOL_H

#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigFunction;
class BrigInstHelper;
class BrigModule;

class BrigSymbol {

  public:

  const BrigString *getName() const {
    return (const BrigString *) (S_.strings + getSymbol()->name);
  }

  BrigSegment getStorageClass() const {
    return BrigSegment(getSymbol()->modifier);
  }

  BrigLinkage8_t getLinkage() const {
    return BrigLinkage8_t(getSymbol()->modifier) & BRIG_SYMBOL_LINKAGE;
  }

  bool isConst() const { return getSymbol()->modifier & BRIG_SYMBOL_CONST; }

  bool isArray() const { return getSymbol()->modifier & BRIG_SYMBOL_ARRAY; }

  bool isFlexArray() const {
    return getSymbol()->modifier & BRIG_SYMBOL_FLEX_ARRAY;
  }

  uint64_t getArrayDim() const {
    return ((uint64_t) getSymbol()->dimHi << 32) + getSymbol()->dimLo;
  }

  uint32_t getCCode() const { return getSymbol()->code; }

  BrigType getType() const { return BrigType(getSymbol()->type); }

  bool isImage() const { return isa<BrigDirectiveImage>(it_); }

  bool isSampler() const { return isa<BrigDirectiveSampler>(it_); }

  bool isSymbol() const { return isa<BrigDirectiveSymbol>(it_); }

  bool operator!=(const BrigSymbol &other) const {
    return it_ != other.it_;
  }

  bool hasInitializer() const {
    const BrigDirectiveSymbol *symbol = dyn_cast<BrigDirectiveSymbol>(it_);
    return symbol && symbol->init;
  };

  template<class T> const T *getInit() const {
    const BrigDirectiveSymbol *symbol = cast<BrigDirectiveSymbol>(it_);
    if (!symbol->init) return NULL;

    const BrigDirectiveVariableInit *init =
      dyn_cast<BrigDirectiveVariableInit>(dir_iterator(S_.directives +
                                                       symbol->init));
    if (!init) return NULL;

    const BrigString *str = (const BrigString *) (S_.strings + init->data);
    return reinterpret_cast<const T *>(str->bytes);
  }

  size_t getAddr() const { return it_ - S_.directives; }

  BrigSymbol &operator++();

  friend BrigSymbol arg_begin(const BrigFunction &F);
  friend BrigSymbol arg_end(const BrigFunction &F);
  friend BrigSymbol local_begin(const BrigFunction &F);
  friend BrigSymbol local_end(const BrigFunction &F);
  friend BrigSymbol global_begin(const BrigModule &mod);
  friend BrigSymbol global_end(const BrigModule &mod);
  friend const BrigSymbol getArgument(const BrigInstHelper &helper,
                                      const BrigOperandArgumentList *argList,
                                      unsigned argNo);

  private:

  BrigSymbol(const BrigSections &S, const dir_iterator &it) :
    S_(S), it_(it) { nextValid(); }

  const BrigDirectiveSymbol *getSymbol() const {
    return cast<BrigDirectiveSymbol>(it_);
  }

  void nextValid();

  const BrigSections &S_;
  dir_iterator it_;
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_SYMBOL_H */
