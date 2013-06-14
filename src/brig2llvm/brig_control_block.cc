//===- brig_control_block.cc ----------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_control_block.h"
#include "brig_function.h"
#include "brig_inst_helper.h"

namespace hsa {
namespace brig {

BrigControlBlock &BrigControlBlock::operator++() {
  dir_iterator E = S_.end();
  if (it_ == E) return *this;

  for (++it_; it_ != E; ++it_)
    if (isa<BrigDirectiveLabel>(it_) || isa<BrigDirectiveExecutable>(it_))
      return *this;

  return *this;
}

BrigInstHelper BrigControlBlock::getInstHelper() const {
  return BrigInstHelper(S_);
}

BrigControlBlock cb_begin(const BrigFunction &F) {
  return BrigControlBlock(F.S_, F.it_);
}

BrigControlBlock cb_end(const BrigFunction &F) {
  dir_iterator dirE(F.S_.directives + F.getMethod()->nextTopLevelDirective);
  assert((dirE == F.S_.end() || !isa<BrigDirectiveLabel>(dirE)) &&
         "Label outside of function!?");
  BrigControlBlock E(F.S_, dirE);
  if (dirE != F.S_.end() && !isa<BrigDirectiveExecutable>(dirE)) ++E;
  return E;
}

BrigControlBlock BrigFunction::begin() const { return cb_begin(*this); }
BrigControlBlock BrigFunction::end() const { return cb_end(*this); }

}  // namespace brig
}  // namespace hsa
