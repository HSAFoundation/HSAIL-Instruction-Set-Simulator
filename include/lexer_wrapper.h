/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_LEXER_WRAPPER_H_
#define INCLUDE_LEXER_WRAPPER_H_

#include <string>
#include "lexer.h"
#include "context.h"

namespace hsa {
namespace brig {


class Lexer {
  public:
    Lexer() {
      bp = NULL;
    }
    ~Lexer() {
      if (bp != NULL) {
        yy_delete_buffer(bp);
        bp = NULL;
      }
    }

    explicit Lexer(const std::string& s) {
      src.clear();
      src.assign(s.c_str());
      bp = yy_scan_string(reinterpret_cast<const char*>(src.c_str()));
    }

    // get the next token
    unsigned int get_next_token(void);

    // set source string
    void set_source_string(const std::string& src);

    // get source string
    std::string get_source_string(void) {
      return this->src;
    }

    // reset to begining of src
    void restart(void);

  private:
    std::string src;
    YY_BUFFER_STATE bp;
  };

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_LEXER_WRAPPER_H_

