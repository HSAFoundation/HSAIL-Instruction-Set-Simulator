#ifndef _BRIG_CONTROL_BLOCK_H_
#define _BRIG_CONTROL_BLOCK_H_

#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigFunction;

class BrigControlBlock {

  public:

  const char *getName() const {
    if(const BrigDirectiveLabel *label = dyn_cast<BrigDirectiveLabel>(it_))
      return S_.strings + label->s_name + 1;
    return "";
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

  friend BrigControlBlock cb_begin(const BrigFunction &F);
  friend BrigControlBlock cb_end(const BrigFunction &F);

  private:
  BrigControlBlock(const BrigSections &S, const dir_iterator it) :
    S_(S),  it_(it) {}

  uint32_t getCCode() const {
    const BrigDirectiveFunction *fun = dyn_cast<BrigDirectiveFunction>(it_);
    const BrigDirectiveLabel *label = dyn_cast<BrigDirectiveLabel>(it_);

    if(it_ == S_.end()) return S_.codeSize;
    else if(fun) return fun->c_code;
    else if(label) return label->c_code;
    else assert(false && "Bad control block");
  }

  const BrigSections S_;
  dir_iterator it_;
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_CONTROL_BLOCK_H_ */
