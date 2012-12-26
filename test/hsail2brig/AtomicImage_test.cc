#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"


namespace hsa {
namespace brig {

template <typename T2 = BrigOperandReg, 
          typename T3 = BrigOperandReg, 
          typename T4 = BrigOperandReg>
class AtomicImage_Test: public BrigCodeGenTest {

private:
  const BrigInstAtomicImage* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const BrigOperandOpaque* RefSrc1;
  const T2* RefSrc2;
  const T3* RefSrc3;
  const T4* RefSrc4;
  const BrigOperandReg* RegList;
  const BrigOperandReg* ImageReg;
  const std::string *SymbolName;

public:
  AtomicImage_Test(std::string& in, std::string *name = NULL):
    BrigCodeGenTest(in), SymbolName(name) {}

  AtomicImage_Test(std::string& in, StringBuffer* sbuf, BrigInstAtomicImage* ref,
           BrigOperandReg* Dest, BrigOperandOpaque* Src1, T2* Src2, T3* Src3,
           T4* Src4 = NULL, BrigOperandReg* regList = NULL, 
           BrigOperandReg *imageReg = NULL, std::string *name = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),           RefDest(Dest),
    RefSrc1(Src1),          RefSrc2(Src2),
    RefSrc3(Src3),          RefSrc4(Src4),
    RegList(regList),       ImageReg(imageReg),
    SymbolName(name) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();

    code->append(RefInst);

    if (RefDest != NULL) {
      oper->append(RefDest);
    }
    if (ImageReg != NULL) {
      oper->append(ImageReg);
    }
    oper->append(RefSrc1);

    if (RegList != NULL) {
      for (int i = 0 ; i < 4 ; ++i) {
        if (RegList[i].size != 0) {
          oper->append(&RegList[i]);
        }
      }
    }
    oper->append(RefSrc2);
    if (RefSrc3 != NULL) {
      oper->append(RefSrc3);
    }

    if (RefSrc4 != NULL) {
      oper->append(RefSrc4);
    }
    
    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      NULL, reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), 0, code->size(), oper->size(), 0);    
    
    Parse_Validate(Rule, &RefOutput, SymbolName);

    delete code;
    delete oper;
  }
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError, SymbolName);
  }
};


