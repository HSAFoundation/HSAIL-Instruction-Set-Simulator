// Copyright 2012 MulticoreWare Inc.

#include "location_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestLocation,Location)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(location_pair[n].str);
 
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Location(context));

  BrigDirectiveLoc get;
  context->get_directive(directive_offset, &get);

  BrigDirectiveLoc ref = location_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.sourceFile,get.sourceFile);
  EXPECT_EQ(ref.sourceLine,get.sourceLine);
  EXPECT_EQ(ref.sourceColumn,get.sourceColumn);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestLocation, testing::Range(0,1));

TEST_P(TestLocationInvalid, LocationInvalid)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_location_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Location(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestLocationInvalid, testing::Range(0,5));

}  // namespace brig
}  // namespace hsa
