#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

 class Control_Test : public BrigCodeGenTest{
private:

  //Instruction in .directive buffer
  const BrigDirectiveControl* RefDir;

public:
  Control_Test(std::string& in, StringBuffer* sbuf, BrigDirectiveControl* ref) :
    BrigCodeGenTest(in, sbuf),
    RefDir(ref)  { }

  void validate(struct BrigSections* TestOutput){

    dir_iterator getdir = TestOutput->begin();
    const BrigDirectiveControl* getDir = (cast<BrigDirectiveControl>(getdir));
    validate_brig::validate(RefDir, getDir);
  }
 };

TEST(CodegenTest,Control_CodeGen){
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  StringBuffer* symbols = new StringBuffer();
  std:: string in;

  /************************************* Test Case 1************************************/
  in.assign( "memopt_on; \n");

    BrigDirectiveControl out1 = {
    0,                       // size
    BrigEDirectiveControl,   // kind
    0,                       // c_code
    BrigEMemOpt,             // controlType
    {1, 0, 0},
  };
  out1.size = sizeof(out1);

  Control_Test
            TestCase1(in, symbols, &out1);
  TestCase1.Run_Test(&Control);
  symbols->clear();  

  /************************************* Test Case 2************************************/
  in.assign( "workgroupspercu 6; \n");

    BrigDirectiveControl out2 = {
    0,                       // size
    BrigEDirectiveControl,   // kind
    0,                       // c_code
    BrigEMaxGperC,           // controlType
    {6, 0, 0},
  };
  out2.size = sizeof(out2);

  Control_Test
            TestCase2(in, symbols, &out2);
  TestCase2.Run_Test(&Control);
  symbols->clear();  

  /************************************* Test Case 3************************************/
  in.assign( "itemsperworkgroup 2,3,4; \n");

    BrigDirectiveControl out3 = {
    0,                       // size
    BrigEDirectiveControl,   // kind
    0,                       // c_code
    BrigEMaxWIperG,           // controlType
    {2, 3, 4},
  };
  out3.size = sizeof(out3);

  Control_Test
            TestCase3(in, symbols, &out3);
  TestCase3.Run_Test(&Control);
  symbols->clear();  

  /************************************* Test Case 4************************************/
  in.assign( "memopt_off; \n");

    BrigDirectiveControl out4 = {
    0,                       // size
    BrigEDirectiveControl,   // kind
    0,                       // c_code
    BrigEMemOpt,             // controlType
    {0, 0, 0},
  };
  out4.size = sizeof(out4);

  Control_Test
            TestCase4(in, symbols, &out4);
  TestCase4.Run_Test(&Control);
  symbols->clear();  

  /*************************************end*********************************/
  delete symbols;
}

}
}
