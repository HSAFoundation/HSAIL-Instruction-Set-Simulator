// Copyright 2012 MulticoreWare Inc.

#include "bar_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestBar,Bar)
{

  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(bar_pair[n].str);

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Bar(context));

  BrigInstBar get;
  context->get_code(code_offset, &get);

  BrigInstBar ref = bar_pair[n].ref;
 

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
  EXPECT_EQ(ref.syncFlags, get.syncFlags);

  BrigOperandImmed getImmed;
  context->get_operand(operand_offset, &getImmed);
  if (n<=3){
    EXPECT_EQ(immed_size, getImmed.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed.kind);
    EXPECT_EQ(Brigb32, getImmed.type);
    EXPECT_EQ(0, getImmed.reserved);
    EXPECT_EQ(0, getImmed.bits.u);
  }else if (n>3 and n< 8){
    EXPECT_EQ(immed_size, getImmed.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed.kind);
    EXPECT_EQ(Brigb32, getImmed.type);
    EXPECT_EQ(0, getImmed.reserved);
    EXPECT_EQ(64, getImmed.bits.u);
  }else if (n>=8){
    EXPECT_EQ(immed_size, getImmed.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed.kind);
    EXPECT_EQ(Brigb32, getImmed.type);
    EXPECT_EQ(0, getImmed.reserved);
    EXPECT_EQ(0, getImmed.bits.u);
  }
  delete lexer;

}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestBar, testing::Range(0,12));

TEST_P(TestBarInvalidInput, BarInvalidInput) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_bar_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Sync(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestBarInvalidInput, testing::Range(0,4));

}  // namespace brig
}  // namespace hsa
