#ifndef _BRIG_MODULE_H_
#define _BRIG_MODULE_H_

#include "brig.h"
#include "brig_reader.h"
#include "brig_util.h"

// Not included in C++98
#include <stdint.h>

namespace llvm {
class raw_ostream;
}

namespace hsa {
namespace brig {

class BrigFunction;
class BrigSymbol;

class BrigModule {

  public:

  BrigModule(const BrigReader &reader, llvm::raw_ostream *out) :
    S_(reader.getStrings().data(),
       reader.getDirectives().data(),
       reader.getCode().data(),
       reader.getOperands().data(),
       reader.getDebug().data(),
       reader.getStrings().size(),
       reader.getDirectives().size(),
       reader.getCode().size(),
       reader.getOperands().size(),
       reader.getDebug().size()
      ),
    out_(out),
    valid_(validate()) {}

  bool isValid() const { return valid_; }

  BrigFunction begin() const;
  BrigFunction end() const;

  BrigSymbol global_begin() const;
  BrigSymbol global_end() const;

  private:

  template<class Message>
  bool check(bool test, const Message &msg,
             const char *filename, unsigned lineno,
             const char *cause) const;

  bool validate(void) const;
  bool validateDirectives(void) const;
  bool validateCode(void) const;
  bool validateOperands(void) const;
  bool validateStrings(void) const;
  bool validateDebug(void) const;
  bool validateCCode(void) const;
  bool validateInstructions(void) const;

  bool validate(const BrigDirectiveExecutable *dir) const;
  bool validate(const BrigDirectiveVariable *dir) const;
  bool validate(const BrigDirectiveImage *dir) const;
  bool validate(const BrigDirectiveSampler *dir) const;
  bool validate(const BrigDirectiveLabel *dir) const;
  bool validate(const BrigDirectiveVersion *dir) const;
  bool validate(const BrigDirectiveSignature *dir) const;
  bool validate(const BrigDirectiveFile *dir) const;
  bool validate(const BrigDirectiveComment *dir) const;
  bool validate(const BrigDirectiveLoc *dir) const;
  bool validate(const BrigDirectiveVariableInit *dir) const;
  bool validate(const BrigDirectiveLabelInit *dir) const;
  bool validate(const BrigDirectiveControl *dir) const;
  bool validate(const BrigDirectivePragma *dir) const;
  bool validate(const BrigDirectiveExtension *dir) const;
  bool validate(const BrigDirectiveArgScopeStart *dir) const;
  bool validate(const BrigDirectiveArgScopeEnd *dir) const;
  bool validate(const BrigDirectiveBlockStart *dir) const;
  bool validate(const BrigDirectiveBlockNumeric *dir) const;
  bool validate(const BrigDirectiveBlockString *dir) const;
  bool validate(const BrigDirectiveBlockEnd *dir) const;
  bool validate(const BrigDirectiveLabelTargets *dir) const;

  bool validate(const BrigInstAddr *code) const;
  bool validate(const BrigInstAtomic *code) const;
  bool validate(const BrigInstAtomicImage *code) const;
  bool validate(const BrigInstBar *code) const;
  bool validate(const BrigInstBasic *code) const;
  bool validate(const BrigInstBr *code) const;
  bool validate(const BrigInstCmp *code) const;
  bool validate(const BrigInstCvt *code) const;
  bool validate(const BrigInstFbar *code) const;
  bool validate(const BrigInstImage *code) const;
  bool validate(const BrigInstMem *code) const;
  bool validate(const BrigInstMod *code) const;
  bool validate(const BrigInstSeg *code) const;
  bool validate(const BrigInstSourceType *code) const;

  bool validate(const BrigOperandAddress *operand) const;
  bool validate(const BrigOperandArgumentList *operand) const;
  bool validate(const BrigOperandFunctionList *operand) const;
  bool validate(const BrigOperandFunctionRef *operand) const;
  bool validate(const BrigOperandImmed *operand) const;
  bool validate(const BrigOperandLabelRef *operand) const;
  bool validate(const BrigOperandReg *operand) const;
  bool validate(const BrigOperandRegVector *operand) const;
  bool validate(const BrigOperandWavesize *operand) const;

  bool validate(const oper_iterator operands) const;
  bool validate(const inst_iterator inst) const;

