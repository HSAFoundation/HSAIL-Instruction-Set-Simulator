#include "brig_symbol.h"
#include "brig_function.h"

namespace hsa {
namespace brig {

BrigSymbol &BrigSymbol::operator++() {

  const dir_iterator E = S_.end();
  if(it_ == E) return *this;

  ++it_;
  while(it_ != E) {
    if(const BrigDirectiveMethod *method = dyn_cast<BrigDirectiveMethod>(it_)) {
      it_ = dir_iterator(S_.directives + method->d_nextDirective);
    } else if(isa<BrigDirectiveSymbolCommon>(it_)) {
      return *this;
    } else {
      ++it_;
    }
  }

  return *this;
}

BrigSymbol arg_begin(const BrigFunction &fun) {
  return BrigSymbol(fun.S_, fun.it_ + 1);
}

BrigSymbol arg_end(const BrigFunction &fun) {
  BrigSymbol symbol(fun.S_, fun.it_ + 1 + fun.getNumArgs());
  if(!isa<BrigDirectiveSymbolCommon>(symbol.it_)) ++symbol;
  return symbol;
}

BrigSymbol BrigFunction::arg_begin() const {
  return hsa::brig::arg_begin(*this);
}

BrigSymbol BrigFunction::arg_end() const {
  return hsa::brig::arg_end(*this);
}

} // namespace brig
} // namespace hsa
