#include "hsailasm_wrapper.h"
#include "llvm/Support/raw_ostream.h"

#include "gtest/gtest.h"

#define STR(X) #X
#define XSTR(X) STR(X)

TEST(HsaAsmTest, VectorAdd) {
  const char *source_file = XSTR(TEST_PATH) "/VectorAdd.hsail";
  const char *output_file = "VectorAdd.o";
  std::string ErrMsg;

  bool res = hsa::brig::HsailAsm::assembleHSAILSource(source_file,
                                                      output_file,
                                                      &ErrMsg);

  EXPECT_GE(res, true);
  if(!res)
    llvm::errs() << "Failed to assemble code: " << ErrMsg << "\n";

  llvm::sys::Path outputPath(output_file);
  outputPath.eraseFromDisk();
}

TEST(HsaAsmTest, SourceCode) {
  const char *source = "version 1:0:$small;\n";
  const char *output_file = "VectorAdd.o";
  std::string ErrMsg;

  bool res = hsa::brig::HsailAsm::assembleHSAILString(source,
                                                      output_file,
                                                      &ErrMsg);

  EXPECT_GE(res, true);
  if(!res)
    llvm::errs() << "Failed to assemble code: " << ErrMsg << "\n";

  llvm::sys::Path outputPath(output_file);
  outputPath.eraseFromDisk();
}
