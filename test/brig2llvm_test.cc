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
  for(size_t i = 0; i < numArgs; ++i)
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
                             0, // c_code
                             0, // s_name
                             0, // fbarCount
                             0, // reserved
                             1, // outCount
                             1, // inCount
                             &args[0],
                             &args[2]);

    EXPECT_EQ(sizeof(BrigDirectiveProto) +
              sizeof(sizeof(BrigDirectiveProto::BrigProtoType)),
              bb.size());
  }
}
