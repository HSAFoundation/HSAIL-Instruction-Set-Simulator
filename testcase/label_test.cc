// Copyright 2012 MulticoreWare Inc.

#include "label_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestLabel,Label)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(label_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Label(context));

  BrigDirectiveLabel get;
  context->get_directive(directive_offset, &get);

  BrigDirectiveLabel ref = label_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.s_name, get.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestLabel, testing::Range(0,2));

TEST_P(TestLabelInvalid, LabelInvalid) 
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_label_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Label(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestLabelInvalid, testing::Range(0,3));

}  // namespace brig
}  // namespace hsa
