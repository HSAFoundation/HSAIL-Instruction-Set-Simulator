#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

class Extension_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveExtension* RefExt;
  // Operands in .operands buffer

public:
  Extension_Test(std::string& in, StringBuffer* sbuf, BrigDirectiveExtension* ref):
    BrigCodeGenTest(in, sbuf),
    RefExt(ref) { }

  void validate(struct BrigSections* TestOutput) {  
    
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   

    dir_iterator getdirective = TestOutput->begin();
    const BrigDirectiveExtension* getext = (cast<BrigDirectiveExtension>(getdirective));
    validate_brig::validate(RefExt, refbuf, getext, getbuf);
         
  }
};


TEST(CodegenTest, Extension_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
    
  std::string in;
  BrigDirectiveExtension out;
  StringBuffer* buf = new StringBuffer;
  buf->clear();
   
  /************************************* Test Case 1 ************************************/
  in.assign("extension \"\\device\\amd.hsa\";\n");
  buf->append("\\device\\amd.hsa");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveExtension;
  out.c_code = 0;
  out.s_name = 0;

  Extension_Test TestCase1(in, buf, &out);
  TestCase1.Run_Test(&Extension);
  
  delete buf;
}
} // namespace hsa
} // namespace brig




