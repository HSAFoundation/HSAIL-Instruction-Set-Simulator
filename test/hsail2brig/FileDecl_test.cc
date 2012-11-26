#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class FileDecl_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveFile* RefFile;
  // Operands in .operands buffer

public:
  FileDecl_Test(std::string& in):
    BrigCodeGenTest(in) {}

  FileDecl_Test(std::string& in, StringBuffer* sbuf, BrigDirectiveFile* ref):
    BrigCodeGenTest(in, sbuf),
    RefFile(ref) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* dir = new Buffer();

    dir->append(RefFile);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]), NULL, NULL, NULL, 
      RefStr->size(), dir->size(), 0, 0, 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete dir;
  } 
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
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

TEST(ErrorReportTest, FileDecl) {  
  std::string input = "file 1 \"math.c\"\n";
  FileDecl_Test TestCase1(input);
  TestCase1.Run_Test(&FileDecl, MISSING_SEMICOLON);
  
  input.assign("file \"math.c\";\n");
  FileDecl_Test TestCase2(input);
  TestCase2.Run_Test(&FileDecl, MISSING_INTEGER_CONSTANT);

  input.assign("file 1;\n");
  FileDecl_Test TestCase3(input);
  TestCase3.Run_Test(&FileDecl, MISSING_STRING);
}

} // namespace hsa
} // namespace brig




