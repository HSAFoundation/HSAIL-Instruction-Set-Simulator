#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename Tinst, typename T> class Branch_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const Tinst* RefInst;
  
  //Operands
  const T* RefSrc1;
  const BrigOperandImmed* RefWidth;
  const BrigOperandReg* RefReg;
public:
 
  Branch_Test(std::string& in, StringBuffer* sbuf, Tinst* ref, BrigOperandImmed* width, T* src1, BrigOperandReg* reg = NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref), 
    RefSrc1(src1),
    RefWidth(width),
    RefReg(reg) { }
    
  void validate(struct BrigSections* TestOutput){   

    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;
    uint32_t opCount = 0;
    inst_iterator getcode = TestOutput->code_begin();   
    const Tinst* getinst = (cast<Tinst>(getcode));

    validate_brig::validate(RefInst, getinst);

    const BrigOperandImmed *getwidth = reinterpret_cast <const BrigOperandImmed *> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validate(RefWidth, getwidth);

    if (RefReg != NULL) {
      const BrigOperandReg *getreg = reinterpret_cast <const BrigOperandReg *> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validate(RefReg, refbuf, getreg, getbuf);
    }

    const T* getsrc1 = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validateOpType<T>(RefSrc1, refbuf, getsrc1, getbuf);
    
    while (opCount <= 4) {
      EXPECT_EQ(0, getinst->o_operands[opCount++]);       
    }       
    
  }
};

/*********************** brn Test ***************************/
TEST(CodegenTest, Brn_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "brn @loop;\n");

  BrigOperandImmed width1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width1.size = sizeof(width1);

  BrigOperandLabelRef label1 = {
    0,
    BrigEOperandLabelRef,
    0
  };
  label1.size = sizeof(label1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase, 
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width1), 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Branch_Test<BrigInstBase, BrigOperandLabelRef> TestCase1(in, sbuf, &out1, &width1, &label1);
  TestCase1.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn_width(64)_fbar @label;\n");

  BrigOperandImmed width2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}  
  }; 
  width2.size = sizeof(width2);

  BrigOperandLabelRef label2 = {
    0,
    BrigEOperandLabelRef,
    0
  };
  label2.size = sizeof(label2);
  
  BrigAluModifier bam2;
  bam2.ftz = 1;

  BrigInstMod out2 = {
    0,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width2), 0, 0, 0},
    bam2
};

  Branch_Test<BrigInstMod, BrigOperandLabelRef> TestCase2(in, sbuf, &out2, &width2, &label2);
  TestCase2.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn_width(64)_fbar $s1;\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandImmed width3 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}  
  }; 
  width3.size = sizeof(width3);

  BrigOperandReg reg3 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  reg3.size = sizeof(reg3);  
  
  BrigAluModifier bam3;
  bam3.ftz = 1;

  BrigInstMod out3 = {
    0,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width2), 0, 0, 0},
    bam3
};

  Branch_Test<BrigInstMod, BrigOperandReg> TestCase3(in, sbuf, &out3, &width3, &reg3);
  TestCase3.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn_width(64) $s0;\n");
  op1.assign("$s0"); sbuf->append(op1);

  BrigOperandImmed width4 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}  
  }; 
  width4.size = sizeof(width4);

  BrigOperandReg reg4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  reg4.size = sizeof(reg4);  
  
  BrigInstBase out4 = {
    0,
    BrigEInstBase, 
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width4), 0, 0, 0}
  };
  out4.size = sizeof(out4);

  Branch_Test<BrigInstBase, BrigOperandReg> TestCase4(in, sbuf, &out4, &width4, &reg4);
  TestCase4.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn $s1, [%jumptable2];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandImmed width5 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width5.size = sizeof(width5);

  BrigOperandReg reg5 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  reg5.size = sizeof(reg5);

  BrigOperandAddress addr5 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr5.size = sizeof(addr5);

  BrigInstBase out5 = {
    0,
    BrigEInstBase, 
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width5), sizeof(width5) + sizeof(reg5), 0, 0}
  };
  out5.size = sizeof(out5);

  Branch_Test<BrigInstBase, BrigOperandAddress> TestCase5(in, sbuf, &out5, &width5, &addr5, &reg5);
  TestCase5.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn_width(64)_fbar $s3, [@targets];\n");
  op1.assign("$s3"); sbuf->append(op1);

  BrigOperandImmed width6 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width6.size = sizeof(width6);

  BrigOperandReg reg6 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  reg6.size = sizeof(reg6);

  BrigOperandAddress addr6 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr6.size = sizeof(addr6);

  BrigAluModifier bam6;
  bam6.ftz = 1;

  BrigInstMod out6 = {
    0,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width6), sizeof(width6) + sizeof(reg6), 0, 0},
    bam6
};

  Branch_Test<BrigInstMod, BrigOperandAddress> TestCase6(in, sbuf, &out6, &width6, &addr6, &reg6);
  TestCase6.Run_Test(&Branch);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "brn_width(all)_fbar $s4, [%x];\n");
  op1.assign("$s4"); sbuf->append(op1);

  BrigOperandReg reg7 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  reg7.size = sizeof(reg7);

  BrigOperandAddress addr7 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr7.size = sizeof(addr7);

  BrigAluModifier bam7;
  bam7.ftz = 1;

  BrigInstMod out7 = {
    0,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {0, 0, sizeof(reg7), 0, 0},
    bam7
};

  Branch_Test<BrigInstMod, BrigOperandAddress> TestCase7(in, sbuf, &out7, NULL, &addr7, &reg7);
  TestCase7.Run_Test(&Branch);  
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

}
}
