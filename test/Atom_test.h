#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

template <typename T1, typename T2 = BrigOperandReg> 
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
 
  void validate(struct BrigSections* TestOutput) {
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   

    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstAtomic* getinst = (cast<BrigInstAtomic>(getcode));
    validate_brig::validate(RefInst, getinst);

    const BrigOperandReg* getdest = reinterpret_cast <const BrigOperandReg*> 
                                    (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(RefDest, refbuf, getdest, getbuf);

    const BrigOperandAddress* getaddr = NULL;
    const BrigOperandReg* getreg = NULL;

    if (RefCompound != NULL) {
      const BrigOperandCompound* getcomp = reinterpret_cast <const BrigOperandCompound*> 
                                           (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefCompound, getcomp);
      
      getaddr = reinterpret_cast <const BrigOperandAddress*> 
                                 (&(TestOutput->operands[getcomp->name]));
      validate_brig::validate(RefAddr, getaddr);
      
      if (RefReg != NULL) {
        getreg = reinterpret_cast <const BrigOperandReg*> 
                                   (&(TestOutput->operands[getcomp->reg]));
        validate_brig::validate(RefReg, refbuf, getreg, getbuf);
      }
    } else if (RefIndirect != NULL) {
      const BrigOperandIndirect* getindi = reinterpret_cast <const BrigOperandIndirect*> 
                                           (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefIndirect, getindi);

      if (RefReg != NULL) {
        getreg = reinterpret_cast <const BrigOperandReg*> 
                                  (&(TestOutput->operands[getindi->reg]));
        validate_brig::validate(RefReg, refbuf, getreg, getbuf);
      }
    } else if (RefAddr != NULL) {
      getaddr = reinterpret_cast <const BrigOperandAddress*> 
                                 (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefAddr, getaddr);
    }

    const T1 *getsrc1 = reinterpret_cast <const T1*> 
                        (&(TestOutput->operands[getinst->o_operands[2]]));
    validate_brig::validateOpType<T1>(RefSrc1, refbuf, getsrc1, getbuf);

    if (RefSrc2 != NULL) {
      const T2 *getsrc2 = reinterpret_cast <const T2*> 
                          (&(TestOutput->operands[getinst->o_operands[3]]));
      validate_brig::validateOpType<T2>(RefSrc2, refbuf, getsrc2, getbuf);
    } else {
      EXPECT_EQ(0, getinst->o_operands[3]);
    }

    EXPECT_EQ(0, getinst->o_operands[4]);  

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
  BrigOperandWaveSz wav1, wav2;
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
  dest.name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  // TODO(Chuang): the type of immed should be b64.
  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 23;

  imm2.size = sizeof(imm2);
  imm2.kind = BrigEOperandImmed;
  imm2.type = Brigb32;
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
  dest.name = 0;

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
  in.assign("atomic_min_u64 $d1, [$s7], 23;\n");
  destName.assign("$d1");  symbols->append(destName);
  regName.assign("$s7");  symbols->append(regName);

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
  dest.name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = destName.size() + 1;

  imm1.size = sizeof(imm1);
  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
  imm1.reserved = 0;
  memset(&imm1.bits, 0, sizeof(imm1.bits));
  imm1.bits.u = 23;

  Atom_Test<BrigOperandImmed> TestCase3(in, symbols, &out, &dest, &indi, &reg, &imm1);
  TestCase3.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 4 ************************************/
  in.assign("atomic_or_group_u64 $d1, [%x][$s2 - 0x4], $d1;\n");
  destName.assign("$d1");  symbols->append(destName);
  regName.assign("$s2");  symbols->append(regName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu64;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg) + sizeof(addr);
  out.o_operands[2] = out.o_operands[1] + sizeof(comp);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicOr;
  out.storageClass = BrigGroupSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb64;
  dest.reserved = 0;
  dest.name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = sizeof(addr) + sizeof(dest);
  comp.offset = -4;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = destName.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1 = dest;

  Atom_Test<BrigOperandReg> TestCase4(in, symbols, &out, &dest, &comp, &addr, &reg, &reg1);
  TestCase4.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 5 ************************************/
  in.assign("atomic_cas_global_b32 $s1, [&map][$s1 + 0x7], WAVESIZE, WAVESIZE;\n");
  destName.assign("$s1");  
  symbols->append(destName);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(addr);
  out.o_operands[2] = out.o_operands[1] + sizeof(comp);
  out.o_operands[3] = out.o_operands[2] + sizeof(wav1);
  out.o_operands[4] = 0;
  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = sizeof(dest) + sizeof(addr);
  comp.offset = 7;

  reg = dest;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  wav1.size = sizeof(wav1);
  wav1.kind = BrigEOperandWaveSz;

  wav2 = wav1;

  Atom_Test<BrigOperandWaveSz, BrigOperandWaveSz> TestCase5(in, symbols, &out, &dest, &comp, &addr, &reg, &wav1, &wav2);
  TestCase5.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 6 ************************************/
  in.assign("atomic_cas_b32 $s1, [$s14 + 0x32], $s4, $s6;\n");
  destName.assign("$s1");  regName.assign("$s14");
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
  dest.name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x32;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = destName.size() + 1;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + regName.size() + 2;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = reg1.name + op1Name.size() + 1;

  Atom_Test<BrigOperandReg, BrigOperandReg> TestCase6(in, symbols, &out, &dest, &indi, &reg, &reg1, &reg2);
  TestCase6.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 7 ************************************/
  in.assign("atomic_max_group_f32 $s1, [0x77], $s14;\n");
  destName.assign("$s1");  op1Name.assign("$s14");  
  symbols->append(destName);  symbols->append(op1Name); 

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigf32;
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
  dest.name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = sizeof(dest);
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0x77;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.name = destName.size() + 1;

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
  dest.name = 0;

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
  reg1.name = destName.size() + 1;

  Atom_Test<BrigOperandReg> TestCase8(in, symbols, &out, &dest, &comp, &addr, NULL, &reg1);
  TestCase8.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 9 ************************************/
  in.assign("atomic_xor_global_u64 $d2, [&glo][$s2], 0x74;\n");
  destName.assign("$d2");  regName.assign("$s2");  
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
  dest.name = 0;

  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(dest);
  comp.reg = sizeof(dest) + sizeof(addr);
  comp.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = destName.size() + 1;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  // TODO(Chuang): the type of immed should be b64.
  imm1.size = sizeof(imm1);

  imm1.kind = BrigEOperandImmed;
  imm1.type = Brigb32;
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
  dest.name = 0;

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
  dest.name = 0;

  addr.size = sizeof(addr);
  addr.kind = BrigEOperandAddress;
  addr.type = Brigb64;
  addr.reserved = 0;
  addr.directive = 0;

  reg1 = dest;

  Atom_Test<BrigOperandReg> TestCase11(in, symbols, &out, &dest, &addr, &reg1);
  TestCase11.Run_Test(&Atom);
  symbols->clear();

  /************************************* Test Case 12 ************************************/
  in.assign("atomic_inc_group_u32 $s1, [$s1], $s1;\n");
  destName.assign("$s1");  symbols->append(destName); 

  out.size = sizeof(out);
  out.kind = BrigEInstAtomic;
  out.opcode = BrigAtomic;
  out.type = Brigu32;
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
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  indi.size = sizeof(indi);
  indi.kind = BrigEOperandIndirect;
  indi.reg = 0;
  indi.type = Brigb64;
  indi.reserved = 0;
  indi.offset = 0;

  reg = reg1 = dest;
 
  Atom_Test<BrigOperandReg> TestCase12(in, symbols, &out, &dest, &indi, &reg, &reg1);
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
  dest.name = 0;

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
  reg1.name = destName.size() + 1;
 
  Atom_Test<BrigOperandReg> TestCase13(in, symbols, &out, &dest, &indi, NULL, &reg1);
  TestCase13.Run_Test(&Atom);
  symbols->clear();


  delete symbols;

}



} // namespace hsa
} // namespace brig




