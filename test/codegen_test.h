#ifndef BRIG_CODEGEN_TEST
#define BRIG_CODEGEN_TEST
#include "error_reporter.h"
#include "context.h"
#include "brig_util.h"
#include "brig_buffer.h"

namespace hsa{
namespace brig{

class BrigCodeGenTest {
protected:
  Context* context;
  ErrorReporter* main_reporter;
  std::string Input;
  StringBuffer* RefStr;  
  Buffer* RefDir;
  Buffer* RefCode;
  Buffer* RefOper;
    
public:
  BrigCodeGenTest(std::string &In, StringBuffer* Sbuf = NULL, Buffer* dir = NULL, Buffer* code = NULL, Buffer* oper = NULL){  
    context = NULL;
    main_reporter = NULL;
    Input.assign(In);
    RefStr = Sbuf;
    RefDir = dir;
    RefCode = code;
    RefOper = oper;
  }  
  
  virtual void validate(struct BrigSections* BrigOutput) = 0;
  
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
    if(!ret){
      
      StringBuffer* str = context->get_strings();
      Buffer* dir = context->get_directive();
      Buffer* oper = context->get_operands();
      Buffer* code = context->get_code();
      struct BrigSections S_(reinterpret_cast<const char *>(&str->get()[0]), reinterpret_cast<const char *> (&dir->get()[0]), reinterpret_cast<const char *>(&code->get()[0]), reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
         str->size(), dir->size(), code->size(), oper->size(), (size_t)0);
      validate(&S_);
      
    }
    context->clear_context();        
    delete lexer;    
  }  
};
}
}
#endif
