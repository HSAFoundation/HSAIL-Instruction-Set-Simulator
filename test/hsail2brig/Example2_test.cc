#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example2_Test: public BrigCodeGenTest {

private:
  Buffer* RefCode;
  Buffer* RefOper;
  Buffer* RefDir;

public:
  Example2_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Example2_CodeGen) {

  std::string in;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveVersion verRef;
  BrigDirectiveFunction funRef;
  BrigDirectiveSymbol symRef;

  in.append("version 1:0:$small; \n");
  in.append("function &return_true(arg_f32 %ret_val)(){ \n");
  in.append(" ret; \n");
  in.append("};");

  symbols->append("&return_true");
  symbols->append("%ret_val");

  BrigInstBase instRet = {
    0,                      // size
    BrigEInstBase,          // kind
    BrigRet,                // opcode
    Brigb32,                // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0}         // o_operands[5]
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
  funRef.operationCount = 1;
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
  symRef.s.s_name = 13;
  symRef.s.type = Brigf32;
  symRef.s.align = 1;
  symRef.d_init = 0;
  symRef.reserved = 0;

  dir->append(&symRef);

  Example2_Test TestCase(in, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




