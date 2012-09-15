#ifndef BRIG_READER_H
#define BRIG_READER_H

#include "llvm/ADT/StringRef.h"

namespace llvm {
namespace object {
class ObjectFile;
} // namespace object
} // namespace llvm

namespace hsa {
namespace brig {

class BrigReader {

 private:
  const llvm::object::ObjectFile *objFile_;
  const llvm::StringRef directives_;
  const llvm::StringRef code_;
  const llvm::StringRef operands_;
  const llvm::StringRef debug_;
  const llvm::StringRef strings_;

  const char *getDirectives() const { return directives_.data(); }
  const char *getCode() const { return code_.data(); }
  const char *getOperands() const { return operands_.data(); }
  const char *getDebug() const { return debug_.data(); }
  const char *getStrings() const { return strings_.data(); }

 public:

  ~BrigReader();

  static BrigReader *createBrigReader(const char *filename);

 private:

  BrigReader(llvm::object::ObjectFile *objFile,
             llvm::StringRef directives,
             llvm::StringRef code,
             llvm::StringRef operands,
             llvm::StringRef debug,
             llvm::StringRef strings) :
    objFile_(objFile), directives_(directives), code_(code),
    operands_(operands), debug_(debug), strings_(strings) {}

  static BrigReader *createBrigReader(llvm::object::ObjectFile *objFile);
};

} // namespace brig
} // namespace hsa

#endif // BRIG_READER_H
