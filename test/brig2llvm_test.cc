// Copyright 2012 MulticoreWare Inc.

#include "gtest/gtest.h"
#include "llvm/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_engine.h"
#include "brig_runtime.h"
// ------------------ Brig2LLVM TESTS -----------------

TEST(Brig2LLVMTest, AppendBuffer) {
  {
    BrigInstLdSt foo = {
      sizeof(foo), BrigEInstLdSt, 0, 0, 0,
      { 0, 0, 0, 0, 0 }, 0, 0, 0
    };
    hsa::brig::Buffer bb;
    bb.append(&foo);
    EXPECT_EQ(bb.get().size(), sizeof(foo));
  }
}

TEST(Brig2LLVMTest, Example1) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    strings.append(std::string("%ret_val"));
    strings.append(std::string("%arg_val0"));
    strings.append(std::string("&abort"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFunction get_global_id = {
      sizeof(get_global_id), BrigEDirectiveFunction,
      0,   // c_code
      0,   // s_name
      1,   // inParamCount
      directives.size() + sizeof(get_global_id) +
      2 * sizeof(BrigDirectiveSymbol),  // d_firstScopedDirective
      0,   // operationCount
      directives.size() + sizeof(get_global_id) +
      2 * sizeof(BrigDirectiveSymbol),  // d_nextDirective
      0,   // attribute
      0,   // fbarCount
      1,   // outParamCount
      directives.size() + sizeof(get_global_id) +
      sizeof(BrigDirectiveSymbol)    // d_firstInParam
    };
    directives.append(&get_global_id);

    BrigDirectiveSymbol ret_val = {
      sizeof(ret_val),       // size
      BrigEDirectiveSymbol,  // kind
      {
        0,             // c_code
        BrigArgSpace,  // storageClass
        BrigNone,      // attribute
        0,             // reserved
        0,             // symbolModifier
        0,             // dim
        15,            // s_name
        Brigu32,       // type
        1,             // align
      },
      0,  // d_init
      0,   // reserved
    };
    directives.append(&ret_val);

    BrigDirectiveSymbol arg_val = {
      sizeof(arg_val),       // size
      BrigEDirectiveSymbol,  // kind
      {
        0,             // c_code
        BrigArgSpace,  // storageClass
        BrigNone,      // attribute
        0,             // reserved
        0,             // symbolModifier
        0,             // dim
        24,            // s_name
        Brigu32,       // type
        1,             // align
      },
      0,  // d_init
      0,   // reserved
    };
    directives.append(&arg_val);

    BrigDirectiveFunction abort = {
      sizeof(abort), BrigEDirectiveFunction,
      0,   // c_code
      34,  // s_name
      0,   // inParamCount
      directives.size() + sizeof(abort),  // d_firstScopedDirective
      0,   // operationCount
      directives.size() + sizeof(abort),  // d_nextDirective
      0,   // attribute
      0,   // fbarCount
      0,   // outParamCount
      0    // d_firstInParam
    };
    directives.append(&abort);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0U, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare void @get_global_id(i32*, i32*)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare void @abort()")));
  }
}

TEST(Brig2LLVMTest, Example2) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&return_true"));
    strings.append(std::string("%ret_val"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveFunction bdf = {
      sizeof(bdf), BrigEDirectiveFunction,
      0,   // c_code
      0,   // s_name
      0,   // inParamCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol),  // d_firstScopedDirective
      1,   // operationCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol),  // d_nextDirective
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
      0,   // d_init
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
    EXPECT_NE(0U, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @return_true(float* %ret_val)")));
    // HSA-48
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%c_regs = type { [8 x i1] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%s_regs = type { [32 x i32] }")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%d_regs = type { [32 x i64] }")));
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

    llvm::Module *mod = codegen.getModule();
    bool ret_val;
    void *args[] = { &ret_val };
    hsa::brig::launchBrig(mod, mod->getFunction("return_true"), args);
  }
}

TEST(Brig2LLVMTest, Example3) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&packed_ops"));
    strings.append(std::string("%x"));
    strings.append(std::string("$s1"));
    strings.append(std::string("$s2"));
    strings.append(std::string("$s0"));
    strings.append(std::string("$s3"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveFunction bdf = {
      sizeof(bdf), BrigEDirectiveFunction,
      0,   // c_code
      0,   // s_name
      0,   // inParamCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol),  // d_firstScopedDirective
      1,   // operationCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol),  // d_nextDirective
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
      12,            // s_name
      Brigu8x4,      // type
      1,            // align
    };
    BrigDirectiveSymbol bds = {
      sizeof(bds),
      BrigEDirectiveSymbol,
      s,
      0,   // d_init
      0,   // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;
    BrigInstBase abs = {
      sizeof(abs),
      BrigEInstBase,
      BrigAbs,
      Brigs8x4,
      BrigPackP,
      { 8, 20, 0, 0, 0}
    };
    code.append(&abs);
    BrigInstBase add = {
      sizeof(add),
      BrigEInstBase,
      BrigAdd,
      Brigu16x2,
      BrigPackPPsat,
      { 8, 32, 44, 0, 0}
    };
    code.append(&add);
    BrigInstBase ret = {
      sizeof(add),
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      { 0, 0, 0, 0, 0}
    };
    code.append(&ret);

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);
    BrigOperandReg bor1 = {
      sizeof(bor1),
      BrigEOperandReg,
      Brigb32,
      0,
      15
    };
    operands.append(&bor1);
    BrigOperandReg bor2 = {
      sizeof(bor2),
      BrigEOperandReg,
      Brigb32,
      0,
      19
    };
    operands.append(&bor2);
    BrigOperandReg bor3 = {
      sizeof(bor3),
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor3);
    BrigOperandReg bor4 = {
      sizeof(bor4),
      BrigEOperandReg,
      Brigb32,
      0,
      27
    };
    operands.append(&bor4);

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0, codegen.str().size());

    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare <4 x i8> @Abs_P_s8x4(<4 x i8>)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare <2 x i16> @AddSat_PP_u16x2(<2 x i16>, <2 x i16>)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @packed_ops(<4 x i8>* %x)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "getelementptr %struct.regs* %gpu_reg_p, i32 0, i32 1, i32 0, i32 2")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "getelementptr %struct.regs* %gpu_reg_p, i32 0, i32 1, i32 0, i32 3")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "call <4 x i8> @Abs_P_s8x4")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "call <2 x i16> @AddSat_PP_u16x2")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "%gpu_reg_p = alloca %struct.regs")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "ret void")));

    llvm::Module *mod = codegen.getModule();
    u8x4 x;
    void *args[] = { &x };
    hsa::brig::launchBrig(mod, mod->getFunction("packed_ops"), args);
  }
}

