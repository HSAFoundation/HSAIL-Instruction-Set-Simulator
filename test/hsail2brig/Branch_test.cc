#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"
 
namespace hsa {
namespace brig {
 
template <typename Tinst = BrigInstBase, 
          typename T1 = BrigOperandReg, 
          typename T2 = BrigOperandReg, 
          typename T3 = BrigOperandReg>
class Branch_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer - Pointers to brig structures
  const Tinst* RefInst;
   
  //Operands
  const BrigOperandImmed* RefWidth;
  const T1* RefSrc1;
  const T2* RefSrc2;
  const T3* RefSrc3;

public:
  Branch_Test(std::string& in):
    BrigCodeGenTest(in) {}
  
  Branch_Test(std::string& in, StringBuffer* sbuf, Tinst* ref, 
              BrigOperandImmed* width, T1* src1, T2* src2 = NULL, T3* src3 = NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref), 
    RefWidth(width),
    RefSrc1(src1),
    RefSrc2(src2),
    RefSrc3(src3) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();
    code->append(RefInst);
    oper->append(RefWidth);
    oper->append(RefSrc1);
    if (RefSrc2 != NULL) {
      oper->append(RefSrc2);
    }
    if (RefSrc3 != NULL) {
      oper->append(RefSrc3);
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
  }  
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }
};

TEST(CodegenTest, Brn_Codegen){

  /********************* Common variables **********************/
  std::string in, regName; 
  StringBuffer* sbuf = new StringBuffer();
  BrigOperandImmed width;
  BrigOperandLabelRef labRef;
  BrigInstBase outInst;
  BrigInstMod outMod;
  BrigOperandReg reg;
  BrigOperandAddress addr;

  /*************************** Case 1 ********************************/
  in.assign("brn @loop;\n");

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;


  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigBrn;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = 0;
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  Branch_Test<BrigInstBase, BrigOperandLabelRef> TestCase1(in, sbuf, &outInst, &width, &labRef);
  TestCase1.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 2 ********************************/
  in.assign("brn_width(all)_fbar @label;\n");
  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigBrn;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width);
  outMod.o_operands[2] = 0;
  outMod.o_operands[3] = 0;
  outMod.o_operands[4] = 0;
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.valid = 1;
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;
 
  Branch_Test<BrigInstMod, BrigOperandLabelRef> TestCase2(in, sbuf, &outMod, &width, &labRef);
  TestCase2.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 3 ********************************/
  in.assign("brn_width(0)_fbar $s1;\n");
  regName.assign("$s1");  sbuf->append(regName);
  
  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigBrn;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width);
  outMod.o_operands[2] = 0;
  outMod.o_operands[3] = 0;
  outMod.o_operands[4] = 0;
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.valid = 1;
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;
  
  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  Branch_Test<BrigInstMod, BrigOperandReg> TestCase3(in, sbuf, &outMod, &width, &reg);
  TestCase3.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 4 ********************************/
  in.assign("brn_width(64) $s0;\n");
  regName.assign("$s0");  sbuf->append(regName);
  
  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigBrn;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = 0;
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 64;
  
  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  Branch_Test<BrigInstBase, BrigOperandReg> TestCase4(in, sbuf, &outInst, &width, &reg);
  TestCase4.Run_Test(&Branch);
  sbuf->clear();

  /*************************** Case 5 ********************************/
  in.assign("brn $s1, [%jumptable2];\n");
  regName.assign("$s1");  sbuf->append(regName);
  
  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigBrn;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = sizeof(width) + sizeof(reg);
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;
  
  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  Branch_Test<BrigInstBase, BrigOperandReg, BrigOperandAddress> TestCase5(in, sbuf, &outInst, &width, &reg, &addr);
  TestCase5.Run_Test(&Branch);
  sbuf->clear();

  /*************************** Case 6 ********************************/
  in.assign("brn_fbar $s3, [@targets];\n");
  regName.assign("$s3");  sbuf->append(regName);
  
  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigBrn;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width);
  outMod.o_operands[2] = sizeof(width) + sizeof(reg);
  outMod.o_operands[3] = 0;
  outMod.o_operands[4] = 0;
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.valid = 1;
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;
  
  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;

  Branch_Test<BrigInstMod, BrigOperandReg, BrigOperandLabelRef> TestCase6(in, sbuf, &outMod, &width, &reg, &labRef);
  TestCase6.Run_Test(&Branch);
  sbuf->clear();

  /***************************  End of tests ***********************/
  delete sbuf;
}


