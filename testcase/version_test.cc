// Copyright 2012 MulticoreWare Inc.

#include "version_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestVersion, Version) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(version_pair[n].str);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  BrigDirectiveVersion ref = version_pair[n].ref;

  BrigDirectiveVersion get;
  context->get_directive(directive_offset, &get);

  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
     
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestVersion, testing::Range(0,34));

TEST_P(TestVersionInvalid, VersionInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_version_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Version(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestVersionInvalid, testing::Range(0,7));

}  // namespace brig
}  // namespace hsa
