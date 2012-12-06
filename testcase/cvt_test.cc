// Copyright 2012 MulticoreWare Inc.

#include "cvt_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestCvt,Cvt)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(cvt_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

  BrigInstCvt get;
  context->get_code(code_offset, &get);

  BrigInstCvt ref = cvt_pair[n].ref;

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

INSTANTIATE_TEST_CASE_P(CodegenTestCvt, TestCvt, testing::Range(0, 62));

}  // namespace brig
}  // namespace hsa
