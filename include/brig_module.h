#ifndef _BRIG_MODULE_H_
#define _BRIG_MODULE_H_

#include "brig.h"
#include "brig_buffer.h"
#include "brig_util.h"

// Not included in C++98
#include <stdint.h>

namespace llvm {
class raw_ostream;
}

namespace hsa {
namespace brig {

class BrigFunction;

class BrigModule {

  public:

  BrigModule(const Buffer &strings,
             const Buffer &directives,
             const Buffer &code,
             const Buffer &operands,
             llvm::raw_ostream *out) :
    S_(reinterpret_cast<const char *>(&strings.get()[0]),
       &directives.get()[0],
       &code.get()[0],
       &operands.get()[0],
       strings.size(),
       directives.size(),
       code.size(),
       operands.size()),
    out_(out),
    valid_(validate()) {}

  bool isValid() { return valid_; }

  BrigFunction begin() const;
  BrigFunction end() const;

  private:

  template<class Message>
  bool check(bool test, const Message &msg,
             const char *filename, unsigned lineno,
             const char *cause);

  template<class Directive>
  bool checkSize(const Directive *dir, const char *filename,
                 unsigned lineno);

  bool validate(void);
  bool validate(const BrigDirectiveMethod *dir);
  bool validate(const BrigDirectiveSymbol *dir);
  bool validate(const BrigDirectiveImage *dir);
  bool validate(const BrigDirectiveSampler *dir);
  bool validate(const BrigDirectiveLabel *dir);
  bool validate(const BrigDirectiveLabelList *dir);
  bool validate(const BrigDirectiveVersion *dir);
  bool validate(const BrigDirectiveProto *dir);
  bool validate(const BrigDirectiveFile *dir);
  bool validate(const BrigDirectiveComment *dir);
  bool validate(const BrigDirectiveLoc *dir);
  bool validate(const BrigDirectiveInit *dir);
  bool validate(const BrigDirectiveLabelInit *dir);
  bool validate(const BrigDirectiveControl *dir);
  bool validate(const BrigDirectivePragma *dir);
  bool validate(const BrigDirectiveExtension *dir);
  bool validate(const BrigDirectiveArgStart *dir);
  bool validate(const BrigDirectiveArgEnd *dir);
  bool validate(const BrigDirectiveBlockStart *dir);
  bool validate(const BrigDirectiveBlockNumeric *dir);
  bool validate(const BrigDirectiveBlockString *dir);
  bool validate(const BrigDirectiveBlockEnd *dir);
  bool validate(const BrigDirectivePad *dir);

  bool validate(const BrigSymbolCommon *s);
  bool validateSName(BrigsOffset32_t s_name);
  bool validateAlignment(const void *dir, uint8_t alignment);

  const BrigSections S_;
  llvm::raw_ostream *out_;
  const bool valid_;

  friend class BrigFunction;
};

} // namespace brig
} // namespace hsa

#endif /* _BRIG_MODULE_H_ */
