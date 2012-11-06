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

  BrigDirectiveVersion verRef;
  BrigDirectiveKernel kerRef;
  BrigDirectiveSymbol kerSymRef;
  BrigDirectiveSymbol priSymRef;

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

  verRef.size = sizeof(verRef);
  verRef.kind = BrigEDirectiveVersion;
  verRef.c_code = 0;
  verRef.major = 1;
  verRef.minor = 0;
  verRef.machine = BrigELarge;
  verRef.profile = BrigEFull;
  verRef.ftz = BrigENosftz;
  verRef.reserved = 0;

  dir->append(&verRef);

  kerRef.size = sizeof(kerRef);
  kerRef.kind = BrigEDirectiveKernel;
  kerRef.c_code = 0;
  kerRef.s_name = 0;
  kerRef.inParamCount = 1;
  kerRef.d_firstScopedDirective = sizeof(verRef) + sizeof(kerRef) +
                                  sizeof(kerSymRef);
  kerRef.operationCount = 1;
  kerRef.d_nextDirective = sizeof(verRef) + sizeof(kerRef) +
                           sizeof(kerSymRef) + sizeof(priSymRef);
  kerRef.attribute = BrigNone;
  kerRef.reserved = 0;
  kerRef.outParamCount = 0;
  kerRef.d_firstInParam = 0;

  dir->append(&kerRef);


  kerSymRef.size = sizeof(kerSymRef);
  kerSymRef.kind = BrigEDirectiveSymbol;
  kerSymRef.s.c_code = 0;
  kerSymRef.s.storageClass = BrigKernargSpace;
  kerSymRef.s.attribute = BrigNone;
  kerSymRef.s.reserved = 0;
  kerSymRef.s.symbolModifier = 0;
  kerSymRef.s.dim = 0;
  kerSymRef.s.s_name = 6;
  kerSymRef.s.type = Brigf32;
  kerSymRef.s.align = 1;
  kerSymRef.d_init = 0;
  kerSymRef.reserved = 0;

  dir->append(&kerSymRef);

  priSymRef.size = sizeof(priSymRef);
  priSymRef.kind = BrigEDirectiveSymbol;
  priSymRef.s.c_code = 0;
  priSymRef.s.storageClass = BrigPrivateSpace;
  priSymRef.s.attribute = BrigNone;
  priSymRef.s.reserved = 0;
  priSymRef.s.symbolModifier = 0;
  priSymRef.s.dim = 0;
  priSymRef.s.s_name = 9;
  priSymRef.s.type = Brigu32;
  priSymRef.s.align = 1;
  priSymRef.d_init = 0;
  priSymRef.reserved = 0;

  dir->append(&priSymRef);

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