TEST(Brig2LLVMTest, Example4) {
  {
    hsa::brig::StringBuffer strings;
    strings.append("&branch_ops");
    strings.append("%x");
    strings.append("$c1");
    strings.append("$s1");
    strings.append("$s2");
    strings.append("@then");
    strings.append("$s0");
    strings.append("$s3");
    strings.append("@outof_IF");

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFunction bdf = {
      sizeof(bdf),                    // size
      BrigEDirectiveFunction,         // kind
      0,                              // c_code
      0,                              // s_name
      0,                              // inParamCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol),    // d_firstSCopedDirective
      5,                              // operationCount
      directives.size() + sizeof(bdf) +
      sizeof(BrigDirectiveSymbol) +
      2 * sizeof(BrigDirectiveLabel), // d_nextDirectivef
      BrigNone,                       // attribute
      0,                              // fbarCount
      1,                              // outParamCount
      0                               // d_firstInParam
    };
    directives.append(&bdf);

    BrigDirectiveSymbol bds = {
      sizeof(bds),          // size
      BrigEDirectiveSymbol, // kind
      {
        0,                    // c_code
        BrigArgSpace,         // storageClass
        BrigNone,             // attribute
        0,                    // reserved
        0,                    // symbolModifier
        0,                    // dim
        12,                   // s_name
        Brigs8x4,             // type
        1                     // align
      },
      0,                    // d_init
      0                     // reserved
    };
    directives.append(&bds);

    BrigDirectiveLabel bdl1 = {
      sizeof(bdl1),         // size
      BrigEDirectiveLabel, // kind
      100,                 // c_code
      27                   // s_name
    };
    directives.append(&bdl1);

    BrigDirectiveLabel bdl2 = {
      sizeof(bdl2),         // size
      BrigEDirectiveLabel, // kind
      132,                 // c_code
      41                   // s_name
    };
    directives.append(&bdl2);

    hsa::brig::Buffer code;
    BrigInstBase cbr = {
      sizeof(cbr),       // size
      BrigEInstBase,     // kind
      BrigCbr,           // opcode
      Brigb1,            // type
      BrigNoPacking,     // packing
      { 84, 8, 44, 0, 0 } // o_operand
    };
    code.append(&cbr);

    BrigInstBase abs = {
      sizeof(abs),        // size
      BrigEInstBase,      // kind
      BrigAbs,            // opcode
      Brigs8x4,           // type
      BrigPackP,          // packing
      { 20, 32, 0, 0, 0 } // o_operand
    };
    code.append(&abs);

    BrigInstBar brn = {
      sizeof(brn),         // size
      BrigEInstBar,        // kind
      BrigBrn,             // opcode
      Brigb32,             // type
      BrigNoPacking,       // type
      { 84, 76, 0, 0, 0 }, // o_operand
      0                    // syncFlags
    };
    code.append(&brn);

    BrigInstBase add = {
      sizeof(add),         // size
      BrigEInstBase,       // kind
      BrigAdd,             // opcode
      Brigu16x2,           // type
      BrigPackPPsat,       // packing
      { 20, 52, 64, 0, 0 } // o_operand
    };
    code.append(&add);

    BrigInstBase ret = {
      sizeof(ret),         // size
      BrigEInstBase,       // kind
      BrigRet,             // opcode
      Brigb32,             // type
      BrigNoPacking,       // packing
      { 0, 0, 0, 0, 0 }    // o_operand
    };
    code.append(&ret);

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg c1 = {
      sizeof(c1),      // size
      BrigEOperandReg, // kind
      Brigb1,          // type
      0,               // reserved
      15               // name
    };
    operands.append(&c1);

    BrigOperandReg s1 = {
      sizeof(s1),      // size
      BrigEOperandReg, // kind
      Brigb32,         // type
      0,               // reserved
      19               // name
    };
    operands.append(&s1);

    BrigOperandReg s2 = {
      sizeof(s2),      // size
      BrigEOperandReg, // kind
      Brigb32,         // type
      0,               // reserved
      23               // name
    };
    operands.append(&s2);

    BrigOperandLabelRef then = {
      sizeof(then),         // size
      BrigEOperandLabelRef, // kind
      100                   // labeldirective
    };
    operands.append(&then);

    BrigOperandReg s0 = {
      sizeof(s0),      // size
      BrigEOperandReg, // kind
      Brigb32,         // type
      0,               // reserved
      33               // name
    };
    operands.append(&s0);

    BrigOperandReg s3 = {
      sizeof(s3),      // size
      BrigEOperandReg, // kind
      Brigb32,         // type
      0,               // reserved
      37               // name
    };
    operands.append(&s3);

    BrigOperandLabelRef outOfIf = {
      sizeof(outOfIf),      // size
      BrigEOperandLabelRef, // kind
      112                   // labeldirective
    };
    operands.append(&outOfIf);

    BrigOperandImmed zero = {
      sizeof(zero),      // size
      BrigEOperandImmed, // kind
      Brigb32,           // type
      0,                 // reserved
      { 0 }              // bits
    };
    zero.bits.u = 0;
    operands.append(&zero);

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare <4 x i8> @Abs_P_s8x4(<4 x i8>)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "declare <2 x i16> @AddSat_PP_u16x2(<2 x i16>, <2 x i16>)")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "outof_IF:")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "then:")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "brig.init.succ:")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    ", label %then, label %brig.init.succ")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "br label %outof_IF")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "; preds = %then, %brig.init.succ")));

    llvm::Module *mod = codegen.getModule();
    u8x4 x;
    void *args[] = { &x };
    hsa::brig::launchBrig(mod, mod->getFunction("branch_ops"), args);
  }
}

TEST(Brig2LLVMTest, Example5) {
  {
    hsa::brig::StringBuffer strings;
    strings.append("&callee");
    strings.append("&caller");

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFunction callee = {
      sizeof(callee),                 // size
      BrigEDirectiveFunction,         // kind
      0,                              // c_code
      0,                              // s_name
      0,                              // inParamCount
      directives.size() +
      sizeof(callee),                 // d_firstSCopedDirective
      1,                              // operationCount
      directives.size() +
      sizeof(callee),                 // d_nextDirective
      BrigNone,                       // attribute
      0,                              // fbarCount
      0,                              // outParamCount
      0                               // d_firstInParam
    };
    directives.append(&callee);

    BrigDirectiveFunction caller = {
      sizeof(caller),                 // size
      BrigEDirectiveFunction,         // kind
      32,                             // c_code
      8,                              // s_name
      0,                              // inParamCount
      directives.size() +
      sizeof(caller),                 // d_firstSCopedDirective
      2,                              // operationCount
      directives.size() + sizeof(callee) +
      2 * sizeof(BrigDirectiveScope), // d_nextDirective
      BrigNone,                       // attribute
      0,                              // fbarCount
      0,                              // outParamCount
      0                               // d_firstInParam
    };
    directives.append(&caller);

    BrigDirectiveScope bds1 = {
      sizeof(bds1),           // size
      BrigEDirectiveArgStart, // kind
      32                      // c_code
    };
    directives.append(&bds1);

    BrigDirectiveScope bds2 = {
      sizeof(bds2),           // size
      BrigEDirectiveArgEnd,   // kind
      64                      // c_code
    };
    directives.append(&bds2);

    hsa::brig::Buffer code;
    BrigInstBase ret = {
      sizeof(ret),         // size
      BrigEInstBase,       // kind
      BrigRet,             // opcode
      Brigb32,             // type
      BrigNoPacking,       // packing
      { 0, 0, 0, 0, 0 }    // o_operand
    };
    code.append(&ret);

    BrigInstBase call = {
      sizeof(call),        // size
      BrigEInstBase,       // kind
      BrigCall,            // opcode
      Brigb32,             // type
      BrigNoPacking,       // packing
      { 16, 40, 8, 40, 0 }  // o_operand
    };
    code.append(&call);
    code.append(&ret);

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandFunctionRef calleeFunc = {
      sizeof(BrigOperandFunctionRef), // size
      BrigEOperandFunctionRef,        // kind
      20                              // fn
    };
    operands.append(&calleeFunc);

    BrigOperandImmed zero = {
      sizeof(zero),      // size
      BrigEOperandImmed, // kind
      Brigb32,           // type
      0,                 // reserved
      { 0 }              // bits
    };
    zero.bits.u = 0;
    operands.append(&zero);

    BrigOperandArgumentList argList = {
      sizeof(argList),          // size
      BrigEOperandArgumentList, // kind
      0,                        // elementCount
      { 0 }                     // o_args
    };
    operands.append(&argList);

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @callee() {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @caller() {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "call void @callee()")));

    llvm::Module *mod = codegen.getModule();
    llvm::ArrayRef<void *> args;
    hsa::brig::launchBrig(mod, mod->getFunction("caller"), args);
  }
}

