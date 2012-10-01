/* Copyright 2012 <MulticorewareInc> */
#ifndef INCLUDE_BRIG_LLVM_H_
#define INCLUDE_BRIG_LLVM_H_

namespace llvm {
  class Module;
  class Type;
  class StructType;
  class LLVMContext;
}
#include "brig.h"
#include "brig_buffer.h"
#include "brig_module.h"
#include "brig_reader.h"
#include <string>
namespace hsa {
namespace brig {
class GenLLVM {
 public:
  static llvm::Module *getLLVMModule(const BrigModule &M);
  static std::string getLLVMString(const BrigModule &M);
};

} // namespace brig
} // namespace hsa

#endif  // INCLUDE_BRIG_LLVM_H_
