#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

template <typename T = BrigOperandReg> 
class Cvt_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const BrigInstCvt* RefInst;
  //Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const T* RefSrc1;
     
public:
  Cvt_Test(std::string& in):
    BrigCodeGenTest(in) {}

  Cvt_Test(std::string& in, StringBuffer* sbuf, BrigInstCvt* ref, BrigOperandReg* Dest, T* Src1) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1) { }
 
  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    oper->append(RefDest);
    oper->append(RefSrc1);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), (size_t)0, code->size(), oper->size(), (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
  }  
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }
 }; 

TEST(CodegenTest, Cvt_CodeGen){  
  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; std::string op1, op2;
  StringBuffer* symbols;
  
  BrigOperandReg reg1, reg2;
  BrigOperandWaveSz wav;
  BrigOperandImmed imm2;  
   
  /************************************* Test Case 1************************************/
  symbols = new StringBuffer();
  in.assign( "cvt_f32_f64 $s1, $d1 ;\n");
  op1.assign("$s1"); op2.assign("$d1"); 
  symbols->append(op1); symbols->append(op2); 

  BrigInstCvt out1 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {0,0,0,0,0,0,0},
    Brigf64,
    0
  };
  out1.size = sizeof(out1);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = op1.size() + 1;

  Cvt_Test<BrigOperandReg> 
            TestCase1(in, symbols, &out1, &reg1, &reg2);
  TestCase1.Run_Test(&Cvt);
  symbols->clear();
  
  /************************************* Test Case 2************************************/
  in.assign( "cvt_downi_f32_f32 $s1, $s2 ;\n");
  op1.assign("$s1"); op2.assign("$s2"); 
  symbols->append(op1); symbols->append(op2); 

  BrigInstCvt out2 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,0,3,0,0,0,0},
    Brigf32,
    0
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

  Cvt_Test<BrigOperandReg> 
            TestCase2(in, symbols, &out2, &reg1, &reg2);
  TestCase2.Run_Test(&Cvt);
  symbols->clear();
  
  /************************************* Test Case 3************************************/
  in.assign( "cvt_upi_f32_f32 $s1, 3.14f ;\n");
  op1.assign("$s1");
  symbols->append(op1); 

  BrigInstCvt out3 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,0,2,0,0,0,0},
    Brigf32,
    0
  };
  out3.size = sizeof(out3);
  
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
  imm2.bits.f = 3.14f;

  Cvt_Test<BrigOperandImmed> 
            TestCase3(in, symbols, &out3, &reg1, &imm2);
  TestCase3.Run_Test(&Cvt);
  symbols->clear();

  /************************************* Test Case 4************************************/
  in.assign( "cvt_downi_f32_f32 $s1, WAVESIZE ;\n");
  op1.assign("$s1");
  symbols->append(op1); 

  BrigInstCvt out4 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,1,3,0,0,0,0},
    Brigf32,
    0
  };
  out4.size = sizeof(out4);
  
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  wav.size = sizeof(wav);
  wav.kind = BrigEOperandWaveSz;

  Cvt_Test<BrigOperandWaveSz> 
            TestCase4(in, symbols, &out4, &reg1, &wav);
 // TestCase4.Run_Test(&Cvt);
  symbols->clear();

  /************************************* Test Case 5************************************/
  in.assign( "cvt_ftz_up_f32_f16 $s1, 3.14f ;\n");
  op1.assign("$s1");
  symbols->append(op1); 

  BrigInstCvt out5 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,1,2,1,0,0,0},
    Brigf16,
    0
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
  imm2.bits.f = 3.14f;

  Cvt_Test<BrigOperandImmed> 
            TestCase5(in, symbols, &out5, &reg1, &imm2);
  TestCase5.Run_Test(&Cvt);
  symbols->clear();

  /************************************* Test Case 6************************************/
  in.assign( "cvt_up_u32_f32 $s1, 3.14f ;\n");
  op1.assign("$s1");
  symbols->append(op1); 

  BrigInstCvt out6 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigu32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,1,2,0,0,0,0},
    Brigf32,
    0
  };
  out6.size = sizeof(out6);
  
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
  imm2.bits.f = 3.14f;

  Cvt_Test<BrigOperandImmed> 
            TestCase6(in, symbols, &out6, &reg1, &imm2);
  TestCase6.Run_Test(&Cvt);
  symbols->clear();

 /************************************* Test Case 7************************************/
  in.assign( "cvt_upi_f32_s32 $s1, 35 ;\n");
  op1.assign("$s1");
  symbols->append(op1); 

  BrigInstCvt out7 = {
    0,
    BrigEInstCvt,
    BrigCvt,
    Brigf32,
    BrigNoPacking,
    {0, sizeof(reg1), 0, 0, 0},
    {1,0,2,0,0,0,0},
    Brigs32,
    0
  };
  out7.size = sizeof(out7);
  
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
  imm2.bits.u = 35;

  Cvt_Test<BrigOperandImmed> 
            TestCase7(in, symbols, &out7, &reg1, &imm2);
  TestCase7.Run_Test(&Cvt);
  symbols->clear();

/********************************************************************/
delete symbols;

}

TEST(ErrorReportTest, Cvt) {  
  std::string input =  "cvt_upi_f32_s32 $s1, 35\n";
  Cvt_Test<> TestCase1(input);
  TestCase1.Run_Test(&Cvt, MISSING_SEMICOLON);

  input.assign( "cvt_up_f32 $s1, 3.14f ;\n");
  Cvt_Test<> TestCase2(input);
  TestCase2.Run_Test(&Cvt, MISSING_DATA_TYPE);

  input.assign( "cvt_f64_f64 $d1;\n");
  Cvt_Test<> TestCase3(input);
  TestCase3.Run_Test(&Cvt, MISSING_COMMA);
}

}
}
