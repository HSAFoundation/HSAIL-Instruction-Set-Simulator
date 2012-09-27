#ifndef _BRIG_FUNCTION_H_
#define _BRIG_FUNCTION_H_

#include "brig.h"
#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigSymbol;
class BrigControlBlock;

class BrigFunction {

  public:

  const char *getName() const {
    return S_.strings + getMethod()->s_name + 1;
  }

  uint32_t getNumArgs() const {
    const BrigDirectiveMethod *method = getMethod();
    return method->inParamCount + method->outParamCount;
  }

  bool isDeclaration() const { return !getMethod()->operationCount; }

  BrigAttribute getLinkage() const {
    return BrigAttribute(getMethod()->attribute);
  }

  bool isKernel() const { return isa<BrigDirectiveKernel>(it_); }
  bool isFunction() const { return isa<BrigDirectiveFunction>(it_); }

  uint16_t getFBarCount() const { return getMethod()->fbarCount; }

  uint32_t getOffset() const { return it_ - S_.directives; }

  BrigSymbol arg_begin() const;
  BrigSymbol arg_end() const;

  BrigSymbol local_begin() const;
  BrigSymbol local_end() const;

  BrigControlBlock begin() const;
  BrigControlBlock end() const;

  bool operator!=(const BrigFunction &other) const {
    return it_ != other.it_;
  }

  BrigFunction &operator++();

  friend BrigFunction fun_begin(const BrigSections &S);
  friend BrigFunction fun_end(const BrigSections &S);
  friend BrigSymbol arg_begin(const BrigFunction &F);
  friend BrigSymbol arg_end(const BrigFunction &F);
  friend BrigSymbol local_begin(const BrigFunction &F);
  friend BrigSymbol local_end(const BrigFunction &F);
  friend BrigControlBlock cb_begin(const BrigFunction &F);
  friend BrigControlBlock cb_end(const BrigFunction &F);

  private:
  BrigFunction(const BrigSections &S, const dir_iterator it) :
    S_(S), it_(it), ver_(NULL), maxTID_(0), maxGPerC_(0), memOpt_(true) {}

  const BrigDirectiveMethod *getMethod() const {
    return cast<BrigDirectiveMethod>(it_);
  }

  void updateVersion(const BrigDirectiveVersion *ver);
  void updateControl(const BrigDirectiveControl *control);
  void updatePragma(const BrigDirectivePragma *pragma);
  void updateExtension(const BrigDirectiveExtension *ext);

  // Do not define
  BrigFunction &operator=(const BrigFunction &) /* = delete */;

  const BrigSections &S_;
  dir_iterator it_;

  const BrigDirectiveVersion *ver_;
  uint32_t maxTID_, maxGPerC_, memOpt_;
};

inline BrigFunction fun_begin(const BrigSections &S) {
  BrigFunction B(S, S.begin());
  B.updateVersion(cast<BrigDirectiveVersion>(B.it_));
  ++B;
  return B;
}

inline BrigFunction fun_end(const BrigSections &S) {
  return BrigFunction(S, S.end());
}

} // namespace brig
} // namespace hsa

#endif /* _BRIG_FUNCTION_H_ */
