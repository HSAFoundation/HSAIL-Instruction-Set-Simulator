// Copyright 2012 MulticoreWare Inc.

#include "./gtest/gtest.h"
#include "./brig.h"
#include "./brig_buffer.h"
// ------------------ Brig2LLVM TESTS -----------------

TEST(Brig2LLVMTest, AppendBuffer) {
  BrigInstLdSt foo;
  std::vector<unsigned char> bar;
  hsa::brig::buffer bb;
  bb.append(&foo);
  EXPECT_EQ(bb.get().size(), sizeof(foo));
}
