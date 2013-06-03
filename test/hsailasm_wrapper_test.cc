#include "hsailasm_wrapper.h"
#include "llvm/Support/raw_ostream.h"

#include "gtest/gtest.h"

#define STR(X) #X
#define XSTR(X) STR(X)

TEST(HSAILASM_WRAPPER_TEST, VectorAdd) {
  const char *source_file = XSTR(TEST_PATH) "/VectorAdd.hsail";
  const char *output_file = "VectorAdd.o";
  std::string ErrMsg;

  bool res = hsa::brig::HsailAsm::assembleHSAILSource(source_file,
                                                      output_file,
                                                      &ErrMsg);

  EXPECT_GE(res, true);
  if(!res)
    llvm::errs() << "Failed to assemble code: " << ErrMsg << "\n";
}
