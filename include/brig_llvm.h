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
  GenLLVM(const StringBuffer &strings,
          const Buffer &directives,
          const Buffer &code,
          const Buffer &operands,
          const Buffer &debug);
  GenLLVM(const BrigReader &reader);
  void operator()(void);
  llvm::Module *getModule() { return brig_frontend_; }
  const std::string &str(void) { return output_; }
 private:
  llvm::StructType *create_soa_type(llvm::Type *t, std::string name, int nr);
  void gen_GPU_states(void);
  llvm::LLVMContext *C_;
  BrigModule mod_;
  llvm::Module *brig_frontend_;
  llvm::Type *gpu_states_type_;
  std::string output_;
};

} // namespace brig
} // namespace hsa

#endif  // INCLUDE_BRIG_LLVM_H_
