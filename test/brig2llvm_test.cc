// Copyright 2012 MulticoreWare Inc.

#include "gtest/gtest.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig_module.h"
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
      directives.size() + sizeof(get_global_id),  // d_firstScopedDirective
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
    EXPECT_NE(0, codegen.str().size());
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
      directives.size() + sizeof(bdf),  // d_firstScopedDirective
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

TEST(Brig2LLVMTest, Example3){
  {
    hsa::brig::StringBuffer strings;
    strings.append(std::string("&packeed_ops"));
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
      directives.size() + sizeof(bdf),  // d_firstScopedDirective
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
    BrigInstBase abs = {
      sizeof(abs),
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      { 0, 0, 0, 0, 0}
    };
    code.append(&abs);
    BrigInstBase add = {
      sizeof(add),
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      { 0, 0, 0, 0, 0}
    };
    code.append(&add);

    hsa::brig::Buffer operands;
    BrigOperandReg bor_1 = {
      sizeof(bor_1),
      BrigEOperandReg,
      Brigb32,
      0,
      15
    };
    operands.append(&bor_1);
    BrigOperandReg bor_2 = {
      sizeof(bor_2),
      BrigEOperandReg,
      Brigb32,
      0,
      19
    };
    operands.append(&bor_2);
    BrigOperandReg bor_3 = {
      sizeof(bor_3),
      BrigEOperandReg,
      Brigb32,
      0,
      23
    };
    operands.append(&bor_3);
    BrigOperandReg bor_4 = {
      sizeof(bor_4),
      BrigEOperandReg,
      Brigb32,
      0,
      27
    };
    operands.append(&bor_4);

    // hsa::brig::GenLLVM codegen(strings, directives, code, operands);
    // codegen();
    // EXPECT_NE(0, codegen.str().size());
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "declare void @abs_p_s8x4(%struct.regs*, i32, i32)")));
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "declare void @add_pp_sat_u16x2(%struct.regs*, i32, i32, i32)")));
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "define void @pack_ops(<4 x i8> * %x)")));
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "%struct.regs = type { %c_regs, %s_regs, %d_regs, %q_regs, %pc_regs }")));
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "%gpu_reg_p = alloca %struct.regs")));
    // EXPECT_NE(std::string::npos, codegen.str().find(std::string(
    // "ret void")));
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

  // Since the
  uint8_t array[sizeof(BrigDirectiveProto) +
                sizeof(BrigDirectiveProto::BrigProtoType[numArgs - 1])];

  BrigDirectiveProto *bdp =
    reinterpret_cast<BrigDirectiveProto *>(array);

  bdp->size = sizeof(array);
  bdp->kind = BrigEDirectiveProto;
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
}

TEST(Brig2LLVMTest, VarSizeDirective) {
  {
    hsa::brig::Buffer bb;
    BrigDirectiveProto::BrigProtoType args[] = {
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

    EXPECT_EQ(sizeof(BrigDirectiveProto) +
              sizeof(sizeof(BrigDirectiveProto::BrigProtoType)),
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
      96,                                      // d_firstScopedDirective
      1,                                       // operationCount
      96,                                      // d_nextDirective
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

    hsa::brig::BrigModule mod(strings, directives, code, operands, &std::cerr);
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
    std::ostringstream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod2(strings, directives, code, operands, &errMsgOut);
    errorMsg = errMsgOut.str();
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

    hsa::brig::BrigModule mod(strings, directives, code, operands, &std::cerr);
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
    std::ostringstream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errorMsg = errMsgOut.str();
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

    hsa::brig::BrigModule mod(strings, directives, code, operands, &std::cerr);
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
    std::ostringstream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errorMsg = errMsgOut.str();
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

    hsa::brig::BrigModule mod(strings, directives, code, operands, &std::cerr);
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
    std::ostringstream errMsgOut(errorMsg);
    hsa::brig::BrigModule mod1(strings, directives, code, operands, &errMsgOut);
    errorMsg = errMsgOut.str();
    EXPECT_FALSE(mod1.isValid());
    EXPECT_NE(std::string::npos, errorMsg.find(std::string(
    "c_code past the code section")));
  }
}
