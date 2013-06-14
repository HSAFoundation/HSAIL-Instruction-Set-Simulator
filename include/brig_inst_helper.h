//===- brig_inst_helper.h  ------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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

  const BrigString *getName(const BrigDirectiveSymbol *symbol) const {
    return (const BrigString *) (S_.strings + symbol->name);
  }

  // Operand methods
  const BrigOperandBase *getOperand(int offset) const {
    return oper_iterator(S_.operands + offset);
  }

  template<class T>
  const BrigString *getRegName(const T *reg) const {
    return (const BrigString *) (S_.strings + reg->reg);
  }


  const BrigString *getRegName(const BrigOperandRegVector *vec,
                               unsigned i) const {
    return (const BrigString *) (S_.strings + vec->regs[i]);
  }

  const BrigString *getData(const BrigBlockNumeric *block) {
    return (const BrigString *) (S_.strings + block->data);
  }

  const BrigString *getString(const BrigBlockString *block) {
    return (const BrigString *) (S_.strings + block->string);
  }

  template<class T>
  const BrigOperandBase *getReg(const T *t) const {
    if (!t->reg) return NULL;
    oper_iterator base(S_.operands + t->reg);
    assert(isa<BrigOperandReg>(base));
    return base;
  }

  const BrigOperandBase *getOperand(const inst_iterator &inst,
                                    unsigned opNum) const {
    assert(opNum < 5 && "Instructions never have more than five operands");
    uint32_t o_operand = inst->operands[opNum];
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

  size_t getAddr(const inst_iterator inst) const {
    return (char *) &*inst - S_.code;
  }

  static bool hasDest(const inst_iterator inst) {
    // Most instruction have a destination. Check for each of the special cases.
    BrigOpcode opcode = BrigOpcode(inst->opcode);
    return
      opcode != BRIG_OPCODE_ST &&
      opcode != BRIG_OPCODE_ATOMICNORET &&
      opcode != BRIG_OPCODE_STIMAGE &&
      opcode != BRIG_OPCODE_ATOMICIMAGENORET &&
      opcode != BRIG_OPCODE_BRN &&
      opcode != BRIG_OPCODE_CBR &&
      opcode != BRIG_OPCODE_SYNC &&
      opcode != BRIG_OPCODE_BARRIER &&
      opcode != BRIG_OPCODE_RET &&
      opcode != BRIG_OPCODE_CALL &&
      opcode != BRIG_OPCODE_DEBUGTRAP &&
      opcode != BRIG_OPCODE_NOP &&
      opcode != BRIG_OPCODE_CLEARDETECTEXCEPT &&
      opcode != BRIG_OPCODE_SETDETECTEXCEPT;
  }

  static const BrigAluModifier16_t *getAluModifier(const inst_iterator inst) {
    if (const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst))
      return &cmp->modifier;

    if (const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst))
      return &cvt->modifier;

    if (const BrigInstMod *mod = dyn_cast<BrigInstMod>(inst))
      return &mod->modifier;

    return NULL;
  }

  static bool isFtz(const inst_iterator inst) {
    const BrigAluModifier16_t *aluMod = getAluModifier(inst);
    return aluMod && *aluMod & BRIG_ALU_FTZ;
  }

  static bool hasRoundingMode(const inst_iterator inst) {
    // Cvt instructions are handled as a special case
    if (isa<BrigInstCvt>(inst)) return false;
    // Ignore near mode, since it is the default anway
    const BrigAluModifier16_t *aluMod = getAluModifier(inst);
    return aluMod && *aluMod & BRIG_ALU_ROUND;
  }

  static const char *getRoundingName(BrigAluModifier16_t mod);

  // Type methods
  static bool isFloatTy(BrigType type) {
    return
      type == BRIG_TYPE_F16   || type == BRIG_TYPE_F32   ||
      type == BRIG_TYPE_F64   ||
      type == BRIG_TYPE_F16X2 || type == BRIG_TYPE_F16X4 ||
      type == BRIG_TYPE_F16X8 ||
      type == BRIG_TYPE_F32X2 || type == BRIG_TYPE_F32X4 ||
      type == BRIG_TYPE_F64X2;
  }

  static bool isVectorTy(BrigType type) {
    return
      (type & BRIG_TYPE_PACK_32) ||
      (type & BRIG_TYPE_PACK_64) ||
      (type & BRIG_TYPE_PACK_128);
  }

  static bool isSignedTy(BrigType type) {
    return
      type == BRIG_TYPE_S8    || type == BRIG_TYPE_S16   ||
      type == BRIG_TYPE_S32   ||
      type == BRIG_TYPE_S64   ||
      type == BRIG_TYPE_S8X4  || type == BRIG_TYPE_S8X8  ||
      type == BRIG_TYPE_S8X16 ||
      type == BRIG_TYPE_S16X2 || type == BRIG_TYPE_S16X4 ||
      type == BRIG_TYPE_S16X8 ||
      type == BRIG_TYPE_S32X2 || type == BRIG_TYPE_S32X4 ||
      type == BRIG_TYPE_S64X2;
  }

  static bool isUnsignedTy(BrigType type) {
    return
      type == BRIG_TYPE_U8    || type == BRIG_TYPE_U16   ||
      type == BRIG_TYPE_U32   ||
      type == BRIG_TYPE_U64   ||
      type == BRIG_TYPE_U8X4  || type == BRIG_TYPE_U8X8  ||
      type == BRIG_TYPE_U8X16 ||
      type == BRIG_TYPE_U16X2 || type == BRIG_TYPE_U16X4 ||
      type == BRIG_TYPE_U16X8 ||
      type == BRIG_TYPE_U32X2 || type == BRIG_TYPE_U32X4 ||
      type == BRIG_TYPE_U64X2;
  }

  static bool isBitTy(BrigType type) {
    return
      type == BRIG_TYPE_B1  ||
      type == BRIG_TYPE_B8  ||
      type == BRIG_TYPE_B16 ||
      type == BRIG_TYPE_B32 ||
      type == BRIG_TYPE_B64 ||
      type == BRIG_TYPE_B128;
  }

  static unsigned getVectorLength(BrigType type) {

    assert(isVectorTy(type) && "Cannot get length of non-vector types");

    switch(type) {
    case BRIG_TYPE_U16X2: case BRIG_TYPE_S16X2: case BRIG_TYPE_F16X2:
    case BRIG_TYPE_U32X2: case BRIG_TYPE_S32X2: case BRIG_TYPE_F32X2:
    case BRIG_TYPE_U64X2: case BRIG_TYPE_S64X2: case BRIG_TYPE_F64X2:
      return 2;
    case BRIG_TYPE_U8X4:  case BRIG_TYPE_S8X4:
    case BRIG_TYPE_U16X4: case BRIG_TYPE_S16X4: case BRIG_TYPE_F16X4:
    case BRIG_TYPE_U32X4: case BRIG_TYPE_S32X4: case BRIG_TYPE_F32X4:
      return 4;
    case BRIG_TYPE_U8X8:  case BRIG_TYPE_S8X8:  case BRIG_TYPE_U16X8:
    case BRIG_TYPE_S16X8: case BRIG_TYPE_F16X8:
      return 8;
    case BRIG_TYPE_U8X16: case BRIG_TYPE_S8X16:
      return 16;
    default:
      assert(false && "Unknown type");
    }
  }

  static BrigType getElementTy(BrigType type) {
    assert(isVectorTy(type) && "Cannot get element of non-vector types");
    return BrigType(type & BRIG_TYPE_BASE_MASK);
  }

  static size_t getTypeSize(BrigType16_t type) {
    return getTypeSize(BrigType(type));
  }

  static size_t getTypeSize(BrigType type) {

    if (isVectorTy(type))
      return getVectorLength(type) * getTypeSize(getElementTy(type));

    switch(type) {
      case BRIG_TYPE_B1:
        return 1;
      case BRIG_TYPE_S8:    case BRIG_TYPE_U8:    case BRIG_TYPE_B8:
        return 8;
      case BRIG_TYPE_S16:   case BRIG_TYPE_U16:   case BRIG_TYPE_F16:
      case BRIG_TYPE_B16:
        return 16;
      case BRIG_TYPE_S32:   case BRIG_TYPE_U32:   case BRIG_TYPE_F32:
      case BRIG_TYPE_B32:
        return 32;
      case BRIG_TYPE_S64:   case BRIG_TYPE_U64:   case BRIG_TYPE_F64:
      case BRIG_TYPE_B64:
        return 64;
      case BRIG_TYPE_B128:
        return 128;
      case BRIG_TYPE_ROIMG: case BRIG_TYPE_RWIMG: case BRIG_TYPE_SAMP:
        return 64;
      default: assert(false && "Unknown type");
    }
  }

  // Arithmetic methods
  static bool isPacked(BrigPack8_t packing, unsigned opnum) {
    if (opnum == 1) {
      return
        packing == BRIG_PACK_PP    || packing == BRIG_PACK_PS    ||
        packing == BRIG_PACK_P     ||
        packing == BRIG_PACK_PPSAT || packing == BRIG_PACK_PSSAT ||
        packing == BRIG_PACK_PSAT;
    } else if (opnum == 2) {
      return
        packing == BRIG_PACK_PP    || packing == BRIG_PACK_SP    ||
        packing == BRIG_PACK_PPSAT || packing == BRIG_PACK_SPSAT;
    }

    return false;
  }

  static bool isBroadcast(BrigPack8_t packing, unsigned opnum) {
    if (opnum == 1) {
      return
        packing == BRIG_PACK_SS    || packing == BRIG_PACK_SP    ||
        packing == BRIG_PACK_S     ||
        packing == BRIG_PACK_SSSAT || packing == BRIG_PACK_SPSAT ||
        packing == BRIG_PACK_SSAT;
    } else if (opnum == 2) {
      return
        packing == BRIG_PACK_SS    || packing == BRIG_PACK_PS    ||
        packing == BRIG_PACK_SSSAT || packing == BRIG_PACK_PSSAT;
    }

    return false;
  }

  static bool isSaturated(BrigPack8_t packing) {
    return
      packing == BRIG_PACK_PPSAT || packing == BRIG_PACK_PSSAT ||
      packing == BRIG_PACK_SPSAT || packing == BRIG_PACK_SSSAT ||
      packing == BRIG_PACK_PSAT  || packing == BRIG_PACK_SSAT;
  }

  static bool isValidPacking(BrigPack8_t packing, unsigned opnum) {
    switch(packing) {
      case BRIG_PACK_NONE:
        return false;
      case BRIG_PACK_S:
      case BRIG_PACK_P:
      case BRIG_PACK_SSAT:
      case BRIG_PACK_PSAT:
        return opnum == 1;
      case BRIG_PACK_PP:
      case BRIG_PACK_PS:
      case BRIG_PACK_SP:
      case BRIG_PACK_SS:
      case BRIG_PACK_PPSAT:
      case BRIG_PACK_PSSAT:
      case BRIG_PACK_SPSAT:
      case BRIG_PACK_SSSAT:
        return opnum == 2;
      default:
        assert(false && "Invalid packing");
    }
  }

  static bool isBranchInst(const inst_iterator inst) {
    BrigOpcode opcode = BrigOpcode(inst->opcode);
    return opcode == BRIG_OPCODE_BRN || opcode == BRIG_OPCODE_CBR;
  }

  const BrigOperandBase *getBranchTarget(const inst_iterator inst) const {
    BrigOpcode opcode = BrigOpcode(inst->opcode);
    if (opcode == BRIG_OPCODE_BRN)
      return getOperand(inst, 0);
    if (opcode == BRIG_OPCODE_CBR)
      return getOperand(inst, 1);

    assert(false && "Not a branch instruction");
  }

  bool isDirectBranchInst(const inst_iterator inst) const {
    if (!isBranchInst(inst)) return false;
    return isa<BrigOperandLabelRef>(getBranchTarget(inst));
  }

  bool isIndirectBranchInst(const inst_iterator inst) const {
    if (!isBranchInst(inst)) return false;
    return isa<BrigOperandReg>(getBranchTarget(inst));
  }

 private:
  const BrigSections &S_;
};

long int nstrtol(size_t size, const uint8_t *str,
                 const uint8_t **endptr, int base);

long int nstrtol(const BrigString *str,
                 const uint8_t **endptr, int base);

}
}

#endif // BRIG_INST_HELPER_H