TEST(Brig2LLVMTest, Example6) {
#if 0  // this test case is same as the wiki
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));
    strings.append(std::string("&caller"));
    strings.append(std::string("%an_input"));
    strings.append(std::string("$s1"));
    strings.append(std::string("%an_output"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),                     // size
      BrigEDirectiveVersion,           // kind
      0,                               // c_code
      1,                               // major
      0,                               // minor
      BrigESmall,                      // machine
      BrigEFull,                       // profile
      BrigENosftz,                     // ftz
      0                                // reserved
    };
    directives.append(&bdv);

    BrigDirectiveFunction callee = {
      sizeof(callee),                  // size
      BrigEDirectiveFunction,          // kind
      0,                               // c_code
      0,                               // s_name
      1,                               // inParamCount
      140,                             // d_firstScopeDirective
      3,                               // operationCount
      140,                             // d_nextDirective
      BrigNone,                        // attribute
      0,                               // fbarCount
      1,                               // outParamCount
      100                               // d_firstParam
    };
    directives.append(&callee);

    BrigDirectiveSymbol bdsy1 = {
      sizeof(bdsy1),                   // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        8,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy1);

    BrigDirectiveSymbol bdsy2 = {
      sizeof(bdsy2),                   // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        16,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy2);

    BrigDirectiveFunction caller = {
      sizeof(caller),                  // size
      BrigEDirectiveFunction,          // kind
      120,                             // c_code
      27,                              // s_name
      0,                               // inParamCount
      180,                             // d_firstScopeDirective
      3,                               // operationCount
      276,                             // d_nextDirective
      BrigNone,                        // attribute
      0,                               // fbarCount
      0,                               // outParamCount
      0                                // d_firstParam
    };
    directives.append(&caller);

    BrigDirectiveScope bdsc1 = {
      sizeof(bdsc1),                   // size
      BrigEDirectiveArgStart,          // kind
      120                              // c_code
    };
    directives.append(&bdsc1);

    BrigDirectiveSymbol bdsy3 = {
      sizeof(bdsy3),                    // size
      BrigEDirectiveSymbol,             // kind
      {
        120,                             // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        35,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy3);

    BrigDirectiveSymbol bdsy4 = {
      sizeof(bdsy4),                   // size
      BrigEDirectiveSymbol,            // kind
      {
        164,                             // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        49,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy4);

    BrigDirectiveScope bdsc2 = {
      sizeof(bdsc2),                   // size
      BrigEDirectiveArgEnd,            // kind
      240                              // c_code
    };
    directives.append(&bdsc2);

    hsa::brig::Buffer code;
    BrigInstLdSt ld1 = {
      sizeof(ld1),                     // size
      BrigEInstLdSt,                   // kind
      BrigLd,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 8, 20, 0, 0 },              // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&ld1);

    BrigInstLdSt st1 = {
      sizeof(st1),                     // size
      BrigEInstLdSt,                   // kind
      BrigSt,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 8, 36, 0, 0, 0 },              // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&st1);

    BrigInstBase ret = {
      sizeof(ret),                     //size
      BrigEInstBase,                   //kind
      BrigRet,                         //opcode
      Brigf32,                         //type
      BrigNoPacking,                   //packing
      {0, 0, 0, 0, 0},                 //o_operand
    };
    code.append(&ret);

    BrigInstLdSt st2 = {
      sizeof(st2),                     // size
      BrigEInstLdSt,                   // kind
      BrigSt,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 52, 64, 0, 0, 0 },             // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&st2);

    BrigInstBase call = {
      sizeof(call),                    // size
      BrigEInstBase,                   // kind
      BrigCall,                        // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 104, 96, 124, 0 },          // o_operand
    };
    code.append(&call);
    code.append(&ld1);
    code.append(&st1);
    code.append(&ret);

    BrigInstLdSt ld2 = {
      sizeof(ld2),                     // size
      BrigEInstLdSt,                   // kind
      BrigLd,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 8,  80, 0, 0 },             // o_operand
      BrigArgSpace,                    // storageClasss
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&ld2);

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor1 = {
      sizeof(bor1),                    // size
      BrigEOperandReg,                 // kind
      Brigb32,                         // type
      0,                               // reserved
      23                               // name
    };
    operands.append(&bor1);

    BrigOperandAddress boa1 = {
      sizeof(boa1),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      100,                             // directive
      0                                // offset
    };
    operands.append(&boa1);

    BrigOperandAddress boa2 = {
      sizeof(boa2),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      60,                              // directive
      0                                // offset
    };
    operands.append(&boa2);

    BrigOperandReg bor2 = {
      sizeof(bor2),                    // size
      BrigEOperandReg,                 // kind
      Brigb32,                         // type
      0,                               // reserved
      45                               // name
    };
    operands.append(&bor2);

    BrigOperandAddress boa3 = {
      sizeof(boa3),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      188,                             // directive
      0                                // offset
    };
    operands.append(&boa3);

    BrigOperandAddress boa4 = {
      sizeof(boa4),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      228,                             // directive
      0                                // offset
    };
    operands.append(&boa4);

    BrigOperandFunctionRef bofr = {
      sizeof(bofr),                    // size
      BrigEOperandFunctionRef,         // kind
      20                               // fn
    };
    operands.append(&bofr);

    BrigOperandArgumentList boal1 = {
      sizeof(boal1),                   // size
      BrigEOperandArgumentList,        // kind
      1,                               // elementCount
      {116}                            // o_args[0]
    };
    operands.append(&boal1);

    BrigOperandArgumentRef boar1 = {
      sizeof(boar1),                   // size
      BrigEOperandArgumentRef,         // kind
      188                              // arg
    };
    operands.append(&boar1);

    BrigOperandArgumentList boal2 = {
      sizeof(boal2),                   // size
      BrigEOperandArgumentList,        // kind
      1,                               // elementCount
      {136}                            // o_args[0]
    };
    operands.append(&boal2);

    BrigOperandArgumentRef boar2 = {
      sizeof(boar2),                   // size
      BrigEOperandArgumentRef,         // kind
      228                              // arg
    };
    operands.append(&boar2);

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();

    EXPECT_NE(0, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @callee(float *%output, float *%input) {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @caller() {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "call void @callee(float *%output, float *%input)")));

    llvm::Module *mod = codegen.getModule();
    llvm::ArrayRef<void *> args;
    hsa::brig::launchBrig(mod, mod->getFunction("caller"), args);
  }
#endif  //end of this test case is same as the wiki

#if 0  //this test case move the seventh operands to the last
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));
    strings.append(std::string("&caller"));
    strings.append(std::string("%an_input"));
    strings.append(std::string("$s1"));
    strings.append(std::string("%an_output"));

    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      20,                              // size
      BrigEDirectiveVersion,           // kind
      0,                               // c_code
      1,                               // major
      0,                               // minor
      BrigESmall,                      // machine
      BrigEFull,                       // profile
      BrigENosftz,                     // ftz
      0                                // reserved
    };
    directives.append(&bdv);

    BrigDirectiveFunction callee = {
      40,                              // size
      BrigEDirectiveFunction,          // kind
      0,                               // c_code
      0,                               // s_name
      1,                               // inParamCount
      140,                             // d_firstScopeDirective
      3,                               // operationCount
      140,                             // d_nextDirective
      BrigNone,                        // attribute
      0,                               // fbarCount
      1,                               // outParamCount
      100                              // d_firstParam
    };
    directives.append(&callee);

    BrigDirectiveSymbol bdsy1 = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        8,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy1);

    BrigDirectiveSymbol bdsy2 = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        16,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy2);

    BrigDirectiveFunction caller = {
      40,                              // size
      BrigEDirectiveFunction,          // kind
      120,                             // c_code
      27,                              // s_name
      0,                               // inParamCount
      180,                             // d_firstScopeDirective
      3,                               // operationCount
      276,                             // d_nextDirective
      BrigNone,                        // attribute
      0,                               // fbarCount
      0,                               // outParamCount
      0                                // d_firstParam
    };
    directives.append(&caller);

    BrigDirectiveScope bdsc1 = {
      8,                               // size
      BrigEDirectiveArgStart,          // kind
      120                              // c_code
    };
    directives.append(&bdsc1);

    BrigDirectiveSymbol bdsy3 = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        120,                             // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        35,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy3);

    BrigDirectiveSymbol bdsy4 = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        164,                             // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        49,                              // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy4);

    BrigDirectiveScope bdsc2 = {
      8,                               // size
      BrigEDirectiveArgEnd,            // kind
      240                              // c_code
    };
    directives.append(&bdsc2);

    hsa::brig::Buffer code;
    BrigInstLdSt ld1 = {
      sizeof(ld1),                     // size
      BrigEInstLdSt,                   // kind
      BrigLd,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 8, 20, 0, 0 },              // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&ld1);

    BrigInstLdSt st1 = {
      sizeof(st1),                     // size
      BrigEInstLdSt,                   // kind
      BrigSt,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 8, 36, 0, 0, 0 },              // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&st1);

    BrigInstBase ret = {
      sizeof(ret),                     //size
      BrigEInstBase,                   //kind
      BrigRet,                         //opcode
      Brigf32,                         //type
      BrigNoPacking,                   //packing
      {0, 0, 0, 0, 0},                 //o_operand
    };
    code.append(&ret);

    BrigInstLdSt st2 = {
      sizeof(st2),                     // size
      BrigEInstLdSt,                   // kind
      BrigSt,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 52, 64, 0, 0, 0 },             // o_operand
      BrigArgSpace,                    // storageClass
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&st2);

    BrigInstBase call = {
      sizeof(call),                    // size
      BrigEInstBase,                   // kind
      BrigCall,                        // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 88, 80, 108, 0 },           // o_operand
    };
    code.append(&call);
    code.append(&ld1);
    code.append(&st1);
    code.append(&ret);

    BrigInstLdSt ld2 = {
      sizeof(ld2),                     // size
      BrigEInstLdSt,                   // kind
      BrigLd,                          // opcode
      Brigf32,                         // type
      BrigNoPacking,                   // packing
      { 0, 8,  128, 0, 0 },            // o_operand
      BrigArgSpace,                    // storageClasss
      BrigRegular,                     // memorySemantic
      0                                // equivClass
    };
    code.append(&ld2);

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor1 = {
      sizeof(bor1),                    // size
      BrigEOperandReg,                 // kind
      Brigb32,                         // type
      0,                               // reserved
      23                               // name
    };
    operands.append(&bor1);

    BrigOperandAddress boa1 = {
      sizeof(boa1),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      100,                             // directive
      0                                // offset
    };
    operands.append(&boa1);

    BrigOperandAddress boa2 = {
      sizeof(boa2),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      60,                              // directive
      0                                // offset
    };
    operands.append(&boa2);

    BrigOperandReg bor2 = {
      sizeof(bor2),                    // size
      BrigEOperandReg,                 // kind
      Brigb32,                         // type
      0,                               // reserved
      45                               // name
    };
    operands.append(&bor2);

    BrigOperandAddress boa3 = {
      sizeof(boa3),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      188,                             // directive
      0                                // offset
    };
    operands.append(&boa3);

    BrigOperandFunctionRef bofr = {
      sizeof(bofr),                    // size
      BrigEOperandFunctionRef,         // kind
      20                               // fn
    };
    operands.append(&bofr);

    BrigOperandArgumentList boal1 = {
      sizeof(boal1),                   // size
      BrigEOperandArgumentList,        // kind
      1,                               // elementCount
      {100}                             // o_args[0]
    };
    operands.append(&boal1);

    BrigOperandArgumentRef boar1 = {
      sizeof(boar1),                   // size
      BrigEOperandArgumentRef,         // kind
      188                              // arg
    };
    operands.append(&boar1);

    BrigOperandArgumentList boal2 = {
      sizeof(boal2),                   // size
      BrigEOperandArgumentList,        // kind
      1,                               // elementCount
      {120}                            // o_args[0]
    };
    operands.append(&boal2);

    BrigOperandArgumentRef boar2 = {
      sizeof(boar2),                   // size
      BrigEOperandArgumentRef,         // kind
      228                              // arg
    };
    operands.append(&boar2);

    BrigOperandAddress boa4 = {
      sizeof(boa4),                    // size
      BrigEOperandAddress,             // kind
      Brigb32,                         // type
      0,                               // reserved
      228,                             // directive
      0                                // offset
    };
    operands.append(&boa4);

    hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    codegen();
    EXPECT_NE(0, codegen.str().size());
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @callee(float *%output, float *%input) {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "define void @caller() {")));
    EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    "call void @callee(float *%output, float *%input)")));

    llvm::Module *mod = codegen.getModule();
    llvm::ArrayRef<void *> args;
    hsa::brig::launchBrig(mod, mod->getFunction("caller"), args);
  }
