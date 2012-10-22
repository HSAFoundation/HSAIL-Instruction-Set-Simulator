#ifndef BRIG_INST_HELPER_H
#define BRIG_INST_HELPER_H

#include "brig_util.h"
#include <string>

namespace hsa {
namespace brig {

class BrigSymbol;

class BrigInstHelper {

 public:
  BrigInstHelper(const BrigSections &S) : S_(S) {}

  // Directive methods
  const BrigDirectiveBase *getDirective(uint32_t offset) const {
    return dir_iterator(S_.directives + offset);
  }

  const char *getName(const BrigDirectiveSymbol *symbol) const {
    return S_.strings + symbol->s.s_name;
  }

  // Operand methods
  const BrigOperandBase *getOperand(int offset) const {
    return oper_iterator(S_.operands + offset);
  }

  const char *getName(const BrigOperandReg *reg) const {
    return S_.strings + reg->name;
  }

  template<class T>
  const BrigOperandBase *getReg(const T *t) const {
    if(!t->reg) return NULL;
    oper_iterator base(S_.operands + t->reg);
    assert(isa<BrigOperandReg>(base));
    return base;
  }

  const BrigOperandBase *getOperand(const inst_iterator &inst,
                                    unsigned opNum) const {
    assert(opNum < 5 && "Instructions never have more than five operands");
    uint32_t o_operand = inst->o_operands[opNum];
    assert(o_operand && "Missing operand?!");
    return reinterpret_cast<const BrigOperandBase *>(S_.operands + o_operand);
  }

  const BrigSymbol getArgument(const BrigOperandArgumentList *argList,
                               unsigned argNo) const;

  friend const BrigSymbol getArgument(const BrigInstHelper &helper,
                                      const BrigOperandArgumentList *argList,
                                      unsigned argNo);

  // Instruction methods
  static std::string getInstName(const inst_iterator inst);

  static bool hasDest(const inst_iterator inst) {
    // Most instruction have a destination. Check for each of the special cases.
    BrigOpcode opcode = BrigOpcode(inst->opcode);
    return
      opcode != BrigSt        && opcode != BrigAtomicNoRet      &&
      opcode != BrigStImage   && opcode != BrigAtomicNoRetImage &&
      opcode != BrigBrn       && opcode != BrigCbr              &&
      opcode != BrigSync      && opcode != BrigBarrier          &&
      opcode != BrigRet       && opcode != BrigCall             &&
      opcode != BrigDebugTrap && opcode != BrigNop;
  }

  // Type methods
  static bool isFloatTy(BrigDataType type) {
    return
      type == Brigf16   || type == Brigf32   || type == Brigf64   ||
      type == Brigf16x2 || type == Brigf16x4 ||
      type == Brigf32x2 || type == Brigf32x4 ||
      type == Brigf64x2;
  }

  static bool isVectorTy(BrigDataType type) {
    return type >= Brigu8x4 && type <= Brigf64x2;
  }

  static bool isSignedTy(BrigDataType type) {
    return
      type == Brigs8    || type == Brigs16   || type == Brigs32   ||
      type == Brigs64   ||
      type == Brigs8x4  || type == Brigs8x8  || type == Brigs8x16 ||
      type == Brigs16x2 || type == Brigs16x4 || type == Brigs16x8 ||
      type == Brigs32x2 || type == Brigs32x4 ||
      type == Brigs64x2;
  }

  static unsigned getVectorLength(BrigDataType type) {

    assert(isVectorTy(type) && "Cannot get element of non-vector types");

    switch(type) {
    case Brigu16x2: case Brigs16x2: case Brigf16x2:
    case Brigu32x2: case Brigs32x2: case Brigf32x2:
    case Brigu64x2: case Brigs64x2: case Brigf64x2:
      return 2;
    case Brigu8x4:  case Brigs8x4:
    case Brigu16x4: case Brigs16x4: case Brigf16x4:
    case Brigu32x4: case Brigs32x4: case Brigf32x4:
      return 4;
    case Brigu8x8:  case Brigs8x8:  case Brigu16x8:
    case Brigs16x8: case Brigf16x8:
      return 8;
    case Brigu8x16: case Brigs8x16:
      return 16;
    default:
      assert(false && "Unknown type");
    }
  }

  // Arithmetic methods
  static bool isPacked(BrigPacking packing, unsigned opnum) {
    if(opnum == 1) {
      return
        packing == BrigPackPP    || packing == BrigPackPS    ||
        packing == BrigPackP     ||
        packing == BrigPackPPsat || packing == BrigPackPSsat ||
        packing == BrigPackPsat;
    } else if(opnum == 2) {
      return
        packing == BrigPackPP    || packing == BrigPackSP    ||
        packing == BrigPackPPsat || packing == BrigPackSPsat;
    }

    return false;
  }


  static bool isBroadcast(BrigPacking packing, unsigned opnum) {
    if(opnum == 1) {
      return
        packing == BrigPackSS    || packing == BrigPackSP    ||
        packing == BrigPackS     ||
        packing == BrigPackSSsat || packing == BrigPackSPsat ||
        packing == BrigPackSsat;
    } else if(opnum == 2) {
      return
        packing == BrigPackSS    || packing == BrigPackPS    ||
        packing == BrigPackSSsat || packing == BrigPackPSsat;
    }

    return false;
  }

  static bool isSaturated(BrigPacking packing) {
    return
      packing == BrigPackPPsat || packing == BrigPackPSsat ||
      packing == BrigPackSPsat || packing == BrigPackSSsat ||
      packing == BrigPackPsat  || packing == BrigPackSsat;
  }

  static bool isBranchInst(const inst_iterator inst) {
    BrigOpcode opcode = BrigOpcode(inst->opcode);
    return opcode == BrigBrn || opcode == BrigCbr;
  }

 private:
  const BrigSections &S_;
};

}
}

#endif // BRIG_INST_HELPER_H
