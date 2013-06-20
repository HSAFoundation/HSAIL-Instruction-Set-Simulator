//===- brig_function.cc ---------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_function.h"
#include "brig_module.h"
#include <cassert>

namespace hsa {
namespace brig {

BrigFunction &BrigFunction::operator++() {

  const dir_iterator E = S_.end();

  if (it_ == E) return *this;
  for (++it_; it_ != E; ++it_) {
    const uint16_t kind = it_->kind;
    if (kind == BRIG_DIRECTIVE_CONTROL)
      updateControl(cast<BrigDirectiveControl>(it_));
    else if (kind == BRIG_DIRECTIVE_EXTENSION)
      updateExtension(cast<BrigDirectiveExtension>(it_));
    else if (kind == BRIG_DIRECTIVE_FUNCTION || kind == BRIG_DIRECTIVE_KERNEL)
      return *this;
    else if (kind == BRIG_DIRECTIVE_PRAGMA)
      updatePragma(cast<BrigDirectivePragma>(it_));
    else if (kind == BRIG_DIRECTIVE_VERSION)
      updateVersion(cast<BrigDirectiveVersion>(it_));
  }

  return *this;
}

void BrigFunction::updateVersion(const BrigDirectiveVersion *ver) {
  ver_ = ver;
}

void BrigFunction::updateControl(const BrigDirectiveControl *control) {
}

void BrigFunction::updatePragma(const BrigDirectivePragma *pragma) {}

void BrigFunction::updateExtension(const BrigDirectiveExtension *ext) {}

BrigFunction BrigModule::begin() const {
  return hsa::brig::fun_begin(S_);
}

BrigFunction BrigModule::end() const {
  return hsa::brig::fun_end(S_);
}

}  // namespace brig
}  // namespace hsa
