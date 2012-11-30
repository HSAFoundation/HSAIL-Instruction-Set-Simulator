#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4 or immediate value*/
template <typename T=BrigOperandReg> class St_Test : public BrigCodeGenTest{
private:

  const BrigInstLdSt* RefInst;

  //source Operand - reg or immed
  const T* RefSrc;
  //destination operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandReg* RefDest_Reg;
  const BrigOperandAddress* RefDest_Addr;
  const BrigOperandIndirect* RefDest_Indir;
  const BrigOperandCompound* RefDest_Comp;
  const BrigOperandReg *RefReg1, *RefReg2, *RefReg3, *RefReg4;

public:
  St_Test(std::string& in):
    BrigCodeGenTest(in) {}
  
  //TestCase outputs a BrigOperandAddress only
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref,
      T* src, BrigOperandAddress* addr,
      BrigOperandReg *Reg1 = NULL,
      BrigOperandReg *Reg2 = NULL,
      BrigOperandReg *Reg3 = NULL,
      BrigOperandReg *Reg4 = NULL) :
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefSrc(src),
    RefDest_Reg(NULL),
    RefDest_Addr(addr),
    RefDest_Indir(NULL),
    RefDest_Comp(NULL),
    RefReg1(Reg1),
    RefReg2(Reg2),
    RefReg3(Reg3),
    RefReg4(Reg4) { }

  //Testcase output is a BrigOperandIndirect
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref,
      T* src, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL,
      BrigOperandReg *Reg1 = NULL,
      BrigOperandReg *Reg2 = NULL,
      BrigOperandReg *Reg3 = NULL,
      BrigOperandReg *Reg4 = NULL) :
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefSrc(src),
    RefDest_Reg(reg),
    RefDest_Addr(NULL),
    RefDest_Indir(indir),
    RefDest_Comp(NULL),
    RefReg1(Reg1),
    RefReg2(Reg2),
    RefReg3(Reg3),
    RefReg4(Reg4)  { }

  //TestCase output is a BrigOperandCompound
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref,
     T* src, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL,
      BrigOperandReg *Reg1 = NULL,
      BrigOperandReg *Reg2 = NULL,
      BrigOperandReg *Reg3 = NULL,
      BrigOperandReg *Reg4 = NULL) :
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefSrc(src),
    RefDest_Reg(reg),
    RefDest_Addr(addr),
    RefDest_Indir(NULL),
    RefDest_Comp(comp),
    RefReg1(Reg1),
    RefReg2(Reg2),
    RefReg3(Reg3),
    RefReg4(Reg4)   { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    code->append(RefInst);
    if (RefReg1)
      oper->append(RefReg1);
    if (RefReg2)
      oper->append(RefReg2);
    if (RefReg3)
      oper->append(RefReg3);
    if (RefReg4)
      oper->append(RefReg4);

    oper->append(RefSrc);
    if (RefDest_Addr)
      oper->append(RefDest_Addr);
    if (RefDest_Reg)
      oper->append(RefDest_Reg);
    if (RefDest_Indir)
      oper->append(RefDest_Indir);
    if (RefDest_Comp)
      oper->append(RefDest_Comp);
    
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

TEST(CodegenTest, St_Codegen){

  /***********************************Common variables used by all tests**************************************/
  std::string in, op1, op2, op3, op4, op5, op6;
  StringBuffer* sbuf = new StringBuffer();

  /*********************************** Test Case 1************************************************************/

  in.assign( "st_arg_f32 $s0, [%output][$d2-4];\n");
  op1.assign("$s0"); op2.assign("%output"); op3.assign("$d2");
  sbuf->append(op1); sbuf->append(op2); sbuf->append(op3);

  //Reference to $s0
  BrigOperandReg src = {
  0,
  BrigEOperandReg,
  Brigb32,
  0,
  0  //Offset to string table
  };
  src.size = sizeof(src);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  BrigOperandAddress dest1 = {
  0,
  BrigEOperandAddress,
  Brigb64,
  0,
  0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
  dest1.size = sizeof(dest1);

  //Ref to $s2
  BrigOperandReg dest2 = {
  0,
  BrigEOperandReg,
  Brigb64,
  0,
  op1.size() + op2.size() +2,
  };
  dest2.size = sizeof(dest2);

  BrigOperandCompound comp = {
  0,
  BrigEOperandCompound,
  Brigb64,
  0,
  sizeof(src),//byte offset to operand addr
  sizeof(src) + sizeof(dest1), //to operand reg
  -4  //Offset in the included test case
  };
  comp.size = sizeof(comp);

  BrigInstLdSt out1 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out1.size = sizeof( out1);

  St_Test<BrigOperandReg> TestCase1(in, sbuf, &out1, &src, &comp, &dest1, &dest2);
  TestCase1.Run_Test(&St);
  sbuf->clear();

  /*********************************** Test Case 2************************************************************/

  in.assign( "st_arg_f32 $s0, [$d2-4];\n");
  op1.assign("$s0");  op2.assign("$d2");
  sbuf->append(op1); sbuf->append(op2);

  //Reference to $s0
  src.size = sizeof(src);
  src.kind = BrigEOperandReg;
  src.type = Brigb32;
  src.reserved = 0;
  src.s_name = 0;

  //Ref to $d2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb64;
  dest2.reserved = 0;
  dest2.s_name = op1.size() + 1;

  BrigOperandIndirect indirect = {
  0,
  BrigEOperandIndirect,
  sizeof(src), //to operand reg
  Brigb64,
  0,
  -4  //Offset in the included test case
  };
  indirect.size = sizeof(indirect);

  BrigInstLdSt out2 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest2) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out2.size = sizeof(out2);

  St_Test<BrigOperandReg> TestCase2(in, sbuf, &out2, &src, &indirect, &dest2);
  TestCase2.Run_Test(&St);
  sbuf->clear();

  /*********************************** Test Case 3************************************************************/

  in.assign( "st_arg_f32 $s0, [%output];\n");
  op1.assign("$s0"); op2.assign("%output");
  sbuf->append(op1);sbuf->append(op2);

  //Reference to $s0
  src.size = sizeof(src);
  src.kind = BrigEOperandReg;
  src.type = Brigb32;
  src.reserved = 0;
  src.s_name = 0;

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  BrigInstLdSt out3 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out3.size = sizeof(out3);

  St_Test<BrigOperandReg> TestCase3(in, sbuf, &out3, &src, &dest1);
  TestCase3.Run_Test(&St);
  sbuf->clear();

  /*********************************** Test Case 4************************************************************/

  in.assign( "st_arg_f32 0x1234, [%output];\n");
  op1.assign("%output");
  sbuf->append(op1);

  //for immed
  BrigOperandImmed src1 = {
    0, //size
    BrigEOperandImmed, //kind
    Brigb32,                      //type
    0,                                 //reserved
    {0x1234}
  };
  src1.size = sizeof(src1);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.


  BrigInstLdSt out4 = {
    0,                // size
    BrigEInstLdSt,     // kind

    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing

    {0, sizeof(src1) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic

    0                  // equivClass
  };
  out4.size = sizeof(out4);

  St_Test<BrigOperandImmed> TestCase4(in, sbuf, &out4, &src1, &dest1);
  TestCase4.Run_Test(&St);
  sbuf->clear();

  /*********************************** Test Case 5************************************************************/

  in.assign( "st_arg_f64 3.1415l, [$d2-4];\n");
  op1.assign("$d2");
  sbuf->append(op1);

  //Reference to immed
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandImmed;
  src1.type = Brigb64;
  src1.reserved = 0;
  src1.bits.d = 3.1415l;

  //Ref to $d2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb64;
  dest2.reserved = 0;
  dest2.s_name = 0;

  indirect.size = sizeof(indirect);
  indirect.kind = BrigEOperandIndirect;
  indirect.reg = sizeof(src1);
  indirect.type = Brigb64;
  indirect.reserved = 0;
  indirect.offset = -4 ;

  BrigInstLdSt out5 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf64,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out5.size = sizeof(out5);

  St_Test<BrigOperandImmed> TestCase5(in, sbuf, &out5, &src1, &indirect, &dest2);
  TestCase5.Run_Test(&St);
  sbuf->clear();

 /*********************************** Test Case 6************************************************************/

  in.assign( "st_arg_f64 3.1415l, [%output][$d2-4];\n");
  op1.assign("%output");  op2.assign("$d2");
  sbuf->append(op1);  sbuf->append(op2);

  //Reference to immed
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandImmed;
  src1.type = Brigb64;
  src1.reserved = 0;
  src1.bits.d = 3.1415l;

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  //Ref to $d2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb64;
  dest2.reserved = 0;
  dest2.s_name = op1.size() + 1;

  // ref to comp
  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(src1);
  comp.reg = sizeof(src1) + sizeof(dest1);
  comp.offset = -4 ;

  BrigInstLdSt out6 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf64,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src1) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out6.size = sizeof( out6);


  St_Test<BrigOperandImmed> TestCase6(in, sbuf, &out6, &src1, &comp, &dest1, &dest2);
  TestCase6.Run_Test(&St);
  sbuf->clear();

 /*********************************** Test Case 7************************************************************/

  in.assign( "st_v2_f32 ($s0,$s1), [%output][$d2-4];\n");
  op1.assign("$s0");  op2.assign("$s1");
  op3.assign("%output");  op4.assign("$d2");
  sbuf->append(op1);  sbuf->append(op2);
  sbuf->append(op3);  sbuf->append(op4);

  //Reference to reg $s0
  BrigOperandReg reg0 = {
   0,
   BrigEOperandReg,
   Brigb32,
   0,
   0
  };
  reg0.size = sizeof(reg0);

  //Reference to reg $s1
  BrigOperandReg reg1 = {
   0,                             //size
   BrigEOperandReg,               //kind
   Brigb32,                       //type
   0,                             //reserved
   op1.size() + 1                 //name
  };
  reg1.size = sizeof(reg1);

  //Reference to reg $s0 and $s1
  BrigOperandRegV2 regv2 = {
   0,                             //size
   BrigEOperandRegV2,             //kind
   Brigb32,                       //type
   0,                             //reserved
   {0, sizeof(reg0)}              //regs[2]
  };
  regv2.size = sizeof(regv2);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  //Ref to $d2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb64;
  dest2.reserved = 0;
  dest2.s_name = op1.size()  + op2.size() + op3.size() + 3;

  // ref to comp
  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(reg0) + sizeof(reg1) + sizeof(regv2) ; //for BrigOperandAddress
  comp.reg = sizeof(reg0) + sizeof(reg1)
                + sizeof(regv2) + sizeof(dest1);
  comp.offset = -4 ;

  BrigInstLdSt out7 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {sizeof(reg0) + sizeof(reg1), sizeof(reg0) + sizeof(reg1) + sizeof(regv2)
         + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigFlatSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out7.size = sizeof( out7);


  St_Test<BrigOperandRegV2> TestCase7(in, sbuf, &out7, &regv2, &comp, &dest1, &dest2,
                              &reg0, &reg1);
  TestCase7.Run_Test(&St);
  sbuf->clear();

 /*********************************** Test Case8************************************************************/

  in.assign( "st_v4_f32 ($s0,$s1,$s2,$s3), [%output][$d4-4];\n");
  op1.assign("$s0");  op2.assign("$s1");
  op3.assign("$s2");  op4.assign("$s3");
  op5.assign("%output");  op6.assign("$d4");
  sbuf->append(op1);  sbuf->append(op2);
  sbuf->append(op3);  sbuf->append(op4);
  sbuf->append(op5);  sbuf->append(op6);

  //Reference to reg $s0
  reg0.size = sizeof(reg0);
  reg0.kind = BrigEOperandReg;
  reg0.type = Brigb32;
  reg0.reserved = 0;
  reg0.s_name = 0;

  //Reference to reg $s1
  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = op1.size() + 1;

  //Reference to reg $s2
  BrigOperandReg reg2 = {
   0,
   BrigEOperandReg,
   Brigb32,
   0,
   op1.size() + op2.size() +2
  };
  reg2.size = sizeof(reg2);

  //Reference to reg $s3
  BrigOperandReg reg3 = {
   0,                                       // size
   BrigEOperandReg,                         // kind
   Brigb32,                                 // type
   0,                                       // reserved
   op1.size() + op2.size() + op3.size() + 3 // name
  };
  reg3.size = sizeof(reg3);

  //Reference to reg $s0 and $s1 $s2 $s3
  BrigOperandRegV4 regv4 = {
   0,                                             //size
   BrigEOperandRegV4,                             //kind
   Brigb32,                                       //type
   0,                                             //reserved
   {0, sizeof(reg0), sizeof(reg0) + sizeof(reg1),
      sizeof(reg0) + sizeof(reg1) + sizeof(reg2)} //regs[4]
  };
  regv4.size = sizeof(regv4);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  //Ref to $d4
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb64;
  dest2.reserved = 0;
  dest2.s_name = op1.size()  + op2.size() +
           op3.size() + op4.size() + op5.size()+ 5;

  // ref to comp
  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(reg0) + sizeof(reg1) +
                sizeof(reg2) + sizeof(reg3) + sizeof(regv4) ; //for BrigOperandAddress
  comp.reg = sizeof(reg0) + sizeof(reg1)
                + sizeof(reg2) + sizeof(reg3)
                + sizeof(regv4) + sizeof(dest1);
  comp.offset = -4 ;

  BrigInstLdSt out8 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {sizeof(reg0) + sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 
     sizeof(reg0) + sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(regv4)
         + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigFlatSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out8.size = sizeof( out8);


  St_Test<BrigOperandRegV4> TestCase8(in, sbuf, &out8, &regv4, &comp, &dest1, &dest2,
                                &reg0, &reg1, &reg2, &reg3);
  TestCase8.Run_Test(&St);
  sbuf->clear();

/************************************ End of tests*******************************************/
  delete sbuf;
}

TEST(ErrorReportTest, St) {  
  std::string input = "st_arg 3.1415l, [$s2-4];\n";
  St_Test<> TestCase1(input);
  TestCase1.Run_Test(&St, MISSING_DATA_TYPE);
  
  input.assign("st_arg_f64 3.1415l;\n");
  St_Test<> TestCase2(input);
  TestCase2.Run_Test(&St, MISSING_COMMA);
  
  input.assign( "st_arg_f32 $s0, [%output]\n");
  St_Test<> TestCase3(input);
  TestCase3.Run_Test(&St, MISSING_SEMICOLON);
}

}
}
