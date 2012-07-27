/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_PARSER_WRAPPER_H_
#define INCLUDE_PARSER_WRAPPER_H_

#include <string>
#include "lexer_wrapper.h"

namespace hsa {
namespace brig {

class Parser {
  public:
    // parse the string
    // return 0 if success
    // return 1 if fail
    int parse();
    
    // set source string
    void set_source_string(const std::string& src);
    
  private:
    Lexer* lexer;
  };

}  // namespace brig
}  // namespace hsa  
#endif  // INCLUDE_PARSER_WRAPPER_H_
