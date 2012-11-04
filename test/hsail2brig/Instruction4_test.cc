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

   void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    oper->append(RefDest);
    oper->append(RefSrc1);
    oper->append(RefSrc2);
    oper->append(RefSrc3);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), (size_t)0, code->size(), oper->size(), (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
  }  
 };

TEST(CodegenTest,Instruction4Op_CodeGen){
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low

  std:: string in; std::string op1, op2, op3, op4;
  StringBuffer* symbols;

  BrigOperandReg reg1, reg2, reg3, reg4;
  // BrigOperandWaveSz wav;
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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

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
  reg3.s_name = op1.size() + 1;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size()  + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb64;
  imm3.reserved = 0;
  imm3.bits.u = 10;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
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
  reg1.s_name = 0;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

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
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

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

/************************************* Test Case 23 reg reg reg immed************************************/
  in.assign( "fma_ftz_f32 $s1, $s0, $s3, 23.0f; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

  BrigInstMod out23 = {
    0,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0},
    {1,1,0,1,0,0,0}
  };
  out23.size = sizeof(out23);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.f = 23.0f;

  Instruction4Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase23(in, symbols, &out23, &reg1, &reg2, &reg3, &imm4);
  TestCase23.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 24 reg immed reg immed************************************/
  in.assign( "fma_ftz_zero_f64 $d1, 1.0f, $d3, 23.0f; \n");
  op1.assign("$d1"); op2.assign("$d3"); 
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out24 = {
    0,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(reg3), 0},
    {1,1,1,1,0,0,0}
  };
  out24.size = sizeof(out24);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  imm2.bits.f = 1.0f;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + 1;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
  imm4.reserved = 0;
  imm4.bits.f = 23.0f;

  Instruction4Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandReg, BrigOperandImmed> 
            TestCase24(in, symbols, &out24, &reg1, &imm2, &reg3, &imm4);
  TestCase24.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 25 reg reg reg immed************************************/
  in.assign( "mad_ftz_f32 $s1, $s0, $s3, 23.0f; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

  BrigInstMod out25 = {
    0,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0},
    {1,1,0,1,0,0,0}
  };
  out25.size = sizeof(out25);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.f = 23.0f;

  Instruction4Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase25(in, symbols, &out25, &reg1, &reg2, &reg3, &imm4);
  TestCase25.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 26 reg immed reg immed************************************/
  in.assign( "mad_ftz_zero_f64 $d1, 1.0f, $d3, 23.0f; \n");
  op1.assign("$d1"); op2.assign("$d3"); 
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out26 = {
    0,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(reg3), 0},
    {1,1,1,1,0,0,0}
  };
  out26.size = sizeof(out26);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  imm2.bits.f = 1.0f;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + 1;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
  imm4.reserved = 0;
  imm4.bits.f = 23.0f;

  Instruction4Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandReg, BrigOperandImmed> 
            TestCase26(in, symbols, &out26, &reg1, &imm2, &reg3, &imm4);
  TestCase26.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 27 reg reg reg reg************************************/
  in.assign( "mad_u32 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);  

  BrigInstBase out27 = {
    0,
    BrigEInstBase,
    BrigMad,
    Brigu32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out27.size = sizeof(out27);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase27(in, symbols, &out27, &reg1, &reg2, &reg3, &reg4);
  TestCase27.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 28 reg immed reg immed************************************/
  in.assign( "mad_u64 $d1, 42, $d3, 23; \n");
  op1.assign("$d1"); op2.assign("$d3"); 
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out28 = {
    0,
    BrigEInstBase,
    BrigMad,
    Brigu64,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(reg3), 0}
  };
  out28.size = sizeof(out28);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  imm2.bits.u = 42;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + 1;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
  imm4.reserved = 0;
  imm4.bits.u = 23;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg, BrigOperandImmed> 
            TestCase28(in, symbols, &out28, &reg1, &imm2, &reg3, &imm4);
  TestCase28.Run_Test(&Instruction4);
  symbols->clear();

  // For the regular operation: b. For the packed operation: s, u, f.
  // For the regular operation, src0 must be a control (c) register or an immediate
  // value. For the packed operation, if the Length is 32-bit, then src0 must be an s register or literal value; if
  // the Length is 64-bit, then src0 must be a d register or literal value. For the packed operation, each element
  // in src0 is assumed to contain either all 1's (true) or all 0's (false); results are undefined for other src0 values
/************************************* Test Case 29 reg reg reg reg************************************/
  in.assign( "cmov_b32 $s1, $c0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$c0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);  

  BrigInstBase out29 = {
    0,
    BrigEInstBase,
    BrigCmov,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out29.size = sizeof(out29);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb1;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase29(in, symbols, &out29, &reg1, &reg2, &reg3, &reg4);
  TestCase29.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 30 reg immed reg reg************************************/
  in.assign( "cmov_b32 $s1, 1, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$c0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);  

  BrigInstBase out30 = {
    0,
    BrigEInstBase,
    BrigCmov,
    Brigb32,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(imm2), 
     sizeof(reg1) + sizeof(imm2) + sizeof(reg3), 0}
  };
  out30.size = sizeof(out30);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg, BrigOperandReg> 
            TestCase30(in, symbols, &out30, &reg1, &imm2, &reg3, &reg4);
  TestCase30.Run_Test(&Instruction4);
  symbols->clear();

// The packing field of BrigInstBase should be set to BrigPackPP for packed source
// types and to BrigNoPacking otherwise.
// p269
/************************************* Test Case 31 reg reg reg reg************************************/
  in.assign( "cmov_s8x4 $s1, $s0, $s3, $s2; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3"); op4.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);  

  BrigInstBase out31 = {
    0,
    BrigEInstBase,
    BrigCmov,
    Brigs8x4,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out31.size = sizeof(out31);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase31(in, symbols, &out31, &reg1, &reg2, &reg3, &reg4);
  TestCase31.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 32 reg reg reg reg************************************/
  in.assign( "cmov_u16x4 $d1, $d0, $d3, $d2; \n");
  op1.assign("$d1"); op2.assign("$d0"); op3.assign("$d3"); op4.assign("$d2"); 
  symbols->append(op1); symbols->append(op2); symbols->append(op3); symbols->append(op4);  

  BrigInstBase out32 = {
    0,
    BrigEInstBase,
    BrigCmov,
    Brigu16x4,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out32.size = sizeof(out32);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb64;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase32(in, symbols, &out32, &reg1, &reg2, &reg3, &reg4);
  TestCase32.Run_Test(&Instruction4);
  symbols->clear();

// src3 must be BrigOperandImmed
/************************************* Test Case 33 reg reg reg immed************************************/
  in.assign( "shuffle_u8x4 $s1, $s0, $s3, 0; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3); 

  BrigInstBase out33 = {
    0,
    BrigEInstBase,
    BrigShuffle,
    Brigu8x4,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0}
  };
  out33.size = sizeof(out33);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.u = 0;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase33(in, symbols, &out33, &reg1, &reg2, &reg3, &imm4);
  TestCase33.Run_Test(&Instruction4);
  symbols->clear();

/************************************* Test Case 34 reg reg immed immed************************************/
  in.assign( "shuffle_u16x4 $d1, $d0, 100, 0; \n");
  op1.assign("$d1"); op2.assign("$d0"); 
  symbols->append(op1); symbols->append(op2); 

  BrigInstBase out34 = {
    0,
    BrigEInstBase,
    BrigShuffle,
    Brigu16x4,
    BrigNoPacking,
    {0,                
     sizeof(reg1), 
     sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(imm3), 0}
  };
  out34.size = sizeof(out34);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb64;
  imm3.reserved = 0;
  imm3.bits.u = 100;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb64;
  imm4.reserved = 0;
  imm4.bits.u = 0;

  Instruction4Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> 
            TestCase34(in, symbols, &out34, &reg1, &reg2, &imm3, &imm4);
  TestCase34.Run_Test(&Instruction4);
  symbols->clear();

  /***************************************  End of tests *************************************/
  delete symbols;
}

}
} 
