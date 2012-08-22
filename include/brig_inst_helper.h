#ifndef BRIG_INST_HELPER_H
#define BRIG_INST_HELPER_H

#include "brig_util.h"

namespace hsa {
namespace brig {

class BrigInstHelper {

 public:
  BrigInstHelper(const BrigSections &S) : S_(S) {}

  const char *getName(const BrigOperandReg *reg) const {
    return S_.strings + reg->name;
  }

  const BrigOperandBase *getOperand(const inst_iterator &inst,
                                    unsigned opNum) const {
    assert(opNum < 5 && "Instructions never have more than five operands");
    uint32_t o_operand = inst->o_operands[opNum];
    assert(o_operand && "Missing operand?!");
    return reinterpret_cast<const BrigOperandBase *>(S_.operands + o_operand);
  }

 private:
  const BrigSections &S_;
};

}
}

#endif // BRIG_INST_HELPER_H
