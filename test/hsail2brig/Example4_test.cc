#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example4_Test: public BrigCodeGenTest {

private:
  Buffer* RefDir;
  Buffer* RefOper;
  Buffer* RefCode;

public:
  Example4_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
                Buffer* code, Buffer* oper):
    BrigCodeGenTest(in, sbuf), RefDir(dir), RefOper(oper), RefCode(code) {}

  void Run_Test(int (*Rule)(Context*)){  

    struct BrigSections RefOutput (
      reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&RefDir->get()[0]),
      reinterpret_cast<const char *>(&RefCode->get()[0]), 
      reinterpret_cast<const char *>(&RefOper->get()[0]), NULL, 
      RefStr->size(), RefDir->size(), RefCode->size(), RefOper->size(), 0
    );    
    
    Parse_Validate(Rule, &RefOutput);
  }
};

TEST(CodegenTest, Example4_CodeGen) {

  std::string in;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveVersion verRef;
  BrigDirectiveFunction funRef;
  BrigDirectiveSymbol symRef;
  BrigDirectiveLabel dirLab1Ref, dirLab2Ref;

  BrigOperandReg reg1, reg2, reg3, reg4, reg5;
  BrigOperandImmed width1, width2;
  BrigOperandLabelRef lab1Ref, lab2Ref;

  std::string funName, symName;
  std::string reg1Name, reg2Name, reg3Name, reg4Name, reg5Name;
  std::string lab1Name, lab2Name;

  in.append("version 1:0:$small;\n");
  in.append("function &branch_ops (arg_u8x4 %x)() { \n");
  in.append("  cbr $c1, @then; \n");
  in.append("  abs_p_s8x4 $s1, $s2; \n");
  in.append("  brn @outof_IF; \n");
  in.append("  @then: \n");
  in.append("  add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  in.append("  @outof_IF: \n");
  in.append("  ret; \n");
  in.append("}; \n");

  funName.append("&branch_ops");
  symName.append("%x");
  reg1Name.append("$c1");   reg2Name.append("$s1");
  reg3Name.append("$s2");   lab1Name.append("@then");
  reg4Name.append("$s0");   reg5Name.append("$s3");
  lab2Name.append("@outof_IF");

  symbols->append(funName);
  symbols->append(symName);
  symbols->append(reg1Name);  symbols->append(reg2Name);
  symbols->append(reg3Name);  symbols->append(lab1Name);
  symbols->append(reg4Name);  symbols->append(reg5Name);
  symbols->append(lab2Name);

  BrigInstBase instCbr = {
    0,
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {0, sizeof(width1), sizeof(reg1) + sizeof(width1), 0, 0}
  };
  instCbr.size = sizeof(instCbr);
 
  code->append(&instCbr);

  width1.size = sizeof(width1);
  width1.kind = BrigEOperandImmed;
  width1.type = Brigb32;
  width1.reserved = 0;
  memset(&width1.bits, 0, sizeof(width1.bits));
  width1.bits.u = 0;

  oper->append(&width1);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb1;
  reg1.reserved = 0;
  reg1.s_name = funName.size() + symName.size() + 2;

  oper->append(&reg1);

  lab1Ref.size = sizeof(lab1Ref);
  lab1Ref.kind = BrigEOperandLabelRef;
  lab1Ref.labeldirective = sizeof(verRef) + sizeof(funRef) + sizeof(symRef);

  oper->append(&lab1Ref);

  BrigInstBase instAbs = {
    0,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackP,
    {sizeof(reg1) + sizeof(width1) + sizeof(lab1Ref), 
     sizeof(width1) + sizeof(lab1Ref) + sizeof(reg1) + sizeof(reg2),
     0, 0, 0}
  };

  instAbs.size = sizeof(instAbs);

  code->append(&instAbs);
 
  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = funName.size() + symName.size() + reg1Name.size() + 3;

  oper->append(&reg2);

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = funName.size() + symName.size() + 
                reg1Name.size() + reg2Name.size() + 4;
  
  oper->append(&reg3);

  BrigInstBase instBrn = {
    0,
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {sizeof(width1) + sizeof(lab1Ref) + sizeof(reg1) + 
     sizeof(reg2) + sizeof(reg3),
     sizeof(width1) + sizeof(lab1Ref) + sizeof(reg1) + 
     sizeof(reg2) + sizeof(reg3) + sizeof(width2), 
     0, 0, 0}
  };
  instBrn.size = sizeof(instBrn);
 
  code->append(&instBrn);

  width2.size = sizeof(width2);
  width2.kind = BrigEOperandImmed;
  width2.type = Brigb32;
  width2.reserved = 0;
  memset(&width2.bits, 0, sizeof(width2.bits));
  width2.bits.u = 0;

  oper->append(&width2);

  lab2Ref.size = sizeof(lab2Ref);
  lab2Ref.kind = BrigEOperandLabelRef;
  lab2Ref.labeldirective = sizeof(verRef) + sizeof(funRef) + sizeof(symRef) + sizeof(dirLab1Ref);

  oper->append(&lab2Ref);

  BrigInstBase instAdd = {
    0,
    BrigEInstBase,
    BrigAdd,
    Brigu16x2,
    BrigPackPPsat,
    {instAbs.o_operands[0], instBrn.o_operands[1] + sizeof(lab2Ref), 
     instBrn.o_operands[1] + sizeof(lab2Ref) + sizeof(reg4), 0, 0}
  };
  instAdd.size = sizeof(instAdd);
 
  code->append(&instAdd);

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = funName.size() + symName.size() + 
                reg1Name.size() + reg2Name.size() + 
                reg3Name.size() + lab1Name.size() + 6;

  oper->append(&reg4);

  reg5.size = sizeof(reg5);
  reg5.kind = BrigEOperandReg;
  reg5.type = Brigb32;
  reg5.reserved = 0;
  reg5.s_name = reg4.s_name + reg4Name.size() + 1;
  
  oper->append(&reg5);

  BrigInstBase instRet = {
    0,                  // size
    BrigEInstBase,      // kind
    BrigRet,            // opcode
    Brigb32,            // type
    BrigNoPacking,      // packing
    {0, 0, 0, 0, 0}     // o_operands[5]
  };
  instRet.size = sizeof(instRet);
  
  code->append(&instRet);

  verRef.size = sizeof(verRef);
  verRef.kind = BrigEDirectiveVersion;
  verRef.c_code = 0;
  verRef.major = 1;
  verRef.minor = 0;
  verRef.machine = BrigESmall;
  verRef.profile = BrigEFull;
  verRef.ftz = BrigENosftz;
  verRef.reserved = 0;

  dir->append(&verRef);

  funRef.size = sizeof(funRef);
  funRef.kind = BrigEDirectiveFunction;
  funRef.c_code = 0;
  funRef.s_name = 0;
  funRef.inParamCount = 0;
  funRef.d_firstScopedDirective = sizeof(verRef) + sizeof(funRef) +
                                  sizeof(symRef);
  funRef.operationCount = 5;
  funRef.d_nextDirective = funRef.d_firstScopedDirective;
  funRef.attribute = BrigNone;
  funRef.reserved = 0;
  funRef.outParamCount = 1;
  funRef.d_firstInParam = 0;

  dir->append(&funRef);

  symRef.size = sizeof(symRef);
  symRef.kind = BrigEDirectiveSymbol;
  symRef.s.c_code = 0;
  symRef.s.storageClass = BrigArgSpace;
  symRef.s.attribute = BrigNone;
  symRef.s.reserved = 0;
  symRef.s.symbolModifier = 0;
  symRef.s.dim = 0;
  symRef.s.s_name = funName.size() + 1;
  symRef.s.type = Brigu8x4;
  symRef.s.align = 1;
  symRef.d_init = 0;
  symRef.reserved = 0;

  dir->append(&symRef);

  dirLab1Ref.size = sizeof(dirLab1Ref);
  dirLab1Ref.kind = BrigEDirectiveLabel;
  dirLab1Ref.c_code = sizeof(instCbr) + sizeof(instAbs) + sizeof(instBrn);
  dirLab1Ref.s_name = funName.size() + symName.size() + reg1Name.size() +
                      reg2Name.size() + reg3Name.size() + 5;

  dir->append(&dirLab1Ref);

  dirLab2Ref.size = sizeof(dirLab2Ref);
  dirLab2Ref.kind = BrigEDirectiveLabel;
  dirLab2Ref.c_code = sizeof(instCbr) + sizeof(instAbs) + sizeof(instBrn) + sizeof(instAdd),
  dirLab2Ref.s_name = funName.size() + symName.size() + reg1Name.size() +
                      reg2Name.size() + reg3Name.size() + reg4Name.size() + 
                      reg5Name.size() + 7;

  dir->append(&dirLab2Ref);

  Example4_Test TestCase(in, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




