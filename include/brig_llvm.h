namespace llvm {
  class Module;
}
#include "brig_buffer.h"
#include <string>
namespace hsa {
namespace brig {
class GenLLVM{
 public:
  explicit GenLLVM(const Buffer& directives);
  void operator()(void);
  std::string str(void) { return std::string(""); }
 private:
  const Buffer& directives_;
  llvm::Module *brig_frontend_;
};
}
}
