#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

template <typename Tinst=BrigInstBase, typename T=BrigOperandReg>
class Ld_Test: public BrigCodeGenTest {

private:
  const Tinst* RefInst;

  const BrigOperandImmed* OpWidth;

  const T* RefDest;
  const BrigOperandReg* RefSrc_Reg;
  const BrigOperandAddress* RefSrc_Addr;
  const BrigOperandIndirect* RefSrc_Indir;
  const BrigOperandCompound* RefSrc_Comp;
  const BrigOperandLabelRef* RefSrc_Label;
  const BrigOperandFunctionRef* RefSrc_Function;
  const std::string* SymbolName;

  BrigOperandReg RefSrc_RegList[4];
  
public:
  Ld_Test(std::string& in, std::string *symName = NULL):
    BrigCodeGenTest(in),
    SymbolName(symName) {}

  Ld_Test(std::string& input, StringBuffer* sbuf, Tinst* ref,
      BrigOperandImmed* width, T* dest, BrigOperandAddress* addr, 
      BrigOperandReg *regList = NULL, std::string* symName = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),       OpWidth(width),
    RefDest(dest),      RefSrc_Reg(NULL),
    RefSrc_Addr(addr),  RefSrc_Indir(NULL),
    RefSrc_Comp(NULL),  RefSrc_Label(NULL),
    RefSrc_Function(NULL), SymbolName(symName) { 
      if (regList != NULL) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          RefSrc_RegList[i] = regList[i];
        } 
      } else {
        memset(RefSrc_RegList, 0 , sizeof(RefSrc_RegList[0]) * 4);
      }
    }

  Ld_Test(std::string& input, StringBuffer* sbuf, Tinst* ref,
      BrigOperandImmed* width, T* dest, BrigOperandIndirect* indir, 
      BrigOperandReg* reg = NULL, BrigOperandReg *regList = NULL, 
      std::string *symName = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),            OpWidth(width),
    RefDest(dest),           RefSrc_Reg(reg),
    RefSrc_Addr(NULL),       RefSrc_Indir(indir),
    RefSrc_Comp(NULL),       RefSrc_Label(NULL),
    RefSrc_Function(NULL),   SymbolName(symName)  { 
      if (regList != NULL) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          RefSrc_RegList[i] = regList[i];
        } 
      } else {
        memset(RefSrc_RegList, 0 , sizeof(RefSrc_RegList[0]) * 4);
      }
    }

  Ld_Test(std::string& input, StringBuffer* sbuf, Tinst* ref,
      BrigOperandImmed* width, T* dest, BrigOperandCompound* comp, 
      BrigOperandAddress* addr, BrigOperandReg* reg = NULL, 
      BrigOperandReg *regList = NULL, std::string *symName = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),           OpWidth(width),
    RefDest(dest),          RefSrc_Reg(reg),
    RefSrc_Addr(addr),      RefSrc_Indir(NULL),
    RefSrc_Comp(comp),      RefSrc_Label(NULL),
    RefSrc_Function(NULL),  SymbolName(symName)  { 
      if (regList != NULL) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          RefSrc_RegList[i] = regList[i];
        } 
      } else {
        memset(RefSrc_RegList, 0 , sizeof(RefSrc_RegList[0]) * 4);
      }
    }

  Ld_Test(std::string& input, StringBuffer* sbuf, Tinst* ref, T* dest, 
      BrigOperandLabelRef* src, BrigOperandReg *regList = NULL, 
      std::string *symName = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),                    OpWidth(NULL),
    RefDest(dest),                   RefSrc_Reg(NULL),
    RefSrc_Addr(NULL),               RefSrc_Indir(NULL),
    RefSrc_Comp(NULL),               RefSrc_Label(src),
    RefSrc_Function(NULL),           SymbolName(symName) { 
      if (regList != NULL) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          RefSrc_RegList[i] = regList[i];
        } 
      } else {
        memset(RefSrc_RegList, 0 , sizeof(RefSrc_RegList[0]) * 4);
      }
    }

  Ld_Test(std::string& input, StringBuffer* sbuf, Tinst* ref, T* dest, 
      BrigOperandFunctionRef* src, BrigOperandReg *regList = NULL,
      std::string *symName = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),                 OpWidth(NULL),
    RefDest(dest),                RefSrc_Reg(NULL),
    RefSrc_Addr(NULL),            RefSrc_Indir(NULL),
    RefSrc_Comp(NULL),            RefSrc_Label(NULL),
    RefSrc_Function(src),         SymbolName(symName)  { 
      if (regList != NULL) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          RefSrc_RegList[i] = regList[i];
        }  
      } else {
        memset(RefSrc_RegList, 0 , sizeof(RefSrc_RegList[0]) * 4);
      }
    }

  
  void Run_Test(int (*Rule)(Context*)){  

    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    if (OpWidth != NULL) {
      oper->append(OpWidth);
    }
    for (uint32_t i = 0 ; i < 4 ; ++i) {
      if (RefSrc_RegList[i].size != 0) { 
        oper->append(&RefSrc_RegList[i]);
      }
    }
    oper->append(RefDest);
    if (RefSrc_Addr != NULL) { 
      oper->append(RefSrc_Addr);
    }
    if (RefSrc_Reg != NULL) { 
      oper->append(RefSrc_Reg);
    }
    if (RefSrc_Indir != NULL) { 
      oper->append(RefSrc_Indir);
    }
    if (RefSrc_Comp != NULL) { 
      oper->append(RefSrc_Comp);
    }
    if (RefSrc_Label != NULL) { 
      oper->append(RefSrc_Label);
    }
    if (RefSrc_Function != NULL) { 
      oper->append(RefSrc_Function);
    }
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), 0, code->size(), oper->size(), (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput, SymbolName);
    delete code;
    delete oper;
  }  
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError, SymbolName);
  }
};
/*********************** Ld Test ***************************/
TEST(CodegenTest, Ld_Codegen){

  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  BrigOperandReg regList[4]; 
  std::string symbolName;

  /*****************************************************************/
  in.assign( "ld_arg_f32 $s0, [&input];\n");
  op1.assign("$s0"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigOperandImmed width1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width1.size = sizeof(width1);
  width1.bits.u = 1;

  BrigOperandAddress addr1 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr1.size = sizeof(addr1);

  BrigInstLdSt out1 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(width1), sizeof(width1) + sizeof(dest1), 0, 0},
    BrigArgSpace,
    BrigRegular,
    0
  };
  out1.size = sizeof(out1);

  symbolName.assign("&input");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase1(in, sbuf, &out1, &width1, &dest1, &addr1, NULL, &symbolName);
  TestCase1.Run_Test(&Ld);
  sbuf->clear();

  /******************************************************/
  in.assign( "ld_group_f16 $d5, [100];\n");
  op1.assign("$d5"); sbuf->append(op1);

  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest2.size = sizeof(dest2);

  BrigOperandImmed width2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width2.size = sizeof(width2);
  width2.bits.u = 1;

  BrigOperandIndirect indir2 = {
    0,
    BrigEOperandIndirect,
    0,
    Brigb32,
    0,
    100
  };
  indir2.size = sizeof(indir2);

  BrigInstLdSt out2 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf16,
    BrigNoPacking,
    {0, sizeof(width2), sizeof(width2) + sizeof(dest2), 0, 0},
    BrigGroupSpace,
    BrigRegular,
    0
  };
  out2.size = sizeof(out2);

  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase2(in, sbuf, &out2, &width2, &dest2, &indir2, NULL);
  TestCase2.Run_Test(&Ld);
  sbuf->clear();

  /**********************************************************/
  in.assign( "ld_f32 $s1, [$d0];\n");
  op1.assign("$s1"); op2.assign("$d0");
  sbuf->append(op1); sbuf->append(op2);

  BrigOperandReg dest3 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest3.size = sizeof(dest3);

  BrigOperandImmed width3 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width3.size = sizeof(width3);
  width3.bits.u = 1;

  BrigOperandIndirect indir3 = {
    0,
    BrigEOperandIndirect,
    sizeof(width3) + sizeof(dest3),
    Brigb64,
    0,
    0
  };
  indir3.size = sizeof(indir3);

  BrigOperandReg reg3 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  reg3.size = sizeof(reg3);

  BrigInstLdSt out3 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(width3), sizeof(width3) + sizeof(dest3) + sizeof(reg3), 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
  };
  out3.size = sizeof(out3);

  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase3(in, sbuf, &out3, &width3, &dest3, &indir3, &reg3);
  TestCase3.Run_Test(&Ld);
  sbuf->clear();

  /*********************************************************/
  in.assign( "ld_global_u32 $s1, [&x];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest4.size = sizeof(dest4);

  BrigOperandImmed width4 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width4.size = sizeof(width4);
  width4.bits.u = 1;

  BrigOperandAddress addr4 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr4.size = sizeof(addr4);

  BrigInstLdSt out4 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(width4), sizeof(width4) + sizeof(dest4), 0, 0},
    BrigGlobalSpace,
    0,
    0
  };
  out4.size = sizeof(out4);

  symbolName.assign("&x");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase4(in, sbuf, &out4, &width4, &dest4, &addr4, NULL, &symbolName);
  TestCase4.Run_Test(&Ld);
  sbuf->clear();

  /*********************************************************/
  in.assign( "ld_width(64)_global_f16 $d1, [&x];\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest5 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest5.size = sizeof(dest5);

  BrigOperandImmed width5 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}
  };
  width5.size = sizeof(width5);

  BrigOperandAddress addr5 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr5.size = sizeof(addr5);

  BrigInstLdSt out5 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf16,
    BrigNoPacking,
    {0, sizeof(width5), sizeof(width5) + sizeof(dest5), 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
  };
  out5.size = sizeof(out5);

  symbolName.assign("&x");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase5(in, sbuf, &out5, &width5, &dest5, &addr5, NULL, &symbolName);
  TestCase5.Run_Test(&Ld);
  sbuf->clear();

  /*************************************************************/
  in.assign( "ld_global_f32 $s1, [&g][0];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest6 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest6.size = sizeof(dest6);

  BrigOperandImmed width6 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width6.size = sizeof(width6);
  width6.bits.u = 1;

  BrigOperandAddress addr6 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr6.size = sizeof(addr6);

  BrigOperandCompound comp6 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width6) + sizeof(dest6),
    0,
    0
  };
  comp6.size = sizeof(comp6);

  BrigInstLdSt out6 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(width6), sizeof(width6) + sizeof(dest6) + sizeof(addr6), 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
  };
  out6.size = sizeof(out6);
  
  symbolName.assign("&g");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase6(in, sbuf, &out6, &width6, &dest6, &comp6, &addr6, NULL, NULL, &symbolName);
  TestCase6.Run_Test(&Ld);
  sbuf->clear();

  /***********************************************************/
  in.assign( "ld_global_u32 $s3, [&g][$d2];\n");
  op1.assign("$s3"); op2.assign("$d2");
  sbuf->append(op1); sbuf->append(op2);

  BrigOperandReg dest7 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest7.size = sizeof(dest7);

  BrigOperandImmed width7 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width7.size = sizeof(width7);
  width7.bits.u = 1;

  BrigOperandAddress addr7 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr7.size = sizeof(addr7);

  BrigOperandReg reg7 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  reg7.size = sizeof(reg7);


  BrigOperandCompound comp7 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width7) + sizeof(dest7),
    sizeof(width7) + sizeof(dest7) + sizeof(addr7),
    0
  };
  comp7.size = sizeof(comp7);

  BrigInstLdSt out7 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(width7), sizeof(width7) + sizeof(dest7) + sizeof(addr7) + sizeof(reg7), 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
  };
  out7.size = sizeof(out7);

  symbolName.assign("&g");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase7(in, sbuf, &out7, &width7, &dest7, &comp7, &addr7, &reg7, NULL, &symbolName);
  TestCase7.Run_Test(&Ld);
  sbuf->clear();

  /****************************************************/
  in.assign( "ld_global_u32 $s4, [&g][$d2+4];\n");
  op1.assign("$s4"); op2.assign("$d2");
  sbuf->append(op1); sbuf->append(op2);

  BrigOperandReg dest8 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest8.size = sizeof(dest8);

  BrigOperandImmed width8 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width8.size = sizeof(width8);
  width8.bits.u = 1;

  BrigOperandAddress addr8 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr8.size = sizeof(addr8);

  BrigOperandReg reg8 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  reg8.size = sizeof(reg8);

  BrigOperandCompound comp8 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width8) + sizeof(dest8),
    sizeof(width8) + sizeof(dest8) + sizeof(addr8),
    4
  };
  comp8.size = sizeof(comp8);

  BrigInstLdSt out8 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(width8), sizeof(width8) + sizeof(dest8) + 
      sizeof(addr8) + sizeof(reg8), 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
  };
  out8.size = sizeof(out8);

  symbolName.assign("&g");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase8(in, sbuf, &out8, &width8, &dest8, &comp8, &addr8, &reg8, NULL, &symbolName);
  TestCase8.Run_Test(&Ld);
  sbuf->clear();

  /*****************************************************/
  in.assign( "ld_readonly_s32 $s1, [&tbl][12];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest9 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest9.size = sizeof(dest9);

  BrigOperandImmed width9 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width9.size = sizeof(width9);
  width9.bits.u = 1;

  BrigOperandAddress addr9 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr9.size = sizeof(addr9);

  BrigOperandCompound comp9 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width9) + sizeof(dest9),
    0,
    12
  };
  comp9.size = sizeof(comp9);

  BrigInstLdSt out9 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigs32,
    BrigNoPacking,
    {0, sizeof(width9), sizeof(width9) + sizeof(dest9) + sizeof(addr9), 0, 0},
    BrigReadonlySpace,
    BrigRegular,
    0
  };
  out9.size = sizeof(out9);

  symbolName.assign("&tbl");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase9(in, sbuf, &out9, &width9, &dest9, &comp9, &addr9, NULL, NULL, &symbolName);
  TestCase9.Run_Test(&Ld);
  sbuf->clear();

  /********************************************************/
  in.assign( "ld_v2_f32 ($s9,$s2), [$d1+3];\n");
  op1.assign("$s9"); sbuf->append(op1);
  op1.assign("$s2"); sbuf->append(op1);
  op2.assign("$d1"); sbuf->append(op2);

  BrigOperandImmed width10 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width10.size = sizeof(width10);
  width10.bits.u = 1;

  BrigOperandReg reg_v1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  reg_v1.size = sizeof(reg_v1);

  BrigOperandReg reg_v2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    op1.size() + 1
  };
  reg_v2.size = sizeof(reg_v2);

  regList[0] = reg_v1;
  regList[1] = reg_v2;
  memset(&regList[2], 0, sizeof(regList[2]));
  memset(&regList[3], 0, sizeof(regList[3])); 

  BrigOperandRegV2 dest10 = {
    0,
    BrigEOperandRegV2,
    Brigb32,
    0,
    {sizeof(width10), sizeof(width10) + sizeof(reg_v1)}
  };
  dest10.size = sizeof(dest10);

  BrigOperandReg reg10 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    2 * (op1.size() + 1)
  };
  reg10.size = sizeof(reg10);

  BrigOperandIndirect indir10 = {
    0,
    BrigEOperandIndirect,
    sizeof(width10) + sizeof(reg_v1) + sizeof (reg_v2) + sizeof(dest10),
    Brigb64,
    0,
    3
  };
  indir10.size = sizeof(indir10);

  BrigInstLdSt out10 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(width10) + sizeof(reg_v1) + sizeof(reg_v2),
     sizeof(width10) + sizeof(reg_v1) + sizeof (reg_v2) + 
     sizeof(dest10) + sizeof(reg10), 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
  };
  out10.size = sizeof(out10);

  Ld_Test<BrigInstLdSt, BrigOperandRegV2> 
    TestCase10(in, sbuf, &out10, &width10, &dest10, &indir10, &reg10, regList);
  TestCase10.Run_Test(&Ld);
  sbuf->clear();

  /*******************************************************/
  in.assign( "ld_v4_f64 ($d1,$d3,$d2,$d5), [&x];\n");
  op1.assign("$d1"); sbuf->append(op1);
  op1.assign("$d3"); sbuf->append(op1);
  op1.assign("$d2"); sbuf->append(op1);
  op1.assign("$d5"); sbuf->append(op1);

  BrigOperandImmed width11 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width11.size = sizeof(width11);
  width11.bits.u = 1;

  BrigOperandReg regv1_11 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  regv1_11.size = sizeof(regv1_11);

  BrigOperandReg regv2_11 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  regv2_11.size = sizeof(regv2_11);

  BrigOperandReg regv3_11 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    2 * (op1.size() + 1)
  };
  regv3_11.size = sizeof(regv3_11);

  BrigOperandReg regv4_11 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    3 * (op1.size() + 1)
  };
  regv4_11.size = sizeof(regv4_11);

  regList[0] = regv1_11;
  regList[1] = regv2_11;
  regList[2] = regv3_11;
  regList[3] = regv4_11; 

  BrigOperandRegV4 dest11 = {
    0,
    BrigEOperandRegV4,
    Brigb64,
    0,
    {sizeof(width11), sizeof(width11) + sizeof(regv1_11), 
     sizeof(width11) + sizeof(regv1_11) + sizeof(regv2_11),
     sizeof(width11) + sizeof(regv1_11) + sizeof(regv2_11) + sizeof(regv3_11)}
  };
  dest11.size = sizeof(dest11);

  BrigOperandAddress addr11 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr11.size = sizeof(addr11);

  BrigInstLdSt out11 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf64,
    BrigNoPacking,
    {0, sizeof(width11) + sizeof(regv1_11) + sizeof(regv2_11) + 
     sizeof(regv3_11) + sizeof(regv4_11), sizeof(width11) + sizeof(regv1_11) + 
     sizeof(regv2_11) + sizeof(regv3_11) + sizeof(regv4_11) + sizeof(dest11), 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
  };
  out11.size = sizeof(out11);

  symbolName.assign("&x");
  Ld_Test<BrigInstLdSt, BrigOperandRegV4> 
    TestCase11(in, sbuf, &out11, &width11, &dest11, &addr11, regList, &symbolName);
  TestCase11.Run_Test(&Ld);
  sbuf->clear();

  /********************************************************/
  in.assign( "ld_width(64)_readonly_v4_f32 ($s0,$s3,$s9,$s1), [&tbl][2];\n");
  op1.assign("$s0"); sbuf->append(op1);
  op1.assign("$s3"); sbuf->append(op1);
  op1.assign("$s9"); sbuf->append(op1);
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandImmed width12 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}
  };
  width12.size = sizeof(width12);

  BrigOperandReg regv1_12 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  regv1_12.size = sizeof(regv1_12);

  BrigOperandReg regv2_12 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    op1.size() + 1
  };
  regv2_12.size = sizeof(regv2_12);

  BrigOperandReg regv3_12 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    2 * (op1.size() + 1)
  };
  regv3_12.size = sizeof(regv3_12);

  BrigOperandReg regv4_12 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    3 * (op1.size() + 1)
  };
  regv4_12.size = sizeof(regv4_12);

  regList[0] = regv1_12;
  regList[1] = regv2_12;
  regList[2] = regv3_12;
  regList[3] = regv4_12; 

  BrigOperandRegV4 dest12 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {sizeof(width12), sizeof(width12) + sizeof(regv1_12),
     sizeof(width12) + sizeof(regv1_12) + sizeof(regv2_12),
     sizeof(width12) + sizeof(regv1_12) + sizeof(regv2_12) + sizeof(regv3_12)}
  };
  dest12.size = sizeof(dest12);

  BrigOperandAddress addr12 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr12.size = sizeof(addr12);

  BrigOperandCompound comp12 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width12) + sizeof(regv1_12) + sizeof(regv2_12) + 
    sizeof(regv3_12) + sizeof(regv4_12) + sizeof(dest12),
    0,
    2
  };
  comp12.size = sizeof(comp12);

  BrigInstLdSt out12 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(width12) + sizeof(regv1_12) + sizeof(regv2_12) + sizeof(regv3_12) + sizeof(regv4_12),
    sizeof(width12) + sizeof(regv1_12) + sizeof(regv2_12) + sizeof(regv3_12) + sizeof(regv4_12) + sizeof(dest12) + sizeof(addr12), 0, 0},
     BrigReadonlySpace,
    BrigRegular,
    0
  };
  out12.size = sizeof(out12);

  symbolName.assign("&tbl");
  Ld_Test<BrigInstLdSt, BrigOperandRegV4> 
    TestCase12(in, sbuf, &out12, &width12, &dest12, &comp12, &addr12, NULL, regList, &symbolName);
  TestCase12.Run_Test(&Ld);
  sbuf->clear();
  /*************************************************************/
  in.assign("ld_equiv(1)_s8 $s1, [&g];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest13 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest13.size = sizeof(dest13);

  BrigOperandImmed width13 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width13.size = sizeof(width13);
  width13.bits.u = 1;

  BrigOperandAddress addr13 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr13.size = sizeof(addr13);

  BrigInstLdSt out13 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigs8,
    BrigNoPacking,
    {0, sizeof(width13), sizeof(width13) + sizeof(dest13), 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
  };
  out13.size = sizeof(out13);

  symbolName.assign("&g");
  Ld_Test<BrigInstLdSt, BrigOperandReg> 
    TestCase13(in, sbuf, &out13, &width13, &dest13, &addr13, NULL, &symbolName);
  TestCase13.Run_Test(&Ld);
  sbuf->clear();

  delete sbuf;
}

