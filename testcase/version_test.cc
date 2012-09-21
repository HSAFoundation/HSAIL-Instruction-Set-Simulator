// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"
#include "version_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(CodegenTestVersion, Version) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_version[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  uint32_t curr_d_offset = context->get_directive_offset();

  BrigDirectiveVersion ref = outputarray_version[n];

  BrigDirectiveVersion get;
  context->get_directive(curr_d_offset-sizeof(get), &get);

  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);
     
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestVersion,CodegenTestVersion,testing::Range(0,34));

TEST_P(TestVersionFalseInput, VersionFalseInput) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_version_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Version(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestVersionFalse,
                        TestVersionFalseInput,
                        testing::Range(0,7));

}  // namespace brig
}  // namespace hsa
