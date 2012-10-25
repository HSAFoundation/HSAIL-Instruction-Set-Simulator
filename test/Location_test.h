#include "codegen_validate.h"
#include "codegen_test.h"

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

  void validate(struct BrigSections* TestOutput) {  
    
    dir_iterator getdirective = TestOutput->begin();
    const BrigDirectiveLoc* getloc = (cast<BrigDirectiveLoc>(getdirective));
    validate_brig::validate(RefLoc, getloc);
         
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




