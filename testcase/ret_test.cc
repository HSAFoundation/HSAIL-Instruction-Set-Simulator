// Copyright 2012 MulticoreWare Inc.

#include "ret_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestRet,Ret)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(ret_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ret(context));

  BrigInstBase get;
  context->get_code(code_offset, &get);

  BrigInstBase ref = ret_pair[n].ref;

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

INSTANTIATE_TEST_CASE_P(CodegenTest, TestRet, testing::Range(0,1));

TEST_P(TestRetInvalid, RetInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_ret_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ret(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestRetInvalid, testing::Range(0,2));

}  // namespace brig
}  // namespace hsa
