#ifndef BRIG_CODEGEN_TEST
#define BRIG_CODEGEN_TEST
#include "error_reporter.h"
#include "context.h"

#define BUFFER_OFFSET 8

namespace hsa{
namespace brig{

class BrigCodeGenTest {
protected:
  Context* context;
  ErrorReporter* main_reporter;
  std::string Input;
    
  //Buffer offsets
  int code_start;
  int operand_start;
  int string_start;
  int directive_start;

public:
  BrigCodeGenTest(std::string &In){
    context = NULL;
    main_reporter = NULL;
    Input.assign(In);
    code_start = BUFFER_OFFSET;
    operand_start = BUFFER_OFFSET;
    string_start = BUFFER_OFFSET;
    directive_start = BUFFER_OFFSET;
  }  
  
  virtual void validate(Context* context) = 0;
  
  void Run_Test(int (*Rule)(Context*)){
    main_reporter = ErrorReporter::get_instance();
    context = Context::get_instance();
    
    context->set_error_reporter(main_reporter);
    context->clear_context();
    Lexer *lexer = new Lexer();
    lexer->set_source_string(Input);
    context->token_to_scan = lexer->get_next_token();
    int ret = Rule(context);
    EXPECT_EQ(0, ret);  
    if(!ret)
      validate(context);
    context->clear_context();        
    delete lexer;    
  }  
};
}
}
#endif