/*********************** Lda Test ***************************/
TEST(CodegenTest, Lda_Codegen){

  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();
  std::string symbolName;

  /*****************************************************************/
  in.assign( "lda_u32 $s1, [&loc];\n");
  op1.assign("$s1");
  sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigOperandAddress addr1 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr1.size = sizeof(addr1);

  BrigInstMem out1 = {
    0,
    BrigEInstMem,
    BrigLda,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(dest1), 0, 0, 0},
    BrigFlatSpace
  };
  out1.size = sizeof(out1);

  symbolName.assign("&loc");
  Ld_Test<BrigInstMem, BrigOperandReg> 
    TestCase1(in, sbuf, &out1, NULL, &dest1, &addr1, NULL, &symbolName);
  TestCase1.Run_Test(&Lda);
  sbuf->clear();

  /**************************************************/
  in.assign( "lda_u64 $d4, [&x];\n");
  op1.assign("$d4"); sbuf->append(op1);

  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest2.size = sizeof(dest2);

  BrigOperandAddress addr2 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr2.size = sizeof(addr2);

  BrigInstMem out2 = {
    0,
    BrigEInstMem,
    BrigLda,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(dest2), 0, 0, 0},
    BrigFlatSpace
  };
  out2.size = sizeof(out2);

  symbolName.assign("&x");
  Ld_Test<BrigInstMem, BrigOperandReg> 
    TestCase2(in, sbuf, &out2, NULL, &dest2, &addr2, NULL, &symbolName);
  TestCase2.Run_Test(&Lda);
  sbuf->clear();

  /*********************************************/
  in.assign( "lda_u32 $s2, [$d1];\n");
  op1.assign("$s2"); op2.assign("$d1");
  sbuf->append(op1); sbuf->append(op2);

  BrigOperandReg dest3 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest3.size = sizeof(dest3);

  BrigOperandReg reg3 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  reg3.size = sizeof(reg3);

  BrigOperandIndirect indir3 = {
    0,
    BrigEOperandIndirect,
    sizeof(dest3),
    Brigb64,
    0,
    0
  };
  indir3.size = sizeof(indir3);

  BrigInstMem out3 = {
    0,
    BrigEInstMem,
    BrigLda,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(dest3) + sizeof(reg3), 0, 0, 0},
    BrigFlatSpace
  };
  out3.size = sizeof(out3);

  Ld_Test<BrigInstMem, BrigOperandReg> 
    TestCase3(in, sbuf, &out3, NULL, &dest3, &indir3, &reg3);
  TestCase3.Run_Test(&Lda);
  sbuf->clear();

  /******************************************/
  in.assign( "lda_u64 $d1, [$d0 + 10];\n");
  op1.assign("$d1"); op2.assign("$d0");
  sbuf->append(op1); sbuf->append(op2);

  BrigOperandReg dest4 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest4.size = sizeof(dest4);

  BrigOperandReg reg4 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    op1.size() + 1
  };
  reg4.size = sizeof(reg4);

  BrigOperandIndirect indir4 = {
    0,
    BrigEOperandIndirect,
    sizeof(dest4),
    Brigb64,
    0,
    10
  };
  indir4.size = sizeof(indir4);

  BrigInstMem out4 = {
    0,
    BrigEInstMem,
    BrigLda,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(dest4) + sizeof(reg4), 0, 0, 0},
    BrigFlatSpace
  };
  out4.size = sizeof(out4);

  Ld_Test<BrigInstMem, BrigOperandReg> 
    TestCase4(in, sbuf, &out4, NULL, &dest4, &indir4, &reg4);
  TestCase4.Run_Test(&Lda);
  sbuf->clear();

  /******************************************************/
  in.assign( "lda_u32 $s1, [&array][4];\n");
  op1.assign("$s1"); sbuf->append(op1);

  BrigOperandReg dest5 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0,
    0
  };
  dest5.size = sizeof(dest5);

  BrigOperandAddress addr5 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0
  };
  addr5.size = sizeof(addr5);

  BrigOperandCompound comp5 = {
    0,
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(dest5),
    0,
    4
  };
  comp5.size = sizeof(comp5);

  BrigInstMem out5 = {
    0,
    BrigEInstMem,
    BrigLda,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(dest5) + sizeof(addr5), 0, 0, 0},
    BrigFlatSpace
  };
  out5.size = sizeof(out5);

  symbolName.assign("&array");
  Ld_Test<BrigInstMem, BrigOperandReg> 
    TestCase5(in, sbuf, &out5, NULL, &dest5, &comp5, &addr5, NULL, NULL, &symbolName);
  TestCase5.Run_Test(&Lda);
  sbuf->clear();

  delete sbuf;

}

