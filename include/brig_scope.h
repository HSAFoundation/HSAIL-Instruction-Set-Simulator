//===- brig_scope.cc ------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BRIG_SCOPE_H
#define BRIG_SCOPE_H

#include "brig.h"
#include "brig_util.h"

#include <cassert>

#include <stdint.h>

namespace hsa {
namespace brig {

class BrigFunction;

class BrigScope {
 public:
  BrigScope &operator++() {
    dir_iterator E = S_.end();
    if (it_ == E) return *this;

    for (++it_; it_ != E; ++it_)
      if (isa<BrigDirectiveArgScopeStart>(it_))
        return *this;

    return *this;
  }

  bool operator!=(BrigScope &other) { return it_ != other.it_; }

  uint32_t getCodeScopeStart() const {
    const BrigDirectiveArgScopeStart *argStart =
      cast<BrigDirectiveArgScopeStart>(it_);
    return argStart->code;
  }

  uint32_t getCodeScopeEnd() const {
    return getScopeEnd()->code;
  }

  uint32_t getDirScopeStart() const { return it_ - S_.directives; }
  uint32_t getDirScopeEnd() const { return (char *) getScopeEnd() - S_.directives; }

  friend BrigScope scope_begin(const BrigFunction &F);
  friend BrigScope scope_end(const BrigFunction &F);

 private:
  BrigScope(const BrigSections &S, const dir_iterator it) :
    S_(S),  it_(it) {}
  const BrigSections &S_;
  dir_iterator it_;

  const BrigDirectiveArgScopeEnd *getScopeEnd() const {
    dir_iterator E = S_.end();
    dir_iterator scopeEndIt = it_;
    for (++scopeEndIt; it_ != E; ++scopeEndIt) {
      const BrigDirectiveArgScopeEnd *scopeEnd =
        dyn_cast<BrigDirectiveArgScopeEnd>(scopeEndIt);
      if (scopeEnd) return scopeEnd;
    }
    assert(false && "Missing argScopeEnd");
  }
};

} // namespace brig
} // namespace hsa

#endif // BRIG_SCOPE_H
