/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_PARSER_WRAPPER_H_
#define INCLUDE_PARSER_WRAPPER_H_

#include <string>
#include "lexer_wrapper.h"
#include "context.h"


namespace hsa {
namespace brig {

class Parser {
  public:
    Parser() {
      lexer = new Lexer();
      context = new Context();
    }

    explicit Parser(const std::string& src) {
      lexer = new Lexer(src);
      context = new Context();
    }

    explicit Parser(Context* context) {
      this->context = context;
      context->clear_context();
      lexer = new Lexer();
    }

    // get context
    Context* get_context(void) const { return context; }

    // clear context
    void clear_context(void) { context->clear_context(); }

    // scan source and put symbol into string buffer
    void scan_symbols(void);

    // parse current source string
    // return 0 if success
    // return 1 if fail
    int parse(void);

    // set source string
    void set_source_string(const std::string& src);

  private:
    Lexer* lexer;
    Context* context;
  };

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_WRAPPER_H_
