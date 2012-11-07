#include "../codegen_test.h"

namespace hsa{
namespace brig{

class Instruction0_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const BrigInstBase* RefInst;
    
public:
 
  Instruction0_Test(std::string& in, BrigInstBase* ref) : 
    BrigCodeGenTest(in),
    RefInst(ref) { }
    
  void validate(struct BrigSections* TestOutput){   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstBase* getinst = (cast<BrigInstBase>(getcode));
    validate_brig::validate(RefInst, getinst);
      
    EXPECT_EQ(0, getinst->o_operands[0]);
    EXPECT_EQ(0, getinst->o_operands[1]);    
    EXPECT_EQ(0, getinst->o_operands[2]);    
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);       
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
