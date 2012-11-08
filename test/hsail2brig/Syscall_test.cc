#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

template <typename T1, typename T2, typename T3>
class Syscall_Test: public BrigCodeGenTest {

private:
  const BrigInstBase* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const BrigOperandImmed* RefSrc1;
  const T1* RefSrc2;
  const T2* RefSrc3;
  const T3* RefSrc4;

public:
  Syscall_Test(std::string& in, StringBuffer* sbuf, BrigInstBase* ref,
               BrigOperandReg* Dest, BrigOperandImmed* Src1, T1* Src2, T2* Src3, T3* Src4):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2),
    RefSrc3(Src3),
    RefSrc4(Src4) { }
  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();

    code->append(RefInst);
    oper->append(RefDest);

    if (RefSrc1 != NULL) {
      oper->append(RefSrc1);
    }
    if (RefSrc2 != NULL) {
      oper->append(RefSrc2);
    }
    if (RefSrc3 != NULL) {
      oper->append(RefSrc3);
    }
    if (RefSrc4 != NULL) {
      oper->append(RefSrc4);
    }

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]),
      reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), 0, code->size(), oper->size(), 0);    
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
    delete dir;
  }
};


TEST(CodegenTest, Syscall_CodeGen) {

  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low

  std:: string in;
  std::string destName, op2Name, op3Name, op4Name;
  StringBuffer* symbols;

  BrigOperandReg dest;
  BrigOperandImmed imm1;
  BrigInstBase out;

  BrigOperandReg reg2, reg3;
  BrigOperandWaveSz wav2, wav4;
  BrigOperandImmed imm2, imm3, imm4;

  symbols = new StringBuffer();

  /************************************* Test Case 1 ************************************/
  in.assign("syscall $s1, 3, $s2, $s3, $s1;\n");
  destName.assign("$s1");   op2Name.assign("$s2");
  op3Name.assign("$s3");
  symbols->append(destName);  symbols->append(op2Name);
  symbols->append(op3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigSysCall;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = out.o_operands[2] + sizeof(reg2);
  out.o_operands[4] = out.o_operands[0];

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 3;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = destName.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + op2Name.size() + 2;

  Syscall_Test<BrigOperandReg, BrigOperandReg, BrigOperandReg>
               TestCase1(in, symbols, &out, &dest, &imm1, &reg2, &reg3, NULL);
  TestCase1.Run_Test(&SysCall);
  symbols->clear();

  /************************************* Test Case 2 ************************************/
  in.assign("syscall $s1, 0x7, 0x8, 0x9, 0xa;\n");
  destName.assign("$s1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigSysCall;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = out.o_operands[2] + sizeof(imm2);
  out.o_operands[4] = out.o_operands[3] + sizeof(imm3);

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 7;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 8;

  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  memset(&imm3.bits, 0, sizeof(imm3.bits));
  imm3.bits.u = 9;

  imm4.size = sizeof(imm4);
  imm4.kind = BrigEOperandImmed;
  imm4.type = Brigb32;
  imm4.reserved = 0;
  memset(&imm4.bits, 0, sizeof(imm4.bits));
  imm4.bits.u = 10;

  Syscall_Test<BrigOperandImmed, BrigOperandImmed, BrigOperandImmed>
               TestCase2(in, symbols, &out, &dest, &imm1, &imm2, &imm3, &imm4);
  TestCase2.Run_Test(&SysCall);
  symbols->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("syscall $s12, 12, WAVESIZE, WAVESIZE, WAVESIZE;\n");
  destName.assign("$s12");   symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigSysCall;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = out.o_operands[2];
  out.o_operands[4] = out.o_operands[2];

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 12;

  wav2.size = sizeof(wav2);
  wav2.kind = BrigEOperandWaveSz;

  Syscall_Test<BrigOperandWaveSz, BrigOperandWaveSz, BrigOperandWaveSz>
               TestCase3(in, symbols, &out, &dest, &imm1, &wav2, NULL, NULL);
  TestCase3.Run_Test(&SysCall);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("syscall $s14, 12, $s11, 13, WAVESIZE;\n");
  destName.assign("$s14");   op2Name.assign("$s11");
  symbols->append(destName);  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstBase;
  out.opcode = BrigSysCall;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(imm1) + sizeof(dest);
  out.o_operands[3] = out.o_operands[2] + sizeof(reg2);
  out.o_operands[4] = out.o_operands[3] + sizeof(imm3);

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 12;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = destName.size() + 1;

  imm3.size = sizeof(imm3);
  imm3.kind = BrigEOperandImmed;
  imm3.type = Brigb32;
  imm3.reserved = 0;
  memset(&imm3.bits, 0, sizeof(imm3.bits));
  imm3.bits.u = 13;

  wav4.size = sizeof(wav4);
  wav4.kind = BrigEOperandWaveSz;

  Syscall_Test<BrigOperandReg, BrigOperandImmed, BrigOperandWaveSz>
               TestCase4(in, symbols, &out, &dest, &imm1, &reg2, &imm3, &wav4);
  TestCase4.Run_Test(&SysCall);
  symbols->clear();

  delete symbols;
}
} // namespace hsa
} // namespace brig




