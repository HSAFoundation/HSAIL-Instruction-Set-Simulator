#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"


namespace hsa {
namespace brig {

class Location_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveLoc* RefLoc;
  // Operands in .operands buffer

public:
  Location_Test(std::string& in, BrigDirectiveLoc* ref):
    BrigCodeGenTest(in),
    RefLoc(ref) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* dir = new Buffer();

    dir->append(RefLoc);
    
    struct BrigSections RefOutput(NULL, reinterpret_cast<const char *>(&dir->get()[0]),
                                  NULL, NULL, NULL, 0, dir->size(), 0, 0, 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete dir;
  } 

};


TEST(CodegenTest, Location_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
    
  std::string in;
  BrigDirectiveLoc out;
   
  /************************************* Test Case 1 ************************************/
  in.assign("loc 1 10 5;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveLoc;
  out.c_code = 0;
  out.sourceFile = 1;
  out.sourceLine = 10;
  out.sourceColumn = 5;

  Location_Test TestCase1(in, &out);
  TestCase1.Run_Test(&Location);
  
}
} // namespace hsa
} // namespace brig




