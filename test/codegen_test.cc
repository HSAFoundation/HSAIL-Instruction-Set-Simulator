// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"
#include "../include/brig.h"

namespace hsa {
namespace brig {
Context* context = new Context();

TEST(CodegenTest, AlignmentCheck) {
  // Try the situation in PRM 20.2 (pg. 226)
 
  // use a new context object to ensure the problem happen 
  // since if at beginning the offset is a multiple of 4 but not a multiple of 8
  // then appending a 4-byte aligned item will lead to a multiple-of-8 offset
  
  Context* context1 = new Context();
  
  // First append a 4-byte aligned item BrigBlockStart
  int old_offset;
  int curr_offset = context1->get_directive_offset();
  
  BrigBlockStart bbs = {
    12,  // size
    BrigEDirectiveBlockStart,  // kind
    0,  // c_code
    0,  // s_name;
  };
  
  context1->append_d(&bbs);    // append_directive
  old_offset = curr_offset;
  curr_offset = context1->get_directive_offset();
  
  EXPECT_EQ(0, curr_offset%4);
  EXPECT_EQ(BrigEAlignment_4, Context::alignment_check(bbs));
  
  // Next append a 8-byte aligned item  such as BrigBlockNumeric
  BrigBlockNumeric bbn = {
    16,  // size
    BrigEDirectiveBlockNumeric,  // kind
    Brigb64,  // type
    1,  // elementCount
    1,  // u64
  };
  
  context1->append_d(&bbn);  
  old_offset = curr_offset;
  curr_offset = context1->get_directive_offset();

  EXPECT_EQ(BrigEAlignment_8, Context::alignment_check(bbn));
  // this is a 8-byte aligned item and has a size of multiple of 8.
  // so the offset after appending this item should be a multiple of 8.
  EXPECT_EQ(0, curr_offset%8);  
  
  delete context1;
}

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

TEST(CodegenTest, RegisterOperandCodeGen) {
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
