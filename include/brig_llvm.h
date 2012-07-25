namespace llvm {
  class Module;
}
#include "brig.h"
#include "brig_buffer.h"
#include <string>
namespace hsa {
namespace brig {
class GenLLVM{
 public:
  explicit GenLLVM(const Buffer& directives, const StringBuffer&);
  void gen_proto(void);
  void gen_version(void);
  size_t gen_directive(size_t);
  size_t gen_function(size_t, const struct BrigDirectiveFunction *);
  void operator()(void);
  const std::string& str(void) { return output_; } 
 private:
  const Buffer& directives_;
  const StringBuffer& strings_;
  llvm::Module *brig_frontend_;
  std::string output_;
};
}
}
