#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2, typename T3> class Instruction3Opcode_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* Output;
  //Operands in .operands buffer
  const T1* dest;
  const T2* src1;
  const T3* src2;
  //Symbols in .string buffer
  const std::string dest_name;
  const std::string src1_name;
  const std::string src2_name;
  
public:
  Instruction3Opcode_Test(std::string& in, T* ref, T1* Dest, T2* Src1, T3* Src2, 
        std::string& op1, std::string& op2, std::string& op3) : 
    BrigCodeGenTest(in),
    Output(ref),
    dest(Dest),
    src1(Src1),
    src2(Src2),
    dest_name(op1),
    src1_name(op2),
    src2_name(op3) { }
      
  void validate(Context* context){
    T getcode;
    context->get_code(code_start, &getcode);
    validate_brig::validate(Output, &getcode);
    
    T1 getreg1;
    context->get_operand(Output->o_operands[0], &getreg1);
    validate_brig::validate(dest, &getreg1);
        
    T2 getreg2;
    context->get_operand(Output->o_operands[1], &getreg2);
    validate_brig::validate(src1, &getreg2);
        
    T3 getreg3;
    context->get_operand(Output->o_operands[2], &getreg3);
    validate_brig::validate(src2, &getreg3);
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
  
  std:: string in; 
  in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  std::string op1, op2, op3; op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
    
  int buffer_start = BUFFER_OFFSET;    //All buffers(.code, .directive, .string) begin at an offset of 8 bytes
  int size_reg = sizeof(BrigOperandReg);
  BrigInstBase Out = {
    32,
    BrigEInstBase, 
    BrigAdd, 
    Brigu16x2,
    BrigPackPPsat,
    {buffer_start, buffer_start + size_reg, buffer_start + 2*size_reg, 0, 0}
  };
  BrigOperandReg reg1 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    buffer_start //Offset to string table  
  };
  BrigOperandReg reg2, reg3;
  reg2 = reg3 = reg1;
  reg2.name = buffer_start + op1.size()+1; reg3.name = buffer_start + op1.size()+1 + op2.size()+1;
  
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase1(in, &Out, &reg1, &reg2, &reg3, op1, op2, op3);
  TestCase1.Run_Test(&Instruction3);
  
  /***************************Add more Unit Tests************************************/
  
  /******* case 2 reg , reg , reg *******/
  in.assign( "add_s64 $d1, $d2, $d3;\n");
  op1.assign("$d1"); op2.assign("$d2"); op3.assign("$d3");

  Out.type = Brigs64,
  Out.packing = BrigNoPacking,

  reg1.type = reg2.type = reg3.type = Brigb64;
  reg1.name = buffer_start;
  reg2.name = buffer_start + op1.size() + 1;
  reg3.name = buffer_start + op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase2(in, &Out, &reg1, &reg2, &reg3, op1, op2, op3);
  TestCase2.Run_Test(&Instruction3);
  
  /******* case 3 reg , WAVESIZE , reg *******/
  in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
  op1.assign("$d1"); op2.assign(""); op3.assign("$d3");

  uint32_t size_wav = sizeof(BrigOperandWaveSz);

  Out.type = Brigu64;
  Out.o_operands[2] = buffer_start + size_reg + size_wav;

  reg1.type = reg3.type = Brigb64;
  reg1.name = buffer_start;
  BrigOperandWaveSz wav = {
    sizeof(BrigOperandWaveSz),
    BrigEOperandWaveSz
  };
  reg3.name = buffer_start + op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandWaveSz, BrigOperandReg> 
            TestCase3(in, &Out, &reg1, &wav, &reg3, op1, op2, op3);
  TestCase3.Run_Test(&Instruction3);

  /******* case 4 reg , immed , WAVESIZE *******/
  in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
  op1.assign("$d1"); op2.assign(""); op3.assign("");

  Out.type = Brigs16x4;
  Out.packing = BrigPackPSsat;
  Out.o_operands[1] = buffer_start + size_reg + sizeof(BrigOperandPad);
  Out.o_operands[2] = buffer_start + size_reg + sizeof(BrigOperandPad) 
                      + sizeof(BrigOperandImmed);

  BrigOperandImmed imm = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  imm.bits.u = 0x40;
  
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandWaveSz> 
            TestCase4(in, &Out, &reg1, &imm, &wav, op1, op2, op3);
  TestCase4.Run_Test(&Instruction3);

  /******* case 5 reg , immed , immed *******/
  in.assign( "div_s32 $s1, 100, 10;\n");
  op1.assign("$s1"); op2.assign(""); op3.assign("");

  Out.opcode = BrigDiv;
  Out.type = Brigs32;
  Out.packing = BrigNoPacking;

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
            TestCase5(in, &Out, &reg1, &imm2, &imm3, op1, op2, op3);
  TestCase5.Run_Test(&Instruction3);

  /******* case 6 reg , reg , immed *******/
  in.assign( "div_u64 $d1, $d3, 0x1234945;\n");
  op1.assign("$d1"); op2.assign("$d3"); op3.assign("");

  Out.type = Brigu64;

  Out.o_operands[1] = buffer_start + size_reg;
  Out.o_operands[2] = buffer_start + size_reg * 2;

  reg1.type = reg2.type = Brigb64;
  reg1.name = buffer_start;
  reg2.name = buffer_start + op1.size() + 1;

  imm3.bits.u = 0x1234945;
 
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase6(in, &Out, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase6.Run_Test(&Instruction3);
  
  /******* case 7 reg , reg , WAVESIZE *******/
  in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
  op1.assign("$d1"); op2.assign("$d3"); op3.assign("");

  Out.opcode = BrigRem;
  Out.type = Brigs64;

  reg1.type = reg2.type = Brigb64;
  reg1.name = buffer_start;
  reg2.name = buffer_start + op1.size() + 1;
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandWaveSz> 
            TestCase7(in, &Out, &reg1, &reg2, &wav, op1, op2, op3);
  TestCase7.Run_Test(&Instruction3);

  /******* case 8 reg , immed , immed *******/
  in.assign( "rem_u64 $d1, 0x040, 0x12349456;\n");

  Out.type = Brigu64;
  Out.packing = BrigNoPacking;
  Out.o_operands[1] = buffer_start + size_reg + sizeof(BrigOperandPad);
  Out.o_operands[2] = buffer_start + size_reg + sizeof(BrigOperandPad)
                      + sizeof(BrigOperandImmed);

  imm2.bits.u = 0x040;
  imm3.bits.u = 0x12349456;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> 
            TestCase8(in, &Out, &reg1, &imm2, &imm3, op1, op2, op3);
  TestCase8.Run_Test(&Instruction3);

  /******* case 9 reg , reg , immed *******/
  in.assign( "shl_u32 $s1, $s1, 2;\n");
  op1.assign("$s1"); op2.assign(""); op3.assign("");

  Out.opcode = BrigShl;
  Out.type = Brigu32;
  Out.o_operands[1] = buffer_start;
  Out.o_operands[2] = buffer_start + size_reg + sizeof(BrigOperandPad);
  
  reg2.type = reg1.type = Brigb32;
  reg2.name = reg1.name = buffer_start;

  imm3.bits.u = 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase9(in, &Out, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase9.Run_Test(&Instruction3);
 
  /******* case 10 reg , reg , immed *******/
  in.assign("class_f32 $c1, $s2, 0x10;");
  op1.assign("$c1"); op2.assign("$s2"); op3.assign("");

  Out.opcode = BrigClass;
  Out.type = Brigb1;
  Out.o_operands[1] = buffer_start + size_reg;
  Out.o_operands[2] = buffer_start + size_reg * 2;

  reg1.type = Brigb1;
  reg1.name = buffer_start;
  reg2.type = Brigb32;
  reg2.name = buffer_start + op1.size() + 1;
  imm3.bits.u = 0x10;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed> 
            TestCase10(in, &Out, &reg1, &reg2, &imm3, op1, op2, op3);
  TestCase10.Run_Test(&Instruction3);

  /******* case 11 reg , reg , reg *******/
  in.assign("class_f64 $c0, $d1, $s1;");
  op1.assign("$c0"); op2.assign("$d1"); op3.assign("$s1");

  reg1.type = Brigb1;
  reg1.name = buffer_start;  
  reg2.type = Brigb64;
  reg2.name = buffer_start + op1.size() + 1;
  reg3.type = Brigb32;
  reg3.name = buffer_start + op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> 
            TestCase11(in, &Out, &reg1, &reg2, &reg3, op1, op2, op3);
  TestCase11.Run_Test(&Instruction3);

  }

} //namespace hsa
} //namespace brig




