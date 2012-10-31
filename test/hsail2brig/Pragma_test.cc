#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

class Pragma_Test: public BrigCodeGenTest {

private:
  const BrigDirectivePragma* RefPrag;
  // Operands in .operands buffer

public:
  Pragma_Test(std::string& in, StringBuffer* sbuf, BrigDirectivePragma* ref):
    BrigCodeGenTest(in, sbuf),
    RefPrag(ref) { }

  void validate(struct BrigSections* TestOutput) {  
    
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   

    dir_iterator getdirective = TestOutput->begin();
    const BrigDirectivePragma* getprag = (cast<BrigDirectivePragma>(getdirective));
    validate_brig::validate(RefPrag, refbuf, getprag, getbuf);
         
  }
};


TEST(CodegenTest, Pragma_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
    
  std::string in;
  BrigDirectivePragma out;
  StringBuffer* buf = new StringBuffer;
  buf->clear();
   
  /************************************* Test Case 1 ************************************/
  in.assign("pragma \"once\";\n");
  buf->append("once");

  out.size = sizeof(out);
  out.kind = BrigEDirectivePragma;
  out.c_code = 0;
  out.s_name = 0;

  Pragma_Test TestCase1(in, buf, &out);
  TestCase1.Run_Test(&Pragma);
  
  delete buf;
}
} // namespace hsa
} // namespace brig




