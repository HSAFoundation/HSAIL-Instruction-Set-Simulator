#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"
#include "../mock_error_reporter.h"

using ::testing::AtLeast;
using ::testing::_;
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

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* dir = new Buffer();

    dir->append(RefPrag);
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]), NULL, NULL, NULL, 
      RefStr->size(), dir->size(), 0, 0, 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete dir;
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

TEST(ErrorReportTest, Pragma) {  
  Context* context = Context::get_instance();
  context->clear_context();

  MockErrorReporter mer;
  context->set_error_reporter(&mer);
  mer.DelegateToFake();
  EXPECT_CALL(mer, report_error(_, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mer, get_last_error()).Times(AtLeast(1));

  std::string input = "pragma \"once\"\n";
  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Pragma(context));
  EXPECT_EQ(MISSING_SEMICOLON, mer.get_last_error());
  
  input.assign("pragma ;\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_FALSE(!Pragma(context));
  EXPECT_EQ(MISSING_STRING, mer.get_last_error());
  
  context->set_error_reporter(ErrorReporter::get_instance());
  delete lexer;
}

} // namespace hsa
} // namespace brig




