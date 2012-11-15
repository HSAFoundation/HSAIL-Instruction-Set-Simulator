#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example6_Test: public BrigCodeGenTest {

private:
  Buffer* RefDir;
  Buffer* RefOper;
  Buffer* RefCode;

public:
  Example6_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Example6_CodeGen) {

  std::string input;

  input.append("version 1:0:$small;\n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input)\n");
  input.append("{\n");
  input.append("  ld_arg_f32 $s0, [%input];\n");
  input.append("  st_arg_f32 $s0, [%output];\n");
  input.append("  ret;\n");
  input.append("};\n");
  input.append("function &caller()()\n");
  input.append("{\n");
  input.append("  {\n");
  input.append("    arg_f32 %an_input;\n");
  input.append("    st_arg_f32 $s1, [%an_input];\n");
  input.append("    arg_f32 %an_output;\n");
  input.append("    call &callee (%an_output)(%an_input);\n");
  input.append("    ld_arg_f32 $s0, [%an_output];\n");
  input.append("  }\n");
  input.append("};\n");

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveVersion verDir;
  BrigDirectiveFunction fun1Dir, fun2Dir;
  BrigDirectiveSymbol symInDir, symOutDir, sym1Dir, sym2Dir;
  BrigDirectiveScope startScopeDir, endScopeDir;

  BrigOperandReg reg1Oper, reg2Oper;
  BrigOperandAddress addrOper;
  BrigOperandImmed width;
  BrigOperandFunctionRef fun1Oper;

  BrigOperandArgumentRef argOutRefOper, argInRefOper;
  BrigOperandArgumentList argOutListOper, argInListOper;

  BrigInstLdSt ldCode, stCode;
  BrigInstBase callCode, retCode;

  std::string fun1Name,fun2Name;
  std::string argInName, argOutName;
  std::string sym1Name, sym2Name;
  std::string reg1Name, reg2Name;
  /**************** String Section ****************/
  fun1Name.assign("&callee");
  argOutName.assign("%output");
  argInName.assign("%input");
  reg1Name.assign("$s0");
  fun2Name.assign("&caller");
  sym1Name.assign("%an_input");
  reg2Name.assign("$s1");
  sym2Name.assign("%an_output");

  symbols->append(fun1Name);
  symbols->append(argOutName);
  symbols->append(argInName); 
  symbols->append(reg1Name);
  symbols->append(fun2Name);
  symbols->append(sym1Name);
  symbols->append(reg2Name);
  symbols->append(sym2Name);
    
  /********************** Operand section *********************/
  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;
  
  oper->append(&width);
 
  reg1Oper.size = sizeof(reg1Oper);
  reg1Oper.kind = BrigEOperandReg;
  reg1Oper.type = Brigb32;
  reg1Oper.reserved = 0;
  reg1Oper.s_name = fun1Name.size() + argOutName.size() + 
                    argInName.size() + 3;

  oper->append(&reg1Oper);

  addrOper.size = sizeof(addrOper);
  addrOper.kind = BrigEOperandAddress;
  addrOper.type = Brigb32;
  addrOper.reserved = 0;
  addrOper.directive = sizeof(fun1Dir) + sizeof(verDir) + sizeof(symOutDir);

  oper->append(&addrOper);

  addrOper.size = sizeof(addrOper);
  addrOper.kind = BrigEOperandAddress;
  addrOper.type = Brigb32;
  addrOper.reserved = 0;
  addrOper.directive = sizeof(fun1Dir) + sizeof(verDir);

  oper->append(&addrOper);

  reg2Oper.size = sizeof(reg2Oper);
  reg2Oper.kind = BrigEOperandReg;
  reg2Oper.type = Brigb32;
  reg2Oper.reserved = 0;
  reg2Oper.s_name = fun1Name.size() + argOutName.size() + fun2Name.size() +
                    argInName.size() + sym1Name.size() + reg1Name.size() + 6;

  oper->append(&reg2Oper);

  addrOper.size = sizeof(addrOper);
  addrOper.kind = BrigEOperandAddress;
  addrOper.type = Brigb32;
  addrOper.reserved = 0;
  addrOper.directive = sizeof(fun1Dir) + sizeof(fun2Dir) + sizeof(verDir) + 
                       sizeof(symOutDir) + sizeof(symInDir) + sizeof(startScopeDir);

  oper->append(&addrOper);
  
  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;
  
  oper->append(&width);

  fun1Oper.size = sizeof(fun1Oper);
  fun1Oper.kind = BrigEOperandFunctionRef;
  fun1Oper.fn = sizeof(verDir);

  oper->append(&fun1Oper);
  
  argOutRefOper.size = sizeof(argOutRefOper);
  argOutRefOper.kind = BrigEOperandArgumentRef;
  argOutRefOper.arg = sizeof(verDir) + sizeof(fun1Dir) + sizeof(symOutDir) +
                      sizeof(symInDir) + sizeof(fun2Dir) + sizeof(startScopeDir) +
                      sizeof(sym1Dir);

  oper->append(&argOutRefOper);

  argOutListOper.size = sizeof(argOutListOper);
  argOutListOper.kind = BrigEOperandArgumentList;
  argOutListOper.elementCount = 1;
  argOutListOper.o_args[0] = sizeof(addrOper) * 3 + sizeof(reg1Oper) + sizeof(reg2Oper) +
                             sizeof(fun1Oper) + sizeof(width) * 2;

  oper->append(&argOutListOper);

  argInRefOper.size = sizeof(argInRefOper);
  argInRefOper.kind = BrigEOperandArgumentRef;
  argInRefOper.arg = sizeof(verDir) + sizeof(fun1Dir) + sizeof(symOutDir) +
                     sizeof(symInDir) + sizeof(fun2Dir) + sizeof(startScopeDir);

  oper->append(&argInRefOper);

  argInListOper.size = sizeof(argInListOper);
  argInListOper.kind = BrigEOperandArgumentList;
  argInListOper.elementCount = 1;
  argInListOper.o_args[0] = sizeof(addrOper) * 3 + sizeof(reg1Oper) + sizeof(reg2Oper) +
                            sizeof(fun1Oper) + sizeof(width) * 2 + sizeof(argOutRefOper) +
                            sizeof(argOutListOper);

  oper->append(&argInListOper);

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  oper->append(&width);

  addrOper.size = sizeof(addrOper);
  addrOper.kind = BrigEOperandAddress;
  addrOper.type = Brigb32;
  addrOper.reserved = 0;
  addrOper.directive = sizeof(verDir) + sizeof(fun1Dir) + sizeof(symOutDir) +
                       sizeof(symInDir) + sizeof(fun2Dir) + sizeof(startScopeDir) +
                       sizeof(sym1Dir);

  oper->append(&addrOper);

  /************************ Code section *********************/

  ldCode.size = sizeof(ldCode);
  ldCode.kind = BrigEInstLdSt;
  ldCode.opcode = BrigLd;
  ldCode.type = Brigf32;
  ldCode.packing = BrigNoPacking;
  ldCode.o_operands[0] = 0;
  ldCode.o_operands[1] = sizeof(width);
  ldCode.o_operands[2] = sizeof(reg1Oper) + sizeof(width);
  ldCode.o_operands[3] = 0;
  ldCode.o_operands[4] = 0;
  ldCode.storageClass = BrigArgSpace;
  ldCode.memorySemantic = BrigRegular;
  ldCode.equivClass = 0;

  code->append(&ldCode);

  stCode.size = sizeof(stCode);
  stCode.kind = BrigEInstLdSt;
  stCode.opcode = BrigSt;
  stCode.type = Brigf32;
  stCode.packing = BrigNoPacking;
  stCode.o_operands[0] = sizeof(width);
  stCode.o_operands[1] = sizeof(reg1Oper) + sizeof(width) + sizeof(addrOper);
  stCode.o_operands[2] = 0;
  stCode.o_operands[3] = 0;
  stCode.o_operands[4] = 0;
  stCode.storageClass = BrigArgSpace;
  stCode.memorySemantic = BrigRegular;
  stCode.equivClass = 0;

  code->append(&stCode);

  retCode.size = sizeof(retCode);
  retCode.kind = BrigEInstBase;
  retCode.opcode = BrigRet;
  retCode.type = Brigb32;
  retCode.packing = BrigNoPacking;
  retCode.o_operands[0] = 0;
  retCode.o_operands[1] = 0;
  retCode.o_operands[2] = 0;
  retCode.o_operands[3] = 0;
  retCode.o_operands[4] = 0;

  code->append(&retCode);

  stCode.size = sizeof(stCode);
  stCode.kind = BrigEInstLdSt;
  stCode.opcode = BrigSt;
  stCode.type = Brigf32;
  stCode.packing = BrigNoPacking;
  stCode.o_operands[0] = sizeof(reg1Oper) + sizeof(width) + sizeof(addrOper) * 2;
  stCode.o_operands[1] = sizeof(reg1Oper) + sizeof(reg2Oper) + sizeof(width) + sizeof(addrOper) * 2;
  stCode.o_operands[2] = 0;
  stCode.o_operands[3] = 0;
  stCode.o_operands[4] = 0;
  stCode.storageClass = BrigArgSpace;
  stCode.memorySemantic = BrigRegular;
  stCode.equivClass = 0;

  code->append(&stCode);
  
  callCode.size = sizeof(callCode);
  callCode.kind = BrigEInstBase;
  callCode.opcode = BrigCall;
  callCode.type = Brigb32;
  callCode.packing = BrigNoPacking;
  callCode.o_operands[0] = stCode.o_operands[1] + sizeof(addrOper);
  callCode.o_operands[1] = callCode.o_operands[0] + sizeof(width) + 
                           sizeof(fun1Oper) + sizeof(argOutRefOper);
  callCode.o_operands[2] = callCode.o_operands[0] + sizeof(width);
  callCode.o_operands[3] = callCode.o_operands[1] + sizeof(argOutListOper) + sizeof(argInRefOper);
  callCode.o_operands[4] = 0;

  code->append(&callCode);

  ldCode.size = sizeof(ldCode);
  ldCode.kind = BrigEInstLdSt;
  ldCode.opcode = BrigLd;
  ldCode.type = Brigf32;
  ldCode.packing = BrigNoPacking;
  ldCode.o_operands[0] = callCode.o_operands[3] + sizeof(argInListOper);
  ldCode.o_operands[1] = sizeof(width);
  ldCode.o_operands[2] = ldCode.o_operands[0] + sizeof(width);
  ldCode.o_operands[3] = 0;
  ldCode.o_operands[4] = 0;
  ldCode.storageClass = BrigArgSpace;
  ldCode.memorySemantic = BrigRegular;
  ldCode.equivClass = 0;

  code->append(&ldCode);

  /************************ Directive Section ************************/

  verDir.size = sizeof(verDir);
  verDir.kind = BrigEDirectiveVersion;  
  verDir.c_code = 0;
  verDir.major = 1;
  verDir.minor = 0;
  verDir.machine = BrigESmall;
  verDir.profile = BrigEFull;
  verDir.ftz = BrigENosftz;
  verDir.reserved = 0;

  dir->append(&verDir);

  fun1Dir.size = sizeof(fun1Dir);
  fun1Dir.kind = BrigEDirectiveFunction;
  fun1Dir.c_code = 0;
  fun1Dir.s_name = 0;
  fun1Dir.inParamCount = 1;
  fun1Dir.d_firstScopedDirective = sizeof(verDir) + sizeof(fun1Dir) + 
                                   sizeof(symInDir) + sizeof(symOutDir);
  fun1Dir.operationCount = 3;
  fun1Dir.d_nextDirective = fun1Dir.d_firstScopedDirective;
  fun1Dir.attribute = BrigNone;
  fun1Dir.reserved = 0;
  fun1Dir.outParamCount = 1;
  fun1Dir.d_firstInParam = 0;

  dir->append(&fun1Dir);

  symOutDir.size = sizeof(symOutDir);
  symOutDir.kind = BrigEDirectiveSymbol;
  symOutDir.s.c_code = 0;
  symOutDir.s.storageClass = BrigArgSpace;
  symOutDir.s.attribute = BrigNone;
  symOutDir.s.reserved = 0;
  symOutDir.s.symbolModifier = 0;
  symOutDir.s.dim = 0;
  symOutDir.s.s_name = fun1Name.size() + 1;
  symOutDir.s.type = Brigf32;
  symOutDir.s.align = 1;
  symOutDir.d_init = 0;
  symOutDir.reserved = 0;

  dir->append(&symOutDir);

  symInDir.size = sizeof(symInDir);
  symInDir.kind = BrigEDirectiveSymbol;
  symInDir.s.c_code = 0;
  symInDir.s.storageClass = BrigArgSpace;
  symInDir.s.attribute = BrigNone;
  symInDir.s.reserved = 0;
  symInDir.s.symbolModifier = 0;
  symInDir.s.dim = 0;
  symInDir.s.s_name = fun1Name.size() + argOutName.size() + 2;
  symInDir.s.type = Brigf32;
  symInDir.s.align = 1;
  symInDir.d_init = 0;
  symInDir.reserved = 0;

  dir->append(&symInDir);

  fun2Dir.size = sizeof(fun2Dir);
  fun2Dir.kind = BrigEDirectiveFunction;
  fun2Dir.c_code = sizeof(retCode) + sizeof(stCode) + sizeof(ldCode);
  fun2Dir.s_name = fun1Name.size() + reg1Name.size() + argOutName.size() + argInName.size() + 4;
  fun2Dir.inParamCount = 0;
  fun2Dir.d_firstScopedDirective = sizeof(verDir) + sizeof(fun1Dir) + 
                                   sizeof(fun2Dir) + sizeof(symInDir) + sizeof(symOutDir);
  fun2Dir.operationCount = 3;
  fun2Dir.d_nextDirective = fun2Dir.d_firstScopedDirective + sizeof(startScopeDir) + sizeof(endScopeDir) + 
                            sizeof(sym1Dir) + sizeof(sym2Dir);
  fun2Dir.attribute = BrigNone;
  fun2Dir.reserved = 0;
  fun2Dir.outParamCount = 0;
  fun2Dir.d_firstInParam = 0;

  dir->append(&fun2Dir);

  startScopeDir.size = sizeof(startScopeDir);
  startScopeDir.kind = BrigEDirectiveArgStart;
  startScopeDir.c_code = fun2Dir.c_code;
  
  dir->append(&startScopeDir);

  sym1Dir.size = sizeof(sym1Dir);
  sym1Dir.kind = BrigEDirectiveSymbol;
  sym1Dir.s.c_code = fun2Dir.c_code;
  sym1Dir.s.storageClass = BrigArgSpace;
  sym1Dir.s.attribute = BrigNone;
  sym1Dir.s.reserved = 0;
  sym1Dir.s.symbolModifier = 0;
  sym1Dir.s.dim = 0;
  sym1Dir.s.s_name = reg1Name.size() + argOutName.size() +
                     argInName.size() + fun1Name.size() + fun2Name.size() + 5;
  sym1Dir.s.type = Brigf32;
  sym1Dir.s.align = 1;
  sym1Dir.d_init = 0;
  sym1Dir.reserved = 0;

  dir->append(&sym1Dir);

  sym2Dir.size = sizeof(sym2Dir);
  sym2Dir.kind = BrigEDirectiveSymbol;
  sym2Dir.s.c_code = fun2Dir.c_code + sizeof(stCode);
  sym2Dir.s.storageClass = BrigArgSpace;
  sym2Dir.s.attribute = BrigNone;
  sym2Dir.s.reserved = 0;
  sym2Dir.s.symbolModifier = 0;
  sym2Dir.s.dim = 0;
  sym2Dir.s.s_name = reg1Name.size() + argOutName.size() +
                     reg2Name.size() + argInName.size() + fun1Name.size() +
                     sym1Name.size() + fun2Name.size() + 7;
  sym2Dir.s.type = Brigf32;
  sym2Dir.s.align = 1;
  sym2Dir.d_init = 0;
  sym2Dir.reserved = 0;

  dir->append(&sym2Dir);

  endScopeDir.size = sizeof(startScopeDir);
  endScopeDir.kind = BrigEDirectiveArgEnd;
  endScopeDir.c_code = fun2Dir.c_code + sizeof(stCode) + 
                       sizeof(callCode) + sizeof(ldCode);

  dir->append(&endScopeDir);

  Example6_Test TestCase(input, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig

