#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

class FileDecl_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveFile* RefFile;
  // Operands in .operands buffer

public:
  FileDecl_Test(std::string& in, StringBuffer* sbuf, BrigDirectiveFile* ref):
    BrigCodeGenTest(in, sbuf),
    RefFile(ref) { }

  void validate(struct BrigSections* TestOutput) {  
    
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   

    dir_iterator getdirective = TestOutput->begin();
    const BrigDirectiveFile* getfile = (cast<BrigDirectiveFile>(getdirective));
    validate_brig::validate(RefFile, refbuf, getfile, getbuf);
         
  }
};


TEST(CodegenTest, FileDecl_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
    
  std::string in;
  BrigDirectiveFile out;
  StringBuffer* buf = new StringBuffer;
  buf->clear();
   
  /************************************* Test Case 1 ************************************/
  in.assign("file 1 \"math.c\";\n");
  buf->append("math.c");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveFile;
  out.c_code = 0;
  out.fileid = 1;
  out.s_filename = 0;

  FileDecl_Test TestCase1(in, buf, &out);
  TestCase1.Run_Test(&FileDecl);
  
  delete buf;
}
} // namespace hsa
} // namespace brig




