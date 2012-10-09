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
  
/*  
  in.assign( "add_s64 $d1, $d2, $d3;\n");
  BrigInstBase out1 = {
    32,
    BrigEInstBase, 
    BrigAdd, 
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  };
  init(in, out1, numTestCases++);    
  
  in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
  BrigInstBase out2 = {
    32,
    BrigEInstBase, 
    BrigAdd, 
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  };
  init(in, out2, numTestCases++);

  in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
  BrigInstBase out3 = {
    32,
    BrigEInstBase, 
    BrigAdd, 
    Brigs16x4,
    BrigPackPSsat,
    {8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
  };
  init(in, out3, numTestCases++);

  in.assign( "div_s32 $s1, 100, 10;\n");
  BrigInstBase out4 = {
    32,
    BrigEInstBase, 
    BrigDiv, 
    Brigs32,
    BrigNoPacking,
    {8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
  };
  init(in, out4, numTestCases++);

  in.assign( "div_u64 $d1, $d3, 0x233412349456;\n");
  BrigInstBase out5 = {
    32,
    BrigEInstBase, 
    BrigDiv, 
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  };
  init(in, out5, numTestCases++);
  
  in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
  BrigInstBase out6 = {
    32,
    BrigEInstBase, 
    BrigRem, 
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  };
  init(in, out6, numTestCases++); 
  
  in.assign( "rem_u64 $d1, 0x040, 0x233412349456;\n");
  BrigInstBase out7 = {
    32,
    BrigEInstBase, 
    BrigRem, 
    Brigu64,
    BrigNoPacking,
    {8, 24, 48, 0, 0}
  };
  init(in, out7, numTestCases++);
  
  in.assign( "shl_u32 $s1, $s1, 2;\n");
  BrigInstBase out8 = {
    32,
    BrigEInstBase, 
    BrigShl, 
    Brigu32,
    BrigNoPacking,
    {8, 8, 24, 0, 0}
  };
  init(in, out8, numTestCases++);
  
  in.assign("class_f32 $c1, $s2, 0x10;");
  BrigInstBase out9 = {
    32,
    BrigEInstBase, 
    BrigClass, 
    Brigb1,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  };
  init(in, out9, numTestCases++);
  
  in.assign("class_f64 $c0, $d1, $s1;");
  BrigInstBase out10 = {
    32,
    BrigEInstBase, 
    BrigClass, 
    Brigb1,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  };
  init(in, out10, numTestCases++);
*/  
  }

} //namespace hsa
} //namespace brig




