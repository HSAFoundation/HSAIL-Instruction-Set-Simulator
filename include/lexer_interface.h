/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_LEXER_INTERFACE_H_
#define INCLUDE_LEXER_INTERFACE_H_

#include <string>

namespace hsa {
namespace brig {
class LexerInterface {
  public:
    virtual ~LexerInterface() {}
    LexerInterface() {}

    explicit LexerInterface(const std::string& s) {
      set_source_string(s);
    };

    virtual unsigned int get_next_token() = 0;
    virtual int get_int_value() = 0;
    virtual double get_double_value() = 0;
    virtual float get_float_value() = 0;
    virtual std::string get_string_value() = 0;

    void set_source_string(const std::string& src) {
      this->src.assign(src.c_str());
    }

    void restart() {}  // rescan the source string from beginning

  protected:
    std::string src;
};


}  // namespace brig
}  // namespace hsa

#endif  // INCLUDE_LEXER_INTERFACE_H_
