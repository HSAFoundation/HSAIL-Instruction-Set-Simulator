#include "brig_symbol.h"
#include "brig_function.h"

namespace hsa {
namespace brig {

BrigSymbol &BrigSymbol::operator++() {

  const dir_iterator E = S_.end();
  if(it_ == E) return *this;

  ++it_;
  nextValid();

  return *this;
}

void BrigSymbol::nextValid() {
  const dir_iterator E = S_.end();
  while(it_ != E && !isa<BrigDirectiveSymbolCommon>(it_)) {
    const BrigDirectiveMethod *method = dyn_cast<BrigDirectiveMethod>(it_);
    if(method) {
      it_ = dir_iterator(S_.directives + method->d_nextDirective);
    } else {
      ++it_;
    }
  }
}

BrigSymbol arg_begin(const BrigFunction &fun) {
  BrigSymbol symbol(fun.S_, fun.it_ + 1);
  return symbol;
}

BrigSymbol arg_end(const BrigFunction &fun) {
  BrigSymbol symbol(fun.S_, fun.it_ + 1 + fun.getNumArgs());
  return symbol;
}

BrigSymbol local_begin(const BrigFunction &fun) {
  dir_iterator scopeBegin(fun.S_.directives +
                          fun.getMethod()->d_firstScopedDirective);
  BrigSymbol symbol(fun.S_, scopeBegin);
  return symbol;
}

BrigSymbol local_end(const BrigFunction &fun) {
  dir_iterator scopeEnd(fun.S_.directives +
                        fun.getMethod()->d_nextDirective);
  BrigSymbol symbol(fun.S_, scopeEnd);
  return symbol;
}

BrigSymbol BrigFunction::arg_begin() const {
  return hsa::brig::arg_begin(*this);
}

BrigSymbol BrigFunction::arg_end() const {
  return hsa::brig::arg_end(*this);
}

BrigSymbol BrigFunction::local_begin() const {
  return hsa::brig::local_begin(*this);
}

BrigSymbol BrigFunction::local_end() const {
  return hsa::brig::local_end(*this);
}

} // namespace brig
} // namespace hsa
