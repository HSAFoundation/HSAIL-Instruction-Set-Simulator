//===- llvm_shutdown.cc ---------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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
