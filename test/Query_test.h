#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2> class Query_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
     
public:
  Query_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1) { }
 
  void validate(struct BrigSections* TestOutput){
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const T* getinst = (cast<T>(getcode));
    validate_brig::validate(RefInst, getinst);
    
    const T1 *getdest = reinterpret_cast <const T1*> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validateOpType<T1>(RefDest, refbuf, getdest, getbuf);
        
    const T2 *getsrc1 = reinterpret_cast <const T2*> (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validateOpType<T2>(RefSrc1, refbuf, getsrc1, getbuf);
    
    EXPECT_EQ(0, getinst->o_operands[2]);
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
};

TEST(CodegenTest, QueryOp_CodeGen){
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; std::string op1, op2,op3 ;
  StringBuffer* symbols;
  
  BrigOperandReg reg1;

  /************************************* Test Case 1************************************/ 
  symbols = new StringBuffer();
  in.assign( "query_width_u32 $s1, [%RWImg3]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3");
  symbols->append(op1); symbols->append(op2);

   BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out1.size = sizeof(out1);
  
  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  //ref to %RWImg3 - Not added in symbol table; it is assumed that it was added during variable decl
  BrigOperandOpaque src1 = {
  0,                    // size
  BrigEOperandOpaque,   // kind 
  0,                    // name
  0,                    // reg
  0                     // Offset
  };
  src1.size = sizeof(src1);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = op1.size() + op2.size() + 2;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase1(in, symbols, &out1, &dest1, &src1);
  TestCase1.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 2************************************/ 
  in.assign( "query_width_u32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out2.size = sizeof(out2);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase2(in, symbols, &out2, &dest1, &src1);
  TestCase2.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 3************************************/ 
  in.assign( "query_width_u32 $s1, [%RWImg3<$s0>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out3.size = sizeof(out3);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 0;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase3(in, symbols, &out3, &dest1, &src1);
  TestCase3.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 4************************************/ 
  in.assign( "query_width_u32 $s1, [%RWImg3<4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3");
  symbols->append(op1); symbols->append(op2);

   BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out4.size = sizeof(out4);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = 0;
  src1.offset = 0;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase4(in, symbols, &out4, &dest1, &src1);
  TestCase4.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 5************************************/ 
  in.assign( "query_height_u32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigQueryHeight,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out5.size = sizeof(out5);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase5(in, symbols, &out5, &dest1, &src1);
  TestCase5.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 6************************************/ 
  in.assign( "query_depth_u32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigQueryDepth,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out6.size = sizeof(out6);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase6(in, symbols, &out6, &dest1, &src1);
  TestCase6.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 7************************************/ 
  in.assign( "query_array_b32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigQueryArray,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out7.size = sizeof(out7);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase7(in, symbols, &out7, &dest1, &src1);
  TestCase7.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 8************************************/ 
  in.assign( "query_normalized_b32 $s1, [%RWSamp3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWSamp3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out8 = {
    0,
    BrigEInstBase,
    BrigQueryNormalized,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out8.size = sizeof(out8);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase8(in, symbols, &out8, &dest1, &src1);
  TestCase8.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 9************************************/ 
  in.assign( "query_filtering_b32 $s1, [%RWSamp3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWSamp3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out9 = {
    0,
    BrigEInstBase,
    BrigQueryFiltering,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out9.size = sizeof(out9);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase9(in, symbols, &out9, &dest1, &src1);
  TestCase9.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 10************************************/ 
  in.assign( "query_order_b32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out10 = {
    0,
    BrigEInstBase,
    BrigQueryOrder,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out10.size = sizeof(out10);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase10(in, symbols, &out10, &dest1, &src1);
  TestCase10.Run_Test(&Query);
  symbols->clear();

  /************************************* Test Case 11************************************/ 
  in.assign( "query_data_b32 $s1, [%RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op2.assign("%RWImg3"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

   BrigInstBase out11 = {
    0,
    BrigEInstBase,
    BrigQueryData,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     0, 0, 0}
  };
  out11.size = sizeof(out11);
  
  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.name = 0;

  //ref to %RWImg3<$s0 + 4>
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.name = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  Query_Test<BrigInstBase, BrigOperandReg, BrigOperandOpaque> 
            TestCase11(in, symbols, &out11, &dest1, &src1);
  TestCase11.Run_Test(&Query);
  symbols->clear();

  /*************************************************************************/ 
  delete symbols;
}
}
}
