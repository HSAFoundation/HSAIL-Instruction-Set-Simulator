#include "llvm/Support/ManagedStatic.h"
#include "gtest/gtest.h"

namespace {

struct LLVMEnvironment : public ::testing::Environment {
  virtual void TearDown() {
    llvm::llvm_shutdown();
  }
};

}

::testing::Environment* const llvm_env =
    ::testing::AddGlobalTestEnvironment(new LLVMEnvironment);
