//===- brig_control_block.cc ----------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _BRIG_CONTROL_BLOCK_H_
#define _BRIG_CONTROL_BLOCK_H_

#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigFunction;
class BrigInstHelper;

class BrigControlBlock {

  public:

  const char *getName() const {
    if(const BrigDirectiveLabel *label = dyn_cast<BrigDirectiveLabel>(it_))
      return S_.strings + label->name + 1;
    return "brig.init";
  }

  bool operator!=(const BrigControlBlock &other) const {
    return it_ != other.it_;
  }

  BrigControlBlock &operator++();

  inst_iterator begin() const {
    uint32_t ccode = getCCode();
    return inst_iterator(S_.code + ccode);
  }

  inst_iterator end() const {
    BrigControlBlock other = *this;
    ++other;
    return other.begin();
  }

  BrigInstHelper getInstHelper() const;

  uint32_t getOffset() const { return it_ - S_.directives; }

  friend BrigControlBlock cb_begin(const BrigFunction &F);
  friend BrigControlBlock cb_end(const BrigFunction &F);

  private:
  BrigControlBlock(const BrigSections &S, const dir_iterator it) :
    S_(S),  it_(it) {}

  uint32_t getCCode() const {
    if(it_ == S_.end()) return S_.codeSize;

    const BrigDirectiveExecutable *fun = dyn_cast<BrigDirectiveExecutable>(it_);
    const BrigDirectiveLabel *label = dyn_cast<BrigDirectiveLabel>(it_);
    if(fun) return fun->code;
    else if(label) return label->code;
    else assert(false && "Bad control block");
  }

  const BrigSections &S_;
  dir_iterator it_;
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_CONTROL_BLOCK_H_ */
