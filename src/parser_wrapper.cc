/* Copyright 2012 <MulticorewareInc> */

#include "parser_wrapper.h"
#include "parser.h"
#include "tokens.h"
extern TerminalType token_type;

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
          (token_type == REGISTER) ||
          (token == TOKEN_LABEL)) {
      int offset = context->add_symbol(lexer->get_string_value());
      }
      token = lexer->get_next_token();
    }
  }

  int Parser::parse(void) {
    // first scan for all symbols in source
    scan_symbols();

    // restart lexer
    lexer->restart();

    // begin parse src
    unsigned int token = lexer->get_next_token();

    while (token) {
      if (token == VERSION) {
        if (Program(token, context))
          return 1;
      }
      token = lexer->get_next_token();
    }
    return 0;
  }
}  // namespace brig
}  // namespace hsa

