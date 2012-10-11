#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2> class Instruction2_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const T* Output;
  //Operands in .operands buffer
  const T1* dest;
  const T2* src1;
  
  //Symbols in .string buffer
  const std::string dest_name;
  const std::string src1_name;
 
public:
 
  Instruction2_Test(std::string& in, T* ref, T1* Dest, T2* Src1, 
        std::string& op1, std::string& op2) : 
    BrigCodeGenTest(in),
    Output(ref),
    dest(Dest),
    src1(Src1),
    dest_name(op1),
    src1_name(op2)  { }
    
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
  }
};
  
TEST(CodegenTest, Instruction2_CodeGen){

  std::string in; 
  in.assign( "abs_s32 $s1, $s2;\n");
  std::string op1, op2; op1.assign("$s1"); op2.assign("$s2"); 
  int buffer_start = BUFFER_OFFSET;
  int align;
  int size_reg = sizeof(BrigOperandReg);

  BrigInstBase out1 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };

  BrigOperandReg reg1 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    buffer_start //Offset to string table  
  };
  BrigOperandReg reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase1(in, &out1, &reg1, &reg2, op1, op2);
  TestCase1.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_s64 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out2 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase2(in, &out2, &reg1, &reg2, op1, op2);
  TestCase2.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_f32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out3 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase3(in, &out3, &reg1, &reg2, op1, op2);
  TestCase3.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_f64 $d1,$d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out4 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase4(in, &out4, &reg1, &reg2, op1, op2);
  TestCase4.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_p_s8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out5 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackP,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase5(in, &out5, &reg1, &reg2, op1, op2);
  TestCase5.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_p_f32x2 $d1, $d1;\n");
  op1.assign("$d1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out6 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32x2,
    BrigPackP,
    {buffer_start, buffer_start, 0, 0, 0}
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase6(in, &out6, &reg1, &reg2, op1, op2);
  TestCase6.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_s_s8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out7 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackS,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase7(in, &out7, &reg1, &reg2, op1, op2);
  TestCase7.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_p_s8x8 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out8 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackP,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase8(in, &out8, &reg1, &reg2, op1, op2);
  TestCase8.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_s_s8x8 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out9 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackS,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase9(in, &out9, &reg1, &reg2, op1, op2);
  TestCase9.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_p_s16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out10 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackP,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase10(in, &out10, &reg1, &reg2, op1, op2);
  TestCase10.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_s_s16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out11 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackS,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase11(in, &out11, &reg1, &reg2, op1, op2);
  TestCase11.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_s32 $s1, 99;\n");
  op1.assign("$s1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
  BrigInstBase out12 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  BrigOperandImmed imm = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm.bits.u = 99;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase12(in, &out12, &reg1, &imm, op1, op2);
  TestCase12.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_s64 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out13 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase13(in, &out13, &reg1, &reg2, op1, op2);
  TestCase13.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_f32 $s3,1.0f;\n");
  op1.assign("$s3"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
   BrigInstBase out14 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  imm.bits.f = 1.0f;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase14(in, &out14, &reg1, &imm, op1, op2);
  TestCase14.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_f64 $d3,1.0;\n");
  op1.assign("$d3"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
   BrigInstBase out15 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb64;
  imm.reserved = 0;
  imm.bits.d = 1.0;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase15(in, &out15, &reg1, &imm, op1, op2);
  TestCase15.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_p_f16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
 BrigInstBase out16 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf16x2,
    BrigPackP,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase16(in, &out16, &reg1, &reg2, op1, op2);
  TestCase16.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_s_u8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out17 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigu8x4,
    BrigPackS,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase17(in, &out17, &reg1, &reg2, op1, op2);
  TestCase17.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_p_s32x2 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out18 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32x2,
    BrigPackP,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase18(in, &out18, &reg1, &reg2, op1, op2);
  TestCase18.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "not_b1 $c1, $c2;\n");
  op1.assign("$c1"); op2.assign("$c2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out19 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb1,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase19(in, &out19, &reg1, &reg2, op1, op2);
  TestCase19.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "not_b32 $s0, $s2;\n");
  op1.assign("$s0"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out20 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase20(in, &out20, &reg1, &reg2, op1, op2);
  TestCase20.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "not_b64 $d0, $d1;\n");
  op1.assign("$d0"); op2.assign("$d1"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out21 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase21(in, &out21, &reg1, &reg2, op1, op2);
  TestCase21.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "popcount_b32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out22 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase22(in, &out22, &reg1, &reg2, op1, op2);
  TestCase22.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "popcount_b64 $s1, $d2;\n");
  op1.assign("$s1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out23 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  reg2.type = Brigb64;
  reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase23(in, &out23, &reg1, &reg2, op1, op2);
  TestCase23.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "bitrev_s32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out24 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase24(in, &out24, &reg1, &reg2, op1, op2);
  TestCase24.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "bitrev_u64 $d1, 0x234;\n");
  op1.assign("$d1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
   BrigInstBase out25 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigu64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  imm.bits.l[0] = 0x234;
  
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase25(in, &out25, &reg1, &imm, op1, op2);
  TestCase25.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "firstbit_s32 $s0, $s0;\n");
  op1.assign("$s0"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out26 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase26(in, &out26, &reg1, &reg2, op1, op2);
  TestCase26.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "firstbit_u64 $s0, $d6;\n");
  op1.assign("$s0"); op2.assign("$d6"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out27 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigu64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  reg2.name = buffer_start + op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase27(in, &out27, &reg1, &reg2, op1, op2);
  TestCase27.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "lastbit_u32 $s0, $s0;\n");
  op1.assign("$s0"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out28 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigu32,
    BrigNoPacking,
    {buffer_start, buffer_start, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase28(in, &out28, &reg1, &reg2, op1, op2);
  TestCase28.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "lastbit_s64 $s0, $d6;\n");
  op1.assign("$s0"); op2.assign("$d6"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out29 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigs64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  reg2.name = buffer_start + op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase29(in, &out29, &reg1, &reg2, op1, op2);
  TestCase29.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "movs_lo_b32 $s1, $d1;\n");
  op1.assign("$s1"); op2.assign("$d1"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out30 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsLo, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  reg2.name = buffer_start + op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase30(in, &out30, &reg1, &reg2, op1, op2);
  TestCase30.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "movs_hi_b32 $s1, $d1;\n");
  op1.assign("$s1"); op2.assign("$d1"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out31 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsHi, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; 
  reg2.name = buffer_start + op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase31(in, &out31, &reg1, &reg2, op1, op2);
  TestCase31.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "fract_f32 $s0, 3.2f;\n");
  op1.assign("$s0"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
  BrigInstBase out32 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFract, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  imm.bits.f = 3.2f;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase32(in, &out32, &reg1, &imm, op1, op2);
  TestCase32.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "fcos_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out33 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFcos, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase33(in, &out33, &reg1, &reg2, op1, op2);
  TestCase33.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "fsin_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out34 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFsin, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase34(in, &out34, &reg1, &reg2, op1, op2);
  TestCase34.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "flog2_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out35 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFlog2, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase35(in, &out35, &reg1, &reg2, op1, op2);
  TestCase35.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "fexp2_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out36 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFexp2, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase36(in, &out36, &reg1, &reg2, op1, op2);
  TestCase36.Run_Test(&Instruction2);
 
/**********************************************************************************/
 in.assign( "frsqrt_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out37 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrsqrt, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase37(in, &out37, &reg1, &reg2, op1, op2);
  TestCase37.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "frcp_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out38 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrcp, 
    Brigf32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase38(in, &out38, &reg1, &reg2, op1, op2);
  TestCase38.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "sqrt_f64 $d1, 1.21;\n");
  op1.assign("$d1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;
 
  BrigInstBase out39 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigSqrt, 
    Brigf64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0}
  };
    
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb64;
  imm.reserved = 0;
  imm.bits.d = 1.21;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase39(in, &out39, &reg1, &imm, op1, op2);
  TestCase39.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "unpack3 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
   BrigInstBase out40 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack3, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase40(in, &out40, &reg1, &reg2, op1, op2);
  TestCase40.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "unpack2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
   BrigInstBase out41 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack2, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase41(in, &out41, &reg1, &reg2, op1, op2);
  TestCase41.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "unpack1 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out42 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack1, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase42(in, &out42, &reg1, &reg2, op1, op2);
  TestCase42.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "unpack0 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out43 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack0, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase43(in, &out43, &reg1, &reg2, op1, op2);
  TestCase43.Run_Test(&Instruction2);

/**********************************************************************************/
 in.assign( "workitemaid $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out44 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkItemAId, 
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase44(in, &out44, &reg1, &reg2, op1, op2);
  TestCase44.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "sqrt_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out45 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigSqrt, 
    Brigs8x4,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase45(in, &out45, &reg1, &reg2, op1, op2);
  TestCase45.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_s32 $s1, $s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out46 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase46(in, &out46, &reg1, &reg2, op1, op2);
  TestCase46.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "mask_b64 $d1, $d2;");
  op1.assign("$d1"); op2.assign("$d2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out47 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase47(in, &out47, &reg1, &reg2, op1, op2);
  TestCase47.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "count_u32 $s1, $s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out48 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase48(in, &out48, &reg1, &reg2, op1, op2);
  TestCase48.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "alloca $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out49 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAlloca,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase49(in, &out49, &reg1, &reg2, op1, op2);
  TestCase49.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "workitemid $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out50 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkItemId,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase50(in, &out50, &reg1, &reg2, op1, op2);
  TestCase50.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "workgroupid $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out51 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkGroupId,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase51(in, &out51, &reg1, &reg2, op1, op2);
  TestCase51.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "workgroupsize $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out52 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkGroupSize,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase52(in, &out52, &reg1, &reg2, op1, op2);
  TestCase52.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "NDRangesize $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out53 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNDRangeSize,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase53(in, &out53, &reg1, &reg2, op1, op2);
  TestCase53.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "NDRangegroups $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);
 
  BrigInstBase out54 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNDRangeGroups,
    Brigb32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase54(in, &out54, &reg1, &reg2, op1, op2);
  TestCase54.Run_Test(&Instruction2);
}

/**********************************************************************************/
/**********************************************************************************/
TEST(CodegenTest, Instrustion2_With_Modifier_CodeGen) {

  std::string in; 
  in.assign( "sqrt_ftz_s8x4 $s1, $s2; \n");
  std::string op1, op2; op1.assign("$s1"); op2.assign("$s2"); 
  int buffer_start = BUFFER_OFFSET;
  int size_reg = sizeof(BrigOperandReg);
  int align;

  BrigAluModifier bam;
  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out1 = {
    sizeof(BrigInstMod),
    BrigEInstMod,      
    BrigSqrt,         
    Brigs8x4,          
    BrigNoPacking,     
    {buffer_start, buffer_start + size_reg, 0, 0, 0},
    bam
  };

  BrigOperandReg reg1 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    buffer_start //Offset to string table  
  };
  BrigOperandReg reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandReg> TestCase1(in, &out1, &reg1, &reg2, op1, op2);
  TestCase1.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "abs_ftz_p_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out2 = {
    sizeof(BrigInstMod),
    BrigEInstMod,      
    BrigAbs,           
    Brigs8x4,          
    BrigPackP,     
    {buffer_start, buffer_start + size_reg, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandReg> TestCase2(in, &out2, &reg1, &reg2, op1, op2);
  TestCase2.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "fract_ftz_f32 $s0, 1.6f;\n");
  op1.assign("$s0"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;

  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out3 = {
    sizeof(BrigInstMod),
    BrigEInstMod,      
    BrigFract,           
    Brigf32,          
    BrigNoPacking,     
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  BrigOperandImmed imm = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm.bits.f = 1.6f;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase3(in, &out3, &reg1, &imm, op1, op2);
  TestCase3.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_ftz_p_s32 $s1, 1.0;\n");
  op1.assign("$s1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;

  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out4 = {
    sizeof(BrigInstMod),
    BrigEInstMod,      
    BrigNeg,           
    Brigs32,          
    BrigPackP,     
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb64;
  imm.reserved = 0;
  imm.bits.d = 1.0;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase4(in, &out4, &reg1, &imm, op1, op2);
  TestCase4.Run_Test(&Instruction2);

/**********************************************************************************/
  in.assign( "neg_upi_s32 $s1, 99;\n");
  op1.assign("$s1"); op2.assign(""); 
  buffer_start = BUFFER_OFFSET;
  size_reg = sizeof(BrigOperandReg);

  align = 8 - ( buffer_start + size_reg ) % 8;

  bam.rounding = 2;
  bam.ftz = 0;
  bam.floatOrInt = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out5 = {
    sizeof(BrigInstMod),
    BrigEInstMod,      
    BrigNeg,           
    Brigs32,          
    BrigNoPacking,    
    {buffer_start, buffer_start + size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = buffer_start;

  imm.size = sizeof(BrigOperandImmed);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  imm.bits.u = 99;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase5(in, &out5, &reg1, &imm, op1, op2);
  TestCase5.Run_Test(&Instruction2);
}
}
}