#endif  //end of this test case move the seventh operands to the last
}

TEST(Brig2LLVMTest, validateBrigDirectiveComment) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig comment"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveComment bdc = {
      sizeof(bdc), //uint16_t size;
      BrigEDirectiveComment, //uint16_t kind;
      0, //BrigcOffset32_t c_code;
      0 //BrigsOffset32_t s_name;
    };
    directives.append(&bdc);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig comment"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveComment bdc = {
      sizeof(bdc), //uint16_t size;
      BrigEDirectiveComment, //uint16_t kind;
      10, //BrigcOffset32_t c_code; correct: 0
      0 //BrigsOffset32_t s_name; correct: 0
    };
    directives.append(&bdc);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig comment"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveComment bdc = {
      sizeof(bdc), //uint16_t size;
      BrigEDirectiveComment, //uint16_t kind;
      0, //BrigcOffset32_t c_code; correct: 0
      100 //BrigsOffset32_t s_name; correct: 0
    };
    directives.append(&bdc);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}
TEST(Brig2LLVMTest, validateBrigDirectiveFile) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("filename"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveFile bdf = {
      sizeof(bdf), //uint16_t size;
      BrigEDirectiveFile, //uint16_t kind;
      0, //BrigcOffset32_t c_code;
      1, //uint32_t fileid;
      0 //BrigsOffset32_t s_filename;
    };
    directives.append(&bdf);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("filename"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFile bdf = {
      sizeof(bdf), //uint16_t size;
      BrigEDirectiveFile, //uint16_t kind;
      10, //BrigcOffset32_t c_code; correct: 0
      1, //uint32_t fileid;
      0 //BrigsOffset32_t s_filename; correct: 0
    };
    directives.append(&bdf);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("filename"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFile bdf = {
      sizeof(bdf), //uint16_t size;
      BrigEDirectiveFile, //uint16_t kind;
      0, //BrigcOffset32_t c_code; correct: 0
      1, //uint32_t fileid;
      100 //BrigsOffset32_t s_filename; correct: 0
    };
    directives.append(&bdf);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}
TEST(Brig2LLVMTest, validateBrigDirectiveLoc) {
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveLoc bdl =  {
      sizeof(bdl), //uint16_t size;
      BrigEDirectiveLoc, //uint16_t kind;
      0, //BrigcOffset32_t c_code;
      0, //uint32_t sourceFile;
      0, //uint32_t sourceLine;
      0, //uint32_t sourceColumn;
    };
    directives.append(&bdl);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveLoc bdl =  {
      sizeof(bdl), //uint16_t size;
      BrigEDirectiveLoc, //uint16_t kind;
      10, //BrigcOffset32_t c_code;  correct: 0
      0, //uint32_t sourceFile;
      0, //uint32_t sourceLine;
      0, //uint32_t sourceColumn;
    };
    directives.append(&bdl);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}

TEST(Brig2LLVMTest, validateBrigDirectiveControl) {
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveControl bdc = {
      sizeof(bdc), //uint16_t size;
      BrigEDirectiveControl, //uint16_t kind;
      0,
      0,
      {0, 0, 0}
    };
    directives.append(&bdc);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveControl bdc = {
      sizeof(bdc), //uint16_t size;
      BrigEDirectiveControl, //uint16_t kind;
      10, //BrigcOffset32_t c_code; correct: 0
      0,
      {0, 0, 0}
    };
    directives.append(&bdc);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}

// This method appends a BrigDirectiveProto to the buffer. BrigDirectiveProto is
// a variable length structure. This means the last field of BrigDirectiveProto
// is an array of unknown size. Unfortunately, variable length structures are
// not well suppored in C++ 98. We work around this limitation by creating an
// appropriately sized variable length array and then reinterpret casting the
// array to a BrigDirectiveProto.
//
// This method takes input Iterators to the beginning and end of an object
// containing the BrigDirectiveProto::BrigProtoTypes of the BrigDirectiveProto's
// input and output types. Usually, it is convenient to use a pointer, but other
// iterators can also be used. To prevent errors, the method checks that
// outCount + inCount is equal to the distance between the B and E iterators.
template<class Buffer, class Iterator>
static void appendBrigDirectiveProto(Buffer &buffer,
                                     BrigsOffset32_t c_code,
                                     BrigsOffset32_t s_name,
                                     uint16_t fbarCount,
                                     uint16_t reserved,
                                     uint32_t outCount,
                                     uint32_t inCount,
                                     const Iterator &B,
                                     const Iterator &E) {
  const size_t numArgs = E - B;
  EXPECT_EQ(outCount + inCount, numArgs);

  size_t arraySize = sizeof(BrigDirectiveSignature) +
    sizeof(BrigDirectiveSignature::BrigProtoType) * (numArgs - 1);
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveSignature *bdp =
    reinterpret_cast<BrigDirectiveSignature *>(array);

  bdp->size = arraySize;
  bdp->kind = BrigEDirectiveSignature;
  bdp->c_code = c_code;
  bdp->s_name = s_name;
  bdp->fbarCount = fbarCount;
  bdp->reserved = reserved;
  bdp->outCount = outCount;
  bdp->inCount = inCount;

  Iterator it = B;
  for (size_t i = 0; i < numArgs; ++i)
    bdp->types[i] = *it++;

  buffer.append(bdp);

  delete[] array;
}

TEST(Brig2LLVMTest, VarSizeDirective) {
  {
    hsa::brig::Buffer bb;
    BrigDirectiveSignature::BrigProtoType args[] = {
      // type, align, hasDim, dim
      {Brigu32, 1, 0, 0},
      {Brigu32, 1, 0, 0}
    };
    appendBrigDirectiveProto(bb,
                             0,  // c_code
                             0,  // s_name
                             0,  // fbarCount
                             0,  // reserved
                             1,  // outCount
                             1,  // inCount
                             &args[0],
                             &args[2]);

    EXPECT_EQ(sizeof(BrigDirectiveSignature) +
              sizeof(sizeof(BrigDirectiveSignature::BrigProtoType)),
              bb.size());
  }
}

TEST(Brig2LLVMTest, BrigDirectiveKernel_test) {
  //true case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveKernel bdk = {
      sizeof(bdk),
      BrigEDirectiveKernel,
      0,                                       // c_code
      1,                                       // s_name
      0,                                       // inParamCount
      directives.size() + sizeof(bdk) +
      sizeof(BrigDirectiveSymbol),             // d_firstScopedDirective
      1,                                       // operationCount
      directives.size() + sizeof(bdk) +
      sizeof(BrigDirectiveSymbol),             // d_nextDirective
      0,                                       // attribute
      0,                                       // fbarCount
      1,                                       // outParamCount
      0                                        // d_firstInParam
    };
    directives.append(&bdk);

    BrigSymbolCommon s = {
      0,                                      // c_code
      BrigArgSpace,                           // storageClass
      BrigNone,                               // attribute
      0,                                      // reserved
      0,                                      // symbolModifier
      0,                                      // dim
      13,                                     // s_name
      Brigf32,                                // type
      1,                                      // align
    };
    BrigDirectiveSymbol bds = {
      sizeof(bds),
      BrigEDirectiveSymbol,
      s,
      0,   // d_init
      0,   // reserved
    };
    directives.append(&bds);


    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
   }
   //false case
   {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveKernel bdk = {
      sizeof(bdk),
      BrigEDirectiveKernel,
      1,                                       // c_code
      13,                                      // s_name
      0,                                       // inParamCount
      96,                                      // d_firstScopedDirective
      1,                                       // operationCount
      96,                                      // d_nextDirective
      10,                                      // attribute
      0,                                       // fbarCount
      0,                                       // outParamCount
      0                                        // d_firstInParam
    };
    directives.append(&bdk);

    BrigSymbolCommon s = {
      0,                                      // c_code
      BrigArgSpace,                           // storageClass
      BrigNone,                               // attribute
      0,                                      // reserved
      0,                                      // symbolModifier
      0,                                      // dim
      1,                                      // s_name
      Brigf32,                                // type
      1,                                      // align
    };
    BrigDirectiveSymbol bds = {
      sizeof(bds),
      BrigEDirectiveSymbol,
      s,
      0,   // d_init
      0,   // reserved
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod2(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod2.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
    EXPECT_EQ(std::string::npos, errorMsg.find(std::string(
    "Too few argument symbols")));
    EXPECT_EQ(std::string::npos, errorMsg.find(std::string(
    "Argument not in arg space")));
    EXPECT_EQ(std::string::npos, errorMsg.find(std::string(
    "The first scoped directive is too early")));
    EXPECT_EQ(std::string::npos, errorMsg.find(std::string(
    "The next directive is before the first scoped directive")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid linkage type")));
    EXPECT_EQ(std::string::npos, errorMsg.find(std::string(
    "d_firstInParam is wrong")));
   }
}

TEST(Brig2LLVMTest, BrigDirectiveExtension_test) {
  //true case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveExtension bde = {
    sizeof(bde),                         //size
    BrigEDirectiveExtension,             //kind
    0,                                   //c_code
    1                                    //s_name
    };
    directives.append(&bde);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //false case
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveExtension bde = {
    sizeof(bde),                         //size
    BrigEDirectiveExtension,             //kind
    10,                                  //c_code
    1                                    //s_name
    };
    directives.append(&bde);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod1.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveArgStart_test) {
  //true case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveArgStart bdas = {
      sizeof(bdas),                         //size
      BrigEDirectiveArgStart,               //kind
      0                                     //c_code
    };
    directives.append(&bdas);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //false case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveArgStart bdas = {
      sizeof(bdas),                         //size
      BrigEDirectiveArgStart,               //kind
      10                                    //c_code
    };
    directives.append(&bdas);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod1.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveArgEnd_test) {
  //true case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveArgEnd bdae = {
      sizeof(bdae),                         //size
      BrigEDirectiveArgEnd,                 //kind
      0                                     //c_code
    };
    directives.append(&bdae);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //false case
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveArgEnd bdae = {
      sizeof(bdae),                         //size
      BrigEDirectiveArgEnd,                 //kind
      10                                    //c_code
    };
    directives.append(&bdae);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod1.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}

