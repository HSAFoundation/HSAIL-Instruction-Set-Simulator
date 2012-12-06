// Copyright 2012 MulticoreWare Inc.

#include "branch_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestBrn,Brn)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(branch_brn_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  BrigInstBase get;
  context->get_code(code_offset, &get);

  BrigInstBase ref = branch_brn_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestBrn, testing::Range(0,7));

TEST_P(TestCbr, Cbr)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(branch_cbr_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  BrigInstBase get;
  context->get_code(code_offset, &get);

  BrigInstBase ref = branch_cbr_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestCbr, testing::Range(0,9));

TEST_P(TestBrnFbr,BrnFbr)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(branch_brn_fbr_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  BrigInstMod get;
  context->get_code(code_offset, &get);

  BrigInstMod ref = branch_brn_fbr_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestBrnFbr, testing::Range(0,7));

TEST_P(TestCbrFbr,CbrFbr)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(branch_cbr_fbr_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  BrigInstMod get;
  context->get_code(code_offset, &get);

  BrigInstMod ref = branch_cbr_fbr_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestCbrFbr, testing::Range(0,9));

}  // namespace brig
}  // namespace hsa
