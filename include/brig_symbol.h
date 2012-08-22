#ifndef _BRIG_SYMBOL_H
#define _BRIG_SYMBOL_H

#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigFunction;

class BrigSymbol {

  public:

  const char *getName() const {
    return S_.strings + getSymbol()->s.s_name + 1;
  }

  BrigStorageClass getStorageClass() const {
    return BrigStorageClass(getSymbol()->s.storageClass);
  }

  BrigAttribute getLinkage() const {
    return BrigAttribute(getSymbol()->s.attribute);
  }

  bool isConst() const { return getSymbol()->s.symbolModifier & BrigConst; }

  bool isArray() const { return getSymbol()->s.symbolModifier & BrigArray; }

  bool isFlexArray() const {
    return getSymbol()->s.symbolModifier & BrigFlex;
  }

  uint32_t getArrayDim() const { return getSymbol()->s.dim; }

  BrigDataType getType() const { return BrigDataType(getSymbol()->s.type); }

  bool isImage() const { return isa<BrigDirectiveImage>(it_); }

  bool isSampler() const { return isa<BrigDirectiveSampler>(it_); }

  bool isSymbol() const { return isa<BrigDirectiveSymbol>(it_); }

  bool operator!=(const BrigSymbol &other) const {
    return it_ != other.it_;
  }

  BrigSymbol &operator++();

  friend BrigSymbol arg_begin(const BrigFunction &F);
  friend BrigSymbol arg_end(const BrigFunction &F);

  private:

  BrigSymbol(const BrigSections &S, const dir_iterator &it) :
    S_(S), it_(it) {}

  const BrigDirectiveSymbolCommon *getSymbol() const {
    return cast<BrigDirectiveSymbolCommon>(it_);
  }

  const BrigSections &S_;
  dir_iterator it_;
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_SYMBOL_H */
