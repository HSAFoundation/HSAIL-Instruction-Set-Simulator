/* Copyright 2012 <MulticorewareInc> */
#ifndef INCLUDE_MOCK_LEXER_H_
#define INCLUDE_MOCK_LEXER_H_

#include <string>
#include "gmock/gmock.h"
#include "lexer_interface.h"

namespace hsa {
namespace brig {
class MockLexer: public LexerInterface {
  public:
    MockLexer() {}
    explicit MockLexer(const std::string& s)
    : LexerInterface(s) {}

    MOCK_METHOD0(get_next_token, unsigned int());
    MOCK_METHOD0(get_int_value, int());
    MOCK_METHOD0(get_double_value, double());
    MOCK_METHOD0(get_float_value, float());
    MOCK_METHOD0(get_string_value, std::string());
};


}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_MOCK_LEXER_H_
