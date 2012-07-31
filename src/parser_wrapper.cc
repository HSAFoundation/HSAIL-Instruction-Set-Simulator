/* Copyright 2012 <MulticorewareInc> */

#include "parser_wrapper.h"
#include "parser.h"
#include "tokens.h"

namespace hsa {
namespace brig {
  void Parser::set_source_string(const std::string& src) {
    lexer->set_source_string(src);
  };

  // scan source for symbols
  void Parser::scan_symbols(void) {
    lexer->restart();
    context->clear_all_buffers();
    unsigned int token = lexer->get_next_token();

    while (token) {
      if ((token == TOKEN_GLOBAL_IDENTIFIER) ||
          (token == TOKEN_LOCAL_IDENTIFIER) ||
          (GetTokenType(token) == REGISTER) ||
          (token == TOKEN_LABEL)) {
      int offset = context->add_symbol(lexer->get_string_value());
      }
      token = lexer->get_next_token();
    }
  }

  int Parser::parse(void) {
    return 1;
  }
}  // namespace brig
}  // namespace hsa

