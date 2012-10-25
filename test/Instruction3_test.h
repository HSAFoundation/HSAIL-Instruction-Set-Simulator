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

    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);
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
  StringBuffer* symbols;

  BrigOperandReg reg1, reg2, reg3;
  BrigOperandWaveSz wav;
  BrigOperandImmed imm2, imm3;
  BrigInstBase Out;


  /************************************* Test Case 1************************************/
  symbols = new StringBuffer();
  in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  Out.size = sizeof(Out);
  Out.kind = BrigEInstBase;
  Out.opcode = BrigAdd;
  Out.type = Brigu16x2;
  Out.packing = BrigPackPPsat;
  Out.o_operands[0] = 0; Out.o_operands[1] = sizeof(reg1);
  Out.o_operands[2] = sizeof(reg1) + sizeof(reg2); Out.o_operands[3] = 0; Out.o_operands[4] = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg3 = reg1;
  reg2.name = op1.size()+1; reg3.name = op1.size()+1 + op2.size()+1;
  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase1(in, symbols, &Out, &reg1, &reg2, &reg3);
  TestCase1.Run_Test(&Instruction3);
  symbols->clear();


  /******************************************** Test case 2 reg , reg , reg ************/

  in.assign( "add_s64 $d1, $d2, $d3;\n");
  op1.assign("$d1"); op2.assign("$d2"); op3.assign("$d3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigAdd,
    Brigs64,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     0, 0}
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
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase2(in, symbols, &out2, &reg1, &reg2, &reg3);
  TestCase2.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* Test case 3 reg , WAVESIZE , reg ***************/
  in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
  op1.assign("$d1"); op3.assign("$d3");
  symbols->append(op1); symbols->append(op3);

  BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigAdd,
    Brigu64,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(wav),
     0, 0}
  };
  out3.size = sizeof(out3);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  wav.size = sizeof(wav);
  wav.kind = BrigEOperandWaveSz;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandWaveSz, BrigOperandReg>
            TestCase3(in, symbols, &out3, &reg1, &wav, &reg3);
  TestCase3.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* case 4 reg , immed , WAVESIZE *******/
  in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
  op1.assign("$d1"); symbols->append(op1);
  BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigAdd,
    Brigs16x4,
    BrigPackPSsat,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(imm2),
     0, 0}
  };
  out4.size = sizeof(out4);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 0x40;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandWaveSz>
            TestCase4(in, symbols, &out4, &reg1, &imm2, &wav);
  TestCase4.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* case 5 reg , immed , immed *******/
  in.assign( "div_s32 $s1, 100, 10;\n");
  op1.assign("$s1"); symbols->append(op1);
  BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigDiv,
    Brigs32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(imm2),
     0, 0}
  };
  out5.size = sizeof(out5);
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
  imm2.bits.u = 100;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 10;


  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase5(in, symbols, &out5, &reg1, &imm2, &imm3);
  TestCase5.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* case 6 reg , reg , immed *******/
  in.assign( "div_u64 $d1, $d3, 0x1234945;\n");
  op1.assign("$d1"); op2.assign("$d3");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigDiv,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2) , 0, 0}
  };
  out6.size = sizeof(out6);

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
  imm3.bits.u = 0x1234945;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase6(in, symbols, &out6, &reg1, &reg2, &imm3);
  TestCase6.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* case 7 reg , reg , WAVESIZE *******/
  in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
  op1.assign("$d1"); op2.assign("$d3");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigRem,
    Brigs64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandWaveSz>
            TestCase7(in, symbols, &out7, &reg1, &reg2, &wav);
  TestCase7.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 8 reg , immed , immed *******/
  in.assign( "rem_u64 $d1, 0x040, 0x12349456;\n");
  op1.assign("$d1"); symbols->append(op1);

  BrigInstBase out8 = {
    0,
    BrigEInstBase,
    BrigRem,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(imm2),
     0, 0}
  };
  out8.size = sizeof(out8);

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.u = 0x040;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 0x12349456;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase8(in, symbols, &out8, &reg1, &imm2, &imm3);
  TestCase8.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 9 reg , reg , immed *******/
  in.assign( "shl_u32 $s1, $s1, 2;\n");
  op1.assign("$s1");  symbols->append(op1);
  BrigInstBase out9 = {
    0,
    BrigEInstBase,
    BrigShl,
    Brigu32,
    BrigNoPacking,
    {0, 0, sizeof(reg1), 0, 0}
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
  reg2.name = 0;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase9(in, symbols, &out9, &reg1, &reg2, &imm3);
  TestCase9.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 10 reg , reg , immed *******/
  in.assign("class_f32 $c1, $s2, 0x10;");
  op1.assign("$c1"); op2.assign("$s2");
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out10 = {
    0,
    BrigEInstBase,
    BrigClass,
    Brigb1,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg2) * 2, 0, 0}
  };
  out10.size = sizeof(out10);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 0x10;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase10(in, symbols, &out10, &reg1, &reg2, &imm3);
  TestCase10.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 11 reg , reg , reg *******/
  in.assign("class_f64 $c0, $d1, $s1;");
  op1.assign("$c0"); op2.assign("$d1"); op3.assign("$s1");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out11 = {
    0,
    BrigEInstBase,
    BrigClass,
    Brigb1,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out11.size = sizeof(out11);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase11(in, symbols, &out11, &reg1, &reg2, &reg3);
  TestCase11.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 12 reg , reg , immed *******/
  in.assign( "borrow_u64 $d1, $d3, 0x45;\n");
  op1.assign("$d1"); op2.assign("$d3");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out12 = {
    0,
    BrigEInstBase,
    BrigBorrow,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out12.size = sizeof(out12);

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
  imm3.bits.u = 0x45;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase12(in, symbols, &out12, &reg1, &reg2, &imm3);
  TestCase12.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 13 reg , reg , reg *******/
  in.assign("sub_sp_u8x8 $d1, $d0, $d3;\n");
  op1.assign("$d1"); op2.assign("$d0"); op3.assign("$d3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out13 = {
    0,
    BrigEInstBase,
    BrigSub,
    Brigu8x8,
    BrigPackSP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out13.size = sizeof(out13);

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
  reg3.name = op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase13(in, symbols, &out13, &reg1, &reg2, &reg3);
  TestCase13.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 14 reg , reg , immed *******/
  in.assign( "shr_u8x4 $s1, $s2, 2;\n");
  op1.assign("$s1"); op2.assign("$s2");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out14 = {
    0,
    BrigEInstBase,
    BrigShr,
    Brigu8x4,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase14(in, symbols, &out14, &reg1, &reg2, &imm3);
  TestCase14.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 15 reg , reg , reg *******/
  in.assign("and_b1 $c0, $c2, $c3;\n");
  op1.assign("$c0"); op2.assign("$c2"); op3.assign("$c3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out15 = {
    0,
    BrigEInstBase,
    BrigAnd,
    Brigb1,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out15.size = sizeof(out15);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb1;
  reg2.reserved = 0;
  reg2.name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb1;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase15(in, symbols, &out15, &reg1, &reg2, &reg3);
  TestCase15.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 16 reg , reg , reg *******/
  in.assign("or_b32 $s0, $s2, $s3;\n");
  op1.assign("$s0"); op2.assign("$s2"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out16 = {
    0,
    BrigEInstBase,
    BrigOr,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase16(in, symbols, &out16, &reg1, &reg2, &reg3);
  TestCase16.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 17 reg , reg , reg *******/
  in.assign("xor_b64 $d0, $d1, $d2;\n");
  op1.assign("$d0"); op2.assign("$d1"); op3.assign("$d2");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out17 = {
    0,
    BrigEInstBase,
    BrigXor,
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out17.size = sizeof(out17);

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
  reg3.name = op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase17(in, symbols, &out17, &reg1, &reg2, &reg3);
  TestCase17.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 18 reg , reg , immed *******/
  in.assign( "unpacklo_u8x4 $s1, $s2, 72;\n");
  op1.assign("$s1"); op2.assign("$s2");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out18 = {
    0,
    BrigEInstBase,
    BrigUnpackLo,
    Brigu8x4,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 72;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase18(in, symbols, &out18, &reg1, &reg2, &imm3);
  TestCase18.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 19 reg , reg , reg *******/
  in.assign("unpackhi_f16x2 $s3, $s3,$s4;\n");
  op1.assign("$s3");  op3.assign("$s4");
  symbols->append(op1); symbols->append(op3);

  BrigInstBase out19 = {
    0,
    BrigEInstBase,
    BrigUnpackHi,
    Brigf16x2,
    BrigNoPacking,
    {0, 0, sizeof(reg1), 0, 0}
  };
  out19.size = sizeof(out19);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = 0;

  reg2 = reg1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase19(in, symbols, &out19, &reg1, &reg2, &reg3);
  TestCase19.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 20 reg , reg , immed *******/
  in.assign( "movd_lo_b64 $d2, $d1, 23;");
  op1.assign("$d2"); op2.assign("$d1");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out20 = {
    0,
    BrigEInstBase,
    BrigMovdLo,
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out20.size = sizeof(out20);

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
  imm3.bits.u = 23;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase20(in, symbols, &out20, &reg1, &reg2, &imm3);
  TestCase20.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 21 reg , reg , reg *******/
  in.assign("movd_hi_b64 $d2, $d1, $s1;\n");
  op1.assign("$d2"); op2.assign("$d1"); op3.assign("$s1");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out21 = {
    0,
    BrigEInstBase,
    BrigMovdHi,
    Brigb64,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out21.size = sizeof(out21);

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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase21(in, symbols, &out21, &reg1, &reg2, &reg3);
  TestCase21.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 22 reg , reg , reg *******/
  in.assign("copysign_f64 $d3,$d2,$d1;\n");
  op1.assign("$d3"); op2.assign("$d2"); op3.assign("$d1");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out22 = {
    0,
    BrigEInstBase,
    BrigCopySign,
    Brigf64,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out22.size = sizeof(out22);

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
  reg3.name = op1.size() + op2.size() + 2;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase22(in, symbols, &out22, &reg1, &reg2, &reg3);
  TestCase22.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 23 reg , imm , reg *******/
  in.assign("send_b32 $s1, 3, $s2;\n");
  op1.assign("$s1"); op3.assign("$s2");
  symbols->append(op1); symbols->append(op3);

  BrigInstBase out23 = {
    0,
    BrigEInstBase,
    BrigSend,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out23.size = sizeof(out23);

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
  imm2.bits.u = 3;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase23(in, symbols, &out23, &reg1, &imm2, &reg3);
  TestCase23.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 24 reg , imm , reg *******/
  in.assign("receive_b32 $s1, 3.4f, $s2;\n");
  op1.assign("$s1"); op3.assign("$s2");
  symbols->append(op1); symbols->append(op3);

  BrigInstBase out24 = {
    0,
    BrigEInstBase,
    BrigReceive,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out24.size = sizeof(out24);

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
  imm2.bits.f = 3.4f;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase24(in, symbols, &out24, &reg1, &imm2, &reg3);
  TestCase24.Run_Test(&Instruction3);
  symbols->clear();

  /*********************************  case 25 reg , reg , immed *******/
  in.assign( "carry_u64 $d1, $d3, 0x44;\n");
  op1.assign("$d1"); op2.assign("$d3");
  symbols->append(op1); symbols->append(op2);
  BrigInstBase out25 = {
    0,
    BrigEInstBase,
    BrigCarry,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(reg1),
     sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out25.size = sizeof(out25);

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
  imm3.bits.u = 0x44;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase25(in, symbols, &out25, &reg1, &reg2, &imm3);
  TestCase25.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 26 reg , reg , reg *******/
  in.assign("max_pp_u8x4 $s1, $s0, $s3;\n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out26 = {
    0,
    BrigEInstBase,
    BrigMax,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out26.size = sizeof(out26);

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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase26(in, symbols, &out26, &reg1, &reg2, &reg3);
  TestCase26.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 27 reg , reg , immed *******/
  in.assign("max_pp_u8x4 $s1, $s0, 34;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out27 = {
    0,
    BrigEInstBase,
    BrigMax,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out27.size = sizeof(out27);

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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 34;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase27(in, symbols, &out27, &reg1, &reg2, &imm3);
  TestCase27.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 28 reg , immed , reg *******/
  in.assign("max_pp_u8x4 $s1, 33, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out28= {
    0,
    BrigEInstBase,
    BrigMax,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out28.size = sizeof(out28);

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
  imm2.bits.u = 33;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;


  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase28(in, symbols, &out28, &reg1, &imm2, &reg3);
  TestCase28.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 29 reg , immed , immed *******/
  in.assign("max_u8x4 $s1, 33, 134;\n");
  op1.assign("$s1");
  symbols->append(op1);

  BrigInstBase out29= {
    0,
    BrigEInstBase,
    BrigMax,
    Brigu8x4,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out29.size = sizeof(out29);

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
  imm2.bits.u = 33;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 134;


  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase29(in, symbols, &out29, &reg1, &imm2, &imm3);
  TestCase29.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 30 reg , reg , reg *******/
  in.assign("min_pp_u8x4 $s1, $s0, $s3;\n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstBase out30 = {
    0,
    BrigEInstBase,
    BrigMin,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out30.size = sizeof(out30);

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

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase30(in, symbols, &out30, &reg1, &reg2, &reg3);
  TestCase30.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 31 reg , reg , immed *******/
  in.assign("min_pp_u8x4 $s1, $s0, 3;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out31 = {
    0,
    BrigEInstBase,
    BrigMin,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0}
  };
  out31.size = sizeof(out31);

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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 3;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase31(in, symbols, &out31, &reg1, &reg2, &imm3);
  TestCase31.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 32 reg , immed , reg *******/
  in.assign("min_pp_u8x4 $s1, 3, $s0 ;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstBase out32 = {
    0,
    BrigEInstBase,
    BrigMin,
    Brigu8x4,
    BrigPackPP,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out32.size = sizeof(out32);

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
  imm2.bits.u = 3;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase32(in, symbols, &out32, &reg1, &imm2, &reg3);
  TestCase32.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 33 reg , immed , immed *******/
  in.assign("min_u8x4 $s1, 3, 4 ;\n");
  op1.assign("$s1");
  symbols->append(op1);

  BrigInstBase out33 = {
    0,
    BrigEInstBase,
    BrigMin,
    Brigu8x4,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0}
  };
  out33.size = sizeof(out33);

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
  imm2.bits.u = 3;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.u = 4;

  Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase33(in, symbols, &out33, &reg1, &imm2, &imm3);
  TestCase33.Run_Test(&Instruction3);
  symbols->clear();


  /********************************* case 34 reg , reg , reg *******/
  in.assign("max_ftz_u16x2 $s1, $s0, $s3;\n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstMod out34 = {
    0,
    BrigEInstMod,
    BrigMax,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out34.size = sizeof(out34);

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

  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase34(in, symbols, &out34, &reg1, &reg2, &reg3);
  TestCase34.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 35 reg , reg , immed *******/
  in.assign("max_ftz_u16x2 $s1, $s0, 3.14f;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out35 = {
    0,
    BrigEInstMod,
    BrigMax,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out35.size = sizeof(out35);

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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 3.14f;

  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase35(in, symbols, &out35, &reg1, &reg2, &imm3);
  TestCase35.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 36 reg , immed , reg *******/
  in.assign("max_ftz_u16x2 $s1, 3.14f, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out36= {
    0,
    BrigEInstMod,
    BrigMax,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out36.size = sizeof(out36);

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
  imm2.bits.f = 3.14f;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;


  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase36(in, symbols, &out36, &reg1, &imm2, &reg3);
  TestCase36.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 37 reg , immed , immed *******/
  in.assign("max_ftz_u16x2 $s1, 3.14f, 3.1415f;\n");
  op1.assign("$s1");
  symbols->append(op1);

  BrigInstMod out37= {
    0,
    BrigEInstMod,
    BrigMax,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out37.size = sizeof(out37);

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
  imm2.bits.f = 3.14f;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 3.1415f;


  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase37(in, symbols, &out37, &reg1, &imm2, &imm3);
  TestCase37.Run_Test(&Instruction3);
  symbols->clear();

  /********************************* case 38 reg , reg , reg *******/
  in.assign("min_ftz_u16x2 $s1, $s0, $s3;\n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

  BrigInstMod out38 = {
    0,
    BrigEInstMod,
    BrigMin,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out38.size = sizeof(out38);

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


  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase38(in, symbols, &out38, &reg1, &reg2, &reg3);
  TestCase38.Run_Test(&Instruction3);

  symbols->clear();

 /********************************* case 39 reg , reg , immed *******/

  in.assign("min_ftz_u16x2 $s1, $s0, 3.14f;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out39 = {
    0,
    BrigEInstMod,
    BrigMin,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(reg2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out39.size = sizeof(out39);

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

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 3.14f;

  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase39(in, symbols, &out39, &reg1, &reg2, &imm3);
  TestCase39.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 40 reg , immed , reg *******/
  in.assign("min_ftz_u16x2 $s1, 3.14f, $s0;\n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

  BrigInstMod out40= {
    0,
    BrigEInstMod,
    BrigMin,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out40.size = sizeof(out40);

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
  imm2.bits.f = 3.14f;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = op1.size() + 1;


  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandReg>
            TestCase40(in, symbols, &out40, &reg1, &imm2, &reg3);
  TestCase40.Run_Test(&Instruction3);
  symbols->clear();

 /********************************* case 41 reg , immed , immed *******/
  in.assign("min_ftz_u16x2 $s1, 3.14f, 3.1415f;\n");
  op1.assign("$s1");
  symbols->append(op1);

  BrigInstMod out41= {
    0,
    BrigEInstMod,
    BrigMin,
    Brigu16x2,
    BrigNoPacking,
    {0, sizeof(reg1), sizeof(reg1) + sizeof(imm2), 0, 0},
    {1,0,0,1,0,0,0}
  };
  out41.size = sizeof(out41);

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
  imm2.bits.f = 3.14f;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 3.1415f;


  Instruction3Opcode_Test<BrigInstMod, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase41(in, symbols, &out41, &reg1, &imm2, &imm3);
  TestCase41.Run_Test(&Instruction3);
  symbols->clear();

  /***************************************  End of tests *************************************/
  delete symbols;
}

} //namespace hsa
} //namespace brig




