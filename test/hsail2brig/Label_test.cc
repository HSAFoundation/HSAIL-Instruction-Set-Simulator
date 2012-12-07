#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Label_Test: public BrigCodeGenTest {

private:
  Buffer* RefDir;
  Buffer* RefOper;
  Buffer* RefCode; 

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

  BrigOperandLabelRef lab1Oper, lab2Oper, lab3Oper;
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
  instCbr.o_operands[2] = sizeof(width1);
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
  lab1Dir.s_name = funName.size() + 1;

  dir->append(&lab1Dir);

  lab2Dir.size = sizeof(lab2Dir);
  lab2Dir.kind = BrigEDirectiveLabel;
  lab2Dir.c_code = sizeof(instBrn) + sizeof(instCbr);
  lab2Dir.s_name = funName.size() + regName.size() + lab1Name.size() + 3;
  
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

  Label_Test TestCase1(in, symbols, dir, code, oper);
  TestCase1.Run_Test(&Program);
  symbols->clear();
  dir->clear();
  code->clear();
  oper->clear();

  in.assign("version 1:0:$small;\n");
  in.append("kernel &LabelTest()\n");
  in.append("{\n");
  in.append("  @t1:  add_u32 $s2, $s2, 2;\n");
  in.append("  @t2:  add_u32 $s2, $s2, 1;\n");
  in.append("  global_u32 %tab1[] =  {@t1, @t2};\n");
  in.append("  @t3:  add_u32 $s2, $s2, 3;\n");
  in.append("  @targets: labeltargets @t2, @t3, @t4, @t6;\n");
  in.append("  global_u32 %tab2[3] =  {@t3, @t4, @t5};\n");
  in.append("  @t4:  add_u32 $s2, $s2, 4;\n");
  in.append("  @t5:  add_u32 $s2, $s2, 5;\n");
  in.append("  @t6:  ret;\n");
  in.append("};\n");

  std::string t1Name, t2Name, t3Name, t4Name, t5Name, t6Name;
  std::string tab1Name, tab2Name, kerName, tarName;
  BrigInstBase instAdd, instRet;
  BrigOperandImmed imm;

  kerName.assign("&LabelTest");
  t1Name.assign("@t1");
  regName.assign("$s2");
  t2Name.assign("@t2");
  tab1Name.assign("%tab1");
  t3Name.assign("@t3");
  tarName.assign("@targets");
  tab2Name.assign("%tab2");
  t4Name.assign("@t4");
  t5Name.assign("@t5");
  t6Name.assign("@t6");
  
  symbols->append(kerName);
  symbols->append(t1Name);
  symbols->append(regName);
  symbols->append(t2Name);
  symbols->append(tab1Name);
  symbols->append(t3Name);
  symbols->append(tarName);
  symbols->append(tab2Name);
  symbols->append(t4Name);
  symbols->append(t5Name);
  symbols->append(t6Name);

  instAdd.size = sizeof(instAdd);
  instAdd.kind = BrigEInstBase;
  instAdd.opcode = BrigAdd;
  instAdd.type = Brigu32;
  instAdd.packing = BrigNoPacking;
  instAdd.o_operands[0] = 0;
  instAdd.o_operands[1] = 0;
  instAdd.o_operands[2] = sizeof(reg);
  instAdd.o_operands[3] = 0;
  instAdd.o_operands[4] = 0;

  code->append(&instAdd);

  instAdd.size = sizeof(instAdd);
  instAdd.kind = BrigEInstBase;
  instAdd.opcode = BrigAdd;
  instAdd.type = Brigu32;
  instAdd.packing = BrigNoPacking;
  instAdd.o_operands[0] = 0;
  instAdd.o_operands[1] = 0;
  instAdd.o_operands[2] = sizeof(reg) + sizeof(imm);
  instAdd.o_operands[3] = 0;
  instAdd.o_operands[4] = 0;

  code->append(&instAdd);

  instAdd.size = sizeof(instAdd);
  instAdd.kind = BrigEInstBase;
  instAdd.opcode = BrigAdd;
  instAdd.type = Brigu32;
  instAdd.packing = BrigNoPacking;
  instAdd.o_operands[0] = 0;
  instAdd.o_operands[1] = 0;
  instAdd.o_operands[2] = sizeof(reg) + sizeof(imm) * 2;
  instAdd.o_operands[3] = 0;
  instAdd.o_operands[4] = 0;

  code->append(&instAdd);

  instAdd.size = sizeof(instAdd);
  instAdd.kind = BrigEInstBase;
  instAdd.opcode = BrigAdd;
  instAdd.type = Brigu32;
  instAdd.packing = BrigNoPacking;
  instAdd.o_operands[0] = 0;
  instAdd.o_operands[1] = 0;
  instAdd.o_operands[2] = sizeof(reg) + sizeof(imm) * 3;
  instAdd.o_operands[3] = 0;
  instAdd.o_operands[4] = 0;

  code->append(&instAdd);

  instAdd.size = sizeof(instAdd);
  instAdd.kind = BrigEInstBase;
  instAdd.opcode = BrigAdd;
  instAdd.type = Brigu32;
  instAdd.packing = BrigNoPacking;
  instAdd.o_operands[0] = 0;
  instAdd.o_operands[1] = 0;
  instAdd.o_operands[2] = sizeof(reg) + sizeof(imm) * 4;
  instAdd.o_operands[3] = 0;
  instAdd.o_operands[4] = 0;

  code->append(&instAdd);

  instRet.size = sizeof(instRet);
  instRet.kind = BrigEInstBase;
  instRet.opcode = BrigRet;
  instRet.type = Brigb32;
  instRet.packing = BrigNoPacking;
  instRet.o_operands[0] = 0;
  instRet.o_operands[1] = 0;
  instRet.o_operands[2] = 0;
  instRet.o_operands[3] = 0;
  instRet.o_operands[4] = 0;

  code->append(&instRet);

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = kerName.size() + t1Name.size() + 2;

  oper->append(&reg);

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 2;

  oper->append(&imm);

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 1;

  oper->append(&imm);

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 3;

  oper->append(&imm);

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 4;

  oper->append(&imm);

  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  memset(&imm.bits, 0, sizeof(imm.bits));
  imm.bits.u = 5;

  oper->append(&imm);

  BrigDirectiveKernel kerRef;
  BrigDirectiveLabel t1, t2, t3, t4, t5, t6, tar;
  BrigDirectiveSymbol tab1, tab2;
  BrigDirectiveLabelInit* tab1Init, *tab2Init;
  BrigDirectiveLabelList* tabList;

  tab1Init = (BrigDirectiveLabelInit*)malloc(sizeof(BrigDirectiveLabelInit) +
                                             sizeof(BrigdOffset32_t));

  tab2Init = (BrigDirectiveLabelInit*)malloc(sizeof(BrigDirectiveLabelInit) +
                                             sizeof(BrigdOffset32_t) * 2);

  tabList = (BrigDirectiveLabelList*)malloc(sizeof(BrigDirectiveLabelList) +
                                            sizeof(BrigdOffset32_t) * 3);
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

  kerRef.size = sizeof(kerRef);
  kerRef.kind = BrigEDirectiveKernel;
  kerRef.c_code = 0;
  kerRef.s_name = 0;
  kerRef.inParamCount = 0;
  kerRef.d_firstScopedDirective = sizeof(verRef) + sizeof(kerRef);  
  kerRef.operationCount = 6;
  kerRef.d_nextDirective = kerRef.d_firstScopedDirective + sizeof(tar) + sizeof(t1) + 
                           sizeof(t2) + sizeof(t3) + sizeof(t4) + sizeof(t5) + sizeof(t6) +
                           sizeof(tab1) + sizeof(tab2) + sizeof(BrigdOffset32_t) * 6 + 
                           sizeof(BrigDirectiveLabelInit) * 2 + sizeof(BrigDirectiveLabelList);

  kerRef.attribute = BrigNone;
  kerRef.reserved = 0;
  kerRef.outParamCount = 0;
  kerRef.d_firstInParam = 0;

  dir->append(&kerRef);

  t1.size = sizeof(t1);
  t1.kind = BrigEDirectiveLabel;
  t1.c_code = 0;
  t1.s_name = kerName.size() + 1;

  dir->append(&t1);

  t2.size = sizeof(t2);
  t2.kind = BrigEDirectiveLabel;
  t2.c_code = sizeof(instAdd);
  t2.s_name = kerName.size() + t1Name.size() + regName.size() + 3;

  dir->append(&t2);
  
  tab1.size = sizeof(tab1);
  tab1.kind = BrigEDirectiveSymbol;
  tab1.s.c_code = sizeof(instAdd) * 2;
  tab1.s.storageClass = BrigGlobalSpace;
  tab1.s.attribute = BrigNone;
  tab1.s.reserved = 0;
  tab1.s.symbolModifier = BrigArray;
  tab1.s.dim = 2;
  tab1.s.s_name = t1Name.size() + regName.size() + t2Name.size() + kerName.size() + 4;
  tab1.s.type = Brigu32;
  tab1.s.align = 1;
  tab1.d_init = sizeof(verRef) + sizeof(kerRef) + sizeof(t1) + 
                sizeof(tab1) + sizeof(t2);
  tab1.reserved = 0;

  dir->append(&tab1);

  tab1Init->size = sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t);
  tab1Init->kind = BrigEDirectiveLabelInit;
  tab1Init->c_code = sizeof(instAdd) * 2;
  tab1Init->elementCount = 2;
  tab1Init->s_name = tab1.s.s_name;
  tab1Init->d_labels[0] = sizeof(verRef) + sizeof(kerRef);
  tab1Init->d_labels[1] = sizeof(verRef) + sizeof(kerRef) + sizeof(t1);

  dir->append(tab1Init);

  t3.size = sizeof(t3);
  t3.kind = BrigEDirectiveLabel;
  t3.c_code = sizeof(instAdd) * 2;
  t3.s_name = kerName.size() + regName.size() + t1Name.size() + 
              t2Name.size() + tab1Name.size() + 5;

  dir->append(&t3);

  tar.size = sizeof(tar);
  tar.kind = BrigEDirectiveLabel;
  tar.c_code = sizeof(instAdd) * 3;
  tar.s_name = kerName.size() + regName.size() + t1Name.size() + 
               t2Name.size() + tab1Name.size() + t3Name.size() + 6;

  dir->append(&tar);

  t4.size = sizeof(t4);
  t4.kind = BrigEDirectiveLabel;
  t4.c_code = sizeof(instAdd) * 3;
  t4.s_name = kerName.size() + t1Name.size() + t2Name.size() + regName.size() +
              t3Name.size() + tab1Name.size() + tab2Name.size() + tarName.size() + 8;

  dir->append(&t4);

  t6.size = sizeof(t6);
  t6.kind = BrigEDirectiveLabel;
  t6.c_code = sizeof(instAdd) * 5;
  t6.s_name = kerName.size() + t1Name.size() + t2Name.size() + regName.size() +
              t3Name.size() + t4Name.size() + t5Name.size() + 
	          tab1Name.size() + tab2Name.size() + tarName.size() + 10;

  dir->append(&t6);

  tabList->size = sizeof(BrigDirectiveLabelList) + sizeof(BrigdOffset32_t) * 3;
  tabList->kind = BrigEDirectiveLabelList;
  tabList->c_code = sizeof(instAdd) * 3;
  tabList->elementCount = 4;
  tabList->label = sizeof(verRef) + sizeof(kerRef) + sizeof(t1) + sizeof(t2) + 
                   sizeof(tab1) + sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t) + 
                   sizeof(t3);
  tabList->d_labels[0] = sizeof(verRef) + sizeof(kerRef) + sizeof(t1);
  tabList->d_labels[1] = sizeof(verRef) + sizeof(kerRef) + sizeof(t1) + sizeof(t2) + 
                         sizeof(tab1) + sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t); 
  tabList->d_labels[2] = tabList->label + sizeof(tar);
  tabList->d_labels[3] = tabList->d_labels[2] + sizeof(t4);

  dir->append(tabList);

  tab2.size = sizeof(tab2);
  tab2.kind = BrigEDirectiveSymbol;
  tab2.s.c_code = sizeof(instAdd) * 3;
  tab2.s.storageClass = BrigGlobalSpace;
  tab2.s.attribute = BrigNone;
  tab2.s.reserved = 0;
  tab2.s.symbolModifier = BrigArray;
  tab2.s.dim = 3;
  tab2.s.s_name = t1Name.size() + t2Name.size() + kerName.size() + tarName.size() +  
                  tab1Name.size() + t3Name.size() + regName.size() + 7;
  tab2.s.type = Brigu32;
  tab2.s.align = 1;
  tab2.d_init =  tabList->d_labels[3] + sizeof(t6) + sizeof(BrigDirectiveLabelList) +
                 sizeof(BrigdOffset32_t) * 3 + sizeof(tab2) + sizeof(t5);
  tab2.reserved = 0;

  dir->append(&tab2);

  t5.size = sizeof(t5);
  t5.kind = BrigEDirectiveLabel;
  t5.c_code = sizeof(instAdd) * 4;
  t5.s_name = kerName.size() + t1Name.size() + t2Name.size() + regName.size() + tarName.size() +
              tab1Name.size() + t3Name.size() + t4Name.size() + tab2Name.size() + 9;

  dir->append(&t5);

  tab2Init->size = sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t) * 2;
  tab2Init->kind = BrigEDirectiveLabelInit;
  tab2Init->c_code = sizeof(instAdd) * 3;
  tab2Init->elementCount = 3;
  tab2Init->s_name = tab2.s.s_name;
  tab2Init->d_labels[0] = tab1.d_init + sizeof(BrigDirectiveLabelInit) + 
                          sizeof(BrigdOffset32_t);
  tab2Init->d_labels[1] = tabList->label + sizeof(tar);
  tab2Init->d_labels[2] = tab2.d_init - sizeof(t5);

  dir->append(tab2Init);

  Label_Test TestCase2(in, symbols, dir, code, oper);
  TestCase2.Run_Test(&Program);
  symbols->clear();
  dir->clear();
  code->clear();
  oper->clear();

  free(tab1Init);
  free(tab2Init);
  delete dir;
  delete code;
  delete oper;
  delete symbols;
}

  

} // namespace hsa
} // namespace brig