TEST(CodegenTest, ImageRet_CodeGen) {

  std::string in, symbolName;
  std::string destName, src2Name, src3Name, src4Name;
  std::string reg1Name, reg2Name, reg3Name, reg4Name;
  std::string regName;
  StringBuffer* symbols;

  BrigOperandReg dest;
  BrigInstAtomicImage out;

  BrigOperandReg reg2, reg3, reg4;
  BrigOperandReg reg;
  BrigOperandReg regList[4];
  BrigOperandOpaque image1;
  BrigOperandRegV2 regV2;
  BrigOperandRegV4 regV4;

  symbols = new StringBuffer();

  /************************* Test Case 1 *****************************/
  in.assign("atomic_image_xor_1d_b32 $s0, [&name], $s1, $s3;\n");
  destName.assign("$s0");   src2Name.assign("$s1");
  src3Name.assign("$s3");
  symbols->append(destName);  symbols->append(src2Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1);
  out.o_operands[3] = sizeof(dest) + sizeof(image1) + sizeof(reg2);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicXor;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = destName.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + src2Name.size() + 2;

  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandReg, BrigOperandReg> 
    TestCase1(in, symbols, &out, &dest, &image1, &reg2, &reg3,
        NULL, NULL, NULL, &symbolName);
  TestCase1.Run_Test(&ImageRet);
  symbols->clear();


  /********************* Test Case 2 *************************/
  in.assign("atomic_image_cas_1d_b32 $s10, [&name], $s1, $s3, $s4;\n");
  destName.assign("$s10");   src2Name.assign("$s1");
  src3Name.assign("$s3");    src4Name.assign("$s4");
  symbols->append(destName);  symbols->append(src2Name);
  symbols->append(src3Name);  symbols->append(src4Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1);
  out.o_operands[3] = sizeof(dest) + sizeof(image1) + sizeof(reg2);
  out.o_operands[4] = sizeof(dest) + sizeof(image1) + sizeof(reg2) + sizeof(reg3);

  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = destName.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + src2Name.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = destName.size() + src2Name.size() + src3Name.size() + 3;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandReg, BrigOperandReg, BrigOperandReg> 
    TestCase2(in, symbols, &out, &dest, &image1, &reg2, &reg3, &reg4,
        NULL, NULL, &symbolName);
  TestCase2.Run_Test(&ImageRet);
  symbols->clear();


  /*********************** Test Case 3 ***********************/
  in.assign("atomic_image_or_3d_u32 $s4, [&name], ($s0,$s3,$s1,$s10), $s2;\n");
  destName.assign("$s4");
  reg1Name.assign("$s0");  reg2Name.assign("$s3");
  reg3Name.assign("$s1");  reg4Name.assign("$s10");
  src3Name.assign("$s2");
  symbols->append(destName);
  symbols->append(reg1Name);  symbols->append(reg2Name);
  symbols->append(reg3Name);  symbols->append(reg4Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[3] = out.o_operands[2] + sizeof(regV4);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicOr;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_3d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = destName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(dest) + sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + reg1Name.size() + reg2Name.size() +
              reg3Name.size() + reg4Name.size() + 5;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase3(in, symbols, &out, &dest, &image1, &regV4, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase3.Run_Test(&ImageRet);
  symbols->clear();

  /********************* Test Case 4 ***********************/
  in.assign("atomic_image_and_ar_2d_s32 $s1, [&name], ($s0,$s3), $s2;\n");
  destName.assign("$s1");   reg1Name.assign("$s0");
  reg2Name.assign("$s3");   src3Name.assign("$s2");
  symbols->append(destName);  symbols->append(reg1Name);
  symbols->append(reg2Name);  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 2;
  out.o_operands[3] = out.o_operands[2] + sizeof(regV2);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicAnd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = destName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));


  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(dest) + sizeof(image1);
  regV2.regs[1] = sizeof(reg) + regV2.regs[0];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + reg1Name.size() + reg2Name.size() + 3;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase4(in, symbols, &out, &dest, &image1, &regV2, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase4.Run_Test(&ImageRet);
  symbols->clear();

  /******************* Test Case 5 *************************/
  in.assign("atomic_image_add_ar_2d_u32 $s1, [&name<$s5 + 4>], ($s0,$s3), $s2;\n");
  destName.assign("$s1");   regName.assign("$s5");
  reg1Name.assign("$s0");
  reg2Name.assign("$s3");   src3Name.assign("$s2");
  symbols->append(destName);  symbols->append(regName);
  symbols->append(reg1Name);
  symbols->append(reg2Name);  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(reg) + sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 3;
  out.o_operands[3] = out.o_operands[2] + sizeof(regV2);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicAdd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = sizeof(dest);
  image1.offset = 4;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = destName.size() + regName.size() + 2;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(dest) + sizeof(image1) + sizeof(reg);
  regV2.regs[1] = sizeof(reg3) + regV2.regs[0];

  reg3.size = sizeof(reg4);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + regName.size() + reg1Name.size() + reg2Name.size() + 4;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase5(in, symbols, &out, &dest, &image1, &regV2, &reg3, NULL, 
        regList, &reg, &symbolName);
  TestCase5.Run_Test(&ImageRet);
  symbols->clear();

  /*********************** Test Case 6 ***********************/
  in.assign("atomic_image_sub_acq_2d_u32 $s1, [&name<4>], ($s1,$s2), $s2;\n");
  destName.assign("$s1");    reg2Name.assign("$s2");
  symbols->append(destName); symbols->append(reg2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg);
  out.o_operands[3] = sizeof(dest) + sizeof(image1);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquire;
  out.geom = Briggeom_2d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 4;

  memset(&regList[0], 0, sizeof(regList[0]));

  regList[1].size = sizeof(regList[1]);
  regList[1].kind = BrigEOperandReg;
  regList[1].type = Brigb32;
  regList[1].reserved = 0;
  regList[1].s_name = dest.s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = 0;
  regV2.regs[1] = sizeof(dest) + sizeof(image1);

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + 1;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase6(in, symbols, &out, &dest, &image1, &regV2, NULL, NULL, 
        regList, NULL, &symbolName);
  TestCase6.Run_Test(&ImageRet);
  symbols->clear();

  /****************** Test Case 7 **************/
  in.assign("atomic_image_inc_rel_1db_s32 $s1, [&name<$s3 - 8>], ($s2), $s2;\n");
  destName.assign("$s1");
  regName.assign("$s3");
  src2Name.assign("$s2");
  symbols->append(destName);
  symbols->append(regName);
  symbols->append(src2Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest) + sizeof(reg);
  out.o_operands[2] = sizeof(reg) + sizeof(dest) + sizeof(image1);
  out.o_operands[3] = sizeof(reg) + sizeof(dest) + sizeof(image1);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRelease;
  out.geom = Briggeom_1db;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = destName.size() + 1;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = sizeof(dest);
  image1.offset = -8;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = destName.size() + regName.size() + 2;

  reg3 = reg2;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandReg, BrigOperandReg> 
    TestCase7(in, symbols, &out, &dest, &image1, &reg2, &reg3, NULL, 
        NULL, &reg, &symbolName);
  TestCase7.Run_Test(&ImageRet);
  symbols->clear();


  /****************** Test Case 8 ****************/
  in.assign("atomic_image_dec_1da_s32 $s1, [&name<$s1>], ($s1,$s2,$s3,$s4), $s3;\n");
  destName.assign("$s1");      reg2Name.assign("$s2");
  reg3Name.assign("$s3");      reg4Name.assign("$s4");
  symbols->append(destName);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 3;
  out.o_operands[3] = sizeof(dest) + sizeof(image1) + sizeof(reg);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicDec;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1da;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  reg = dest;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  memset(&regList[0], 0, sizeof(regList[0]));

  regList[1].size = sizeof(regList[1]);
  regList[1].kind = BrigEOperandReg;
  regList[1].type = Brigb32;
  regList[1].reserved = 0;
  regList[1].s_name = dest.s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = 0;
  regV4.regs[1] = sizeof(dest) + sizeof(image1);
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + reg2Name.size() + 2;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase8(in, symbols, &out, &dest, &image1, &regV4, NULL, NULL, 
        regList, NULL, &symbolName);
  TestCase8.Run_Test(&ImageRet);
  symbols->clear();


  /********************* Test Case 9 ****************/
  in.assign("atomic_image_min_2da_u32 $s1, [&name], ($s2,$s3,$s4,$s5), $s6;\n");
  destName.assign("$s1");
  reg1Name.assign("$s2");      reg2Name.assign("$s3");
  reg3Name.assign("$s4");      reg4Name.assign("$s5");
  src3Name.assign("$s6");
  symbols->append(destName);
  symbols->append(reg1Name);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[3] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 4 + sizeof(regV4);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicMin;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_2da;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = destName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(dest) + sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + reg1Name.size() + reg2Name.size() +
              reg3Name.size() + reg4Name.size() + 5;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase9(in, symbols, &out, &dest, &image1, &regV4, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase9.Run_Test(&ImageRet);
  symbols->clear();

  /********************* Test Case 10 *****************/
  in.assign("atomic_image_max_ar_2da_s32 $s1, [&name], ($s2,$s3,$s4,$s5), $s6;\n");
  destName.assign("$s1");
  reg1Name.assign("$s2");      reg2Name.assign("$s3");
  reg3Name.assign("$s4");      reg4Name.assign("$s5");
  src3Name.assign("$s6");
  symbols->append(destName);
  symbols->append(reg1Name);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(dest);
  out.o_operands[2] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[3] = sizeof(dest) + sizeof(image1) + sizeof(reg) * 4 + sizeof(regV4);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicMax;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2da;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = destName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(dest) + sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = destName.size() + reg1Name.size() + reg2Name.size() +
              reg3Name.size() + reg4Name.size() + 5;
  
  symbolName.assign("&name");
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase10(in, symbols, &out, &dest, &image1, &regV4, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase10.Run_Test(&ImageRet);
  symbols->clear();

  delete symbols;
}


TEST(CodegenTest, ImageNoRet_CodeGen) {
  
  std::string in, symbolName; 
  std::string src2Name, src3Name, src4Name;
  std::string reg1Name, reg2Name, reg3Name, reg4Name;
  std::string regName;
  BrigOperandReg regList[4];
  StringBuffer* symbols;
 
  BrigInstAtomicImage out;

  BrigOperandReg reg2, reg3, reg4;
  BrigOperandReg reg;
  BrigOperandOpaque image1;
  BrigOperandRegV2 regV2;
  BrigOperandRegV4 regV4;

  symbols = new StringBuffer();
   
  /********************* Test Case 1 ********************/
  in.assign("atomicNoRet_image_cas_1d_b32 [&name], $s1, $s3, $s4;\n");
  src2Name.assign("$s1");  src3Name.assign("$s3");  src4Name.assign("$s4");
  symbols->append(src2Name);  symbols->append(src3Name);  symbols->append(src4Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1);
  out.o_operands[2] = sizeof(image1) + sizeof(reg2);
  out.o_operands[3] = sizeof(image1) + sizeof(reg2) + sizeof(reg3);
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = 0;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = src2Name.size() + 1;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = src2Name.size() + src3Name.size() + 2;
   
 symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandReg, BrigOperandReg, BrigOperandReg> 
    TestCase1(in, symbols, &out, NULL, &image1, &reg2, &reg3, &reg4,
        NULL, NULL, &symbolName);
  TestCase1.Run_Test(&ImageNoRet);
  symbols->clear();


  /******************** Test Case 2 ***********************/
  in.assign("atomicNoRet_image_and_ar_2d_b32 [&name], ($s0,$s3), $s2;\n");
  reg1Name.assign("$s0");     reg2Name.assign("$s3");     src3Name.assign("$s2");
  symbols->append(reg1Name);  symbols->append(reg2Name);  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigb32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 2;
  out.o_operands[2] = out.o_operands[1] + sizeof(regV2);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicAnd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = 0;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(image1);
  regV2.regs[1] = sizeof(image1) + sizeof(reg);

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
   
 symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase2(in, symbols, &out, NULL, &image1, &regV2, &reg3, NULL, 
        regList, NULL,  &symbolName);
  TestCase2.Run_Test(&ImageNoRet);
  symbols->clear();

  /***************** Test Case 3 *****************/
  in.assign("atomicNoRet_image_or_3d_u32 [&name], ($s0,$s3,$s1,$s10), $s2;\n");
  reg1Name.assign("$s0");  reg2Name.assign("$s3");
  reg3Name.assign("$s1");  reg4Name.assign("$s10");
  src3Name.assign("$s2");
  symbols->append(reg1Name);  symbols->append(reg2Name);
  symbols->append(reg3Name);  symbols->append(reg4Name);
  symbols->append(src3Name); 

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[2] = out.o_operands[1] + sizeof(regV4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicOr;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_3d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = 0;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() +
              reg3Name.size() + reg4Name.size() + 4;
   
 symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase3(in, symbols, &out, NULL, &image1, &regV4, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase3.Run_Test(&ImageNoRet);
  symbols->clear();

  /****************** Test Case 4 *******************/
  in.assign("atomicNoRet_image_xor_ar_2d_s32 [&name], ($s0,$s3), $s2;\n");
  reg1Name.assign("$s0");
  reg2Name.assign("$s3");   src3Name.assign("$s2");
  symbols->append(reg1Name);
  symbols->append(reg2Name);  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 2;
  out.o_operands[2] = out.o_operands[1] + sizeof(regV2);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicXor;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = 0;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(image1);
  regV2.regs[1] = sizeof(reg) + regV2.regs[0];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase4(in, symbols, &out, NULL, &image1, &regV2, &reg3, NULL, 
        regList, NULL, &symbolName);
  TestCase4.Run_Test(&ImageNoRet);
  symbols->clear();

  /********************** Test Case 5 *******************/
  in.assign("atomicNoRet_image_add_ar_2d_u32 [&name<$s5 + 10>], ($s0,$s3), $s2;\n");
  regName.assign("$s5");    reg1Name.assign("$s0");   
  reg2Name.assign("$s3");   src3Name.assign("$s2");
  symbols->append(regName);   symbols->append(reg1Name);  
  symbols->append(reg2Name);  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 3;
  out.o_operands[2] = out.o_operands[1] + sizeof(regV2);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicAdd;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 10;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = regName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(image1) + sizeof(reg);
  regV2.regs[1] = sizeof(reg) + regV2.regs[0];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = regName.size() + reg1Name.size() + reg2Name.size() + 3;
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase5(in, symbols, &out, NULL, &image1, &regV2, &reg3, NULL, 
        regList, &reg, &symbolName);
  TestCase5.Run_Test(&ImageNoRet);
  symbols->clear();

  /*********************** Test Case 6 *******************/
  in.assign("atomicNoRet_image_sub_acq_2d_u32 [&name<$s5>], ($s1,$s2), $s2;\n");
  regName.assign("$s5");    reg1Name.assign("$s1");     reg2Name.assign("$s2");
  symbols->append(regName); symbols->append(reg1Name);  symbols->append(reg2Name); 

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 3;
  out.o_operands[2] = sizeof(image1) + sizeof(reg) * 2;
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicSub;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquire;
  out.geom = Briggeom_2d;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = regName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  memset(&regList[2], 0, sizeof(regList[2]) + sizeof(regList[3]));

  regV2.size = sizeof(regV2);
  regV2.kind = BrigEOperandRegV2;
  regV2.type = Brigb32;
  regV2.reserved = 0;
  regV2.regs[0] = sizeof(reg) + sizeof(image1);
  regV2.regs[1] = sizeof(reg) * 2 + sizeof(image1);
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV2, BrigOperandReg> 
    TestCase6(in, symbols, &out, NULL, &image1, &regV2, NULL, NULL, 
        regList, &reg, &symbolName);
  TestCase6.Run_Test(&ImageNoRet);
  symbols->clear();

  /******************** Test Case 7 *********************/
  in.assign("atomicNoRet_image_inc_rel_1db_s32 [&name<4>], ($s2), $s2;\n");
  src2Name.assign("$s2");   symbols->append(src2Name); 
  

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1);
  out.o_operands[2] = sizeof(image1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicInc;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRelease;
  out.geom = Briggeom_1db;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 4;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = 0;

   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandReg, BrigOperandReg> 
    TestCase7(in, symbols, &out, NULL, &image1, &reg2, NULL, 
        NULL, NULL, NULL, &symbolName);
  TestCase7.Run_Test(&ImageNoRet);
  symbols->clear();


  /********************* Test Case 8 *******************/
  in.assign("atomicNoRet_image_dec_1da_s32 [&name<$s1 - 2>], ($s1,$s2,$s3,$s4), $s3;\n");
  regName.assign("$s1");      reg2Name.assign("$s2");
  reg3Name.assign("$s3");      reg4Name.assign("$s4");
  symbols->append(regName);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[2] = sizeof(image1) + sizeof(reg) * 2; 
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicDec;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1da;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = -2;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  memset(&regList[0], 0, sizeof(regList[0]));
  
  regList[1].size = sizeof(regList[1]);
  regList[1].kind = BrigEOperandReg;
  regList[1].type = Brigb32;
  regList[1].reserved = 0;
  regList[1].s_name = reg.s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = 0;
  regV4.regs[1] = sizeof(reg) + sizeof(image1);
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase8(in, symbols, &out, NULL, &image1, &regV4, NULL, NULL, 
        regList, &reg, &symbolName);
  TestCase8.Run_Test(&ImageNoRet);
  symbols->clear();


  /********************** Test Case 9 *******************/
  in.assign("atomicNoRet_image_min_2da_u32 [&name<$s1>], ($s2,$s3,$s4,$s5), $s6;\n");
  regName.assign("$s1");      
  reg1Name.assign("$s2");      reg2Name.assign("$s3");
  reg3Name.assign("$s4");      reg4Name.assign("$s5");
  src3Name.assign("$s6");
  symbols->append(regName);   
  symbols->append(reg1Name);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigu32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = sizeof(reg);
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 5;
  out.o_operands[2] = sizeof(image1) + sizeof(reg) * 5 + sizeof(regV4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicMin;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_2da;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = regName.size() + 1;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(reg) + sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = regName.size() + reg1Name.size() + reg2Name.size() + 
              reg3Name.size() + reg4Name.size() + 5;
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase9(in, symbols, &out, NULL, &image1, &regV4, &reg3, 
        NULL, regList, &reg, &symbolName);
  TestCase9.Run_Test(&ImageNoRet);
  symbols->clear();

  /**************** Test Case 10 *************/
  in.assign("atomicNoRet_image_max_ar_2da_s32 [&name], ($s2,$s3,$s4,$s5), $s6;\n");
  reg1Name.assign("$s2");      reg2Name.assign("$s3");
  reg3Name.assign("$s4");      reg4Name.assign("$s5");
  src3Name.assign("$s6");
  symbols->append(reg1Name);   symbols->append(reg2Name);
  symbols->append(reg3Name);   symbols->append(reg4Name);
  symbols->append(src3Name);

  out.size = sizeof(out);
  out.kind = BrigEInstAtomicImage;
  out.opcode = BrigAtomicNoRetImage;
  out.type = Brigs32;
  out.packing = BrigNoPacking;
  out.o_operands[0] = 0;
  out.o_operands[1] = sizeof(image1) + sizeof(reg) * 4;
  out.o_operands[2] = sizeof(image1) + sizeof(reg) * 4 + sizeof(regV4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicMax;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigAcquireRelease;
  out.geom = Briggeom_2da;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  regList[0].size = sizeof(regList[0]);
  regList[0].kind = BrigEOperandReg;
  regList[0].type = Brigb32;
  regList[0].reserved = 0;
  regList[0].s_name = 0;
  
  regList[1] = regList[0];
  regList[1].s_name = regList[0].s_name + reg1Name.size() + 1;

  regList[2] = regList[1];
  regList[2].s_name = regList[1].s_name + reg2Name.size() + 1;

  regList[3] = regList[2];
  regList[3].s_name = regList[2].s_name + reg3Name.size() + 1;

  regV4.size = sizeof(regV4);
  regV4.kind = BrigEOperandRegV4;
  regV4.type = Brigb32;
  regV4.reserved = 0;
  regV4.regs[0] = sizeof(image1);
  regV4.regs[1] = sizeof(reg) + regV4.regs[0];
  regV4.regs[2] = sizeof(reg) + regV4.regs[1];
  regV4.regs[3] = sizeof(reg) + regV4.regs[2];

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 
              reg3Name.size() + reg4Name.size() + 4;
   
  symbolName.assign("&name"); 
  AtomicImage_Test<BrigOperandRegV4, BrigOperandReg> 
    TestCase10(in, symbols, &out, NULL, &image1, &regV4, &reg3, 
        NULL, regList, NULL, &symbolName);
  TestCase10.Run_Test(&ImageNoRet);
  symbols->clear();

  delete symbols;

}

TEST(ErrorReportTest, ImageRet) {  
  std::string symbolName;
  std::string input = "atomic_image_cas_1d_b32 $s10, [&name], $s1, $s3, $s4\n";
  
  symbolName.assign("&name");
  AtomicImage_Test<> TestCase1(input, &symbolName);
  TestCase1.Run_Test(&ImageRet, MISSING_SEMICOLON);
  input.assign("atomic_image_cas_1d_b32 $s10, [&name], $s1, $s3;\n");
  
  symbolName.assign("&name");
  AtomicImage_Test<> TestCase2(input, &symbolName);
  TestCase2.Run_Test(&ImageRet, MISSING_COMMA);

  input.assign("atomic_image_add_ar_2d $s1, [&name<$s5 + 4>], ($s0,$s3), $s2;\n");
  
  symbolName.assign("&name");
  AtomicImage_Test<> TestCase3(input, &symbolName);
  TestCase3.Run_Test(&ImageRet, MISSING_DATA_TYPE);
}

TEST(ErrorReportTest, ImageNoRet) {  
  std::string symbolName;
  std::string input = "atomicNoRet_image_cas_1d_b32 [&name], $s1, $s3, $s4\n";
   
  symbolName.assign("&name"); 
  AtomicImage_Test<> TestCase1(input, &symbolName);
  TestCase1.Run_Test(&ImageNoRet, MISSING_SEMICOLON);
  input.assign("atomicNoRet_image_max_ar_2da [&name], ($s2,$s3,$s4,$s5), $s6;\n");
   
  symbolName.assign("&name"); 
  AtomicImage_Test<> TestCase2(input, &symbolName);
  TestCase2.Run_Test(&ImageNoRet, MISSING_DATA_TYPE);

  input.assign("atomicNoRet_image_add_ar_2d_u32 [&name<$s5 + 10>], ($s0,$s3);\n");
  
  symbolName.assign("&name");
  AtomicImage_Test<> TestCase3(input, &symbolName);
  TestCase3.Run_Test(&ImageNoRet, MISSING_COMMA);
}

} // namespace hsa
} // namespace brig




