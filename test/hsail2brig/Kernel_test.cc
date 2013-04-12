#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

class Kernel_CodeGen_SimpleTest : public BrigCodeGenTest{
private:
  const Buffer *RefDir;
  const Buffer *RefOper;
  const Buffer *RefCode;

public:
  Kernel_CodeGen_SimpleTest(std::string& input, StringBuffer* sbuf, Buffer* dir,
      Buffer* oper, Buffer* code) :
    BrigCodeGenTest(input, sbuf),
    RefDir(dir),
    RefOper(oper),
    RefCode(code) { }

  void Run_Test(int (*Rule)(Context*)){

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]),
      reinterpret_cast<const char *>(&RefDir->get()[0]), reinterpret_cast<const char *>(&RefCode->get()[0]),
      reinterpret_cast<const char *>(&RefOper->get()[0]), NULL,
      RefStr->size(), RefDir->size(), RefCode->size(), RefOper->size(), (size_t)0);

    Parse_Validate(Rule, &RefOutput);
  }
};

TEST(CodegenTest, Kernel_SimpleTest_Codegen){

  /***********************************Common variables used by all tests**************************************/
  std::string in, op1, op2, op3, op4, op5, op6, op7, op8, op9, op10;
  StringBuffer* sbuf = new StringBuffer();
  Buffer *dir  = new Buffer();
  Buffer *oper = new Buffer();
  Buffer *code = new Buffer();
  BrigOperandReg regs0, regs2, regs3, regs4, regs5;
  BrigOperandImmed imm;
  BrigsOffset32_t s_name = sbuf->size();
  BrigoOffset32_t o_operand = oper->size();
  BrigoOffset32_t s2_operand = oper->size();

 /****************************************************************/
  in.assign("kernel &_kernel( \n");
  in.append("kernarg_u32 %arg0, \n");
  in.append("kernarg_u32 %arg1) \n");
  in.append("{ \n");
  in.append("@begin: \n");
  in.append("  private_s32 %arg2;\n");
  in.append("  workitemabsid $s0, 0; \n");
  in.append("  ld_kernarg_u32 $s2, [%arg0]; \n");
  in.append("  mad_u32 $s5, $s4, $s2, $s3; \n");
  in.append("  ret; \n");
  in.append("}; \n");

  op1.assign("&_kernel"); op2.assign("%arg0"); op3.assign("%arg1"); op4.assign("@begin");
  op5.assign("%arg2"); op6.assign("$s0"); op7.assign("$s2"); op8.assign("$s5");
  op9.assign("$s4"); op10.assign("$s3");

  sbuf->append(op1); sbuf->append(op2); sbuf->append(op3); sbuf->append(op4); sbuf->append(op5);
  sbuf->append(op6); sbuf->append(op7); sbuf->append(op8); sbuf->append(op9); sbuf->append(op10);

/*******************************directive****************************************/
  s_name = 0 ;
  BrigDirectiveKernel bdk = {
    sizeof(BrigDirectiveKernel),   // size
    BrigEDirectiveKernel,          // kind
    0,                             // c_code
    s_name,                        // s_name
    2,                             // inParamCount
    120,                           // d_firstScopedDirective
    4,                             // operationCount
    172,                           // d_nextDirective
    BrigNone,                      // attribute
    0,                             // fbar
    0,                             // outParamCount
    40,                            // d_firstInParam
  };

  s_name += (op1.size() + 1);
  BrigDirectiveSymbol kernarg0 = {
    sizeof(BrigDirectiveSymbol),   // size
    BrigEDirectiveSymbol ,        // kind
    {
      0,                         // c_code
      BrigKernargSpace,          // storag class kernarg
      BrigNone ,                 // attribut
      0,                         // reserved
      0,                         // symbolModifier
      0,                         // dim
      s_name,                    // s_name
      Brigu32,                   // type
      4,                         // align
    },
    0,                          // d_init
    0                          // reserved
  };

  s_name += (op2.size() + 1);
  BrigDirectiveSymbol kernarg1 = {
    sizeof(BrigDirectiveSymbol),  // size
    BrigEDirectiveSymbol ,    // kind
    {
      0,                        // c_code
      BrigKernargSpace,         // storag class kernarg
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      s_name,                   // s_name
      Brigu32,                  // type
      4,                        // align
    },
    0,                        // d_init
    0                         // reserved
  };

  s_name += (op3.size() + 1);
  BrigDirectiveLabel labRef = {
    sizeof(BrigDirectiveLabel), // size
    BrigEDirectiveLabel,        // kind
    0,                          // c_code
    s_name                      // s_name
  };

  s_name += (op4.size() + 1);
  BrigDirectiveSymbol arg2 = {
    sizeof(BrigDirectiveSymbol),  // size
    BrigEDirectiveSymbol ,        // kind
    {
      0,                        // c_code
      BrigPrivateSpace,         // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      s_name,                   // s_name
      Brigs32,                  // type
      4,                        // align
    },
    0,                          // d_init
    0                           // reserved
  };
  dir->append(&bdk);
  dir->append(&kernarg0);
  dir->append(&kernarg1);
  dir->append(&labRef);
  dir->append(&arg2);

/*******************instruction 1*****************/
  s_name += (op5.size()  + 1);
  o_operand = oper->size();

  regs0.size = sizeof(regs0);
  regs0.kind = BrigEOperandReg;
  regs0.type = Brigb32;
  regs0.reserved = 0;
  regs0.s_name = s_name;

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 0;

  BrigInstBase inst1 = {
    0,
    BrigEInstBase,
    BrigWorkItemAbsId,
    Brigb32,
    BrigNoPacking,
    {o_operand, (unsigned) (o_operand + sizeof(regs0)), 0, 0, 0}
  };
  inst1.size = sizeof(inst1);

  oper->append(&regs0);
  oper->append(&imm);
  code->append(&inst1);

/*******************instruction 2*****************/
  s_name += (op6.size() + 1);
  o_operand = oper->size();

  regs2.size = sizeof(regs2);
  regs2.kind = BrigEOperandReg;
  regs2.type = Brigb32;
  regs2.reserved = 0;
  regs2.s_name = s_name;

  BrigOperandImmed width = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };

  width.size = sizeof(width);
  width.bits.u = 1;

  BrigOperandAddress addr = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    sizeof(bdk)               //directive
  };
  addr.size = sizeof(addr);

  BrigInstLdSt inst2 = {
    0,
    BrigEInstLdSt,
    BrigLd,
    Brigu32,
    BrigNoPacking,
    {o_operand,
     (unsigned) (o_operand + sizeof(width)),
     (unsigned) (o_operand + sizeof(width) + sizeof(regs2)),
     0, 0},
    BrigKernargSpace,
    BrigRegular,
    0
  };
  inst2.size = sizeof(inst2);

  oper->append(&width);
  s2_operand = oper->size();
  oper->append(&regs2);
  oper->append(&addr);
  code->append(&inst2);

