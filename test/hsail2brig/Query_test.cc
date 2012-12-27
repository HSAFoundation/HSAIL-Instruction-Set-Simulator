#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

class Query_Test : public BrigCodeGenTest{
private:

  const BrigInstBase* RefInst;
  const BrigOperandReg* RefDest;
  const BrigOperandOpaque* RefOpaque;
  const BrigOperandReg* RefOpaqueReg;
  const std::string *SymbolName;

public:
  Query_Test(std::string& in):
    BrigCodeGenTest(in), RefInst(NULL), RefDest(NULL),
    RefOpaque(NULL), RefOpaqueReg(NULL), SymbolName(NULL) {}

  Query_Test(std::string& in, StringBuffer* sbuf, BrigInstBase* ref,
      BrigOperandReg* Dest, BrigOperandOpaque* Src1, BrigOperandReg* Src2 = NULL,
      std::string *name = NULL) :
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),           RefDest(Dest),
    RefOpaque(Src1),        RefOpaqueReg(Src2),
    SymbolName(name) {}

   void Run_Test(int (*Rule)(Context*)){
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    oper->append(RefDest);
    if (RefOpaqueReg)
      oper->append(RefOpaqueReg);
    oper->append(RefOpaque);

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]),
      NULL, reinterpret_cast<const char *>(&code->get()[0]),
      reinterpret_cast<const char *>(&oper->get()[0]), NULL,
      RefStr->size(), (size_t)0, code->size(), oper->size(), (size_t)0);

    Parse_Validate(Rule, &RefOutput, SymbolName);
    delete code;
    delete oper;
  }

  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError, SymbolName);
  }

};

