/* Copyright 2012 <MulticorewareInc> */

#include "parser_wrapper.h"
#include "parser.h"

namespace hsa {
namespace brig {
  void Parser::set_source_string(const std::string& src) {
    lexer->set_source_string(src);
  };

  // scan source for symbols
  void Parser::scan_symbols(void) {
    lexer->restart();
    context->clear_all_buffers();
    ScanString(lexer->get_next_token(), context);
  }
}  // namespace brig
}  // namespace hsa