/*********************** Ldc Test ***************************/
TEST(CodegenTest, Ldc_Codegen){

  std::string in, op1, op2;
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "ldc_u64 $d2, @lab;\n");
  op1.assign("$d2"); sbuf->append(op1);

  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest1.size = sizeof(dest1);

  BrigOperandLabelRef label1 = {
    0,
    BrigEOperandLabelRef,
    0,
  };
  label1.size = sizeof(label1);

  BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigLdc,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(dest1), 0, 0, 0},
  };
  out1.size = sizeof(out1);

  Ld_Test<BrigInstBase, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1, &label1);
  TestCase1.Run_Test(&Ldc);
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ldc_u64 $d1, &some_function;\n");
  op1.assign("$d1"); sbuf->append(op1);

  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0,
    0
  };
  dest2.size = sizeof(dest2);

  BrigOperandFunctionRef function2 = {
    0,
    BrigEOperandFunctionRef,
    0,
  };
  function2.size = sizeof(function2);

  BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigLdc,
    Brigu64,
    BrigNoPacking,
    {0, sizeof(dest2), 0, 0, 0},
  };
  out2.size = sizeof(out2);

  Ld_Test<BrigInstBase, BrigOperandReg> TestCase2(in, sbuf, &out2, &dest2, &function2);
  TestCase2.Run_Test(&Ldc);
  sbuf->clear();