TEST(CodegenTest, Cbr_Codegen){

  /********************* Common variables **********************/
  std::string in, reg1Name, reg2Name;
  StringBuffer* sbuf = new StringBuffer();
  BrigOperandImmed width;
  BrigOperandLabelRef labRef;
  BrigInstBase outInst;
  BrigInstMod outMod;
  BrigOperandReg reg1, reg2;
  BrigOperandAddress addr;

  /*************************** Case 1 ********************************/
  in.assign("cbr_width(4) $c1, $s1, [@tab];\n");
  reg1Name.assign("$c1");   reg2Name.assign("$s1");
  sbuf->append(reg1Name);   sbuf->append(reg2Name);

  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigCbr;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = sizeof(width) + sizeof(reg1);
  outInst.o_operands[3] = sizeof(width) + sizeof(reg1) + sizeof(reg2);
  outInst.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 4;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;

  Branch_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandLabelRef> 
  TestCase1(in, sbuf, &outInst, &width, &reg1, &reg2, &labRef);
  TestCase1.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 2 ********************************/
  in.assign("cbr_width(all)_fbar $c2, $s2, [%x];");
  reg1Name.assign("$c2");   reg2Name.assign("$s2");
  sbuf->append(reg1Name);   sbuf->append(reg2Name);

  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigCbr;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width);
  outMod.o_operands[2] = sizeof(width) + sizeof(reg1);
  outMod.o_operands[3] = sizeof(width) + sizeof(reg1) + sizeof(reg2);
  outMod.o_operands[4] = 0;
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.valid = 1;
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;
 
  Branch_Test<BrigInstMod, BrigOperandReg, BrigOperandReg, BrigOperandAddress> 
  TestCase2(in, sbuf, &outMod, &width, &reg1, &reg2, &addr);
  TestCase2.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 3 ********************************/

  in.assign("cbr_width(2) $c0, @label;\n");
  reg1Name.assign("$c0");
  sbuf->append(reg1Name);

  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigCbr;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = sizeof(width) + sizeof(reg1);
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 2;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.s_name = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;
 
  Branch_Test<BrigInstBase, BrigOperandReg, BrigOperandLabelRef> 
  TestCase3(in, sbuf, &outInst, &width, &reg1, &labRef);
  TestCase3.Run_Test(&Branch);  
  sbuf->clear();

  /*************************** Case 4 ********************************/

  in.assign("cbr $c1, $s1;\n");
  reg1Name.assign("$c1");   reg2Name.assign("$s1");
  sbuf->append(reg1Name);   sbuf->append(reg2Name);

  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigCbr;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = sizeof(width) + sizeof(reg1);
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.s_name = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;
 
  Branch_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> 
  TestCase4(in, sbuf, &outInst, &width, &reg1, &reg2);
  TestCase4.Run_Test(&Branch);  
  sbuf->clear();


  /***************************  End of tests ***********************/
  delete sbuf;
}
TEST(ErrorReportTest, Branch) {  
  std::string input = "brn_width(all)_fbar @label\n";
  Branch_Test<> TestCase1(input);
  TestCase1.Run_Test(&Branch, MISSING_SEMICOLON);

  input.assign("cbr $c1;\n");
  Branch_Test<> TestCase2(input);
  TestCase2.Run_Test(&Branch, MISSING_COMMA);
}  
 
}  // namespace hsa 
}  // namespace brig 

