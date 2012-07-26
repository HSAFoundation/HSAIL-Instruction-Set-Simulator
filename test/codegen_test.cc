// Copyright 2012 MulticoreWare Inc.

#include <iostream>
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
  
  int curr_d_offset = context->get_directive_offset();  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));
  int new_d_offset = context->get_directive_offset(); // after append BrigDirectiveVersion
   EXPECT_EQ(new_d_offset, curr_d_offset + sizeof(BrigDirectiveVersion));
  // get structure back
  BrigDirectiveVersion get;
  context->get_d<BrigDirectiveVersion>(&get, curr_d_offset);
  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

      /* TEST 2 */
  input.assign("version 2:0:$large;");
  
  curr_d_offset = context->get_directive_offset();
  
  
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));
 
  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;

  // get structure back
  context->get_d(&get, curr_d_offset);

  // compare two structs

  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

      /* TEST 3, Multi Target */
  input.assign("version 2:0:$large, $reduced, $sftz;");
  curr_d_offset = context->get_directive_offset();
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;
  ref.profile = BrigEReduced;
  ref.ftz = BrigESftz;

  // get structure back
  context->get_d<BrigDirectiveVersion>(&get, curr_d_offset);

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
  
  int curr_o_offset = context->get_operand_offset();  // current operand offset value
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
  context->get_o<BrigOperandReg>(&get, curr_o_offset);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);
}

}  // namespace brig
}  // namespace hsa