TEST(Brig2LLVMTest, BrigBlockMethodValidTest) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("rti"));
    strings.append(std::string("this is a string"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigBlockStart bst = {
      sizeof(bst),
      BrigEDirectiveBlockStart,   // kind
      0,  // c_code
      0   // s_name
    };
    directives.append(&bst);
    BrigBlockString bstr = {
      sizeof(bstr),
      BrigEDirectiveBlockString,   // kind
      4   // s_name
    };
    directives.append(&bstr);
    BrigBlockNumeric bnu = {
      sizeof(bnu),
      BrigEDirectiveBlockNumeric,   // kind
      Brigb16,   //type
      4,
      { { 0 } }
    };
    bnu.u16[0] = 255;
    bnu.u16[1] = 23;
    bnu.u16[2] = 10;
    bnu.u16[3] = 23;
    directives.append(&bnu);
    BrigBlockEnd bend = {
      sizeof(bend),
      BrigEDirectiveBlockEnd   // kind
    };
    directives.append(&bend);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
}

TEST(Brig2LLVMTest, BrigDirectiveBlockStart_invalid) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("wrong"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigBlockStart bst = {
      sizeof(bst),
      BrigEDirectiveBlockStart,   // kind
      0,  // c_code
      0   // s_name
    };
    directives.append(&bst);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid s_name, should be either debug or rti")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveBlockNumeric_invalid) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigBlockNumeric bnu = {
      sizeof(bnu),
      BrigEDirectiveBlockNumeric,   // kind
      Brigb128,   //type
      4,
      { { 0 } }
    };
    directives.append(&bnu);
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type, must be b1, b8, b16, b32, or b64")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveBlockNumeric_invalid2) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigBlockNumeric bnu = {
      sizeof(bnu),
      BrigEDirectiveBlockNumeric,   // kind
      Brigb64,   //type
      4,
      { { 0 } }
    };
    directives.append(&bnu);
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Directive size too small for elementCount")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveBlockString_invalid) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigBlockString bstr = {
      sizeof(bstr),
      BrigEDirectiveBlockString,   // kind
      4   // s_name
    };
    directives.append(&bstr);
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    errMsgOut.flush();
    EXPECT_FALSE(mod.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}

TEST(Brig2LLVMTest, validateBrigDirectivePad) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig Pad"));
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectivePad bdp = {
      sizeof(bdp), //uint16_t size;
      BrigEDirectivePad, //uint16_t kind;
    };
    directives.append(&bdp);
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;
    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
}
TEST(Brig2LLVMTest, validateBrigDirectivePragma) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig Pragma"));
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);


    BrigDirectivePragma bdp = {
      sizeof(bdp), //uint16_t size;
      BrigEDirectivePragma, //uint16_t kind;
      0, //BrigcOffset32_t c_code;
      0 //BrigsOffset32_t s_name;
    };
    directives.append(&bdp);
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;
    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig Pragma"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectivePragma bdp = {
      sizeof(bdp), //uint16_t size;
      BrigEDirectivePragma, //uint16_t kind;
      10, //BrigcOffset32_t c_code; correct: 0
      0 //BrigsOffset32_t s_name; correct: 0
    };
    directives.append(&bdp);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("// content of brig Pragma"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectivePragma bdp = {
      sizeof(bdp), //uint16_t size;
      BrigEDirectivePragma, //uint16_t kind;
      0, //BrigcOffset32_t c_code; correct: 0
      100 //BrigsOffset32_t s_name; correct: 0
    };
    directives.append(&bdp);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}
