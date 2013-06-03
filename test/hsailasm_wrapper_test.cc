#include "hsailasm_wrapper.h"
#include "llvm/Support/raw_ostream.h"

#include "gtest/gtest.h"

TEST(HSAILASM_WRAPPER_TEST, VectorAdd) {
  const char *source_file = "VectorAdd.hsail";
  const char *output_file = "VectorAdd.o";
  std::string ErrMsg;

  int res = hsa::brig::HsailAsm::assembleHSAILSource(source_file,
                                                     output_file,
                                                     &ErrMsg);

  EXPECT_GE(res, 0);
  if(res < 0) {
    llvm::errs() << "Failed to assemble code.\n";
    llvm::errs() << ErrMsg;

    return;
  }
}
