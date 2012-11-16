#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"
#include "mock_error_reporter.h"

using ::testing::AtLeast;
using ::testing::_;

namespace hsa {
namespace brig {

class Version_Test: public BrigCodeGenTest {

private:
  const BrigDirectiveVersion* RefVer;
  // Operands in .operands buffer

public:
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
  out.ftz = BrigENosftz;
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
  out.ftz = BrigENosftz;
  out.reserved = 0;

  Version_Test TestCase2(in, &out);
  TestCase2.Run_Test(&Version);

  /************************************* Test Case 3 ************************************/
  in.assign("version 2:0:$large, $reduced, $sftz;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 2;
  out.minor = 0;
  out.machine = BrigELarge;
  out.profile = BrigEReduced;
  out.ftz = BrigESftz;
  out.reserved = 0;

  Version_Test TestCase3(in, &out);
  TestCase3.Run_Test(&Version);

  /************************************* Test Case 4 ************************************/
  in.assign("version 2:1:$nosftz, $small, $full;\n");

  out.size = sizeof(out);
  out.kind = BrigEDirectiveVersion;
  out.c_code = 0;
  out.major = 2;
  out.minor = 1;
  out.machine = BrigESmall;
  out.profile = BrigEFull;
  out.ftz = BrigENosftz;
  out.reserved = 0;

  Version_Test TestCase4(in, &out);
  TestCase4.Run_Test(&Version);

}

TEST(ErrorReportTest, Version_CodeGen) {  
  Context* context = Context::get_instance();
  context->clear_context();

  MockErrorReporter mer;
  context->set_error_reporter(&mer);
  mer.DelegateToFake();
  EXPECT_CALL(mer, report_error(_, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mer, get_last_error()).Times(AtLeast(1));

  std::string input = "version 2:1\n";
  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Version(context));
  EXPECT_EQ(MISSING_SEMICOLON, mer.get_last_error());
  context->set_error_reporter(ErrorReporter::get_instance());
  
  delete lexer;
}

} // namespace hsa
} // namespace brig