TEST(CodegenTest, Query_CodeGen){

  std::string in, symbolName;
  std::string op1, op2, op3;
  StringBuffer* symbols;

  BrigOperandReg reg1;

  /***************** Test Case 1 **********************/
  symbols = new StringBuffer();
  in.assign( "query_width_u32 $s1, [&RWImg3];\n");
  op1.assign("$s1");
  symbols->append(op1); symbols->append(op2);

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
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(dest1),
     0, 0, 0}
  };
  out1.size = sizeof(out1);

  BrigOperandOpaque src1 = {
    0,                    // size
    BrigEOperandOpaque,   // kind
    0,
    0,                    // reg
    0                     // Offset
  };
  src1.size = sizeof(src1);

  symbolName.assign("&RWImg3");
  Query_Test TestCase1(in, symbols, &out1, &dest1, &src1, NULL, &symbolName);
  TestCase1.Run_Test(&Query);
  symbols->clear();


  /***************** Test Case 2 ************************/
  in.assign( "query_width_u32 $s1, [&RWImg3<$s0 + 4>];\n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out2 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out2.size = sizeof(out2);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase2(in, symbols, &out2, &dest1, &src1, &reg1, &symbolName);
  TestCase2.Run_Test(&Query);
  symbols->clear();


  /*********************** Test Case 3 *************************/
  in.assign( "query_width_u32 $s1, [&RWImg3<$s0>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out3 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out3.size = sizeof(out3);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase3(in, symbols, &out3, &dest1, &src1, &reg1, &symbolName);
  TestCase3.Run_Test(&Query);
  symbols->clear();

  /************************ Test Case 4 ************************/
  in.assign( "query_width_u32 $s1, [&RWImg3<4>]; \n");
  op1.assign("$s1");
  symbols->append(op1);

   BrigInstBase out4 = {
    0,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(reg1),
     0, 0, 0}
  };
  out4.size = sizeof(out4);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = 0;
  src1.offset = 4;

  symbolName.assign("&RWImg3");
  Query_Test TestCase4(in, symbols, &out4, &dest1, &src1, NULL, &symbolName);
  TestCase4.Run_Test(&Query);
  symbols->clear();

  /******************** Test Case 5 ************************/
  in.assign( "query_height_u32 $s1, [&RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out5 = {
    0,
    BrigEInstBase,
    BrigQueryHeight,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out5.size = sizeof(out5);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase5(in, symbols, &out5, &dest1, &src1, &reg1, &symbolName);
  TestCase5.Run_Test(&Query);
  symbols->clear();

  /******************* Test Case 6 *******************/
  in.assign( "query_depth_u32 $s1, [&RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out6 = {
    0,
    BrigEInstBase,
    BrigQueryDepth,
    Brigu32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out6.size = sizeof(out6);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase6(in, symbols, &out6, &dest1, &src1, &reg1, &symbolName);
  TestCase6.Run_Test(&Query);
  symbols->clear();

  /************************* Test Case 7 ************************/
  in.assign( "query_array_b32 $s1, [&RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out7 = {
    0,
    BrigEInstBase,
    BrigQueryArray,
    Brigb32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out7.size = sizeof(out7);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase7(in, symbols, &out7, &dest1, &src1, &reg1, &symbolName);
  TestCase7.Run_Test(&Query);
  symbols->clear();

  /******************* Test Case 8 *********************/
  in.assign( "query_normalized_b32 $s1, [&RWSamp3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out8 = {
    0,
    BrigEInstBase,
    BrigQueryNormalized,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out8.size = sizeof(out8);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWSamp3");
  Query_Test TestCase8(in, symbols, &out8, &dest1, &src1, &reg1, &symbolName);
  TestCase8.Run_Test(&Query);
  symbols->clear();

  /********************** Test Case 9 *******************/
  in.assign( "query_filtering_b32 $s1, [&RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out9 = {
    0,
    BrigEInstBase,
    BrigQueryFiltering,
    Brigb32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out9.size = sizeof(out9);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase9(in, symbols, &out9, &dest1, &src1, &reg1, &symbolName);
  TestCase9.Run_Test(&Query);
  symbols->clear();

  /***************** Test Case 10 *******************/
  in.assign( "query_order_b32 $s1, [&RWImg3<$s0 + 10>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out10 = {
    0,
    BrigEInstBase,
    BrigQueryOrder,
    Brigb32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out10.size = sizeof(out10);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 10;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase10(in, symbols, &out10, &dest1, &src1, &reg1, &symbolName);
  TestCase10.Run_Test(&Query);
  symbols->clear();

  /********************** Test Case 11 ******************/
  in.assign( "query_data_b32 $s1, [&RWImg3<$s0 + 4>]; \n");
  op1.assign("$s1"); op3.assign("$s0");
  symbols->append(op1); symbols->append(op3);

   BrigInstBase out11 = {
    0,
    BrigEInstBase,
    BrigQueryData,
    Brigb32,
    BrigNoPacking,
    {0,
     sizeof(dest1) + sizeof(reg1),
     0, 0, 0}
  };
  out11.size = sizeof(out11);

  dest1.size = sizeof(reg1);
  dest1.kind = BrigEOperandReg;
  dest1.type = Brigb32;
  dest1.reserved = 0;
  dest1.s_name = 0;

  src1.size = sizeof(src1);
  src1.kind = BrigEOperandOpaque;
  src1.directive = 0;
  src1.reg = sizeof(dest1);
  src1.offset = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size()+1;

  symbolName.assign("&RWImg3");
  Query_Test TestCase11(in, symbols, &out11, &dest1, &src1, &reg1, &symbolName);
  TestCase11.Run_Test(&Query);
  symbols->clear();

  delete symbols;
}

TEST(ErrorReportTest, Query) {
  std::string input = "query_order $s1, [&RWImg3<$s0 + 10>];\n";
  Query_Test TestCase1(input);
  TestCase1.Run_Test(&Query, MISSING_DATA_TYPE);

  input.assign( "query_width_u32 $s1 [&RWImg3]; \n");
  Query_Test TestCase2(input);
  TestCase2.Run_Test(&Query, MISSING_COMMA);

  input.assign( "query_width_u32 $s1, $s3; \n");
  Query_Test TestCase3(input);
  TestCase3.Run_Test(&Query, INVALID_OPERAND);
}

} // namespace hsa
} // namespace brig
