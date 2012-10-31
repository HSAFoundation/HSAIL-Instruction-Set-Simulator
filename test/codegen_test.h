#ifndef BRIG_CODEGEN_TEST
#define BRIG_CODEGEN_TEST
#include "error_reporter.h"
#include "context.h"
#include "brig_util.h"
#include "brig_buffer.h"
#include "codegen_validate.h"

namespace hsa{
namespace brig{
    
class BrigCodeGenTest {
protected:
  Context* context;
  ErrorReporter* main_reporter;
  std::string Input;
  StringBuffer* RefStr;  
     
public:

  BrigCodeGenTest(std::string &In, StringBuffer* Sbuf = NULL){  
    context = NULL;
    main_reporter = NULL;
    Input.assign(In);
    RefStr = Sbuf;    
  }  
  
  void validate(struct BrigSections* RefOutput, struct BrigSections* GetOutput){
  
    inst_iterator getinst = GetOutput->code_begin();
    inst_iterator refinst = RefOutput->ref_code_begin();
    const inst_iterator getinst_end = GetOutput->code_end();
    //const inst_iterator refinst_end = RefOutput->code_end();
    for(; (getinst != getinst_end) && (GetOutput->codeSize>8); getinst++, refinst++){
      switch(getinst->kind){
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
    
    dir_iterator getdir = GetOutput->begin();
    dir_iterator refdir = RefOutput->ref_begin();
    const dir_iterator getdir_end = GetOutput->end();
    //const dir_iterator refdir_end = RefOutput->end();  
    for(; (getdir != getdir_end) && (GetOutput->directivesSize>8) ; getdir++, refdir++){
    if(getdir->kind==BrigEDirectivePad)
        getdir++;
      switch(getdir->kind){
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
        case(BrigEDirectiveArgStart):        
        case(BrigEDirectiveArgEnd): /*validate_brig::validate((cast<BrigDirectiveScope>(refdir)), (cast<BrigDirectiveScope>(getdir)), 
                                      RefOutput, GetOutput);*/
                                    break;
        caseDirBrig(DirectiveBlockStart);        
        caseDirBrig(DirectiveBlockNumeric);       
        caseDirBrig(DirectiveBlockString);        
        caseDirBrig(DirectiveBlockEnd);
        default: 
            printf("Invalid instruction\n");
            exit(1);        
      }      
    }  
    
    oper_iterator getoper = GetOutput->oper_begin();
    oper_iterator refoper = RefOutput->ref_oper_begin();
    const oper_iterator getoper_end = GetOutput->oper_end();
    //const oper_iterator refoper_end = RefOutput->oper_end();
    for(; (getoper != getoper_end) && (GetOutput->operandsSize>8); getoper++, refoper++){
      if(getoper->kind==BrigEOperandPad)
        getoper++;
      switch(getoper->kind){
        case(BrigEOperandBase): break;
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandRegV2);
        caseOperBrig(OperandRegV4);
        caseOperBrig(OperandAddress);
        caseOperBrig(OperandLabelRef);
        caseOperBrig(OperandIndirect);
        caseOperBrig(OperandCompound);
        case(BrigEOperandFunctionList):
        caseOperBrig(OperandArgumentList);
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
  
  void Parse_Validate(int (*Rule)(Context*), struct BrigSections* RefOutput){
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
      struct BrigSections GetOutput(reinterpret_cast<const char *>(&str->get()[0]), reinterpret_cast<const char *> (&dir->get()[0]), reinterpret_cast<const char *>(&code->get()[0]), reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
         str->size(), dir->size(), code->size(), oper->size(), (size_t)0);
      validate(RefOutput, &GetOutput);
      
    }
    context->clear_context();        
    delete lexer;    
  }  
};
}
}
#endif
