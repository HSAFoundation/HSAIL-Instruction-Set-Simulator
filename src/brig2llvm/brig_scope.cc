//===- brig_scope.cc ------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_scope.h"
#include "brig_function.h"

namespace hsa {
namespace brig {

BrigScope scope_begin(const BrigFunction &F) {
  BrigScope scope(F.S_, F.it_);
  return ++scope;
}

BrigScope scope_end(const BrigFunction &F) {
  dir_iterator dirE(F.S_.directives + F.getMethod()->nextTopLevelDirective);
  BrigScope E(F.S_, dirE);
  if (dirE != F.S_.end()) ++E;
  return E;
}

BrigScope BrigFunction::scope_begin() const {
  return hsa::brig::scope_begin(*this);
}
BrigScope BrigFunction::scope_end() const {
  return hsa::brig::scope_end(*this);
}

} // namespace brig
} // namespace hsa
