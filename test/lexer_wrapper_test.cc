// Copyright 2012 MulticoreWare Inc.
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer_wrapper.h"

namespace hsa {
namespace brig {
TEST(LexerWrapperTest, TestLexWrapper) {
  Lexer* lexer = new Lexer();
  std::string input("$c1");
  lexer->set_source_string(input);

  int token = lexer->get_next_token();

  EXPECT_GE(token, 0);
  EXPECT_EQ(TOKEN_CREGISTER, token);

  std::string token_str = lexer->get_string_value();
  EXPECT_STREQ("$c1", token_str.c_str());
};

TEST(LexerWrapperTest, GetIntValFromLexer) {
  std::string input("10");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, lexer->get_int_value());
}

TEST(LexerWrapperTest, GetFloatValFromLexer) {
  std::string input("10.0f");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.0, lexer->get_float_value());
}

TEST(LexerWrapperTest, GetDoubleValFromLexer) {
  std::string input("10.0l");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_DOUBLE_CONSTANT);
  EXPECT_EQ(10.0, lexer->get_double_value());
}

TEST(LexerWrapperTest, ResetLexer) {
  std::string input("$c1 10 10.5f");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, lexer->get_int_value());

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.5, lexer->get_float_value());

  // restart from begin
  lexer->restart();
  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);
}

}  // namespace brig
}  // namespace hsa