TEST(Brig2LLVMTest, BrigDirectiveImage_test) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&packeed_ops"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigSymbolCommon s = {
      0,             // c_code
      BrigArgSpace,  // storageClass
      BrigNone,      // attribute
      0,             // reserved
      0,             // symbolModifier
      0,             // dim
      0,            // s_name
      Brigf32,       // type
      1,            // align
    };
    BrigDirectiveImage bdi = {
      sizeof(bdi),
      BrigEDirectiveImage,
      s,
      1,
      1,
      1,
      0,
      0,
      0
    };
    directives.append(&bdi);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveImage bdi = {
      sizeof(bdi),
      BrigEDirectiveImage,
      s,
      1,
      1,
      1,
      2,
      0,
      0
    };
    directives.append(&bdi);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "depth value is wrong for 1DA and 2DA images")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveSampler_test) {
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      0,
      0,
      0,
      0,
      0
    };
    directives.append(&bds);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      20,
      0,
      0,
      0,
      0
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
      "Invalid filter")));
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      0,
      20,
      0,
      0,
      0
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid boundaryU")));
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      0,
      0,
      20,
      0,
      0
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid boundaryV")));
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      0,
      0,
      0,
      20,
      0
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid boundaryW")));
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
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
    BrigDirectiveSampler bds = {
      sizeof(bds),
      BrigEDirectiveSampler,
      s,
      1,
      0,
      0,
      0,
      0,
      0,
      20
    };
    directives.append(&bds);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "The value of reserved1 must be zero")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveLabel_test) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&return_true"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveLabel bdl = {
      sizeof(bdl),
      BrigEDirectiveLabel,
      0,
      0
    };
    directives.append(&bdl);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveLabel bdl = {
      sizeof(bdl),
      BrigEDirectiveLabel,
      1,
      0
    };
    directives.append(&bdl);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&return_true"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveLabel bdl = {
      sizeof(bdl),
      BrigEDirectiveLabel,
      0,
      -1
    };
    directives.append(&bdl);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
  }
}
TEST(Brig2LLVMTest, BrigDirectiveLabelList_test) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&return_true"));
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveLabelList bdll = {
      sizeof(bdll),
      BrigEDirectiveLabelList,
      0,
      0,
      { 0 }
    };
    directives.append(&bdll);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::Buffer strings;
    hsa::brig::Buffer directives;
    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveLabelList bdll = {
      sizeof(bdll),
      BrigEDirectiveLabelList,
      1,
      0,
      { 0 }
    };
    directives.append(&bdll);
    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}

TEST(Brig2LLVMTest, BrigDirectiveLabelInit) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("Label1"));
    strings.append(std::string("Label2"));
    strings.append(std::string("Label3"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    size_t arraySize =
      sizeof(BrigDirectiveLabelInit) + (3 - 1) * sizeof(uint32_t);
    uint8_t *array = new uint8_t[arraySize];
    BrigDirectiveLabelInit *bdli =
      reinterpret_cast<BrigDirectiveLabelInit *>(array);
    bdli->size = arraySize;
    bdli->kind = BrigEDirectiveLabelInit;
    bdli->c_code = 0;
    bdli->elementCount = 3;
    bdli->d_labels[0] = 44;
    bdli->d_labels[1] = 56;
    bdli->d_labels[2] = 68;
    directives.append(bdli);
    delete[] array;
    BrigDirectiveLabel bdl1 = {
      sizeof(bdl1), //uint16_t size;
      BrigEDirectiveLabel, //uint16_t kind;
      0,
      0,
    };
    directives.append(&bdl1);
    BrigDirectiveLabel bdl2 = {
      sizeof(bdl2), //uint16_t size;
      BrigEDirectiveLabel, //uint16_t kind;
      0,
      7,
    };
    directives.append(&bdl2);
    BrigDirectiveLabel bdl3 = {
      sizeof(bdl3), //uint16_t size;
      BrigEDirectiveLabel, //uint16_t kind;
      0,
      14,
    };
    directives.append(&bdl3);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    size_t arraySize =
      sizeof(BrigDirectiveLabelInit) + (2 - 1) * sizeof(uint32_t);
    uint8_t *array = new uint8_t[arraySize];
    BrigDirectiveLabelInit *bdli =
      reinterpret_cast<BrigDirectiveLabelInit *>(array);
    bdli->size = arraySize;
    bdli->kind = BrigEDirectiveLabelInit;
    bdli->c_code = 0;
    bdli->elementCount = 2;
    bdli->d_labels[0] = directives.size() + arraySize;
    bdli->d_labels[1] = bdli->d_labels[0] + sizeof(BrigDirectivePad);
    directives.append(bdli);
    delete[] array;
    BrigDirectivePad bdp = {
      sizeof(bdp), //uint16_t size;
      BrigEDirectivePad, //uint16_t kind;
    };
    directives.append(&bdp);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "d_labels offset is wrong, not a BrigDirectiveLabel")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "d_labels past the directives section")));
  }
}

TEST(Brig2LLVMTest, UniqueString) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("Foo"));
    strings.append(std::string("Foo"));
    strings.append_char('a');
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Duplicate string detected")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "String not null terminated")));
  }
}

TEST(Brig2LLVMTest, validateBrigDirectiveInit) {
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectivePad bdp = {
      sizeof(bdp),
      BrigEDirectivePad
    };
    directives.append(&bdp);

    //BrigDirectiveInit::initializationData type:uint8_t
    uint8_t values[16] = {
      //elementCount = 9, allocate 16 byte memory
        1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t array[sizeof(BrigDirectiveInit) +
                  sizeof(values) - sizeof(uint64_t)];
    BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

    bdi->size = sizeof(array);
    bdi->kind = BrigEDirectiveInit;
    bdi->c_code = 0;
    bdi->elementCount = 9;
    bdi->type = Brigb8;
    bdi->reserved = 0;

    for (size_t i = 0; i < sizeof(values) / sizeof(uint8_t); ++i)
        bdi->initializationData.u8[i] = values[i];
    directives.append(bdi);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    //BrigDirectiveInit::initializationData type:uint8_t
    uint8_t values[16] = {
      //elementCount = 9, allocate 16 byte memory
        1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t array[sizeof(BrigDirectiveInit) +
                  sizeof(values) - sizeof(uint64_t)];
    BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

    bdi->size = sizeof(array);
    bdi->kind = BrigEDirectiveInit;
    bdi->c_code = 0;
    bdi->elementCount = 9;
    bdi->type = Brigb8;
    bdi->reserved = 1;

    for (size_t i = 0; i < sizeof(values) / sizeof(uint8_t); ++i)
        bdi->initializationData.u8[i] = values[i];
    directives.append(bdi);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Improperly aligned directive")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Reserved not zero")));
  }
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectivePad bdp = {
      sizeof(bdp),
      BrigEDirectivePad
    };
    directives.append(&bdp);

    //BrigDirectiveInit::initializationData type:uint8_t
    uint8_t values[16] = {
      //elementCount = 9, allocate 16 byte memory
        1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t array[sizeof(BrigDirectiveInit) +
                  sizeof(values) - sizeof(uint64_t)];
    BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

    bdi->size = sizeof(array);
    bdi->kind = BrigEDirectiveInit;
    bdi->c_code = 0;
    bdi->elementCount = 9;
    bdi->type = Brigf32;
    bdi->reserved = 0;

    for (size_t i = 0; i < sizeof(values) / sizeof(uint8_t); ++i)
        bdi->initializationData.u8[i] = values[i];
    directives.append(bdi);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type, must be b1, b8, b16, b32, b64, or b128")));
  }
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectivePad bdp = {
      sizeof(bdp),
      BrigEDirectivePad
    };
    directives.append(&bdp);

    //BrigDirectiveInit::initializationData type:uint8_t
    uint8_t values[16] = {
      //elementCount = 9, allocate 16 byte memory
        1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t array[sizeof(BrigDirectiveInit) +
                  sizeof(values) - sizeof(uint64_t)];
    BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

    bdi->size = sizeof(array);
    bdi->kind = BrigEDirectiveInit;
    bdi->c_code = 0;
    bdi->elementCount = 100;
    bdi->type = Brigb8;
    bdi->reserved = 0;

    for (size_t i = 0; i < sizeof(values) / sizeof(uint8_t); ++i)
        bdi->initializationData.u8[i] = values[i];
    directives.append(bdi);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Directive size too small for elementCount")));
  }
}

