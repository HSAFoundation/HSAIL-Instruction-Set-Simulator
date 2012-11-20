#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"
#include "../mock_error_reporter.h"

using ::testing::AtLeast;
using ::testing::_;

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5> class Instruction5Opcode_Test : public BrigCodeGenTest{
private:

  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  const T3* RefSrc2;
  const T4* RefSrc3;
  const T5* RefSrc4;

public:
  Instruction5Opcode_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1, T3* Src2, T4* Src3, T5 *Src4) :
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2),
    RefSrc3(Src3),
    RefSrc4(Src4)  { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    oper->append(RefDest);
    oper->append(RefSrc1);
    oper->append(RefSrc2);
    oper->append(RefSrc3);
    oper->append(RefSrc4);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), (size_t)0, code->size(), oper->size(), (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
  }  
 };

TEST(CodegenTest,Instruction5Op_CodeGen){
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low

  std:: string in; std::string op1, op2, op3, op4, op5;
  StringBuffer* symbols;

  BrigOperandReg reg1, reg2, reg3, reg4, reg5;
  BrigOperandWaveSz wav;
  BrigOperandImmed imm2, imm3, imm4, imm5;


  /************************************* Test Case 1 reg reg reg reg reg************************************/
  symbols = new StringBuffer();
  in.assign( "f2u4_u32 $s1, $s0, $s3, $s2, $s4; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  op4.assign("$s2"); op5.assign("$s4");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  symbols->append(op4); symbols->append(op5);

    BrigInstBase out1 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4)}
  };
  out1.size = sizeof(out1);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  reg5.size = sizeof(reg5);
  reg5.kind = BrigEOperandReg;
  reg5.type = Brigb32;
  reg5.reserved = 0;
  reg5.s_name = op1.size() + op2.size() + op3.size() + op4.size() + 4;

  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg>
            TestCase1(in, symbols, &out1, &reg1, &reg2, &reg3, &reg4, &reg5);
  TestCase1.Run_Test(&Instruction5);
  symbols->clear();

  /*********************************** Test Case 2 reg reg reg reg imm**********************************/
  in.assign( "f2u4_u32 $s1, $s0, $s3, $s2, 3.4f; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  op4.assign("$s2");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  symbols->append(op4);

    BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4)}
  };
  out2.size = sizeof(out2);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  memset(&imm5, 0, sizeof(imm5));
  imm5.size = sizeof(imm5);
  imm5.kind = BrigEOperandImmed;
  imm5.type = Brigb32;
  imm5.reserved = 0;
  imm5.bits.f = 3.4f;

  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed>
            TestCase2(in, symbols, &out2, &reg1, &reg2, &reg3, &reg4, &imm5);
  TestCase2.Run_Test(&Instruction5);
  symbols->clear();

  /*********************************** Test Case 3 reg reg reg imm imm**********************************/
  in.assign( "f2u4_u32 $s1, $s0, $s3, 0.13f, 3.4f; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);

    BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(imm4)}
  };
  out3.size = sizeof(out3);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.f = 0.13f;

  memset(&imm5, 0, sizeof(imm5));
  imm5.size = sizeof(imm5);
  imm5.kind = BrigEOperandImmed;
  imm5.type = Brigb32;
  imm5.reserved = 0;
  imm5.bits.f = 3.4f;

  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandImmed, BrigOperandImmed>
            TestCase3(in, symbols, &out3, &reg1, &reg2, &reg3, &imm4, &imm5);
  TestCase3.Run_Test(&Instruction5);
  symbols->clear();

  /*********************************** Test Case 4 reg reg imm imm imm**********************************/
  in.assign( "f2u4_u32 $s1, $s0, 123.11f, 0.13f, 3.4f; \n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

    BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(imm3),
     sizeof(reg1) + sizeof(reg2) + sizeof(imm3) + sizeof(imm4)}
  };
  out4.size = sizeof(out4);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 123.11f;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.f = 0.13f;

  memset(&imm5, 0, sizeof(imm5));
  imm5.size = sizeof(imm5);
  imm5.kind = BrigEOperandImmed;
  imm5.type = Brigb32;
  imm5.reserved = 0;
  imm5.bits.f = 3.4f;

  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandImmed, BrigOperandImmed, BrigOperandImmed>
            TestCase4(in, symbols, &out4, &reg1, &reg2, &imm3, &imm4, &imm5);
  TestCase4.Run_Test(&Instruction5);
  symbols->clear();

  /*********************************** Test Case 5 reg imm imm imm imm**********************************/
  in.assign( "f2u4_u32 $s1, 255.0f, 123.11f, 0.13f, 3.4f; \n");
  op1.assign("$s1"); op2.assign("$s0");
  symbols->append(op1); symbols->append(op2);

    BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(imm2),
     sizeof(reg1) + sizeof(imm2) + sizeof(imm3),
     sizeof(reg1) + sizeof(imm2) + sizeof(imm3) + sizeof(imm4)}
  };
  out5.size = sizeof(out5);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  memset(&imm2, 0, sizeof(imm2));
  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  imm2.bits.f = 255.0f;

  memset(&imm3, 0, sizeof(imm3));
  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  imm3.bits.f = 123.11f;

  memset(&imm4, 0, sizeof(imm4));
  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  imm4.bits.f = 0.13f;

  memset(&imm5, 0, sizeof(imm5));
  imm5.size = sizeof(imm5);
  imm5.kind = BrigEOperandImmed;
  imm5.type = Brigb32;
  imm5.reserved = 0;
  imm5.bits.f = 3.4f;

  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandImmed, BrigOperandImmed, BrigOperandImmed, BrigOperandImmed>
            TestCase5(in, symbols, &out5, &reg1, &imm2, &imm3, &imm4, &imm5);
  TestCase5.Run_Test(&Instruction5);
  symbols->clear();

  /********************************* Test Case 6 reg reg reg reg wavesize*********************************/
  in.assign( "f2u4_u32 $s1, $s0, $s3, $s2, WAVESIZE; \n");
  op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
  op4.assign("$s2");
  symbols->append(op1); symbols->append(op2); symbols->append(op3);
  symbols->append(op4);

    BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     sizeof(reg1) + sizeof(reg2),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3),
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4)}
  };
  out6.size = sizeof(out6);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = op1.size() + op2.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = op1.size() + op2.size() + op3.size() + 3;

  wav.size = sizeof(wav);
  wav.kind = BrigEOperandWaveSz;


  Instruction5Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandReg, BrigOperandWaveSz>
            TestCase6(in, symbols, &out2, &reg1, &reg2, &reg3, &reg4, &wav);
  TestCase6.Run_Test(&Instruction5);
  symbols->clear();

/*******************************************************************/
delete symbols;
}

TEST(ErrorReportTest, Instruction5) {  
  Context* context = Context::get_instance();
  context->clear_context();

  MockErrorReporter mer;
  context->set_error_reporter(&mer);
  mer.DelegateToFake();
  EXPECT_CALL(mer, report_error(_, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mer, get_last_error()).Times(AtLeast(1));

  std::string input = "f2u4 $s1, $s0, 123, 0, 3;\n";
  
  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Instruction5(context));
  EXPECT_EQ(MISSING_DATA_TYPE, mer.get_last_error());
  
  input.assign("f2u4_u32 $s1, $s0, 123, 0, 3\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Instruction5(context));
  EXPECT_EQ(MISSING_SEMICOLON, mer.get_last_error());

  input.assign("f2u4_u32 $s1, $s0, 123, 3;\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Instruction5(context));
  EXPECT_EQ(MISSING_COMMA, mer.get_last_error());

  context->set_error_reporter(ErrorReporter::get_instance());
  delete lexer;
}

}
}
