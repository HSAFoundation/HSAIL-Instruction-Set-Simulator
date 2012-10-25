#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2> class Mov_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  const BrigOperandReg *RefReg1, *RefReg2, *RefReg3, *RefReg4;
     
public:
    Mov_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1,
     BrigOperandReg *Reg1 = NULL,
     BrigOperandReg *Reg2 = NULL,
     BrigOperandReg *Reg3 = NULL,
     BrigOperandReg *Reg4 = NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefReg1(Reg1), 
    RefReg2(Reg2), 
    RefReg3(Reg3), 
    RefReg4(Reg4) { }
 
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

    if (getdest->kind == BrigEOperandRegV4) {
        const BrigOperandRegV4 *getdest_opv = reinterpret_cast <const BrigOperandRegV4*> (getdest);

        const BrigOperandReg *getreg1 = 
             reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv ->regs[0]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg1, refbuf, getreg1, getbuf);

        const BrigOperandReg *getreg2 = 
              reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv ->regs[1]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg2, refbuf, getreg2, getbuf);

        const BrigOperandReg *getreg3 = 
               reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv->regs[2]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg3, refbuf, getreg3, getbuf);

        const BrigOperandReg *getreg4 = 
               reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv->regs[3]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg4, refbuf, getreg4, getbuf);           
      } else if (getdest->kind == BrigEOperandRegV2 ){
        const BrigOperandRegV2 *getdest_opv = reinterpret_cast <const BrigOperandRegV2*> (getdest);

        const BrigOperandReg *getreg1 = 
             reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv ->regs[0]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg1, refbuf, getreg1, getbuf);

        const BrigOperandReg *getreg2 = 
              reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_opv ->regs[1]]));
        validate_brig::validateOpType<BrigOperandReg>(RefReg2, refbuf, getreg2, getbuf);
      } 

    if (getsrc1->kind == BrigEOperandRegV4){
      const BrigOperandRegV4 *getsrc1_opv = reinterpret_cast <const BrigOperandRegV4*> (getsrc1);
      const BrigOperandReg *getreg1 = 
                reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[0]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg1, refbuf, getreg1, getbuf);

      const BrigOperandReg *getreg2 = 
            reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[1]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg2, refbuf, getreg2, getbuf);

      const BrigOperandReg *getreg3 = 
             reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[2]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg3, refbuf, getreg3, getbuf);

      const BrigOperandReg *getreg4 = 
                 reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[3]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg4, refbuf, getreg4, getbuf);           
    } else if (getsrc1->kind == BrigEOperandRegV2){
      const BrigOperandRegV2 *getsrc1_opv = reinterpret_cast <const BrigOperandRegV2*> (getsrc1);

      const BrigOperandReg *getreg1 = 
                reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[0]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg1, refbuf, getreg1, getbuf);

      const BrigOperandReg *getreg2 = 
            reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc1_opv->regs[1]]));
      validate_brig::validateOpType<BrigOperandReg>(RefReg2, refbuf, getreg2, getbuf);
    }

    EXPECT_EQ(0, getinst->o_operands[2]);
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
 }; 
    
  
