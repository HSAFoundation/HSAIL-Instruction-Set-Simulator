/* Copyright 2012 <MulticorewareInc> */

// test mock lexer
#include "mock_lexer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tokens.h"


using ::testing::Return;

namespace hsa {
namespace brig {
TEST(MockLexerTest, GetNewToken) {
  MockLexer lexer;

  EXPECT_CALL(lexer, get_next_token())
    .WillOnce(Return(TOKEN_CREGISTER))
    .WillOnce(Return(TOKEN_DREGISTER));

  unsigned int token = lexer.get_next_token();
  EXPECT_EQ(TOKEN_CREGISTER, token);

  token = lexer.get_next_token();
  EXPECT_EQ(TOKEN_DREGISTER, token);  
};
}  // namespace brig
}  // namespace hsa
