#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

template <typename TInst, typename T1, typename T2, typename T3 = BrigOperandReg> 
class Mul_Test: public BrigCodeGenTest {

private:
  const TInst* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const T1* RefSrc1;
  const T2* RefSrc2;
  const T3* RefSrc3;

public:
  Mul_Test(std::string& in, StringBuffer* sbuf, TInst* ref,
           BrigOperandReg* Dest, T1* Src1, T2* Src2, T3* Src3 = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2),
    RefSrc3(Src3) { }
 
  void validate(struct BrigSections* TestOutput) {
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const TInst* getinst = (cast<TInst>(getcode));
    validate_brig::validate(RefInst, getinst);

    const BrigOperandReg* getdest = reinterpret_cast <const BrigOperandReg*> 
                                    (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(RefDest, refbuf, getdest, getbuf);
    
    const T1 *getsrc1 = reinterpret_cast <const T1*> 
                        (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validateOpType<T1>(RefSrc1, refbuf, getsrc1, getbuf);

    const T2 *getsrc2 = reinterpret_cast <const T2*> 
                        (&(TestOutput->operands[getinst->o_operands[2]]));
    validate_brig::validateOpType<T2>(RefSrc2, refbuf, getsrc2, getbuf);
    
    if (RefSrc3 != NULL) {
      const T3 *getsrc3 = reinterpret_cast <const T3*> 
                          (&(TestOutput->operands[getinst->o_operands[3]]));
      validate_brig::validateOpType<T3>(RefSrc3, refbuf, getsrc3, getbuf);      
    } else {
      EXPECT_EQ(0, getinst->o_operands[3]);
    }
    EXPECT_EQ(0, getinst->o_operands[4]);
  }
};


TEST(CodegenTest, Mul_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; 
  std::string destName, src1Name, src2Name;
  StringBuffer* symbols;
  
  BrigOperandReg dest;
  BrigInstBase out;

  BrigOperandReg reg1, reg2;
  BrigOperandWaveSz wav1, wav2;
  BrigOperandImmed imm1, imm2;

  symbols = new StringBuffer();
   
  /************************************* Test Case 1 ************************************/
  in.assign("mul_u32 $s1 ,$s2, $s3;\n");
  destName.assign("$s1");   src1Name.assign("$s2");
  src2Name.assign("$s3"); 
  symbols->append(destName);  symbols->append(src1Name);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + src1Name.size() + 2;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase1(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase1.Run_Test(&Mul);
  symbols->clear();


  /************************************* Test Case 2 ************************************/
  in.assign("mul_hi_u32 $s1, $s2, $s9;\n");
  destName.assign("$s1");   src1Name.assign("$s2");
  src2Name.assign("$s9"); 
  symbols->append(destName);  symbols->append(src1Name);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMulHi;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + src1Name.size() + 2;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase2(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase2.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("mul_f32 $s3,1.0f,$s1;\n");
  destName.assign("$s3");  src2Name.assign("$s1"); 
  symbols->append(destName);  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul;
  out.type = Brigf32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.f = 1.0f;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + 1;
  
  Mul_Test<BrigInstBase, BrigOperandImmed, BrigOperandReg> TestCase3(in, symbols, &out, &dest, &imm1, &reg2);
  TestCase3.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("mul_f64 $d3,$d0,1.0;\n");
  destName.assign("$d3");  src2Name.assign("$d0"); 
  symbols->append(destName);  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul;
  out.type = Brigf64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  *(double*)(&imm2.bits.l[0]) = 1.0;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase4(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase4.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 5 ************************************/
  in.assign("mul_pp_u16x4 $d1, $d0, $d3;\n");
  destName.assign("$d1");  src1Name.assign("$d0"); 
  src2Name.assign("$d3");
  symbols->append(destName);  symbols->append(src1Name);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul;
  out.type = Brigu16x4;
  out.packing = BrigPackPP;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = destName.size() + src1Name.size() + 2;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase5(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase5.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 6 ************************************/
  in.assign("mul_hi_pp_u8x8 $d1, $d3, $d4;\n");
  destName.assign("$d1");  src1Name.assign("$d3"); 
  src2Name.assign("$d4");
  symbols->append(destName);  symbols->append(src1Name);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMulHi;
  out.type = Brigu8x8;
  out.packing = BrigPackPP;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.name = destName.size() + src1Name.size() + 2;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase6(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase6.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 7 ************************************/
  in.assign("mul24_u32 $s1, $s2, 12;\n");
  destName.assign("$s1");  src1Name.assign("$s2"); 
  symbols->append(destName);  symbols->append(src1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul24;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 12;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase7(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase7.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 8 ************************************/
  in.assign("mul24_hi_s32 $s1, $s2, -12;\n");
  destName.assign("$s1");  src1Name.assign("$s2"); 
  symbols->append(destName);  symbols->append(src1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul24Hi;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = -12;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> TestCase8(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase8.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 9 ************************************/
  in.assign("mad24_u64 $d1, $d2, 12, 2;\n");
  destName.assign("$d1");  src1Name.assign("$d2"); 
  symbols->append(destName);  symbols->append(src1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMad24;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = sizeof(reg1) + sizeof(dest) + sizeof(imm1);
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;


  // TODO(Chuang): the type of immediate should be Brigb64.
  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 12;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 2;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> TestCase9(in, symbols, &out, &dest, &reg1, &imm1, &imm2);
  TestCase9.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 10 ************************************/
  in.assign("mad24_hi_s32 $s1, $s2, -12, 23;\n");
  destName.assign("$s1");  src1Name.assign("$s2"); 
  symbols->append(destName);  symbols->append(src1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMad24Hi;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = sizeof(reg1) + sizeof(dest) + sizeof(imm1);
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = -12;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 23;
  
  Mul_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed> TestCase10(in, symbols, &out, &dest, &reg1, &imm1, &imm2);
  TestCase10.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 11 ************************************/
  in.assign("mul_u32 $s1 , WAVESIZE, WAVESIZE;\n");
  destName.assign("$s1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigMul;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(wav1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;
  
  wav2 = wav1;
    
  Mul_Test<BrigInstBase, BrigOperandWaveSz, BrigOperandWaveSz> TestCase11(in, symbols, &out, &dest, &wav1, &wav2);
  TestCase11.Run_Test(&Mul);
  symbols->clear();

  delete symbols;
}


TEST(CodegenTest, Mul_AluModifier_CodeGen) {
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; 
  std::string destName, src1Name, src2Name;
  StringBuffer* symbols;
  
  BrigOperandReg dest;
  BrigInstMod out;

  BrigOperandReg reg1, reg2;
  BrigOperandImmed imm1, imm2;

  symbols = new StringBuffer();
   
  /************************************* Test Case 1 ************************************/
  in.assign("mul_ftz_up_f32 $s1 ,$s2, $s3;\n");
  destName.assign("$s1");   src1Name.assign("$s2");
  src2Name.assign("$s3"); 
  symbols->append(destName);  symbols->append(src1Name);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstMod;
  out.opcode = BrigMul;
  out.type = Brigf32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.aluModifier.floatOrInt = 1;
  out.aluModifier.rounding = 2;
  out.aluModifier.ftz = 1;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + src1Name.size() + 2;
  
  Mul_Test<BrigInstMod, BrigOperandReg, BrigOperandReg> TestCase1(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase1.Run_Test(&Mul);
  symbols->clear();

  /************************************* Test Case 2 ************************************/
  in.assign("mul_ftz_f64 $d3,1.2, 1.3;\n");
  destName.assign("$d3");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstMod;
  out.opcode = BrigMul;
  out.type = Brigf64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.aluModifier.floatOrInt = 1;
  out.aluModifier.ftz = 1;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  *(double*)(&imm1.bits.l[0]) = 1.2;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  *(double*)(&imm2.bits.l[0]) = 1.3;
  
  Mul_Test<BrigInstMod, BrigOperandImmed, BrigOperandImmed> TestCase2(in, symbols, &out, &dest, &imm1, &imm2);
  TestCase2.Run_Test(&Mul);
  symbols->clear();

}

} // namespace hsa
} // namespace brig




