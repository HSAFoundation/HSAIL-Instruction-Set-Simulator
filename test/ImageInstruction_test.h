#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename TInst, typename TSrc> 
class ImageInst_Test : public BrigCodeGenTest{
private:

  const TInst* RefInst;
  const BrigOperandRegV4* RefDest;
  const BrigOperandReg* RefDestList;

  const BrigOperandOpaque* RefImage;
  const BrigOperandOpaque* RefSample;
  const TSrc* RefSrc;
  const BrigOperandReg* RefSrcList;

public:
  ImageInst_Test(std::string& input, StringBuffer* sbuf, TInst* ref,
                 BrigOperandRegV4* dest, BrigOperandReg* destList, 
                 BrigOperandOpaque* image, BrigOperandOpaque* sample, 
                 TSrc* src, BrigOperandReg* srcList = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefDest(dest),
    RefDestList(destList),
    RefImage(image),
    RefSample(sample),
    RefSrc(src),
    RefSrcList(srcList) { }

  void validate(struct BrigSections* TestOutput) {
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;

    inst_iterator getcode = TestOutput->code_begin();
    const TInst* getinst = (cast<TInst>(getcode));
    validate_brig::validate(RefInst, getinst);
    uint32_t opCount = 0;

    const BrigOperandRegV4 *getdest = reinterpret_cast <const BrigOperandRegV4*>
                                      (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validate(RefDest, getdest);

    const BrigOperandReg* getreg;
    for (uint32_t i = 0 ; i < 4 ; ++i) {
      getreg = reinterpret_cast <const BrigOperandReg*>
               (&(TestOutput->operands[getdest->regs[i]]));
      validate_brig::validateOpType<BrigOperandReg>(&RefDestList[i], refbuf, getreg, getbuf);
    }

    const BrigOperandOpaque* getimage = reinterpret_cast <const BrigOperandOpaque*>
                                        (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validate(RefImage, getimage);

    if (RefSample != NULL) {
      const BrigOperandOpaque* getsample = reinterpret_cast <const BrigOperandOpaque*>
                                           (&(TestOutput->operands[getinst->o_operands[opCount++]]));
      validate_brig::validate(RefSample, getsample);
    }

    const TSrc* getsrc = reinterpret_cast <const TSrc*>
                        (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validateOpType<TSrc>(RefSrc, refbuf, getsrc, getbuf);
    
    if (RefSrcList != NULL) {
      if (getsrc->kind == BrigEOperandRegV2) {
        const BrigOperandRegV2* getregV2 = reinterpret_cast<const BrigOperandRegV2*>(getsrc);
        for (uint32_t i = 0 ; i < 2 ; ++i) {
          getreg = reinterpret_cast <const BrigOperandReg*>
                   (&(TestOutput->operands[getregV2->regs[i]]));
          validate_brig::validateOpType<BrigOperandReg>(&RefSrcList[i], refbuf, getreg, getbuf); 
        }
      } else if (getsrc->kind == BrigEOperandRegV4) {
        const BrigOperandRegV4* getregV4 = reinterpret_cast<const BrigOperandRegV4*>(getsrc);
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          getreg = reinterpret_cast <const BrigOperandReg*>
                   (&(TestOutput->operands[getregV4->regs[i]]));
          validate_brig::validateOpType<BrigOperandReg>(&RefSrcList[i], refbuf, getreg, getbuf); 
        }
      }
    }

    while (opCount <= 4) {
      EXPECT_EQ(0, getinst->o_operands[opCount++]);
    }
  }
};

/*********************** ImageStore Test ***************************/
TEST(CodegenTest, ImageStore_Codegen) {

  /********************* Common variables **********************/
  std::string in;
  std::string reg1Name, reg2Name, reg3Name, reg4Name, reg5Name;
  StringBuffer* sbuf = new StringBuffer();

  BrigInstImage out;
  BrigOperandReg reg1, reg2, reg3, reg4;
  BrigOperandRegV4 dest, srcRegV4;
  BrigOperandRegV2 srcRegV2;
  BrigOperandReg destList[4], srcList[4];
  BrigOperandOpaque image1;
  

  /**************************** Case 1 ***************************/
  in.assign("st_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s2,$s3,$s4,$s5);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigStImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_2da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;
  srcList[0] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  srcList[1] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  srcList[2] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;
  
  srcRegV4.size = sizeof(srcRegV4);
  srcRegV4.kind = BrigEOperandRegV4;
  srcRegV4.type = Brigb32;
  srcRegV4.reserved = 0;
  srcRegV4.regs[0] = sizeof(reg1);
  srcRegV4.regs[1] = sizeof(reg1) + sizeof(reg2);
  srcRegV4.regs[2] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  srcRegV4.regs[3] = sizeof(reg1) + sizeof(reg2) + 
                      sizeof(reg3) + sizeof(reg4) + 
                      sizeof(dest) + sizeof(image1);

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  srcList[3] = reg4;

  ImageInst_Test<BrigInstImage, BrigOperandRegV4> 
  TestCase1(in, sbuf, &out, &dest, destList, &image1, NULL, &srcRegV4, srcList);
  TestCase1.Run_Test(&ImageStore);
  
  sbuf->clear();

  /**************************** Case 2 ***************************/
  in.assign("st_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigStImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  srcList[0] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;
  
  srcRegV2.size = sizeof(srcRegV2);
  srcRegV2.kind = BrigEOperandRegV2;
  srcRegV2.type = Brigb32;
  srcRegV2.reserved = 0;
  srcRegV2.regs[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  srcRegV2.regs[1] = sizeof(reg1) + sizeof(reg2) + 
                      sizeof(reg3) + sizeof(reg4) + 
                      sizeof(dest) + sizeof(image1);

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  srcList[1] = reg4;

  ImageInst_Test<BrigInstImage, BrigOperandRegV2> 
  TestCase2(in, sbuf, &out, &dest, destList, &image1, NULL, &srcRegV2, srcList);
  TestCase2.Run_Test(&ImageStore);
  
  sbuf->clear();

  /**************************** Case 3 ***************************/
  in.assign("st_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3<$s4 - 0x4>], ($s5);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigStImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1db;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  image1.offset = -4;
  
  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  ImageInst_Test<BrigInstImage, BrigOperandReg> 
  TestCase3(in, sbuf, &out, &dest, destList, &image1, NULL, &reg2);
  TestCase3.Run_Test(&ImageStore);
  
  sbuf->clear();
  /***********************  End of tests *************************/
  delete sbuf;
}


/*********************** ImageLoad Test ***************************/
TEST(CodegenTest, ImageLoad_Codegen) {

  /********************* Common variables **********************/
  std::string in;
  std::string reg1Name, reg2Name, reg3Name, reg4Name, reg5Name;
  StringBuffer* sbuf = new StringBuffer();

  BrigInstImage out;
  BrigOperandReg reg1, reg2, reg3, reg4;
  BrigOperandRegV4 dest, srcRegV4;
  BrigOperandRegV2 srcRegV2;
  BrigOperandReg destList[4], srcList[4];
  BrigOperandOpaque image1;
  

  /**************************** Case 1 ***************************/
  in.assign("ld_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5,$s2,$s3);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigLdImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_2da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;
  srcList[2] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  srcList[3] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  srcList[0] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;
  
  srcRegV4.size = sizeof(srcRegV4);
  srcRegV4.kind = BrigEOperandRegV4;
  srcRegV4.type = Brigb32;
  srcRegV4.reserved = 0;
  srcRegV4.regs[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  srcRegV4.regs[1] = sizeof(reg1) + sizeof(reg2) + 
                      sizeof(reg3) + sizeof(reg4) + 
                      sizeof(dest) + sizeof(image1);
  srcRegV4.regs[2] = sizeof(reg1);
  srcRegV4.regs[3] = sizeof(reg1) + sizeof(reg2);


  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  srcList[1] = reg4;

  ImageInst_Test<BrigInstImage, BrigOperandRegV4> 
  TestCase1(in, sbuf, &out, &dest, destList, &image1, NULL, &srcRegV4, srcList);
  TestCase1.Run_Test(&ImageLoad);
  
  sbuf->clear();

  /**************************** Case 2 ***************************/
  in.assign("ld_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigLdImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  srcList[0] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;
  
  srcRegV2.size = sizeof(srcRegV2);
  srcRegV2.kind = BrigEOperandRegV2;
  srcRegV2.type = Brigb32;
  srcRegV2.reserved = 0;
  srcRegV2.regs[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  srcRegV2.regs[1] = sizeof(reg1) + sizeof(reg2) + 
                      sizeof(reg3) + sizeof(reg4) + 
                      sizeof(dest) + sizeof(image1);

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  srcList[1] = reg4;

  ImageInst_Test<BrigInstImage, BrigOperandRegV2> 
  TestCase2(in, sbuf, &out, &dest, destList, &image1, NULL, &srcRegV2, srcList);
  TestCase2.Run_Test(&ImageLoad);
  
  sbuf->clear();

  /**************************** Case 3 ***************************/
  in.assign("ld_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3<$s4>], ($s5);\n");
  reg1Name.assign("$s1");  reg2Name.assign("$s2");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstImage;
  out.opcode = BrigLdImage;
  out.type = Brigf32;
  out.stype = Brigu32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1db;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  image1.offset = 0;
  
  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  ImageInst_Test<BrigInstImage, BrigOperandReg> 
  TestCase3(in, sbuf, &out, &dest, destList, &image1, NULL, &reg2);
  TestCase3.Run_Test(&ImageLoad);
  
  sbuf->clear();

  /***********************  End of tests *************************/
  delete sbuf;
}

/*********************** ImageRead Test ***************************/
TEST(CodegenTest, ImageRead_Codegen) {

  /********************* Common variables **********************/
  std::string in;
  std::string reg1Name, reg2Name, reg3Name, reg4Name, reg5Name, reg6Name;
  StringBuffer* sbuf = new StringBuffer();

  BrigInstRead out;
  BrigOperandReg reg1, reg2, reg3, reg4, reg5;
  BrigOperandRegV4 dest, srcRegV4;
  BrigOperandRegV2 srcRegV2;
  BrigOperandReg destList[4], srcList[4];
  BrigOperandOpaque image1, samp2;
  

  /**************************** Case 1 ***************************/
  in.assign("rd_image_v4_2da_s32_f32 ($s0,$s1,$s3,$s4), [%RWImg3],[%Samp3],($s5, $s6, $s4, $s3);\n");
  reg1Name.assign("$s0");  reg2Name.assign("$s1");
  reg3Name.assign("$s3");  reg4Name.assign("$s4");
  reg5Name.assign("$s5");  reg6Name.assign("$s6");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);  sbuf->append(reg6Name);

  out.size = sizeof(out);
  out.kind = BrigEInstRead;
  out.opcode = BrigRdImage;
  out.type = Brigs32;
  out.stype = Brigf32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_2da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;
  srcList[2] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  srcList[3] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  srcList[2] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  samp2.size = sizeof(samp2);
  samp2.kind = BrigEOperandOpaque;
  samp2.directive = 0;
  samp2.reg = 0;
  samp2.offset = 0;
  
  srcRegV4.size = sizeof(srcRegV4);
  srcRegV4.kind = BrigEOperandRegV4;
  srcRegV4.type = Brigb32;
  srcRegV4.reserved = 0;
  srcRegV4.regs[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  srcRegV4.regs[1] = sizeof(reg1) + sizeof(reg2) + 
                      sizeof(reg3) + sizeof(reg4) + 
                      sizeof(dest) + sizeof(image1);
  srcRegV4.regs[2] = sizeof(reg1);
  srcRegV4.regs[3] = sizeof(reg1) + sizeof(reg2);


  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;
  srcList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 
                reg5Name.size() + 5;
  srcList[1] = reg2;

  ImageInst_Test<BrigInstRead, BrigOperandRegV4> 
  TestCase1(in, sbuf, &out, &dest, destList, &image1, &samp2, &srcRegV4, srcList);
  TestCase1.Run_Test(&ImageRead);
  
  sbuf->clear();

  /**************************** Case 2 ***************************/
  in.assign("rd_image_v4_1da_s32_f32 ($s0,$s1,$s2,$s3), [%RWImg3<$s5 + 3>],[%Samp3<$s6>],($s0, $s1);\n");
  reg1Name.assign("$s0");  reg2Name.assign("$s1");
  reg3Name.assign("$s2");  reg4Name.assign("$s3");
  reg5Name.assign("$s5");  reg6Name.assign("$s6");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);  sbuf->append(reg6Name);

  out.size = sizeof(out);
  out.kind = BrigEInstRead;
  out.opcode = BrigRdImage;
  out.type = Brigs32;
  out.stype = Brigf32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg4);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;
  srcList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;
  srcList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) +
               sizeof(reg4) + sizeof(dest); 
  image1.offset = 3;

  samp2.size = sizeof(samp2);
  samp2.kind = BrigEOperandOpaque;
  samp2.directive = 0;
  samp2.reg = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) +
               sizeof(reg4) + sizeof(dest) + sizeof(image1) +
               sizeof(reg5);
  samp2.offset = 0;
  
  
  srcRegV2.size = sizeof(srcRegV2);
  srcRegV2.kind = BrigEOperandRegV2;
  srcRegV2.type = Brigb32;
  srcRegV2.reserved = 0;
  srcRegV2.regs[0] = 0;
  srcRegV2.regs[1] = sizeof(reg1);

  ImageInst_Test<BrigInstRead, BrigOperandRegV2> 
  TestCase2(in, sbuf, &out, &dest, destList, &image1, &samp2, &srcRegV2, srcList);
  TestCase2.Run_Test(&ImageRead);
  
  sbuf->clear();

  /**************************** Case 3 ***************************/
  in.assign("rd_image_v4_1d_s32_f32 ($s0,$s1,$s5,$s3),[%RWImg3], [%Samp3], ($s6);\n");
  reg1Name.assign("$s0");  reg2Name.assign("$s1");
  reg3Name.assign("$s5");  reg4Name.assign("$s3");
  reg5Name.assign("$s6");
  sbuf->append(reg1Name);  sbuf->append(reg2Name);
  sbuf->append(reg3Name);  sbuf->append(reg4Name);
  sbuf->append(reg5Name);

  out.size = sizeof(out);
  out.kind = BrigEInstRead;
  out.opcode = BrigRdImage;
  out.type = Brigs32;
  out.stype = Brigf32;
  out.packing = BrigNoPacking;
  out.reserved = 0;
  out.o_operands[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4);
  out.o_operands[1] = out.o_operands[0] + sizeof(dest);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1);
  out.o_operands[3] = 0;
  out.o_operands[4] = 0;
  out.geom = Briggeom_1d;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;

  dest.size = sizeof(dest);
  dest.kind = BrigEOperandRegV4;
  dest.type = Brigb32;
  dest.reserved = 0;
  dest.regs[0] = 0;
  dest.regs[1] = sizeof(reg1);
  dest.regs[2] = sizeof(reg1) + sizeof(reg2);
  dest.regs[3] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
  
  image1.size = sizeof(image1);
  image1.kind = BrigEOperandOpaque;
  image1.directive = 0;
  image1.reg = 0;
  image1.offset = 0;

  samp2.size = sizeof(samp2);
  samp2.kind = BrigEOperandOpaque;
  samp2.directive = 0;
  samp2.reg = 0;
  samp2.offset = 0;
  
  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  ImageInst_Test<BrigInstRead, BrigOperandReg> 
  TestCase3(in, sbuf, &out, &dest, destList, &image1, &samp2, &reg3);
  TestCase3.Run_Test(&ImageRead);
  
  sbuf->clear();

  /***********************  End of tests *************************/
  delete sbuf;
}

}
}
