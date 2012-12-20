#include "brig_function.h"
#include "brig_module.h"
#include <cassert>

namespace hsa {
namespace brig {

BrigFunction &BrigFunction::operator++() {

  const dir_iterator E = S_.end();

  if(it_ == E) return *this;
  for(++it_; it_ != E; ++it_) {
    const uint16_t kind = it_->kind;
    if(kind == BrigEDirectiveControl)
      updateControl(cast<BrigDirectiveControl>(it_));
    else if(kind == BrigEDirectiveExtension)
      updateExtension(cast<BrigDirectiveExtension>(it_));
    else if(kind == BrigEDirectiveFunction || kind == BrigEDirectiveKernel)
      return *this;
    else if(kind == BrigEDirectivePragma)
      updatePragma(cast<BrigDirectivePragma>(it_));
    else if(kind == BrigEDirectiveVersion)
      updateVersion(cast<BrigDirectiveVersion>(it_));
  }

  return *this;
}

void BrigFunction::updateVersion(const BrigDirectiveVersion *ver) {
  ver_ = ver;
}

void BrigFunction::updateControl(const BrigDirectiveControl *control) {
  if(control->controlType == BrigEMaxWIperG )
    maxWIPerG_ = control->values[0] * control->values[1] * control->values[2];
  else if(control->controlType == BrigEMaxGperC)
    maxGPerC_ = control->values[0];
  else if(control->controlType == BrigEMemOpt)
    memOpt_ = control->values[0];
  else
    assert(false && "Invalid control type");
}

void BrigFunction::updatePragma(const BrigDirectivePragma *pragma) {}

void BrigFunction::updateExtension(const BrigDirectiveExtension *ext) {}

BrigFunction BrigModule::begin() const {
  return hsa::brig::fun_begin(S_);
}

BrigFunction BrigModule::end() const {
  return hsa::brig::fun_end(S_);
}

} // namespace brig
} // namespace hsa