TEST(CodegenTest,Mov_CodeGen){  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; std::string op1, op2, op3, op4, op5;
  StringBuffer* symbols;
  
  BrigOperandReg reg1, reg2, reg3, reg4, reg5;
  BrigOperandRegV2 regV2;
  BrigOperandRegV4 regV4;
  BrigOperandImmed imm2;
  
   
  /************************************* Test Case 1************************************/
  symbols = new StringBuffer();
  in.assign( "mov_b32 $s1, $s0; \n");
  op1.assign("$s1"); op2.assign("$s0"); 
  symbols->append(op1); symbols->append(op2);
  
  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb32,
    BrigNoPacking,
    {0,sizeof(reg1),0,0,0}
  };
  out1.size = sizeof(out1);

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

  Mov_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> 
            TestCase1(in, symbols, &out1, &reg1, &reg2);
  TestCase1.Run_Test(&Mov);
  symbols->clear();


  /************************************* Test Case 2************************************/
  in.assign( "mov_b64 $d1, ($s0, $s3); \n");
  op1.assign("$d1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  
  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb64,
    BrigNoPacking,
    {0,
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3),
     0,0,0}
  };
  out2.size = sizeof(out2);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
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

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(reg1);
  regV2.regs[1] = sizeof(reg1) + sizeof(reg2);


  Mov_Test<BrigInstBase, BrigOperandReg, BrigOperandRegV2> 
            TestCase2(in, symbols, &out2, &reg1, &regV2, &reg2, &reg3);
  TestCase2.Run_Test(&Mov);
  symbols->clear();

  /************************************* Test Case 3************************************/
  in.assign( "mov_b64 ($s0, $s3), $d1; \n");
  op1.assign("$s0"); op2.assign("$s3"); op3.assign("$d1");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  
  BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb64,
    BrigNoPacking,
    {sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(regV2),
     0,0,0}
  };
  out3.size = sizeof(out3);

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

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = 0;
  regV2.regs[1] = sizeof(reg1);

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb64;
  reg3.reserved = 0;
  reg3.name = op1.size() + op2.size() + 2;

  Mov_Test<BrigInstBase, BrigOperandRegV2, BrigOperandReg> 
            TestCase3(in, symbols, &out3, &regV2, &reg3, &reg1, &reg2);
  TestCase3.Run_Test(&Mov);
  symbols->clear();

  /************************************* Test Case 4************************************/
  in.assign( "mov_b128 $q1, ($s0, $s3, $s1, $s2); \n");
  op1.assign("$q1"); op2.assign("$s0"); op3.assign("$s3");
  op4.assign("$s1"); op5.assign("$s2");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  symbols->append(op4); symbols->append(op5);
  
  BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb128,
    BrigNoPacking,
    {0,
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4) +sizeof(reg5),
    0,0,0}
  };
  out4.size = sizeof(out4);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb128;
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

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  reg5.size = sizeof(reg5);
  reg5.kind = BrigEOperandReg;
  reg5.type = Brigb32;
  reg5.reserved = 0;
  reg5.name = op1.size() + op2.size() + op3.size() + op4.size() + 4;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(reg1);
  regV4.regs[1] = sizeof(reg1) + sizeof(reg2);
  regV4.regs[2] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  regV4.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);

  Mov_Test<BrigInstBase, BrigOperandReg, BrigOperandRegV4> 
            TestCase4(in, symbols, &out4, &reg1, &regV4, &reg2, &reg3, &reg4, &reg5);
  TestCase4.Run_Test(&Mov);
  symbols->clear();

  /************************************* Test Case 5************************************/
  in.assign( "mov_b128 ($s0, $s3, $s1, $s2), $q1; \n");
  op1.assign("$s0"); op2.assign("$s3");
  op3.assign("$s1"); op4.assign("$s2"); op5.assign("$q1");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  symbols->append(op4); symbols->append(op5);
  
  BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb128,
    BrigNoPacking,
    {sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4) + sizeof(regV4),
     0,0,0}
  };
  out5.size = sizeof(out5);

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

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = op1.size() + op2.size() + op3.size() + 3;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = 0;
  regV4.regs[1] = sizeof(reg1);
  regV4.regs[2] = sizeof(reg1) + sizeof(reg2);
  regV4.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);

  reg5.size = sizeof(reg5);
  reg5.kind = BrigEOperandReg;
  reg5.type = Brigb128;
  reg5.reserved = 0;
  reg5.name = op1.size() + op2.size() + op3.size() + op4.size() + 4;



  Mov_Test<BrigInstBase, BrigOperandRegV4, BrigOperandReg> 
            TestCase5(in, symbols, &out5, &regV4, &reg5, &reg1, &reg2, &reg3, &reg4);
  TestCase5.Run_Test(&Mov);
  symbols->clear();

  /************************************* Test Case 6************************************/
  in.assign( "mov_b32 $s1, 0x1234; \n");
  op1.assign("$s1");  
  symbols->append(op1);;
  
  BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb32,
    BrigNoPacking,
    {0,sizeof(reg1),0,0,0}
  };
  out6.size = sizeof(out6);

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
  imm2.bits.u = 0x1234;

  Mov_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed> 
            TestCase6(in, symbols, &out6, &reg1, &imm2);
  TestCase6.Run_Test(&Mov);
  symbols->clear();

  /************************************* Test Case 7************************************/
  in.assign( "mov_b64 $d1, $d0; \n");
  op1.assign("$d1"); op2.assign("$d0"); 
  symbols->append(op1); symbols->append(op2);
  
  BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigMov,
    Brigb64,
    BrigNoPacking,
    {0,sizeof(reg1),0,0,0}
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

  Mov_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> 
            TestCase7(in, symbols, &out7, &reg1, &reg2);
  TestCase7.Run_Test(&Mov);
  symbols->clear();


/***********************************************************/

  delete symbols;
}

}
}

