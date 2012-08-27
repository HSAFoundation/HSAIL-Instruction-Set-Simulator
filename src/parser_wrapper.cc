/* Copyright 2012 <MulticorewareInc> */

#include "parser_wrapper.h"
#include "parser.h"
#include "tokens.h"


namespace hsa {
namespace brig {
  void Parser::set_source_string(const std::string& src) {
    lexer->set_source_string(src);
  }

  // scan source for symbols
  void Parser::scan_symbols(void) {
    lexer->restart();
    context->clear_all_buffers();
    unsigned int token = lexer->get_next_token();

    while (token) {
      if ((token == TOKEN_GLOBAL_IDENTIFIER) ||
          (token == TOKEN_LOCAL_IDENTIFIER) ||
          (context->token_type == REGISTER) ||
          (token == TOKEN_LABEL)) {
        context->add_symbol(std::string(context->token_value.string_val));
      }
      token = lexer->get_next_token();
    }
  }

  int Parser::parse(void) {
    // first scan for all symbols in source
    context->clear_context();
    scan_symbols();

    // restart lexer
    lexer->restart();

    // begin parse src
    context->token_to_scan = lexer->get_next_token();

    while (context->token_to_scan != 0) {
      if (context->token_to_scan == VERSION) {
        if (Program(context)) {
          return 1;
        }
      }

      context->token_to_scan = lexer->get_next_token();
    }
    return 0;
  }
}  // namespace brig
}  // namespace hsa

