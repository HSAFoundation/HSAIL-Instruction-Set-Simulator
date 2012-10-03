#ifndef _BRIG_MODULE_H_
#define _BRIG_MODULE_H_

#include "brig.h"
#include "brig_buffer.h"
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

  BrigModule(const Buffer &strings,
             const Buffer &directives,
             const Buffer &code,
             const Buffer &operands,
             const Buffer &debug,
             llvm::raw_ostream *out) :
    S_(reinterpret_cast<const char *>(&strings.get()[0]),
       &directives.get()[0],
       &code.get()[0],
       &operands.get()[0],
       &debug.get()[0],
       strings.size(),
       directives.size(),
       code.size(),
       operands.size(),
       debug.size()),
    out_(out),
    valid_(validate()) {}

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

  bool validate(const BrigDirectiveMethod *dir) const;
  bool validate(const BrigDirectiveSymbol *dir) const;
  bool validate(const BrigDirectiveImage *dir) const;
  bool validate(const BrigDirectiveSampler *dir) const;
  bool validate(const BrigDirectiveLabel *dir) const;
  bool validate(const BrigDirectiveLabelList *dir) const;
  bool validate(const BrigDirectiveVersion *dir) const;
  bool validate(const BrigDirectiveSignature *dir) const;
  bool validate(const BrigDirectiveFile *dir) const;
  bool validate(const BrigDirectiveComment *dir) const;
  bool validate(const BrigDirectiveLoc *dir) const;
  bool validate(const BrigDirectiveInit *dir) const;
  bool validate(const BrigDirectiveLabelInit *dir) const;
  bool validate(const BrigDirectiveControl *dir) const;
  bool validate(const BrigDirectivePragma *dir) const;
  bool validate(const BrigDirectiveExtension *dir) const;
  bool validate(const BrigDirectiveArgStart *dir) const;
  bool validate(const BrigDirectiveArgEnd *dir) const;
  bool validate(const BrigDirectiveBlockStart *dir) const;
  bool validate(const BrigDirectiveBlockNumeric *dir) const;
  bool validate(const BrigDirectiveBlockString *dir) const;
  bool validate(const BrigDirectiveBlockEnd *dir) const;
  bool validate(const BrigDirectivePad *dir) const;

  bool validate(const BrigSymbolCommon *s) const;

  bool validate(const BrigInstAtomic *code) const;
  bool validate(const BrigInstAtomicImage *code) const;
  bool validate(const BrigInstBar *code) const;
  bool validate(const BrigInstBase *code) const;
  bool validate(const BrigInstCmp *code) const;
  bool validate(const BrigInstImage *code) const;
  bool validate(const BrigInstCvt *code) const;
  bool validate(const BrigInstLdSt *code) const;
  bool validate(const BrigInstMem *code) const;
  bool validate(const BrigInstMod *code) const;
  bool validate(const BrigInstRead *code) const;
  bool validate(const BrigAluModifier *c) const;

  bool validate(const BrigOperandAddress *operand) const;
  bool validate(const BrigOperandArgumentList *operand) const;
  bool validate(const BrigOperandFunctionList *operand) const;
  bool validate(const BrigOperandArgumentRef *operand) const;
  bool validate(const BrigOperandBase *operand) const;
  bool validate(const BrigOperandCompound *operand) const;
  bool validate(const BrigOperandFunctionRef *operand) const;
  bool validate(const BrigOperandImmed *operand) const;
  bool validate(const BrigOperandIndirect *operand) const;
  bool validate(const BrigOperandLabelRef *operand) const;
  bool validate(const BrigOperandOpaque *operand) const;
  bool validate(const BrigOperandPad *operand) const;
  bool validate(const BrigOperandReg *operand) const;
  bool validate(const BrigOperandRegV2 *operand) const;
  bool validate(const BrigOperandRegV4 *operand) const;
  bool validate(const BrigOperandWaveSz *operand) const;

  bool validate(const oper_iterator operands) const;
  bool validate(const inst_iterator inst) const;

  bool validOrEnd(const dir_iterator dir) const;
  bool validate(const dir_iterator dir) const;

  bool validateCCode(BrigcOffset32_t c_code) const;
  bool validateSName(BrigsOffset32_t s_name) const;
  bool validateAlignment(const void *dir, uint8_t alignment) const;
  template<typename T> bool validateSize(const T *brig) const;

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
