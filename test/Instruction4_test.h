#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2, typename T3, typename T4> class Instruction4Opcode_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  const T3* RefSrc2;
  const T4* RefSrc3;
     
public:
  Instruction4Opcode_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1, T3* Src2, T4* Src3) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2), 
    RefSrc3(Src3)  { }
 
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
    
    const T3 *getsrc2 = reinterpret_cast <const T3*> (&(TestOutput->operands[getinst->o_operands[2]]));
    validate_brig::validateOpType<T3>(RefSrc2, refbuf, getsrc2, getbuf);
    
    const T4 *getsrc3 = reinterpret_cast <const T4*> (&(TestOutput->operands[getinst->o_operands[3]]));
    validate_brig::validateOpType<T4>(RefSrc3, refbuf, getsrc3, getbuf);
    
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
 };

TEST(CodegenTest,Instruction4Op_CodeGen){
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; std::string op1, op2, op3, op4;
  StringBuffer* symbols;
  
  BrigOperandReg reg1, reg2, reg3, reg4;
  BrigOperandWaveSz wav;
  BrigOperandImmed imm2, imm3, imm4;
  
   
  /************************************* Test Case 1 reg reg reg reg ************************************/
  // extract must follow :
  // The first source src0 and the destination dest must be the size specified in the type.
  // The remaining sources (src1 and src2) must be 32-bit registers

  symbols = new StringBuffer();
  in.assign( "extract_b32 $s1, $s0, $s3,$s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2");
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);

    BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out1.size = sizeof(out1);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase1(in, symbols, &out1, &reg1, &reg2, &reg3, &reg4);
  TestCase1.Run_Test(&Instruction4);
  symbols->clear();

  /************************************* Test Case 2 reg reg reg immed************************************/
  in.assign( "extract_b64 $d1, $d0, $s3, $s2; \n");
  op1.assign("$d1"); op2.assign("$d0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out2.size = sizeof(out2);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase2(in, symbols, &out2, &reg1, &reg2, &reg3, &reg4);
  TestCase2.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 3 reg imm reg reg************************************/
  in.assign( "extract_b32 $s1, 0x12345678, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s3"); op3.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(reg3), 0}
  };
  out3.size = sizeof(out3);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 0x12345678;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + 2;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg, BrigOperandReg> 
            TestCase3(in, symbols, &out3, &reg1, &imm2, &reg3, &reg4);
  TestCase3.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 4 reg reg reg reg************************************/
  in.assign( "bitselect_b32 $s1, $s0, $s3,$s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out4.size = sizeof(out4);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase4(in, symbols, &out4, &reg1, &reg2, &reg3, &reg4);
  TestCase4.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 5 reg reg reg immed************************************/
  in.assign( "bitselect_b64 $d1, $d0, $d3, 33; \n");
  op1.assign("$d1"); op2.assign("$d0"); op3.assign("$d3"); 
  symbols->append(op1); symbols->append(op2);  symbols->append(op3); 

    BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out5.size = sizeof(out5);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size()  + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 33;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase5(in, symbols, &out5, &reg1, &reg2, &reg3, &imm4);
  TestCase5.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 6 reg reg reg reg************************************/
  in.assign( "insert_b32 $s1, $s0, $s3,$s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out6.size = sizeof(out6);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase6(in, symbols, &out6, &reg1, &reg2, &reg3, &reg4);
  TestCase6.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 7 reg reg immed immed************************************/
  in.assign( "insert_b64 $d1, $d0, 10, 33; \n");
  op1.assign("$d1"); op2.assign("$d0"); 
  symbols->append(op1); symbols->append(op2);  

    BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(imm3), 0}
  };
  out7.size = sizeof(out7);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 10;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 33;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> 
            TestCase7(in, symbols, &out7, &reg1, &reg2, &imm3, &imm4);
  TestCase7.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 8 reg immed immed immed************************************/
  in.assign( "insert_b32 $s1, 0x12345678, 8, 16; \n");
  op1.assign("$s1"); 
  symbols->append(op1); 

    BrigInstBase out8 = {
    0,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(imm3), 0}
  };
  out8.size = sizeof(out8);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 0x12345678;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 8;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 16;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed, BrigOperandImmed> 
            TestCase8(in, symbols, &out8, &reg1, &imm2, &imm3, &imm4);
  TestCase8.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 9 reg reg reg reg************************************/
  in.assign( "bitalign_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out9 = {
    0,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out9.size = sizeof(out9);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase9(in, symbols, &out9, &reg1, &reg2, &reg3, &reg4);
  TestCase9.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 10 reg reg reg immed************************************/
  in.assign( "bitalign_b32 $s1, $s0, $s3, 16; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out10 = {
    0,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out10.size = sizeof(out10);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 16;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase10(in, symbols, &out10, &reg1, &reg2, &reg3, &imm4);
  TestCase10.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 11 reg reg reg reg************************************/
  in.assign( "bytealign_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out11 = {
    0,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out11.size = sizeof(out11);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase11(in, symbols, &out11, &reg1, &reg2, &reg3, &reg4);
  TestCase11.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 12 reg reg reg immed************************************/
  in.assign( "bytealign_b32 $s1, $s0, $s3, 2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out12 = {
    0,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out12.size = sizeof(out12);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 2;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase12(in, symbols, &out12, &reg1, &reg2, &reg3, &imm4);
  TestCase12.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 13 reg reg reg reg************************************/
  in.assign( "lerp_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out13 = {
    0,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out13.size = sizeof(out13);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase13(in, symbols, &out13, &reg1, &reg2, &reg3, &reg4);
  TestCase13.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 14 reg reg reg immed************************************/
  in.assign( "lerp_b32 $s1, $s0, $s3, 0x12121212; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out14 = {
    0,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out14.size = sizeof(out14);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0x12121212;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase14(in, symbols, &out14, &reg1, &reg2, &reg3, &imm4);
  TestCase14.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 15 reg reg reg reg************************************/
  in.assign( "sad_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out15 = {
    0,
    BrigEInstBase,
    BrigSad,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out15.size = sizeof(out15);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase15(in, symbols, &out15, &reg1, &reg2, &reg3, &reg4);
  TestCase15.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 16 reg reg reg immed************************************/
  in.assign( "sad_b32 $s1, $s0, $s3, 0x12345678; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out16 = {
    0,
    BrigEInstBase,
    BrigSad,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out16.size = sizeof(out16);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0x12345678;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase16(in, symbols, &out16, &reg1, &reg2, &reg3, &imm4);
  TestCase16.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 17 reg reg reg reg************************************/
  in.assign( "sad2_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out17 = {
    0,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out17.size = sizeof(out17);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase17(in, symbols, &out17, &reg1, &reg2, &reg3, &reg4);
  TestCase17.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 18 reg reg reg immed************************************/
  in.assign( "sad2_b32 $s1, $s0, $s3, 0x12345678; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out18 = {
    0,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out18.size = sizeof(out18);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0x12345678;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase18(in, symbols, &out18, &reg1, &reg2, &reg3, &imm4);
  TestCase18.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 19 reg reg reg reg************************************/
  in.assign( "sad4_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out19 = {
    0,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out19.size = sizeof(out19);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase19(in, symbols, &out19, &reg1, &reg2, &reg3, &reg4);
  TestCase19.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 20 reg reg reg immed************************************/
  in.assign( "sad4_b32 $s1, $s0, $s3, 0x12345678; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out20 = {
    0,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out20.size = sizeof(out20);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0x12345678;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase20(in, symbols, &out20, &reg1, &reg2, &reg3, &imm4);
  TestCase20.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 21 reg reg reg reg************************************/
  in.assign( "sad4hi_b32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4); 

    BrigInstBase out21 = {
    0,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out21.size = sizeof(out21);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase21(in, symbols, &out21, &reg1, &reg2, &reg3, &reg4);
  TestCase21.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 22 reg reg reg immed************************************/
  in.assign( "sad4hi_b32 $s1, $s0, $s3, 0x12345678; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

    BrigInstBase out22 = {
    0,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out22.size = sizeof(out22);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0x12345678;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase22(in, symbols, &out22, &reg1, &reg2, &reg3, &imm4);
  TestCase22.Run_Test(&Instruction4);
  symbols->clear();

  /***************************************  End of tests *************************************/
  delete symbols;
}

}
} 