/******************************  End of tests *****************************************/

  delete sbuf;
}

TEST(ErrorReportTest, Ld) {  
  std::string symbolName;
  std::string input = "ld_readonly_s32 $s1, [&tbl][12]\n";
  symbolName.assign("&tbl");
  Ld_Test<> TestCase1(input, &symbolName);
  TestCase1.Run_Test(&Ld, MISSING_SEMICOLON);
  
  input.assign( "ld_arg_f32 [&input];\n");
  symbolName.assign("&input");
  Ld_Test<> TestCase2(input, &symbolName);
  TestCase2.Run_Test(&Ld, INVALID_FIRST_OPERAND);
  
  input.assign( "ld_group $d5, [100];\n");
  Ld_Test<> TestCase3(input);
  TestCase3.Run_Test(&Ld, MISSING_DATA_TYPE);
}


TEST(ErrorReportTest, Lda) {  
  std::string symbolName;
  std::string input = "lda_u32 $s1, [&loc]\n";
  symbolName.assign("&loc");
  Ld_Test<> TestCase1(input, &symbolName);
  TestCase1.Run_Test(&Lda, MISSING_SEMICOLON);
  
  input.assign( "lda_u64 $d4, $d5;\n");
  Ld_Test<> TestCase2(input);
  TestCase2.Run_Test(&Lda, INVALID_OPERAND);

  input.assign( "lda_u64 $d4 [&loc];\n");
  symbolName.assign("&loc");
  Ld_Test<> TestCase3(input, &symbolName);
  TestCase3.Run_Test(&Lda, MISSING_COMMA);
}

TEST(ErrorReportTest, Ldc) {  
  std::string input = "ldc_u64 $d2, @lab\n";
  Ld_Test<> TestCase1(input);
  TestCase1.Run_Test(&Ldc, MISSING_SEMICOLON);
  
  input.assign( "ldc_u64 $d4;\n");
  Ld_Test<> TestCase2(input);
  TestCase2.Run_Test(&Ldc, MISSING_COMMA);
}


}
}
