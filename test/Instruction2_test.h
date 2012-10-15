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
  
TEST(CodegenTest, Instruction2_CodeGen){

/*********************Common variables**********************/
  std::string in; std::string op1, op2;
  StringBuffer *symbols = new StringBuffer();
  BrigOperandReg reg1, reg2;
  
  /*****************************************************************/
  in.assign( "abs_s32 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
  
  BrigInstBase out1 = {
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out1.size = sizeof(out1);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out2.size = sizeof(out2);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out3.size = sizeof(out3);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigf64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out4.size = sizeof(out4);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out5.size = sizeof(out5);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigf32x2,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out6.size = sizeof(out6);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackS,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out7.size = sizeof(out7);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out8.size = sizeof(out8);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackS,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out9.size = sizeof(out9);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out10.size = sizeof(out10);
 
  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackS,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out11.size = sizeof(out11);

  reg1.size = sizeof(reg1);
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

  BrigInstBase out12 = {
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigs32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out12.size = sizeof(out12);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm1.bits.u = 99;
  imm1.size = sizeof(imm1);
 
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase12(in, symbols, &out12, &reg1, &imm1);
  TestCase12.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_s64 $d1, $d2;\n");
  op1.assign("$d1"); op2.assign("$d2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out13 = {
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigs64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out13.size = sizeof(out13);

  reg1.size = sizeof(reg1);
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
 
   BrigInstBase out14 = {
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out14.size = sizeof(out14);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm2.bits.f = 1.0f;
  imm2.size = sizeof(imm2);

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase14(in, symbols, &out14, &reg1, &imm2);
  TestCase14.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_f64 $d3,1.0;\n");
  op1.assign("$d3"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out15 = {
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigf64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out15.size = sizeof(out15);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm3 = {
    0,
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm3.bits.d = 1.0;
  imm3.size = sizeof(imm3);

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase15(in, symbols, &out15, &reg1, &imm3);
  TestCase15.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_p_f16x2 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out16 = {
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigf16x2,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out16.size = sizeof(out16);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigu8x4,
    BrigPackS,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out17.size = sizeof(out17);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNeg, 
    Brigs32x2,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out18.size = sizeof(out18);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNot, 
    Brigb1,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out19.size = sizeof(out19);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNot, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out20.size = sizeof(out20);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNot, 
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out21.size = sizeof(out21);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigPopcount, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out22.size = sizeof(out22);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigPopcount, 
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out23.size = sizeof(out23);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigBitRev, 
    Brigs32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out24.size = sizeof(out24);

  reg1.size = sizeof(reg1);
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

   BrigInstBase out25 = {
    0,
    BrigEInstBase, 
    BrigBitRev, 
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out25.size = sizeof(out25);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm4 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm4.bits.u = 0x234;
  imm4.size = sizeof(imm4);

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase25(in, symbols, &out25, &reg1, &imm4);
  TestCase25.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "firstbit_s32 $s0, $s0;\n");
  op1.assign("$s0"); 
  symbols->append(op1); 
 
  BrigInstBase out26 = {
    0,
    BrigEInstBase, 
    BrigFirstbit, 
    Brigs32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out26.size = sizeof(out26);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFirstbit, 
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out27.size = sizeof(out27);

  reg1.size = sizeof(reg1);
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
  op1.assign("$s0"); 
  symbols->append(op1); 
 
  BrigInstBase out28 = {
    0,
    BrigEInstBase, 
    BrigLastbit, 
    Brigu32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out28.size = sizeof(out28);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigLastbit, 
    Brigs64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out29.size = sizeof(out29);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigMovsLo, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out30.size = sizeof(out30);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigMovsHi, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out31.size = sizeof(out31);

  reg1.size = sizeof(reg1);
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
  op1.assign("$s0"); 
  symbols->append(op1); 

  BrigInstBase out32 = {
    0,
    BrigEInstBase, 
    BrigFract, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out32.size = sizeof(out32);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm5 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm5.bits.f = 3.2f;
  imm5.size = sizeof(imm5);

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase32(in, symbols, &out32, &reg1, &imm5);
  TestCase32.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "fcos_f32 $s1, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out33 = {
    0,
    BrigEInstBase, 
    BrigFcos, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out33.size = sizeof(out33);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFsin, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out34.size = sizeof(out34);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFlog2, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out35.size = sizeof(out35);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFexp2, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out36.size = sizeof(out36);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFrsqrt, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out37.size = sizeof(out37);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigFrcp, 
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out38.size = sizeof(out38);

  reg1.size = sizeof(reg1);
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
  op1.assign("$d1"); 
  symbols->append(op1); 

  BrigInstBase out39 = {
    0,
    BrigEInstBase, 
    BrigSqrt, 
    Brigf64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out39.size = sizeof(out39);
    
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm6 = {
    0,
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm6.bits.d = 1.21;
  imm6.size = sizeof(imm6);

  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase39(in, symbols, &out39, &reg1, &imm6);
  TestCase39.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "unpack3 $s1, $s2;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out40 = {
    0,
    BrigEInstBase, 
    BrigUnpack3, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out40.size = sizeof(out40);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigUnpack2, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out41.size = sizeof(out41);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigUnpack1, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out42.size = sizeof(out42);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigUnpack0, 
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out43.size = sizeof(out43);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase43(in, symbols, &out43, &reg1, &reg2);
  TestCase43.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  
/**********************************************************************************/
  in.assign( "sqrt_s8x4 $s1, $s2; \n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2);
 
  BrigInstBase out45 = {
    0,
    BrigEInstBase, 
    BrigSqrt, 
    Brigs8x4,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out45.size = sizeof(out45);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out46.size = sizeof(out46);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out47.size = sizeof(out47);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out48.size = sizeof(out48);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstBase, 
    BrigAlloca,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0}
  };
  out49.size = sizeof(out49);

  reg1.size = sizeof(reg1);;
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1; reg2.name = op1.size() + 1;
  Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase49(in, symbols, &out49, &reg1, &reg2);
  TestCase49.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  
/**********************************************************************************/
 
/**********************************************************************************/
 
/**********************************************************************************/
 delete symbols;
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

  BrigAluModifier bam;
  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out1 = {
    0,
    BrigEInstMod,      
    BrigSqrt,         
    Brigs8x4,          
    BrigNoPacking,     
    {0, sizeof(reg1), 0, 0, 0},
    bam
  };
  out1.size = sizeof(out1);

  reg1.size = sizeof(reg1);
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
    0,
    BrigEInstMod,      
    BrigAbs,           
    Brigs8x4,          
    BrigPackP,     
    {0, sizeof(reg1), 0, 0, 0},
    bam
  };
  out2.size = sizeof(out2);

  reg1.size = sizeof(reg1);
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
  op1.assign("$s0"); 
  symbols->append(op1); 

  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out3 = {
    0,
    BrigEInstMod,      
    BrigFract,           
    Brigf32,          
    BrigNoPacking,     
    {0, sizeof(reg1), 0, 0, 0},
    bam
  };
  out3.size = sizeof(out3);
 
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm1.bits.f = 1.6f;
  imm1.size = sizeof(imm1);

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase3(in, symbols, &out3, &reg1, &imm1);
  TestCase3.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_ftz_p_s32 $s1, 1.0;\n");
  op1.assign("$s1"); op2.assign(""); 
  symbols->append(op1); symbols->append(op2);

  bam.ftz = 1;
  bam.floatOrInt = 0;
  bam.rounding = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out4 = {
    0,
    BrigEInstMod,      
    BrigNeg,           
    Brigs32,          
    BrigPackP,     
    {0, sizeof(reg1), 0, 0, 0},
    bam
  };
  out4.size = sizeof(out4);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm2 = {
    0,
    BrigEOperandImmed,
    Brigb64,
    0,
    {0} 
  };
  imm2.bits.d = 1.0;
  imm2.size = sizeof(imm2);

  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase4(in, symbols, &out4, &reg1, &imm2);
  TestCase4.Run_Test(&Instruction2);
  symbols->clear();

/**********************************************************************************/
  in.assign( "neg_upi_s32 $s1, 99;\n");
  op1.assign("$s1"); 
  symbols->append(op1); 

  bam.rounding = 2;
  bam.ftz = 0;
  bam.floatOrInt = 0;
  bam.hi = 0;
  bam.approx = 0;
  bam.fbar = 0;
  bam.reserved = 0;

  BrigInstMod out5 = {
    0,
    BrigEInstMod,      
    BrigNeg,           
    Brigs32,          
    BrigNoPacking,    
    {0, sizeof(reg1), 0, 0, 0},
    bam
  };
  out5.size = sizeof(out5);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  BrigOperandImmed imm3 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0} 
  };
  imm3.bits.u = 99;
  imm3.size = sizeof(imm3);
  Instruction2_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed> TestCase5(in, symbols, &out5, &reg1, &imm3);
  TestCase5.Run_Test(&Instruction2);
  symbols->clear();
  
  /******************************  End of tests *****************************************/
  delete symbols; 
}

TEST(CodegenTest, MemorySegmentOps){
  
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  in.assign("stof_private_u64 $d2, $d1;\n");
  op1.assign("$d2"); op2.assign("$d1");
  sbuf->append(op1); sbuf->append(op2);
  
  BrigInstMem ref1 = {
    0,                    // size
    BrigEInstMem,          // kind
    BrigStoF,              // opcode
    Brigu64,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},      // o_operands[5]
    BrigPrivateSpace       // storageClass
  };
  ref1.size = sizeof(ref1);
  
  BrigOperandReg dest1 = {
  0,
  BrigEOperandReg,
  Brigb64,
  0,
  0
  };
  dest1.size = sizeof(dest1);
  ref1.o_operands[1] = sizeof(dest1);
  
  BrigOperandReg src1 = {
  0,
  BrigEOperandReg,
  Brigb64,
  0,
  op1.size()+1
  };
  src1.size = sizeof(src1);
  
  Instruction2_Test<BrigInstMem, BrigOperandReg, BrigOperandReg> TestCase1(in, sbuf, &ref1, &dest1, &src1);
  TestCase1.Run_Test(&Segp);
  sbuf->clear();
  
  delete sbuf;
  
}

} //namespace brig
} //namespace hsa
