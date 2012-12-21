#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Version_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveVersion* RefVer;
  // Operands in .operands buffer

public:
  Version_Test(std::string& in):
    BrigCodeGenTest(in) {}

  Version_Test(std::string& in, BrigDirectiveVersion* ref):
    BrigCodeGenTest(in),
    RefVer(ref) { }

  void Run_Test(int (*Rule)(Context*)){
    Buffer* dir = new Buffer();

    dir->append(RefVer);

    struct BrigSections RefOutput(NULL, reinterpret_cast<const char *>(&dir->get()[0]),
                                  NULL, NULL, NULL, 0, dir->size(), 0, 0, 0);

    Parse_Validate(Rule, &RefOutput);
    delete dir;
  }

  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }

};


TEST(CodegenTest, Version_CodeGen) {

  /********************************** Common variables used by all tests******************************/

  std::string in;
  BrigDirectiveVersion out;

  /************************************* Test Case 1 ************************************/
  in.assign("version 1:0;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 1;
  out.minor = 0;
  out.machine = BrigELarge;
  out.profile = BrigEFull;
  out.reserved = 0;

  Version_Test TestCase1(in, &out);
  TestCase1.Run_Test(&Version);

  /************************************* Test Case 2 ************************************/
  in.assign("version 2:0:$large;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 2;
  out.minor = 0;
  out.machine = BrigELarge;
  out.profile = BrigEFull;
  out.reserved = 0;

  Version_Test TestCase2(in, &out);
  TestCase2.Run_Test(&Version);

  /************************************* Test Case 3 ************************************/
  in.assign("version 2:0:$large, $base;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 2;
  out.minor = 0;
  out.machine = BrigELarge;
  out.profile = BrigEBase;
  out.reserved = 0;

  Version_Test TestCase3(in, &out);
  TestCase3.Run_Test(&Version);

  /************************************* Test Case 4 ************************************/
  in.assign("version 2:1: $small, $full;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 2;
  out.minor = 1;
  out.machine = BrigESmall;
  out.profile = BrigEFull;
  out.reserved = 0;

  Version_Test TestCase4(in, &out);
  TestCase4.Run_Test(&Version);

}

TEST(ErrorReportTest, Version) {
  std::string input = "version 2:1\n";

  Version_Test TestCase1(input);
  TestCase1.Run_Test(&Version, MISSING_SEMICOLON);

  input.assign("version 2;1:$small, $full;\n");

  Version_Test TestCase2(input);
  TestCase2.Run_Test(&Version, MISSING_COLON);

  input.assign("version 2:1:$full $small;\n");

  Version_Test TestCase3(input);
  TestCase3.Run_Test(&Version, MISSING_SEMICOLON);
}

} // namespace hsa
} // namespace brig