TEST(Brig2LLVMTest, validateBrigDirectiveProto) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveSignature::BrigProtoType args[] = {
      // type, align, hasDim, dim
      {Brigu32, 1, 0, 0},
      {Brigu32, 1, 0, 0}
    };
    appendBrigDirectiveProto(directives,
                             0,  // c_code
                             0,  // s_name
                             0,  // fbarCount
                             0,  // reserved
                             1,  // outCount
                             1,  // inCount
                             &args[0],
                             &args[2]);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveSignature::BrigProtoType args[] = {
      // type, align, hasDim, dim
      {Brigu32, 1, 0, 0},
      {Brigu32, 1, 0, 0}
    };
    appendBrigDirectiveProto(directives,
                             20,  // c_code
                             0,  // s_name
                             0,  // fbarCount
                             1,  // reserved
                             1,  // outCount
                             1,  // inCount
                             &args[0],
                             &args[2]);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "s_name past the strings section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Reserved not zero")));
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&get_global_id"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);
    BrigDirectiveSignature::BrigProtoType args[] = {
      // type, align, hasDim, dim
      {Brigf64x2 + 1, 1, 0, 0},
      {Brigf64x2, 1, 1, 0}
    };
    appendBrigDirectiveProto(directives,
                             0,  // c_code
                             0,  // s_name
                             0,  // fbarCount
                             0,  // reserved
                             1,  // outCount
                             1,  // inCount
                             &args[0],
                             &args[2]);

    hsa::brig::Buffer code;
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "dimension not set when hasDim is 1")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstBase) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstBase bcb = {
      sizeof(bcb),
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
    };
    code.append(&bcb);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstBase bcb = {
      sizeof(bcb),
      BrigEInstBase,
      BrigFbarInitSizeKnown + 1,
      Brigf64x2 + 1,
      BrigPackPsat + 1,
      {20, 0, 0, 0, 0}
    };
    code.append(&bcb);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstAtomic) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomic bca = {
      sizeof(bca),
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquire
    };
    code.append(&bca);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomic bca = {
      sizeof(bca),
      BrigEInstAtomic,
      BrigLd,
      Brigf64x2 + 1,
      BrigPackPsat,
      {20, 0, 0, 0, 0},
      BrigAtomicSub + 1,
      BrigFlatSpace,
      BrigDep
    };
    code.append(&bca);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode, should be either BrigAtomic or BrigAtomicNoRet")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid atomicOperation")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid storage class, can be global, group, private, kernarg, "
    "readonly, spill, or arg")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid memorySemantic, can be BrigAcquire, BrigAcquireRelease, "
    "BrigParAcquireRelease")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstAtomicImage) {
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomicImage biai = {
      sizeof(biai),
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      0,
      {0, 0, 0, 0, 0},
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquire,
      0
    };
    code.append(&biai);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomicImage biai = {
      sizeof(biai),
      BrigEInstAtomicImage,
      BrigLd,
      Brigf64x2 + 1,
      0,
      {20, 0, 0, 0, 0},
      BrigAtomicSub + 1,
      BrigFlatSpace,
      BrigDep,
      Briggeom_2da + 1
    };
    code.append(&biai);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode, should be either BrigAtomicImage "
    "or BrigAtomicNoRetImage")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid atomicOperation")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid storage class, must be global")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid memorySemantic, can be BrigAcquire, BrigAcquireRelease, "
    "BrigParAcquireRelease")));
  }
}
TEST(Brig2LLVMTest, validateBrigInstRead) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstRead bir = {
      sizeof(bir),
      BrigEInstRead,
      BrigRdImage,
      {0, 0, 0, 0, 0},
      0,
      0,
      0,
      0,
      0
    };
    code.append(&bir);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstRead bir = {
      sizeof(bir),
      BrigEInstRead,
      BrigRdImage + 1,
      {20, 0, 0, 0, 0},
      Briggeom_2da + 1,
      Brigf64x2 + 1,
      Brigf64x2 + 1,
      BrigPackPsat + 1,
      1
    };
    code.append(&bir);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type of image geometry")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstMod) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      0,
      0,
      0,
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      BrigFbarInitSizeKnown + 1,
      Brigf64x2 + 1,
      BrigPackPsat + 1,
      {20, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      0,
      0,
      0,
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 1, 0, 0}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid floatOrInt")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      0,
      0,
      0,
      {0, 0, 0, 0, 0},
      {1, 0, 1, 0, 0, 0, 0}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid hi")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      0,
      0,
      0,
      {0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 0}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid ftz")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMod bim = {
      sizeof(bim),
      BrigEInstMod,
      0,
      0,
      0,
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 1}
    };
    code.append(&bim);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reserved")));
  }
}
TEST(Brig2LLVMTest, validateBrigInstCmp) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      BrigCmp,                  //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 0, 0, 0},    //aluModifier
      0,                        //comparisonOperator
      0,                        //sourceType
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      BrigCmp + 1,              //opcode
      Brigf64x2 + 1,            //type
      BrigPackPsat + 1,         //packing
      {20, 0, 0, 0, 0},         //o_operands[5]
      {0, 0, 0, 0, 0, 0, 0},    //aluModifier
      BrigSgtu + 1,             //comparisonOperator;
      Brigf64x2 + 1,            //sourceType
      1                         //reserved
    };
    code.append(&bic);
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid comparisonOperator")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid sourceType")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reserved")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 1, 0, 0},    //aluModifier
      0,                        //comparisonOperator
      0,                        //sourceType
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid floatOrInt")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {1, 0, 1, 0, 0, 0, 0},    //aluModifier
      0,                        //comparisonOperator
      0,                        //sourceType
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid hi")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 1, 0, 0, 0},    //aluModifier
      0,                        //comparisonOperator
      0,                        //sourceType
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid ftz")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCmp bic = {
      sizeof(bic),              //size
      BrigEInstCmp,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 0, 0, 1},    //aluModifier
      0,                        //comparisonOperator
      0,                        //sourceType
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reserved")));
  }
}
TEST(Brig2LLVMTest, validateBrigInstImage) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;

    BrigInstImage bii = {
      sizeof(bii),              //size
      BrigEInstImage,           //kind
      BrigRdImage,              //opcode
      {0, 0, 0, 0, 0},          //o_operands[5]
      0,                        //geom
      0,                        //type
      0,                        //stype
      0,                        //packing
      0                         //reserved
    };
    code.append(&bii);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstImage bii = {
      sizeof(bii),              //size
      BrigEInstImage,           //kind
      BrigRdImage + 1,          //opcode
      {20, 0, 0, 0, 0},         //o_operands[5]
      Briggeom_2da + 1,         //geom
      Brigf64x2 + 1,            //type
      Brigf64x2 + 1,            //stype
      BrigPackPsat + 1,         //packing
      1                         //reserved
    };
    code.append(&bii);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type of image geometry")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid stype")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstBar) {
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstBar bib = {
      sizeof(bib),
      BrigEInstBar,
      BrigBarrier,
      Brigu32,
      0,
      {0, 0, 0, 0, 0},
      1
    };
    code.append(&bib);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstBar bib = {
      sizeof(bib),
      BrigEInstBar,
      BrigLd,
      Brigf64x2 + 1,
      0,
      {20, 0, 0, 0, 0},
      8
    };
    code.append(&bib);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode, should be either BrigBarrier, BrigSync or BrigBrn")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid syncFlags, should be either BrigGroupLevel BrigGlobalLevel"
    "or BrigPartialLevel")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstMem) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMem bcm = {
      sizeof(bcm),
      BrigEInstMem,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigGlobalSpace
    };
    code.append(&bcm);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstMem bcm = {
      sizeof(bcm),
      BrigEInstMem,
      BrigFbarInitSizeKnown + 1,
      Brigf64x2 + 1,
      BrigPackPsat + 1,
      {20, 0, 0, 0, 0},
      BrigFlatSpace
    };
    code.append(&bcm);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
  }
}

