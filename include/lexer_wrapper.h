/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_LEXER_WRAPPER_H_
#define INCLUDE_LEXER_WRAPPER_H_

#include <string>
#include "lexer.h"
extern int int_val;
extern char* string_val;
extern float float_val;
extern double double_val;

namespace hsa {
namespace brig {


class Lexer {
  public:
    Lexer() {}
    
    Lexer(std::string& s) {
      set_source_string(s);
    }
    
    // get the next token
    int get_next_token(void);
    
    // get the value of token 
    int get_int_value(void);
    double get_double_value(void);
    float get_float_value(void);
    std::string get_string_value(void);
    
    // set source string
    void set_source_string(const std::string& src);
    
    // reset to begining of src
    void restart(void);
    
  private:
    std::string src;  // local copy of source string
  };

}  // namespace brig
}  // namespace hsa  
#endif  // INCLUDE_LEXER_WRAPPER_H_