  bool validateArithmeticInst(const inst_iterator inst, unsigned nary) const;
  bool validateShiftInst(const inst_iterator inst) const;
  bool validateMovdInst(const inst_iterator inst) const;
  bool validateMovsInst(const inst_iterator inst) const;
  bool validateUnpackInst(const inst_iterator inst) const;
  bool validateBranchInst(const inst_iterator inst, unsigned nary) const;
  bool validateImageQueryInst(const inst_iterator inst) const;
  bool validateParaSynInst(const inst_iterator inst, unsigned nary) const;
  bool validateLdStImageInst(const inst_iterator) const;
  bool validateFirstLastbitInst(const inst_iterator) const;

  bool validateSpecialInst(const inst_iterator inst, unsigned nary) const;
  bool validateAtomicInst(const inst_iterator inst, bool isRet) const;
  bool validateAtomicImageInst(const inst_iterator inst, bool isRet) const;

  bool validateAbs(const inst_iterator inst) const;
  bool validateAdd(const inst_iterator inst) const;
  bool validateBorrow(const inst_iterator inst) const;
  bool validateCarry(const inst_iterator inst) const;
  bool validateCopySign(const inst_iterator inst) const;
  bool validateDiv(const inst_iterator inst) const;
  bool validateFma(const inst_iterator inst) const;
  bool validateFract(const inst_iterator inst) const;
  bool validateMad(const inst_iterator inst) const;
  bool validateMax(const inst_iterator inst) const;
  bool validateMin(const inst_iterator inst) const;
  bool validateMul(const inst_iterator inst) const;
  bool validateMulHi(const inst_iterator inst) const;
  bool validateNeg(const inst_iterator inst) const;
  bool validateRem(const inst_iterator inst) const;
  bool validateSqrt(const inst_iterator inst) const;
  bool validateSub(const inst_iterator inst) const;
  bool validateMad24(const inst_iterator inst) const;
  bool validateMad24Hi(const inst_iterator inst) const;
  bool validateMul24(const inst_iterator inst) const;
  bool validateMul24Hi(const inst_iterator inst) const;
  bool validateShl(const inst_iterator inst) const;
  bool validateShr(const inst_iterator inst) const;
  bool validateAnd(const inst_iterator inst) const;
  bool validateNot(const inst_iterator inst) const;
  bool validateOr(const inst_iterator inst) const;
  bool validatePopCount(const inst_iterator inst) const;
  bool validateXor(const inst_iterator inst) const;
  bool validateBitRev(const inst_iterator inst) const;
  bool validateBitSelect(const inst_iterator inst) const;
  bool validateExtract(const inst_iterator inst) const;
  bool validateFirstBit(const inst_iterator inst) const;
  bool validateInsert(const inst_iterator inst) const;
  bool validateLastBit(const inst_iterator inst) const;
  bool validateLda(const inst_iterator inst) const;
  bool validateLdc(const inst_iterator inst) const;
  bool validateMov(const inst_iterator inst) const;
  bool validateMovdHi(const inst_iterator inst) const;
  bool validateMovdLo(const inst_iterator inst) const;
  bool validateMovsHi(const inst_iterator inst) const;
  bool validateMovsLo(const inst_iterator inst) const;
  bool validateShuffle(const inst_iterator inst) const;
  bool validateUnpackHi(const inst_iterator inst) const;
  bool validateUnpackLo(const inst_iterator inst) const;
  bool validateCmov(const inst_iterator inst) const;
  bool validateClass(const inst_iterator inst) const;
  bool validateNcos(const inst_iterator inst) const;
  bool validateNexp2(const inst_iterator inst) const;
  bool validateNlog2(const inst_iterator inst) const;
  bool validateNrcp(const inst_iterator inst) const;
  bool validateNsqrt(const inst_iterator inst) const;
  bool validateNrsqrt(const inst_iterator inst) const;
  bool validateNsin(const inst_iterator inst) const;
  bool validateBitAlign(const inst_iterator inst) const;
  bool validateByteAlign(const inst_iterator inst) const;
  bool validateF2u4(const inst_iterator inst) const;
  bool validateLerp(const inst_iterator inst) const;
  bool validateSad(const inst_iterator inst) const;
  bool validateSad2(const inst_iterator inst) const;
  bool validateSad4(const inst_iterator inst) const;
  bool validateSad4Hi(const inst_iterator inst) const;
  bool validateUnpack0(const inst_iterator inst) const;
  bool validateUnpack1(const inst_iterator inst) const;
  bool validateUnpack2(const inst_iterator inst) const;
  bool validateUnpack3(const inst_iterator inst) const;
  bool validateSegmentp(const inst_iterator inst) const;
  bool validateFtoS(const inst_iterator inst) const;
  bool validateStoF(const inst_iterator inst) const;
  bool validateCmp(const inst_iterator inst) const;
  bool validatePackedCmp(const inst_iterator inst) const;
  bool validateCvt(const inst_iterator inst) const;
  bool validateLd(const inst_iterator inst) const;
  bool validateSt(const inst_iterator inst) const;
  bool validateAtomic(const inst_iterator inst) const;
  bool validateAtomicNoRet(const inst_iterator inst) const;
  bool validateRdImage(const inst_iterator inst) const;
  bool validateLdImage(const inst_iterator inst) const;
  bool validateStImage(const inst_iterator inst) const;
  bool validateAtomicImage(const inst_iterator inst) const;
  bool validateAtomicNoRetImage(const inst_iterator inst) const;
  bool validateQueryArray(const inst_iterator inst) const;
  bool validateQueryData(const inst_iterator inst) const;
  bool validateQueryDepth(const inst_iterator inst) const;
  bool validateQueryFiltering(const inst_iterator inst) const;
  bool validateQueryHeight(const inst_iterator inst) const;
  bool validateQueryNormalized(const inst_iterator inst) const;
  bool validateQueryOrder(const inst_iterator inst) const;
  bool validateQueryWidth(const inst_iterator inst) const;
  bool validateCbr(const inst_iterator inst) const;
  bool validateBrn(const inst_iterator inst) const;
  bool validateBarrier(const inst_iterator inst) const;
  bool validateFbarArrive(const inst_iterator inst) const;
  bool validateFbarInit(const inst_iterator inst) const;
  bool validateFbarRelease(const inst_iterator inst) const;
  bool validateFbarSkip(const inst_iterator inst) const;
  bool validateFbarWait(const inst_iterator inst) const;
  bool validateSync(const inst_iterator inst) const;
  bool validateCount(const inst_iterator inst) const;
  bool validateCountUp(const inst_iterator inst) const;
  bool validateMask(const inst_iterator inst) const;
  bool validateSend(const inst_iterator inst) const;
  bool validateReceive(const inst_iterator inst) const;
  bool validateCall(const inst_iterator inst) const;
  bool validateRet(const inst_iterator inst) const;
  bool validateSysCall(const inst_iterator inst) const;
  bool validateAlloca(const inst_iterator inst) const;
  bool validateClock(const inst_iterator inst) const;
  bool validateCU(const inst_iterator inst) const;
  bool validateCurrentWorkGroupSize(const inst_iterator inst) const;
  bool validateDebugTrap(const inst_iterator inst) const;
  bool validateDispatchId(const inst_iterator inst) const;
  bool validateDynWaveId(const inst_iterator inst) const;
  bool validateLaneId(const inst_iterator inst) const;
  bool validateMaxDynWaveId(const inst_iterator inst) const;
  bool validateGridGroups(const inst_iterator inst) const;
  bool validateGridSize(const inst_iterator inst) const;
  bool validateNop(const inst_iterator inst) const;
  bool validateNullPtr(const inst_iterator inst) const;
  bool validateQid(const inst_iterator inst) const;
  bool validateWorkDim(const inst_iterator inst) const;
  bool validateWorkGroupId(const inst_iterator inst) const;
  bool validateWorkGroupSize(const inst_iterator inst) const;
  bool validateWorkItemAbsId(const inst_iterator inst) const;
  bool validateWorkItemAbsIdFlat(const inst_iterator inst) const;
  bool validateWorkItemId(const inst_iterator inst) const;
  bool validateWorkItemIdFlat(const inst_iterator inst) const;
  bool validateCombine(const inst_iterator inst) const;
  bool validateBitExtract(const inst_iterator inst) const;

  bool validOrEnd(const dir_iterator dir) const;
  bool validate(const dir_iterator dir) const;

  bool validateCCode(BrigCodeOffset32_t c_code) const;
  bool validateRegName(BrigStringOffset32_t reg, BrigType type) const;
  bool validateSName(BrigStringOffset32_t s_name) const;
  bool validateAlignment(const void *dir, uint8_t alignment) const;
  template<typename T> bool validateSize(const T *brig) const;

  const BrigDirectiveVersion* getFirstVersionDirective() const;
  bool isCompatibleAddrSize(const BrigSegment8_t sClass,
                            const BrigType16_t type) const;
  const BrigSections S_;
  llvm::raw_ostream *out_;
  const bool valid_;

  friend class BrigFunction;
  friend BrigSymbol global_begin(const BrigModule &mod);
  friend BrigSymbol global_end(const BrigModule &mod);
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_MODULE_H_ */
