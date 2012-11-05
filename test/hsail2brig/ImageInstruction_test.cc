#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"


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

  const BrigOperandReg* ImgReg;
  const BrigOperandReg* SmpReg;

public:
  ImageInst_Test(std::string& input, StringBuffer* sbuf, TInst* ref,
                 BrigOperandRegV4* dest, BrigOperandReg* destList, 
                 BrigOperandOpaque* image, BrigOperandOpaque* sample, 
                 TSrc* src, BrigOperandReg* srcList = NULL, 
                 BrigOperandReg* imgReg = NULL, BrigOperandReg* smpReg = NULL):
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefDest(dest),
    RefDestList(destList),
    RefImage(image),
    RefSample(sample),
    RefSrc(src),
    RefSrcList(srcList) ,
    ImgReg(imgReg),
    SmpReg(smpReg) { }

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();

    code->append(RefInst);

    for (uint32_t i = 0 ; i < 4 ; ++i) {
      oper->append(&RefDestList[i]);
    }

    oper->append(RefDest);
   
    if (ImgReg != NULL) {
      oper->append(ImgReg);
    }

    oper->append(RefImage);

    if (RefSample != NULL) {
      if (SmpReg != NULL) {
        oper->append(SmpReg);
      }
      oper->append(RefSample);
    }

    if (RefSrcList != NULL) {
      if (RefSrc->kind == BrigEOperandRegV2) {
        for (uint32_t i = 0 ; i < 2 ; ++i) {
          // If the structure is empty in srcList, the reg will be generated in other places.
          if (RefSrcList[i].size != 0) {
            oper->append(&RefSrcList[i]);
          }
        }
      } else if (RefSrc->kind == BrigEOperandRegV4) {
        for (uint32_t i = 0 ; i < 4 ; ++i) {
          // If the structure is empty in srcList, the reg will be generated in other places.
          if (RefSrcList[i].size != 0) {
            oper->append(&RefSrcList[i]);
          }
        }
      }
    }

    oper->append(RefSrc);

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
  // If the structure is empty in srcList, the reg will be generated in other places.
  memset(&srcList[0], 0, sizeof(srcList[0]));

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  // If the structure is empty in srcList, the reg will be generated in other places.
  memset(&srcList[1], 0, sizeof(srcList[1]));

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  // If the structure is empty in srcList, the reg will be generated in other places.
  memset(&srcList[2], 0, sizeof(srcList[2]));

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
  // If the structure is empty in srcList, the reg will be generated in other places.
  memset(&srcList[0], 0, sizeof(srcList[0]));

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
  memset(&srcList[2], 0, sizeof(srcList[2]));

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  memset(&srcList[3], 0, sizeof(srcList[3]));

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  memset(&srcList[0], 0, sizeof(srcList[0]));

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
  memset(&srcList[0], 0, sizeof(srcList[0]));

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
  BrigOperandReg reg1, reg2, reg3, reg4, reg5, reg6;
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
  out.o_operands[2] = out.o_operands[1] + sizeof(image1);
  out.o_operands[3] = out.o_operands[2] + sizeof(samp2) + sizeof(reg5) + sizeof(reg6);
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

  reg3.size = sizeof(reg3);
  reg3.kind = BrigEOperandReg;
  reg3.type = Brigb32;
  reg3.reserved = 0;
  reg3.s_name = reg1Name.size() + reg2Name.size() + 2;
  
  destList[2] = reg3;
  memset(&srcList[3], 0, sizeof(srcList[3]));

  reg4.size = sizeof(reg4);
  reg4.kind = BrigEOperandReg;
  reg4.type = Brigb32;
  reg4.reserved = 0;
  reg4.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + 3;

  destList[3] = reg4;
  memset(&srcList[2], 0, sizeof(srcList[2]));

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
  srcRegV4.regs[0] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3) + sizeof(reg4) +
                     sizeof(dest) + sizeof(image1) + sizeof(samp2);
  srcRegV4.regs[1] = srcRegV4.regs[0] + sizeof(reg4);
  srcRegV4.regs[2] = sizeof(reg1) + sizeof(reg2) + sizeof(reg3);
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
  out.o_operands[1] = out.o_operands[0] + sizeof(dest) + sizeof(reg5);
  out.o_operands[2] = out.o_operands[1] + sizeof(image1) + sizeof(reg6);
  out.o_operands[3] = out.o_operands[2] + sizeof(samp2);
  out.o_operands[4] = 0;
  out.geom = Briggeom_1da;

  reg1.size = sizeof(reg1);
  reg1.kind = BrigEOperandReg;
  reg1.type = Brigb32;
  reg1.reserved = 0;
  reg1.s_name = 0;

  destList[0] = reg1;
  memset(&srcList[0], 0 , sizeof(srcList[0]));

  reg2.size = sizeof(reg2);
  reg2.kind = BrigEOperandReg;
  reg2.type = Brigb32;
  reg2.reserved = 0;
  reg2.s_name = reg1Name.size() + 1;

  destList[1] = reg2;
  memset(&srcList[1], 0, sizeof(srcList[1]));

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

  reg5.size = sizeof(reg5);
  reg5.kind = BrigEOperandReg;
  reg5.type = Brigb32;
  reg5.reserved = 0;
  reg5.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 4;

  reg6.size = sizeof(reg6);
  reg6.kind = BrigEOperandReg;
  reg6.type = Brigb32;
  reg6.reserved = 0;
  reg6.s_name = reg1Name.size() + reg2Name.size() +
                reg3Name.size() + reg4Name.size() + 
                reg5Name.size() + 5;


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
  TestCase2(in, sbuf, &out, &dest, destList, &image1, &samp2, &srcRegV2, srcList, &reg5, &reg6);
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
  out.o_operands[3] = out.o_operands[2] + sizeof(samp2);
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
