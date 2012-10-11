#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2, typename T3> class Instruction3Opcode_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  const T3* RefSrc2;
     
public:
  Instruction3Opcode_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1, T3* Src2) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2) { }
      
  void validate(struct BrigSections* TestOutput){
  
    const char* refbuf = reinterpret_cast<const char *>(&Refbuf->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const T* getinst = (cast<T>(getcode));
    validate_brig::validate(RefInst, getinst);
    
    oper_iterator getoper = TestOutput->oper_begin();
    const T1 *getdest = (cast<T1>(getoper));
    validate_brig::validate(RefDest, refbuf, getdest, getbuf);
        
    const T2 *getsrc1 = (cast<T2>(++getoper));
    validate_brig::validate(RefSrc1, refbuf, getsrc1, getbuf);
    
    const T3 *getsrc2 = (cast<T3>(++getoper));
    validate_brig::validate(RefSrc2, refbuf, getsrc2, getbuf);
  }
};
  
/*
Steps for Unit Test Generation
1. Declare the HSAIL string, and sub-strings, if any
2. Declare all Brig structures that have been created by the HSAIL string and enter correct values.
3. Create a class with the appropriate type
4. Assign the appropriate reference values,  and call Run_Test(). 
*/
  
TEST(CodegenTest, Instruction3Op_CodeGen){  
  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; std::string op1, op2, op3;
  StringBuffer* symbols = new StringBuffer();
  
  BrigOperandReg reg1, reg2, reg3;
  BrigInstBase Out;
  int op_buffer_start = OPERAND_BUFFER_OFFSET;
  int size_reg = sizeof(BrigOperandReg);
  
  /************************************* Test Case 1************************************/
  in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  
  Out.size = sizeof(BrigInstBase);
  Out.kind = BrigEInstBase;
  Out.opcode = BrigAdd;
  Out.type = Brigu16x2;
  Out.packing = BrigPackPPsat;
  Out.o_operands[0] = op_buffer_start; Out.o_operands[1] = op_buffer_start + size_reg; 
  Out.o_operands[2] = op_buffer_start + 2*size_reg; Out.o_operands[3] = 0; Out.o_operands[4] = 0;
  
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;
  
  reg2 = reg3 = reg1;
  reg2.name = op1.size()+1; reg3.name = op1.size()+1 + op2.size()+1;  
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase1(in, symbols, &Out, &reg1, &reg2, &reg3);
  TestCase1.Run_Test(&Instruction3);
  delete symbols;

#if 0
  /***************************Add more Unit Tests************************************/
  
  /******* case 2 reg , reg , reg *******/
  in.assign( "add_s64 $d1, $d2, $d3;\n");
  op1.assign("$d1"); op2.assign("$d2"); op3.assign("$d3");
  BrigInstBase out2 = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    BrigAdd,
    Brigs64,
    BrigNoPacking,
    {buffer_start,                
     buffer_start + size_reg, 
     buffer_start + size_reg * 2, 
     0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2.size = sizeof(BrigOperandReg);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = buffer_start + op1.size() + 1;

  reg3.size = sizeof(BrigOperandReg);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = buffer_start + op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase2(in, &out2, &reg1, &reg2, &reg3, op1, op2, op3);
  TestCase2.Run_Test(&Instruction3);
  
  /******* case 3 reg , WAVESIZE , reg *******/
  in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
  op1.assign("$d1"); op2.assign(""); op3.assign("$d3");

  uint32_t size_wav = sizeof(BrigOperandWaveSz);

  BrigInstBase out3 = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    BrigAdd,
    Brigu64,
    BrigNoPacking,
    {buffer_start,  
     buffer_start + size_reg,
     buffer_start + size_reg + size_wav,
     0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg3.size = sizeof(BrigOperandReg);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = buffer_start + op1.size() + 1;

  BrigOperandWaveSz wav = {
    sizeof(BrigOperandWaveSz),
    BrigEOperandWaveSz
  };

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandWaveSz, BrigOperandReg> 
            TestCase3(in, &out3, &reg1, &wav, &reg3, op1, op2, op3);
  TestCase3.Run_Test(&Instruction3);

  /******* case 4 reg , immed , WAVESIZE *******/
  in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
  op1.assign("$d1"); op2.assign(""); op3.assign("");
  BrigInstBase out4 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAdd, 
    Brigs16x4,
    BrigPackPSsat,
    {buffer_start, 
     buffer_start + size_reg + sizeof(BrigOperandPad), 
     buffer_start + size_reg + sizeof(BrigOperandPad) + sizeof(BrigOperandImmed), 
     0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  BrigOperandImmed imm = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  imm.bits.u = 0x40;
  
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandWaveSz> 
            TestCase4(in, &out4, &reg1, &imm, &wav, op1, op2, op3);
  TestCase4.Run_Test(&Instruction3);

  /******* case 5 reg , immed , immed *******/
  in.assign( "div_s32 $s1, 100, 10;\n");
  op1.assign("$s1"); op2.assign(""); op3.assign("");
  BrigInstBase out5 = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    BrigDiv,
    Brigs32,
    BrigNoPacking,
    {buffer_start, 
     buffer_start + size_reg + sizeof(BrigOperandPad), 
     buffer_start + size_reg + sizeof(BrigOperandPad) + sizeof(BrigOperandImmed), 
     0, 0}
  };

  reg1.type = Brigb32;
  reg1.name = buffer_start;

  BrigOperandImmed imm2 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  imm2.bits.u = 100;

  BrigOperandImmed imm3 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  imm3.bits.u = 10;


  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> 
            TestCase5(in, &out5, &reg1, &imm2, &imm3, op1, op2, op3);
  TestCase5.Run_Test(&Instruction3);

  /******* case 6 reg , reg , immed *******/
  in.assign( "div_u64 $d1, $d3, 0x1234945;\n");
  op1.assign("$d1"); op2.assign("$d3"); op3.assign("");
  BrigInstBase out6 = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    BrigDiv,
    Brigu64,
    BrigNoPacking,
    {buffer_start, 
     buffer_start + size_reg, 
     buffer_start + size_reg * 2, 0, 0}
  };

  reg1.type = reg2.type = Brigb64;
  reg1.name = buffer_start;
  reg2.name = buffer_start + op1.size() + 1;

  imm3.bits.u = 0x1234945;
 
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase6(in, &out6, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase6.Run_Test(&Instruction3);
  
  /******* case 7 reg , reg , WAVESIZE *******/
  in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
  op1.assign("$d1"); op2.assign("$d3"); op3.assign("");
  BrigInstBase out7 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigRem, 
    Brigs64,
    BrigNoPacking,
    {buffer_start, 
     buffer_start + size_reg,
     buffer_start + size_reg * 2, 0, 0}
  };
 
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2.size = sizeof(BrigOperandReg);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = buffer_start + op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandWaveSz> 
            TestCase7(in, &out7, &reg1, &reg2, &wav, op1, op2, op3);
  TestCase7.Run_Test(&Instruction3);

  /******* case 8 reg , immed , immed *******/
  in.assign( "rem_u64 $d1, 0x040, 0x12349456;\n");
  BrigInstBase out8 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigRem, 
    Brigu64,
    BrigNoPacking,
    {buffer_start, 
     buffer_start + size_reg + sizeof(BrigOperandPad), 
     buffer_start + size_reg + sizeof(BrigOperandPad) + sizeof(BrigOperandImmed), 
     0, 0}
  };
  
  imm2.size = sizeof(BrigOperandImmed);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 0x040;
  
  imm3.size = sizeof(BrigOperandImmed);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 0x12349456;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> 
            TestCase8(in, &out8, &reg1, &imm2, &imm3, op1, op2, op3);
  TestCase8.Run_Test(&Instruction3);

  /******* case 9 reg , reg , immed *******/
  in.assign( "shl_u32 $s1, $s1, 2;\n");
  op1.assign("$s1"); op2.assign(""); op3.assign("");
  BrigInstBase out9 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigShl, 
    Brigu32,
    BrigNoPacking,
    {buffer_start, buffer_start, 
     buffer_start + size_reg + sizeof(BrigOperandPad), 0, 0}
  };
  
  reg2.size = sizeof(BrigOperandReg);
  reg2.kind = BrigEOperandReg;
  reg2.type = reg1.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = reg1.name = buffer_start;

  imm3.size = sizeof(BrigOperandImmed);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase9(in, &out9, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase9.Run_Test(&Instruction3);

  /******* case 10 reg , reg , immed *******/
  in.assign("class_f32 $c1, $s2, 0x10;");
  op1.assign("$c1"); op2.assign("$s2"); op3.assign("");
  BrigInstBase out10 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigClass, 
    Brigb1,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 
     buffer_start + size_reg * 2, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2.size = sizeof(BrigOperandReg);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = buffer_start + op1.size() + 1;

  imm3.size = sizeof(BrigOperandImmed);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 0x10;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase10(in, &out10, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase10.Run_Test(&Instruction3);

  /******* case 11 reg , reg , reg *******/ 
  in.assign("class_f64 $c0, $d1, $s1;");
  op1.assign("$c0"); op2.assign("$d1"); op3.assign("$s1");
  BrigInstBase out11 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigClass, 
    Brigb1,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 
     buffer_start + size_reg * 2, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2.size = sizeof(BrigOperandReg);
  reg2.kind = BrigEOperandReg;  
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = buffer_start + op1.size() + 1;

  reg3.size = sizeof(BrigOperandReg);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = buffer_start + op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase11(in, &out11, &reg1, &reg2, &reg3, op1, op2, op3);
  TestCase11.Run_Test(&Instruction3);
#endif
  }

} //namespace hsa
} //namespace brig




