#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

template <typename TInst, typename T> class Instruction1_Test : public BrigCodeGenTest{
private:

  //Instruction in .code buffer - Pointers to brig structures
  const TInst* RefInst;
  //Operands in .operands buffer
  const T* RefDest;

public:

  Instruction1_Test(std::string& in, StringBuffer *sbuf, TInst* ref, T* Dest) :
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest)  { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();

    code->append(RefInst);
    if (RefDest != NULL) {
      oper->append(RefDest);
    } 

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]),
      reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), 0, code->size(), oper->size(), 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
    delete dir;
    RefInst = NULL;
    RefDest = NULL;
  }
};

/****************** Instruction1OpcodeNoDT Test ***********************/
TEST(CodegenTest, Instruction1OpcodeNoDT_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "laneid $s1;\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigLaneId,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "dynwaveid $s3;\n");
  op1.assign("$s3"); sbuf->append(op1);

  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest2.size = sizeof(dest2);

  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigDynWaveId,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out2.size = sizeof(out2);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase2(in, sbuf, &out2, &dest2);
  TestCase2.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "maxdynwaveid $s4;\n");
  op1.assign("$s4"); sbuf->append(op1);

  BrigOperandReg dest3 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest3.size = sizeof(dest3);

  BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigMaxDynWaveId,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out3.size = sizeof(out3);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase3(in, sbuf, &out3, &dest3);
  TestCase3.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "dispatchid $s5;\n");
  op1.assign("$s5"); sbuf->append(op1);

  BrigOperandReg dest4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest4.size = sizeof(dest4);

  BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigDispatchId,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out4.size = sizeof(out4);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase4(in, sbuf, &out4, &dest4);
  TestCase4.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "cu $s7;\n");
  op1.assign("$s7"); sbuf->append(op1);

  BrigOperandReg dest5 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest5.size = sizeof(dest5);

  BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigCU,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out5.size = sizeof(out5);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase5(in, sbuf, &out5, &dest5);
  TestCase5.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "workdim $s2;\n");
  op1.assign("$s2"); sbuf->append(op1);

  BrigOperandReg dest6 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest6.size = sizeof(dest6);

  BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigWorkDim,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out6.size = sizeof(out6);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase6(in, sbuf, &out6, &dest6);
  TestCase6.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "workitemid_flat  $s1;\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest7 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest7.size = sizeof(dest7);

  BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigWorkItemIdFlat,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out7.size = sizeof(out7);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase7(in, sbuf, &out7, &dest7);
  TestCase7.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "workitemaid_flat  $s6;\n");
  op1.assign("$s6"); sbuf->append(op1);

  BrigOperandReg dest8 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest8.size = sizeof(dest8);

  BrigInstBase out8 = {
    0,
    BrigEInstBase,
    BrigWorkItemAIdFlat,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out8.size = sizeof(out8);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase8(in, sbuf, &out8, &dest8);
  TestCase8.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "debugtrap  10;\n");

  BrigOperandImmed src9 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {10}
  };
  src9.size = sizeof(src9);

  BrigInstBase out9 = {
    0,
    BrigEInstBase,
    BrigDebugTrap,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out9.size = sizeof(out9);

  Instruction1_Test <BrigInstBase, BrigOperandImmed> TestCase9(in, sbuf, &out9, &src9);
  TestCase9.Run_Test(&Instruction1);
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

#if 0
/****************** fbar_initSizeKnown Test ************************/
TEST(CodegenTest, fbar_initSizeKnown_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "fbar_initSizeKnown_u32 $s3;\n");
  op1.assign("$s3"); sbuf->append(op1);
  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigFbarInitSizeKnown,
    Brigu32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "fbar_initSizeKnown_u32 12;\n");

  BrigOperandImmed dest2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {12}
  };
  dest2.size = sizeof(dest2);

  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigFbarInitSizeKnown,
    Brigu32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out2.size = sizeof(out2);

  Instruction1_Test <BrigInstBase, BrigOperandImmed> TestCase2(in, sbuf, &out2, &dest2);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}
