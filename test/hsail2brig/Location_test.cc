#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"
#include "../mock_error_reporter.h"

using ::testing::AtLeast;
using ::testing::_;

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


TEST(ErrorReportTest, Location) {  
  Context* context = Context::get_instance();
  context->clear_context();

  MockErrorReporter mer;
  context->set_error_reporter(&mer);
  mer.DelegateToFake();
  EXPECT_CALL(mer, report_error(_, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mer, get_last_error()).Times(AtLeast(1));

  std::string input = "loc 1 10 5\n";
  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Location(context));
  EXPECT_EQ(MISSING_SEMICOLON, mer.get_last_error());
  
  input.assign("loc 10 5;\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Location(context));
  EXPECT_EQ(MISSING_INTEGER_CONSTANT, mer.get_last_error());
  
  context->set_error_reporter(ErrorReporter::get_instance());
  delete lexer;
}

} // namespace hsa
} // namespace brig




