#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

template <typename T1, typename T2> 
class Cmp_Test: public BrigCodeGenTest {

private:
  const BrigInstCmp* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const T1* RefSrc1;
  const T2* RefSrc2;

public:
  Cmp_Test(std::string& in, StringBuffer* sbuf, BrigInstCmp* ref, 
               BrigOperandReg* Dest, T1* Src1, T2* Src2):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2) { }
 
  void validate(struct BrigSections* TestOutput) {
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstCmp* getinst = (cast<BrigInstCmp>(getcode));
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
    
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);  
         
  }
};


TEST(CodegenTest, Cmp_CodeGen) {
  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std::string in; 
  std::string destName, op1Name, op2Name;
  StringBuffer* symbols;
  
  BrigOperandReg dest;

  BrigInstCmp out;

  BrigOperandReg reg1, reg2;
  BrigOperandWaveSz wav1, wav2;
  BrigOperandImmed imm1, imm2;

  symbols = new StringBuffer();
  // TODO(Chuang): add more cases about Cmp Inst with aluModifier.
  /************************************* Test Case 1 ************************************/
  in.assign("cmp_ne_f32_b1 $s1, $c2, 0;\n");
  destName.assign("$s1");   op1Name.assign("$c2");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigf32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigNe;
  out.sourceType = Brigb1;
  out.reserved = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;
 
  // TODO(Chuang): the imm2.type should be Brigb1.
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 0;
  
  Cmp_Test<BrigOperandReg, BrigOperandImmed> TestCase1(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase1.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 2 ************************************/
  in.assign("cmp_gt_f32_b32 $s1, $s2, 0.0f;\n");
  destName.assign("$s1");   op1Name.assign("$s2");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigf32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigGt;
  out.sourceType = Brigb32;
  out.reserved = 0;

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
  imm2.bits.f = 0.0f;
  
  Cmp_Test<BrigOperandReg, BrigOperandImmed> TestCase2(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase2.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("cmp_equ_b1_f32 $c1, $s2, 0;\n");
  destName.assign("$c1");   op1Name.assign("$s2");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigb1;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigEqu;
  out.sourceType = Brigf32;
  out.reserved = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb1;
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
  imm2.bits.u = 0;
  
  Cmp_Test<BrigOperandReg, BrigOperandImmed> TestCase3(in, symbols, &out, &dest, &reg1, &imm2);
  TestCase3.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("cmp_sltu_b1_f64 $c1, $d1, $d2;\n");
  destName.assign("$c1");   op1Name.assign("$d1");
  op2Name.assign("$d2");
  symbols->append(destName);  symbols->append(op1Name);
  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigb1;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigSltu;
  out.sourceType = Brigf64;
  out.reserved = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb1;
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
  reg2.name = destName.size() + op1Name.size() + 2;
  
  Cmp_Test<BrigOperandReg, BrigOperandReg> TestCase4(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase4.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 5 ************************************/

  in.assign("packedcmp_lt_f32x2 $d1, $d2, $d3;\n");
  destName.assign("$d1");   op1Name.assign("$d2");
  op2Name.assign("$d3");
  symbols->append(destName);  symbols->append(op1Name);
  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigPackedCmp;
  out.type = Brigf32x2;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigLt;
  out.sourceType = Brigf32x2;
  out.reserved = 0;

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
  reg2.name = destName.size() + op1Name.size() + 2;
  
  Cmp_Test<BrigOperandReg, BrigOperandReg> TestCase5(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase5.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 6 ************************************/
  in.assign("cmp_eq_b1_b32 $c1, WAVESIZE, WAVESIZE;\n");
  destName.assign("$c1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigb1;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = out.o_operands[1];
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigEq;
  out.sourceType = Brigb32;
  out.reserved = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb1;
  dest.reserved = 0;
  dest.name = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;
  
  wav2 = wav1;

  Cmp_Test<BrigOperandWaveSz, BrigOperandWaveSz> TestCase6(in, symbols, &out, &dest, &wav1, &wav2);
  TestCase6.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 7 ************************************/
  in.assign("cmp_eq_b1_b32 $c1, 1024, WAVESIZE;\n");
  destName.assign("$c1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigCmp;
  out.type = Brigb1;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigEq;
  out.sourceType = Brigb32;
  out.reserved = 0;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb1;
  dest.reserved = 0;
  dest.name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 1024;

  wav2.size = sizeof(wav2);
  wav2.kind = BrigEOperandWaveSz;

  Cmp_Test<BrigOperandImmed, BrigOperandWaveSz> TestCase7(in, symbols, &out, &dest, &imm1, &wav2);
  TestCase7.Run_Test(&Cmp);
  symbols->clear();

  /************************************* Test Case 8 ************************************/
  in.assign("packedcmp_eq_u8x4 $s1, $s2, $s3;\n");
  destName.assign("$s1");   op1Name.assign("$s2");
  op2Name.assign("$s3");
  symbols->append(destName);  symbols->append(op1Name);
  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstCmp;
  out.opcode = BrigPackedCmp;
  out.type = Brigu8x4;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(reg1) + sizeof(dest);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  memset(&out.aluModifier, 0, sizeof(out.aluModifier));
  out.comparisonOperator = BrigEq;
  out.sourceType = Brigu8x4;
  out.reserved = 0;

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
  reg2.name = destName.size() + op1Name.size() + 2;
  
  Cmp_Test<BrigOperandReg, BrigOperandReg> TestCase8(in, symbols, &out, &dest, &reg1, &reg2);
  TestCase8.Run_Test(&Cmp);
  symbols->clear();



  delete symbols;
}
} // namespace hsa
} // namespace brig




