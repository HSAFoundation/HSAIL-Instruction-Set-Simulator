#include "hsailasm_wrapper.h"
#include "llvm/Support/raw_ostream.h"

#include "gtest/gtest.h"

#define STR(X) #X
#define XSTR(X) STR(X)

TEST(HSAILASM_WRAPPER_TEST, VectorAdd) {
  char* source_file = "VectorAdd.hsail";
  char* output_file = "VectorAdd.brig";
  std::string* ErrMsg = new std::string();

  int res = hsa::brig::HsailAsm::assembleHSAILSource(source_file, 
                                          output_file,
                                          ErrMsg);

  EXPECT_GE(res, 0);
  if (res < 0) {
    llvm::errs() << "Failed to assemble code.\n";
    llvm::errs() << ErrMsg;

    return;
  }
}
