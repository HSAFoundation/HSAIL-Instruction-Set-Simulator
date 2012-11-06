#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Example1_Test: public BrigCodeGenTest {

private:
  Buffer* RefCode;
  Buffer* RefOper;
  Buffer* RefDir;

public:
  Example1_Test(std::string& in, StringBuffer* sbuf, Buffer* dir,
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

TEST(CodegenTest, Example1_CodeGen) {

  std::string in;

  StringBuffer* symbols = new StringBuffer();
  Buffer* code = new Buffer();
  Buffer* oper = new Buffer();
  Buffer* dir = new Buffer();

  in.append("version 1:0:$large; \n");
  in.append("kernel &demo(kernarg_f32 %x) { \n");
  in.append("private_u32 %z; \n");
  in.append("ret; \n");
  in.append("};\n");

  symbols->append("&demo");
  symbols->append("%x");
  symbols->append("%z");

  BrigInstBase instRet = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigRet,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  
  code->append(&instRet);

  BrigDirectiveVersion verRef = {
    sizeof(BrigDirectiveVersion),
    BrigEDirectiveVersion,
    0,
    1,                    //  major
    0,                    //  minor
    BrigELarge,
    BrigEFull,
    BrigENosftz,
    0
  };

  dir->append(&verRef);

  BrigDirectiveKernel ref = {
    40,                       // size
    BrigEDirectiveKernel,     // kind
    0,                        // c_code
    0,                        // s_name
    1,                        // inParamCount
    100,                      // d_firstScopedDirective
    1,                        // operationCount
    140,                      // d_nextDirective
    BrigNone,                 // attribute
    0,                        // fbar
    0,                        // outParamCount
    60,                       // d_firstInParam
  };

  dir->append(&ref);

  BrigDirectiveSymbol kernarg = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    BrigKernargSpace,         // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    6,                       // s_name
    Brigf32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  dir->append(&kernarg);

  BrigDirectiveSymbol private_var = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    BrigPrivateSpace,         // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    9,                        // s_name
    Brigu32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  dir->append(&private_var);

  Example1_Test TestCase(in, symbols, dir, code, oper);
  TestCase.Run_Test(&Program);
  symbols->clear();

  delete dir;
  delete code;
  delete oper;
  delete symbols;
}
} // namespace hsa
} // namespace brig




