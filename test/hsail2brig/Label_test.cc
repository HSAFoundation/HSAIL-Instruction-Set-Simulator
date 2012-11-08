#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Label_Test: public BrigCodeGenTest {

private:
  Buffer* RefCode;
  Buffer* RefOper;
  Buffer* RefDir;

public:
  Label_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Label_CodeGen) {

  std::string in;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveLabel lab1Dir, lab2Dir, lab3Dir, lab4Dir;
  BrigDirectiveVersion verRef;
  BrigDirectiveFunction funRef;

  BrigInstBase instCbr, instBrn;

  BrigOperandLabelRef lab1Oper, lab2Oper, lab3Oper, lab4Oper;
  BrigOperandImmed width1, width2;
  BrigOperandReg reg;

  std::string lab1Name, lab2Name, lab3Name, lab4Name;
  std::string regName, funName;

  in.append("version 1:0:$small; \n");
  in.append("function &Label()(){ \n");
  in.append("brn @lab1;\n");  // brn lab1
  in.append("@lab1:\n");
  in.append("cbr $c1, @lab3;\n");  // cbr lab3
  in.append("@lab2:\n");
  in.append("@lab3:\n");
  in.append("brn @lab2;\n");       // brn lab2
  in.append("@lab4:\n");
  in.append("cbr $c1, @lab1;\n");  // cbr lab1
  in.append("};");

  funName.append("&Label");
  lab1Name.append("@lab1");
  regName.append("$c1");
  lab2Name.append("@lab2");
  lab3Name.append("@lab3");
  lab4Name.append("@lab4");

  symbols->append(funName);
  symbols->append(lab1Name);
  symbols->append(regName);
  symbols->append(lab2Name);
  symbols->append(lab3Name);
  symbols->append(lab4Name);

  instBrn.size = sizeof(instBrn);
  instBrn.kind = BrigEInstBase;
  instBrn.opcode = BrigBrn;
  instBrn.type = Brigb32;
  instBrn.packing = BrigNoPacking;
  instBrn.o_operands[0] = 0;
  instBrn.o_operands[1] = sizeof(width1);
  instBrn.o_operands[2] = 0;
  instBrn.o_operands[3] = 0;
  instBrn.o_operands[4] = 0;
  
  code->append(&instBrn);

  width1.size = sizeof(width1);
  width1.kind = BrigEOperandImmed;
  width1.type = Brigb32;
  width1.reserved = 0;
  memset(&width1.bits, 0, sizeof(width1.bits));
  width1.bits.u = 0;

  oper->append(&width1);

  lab1Oper.size = sizeof(lab1Oper);
  lab1Oper.kind = BrigEOperandLabelRef;
  lab1Oper.labeldirective = sizeof(funRef) + sizeof(verRef);

  oper->append(&lab1Oper);

  instCbr.size = sizeof(instCbr);
  instCbr.kind = BrigEInstBase;
  instCbr.opcode = BrigCbr;
  instCbr.type = Brigb32;
  instCbr.packing = BrigNoPacking;
  instCbr.o_operands[0] = sizeof(lab1Oper) + sizeof(width1);
  instCbr.o_operands[1] = sizeof(width1) + sizeof(lab1Oper) + sizeof(width2);
  instCbr.o_operands[2] = sizeof(reg) + instCbr.o_operands[1];
  instCbr.o_operands[3] = 0;
  instCbr.o_operands[4] = 0;
 
  code->append(&instCbr);

  width2.size = sizeof(width2);
  width2.kind = BrigEOperandImmed;
  width2.type = Brigb32;
  width2.reserved = 0;
  memset(&width2.bits, 0, sizeof(width2.bits));
  width2.bits.u = 0;

  oper->append(&width2);

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb1;
  reg.reserved = 0;
  reg.s_name = funName.size() + lab1Name.size() + 2;

  oper->append(&reg);

  lab3Oper.size = sizeof(lab3Oper);
  lab3Oper.kind = BrigEOperandLabelRef;
  lab3Oper.labeldirective = sizeof(funRef) + sizeof(verRef) +
                            sizeof(lab1Dir) + sizeof(lab2Dir);

  oper->append(&lab3Oper);

  instBrn.size = sizeof(instBrn);
  instBrn.kind = BrigEInstBase;
  instBrn.opcode = BrigBrn;
  instBrn.type = Brigb32;
  instBrn.packing = BrigNoPacking;
  instBrn.o_operands[0] = instCbr.o_operands[2] + sizeof(lab2Oper);
  instBrn.o_operands[1] = instBrn.o_operands[0] + sizeof(width1);
  instBrn.o_operands[2] = 0;
  instBrn.o_operands[3] = 0;
  instBrn.o_operands[4] = 0;
  
  code->append(&instBrn);

  width1.size = sizeof(width1);
  width1.kind = BrigEOperandImmed;
  width1.type = Brigb32;
  width1.reserved = 0;
  memset(&width1.bits, 0, sizeof(width1.bits));
  width1.bits.u = 0;

  oper->append(&width1);

  lab2Oper.size = sizeof(lab2Oper);
  lab2Oper.kind = BrigEOperandLabelRef;
  lab2Oper.labeldirective = sizeof(funRef) + sizeof(verRef) + sizeof(lab1Dir);

  oper->append(&lab2Oper);

  instCbr.size = sizeof(instCbr);
  instCbr.kind = BrigEInstBase;
  instCbr.opcode = BrigCbr;
  instCbr.type = Brigb32;
  instCbr.packing = BrigNoPacking;
  instCbr.o_operands[0] = instBrn.o_operands[1] + sizeof(lab2Oper);
  instCbr.o_operands[1] = sizeof(width1) + sizeof(lab1Oper) + sizeof(width2);
  instCbr.o_operands[2] = sizeof(reg) + instCbr.o_operands[1];
  instCbr.o_operands[3] = 0;
  instCbr.o_operands[4] = 0;
 
  code->append(&instCbr);

  width2.size = sizeof(width2);
  width2.kind = BrigEOperandImmed;
  width2.type = Brigb32;
  width2.reserved = 0;
  memset(&width2.bits, 0, sizeof(width2.bits));
  width2.bits.u = 0;

  oper->append(&width2);

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
  funRef.d_firstScopedDirective = sizeof(verRef) + sizeof(funRef);
  funRef.operationCount = 4;
  funRef.d_nextDirective = funRef.d_firstScopedDirective + sizeof(lab1Dir) +
                           sizeof(lab2Dir) + sizeof(lab3Dir) + sizeof(lab4Dir);
  funRef.attribute = BrigNone;
  funRef.reserved = 0;
  funRef.outParamCount = 0;
  funRef.d_firstInParam = 0;

  dir->append(&funRef);

  lab1Dir.size = sizeof(lab1Dir);
  lab1Dir.kind = BrigEDirectiveLabel;
  lab1Dir.c_code = sizeof(instBrn);
  lab1Dir.s_name = funName.size();

  dir->append(&lab1Dir);

  lab2Dir.size = sizeof(lab2Dir);
  lab2Dir.kind = BrigEDirectiveLabel;
  lab2Dir.c_code = sizeof(instBrn) + sizeof(instCbr);
  lab2Dir.s_name = funName.size() + regName.size() + lab1Name.size() + 2;
  
  dir->append(&lab2Dir);

  lab3Dir.size = sizeof(lab3Dir);
  lab3Dir.kind = BrigEDirectiveLabel;
  lab3Dir.c_code = sizeof(instBrn) + sizeof(instCbr);
  lab3Dir.s_name = funName.size() + regName.size() + 
                   lab1Name.size() + lab2Name.size() + 4;
  
  dir->append(&lab3Dir);

  lab4Dir.size = sizeof(lab4Dir);
  lab4Dir.kind = BrigEDirectiveLabel;
  lab4Dir.c_code = sizeof(instBrn) * 2 + sizeof(instCbr);
  lab4Dir.s_name = funName.size() + regName.size() + lab1Name.size() + 
                   lab2Name.size() + lab3Name.size() + 5;
  
  dir->append(&lab4Dir);

  Label_Test TestCase(in, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




