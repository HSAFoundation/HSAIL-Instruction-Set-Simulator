#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

template<typename T>
class Operand_Test: public BrigCodeGenTest {

private:
  const T* Ref;

public:
  Operand_Test(std::string& in, StringBuffer* sbuf, T* ref):
    BrigCodeGenTest(in, sbuf),
    Ref(ref) { }

  void Run_Test(int (*Rule)(Context*)){
    Buffer* oper = new Buffer();

    oper->append(Ref);

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]),
      NULL, NULL, reinterpret_cast<const char *>(&oper->get()[0]), NULL,
      RefStr->size(), 0, 0, oper->size(), 0);

    Parse_Validate(Rule, &RefOutput);
    delete oper;
  }
};


TEST(CodegenTest, Operand_CodeGen) {
  std::string in;
  BrigOperandImmed imm;
  StringBuffer* buf = new StringBuffer;
  buf->clear();

  /********************* Test Case 1 ************************/
  in.assign("_u16x2(0x12345, 0x6789a)");
  memset(&imm, 0, sizeof(imm));
  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb32;
  imm.reserved = 0;
  imm.bits.u = 0x2345789a;

  Operand_Test<BrigOperandImmed> TestCase1(in, buf, &imm);
  TestCase1.Run_Test(&Operand);

  /********************* Test Case 2 ************************/
  in.assign("_s32x2(-1, 1)");
  memset(&imm, 0, sizeof(imm));
  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb64;
  imm.reserved = 0;
  imm.bits.l[0] = 0xffffffff00000001LL;

  Operand_Test<BrigOperandImmed> TestCase2(in, buf, &imm);
  TestCase2.Run_Test(&Operand);

  /********************* Test Case 3 ************************/
  in.assign("_s8x16(-1,0,1,0,-2,0,0x1234,0,0x56,0,0x78,0,0x9a,0,0xbc,0xde)");
  memset(&imm, 0, sizeof(imm));
  imm.size = sizeof(imm);
  imm.kind = BrigEOperandImmed;
  imm.type = Brigb128;
  imm.reserved = 0;
  imm.bits.l[0] = 0xff000100fe003400LL;
  imm.bits.l[1] = 0x560078009a00bcdeLL;

  Operand_Test<BrigOperandImmed> TestCase3(in, buf, &imm);
  TestCase3.Run_Test(&Operand);
  delete buf;
}

}
}
