#ifndef BRIG_CODEGEN_TEST
#define BRIG_CODEGEN_TEST
#include "error_reporter.h"
#include "context.h"
#include "brig_util.h"
#include "brig_buffer.h"
//#include "codegen_validate.h"

namespace hsa{
namespace brig{
    
class BrigCodeGenTest {
protected:
  Context* context;
  ErrorReporter* main_reporter;
  std::string Input;
  StringBuffer* RefStr;  
  struct BrigSections* RefOutput;
    
public:

#define caseInstBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refcode)), (cast<Brig ## X>(getcode)),  \
      RefOutput, TestOutput);   \
    break
/*
#define caseDirBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refdir)), (cast<Brig ## X>(getdir)),  \
      RefOutput, TestOutput);   \
    break

#define caseOperBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refoper)), (cast<Brig ## X>(getoper)),  \
      RefOutput, TestOutput);   \
    break    
*/
  BrigCodeGenTest(std::string &In, StringBuffer* Sbuf = NULL, BrigSections* RefOut = NULL){  
    context = NULL;
    main_reporter = NULL;
    Input.assign(In);
    RefStr = Sbuf;
    RefOutput = RefOut;
  }  
  
  void validate(struct BrigSections* TestOutput){
  
    inst_iterator getcode = TestOutput->code_begin();
    inst_iterator refcode = RefOutput->code_begin();
    const inst_iterator getcode_end = TestOutput->code_end();
    const inst_iterator refcode_end = RefOutput->code_end();
    for(; (getcode != getcode_end) && (TestOutput->codeSize>8); getcode++, refcode++){
      switch(getcode->kind){
        caseInstBrig(InstBase);
        caseInstBrig(InstMod) ;
        caseInstBrig(InstCvt) ;
        caseInstBrig(InstRead);
        caseInstBrig(InstBar) ; 
        caseInstBrig(InstLdSt);
        caseInstBrig(InstCmp);
        caseInstBrig(InstMem);
        caseInstBrig(InstAtomic); 
        caseInstBrig(InstAtomicImage);  
        caseInstBrig(InstImage); 
        default: 
            printf("Invalid instruction\n");
            exit(1);
      }
    }
    
    dir_iterator getdir = TestOutput->begin();
    dir_iterator refdir = RefOutput->begin();
    const dir_iterator getdir_end = TestOutput->end();
    const dir_iterator refdir_end = RefOutput->end();  
    for(; (getdir != getdir_end) && (TestOutput.directivesSize>8) ; getdir++, refdir++){
      switch(getdir->kind){
        caseDirBrig(DirectivePad);        
        caseDirBrig(DirectiveFunction) ;        
        caseDirBrig(DirectiveKernel);        
        caseDirBrig(DirectiveSymbol);        
        caseDirBrig(DirectiveImage) ;        
        caseDirBrig(DirectiveSampler);        
        caseDirBrig(DirectiveLabel) ;       
        caseDirBrig(DirectiveLabelList) ;        
        caseDirBrig(DirectiveVersion);         
        caseDirBrig(DirectiveSignature);          
        caseDirBrig(DirectiveFile) ;        
        caseDirBrig(DirectiveComment);        
        caseDirBrig(DirectiveLoc);        
        caseDirBrig(DirectiveInit);        
        caseDirBrig(DirectiveLabelInit);        
        caseDirBrig(DirectiveControl);        
        caseDirBrig(DirectivePragma);       
        caseDirBrig(DirectiveExtension);        
        caseDirBrig(DirectiveArgStart);        
        caseDirBrig(DirectiveArgEnd);
        caseDirBrig(DirectiveBlockStart);        
        caseDirBrig(DirectiveBlockNumeric);       
        caseDirBrig(DirectiveBlockString);        
        caseDirBrig(DirectiveBlockEnd);
        default: 
            printf("Invalid instruction\n");
            exit(1);        
      }      
    }  
    
    oper_iterator getoper = TestOutput->oper_begin();
    oper_iterator refoper = RefOutput->oper_begin();
    const oper_iterator getoper_end = TestOutput->oper_end();
    const oper_iterator refoper_end = RefOutput->oper_end();
    for(; (getoper != getoper_end) && (TestOutput.operandsSize>8); getoper++, refoper++){
      switch(refoper->kind){
        caseOperBrig(OperandPad);
        caseOperBrig(OperandBase);
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandRegV2);
        caseOperBrig(OperandRegV4);
        caseOperBrig(OperandAddress);
        caseOperBrig(OperandLabelRef);
        caseOperBrig(OperandIndirect);
        caseOperBrig(OperandCompound);
        caseOperBrig(OperandArgumentList);
        caseOperBrig(OperandFunctionList);
        caseOperBrig(OperandArgumentRef);
        caseOperBrig(OperandWaveSz);
        caseOperBrig(OperandFunctionRef);
        caseOperBrig(OperandOpaque);
        default: 
          printf("Invalid Operand\n");
          exit(1);
      }
    }
  }
  
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
