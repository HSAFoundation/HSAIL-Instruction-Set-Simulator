// Copyright 2012 <Multicoreware Inc. >

#ifndef INCLUDE_HSAIL2BRIG_H_
#define INCLUDE_HSAIL2BRIG_H_

#include <string>
#include "context.h"
#include "parser_wrapper.h"
#include "lexer_wrapper.h"

namespace hsa {
namespace brig {

class Hsail2Brig {
  public:
    Hsail2Brig() {
      context = new Context();
      lexer = new Lexer();
      parser = new Parser();
    }

    // get current context
    Context* get_current_context(void) const { return context; }

    // clear context
    void clear_context(void) { context->clear_all_buffers(); }

    // Compile hsail text source to Brig
    Context* compile_string(const std::string& src);

  private:
    Context* context;
    Lexer* lexer;
    Parser* parser;
  };

}  // namespace brig
}  // namespace hsa











#endif  // INCLUDE_HSAIL2BRIG_H_
