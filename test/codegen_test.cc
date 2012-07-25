// Copyright 2012 MulticoreWare Inc.

#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"
#include "../include/brig.h"

namespace hsa {
namespace brig {
Context* context = new Context();

TEST(CodegenTest, VersionCodeGen) {
  // reference struct
  BrigDirectiveVersion ref = {
    sizeof(ref),
    BrigEDirectiveVersion,
    0,  // unknown c_code
    1,  // major
    0,  // minor
    BrigESmall,  // machine
    BrigEFull,  // profile
    BrigENosftz,  // ftz
    0   // reserved
    };

  std::string input("version 1:0;");
  ref.major = 1;
  ref.minor = 0;

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));


  // get structure back
  BrigDirectiveVersion get;
  context->get_d<BrigDirectiveVersion>(&get);
  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

      /* TEST 2 */
  input.assign("version 2:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;

  // get structure back
  context->get_d(&get);

  // compare two structs

  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

      /* TEST 3, Multi Target */
  input.assign("version 2:0:$large, $reduced, $sftz;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;
  ref.profile = BrigEReduced;
  ref.ftz = BrigESftz;

  // get structure back
  context->get_d<BrigDirectiveVersion>(&get);

  // compare two structs

  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);
}

TEST(CodegenTest, RegisterCodeGen) {
  std::string input("$d7");  // register
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Operand(yylex(), context));
  
  // reference struct
  BrigOperandReg ref = {
    sizeof(ref),      // size
    BrigEOperandReg,  // kind
    Brigb64,          // type
    0,                // reserved
    0                // name -> offset in .strings section (unknown now)
  };
  
  // get structure from context and compare
  BrigOperandReg get;
  context->get_o<BrigOperandReg>(&get);
  
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);
  
}

}  // namespace brig
}  // namespace hsa