#endif
/****************** fbar_release Test ************************/
TEST(CodegenTest, fbar_release_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "fbar_release_b64 $d1;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigFbarRelease,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

/****************** clock Test ************************/
TEST(CodegenTest, clock_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "clock $d6;\n");
  op1.assign("$d6"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigClock,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

/****************** Instruction1Opcode Test ************************/
TEST(CodegenTest, Instruction1Opcode_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
#if 0
  in.assign( "fbar_initSizeDynamic_b64 $d1;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigFbarInitSizeDynamic,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1);
  TestCase1.Run_Test(&Instruction1);
  sbuf->clear();
#endif
/**********************************************************************************/
  in.assign( "fbar_wait_b64 $d1;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest2.size = sizeof(dest2);

  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigFbarWait,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out2.size = sizeof(out2);


  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase2(in, sbuf, &out2, &dest2);
  TestCase2.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "fbar_arrive_b64 $d1;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest3 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest3.size = sizeof(dest3);

  BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigFbarArrive,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out3.size = sizeof(out3);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase3(in, sbuf, &out3, &dest3);
  TestCase3.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "fbar_skip_b64 $d1;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest4 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest4.size = sizeof(dest4);

  BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigFbarSkip,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out4.size = sizeof(out4);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase4(in, sbuf, &out4, &dest4);
  TestCase4.Run_Test(&Instruction1);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "countup_u32 $s1;\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest5 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest5.size = sizeof(dest5);

  BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigCountUp,
    Brigu32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  out5.size = sizeof(out5);

  Instruction1_Test <BrigInstBase, BrigOperandReg> TestCase5(in, sbuf, &out5, &dest5);
  TestCase5.Run_Test(&Instruction1);
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

/****************** Sync Test ************************/
TEST(CodegenTest, Sync_Codegen){

/*********************Common variables**********************/
  std::string in;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "sync;\n");

  BrigInstBar out1 = {
    0,
    BrigEInstBar,
    BrigSync,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGlobalLevel | BrigGroupLevel
  };
  out1.size = sizeof(out1);

  Instruction1_Test <BrigInstBar, BrigOperandReg> TestCase1(in, sbuf, &out1, NULL);
  TestCase1.Run_Test(&Sync);

/**********************************************************************************/
  in.assign( "sync_group;\n");

  BrigInstBar out2 = {
    0,
    BrigEInstBar,
    BrigSync,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGroupLevel
  };
  out2.size = sizeof(out2);

  Instruction1_Test <BrigInstBar, BrigOperandReg> TestCase2(in, sbuf, &out2, NULL);
  TestCase2.Run_Test(&Sync);

/**********************************************************************************/
  in.assign( "sync_global;\n");

  BrigInstBar out3 = {
    0,
    BrigEInstBar,
    BrigSync,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGlobalLevel
  };
  out3.size = sizeof(out3);

  Instruction1_Test <BrigInstBar, BrigOperandReg> TestCase3(in, sbuf, &out3, NULL);
  TestCase3.Run_Test(&Sync);

/******************************  End of tests *****************************************/
  delete sbuf;
}

/****************** Barrier Test ************************/
TEST(CodegenTest, Barrier_Codegen){

/*********************Common variables**********************/
  std::string in;
  StringBuffer* sbuf = new StringBuffer();
  /*****************************************************************/
  in.assign( "barrier;\n");

  BrigInstBar out1 = {
    0,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGlobalLevel | BrigGroupLevel
  };
  out1.size = sizeof(out1);

  BrigOperandImmed width1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width1.size = sizeof(width1);


  Instruction1_Test <BrigInstBar, BrigOperandImmed> TestCase1(in, sbuf, &out1, &width1);
  TestCase1.Run_Test(&Bar);

/**********************************************************************************/
  in.assign( "barrier_global;\n");

  BrigOperandImmed width2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width2.size = sizeof(width2);

  BrigInstBar out2 = {
    0,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGlobalLevel
  };
  out2.size = sizeof(out2);

  Instruction1_Test <BrigInstBar, BrigOperandImmed> TestCase2(in, sbuf, &out2, &width2);
  TestCase2.Run_Test(&Bar);

/**********************************************************************************/
  in.assign( "barrier_width(all)_group;\n");

  BrigInstBar out3 = {
    0,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGroupLevel
  };
  out3.size = sizeof(out3);

  BrigOperandImmed width3 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width3.size = sizeof(width3);

  Instruction1_Test <BrigInstBar, BrigOperandImmed> TestCase3(in, sbuf, &out3, &width3);
  TestCase3.Run_Test(&Bar);

/**********************************************************************************/
  in.assign( "barrier_width(64)_group;\n");

  BrigOperandImmed width4 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}
  };
  width4.size = sizeof(width4);

  BrigInstBar out4 = {
    0,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGroupLevel
  };
  out4.size = sizeof(out4);

  Instruction1_Test <BrigInstBar, BrigOperandImmed> TestCase4(in, sbuf, &out4, &width4);
  TestCase4.Run_Test(&Bar);

  in.assign( "barrier_partial;\n");
  BrigOperandImmed width5 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width5.size = sizeof(width5);
  BrigInstBar out5 = {
    0,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigPartialLevel
  };
  out5.size = sizeof(out5);

  Instruction1_Test <BrigInstBar, BrigOperandImmed> TestCase5(in, sbuf, &out5, &width5);
  TestCase5.Run_Test(&Bar);
  
/******************************  End of tests *****************************************/
  delete sbuf;
}

}
}
