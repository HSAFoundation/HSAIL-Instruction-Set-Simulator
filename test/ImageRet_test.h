#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

template <typename T2, typename T3, typename T4 = BrigOperandReg> 
class ImageRet_Test: public BrigCodeGenTest {

private:
  const BrigInstAtomicImage* RefInst;
  // Operands in .operands buffer
  const BrigOperandReg* RefDest;
  const BrigOperandOpaque* RefSrc1;
  const T2* RefSrc2;
  const T3* RefSrc3;
  const T4* RefSrc4;

public:
  ImageRet_Test(std::string& in, StringBuffer* sbuf, BrigInstAtomicImage* ref,
           BrigOperandReg* Dest, BrigOperandOpaque* Src1, T2* Src2, T3* Src3, 
           T4* Src4 = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2),
    RefSrc3(Src3),
    RefSrc4(Src4) { }
 
  void validate(struct BrigSections* TestOutput) {
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstAtomicImage* getinst = (cast<BrigInstAtomicImage>(getcode));
    validate_brig::validate(RefInst, getinst);

    const BrigOperandReg* getdest = reinterpret_cast <const BrigOperandReg*> 
                                    (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(RefDest, refbuf, getdest, getbuf);
    
    const BrigOperandOpaque *getsrc1 = reinterpret_cast <const BrigOperandOpaque*> 
                        (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validate(RefSrc1, getsrc1);

    const T2 *getsrc2 = reinterpret_cast <const T2*> 
                        (&(TestOutput->operands[getinst->o_operands[2]]));
    validate_brig::validateOpType<T2>(RefSrc2, refbuf, getsrc2, getbuf);

    const T3 *getsrc3 = reinterpret_cast <const T3*> 
                        (&(TestOutput->operands[getinst->o_operands[3]]));
    validate_brig::validateOpType<T3>(RefSrc3, refbuf, getsrc3, getbuf);

    
    if (RefSrc4 != NULL) {
      const T4 *getsrc4 = reinterpret_cast <const T4*> 
                          (&(TestOutput->operands[getinst->o_operands[4]]));
      validate_brig::validateOpType<T4>(RefSrc4, refbuf, getsrc4, getbuf);
    } else {
      EXPECT_EQ(0, getinst->o_operands[4]);
    }
  }
};


TEST(CodegenTest, ImageRet_CodeGen) {
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std::string in; 
  std::string destName, src2Name, src3Name, src4Name;
  StringBuffer* symbols;
  
  BrigOperandReg dest;
  BrigInstAtomicImage out;

  BrigOperandReg reg2, reg3, reg4;
  BrigOperandOpaque image1;

  symbols = new StringBuffer();
   
  /************************************* Test Case 1 ************************************/
  in.assign("atomic_image_xor_1d_b32 $s0, [&namedRWImg1], $s1, $s3;\n");
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
  dest.name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.name = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = destName.size() + src2Name.size() + 2;
  
  ImageRet_Test<BrigOperandReg, BrigOperandReg> TestCase1(in, symbols, &out, &dest, &image1, &reg2, &reg3);
  TestCase1.Run_Test(&ImageRet);
  symbols->clear();


  /************************************* Test Case 2 ************************************/
  in.assign("atomic_image_cas_1d_b32 $s10, [&namedRWImg2], $s1, $s3, $s4;\n");
  destName.assign("$s10");   src2Name.assign("$s1");
  src3Name.assign("$s3");   src4Name.assign("$s4");
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
  out.o_operands[4] = 0;

  out.atomicOperation = BrigAtomicCas;
  out.storageClass = BrigGlobalSpace;
  out.memorySemantic = BrigRegular;
  out.geom = Briggeom_1d;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandReg;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.name = 0;

  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.name = 0;
  image1.reg = 0;
  image1.offset = 0;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.name = destName.size() + 1;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.name = destName.size() + src2Name.size() + 2;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.name = destName.size() + src2Name.size() + src3Name.size() + 3;
  
  ImageRet_Test<BrigOperandReg, BrigOperandReg, BrigOperandReg> TestCase2(in, symbols, &out, &dest, &image1, &reg2, &reg3, &reg4);
  TestCase2.Run_Test(&ImageRet);
  symbols->clear();

  delete symbols;

}

} // namespace hsa
} // namespace brig




