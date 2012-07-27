/* Copyright 2012 <MulticorewareInc> */

#include "parser_wrapper.h"
#include "parser.h"

namespace hsa {
namespace brig {

  
  void Parser::set_source_string(const std::string& src) {
    this->src.assign(src.c_str());
    lexer->set_source_string(this->src);
  };

  // scan source for symbols
  void Parser::scan_symbols(void) {
    
  
  }

}  // namespace brig
}  // namespace hsa  

