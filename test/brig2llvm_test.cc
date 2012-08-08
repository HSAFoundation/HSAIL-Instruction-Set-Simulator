// Copyright 2012 MulticoreWare Inc.

#include "gtest/gtest.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
// ------------------ Brig2LLVM TESTS -----------------

TEST(Brig2LLVMTest, AppendBuffer) {
  {
    BrigInstLdSt foo = {
      sizeof(foo), BrigEInstLdSt,
    };
    hsa::brig::Buffer bb;
    bb.append(&foo);
    EXPECT_EQ(bb.get().size(), sizeof(foo));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&return_true"));
    strings.append(std::string("%ret_val"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion
    };
    directives.append(&bdv);
    BrigDirectiveFunction bdf = {
      sizeof(bdf), BrigEDirectiveFunction,
      0,   // c_code
      0,   // s_name
      0,   // inParamCount
      60,  // d_firstScopedDirective
      1,   // operationCount
      96,  // d_nextDirective
      0,   // attribute
      0,   // fbarCount
      1,   // outParamCount
      0    // d_firstInParam
    };
    directives.append(&bdf);
    BrigSymbolCommon s = {
      0,             // c_code
      BrigArgSpace,  // storageClass
      BrigNone,      // attribute
      0,             // reserved
      0,             // symbolModifier
      0,             // dim
      13,            // s_name
      Brigf32,       // type
      1,            // align
    };
    BrigDirectiveSymbol bds = {
      sizeof(bds),
      BrigEDirectiveSymbol,
      s,
      96,  // d_init
      0,   // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;
    BrigInstBase ret = {
      sizeof(ret),
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      { 0, 0, 0, 0, 0}
    };
    code.append(&ret);

    hsa::brig::Buffer operands;

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @return_true(float* %ret_val)")));
    // HSA-48
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%c_regs = type { [8 x i1] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%s_regs = type { [8 x i32] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%d_regs = type { [8 x i64] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%q_regs = type { [8 x i128] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%pc_regs = type { [3 x i32] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%struct.regs = type { %c_regs, %s_regs, %d_regs, %q_regs, %pc_regs }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%gpu_reg_p = alloca %struct.regs")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "ret void")));
  }
}
