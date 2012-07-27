/* Copyright 2012 <MulticorewareInc> */

#include "lexer_wrapper.h"

namespace hsa {
namespace brig {
  
  int Lexer::get_next_token(void) { 
    return yylex();
  }
  
  int Lexer::get_int_value(void) {
    return -1;
  }
  
  float Lexer::get_float_value(void) {
    return -1.0;
  }
  
  std::string Lexer::get_string_value(void) {
    std::string value(string_val);
    return value;
  }
  
  void Lexer::set_source_string(const std::string& src) {
    yy_scan_string(reinterpret_cast<const char*>(src.c_str()));
  }

}  // namespace brig
}  // namespace hsa  

