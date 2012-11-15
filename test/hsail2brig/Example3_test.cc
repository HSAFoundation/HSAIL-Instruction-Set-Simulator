#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example3_Test: public BrigCodeGenTest {

private:
  Buffer* RefDir;
  Buffer* RefOper;
  Buffer* RefCode;
  
public:
  Example3_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Example3_CodeGen) {

  std::string in;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveVersion verRef;
  BrigDirectiveFunction funRef;
  BrigDirectiveSymbol symRef;

  BrigOperandReg reg1, reg2, reg3, reg4;

  std::string funName, symName;
  std::string reg1Name, reg2Name, reg3Name, reg4Name;

  in.append("version 1:0:$small; \n");
  in.append("static function &packed_ops (extern arg_u8x4 %x)() {");
  in.append("  abs_p_s8x4 $s1, $s2; \n");
  in.append("  add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  in.append("}; \n");

  funName.append("&packed_ops");
  symName.append("%x");
  reg1Name.append("$s1"); reg2Name.append("$s2");
  reg3Name.append("$s0"); reg4Name.append("$s3");

  symbols->append(funName);
  symbols->append(symName);
  symbols->append(reg1Name);  symbols->append(reg2Name);
  symbols->append(reg3Name);  symbols->append(reg4Name);

  BrigInstBase instAbs = {
    0,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackP,
    {0, sizeof(reg1), 0, 0, 0}
  };
  instAbs.size = sizeof(instAbs);
 
  code->append(&instAbs);

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = funName.size() + symName.size() + 2;

  oper->append(&reg1);

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = funName.size() + symName.size() + reg1Name.size() + 3;
  
  oper->append(&reg2);

  BrigInstBase instAdd = {
    0,
    BrigEInstBase,
    BrigAdd,
    Brigu16x2,
    BrigPackPPsat,
    {0, sizeof(reg1) + sizeof(reg2), 
     sizeof(reg1) + sizeof(reg2) + sizeof(reg3), 0, 0}
  };
  instAdd.size = sizeof(instAdd);
 
  code->append(&instAdd);

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = funName.size() + symName.size() + 
                reg1Name.size() + reg2Name.size() + 4;
  
  oper->append(&reg3);

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = funName.size() + symName.size() + 
                reg1Name.size() + reg2Name.size() + 
                reg3Name.size() + 5;
  
  oper->append(&reg4);

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
  funRef.operationCount = 2;
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
  symRef.s.attribute = BrigExtern;
  symRef.s.reserved = 0;
  symRef.s.symbolModifier = 0;
  symRef.s.dim = 0;
  symRef.s.s_name = funName.size() + 1;
  symRef.s.type = Brigu8x4;
  symRef.s.align = 1;
  symRef.d_init = 0;
  symRef.reserved = 0;

  dir->append(&symRef);

  Example3_Test TestCase(in, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




