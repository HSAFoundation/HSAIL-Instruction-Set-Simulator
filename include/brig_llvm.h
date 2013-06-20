//===- brig_llvm.h --------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_BRIG_LLVM_H_
#define INCLUDE_BRIG_LLVM_H_

#include "brig.h"
#include "bugs.h"

#include "llvm/DebugInfo/DIContext.h"

#include <tr1/memory>

#include <string>

namespace llvm {
class DIContext;
class Module;
class Type;
class StructType;
class LLVMContext;
class StringRef;
}

namespace hsa {
namespace brig {

class BrigModule;

struct BrigRegState;
typedef void *CallbackData;
typedef void (*Callback)(BrigRegState *regs,
                         size_t pc,
                         CallbackData cbd);

struct BrigProgram {
  const std::tr1::shared_ptr<llvm::Module> M;
  const std::tr1::shared_ptr<llvm::DIContext> debugInfo;
  BrigProgram(llvm::Module *M, llvm::DIContext *debugInfo = NULL) :
    M(M, delModule), debugInfo(debugInfo) {}
  operator bool () { return M; }
  bool operator!() { return !M; }
  llvm::Module *operator->() { return M.get(); }
  llvm::DILineInfo getLineInfoForAddress(uint64_t pc) {
    llvm::DILineInfoSpecifier spec(
      llvm::DILineInfoSpecifier::FunctionName |
      llvm::DILineInfoSpecifier::FileLineInfo |
      llvm::DILineInfoSpecifier::AbsoluteFilePath);
    return debugInfo->getLineInfoForAddress(pc, spec);
  }

 private:
  static void delModule(llvm::Module *M);
};

class GenLLVM {
 public:
  static BrigProgram getLLVMModule(const BrigModule &M,
                                   Callback cb = NULL,
                                   CallbackData cbd = NULL);
  static std::string getLLVMString(const BrigModule &M,
                                   Callback cb = NULL,
                                   CallbackData cbd = NULL);
};

} // namespace brig
} // namespace hsa

#endif  // INCLUDE_BRIG_LLVM_H_
