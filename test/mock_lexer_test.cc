/* Copyright 2012 <MulticorewareInc> */

// test mock lexer
#include "mock_lexer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tokens.h"


using ::testing::AtLeast;
using ::testing::Return;

namespace hsa {
namespace brig {
TEST(MockLexerTest, GetNewToken) {
  std::string input("$c1");

  MockLexer lexer;
  lexer.set_source_string(input);

  EXPECT_CALL(lexer, get_next_token())
    .Times(AtLeast(1))
    .WillOnce(Return(TOKEN_CREGISTER));

  int token = lexer.get_next_token();
  EXPECT_EQ(TOKEN_CREGISTER, token);
};
}  // namespace brig
}  // namespace hsa
