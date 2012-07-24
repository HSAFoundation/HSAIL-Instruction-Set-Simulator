// Copyright 2012 MulticoreWare Inc.
#include "./gtest/gtest.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
// ------------------ Brig2LLVM TESTS -----------------
extern void driver(void);
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
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion
    };
    directives.append(&bdv);
    BrigDirectiveFunction bdf = {
      sizeof(bdf), BrigEDirectiveFunction,
      0, //c_code
      0, //s_name
      0, //inParamCount
      60,//d_firstScopedDirective
      1, //operationCount
      96, //d_nextDirective
      0, //attribute
      0, //fbarCount
      1, //outParamCount
      0 //d_firstInParam
    };
    directives.append(&bdf);
    hsa::brig::GenLLVM codegen(directives);
    codegen();
    EXPECT_EQ(std::string(
    "; GPU register declarations\n"
    "%c_regs = type { [8 x i1] }\n"
    "%s_regs = type { [8 x i32] }\n"
    "%d_regs = type { [8 x i64] }\n"
    "%q_regs = type { [8 x i128] }\n"
    "%struct.regs = type {%c_regs, %s_regs, %d_regs, %q_regs}\n"
    "declare void @get_global_id(i32 *, i32 *)\n"
    "declare void @abort()\n"
    ), codegen.str());
  }
}
