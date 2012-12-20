#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

template <typename T1 = BrigOperandReg, 
          typename T2 = BrigOperandReg>
class Atom_Test: public BrigCodeGenTest {

private:
  const BrigInstAtomic* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const BrigOperandCompound* RefCompound;
  const BrigOperandIndirect* RefIndirect;
  const BrigOperandAddress* RefAddr;
  const BrigOperandReg* RefReg;

  const T1* RefSrc1;
  const T2* RefSrc2;

public:
  Atom_Test(std::string& in):
    BrigCodeGenTest(in) {}

  Atom_Test(std::string& in, StringBuffer* sbuf, BrigInstAtomic* ref,
            BrigOperandReg* Dest, BrigOperandCompound* Compound, BrigOperandAddress* Addr,
            BrigOperandReg* Reg, T1* Src1, T2* Src2 = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefCompound(Compound),
    RefIndirect(NULL),
    RefAddr(Addr),
    RefReg(Reg),
    RefSrc1(Src1),
    RefSrc2(Src2) { }

  Atom_Test(std::string& in, StringBuffer* sbuf, BrigInstAtomic* ref,
            BrigOperandReg* Dest, BrigOperandIndirect* Indirect,
            BrigOperandReg* Reg, T1* Src1, T2* Src2 = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefCompound(NULL),
    RefIndirect(Indirect),
    RefAddr(NULL),
    RefReg(Reg),
    RefSrc1(Src1),
    RefSrc2(Src2) { }

  Atom_Test(std::string& in, StringBuffer* sbuf, BrigInstAtomic* ref,
            BrigOperandReg* Dest, BrigOperandAddress* Addr, T1* Src1, T2* Src2 = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefCompound(NULL),
    RefIndirect(NULL),
    RefAddr(Addr),
    RefReg(NULL),
    RefSrc1(Src1),
    RefSrc2(Src2) { }
  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();
    code->append(RefInst);
    if (RefDest != NULL) {
      oper->append(RefDest);
    }
    if (RefCompound != NULL) {
      oper->append(RefAddr);
      if (RefReg != NULL) {
        oper->append(RefReg);
      }
      oper->append(RefCompound);
    } else if (RefIndirect != NULL) {
      if (RefReg != NULL) {
        oper->append(RefReg);
      }
      oper->append(RefIndirect);
    } else if (RefAddr != NULL) {
      oper->append(RefAddr);
    }
    if (RefSrc1 != NULL) {
      oper->append(RefSrc1);
    }
    if (RefSrc2 != NULL) {
      oper->append(RefSrc2);
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
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }
};

TEST(CodegenTest, Atom_CodeGen) {

  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low

  std::string in;
  std::string regName, destName, op1Name, op2Name;
  StringBuffer* symbols;

  BrigOperandReg dest;
  BrigInstAtomic out;
  BrigOperandCompound comp;
  BrigOperandIndirect indi;
  BrigOperandReg reg;
  BrigOperandAddress addr;

  BrigOperandReg reg1, reg2;
  BrigOperandWaveSz wav1;
  BrigOperandImmed imm1, imm2;


  symbols = new StringBuffer();

  /************************************* Test Case 1 ************************************/
  in.assign("atomic_cas_global_ar_b64 $d1, [&x1], 23, 12;\n");
  destName.assign("$d1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(addr);
  out.o_operands[3] = out.o_operands[2] + sizeof(imm1);
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 23;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 12;

  Atom_Test<BrigOperandImmed, BrigOperandImmed> TestCase1(in, symbols, &out, &dest, &addr, &imm1, &imm2);
  TestCase1.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 2 ************************************/
  in.assign("atomic_and_global_ar_u32 $s1, [&x2], 24;\n");
  destName.assign("$s1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(addr);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicAnd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

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
  imm1.bits.u = 24;

  Atom_Test<BrigOperandImmed> TestCase2(in, symbols, &out, &dest, &addr, &imm1);
  TestCase2.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("atomic_min_u64 $d1, [$d7], 23;\n");
  destName.assign("$d1");  symbols->append(destName);
  regName.assign("$d7");  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg);
  out.o_operands[2] = out.o_operands[1] + sizeof(indi);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicMin;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 23;

  Atom_Test<BrigOperandImmed> TestCase3(in, symbols, &out, &dest, &indi, &reg, &imm1);
  TestCase3.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("atomic_or_group_u64 $d1, [%x][$d2 - 0x4], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);
  regName.assign("$d2");  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg) + sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicOr;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb32;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = sizeof(addr) + sizeof(dest);
  comp.offset = -4;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  Atom_Test<BrigOperandReg> TestCase4(in, symbols, &out, &dest, &comp, &addr, &reg, NULL);
  TestCase4.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 5 ************************************/
  in.assign("atomic_cas_global_b32 $d1, [&map][$d1 + 0x7], WAVESIZE, WAVESIZE;\n");
  destName.assign("$d1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(addr);
  out.o_operands[2] = out.o_operands[1] + sizeof(comp);
  out.o_operands[3] = out.o_operands[2];
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = 0;
  comp.offset = 7;

  reg = dest;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;

  Atom_Test<BrigOperandWaveSz, BrigOperandWaveSz> TestCase5(in, symbols, &out, &dest, &comp, &addr, NULL, &wav1, NULL);
  TestCase5.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 6 ************************************/
  in.assign("atomic_cas_b32 $s1, [$d7 + 0x32], $s4, $s6;\n");
  destName.assign("$s1");  regName.assign("$d7");
  op1Name.assign("$s4");  op2Name.assign("$s6");
  symbols->append(destName);  symbols->append(regName);
  symbols->append(op1Name);  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg);
  out.o_operands[2] = out.o_operands[1] + sizeof(indi);
  out.o_operands[3] = out.o_operands[2] + sizeof(reg1);
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x32;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + regName.size() + 2;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1.s_name + op1Name.size() + 1;

  Atom_Test<BrigOperandReg, BrigOperandReg> TestCase6(in, symbols, &out, &dest, &indi, &reg, &reg1, &reg2);
  TestCase6.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 7 ************************************/
  in.assign("atomic_max_group_b32 $s1, [0x77], $s14;\n");
  destName.assign("$s1");  op1Name.assign("$s14");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(indi);
  out.o_operands[3] = out.o_operands[2] + sizeof(reg1);
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicMax;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb32;
  indi.reserved = 0;
  indi.offset = 0x77;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + 1;

  Atom_Test<BrigOperandReg> TestCase7(in, symbols, &out, &dest, &indi, NULL, &reg1);
  TestCase7.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 8 ************************************/
  in.assign("atomic_exch_s64 $d2, [%loc][0x7777], $d3;\n");
  destName.assign("$d2");  op1Name.assign("$d3");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigs64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(addr);
  out.o_operands[2] = out.o_operands[1] + sizeof(comp);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicExch;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = 0;
  comp.offset = 0x7777;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + 1;

  Atom_Test<BrigOperandReg> TestCase8(in, symbols, &out, &dest, &comp, &addr, NULL, &reg1);
  TestCase8.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 9 ************************************/
  in.assign("atomic_xor_global_u64 $d2, [&glo][$d3], 0x74;\n");
  destName.assign("$d2");  regName.assign("$d3");
  symbols->append(destName);  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(addr) + sizeof(reg);
  out.o_operands[2] = out.o_operands[1] + sizeof(comp);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicXor;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = sizeof(dest) + sizeof(addr);
  comp.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);

  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 0x74;

  Atom_Test<BrigOperandImmed> TestCase9(in, symbols, &out, &dest, &comp, &addr, &reg, &imm1);
  TestCase9.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 10 ************************************/
  in.assign("atomic_add_global_ar_b64 $d1, [&x], WAVESIZE;\n");
  destName.assign("$d1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(addr);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicAdd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;

  Atom_Test<BrigOperandWaveSz> TestCase10(in, symbols, &out, &dest, &addr, &wav1);
  TestCase10.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 11 ************************************/
  in.assign("atomic_dec_group_s32 $s1, [&y], $s1;\n");
  destName.assign("$s1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = out.o_operands[0];
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicDec;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  Atom_Test<BrigOperandReg> TestCase11(in, symbols, &out, &dest, &addr, NULL);
  TestCase11.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 12 ************************************/
  in.assign("atomic_inc_group_u64 $d1, [$d1], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = out.o_operands[0];
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb32;
  indi.reserved = 0;
  indi.offset = 0;

  Atom_Test<BrigOperandReg> TestCase12(in, symbols, &out, &dest, &indi, NULL, NULL);
  TestCase12.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 13 ************************************/
  in.assign("atomic_sub_u32 $s1, [0x100], $s2;\n");
  destName.assign("$s1");  op1Name.assign("$s2");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(indi);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x100;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + 1;

  Atom_Test<BrigOperandReg> TestCase13(in, symbols, &out, &dest, &indi, NULL, &reg1);
  TestCase13.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 14 ************************************/
  in.assign("atomic_inc_group_part_ar_u32 $s1, [$d2], $s3;\n");
  destName.assign("$s1");  symbols->append(destName);
  regName.assign("$d2");  symbols->append(regName);
  op1Name.assign("$s3");  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg);
  out.o_operands[2] = sizeof(dest) + sizeof(indi) + sizeof(reg);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigParAcquireRelease;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb32;
  indi.reserved = 0;
  indi.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + regName.size() + 2;

  Atom_Test<BrigOperandReg> TestCase14(in, symbols, &out, &dest, &indi, &reg, &reg1);
  TestCase14.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 15 ************************************/
  in.assign("atomic_sub_acq_u32 $s1, [0x15], $s2;\n");
  destName.assign("$s1");  op1Name.assign("$s2");
  symbols->append(destName);  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(indi);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigAcquire;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x15;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = destName.size() + 1;

  Atom_Test<BrigOperandReg> TestCase15(in, symbols, &out, &dest, &indi, NULL, &reg1);
  TestCase15.Run_Test(&Atom);

  delete symbols;

}

TEST(CodegenTest, AtomicNoRet_CodeGen) {

  /********************************** Common variables used by all tests******************************/

  std::string in;
  std::string regName, destName, op1Name, op2Name;
  StringBuffer* symbols;

  BrigInstAtomic out;
  BrigOperandCompound comp;
  BrigOperandIndirect indi;
  BrigOperandReg reg;
  BrigOperandAddress addr;

  BrigOperandReg reg1, reg2;
  BrigOperandWaveSz wav1;
  BrigOperandImmed imm1, imm2;

  symbols = new StringBuffer();

  /************************************* Test Case 1 ************************************/
  in.assign("atomicNoRet_cas_global_ar_b64 [&x], 23, 12;\n");

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = sizeof(addr) + sizeof(imm1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 23;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb64;
  imm2.reserved = 0;
  memset(&imm2.bits, 0, sizeof(imm2.bits));
  imm2.bits.u = 12;

  // there is no dest operand in AtomicNoRet.
  Atom_Test<BrigOperandImmed, BrigOperandImmed> TestCase1(in, symbols, &out, NULL, &addr, &imm1, &imm2);
  TestCase1.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 2 ************************************/
  in.assign("atomicNoRet_max_global_ar_b64 [&x], 23.0;\n");

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicMax;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.d = 23.0;

  // there is no dest operand in AtomicNoRet.
  Atom_Test<BrigOperandImmed> TestCase2(in, symbols, &out, NULL, &addr, &imm1);
  TestCase2.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("atomicNoRet_and_global_ar_u32 [&x2], 24;\n");

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicAnd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 24;

  Atom_Test<BrigOperandImmed> TestCase3(in, symbols, &out, NULL, &addr, &imm1);
  TestCase3.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("atomicNoRet_min_u64 [$d6], $d7;\n");
  regName.assign("$d6");  symbols->append(regName);
  op2Name.assign("$d7");  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = sizeof(reg) + sizeof(indi);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicMin;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  reg2.size = sizeof(reg);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb64;
  reg2.reserved = 0;
  reg2.s_name = regName.size() + 1;


  Atom_Test<BrigOperandReg> TestCase4(in, symbols, &out, NULL, &indi, &reg, &reg2);
  TestCase4.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 5 ************************************/
  in.assign("atomicNoRet_or_group_u64 [%x][$d2 - 0x4], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);
  regName.assign("$d2");  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(addr) + sizeof(reg);
  out.o_operands[1] = sizeof(addr) + sizeof(reg) + sizeof(comp);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicOr;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb32;
  comp.reserved = 0;
  comp.name = 0;
  comp.reg = sizeof(addr);
  comp.offset = -4;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase5(in, symbols, &out, NULL, &comp, &addr, &reg, &reg1);
  TestCase5.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 6 ************************************/
  in.assign("atomicNoRet_cas_global_b32 [&map][$d1 + 0x7], WAVESIZE, WAVESIZE;\n");
  destName.assign("$d1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(addr) + sizeof(reg);
  out.o_operands[1] = out.o_operands[0] + sizeof(comp);
  out.o_operands[2] = out.o_operands[1];
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;


  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = 0;
  comp.reg = sizeof(addr);
  comp.offset = 7;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;

  Atom_Test<BrigOperandWaveSz, BrigOperandWaveSz> TestCase6(in, symbols, &out, NULL, &comp, &addr, &reg, &wav1, NULL);
  TestCase6.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 7 ************************************/
  in.assign("atomicNoRet_cas_b32 [$d4 + 0x32], $s4, $s6;\n");
  regName.assign("$d4");    op1Name.assign("$s4");     op2Name.assign("$s6");
  symbols->append(regName);  symbols->append(op1Name);  symbols->append(op2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = out.o_operands[0] + sizeof(indi);
  out.o_operands[2] = out.o_operands[1] + sizeof(reg1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x32;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = regName.size() + 1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1.s_name + op1Name.size() + 1;

  Atom_Test<BrigOperandReg, BrigOperandReg> TestCase7(in, symbols, &out, NULL, &indi, &reg, &reg1, &reg2);
  TestCase7.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 8 ************************************/
  in.assign("atomicNoRet_max_group_b32 [0x77], $s14;\n");
  op1Name.assign("$s14");   symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(indi);
  out.o_operands[2] = out.o_operands[1] + sizeof(reg1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicMax;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb32;
  indi.reserved = 0;
  indi.offset = 0x77;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase8(in, symbols, &out, NULL, &indi, NULL, &reg1);
  TestCase8.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 9 ************************************/
  in.assign("atomicNoRet_inc_s64 [%loc][0x7777], $d3;\n");
  op1Name.assign("$d3");  symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigs64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(addr);
  out.o_operands[1] = out.o_operands[0] + sizeof(comp);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = 0;
  comp.reg = 0;
  comp.offset = 0x7777;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase9(in, symbols, &out, NULL, &comp, &addr, NULL, &reg1);
  TestCase9.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 10 ************************************/
  in.assign("atomicNoRet_xor_global_u64 [&glo][$d2], 0x74;\n");
  regName.assign("$d2");  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(addr) + sizeof(reg);
  out.o_operands[1] = out.o_operands[0] + sizeof(comp);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicXor;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = 0;
  comp.reg = sizeof(addr);
  comp.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  imm1.size = sizeof(imm1);

  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb64;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 0x74;

  Atom_Test<BrigOperandImmed> TestCase10(in, symbols, &out, NULL, &comp, &addr, &reg, &imm1);
  TestCase10.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 11 ************************************/
  in.assign("atomicNoRet_add_global_ar_b64 [&x], WAVESIZE;\n");
  destName.assign("$d1");
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigb64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicAdd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;

  Atom_Test<BrigOperandWaveSz> TestCase11(in, symbols, &out, NULL, &addr, &wav1);
  TestCase11.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 12 ************************************/
  in.assign("atomicNoRet_dec_group_s32 [&y], $s1;\n");
  destName.assign("$s1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicDec;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase12(in, symbols, &out, NULL, &addr, &reg1);
  TestCase12.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 13 ************************************/
  in.assign("atomicNoRet_inc_group_u64 [$d1], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = 0;
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb32;
  indi.reserved = 0;
  indi.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase13(in, symbols, &out, NULL, &indi, &reg, NULL);
  TestCase13.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 14 ************************************/
  in.assign("atomicNoRet_sub_u32 [0x100], $s2;\n");
  op1Name.assign("$s2");   symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(indi);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigRegular;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x100;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase14(in, symbols, &out, NULL, &indi, NULL, &reg1);
  TestCase14.Run_Test(&AtomicNoRet);
  symbols->clear();

  /************************************* Test Case 15 ************************************/
  in.assign("atomicNoRet_dec_group_acq_s64 [&y], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigs64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(addr);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicDec;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigAcquire;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb64;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase15(in, symbols, &out, NULL, &addr, &reg1);
  TestCase15.Run_Test(&AtomicNoRet);
  symbols->clear();
  /************************************* Test Case 16 ************************************/
  in.assign("atomicNoRet_sub_part_ar_u32 [0x1], $s2;\n");
  op1Name.assign("$s2");   symbols->append(op1Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomicNoRet;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(indi);
  out.o_operands[2] = 0;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigFlatSpace;
  out.memorySemantic = BrigParAcquireRelease;


  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x1;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  Atom_Test<BrigOperandReg> TestCase16(in, symbols, &out, NULL, &indi, NULL, &reg1);
  TestCase16.Run_Test(&AtomicNoRet);
  symbols->clear();

  delete symbols;

}

TEST(ErrorReportTest, Atom) {  
  std::string input = "atomic_and_global_ar_u32 $s1, [&x2], 24\n";
  Atom_Test<> TestCase1(input);
  TestCase1.Run_Test(&Atom, MISSING_SEMICOLON);

  input.assign("atomic_cas_global_ar_b64 $d1, [&x1], 12;\n");
  Atom_Test<> TestCase2(input);
  TestCase2.Run_Test(&Atom, MISSING_COMMA);

  input.assign("atomic_sub $s1, [0x100], $s2;\n");
  Atom_Test<> TestCase3(input);
  TestCase3.Run_Test(&Atom, MISSING_DATA_TYPE);
}

TEST(ErrorReportTest, AtomicNoRet) {  
  std::string input = "atomicNoRet_sub_u32 [0x100], $s2\n";
  Atom_Test<> TestCase1(input);
  TestCase1.Run_Test(&AtomicNoRet, MISSING_SEMICOLON);

  input.assign("atomicNoRet_dec_group [&y], $s1;\n");
  Atom_Test<> TestCase2(input);
  TestCase2.Run_Test(&AtomicNoRet, MISSING_DATA_TYPE);

  input.assign("atomicNoRet_or_group_u64 [%x][$d2 - 0x4];\n");
  Atom_Test<> TestCase3(input);
  TestCase3.Run_Test(&AtomicNoRet, MISSING_COMMA);
}

} // namespace hsa
} // namespace brig

