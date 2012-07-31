/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_LEXER_WRAPPER_H_
#define INCLUDE_LEXER_WRAPPER_H_

#include <string>
#include "lexer.h"
#include "lexer_interface.h"
extern int int_val;
extern char* string_val;
extern float float_val;
extern double double_val;

namespace hsa {
namespace brig {


class Lexer: public LexerInterface {
  public:
    Lexer() {}

    explicit Lexer(const std::string& s)
    : LexerInterface(s) {
      yy_scan_string(reinterpret_cast<const char*>(src.c_str()));
    }

    // get the next token
    unsigned int get_next_token(void);

    // get the value of token
    int get_int_value(void);
    double get_double_value(void);
    float get_float_value(void);
    std::string get_string_value(void);

    // set source string - inherit from LexerInterface
    void set_source_string(const std::string& src);

    // reset to begining of src
    void restart(void);

  // private:
  //  std::string src;  // local copy of source string - inherit
  };

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_LEXER_WRAPPER_H_