TEST(Brig2LLVMTest, validateBrigInstCvt) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      BrigCvt,                  //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 0, 0, 0},    //aluModifier
      0,                        //stype
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      BrigCvt + 1,              //opcode
      Brigf64x2 + 1,            //type
      BrigPackPsat + 1,         //packing
      {20, 0, 0, 0, 0},         //o_operands[5]
      {0, 0, 0, 0, 0, 0, 0},    //aluModifier
      Brigf64x2 + 1,            //stype
      1                         //reserved
    };
    code.append(&bic);
    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid packing control")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid stype")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 1, 0, 0},    //aluModifier
      0,                        //stype
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid floatOrInt")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {1, 0, 1, 0, 0, 0, 0},    //aluModifier
      0,                        //stype
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid hi")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 1, 0, 0, 0},    //aluModifier
      0,                        //stype
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid ftz")));
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;

    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstCvt bic = {
      sizeof(bic),              //size
      BrigEInstCvt,             //kind
      0,                        //opcode
      0,                        //type
      0,                        //packing
      {0, 0, 0, 0, 0},          //o_operands[5]
      {0, 0, 0, 0, 0, 0, 1},    //aluModifier
      0,                        //stype
      0                         //reserved
    };
    code.append(&bic);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reserved")));
  }
}
TEST(Brig2LLVMTest, validateBrigInstLdSt) {
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomic bils = {
      sizeof(bils),
      BrigEInstLdSt,
      BrigLd,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      BrigGlobalSpace,
      BrigAcquire,
      0
    };
    code.append(&bils);

    hsa::brig::Buffer operands;

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;
    BrigInstAtomic bils = {
      sizeof(bils),
      BrigEInstLdSt,
      BrigFbarInitSizeKnown + 1,
      Brigf64x2 + 1,
      BrigPackPsat,
      {20, 0, 0, 0, 0},
      BrigFlatSpace + 1,
      BrigDep,
      64
    };
    code.append(&bils);

    hsa::brig::Buffer operands;

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid opcode")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid type")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "o_operands past the operands section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid storage class, can be global, group, "
    "private, kernarg, readonly, spill, arg or flat")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid memorySemantic, can be BrigAcquire, BrigAcquireRelease, "
    "BrigParAcquireRelease")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid equivClass, must less than 64")));
  }
}

TEST(Brig2LLVMTest, validateBrigOperandAddress) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("%input"));
    strings.append(std::string("%an_input"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bds = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        7,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandAddress boa = {
      sizeof(boa),
      BrigEOperandAddress,
      Brigb32,
      0,
      20
    };
    operands.append(&boa);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("%input"));
    strings.append(std::string("%an_input"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bds = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        7,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandAddress boa = {
      sizeof(boa),
      BrigEOperandAddress,
      Brigu32,
      1,
      60
    };
    operands.append(&boa);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invald datatype, should be Brigb32 and Brigb64")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "directive past the directive section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid directive, should point to a BrigDirectiveSymbol")));
  }
}

TEST(Brig2LLVMTest, validateBrigOperandCompound) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bds = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        0,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor = {
      12,
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor);

    BrigOperandAddress boa = {
      16,
      BrigEOperandAddress,
      Brigb32,
      0,
      20
    };
    operands.append(&boa);

    BrigOperandCompound boc = {
      sizeof(boc),
      BrigEOperandCompound,
      Brigb32,
      0,
      20,
      8,
      0
    };
    operands.append(&boc);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$c0"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bds = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        0,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor = {
      12,
      BrigEOperandReg,
      Brigb1,
      0,
      23
    };
    operands.append(&bor);

    BrigOperandAddress boa = {
      16,
      BrigEOperandAddress,
      Brigb64,
      0,
      20
    };
    operands.append(&boa);

    BrigOperandCompound boc = {
      sizeof(boc),
      BrigEOperandCompound,
      Brigu32,
      1,
      120,
      8,
      0
    };
    operands.append(&boc);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invald datatype, should be Brigb32 and Brigb64")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid name, should point to BrigOperandAddress")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid register, the register must be an s or d register")));
  }
  //invalid test about reg past operand section
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bds = {
      40,                              // size
      BrigEDirectiveSymbol,            // kind
      {
        0,                               // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        0,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bds);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor = {
      12,
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor);

    BrigOperandAddress boa = {
      16,
      BrigEOperandAddress,
      Brigb32,
      0,
      20
    };
    operands.append(&boa);

    BrigOperandCompound boc = {
      sizeof(boc),
      BrigEOperandCompound,
      Brigb32,
      0,
      20,
      60,
      0
    };
    operands.append(&boc);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "operands past the operands section")));
  }
}

TEST(Brig2LLVMTest, validateBrigOperandFunctionRef) {
  {
    hsa::brig::StringBuffer strings;
    strings.append("&callee");
    strings.append("&caller");

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFunction callee = {
      sizeof(callee),                 // size
      BrigEDirectiveFunction,         // kind
      0,                              // c_code
      0,                              // s_name
      0,                              // inParamCount
      directives.size() +
      sizeof(callee),                 // d_firstSCopedDirective
      1,                              // operationCount
      directives.size() +
      sizeof(callee),                 // d_nextDirective
      BrigNone,                       // attribute
      0,                              // fbarCount
      0,                              // outParamCount
      0                               // d_firstInParam
    };
    directives.append(&callee);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandFunctionRef bofr = {
      sizeof(bofr),
      BrigEOperandFunctionRef,
      20
    };
    operands.append(&bofr);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append("&callee");
    strings.append("&caller");

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveFunction callee = {
      sizeof(callee),                 // size
      BrigEDirectiveFunction,         // kind
      0,                              // c_code
      0,                              // s_name
      0,                              // inParamCount
      directives.size() +
      sizeof(callee),                 // d_firstSCopedDirective
      1,                              // operationCount
      directives.size() +
      sizeof(callee),                 // d_nextDirective
      BrigNone,                       // attribute
      0,                              // fbarCount
      0,                              // outParamCount
      0                               // d_firstInParam
    };
    directives.append(&callee);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandFunctionRef bofr = {
      sizeof(bofr),
      BrigEOperandFunctionRef,
      70
    };
    operands.append(&bofr);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid directive, should point to a BrigDirectiveFunction or BrigDirectiveSibnature")));
  }
}

TEST(Brig2LLVMTest, validateBrigOperandIndirect) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor = {
      12,
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor);

    BrigOperandIndirect boi = {
      sizeof(boi),
      BrigEOperandIndirect,
      8,               
      Brigb32,
      0,
      0
    };
    operands.append(&boi);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  //invalid test
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));
    strings.append(std::string("%input"));
    strings.append(std::string("$s0"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandReg bor = {
      12,
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor);

    BrigOperandIndirect boi = {
      sizeof(boi),
      BrigEOperandIndirect,
      40,               
      Brigu32,
      1,
      0
    };
    operands.append(&boi);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reg, should be point BrigOprandReg")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invald datatype, should be Brigb32 and Brigb64")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "reserved must be zero")));
  }
}

TEST(Brig2LLVMTest, validateBrigOperandArgumentRef) {
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    BrigDirectiveSymbol bdsy3 = {
      sizeof(bdsy3),                    // size
      BrigEDirectiveSymbol,             // kind
      {
        0,                             // c_code
        BrigArgSpace,                    // storageClass
        BrigNone,                        // attribute
        0,                               // reserved
        0,                               // symbolModifier
        0,                               // dim
        8,                               // s_name
        Brigf32,                         // type
        1                                // align
      },
      0,                               // d_init
      0                                // reserved
    };
    directives.append(&bdsy3);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandArgumentRef boar = {
      sizeof(boar),
      BrigEOperandArgumentRef,
      20                    //arg
    };
    operands.append(&boar);

    hsa::brig::BrigModule mod(strings, directives, code, operands,
                              &llvm::errs());
    EXPECT_TRUE(mod.isValid());
  }
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&callee"));
    strings.append(std::string("%output"));

    hsa::brig::Buffer directives;
    BrigDirectiveVersion bdv = {
      sizeof(bdv),
      BrigEDirectiveVersion,
      0,
      1,
      0,
      BrigELarge,
      BrigEFull,
      BrigENosftz,
      0
    };
    directives.append(&bdv);

    hsa::brig::Buffer code;

    hsa::brig::Buffer operands;
    for(unsigned i = 0; i < 8; ++i) operands.append_char(0);

    BrigOperandArgumentRef boar = {
      sizeof(boar),
      BrigEOperandArgumentRef,
      120                    //arg
    };
    operands.append(&boar);

    std::string errorMsg;
    llvm::raw_string_ostream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod(strings, directives, code, operands, &errMsgOut);
    EXPECT_FALSE(mod.isValid());
    errMsgOut.flush();
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "dir past the directives section")));
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "Invalid reg, should be point BrigDirectiveSymbol")));
  }
}
