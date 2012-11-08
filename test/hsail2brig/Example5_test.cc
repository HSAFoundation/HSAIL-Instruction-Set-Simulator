#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example5_Test: public BrigCodeGenTest {

private:
  Buffer* RefCode;
  Buffer* RefOper;
  Buffer* RefDir;

public:
  Example5_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Example5_CodeGen) {

  std::string input;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  BrigDirectiveVersion verRef;
  BrigDirectiveFunction funRef;

  input.append("version 1:0:$small; \n");
  input.append("function &callee()(){ \n");
  input.append(" ret; \n");
  input.append("}; \n");
  input.append(" function &caller()(){ \n");
  input.append(" { \n");
  input.append("   call &callee; \n");
  input.append(" } \n");
  input.append("}; \n");

  std::string fun1,fun2;
  fun1.assign("&callee");
  fun2.assign("&caller");
  symbols->append(fun1);
  symbols->append(fun2);

  BrigOperandFunctionRef function1 = {
    0,
    BrigEOperandFunctionRef,
    sizeof(verRef)
  };
  function1.size = sizeof(function1);

  BrigInstBase instRet = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigRet,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  instRet.size = sizeof(instRet);
  
  code->append(&instRet);

  BrigOperandFunctionRef function2 = {
    0,
    BrigEOperandFunctionRef,
    sizeof(verRef) + sizeof(function1)
  };
  function2.size = sizeof(function2);

  BrigDirectiveScope start = {
    0,
    BrigEDirectiveArgStart,
    sizeof(instRet)
  };
  start.size = sizeof(start);

  BrigOperandArgumentList argumentlist = {
    0,
    BrigEOperandArgumentList,
    0,
    0
  };
  argumentlist.size = sizeof(argumentlist);

  BrigOperandImmed width = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}
  };
  width.size = sizeof(width);

  BrigInstBase instCall = {
    0,  // size
    BrigEInstBase,         // kind
    BrigCall,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, sizeof(width) + sizeof(function1), sizeof(width), sizeof(width) + sizeof(function1), 0}  // o_operands[5]
  };
  instCall.size = sizeof(instCall);

  BrigDirectiveScope end = {
    0,
    BrigEDirectiveArgEnd,
    sizeof(instRet) + sizeof(instCall)
  };
  end.size = sizeof(end);

  code->append(&instCall);
  oper->append(&width);
  oper->append(&function1);
  oper->append(&argumentlist);

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
  funRef.operationCount = 1;
  funRef.d_nextDirective = funRef.d_firstScopedDirective;
  funRef.attribute = BrigNone;
  funRef.reserved = 0;
  funRef.outParamCount = 0;
  funRef.d_firstInParam = 0;

  dir->append(&funRef);

  funRef.size = sizeof(funRef);
  funRef.kind = BrigEDirectiveFunction;
  funRef.c_code = sizeof(instRet);
  funRef.s_name = fun1.size() + 1;
  funRef.inParamCount = 0;
  funRef.d_firstScopedDirective = sizeof(verRef) + sizeof(funRef) * 2;
  funRef.operationCount = 1;
  funRef.d_nextDirective = funRef.d_firstScopedDirective + sizeof(start) + sizeof(end);
  funRef.attribute = BrigNone;
  funRef.reserved = 0;
  funRef.outParamCount = 0;
  funRef.d_firstInParam = 0;

  dir->append(&funRef);
  dir->append(&start);
  dir->append(&end);

  Example5_Test TestCase(input, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




