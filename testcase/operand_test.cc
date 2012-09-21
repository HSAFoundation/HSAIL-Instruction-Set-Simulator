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
#include "operand_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

// ------------------ BASIC LEXER TESTS -----------------
TEST(LexTest, CRegisterInvalidInput) {
  std::string input("$c20");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_CREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$c");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$c8");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, SRegisterInvalidInput) {
  std::string input("$s17");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$s");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$c0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, DRegisterInvalidInput) {
  std::string input("$d8");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$d");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$c0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, QRegisterInvalidInput) {
  std::string input("$q20");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_QREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$q");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$q8");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, LabelInvalidInput) {
  std::string input("@123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@ _GO_TO_THIS");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@@");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, LocalIdInvalidInput) {
  std::string input("%123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("% _Test_id_123");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%test@123");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, GlobalIdInvalidInput) {
  std::string input("&123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("& _Test_id_123");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&test@123");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST_P(IntegerConstant, IntegerConstants) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_integerconstant[n]);

  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(output_integerconstant[n], context->token_value.int_val);

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, IntegerConstant, testing::Range(0, 21));

TEST_P(CRegisterValidInput, CRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_cregister[n]);
  EXPECT_EQ(TOKEN_CREGISTER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, CRegisterValidInput, testing::Range(0, 8));

TEST_P(SRegisterValidInput, SRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_sregister[n]);
  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, SRegisterValidInput, testing::Range(0, 16));

TEST_P(DRegisterValidInput, DRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_dregister[n]);
  EXPECT_EQ(TOKEN_DREGISTER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, DRegisterValidInput, testing::Range(0, 8));

TEST_P(QRegisterValidInput, QRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_qregister[n]);
  EXPECT_EQ(TOKEN_QREGISTER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, QRegisterValidInput, testing::Range(0, 8));

TEST_P(LabelValidInput, Label) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_label[n]);
  EXPECT_EQ(TOKEN_LABEL, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, LabelValidInput, testing::Range(0, 9));

TEST_P(LocalIdValidInput, LocalId) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_localid[n]);
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, LocalIdValidInput, testing::Range(0, 9));

TEST_P(GlobalIdValidInput, GlobalId) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_globalid[n]);
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(LexTest, GlobalIdValidInput, testing::Range(0, 9));

}  // namespace brig
}  // namespace hsa
