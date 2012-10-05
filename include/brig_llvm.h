/* Copyright 2012 <MulticorewareInc> */
#ifndef INCLUDE_BRIG_LLVM_H_
#define INCLUDE_BRIG_LLVM_H_

#include "brig.h"
#include "brig_buffer.h"
#include "brig_module.h"
#include "brig_reader.h"

#include <tr1/memory>
#include <string>

namespace llvm {
  class Module;
  class Type;
  class StructType;
  class LLVMContext;
}

namespace hsa {
namespace brig {

struct BrigProgram {
  const std::tr1::shared_ptr<llvm::Module> M;
  BrigProgram(llvm::Module *M) : M(M, delModule) {}
  operator bool () { return M; }
  bool operator!() { return !M; }
  llvm::Module *operator->() { return M.get(); }

  static void delModule(llvm::Module *M);
};

class GenLLVM {
 public:
  static BrigProgram getLLVMModule(const BrigModule &M);
  static std::string getLLVMString(const BrigModule &M);
};

} // namespace brig
} // namespace hsa

#endif  // INCLUDE_BRIG_LLVM_H_
