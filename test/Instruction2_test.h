#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2> class Instruction2_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  
public:
 
  Instruction2_Test(std::string& in, StringBuffer *sbuf, T* ref, T1* Dest, T2* Src1) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1)  { }
    
  void validate(struct BrigSections* TestOutput){
  
    const char* refbuf = reinterpret_cast<const char *>(&Refbuf->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const T* getinst = (cast<T>(getcode));
    validate_brig::validate(RefInst, getinst);
    
    const T1 *getdest = reinterpret_cast <const T1*> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(RefDest, refbuf, getdest, getbuf);
        
    const T2 *getsrc1 = reinterpret_cast <const T2*> (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validate(RefSrc1, refbuf, getsrc1, getbuf);
    
    EXPECT_EQ(0, getinst->o_operands[2]);    
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
};
  
TEST(CodegenTest, Instruction2_CodeGen){

/*********************Common variables**********************/
  std::string in; std::string op1, op2;
  StringBuffer *symbols = new StringBuffer();
  BrigOperandReg reg1, reg2;
  
  /*****************************************************************/
  in.assign( "abs_s32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
  int align;
  int size_reg = sizeof(BrigOperandReg);

  BrigInstBase out1 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };

  
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;
  
  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase1(in, symbols, &out1, &reg1, &reg2);
  TestCase1.Run_Test(&Instruction2);
  symbols->clear();
 
/**********************************************************************************/
  in.assign( "abs_s64 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out2 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };

  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase2(in, symbols, &out2, &reg1, &reg2);
  TestCase2.Run_Test(&Instruction2);
  symbols->clear();
 
/**********************************************************************************/
  in.assign( "abs_f32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out3 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };

  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase3(in, symbols, &out3, &reg1, &reg2);
  TestCase3.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_f64 $d1,$d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out4 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };

  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase4(in, symbols, &out4, &reg1, &reg2);
  TestCase4.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_p_s8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out5 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };

  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase5(in, symbols, &out5, &reg1, &reg2);
  TestCase5.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_p_f32x2 $d1, $d1;\n");
  op1.assign("$d1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out6 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32x2,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };

  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase6(in, symbols, &out6, &reg1, &reg2);
  TestCase6.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_s_s8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out7 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackS,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase7(in, symbols, &out7, &reg1, &reg2);
  TestCase7.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_p_s8x8 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out8 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase8(in, symbols, &out8, &reg1, &reg2);
  TestCase8.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_s_s8x8 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out9 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackS,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase9(in, symbols, &out9, &reg1, &reg2);
  TestCase9.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_p_s16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out10 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase10(in, symbols, &out10, &reg1, &reg2);
  TestCase10.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_s_s16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out11 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackS,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase11(in, symbols, &out11, &reg1, &reg2);
  TestCase11.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_s32 $s1, 99;\n");
  op1.assign("$s1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;
 
  BrigInstBase out12 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm1 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm1.bits.u = 99;
 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase12(in, symbols, &out12, &reg1, &imm1);
  TestCase12.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_s64 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out13 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase13(in, symbols, &out13, &reg1, &reg2);
  TestCase13.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_f32 $s3,1.0f;\n");
  op1.assign("$s3"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;
 
   BrigInstBase out14 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm2 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm2.bits.f = 1.0f;

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase14(in, symbols, &out14, &reg1, &imm2);
  TestCase14.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_f64 $d3,1.0;\n");
  op1.assign("$d3"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);
 
  align = 8 - size_reg % 8;
 
  BrigInstBase out15 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf64,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm3 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm3.bits.d = 1.0;

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase15(in, symbols, &out15, &reg1, &imm3);
  TestCase15.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_p_f16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out16 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf16x2,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase16(in, symbols, &out16, &reg1, &reg2);
  TestCase16.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_s_u8x4 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out17 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigu8x4,
    BrigPackS,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase17(in, symbols, &out17, &reg1, &reg2);
  TestCase17.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_p_s32x2 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out18 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32x2,
    BrigPackP,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase18(in, symbols, &out18, &reg1, &reg2);
  TestCase18.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "not_b1 $c1, $c2;\n");
  op1.assign("$c1"); op2.assign("$c2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out19 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb1,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase19(in, symbols, &out19, &reg1, &reg2);
  TestCase19.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "not_b32 $s0, $s2;\n");
  op1.assign("$s0"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out20 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase20(in, symbols, &out20, &reg1, &reg2);
  TestCase20.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "not_b64 $d0, $d1;\n");
  op1.assign("$d0"); op2.assign("$d1"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out21 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase21(in, symbols, &out21, &reg1, &reg2);
  TestCase21.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "popcount_b32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out22 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase22(in, symbols, &out22, &reg1, &reg2);
  TestCase22.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "popcount_b64 $s1, $d2;\n");
  op1.assign("$s1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out23 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  reg2.type = Brigb64;
  reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase23(in, symbols, &out23, &reg1, &reg2);
  TestCase23.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "bitrev_s32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out24 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigs32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase24(in, symbols, &out24, &reg1, &reg2);
  TestCase24.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "bitrev_u64 $d1, 0x234;\n");
  op1.assign("$d1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;
 
   BrigInstBase out25 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigu64,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm4 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm4.bits.u = 0x234;

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase25(in, symbols, &out25, &reg1, &imm4);
  TestCase25.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "firstbit_s32 $s0, $s0;\n");
  op1.assign("$s0"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out26 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigs32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase26(in, symbols, &out26, &reg1, &reg2);
  TestCase26.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "firstbit_u64 $s0, $d6;\n");
  op1.assign("$s0"); op2.assign("$d6"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out27 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigu64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  reg2.name = op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase27(in, symbols, &out27, &reg1, &reg2);
  TestCase27.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "lastbit_u32 $s0, $s0;\n");
  op1.assign("$s0"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out28 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigu32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase28(in, symbols, &out28, &reg1, &reg2);
  TestCase28.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "lastbit_s64 $s0, $d6;\n");
  op1.assign("$s0"); op2.assign("$d6"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out29 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigs64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  reg2.name = op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase29(in, symbols, &out29, &reg1, &reg2);
  TestCase29.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "movs_lo_b32 $s1, $d1;\n");
  op1.assign("$s1"); op2.assign("$d1"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out30 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsLo, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  reg2.name = op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase30(in, symbols, &out30, &reg1, &reg2);
  TestCase30.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "movs_hi_b32 $s1, $d1;\n");
  op1.assign("$s1"); op2.assign("$d1"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out31 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsHi, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; 
  reg2.name = op1.size() + 1;
  reg2.type = Brigb64;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase31(in, symbols, &out31, &reg1, &reg2);
  TestCase31.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fract_f32 $s0, 3.2f;\n");
  op1.assign("$s0"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;
 
  BrigInstBase out32 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFract, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm5 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm5.bits.f = 3.2f;

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase32(in, symbols, &out32, &reg1, &imm5);
  TestCase32.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fcos_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out33 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFcos, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase33(in, symbols, &out33, &reg1, &reg2);
  TestCase33.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fsin_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out34 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFsin, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase34(in, symbols, &out34, &reg1, &reg2);
  TestCase34.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "flog2_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0");  
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out35 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFlog2, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase35(in, symbols, &out35, &reg1, &reg2);
  TestCase35.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fexp2_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out36 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFexp2, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase36(in, symbols, &out36, &reg1, &reg2);
  TestCase36.Run_Test(&Instruction2);
  symbols->clear();
 
/**********************************************************************************/
  in.assign( "frsqrt_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out37 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrsqrt, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase37(in, symbols, &out37, &reg1, &reg2);
  TestCase37.Run_Test(&Instruction2);
  symbols->clear();

  
/**********************************************************************************/
  in.assign( "frcp_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out38 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrcp, 
    Brigf32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase38(in, symbols, &out38, &reg1, &reg2);
  TestCase38.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "sqrt_f64 $d1, 1.21;\n");
  op1.assign("$d1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;
 
  BrigInstBase out39 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigSqrt, 
    Brigf64,
    BrigNoPacking,
    {0, size_reg + align, 0, 0, 0}
  };
    
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm6 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm6.bits.d = 1.21;

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase39(in, symbols, &out39, &reg1, &imm6);
  TestCase39.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "unpack3 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out40 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack3, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase40(in, symbols, &out40, &reg1, &reg2);
  TestCase40.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "unpack2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out41 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack2, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase41(in, symbols, &out41, &reg1, &reg2);
  TestCase41.Run_Test(&Instruction2);
  symbols->clear();

 /**********************************************************************************/
  in.assign( "unpack1 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out42 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack1, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase42(in, symbols, &out42, &reg1, &reg2);
  TestCase42.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "unpack0 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out43 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack0, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase43(in, symbols, &out43, &reg1, &reg2);
  TestCase43.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "workitemaid $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out44 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkItemAId, 
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase44(in, symbols, &out44, &reg1, &reg2);
  TestCase44.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "sqrt_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out45 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigSqrt, 
    Brigs8x4,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase45(in, symbols, &out45, &reg1, &reg2);
  TestCase45.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_s32 $s1, $s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out46 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase46(in, symbols, &out46, &reg1, &reg2);
  TestCase46.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "mask_b64 $d1, $d2;");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out47 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase47(in, symbols, &out47, &reg1, &reg2);
  TestCase47.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "count_u32 $s1, $s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out48 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase48(in, symbols, &out48, &reg1, &reg2);
  TestCase48.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "alloca $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out49 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAlloca,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase49(in, symbols, &out49, &reg1, &reg2);
  TestCase49.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "workitemid $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out50 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkItemId,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase50(in, symbols, &out50, &reg1, &reg2);
  TestCase50.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "workgroupid $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out51 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkGroupId,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase51(in, symbols, &out51, &reg1, &reg2);
  TestCase51.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "workgroupsize $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out52 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkGroupSize,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase52(in, symbols, &out52, &reg1, &reg2);
  TestCase52.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "NDRangesize $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
  
  BrigInstBase out53 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNDRangeSize,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase53(in, symbols, &out53, &reg1, &reg2);
  TestCase53.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "NDRangegroups $s1,$s2;");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out54 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNDRangeGroups,
    Brigb32,
    BrigNoPacking,
    {0, size_reg, 0, 0, 0}
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase54(in, symbols, &out54, &reg1, &reg2);
  TestCase54.Run_Test(&Instruction2);
  symbols->clear();
}

/**********************************************************************************/
/**********************************************************************************/
TEST(CodegenTest, Instruction2_With_Modifier_CodeGen) {

/*********************Common variables**********************/
  std::string in; std::string op1, op2;
  StringBuffer *symbols = new StringBuffer();
  BrigOperandReg reg1, reg2;
  
  /*****************************************************************/
  in.assign( "sqrt_ftz_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
  int align;
  int size_reg = sizeof(BrigOperandReg);

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
    {0, size_reg, 0, 0, 0},
    bam
  };
  reg1.size = size_reg;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandReg> TestCase1(in, symbols, &out1, &reg1, &reg2);
  TestCase1.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "abs_ftz_p_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);

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
    {0, size_reg, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandReg> TestCase2(in, symbols, &out2, &reg1, &reg2);
  TestCase2.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fract_ftz_f32 $s0, 1.6f;\n");
  op1.assign("$s0"); op2.assign("");  
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;

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
    {0, size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm1 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm1.bits.f = 1.6f;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase3(in, symbols, &out3, &reg1, &imm1);
  TestCase3.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_ftz_p_s32 $s1, 1.0;\n");
  op1.assign("$s1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;

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
    {0, size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm2 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm2.bits.d = 1.0;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase4(in, symbols, &out4, &reg1, &imm2);
  TestCase4.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_upi_s32 $s1, 99;\n");
  op1.assign("$s1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  align = 8 - size_reg % 8;

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
    {0, size_reg + align, 0, 0, 0},
    bam
  };

  reg1.size = sizeof(BrigOperandReg);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm3 = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm3.bits.u = 99;

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase5(in, symbols, &out5, &reg1, &imm3);
  TestCase5.Run_Test(&Instruction2);
  symbols->clear();
}

} //namespace brig
} //namespace hsa
