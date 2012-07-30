/* Copyright 2012 <MulticorewareInc> */

#include <string>
#include <iostream>
#include "parser_wrapper.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_lexer.h"
#include "tokens.h"

using ::testing::Return;

namespace hsa {
namespace brig {
TEST(ParserWrapperTest, ScanSymbolsWithParser) {
  std::string input("version 1:0:$large;\n");
  input.append("global_f32 &x = 2;\n");
  input.append("function &test()() {\n");
  input.append("{arg_u32 %z;}\n");
  input.append(" }; \n");

  Parser* parser = new Parser(input);
  parser->scan_symbols();

  // read symbols
  Context* context = parser->get_context();

  // Print out string buffer content:
  unsigned int index = 0;
  std::string temp;
  std::cout << "Buffer content: " << std::endl;
  while (index < context->get_string_offset()) {
    temp = context->get_string(index);
    std::cout << "Index " << index << ": " << temp << std::endl;
    index+=temp.length()+1;
  }
};

TEST(ParserWrapperTest, UseMockLexer) {
  MockLexer lexer;
  Parser* parser = new Parser(&lexer);

  EXPECT_CALL(lexer, get_next_token())
    .WillOnce(Return(TOKEN_GLOBAL_IDENTIFIER))
    .WillOnce(Return(TOKEN_LABEL))
    .WillOnce(Return(TOKEN_LOCAL_IDENTIFIER))
    .WillOnce(Return(0));

  EXPECT_CALL(lexer, get_string_value())
    .WillOnce(Return(std::string("&x")))
    .WillOnce(Return(std::string("@label")))
    .WillOnce(Return(std::string("%local")));

  parser->scan_symbols();

  // read symbols
  Context* context = parser->get_context();

  // Print out string buffer content:
  unsigned int index = 0;
  std::string temp;
  std::cout << "Buffer content: " << std::endl;
  while (index < context->get_string_offset()) {
    temp = context->get_string(index);
    std::cout << "Index " << index << ": " << temp << std::endl;
    index+=temp.length()+1;
  }
};

}  // namespace brig
}  // namespace hsa