/************************instruction 3****************************/
  s_name += (op7.size() + 1);
  o_operand = oper->size();

  regs5.size = sizeof(regs5);
  regs5.kind = BrigEOperandReg;
  regs5.type = Brigb32;
  regs5.reserved = 0;
  regs5.s_name = s_name;

  regs4.size = sizeof(regs4);
  regs4.kind = BrigEOperandReg;
  regs4.type = Brigb32;
  regs4.reserved = 0;
  regs4.s_name = s_name + op8.size() + 1;

  regs3.size = sizeof(regs3);
  regs3.kind = BrigEOperandReg;
  regs3.type = Brigb32;
  regs3.reserved = 0;
  regs3.s_name = s_name + op8.size() + op9.size() + 2;

 BrigInstBase inst3 = {
    0,
    BrigEInstBase,
    BrigMad,
    Brigu32,
    BrigNoPacking,
    {o_operand,
     (unsigned) (o_operand + sizeof(regs5)),
     s2_operand,
     (unsigned) (o_operand + sizeof(regs5) + sizeof(regs4)),
     0}
  };
  inst3.size = sizeof(inst3);

  oper->append(&regs5);
  oper->append(&regs4);
  oper->append(&regs3);
  code->append(&inst3);

/************************instruction 4****************************/
  o_operand = oper->size();
  s_name += (op8.size() + op9.size() + op10.size() + 3);

  BrigInstBase inst4 = {
    0,
    BrigEInstBase,
    BrigRet,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
  };
  inst4.size = sizeof(inst4);
  code->append(&inst4);

  Kernel_CodeGen_SimpleTest TestCase1(in, sbuf, dir, oper, code);
  TestCase1.Run_Test(&Kernel);
  sbuf->clear();

/*************************************************************/
  delete sbuf;
  delete dir;
  delete code;
  delete oper;
}

}
}
