#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4 or immediate value*/
class Comment_Test : public BrigCodeGenTest{
private:
  const BrigDirectiveComment *RefDir;

public:
  Comment_Test(std::string& input, StringBuffer* sbuf, BrigDirectiveComment *dir) :
    BrigCodeGenTest(input,sbuf),
    RefDir(dir) {}

  void Run_Test(int (*Rule)(Context*)){ 
    Buffer *dir = new Buffer();
    dir->append(RefDir);

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]), NULL, NULL, NULL, 
      RefStr->size(), dir->size(), (size_t)0, (size_t)0, (size_t)0);    
    
    Parse_Validate(Rule, &RefOutput);

    delete dir;
  }
};

TEST(CodegenTest, Comment_Codegen){
  std::string in, op1;
  StringBuffer* sbuf = new StringBuffer();

/************************************************/
  in.assign("/*This is a comment\n");  
  in.append("hello world*/");
  sbuf->append("This is a comment\nhello world");

  BrigDirectiveComment comm1 = {
    sizeof(BrigDirectiveComment),
    BrigEDirectiveComment,
    0,
    0
  };

  Comment_Test TestCase1(in, sbuf, &comm1);
  TestCase1.Run_Test(&Comment);
  sbuf->clear();

/************************************************/
  in.assign("// This is a comment\n");
  sbuf->append(" This is a comment");

  BrigDirectiveComment comm2 = {
    sizeof(BrigDirectiveComment),
    BrigEDirectiveComment,
    0,
    0
  };

  Comment_Test TestCase2(in, sbuf, &comm2);
  TestCase2.Run_Test(&Comment);
  sbuf->clear();

/*******************************************/
delete sbuf;
}

}
}
