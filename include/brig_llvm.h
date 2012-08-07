/* Copyright 2012 <MulticorewareInc> */
#ifndef INCLUDE_BRIG_LLVM_H_
#define INCLUDE_BRIG_LLVM_H_

namespace llvm {
  class Module;
  class Type;
}
#include "brig.h"
#include "brig_buffer.h"
#include <string>
namespace hsa {
namespace brig {
class GenLLVM {
 public:
  explicit GenLLVM(const StringBuffer&,
                   const Buffer &directives,
                   const Buffer &code,
                   const Buffer &operands);
  void gen_proto(void);
  void gen_version(void);
  size_t gen_directive(size_t);
  size_t gen_function(size_t, const struct BrigDirectiveFunction *);
  void operator()(void);
  const std::string &str(void) { return output_; }
 private:
  void gen_GPU_states(void);
  const StringBuffer &strings_;
  const Buffer &directives_;
  const Buffer &code_;
  const Buffer &operands_;
  llvm::Module *brig_frontend_;
  llvm::Type *gpu_states_type_;
  std::string output_;
};
}
}
#endif  // INCLUDE_BRIG_LLVM_H_
