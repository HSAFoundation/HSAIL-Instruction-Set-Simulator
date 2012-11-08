#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Instruction0_Test: public BrigCodeGenTest {

private:
  const BrigInstBase* RefInst;

public:
  Instruction0_Test(std::string& in, BrigInstBase* ref):
    BrigCodeGenTest(in),
    RefInst(ref) {}
  
  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    code->append(RefInst);
 
    
    struct BrigSections RefOutput(NULL, NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      NULL, NULL, 0, 0, code->size(), 0, (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    
  }  
};

/****************** Nop Test ************************/
TEST(CodegenTest, Nop_Codegen){

/*********************Common variables**********************/
  std::string in; 
  /*****************************************************************/
  in.assign( "nop;\n");
    
  BrigInstBase out1 = {
    0,                     
    BrigEInstBase,         
    BrigNop,                     
    Brigb32,               
    BrigNoPacking,         
    {0, 0, 0, 0, 0},                     
  };
  out1.size = sizeof(out1);
    
  Instruction0_Test TestCase1(in, &out1);
  TestCase1.Run_Test(&Instruction0);  
}

/****************** Ret Test ************************/
TEST(CodegenTest, Ret_Codegen){

/*********************Common variables**********************/
  std::string in; 
  /*****************************************************************/
  in.assign( "ret;\n");
    
  BrigInstBase out1 = {
    0,                     
    BrigEInstBase,         
    BrigRet,                     
    Brigb32,               
    BrigNoPacking,         
    {0, 0, 0, 0, 0},                     
  };
  out1.size = sizeof(out1);
    
  Instruction0_Test TestCase1(in, &out1);
  TestCase1.Run_Test(&Ret);  
}

}
}
