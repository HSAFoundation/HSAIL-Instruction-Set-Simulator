/* Copyright 2012 <MulticorewareInc> */

#include "lexer_wrapper.h"

namespace hsa {
namespace brig {

  unsigned int Lexer::get_next_token(void) {
    return yylex();
  }

  void Lexer::set_source_string(const std::string& src) {
    this->src.clear();
    this->src.assign(src.c_str());
    yy_scan_string(reinterpret_cast<const char*>(src.c_str()));
  }


  void Lexer::restart(void) {
    yy_scan_string(reinterpret_cast<const char*>(src.c_str()));
  }

}  // namespace brig
}  // namespace hsa

