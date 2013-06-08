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

  const BrigString *getName() const {
    return (const BrigString *) (S_.strings + getMethod()->name);
  }

  uint32_t getNumArgs() const {
    const BrigDirectiveExecutable *method = getMethod();
    return method->inArgCount + method->outArgCount;
  }

  bool isDeclaration() const { return !getMethod()->instCount; }

  BrigLinkage8_t getLinkage() const {
    return BrigLinkage8_t(getMethod()->modifier) & BRIG_EXECUTABLE_LINKAGE;
  }

  bool isKernel() const { return isa<BrigDirectiveKernel>(it_); }
  bool isFunction() const { return isa<BrigDirectiveFunction>(it_); }

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
    S_(S), it_(it), ver_(NULL), maxWIPerG_(0), maxGPerC_(0), memOpt_(true) {}

  const BrigDirectiveExecutable *getMethod() const {
    return cast<BrigDirectiveExecutable>(it_);
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
  uint32_t maxWIPerG_, maxGPerC_, memOpt_;
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
