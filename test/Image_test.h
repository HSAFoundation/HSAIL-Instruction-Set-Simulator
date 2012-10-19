#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename Tinst, typename T> class Image_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const Tinst* RefInst;
  
  //Dest Operand
  const BrigOperandRegV4* RefDest;
  //Source operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandOpaque* RefOpaque1;
  const T* RefSrc;
  const BrigOperandOpaque* RefOpaque2;
  
public:
 
  Image_Test(std::string& in, StringBuffer* sbuf, Tinst* ref, BrigOperandRegV4* dest, BrigOperandOpaque* opaque1, T* src,    
  BrigOperandOpaque* opaque2 = NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref), 
    RefDest(dest),
    RefOpaque1(opaque1),
    RefSrc(src),
    RefOpaque2(opaque2) { }
    
  void validate(struct BrigSections* TestOutput){   

    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;
    uint32_t opCount = 0;
    inst_iterator getcode = TestOutput->code_begin();
    const Tinst* getinst = (cast<Tinst>(getcode));
    validate_brig::validate(RefInst, getinst);
   
    const BrigOperandRegV4* getregV4 = reinterpret_cast <const BrigOperandRegV4*> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    const BrigOperandOpaque* getopaque1 = reinterpret_cast <const BrigOperandOpaque*> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    
   
    validate_brig::validate(RefDest, getregV4);
    validate_brig::validate(RefOpaque1, getopaque1);

    if (RefOpaque2 != NULL) {
      const BrigOperandOpaque* getopaque2 = reinterpret_cast <const BrigOperandOpaque*> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
      validate_brig::validate(RefOpaque2, getopaque2);
    } 
    const T* getsrc = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validateOpType<T>(RefSrc, refbuf, getsrc, getbuf);
    while (opCount <= 4) {
      EXPECT_EQ(0, getinst->o_operands[opCount++]);       
    }
  }
};

/*********************** ImageRead Test ***************************/
TEST(CodegenTest, ImageRead_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "rd_image_v4_1d_s32_f32 ($s0,$s1,$s5,$s3), [%RWImg3], ($s6);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s5"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);

  BrigOperandReg regv1_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_1.size = sizeof(regv1_1);

  BrigOperandReg regv2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_1.size = sizeof(regv2_1);

  BrigOperandReg regv3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_1.size = sizeof(regv3_1);

  BrigOperandReg regv4_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_1.size = sizeof(regv4_1);

  BrigOperandRegV4 dest1 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_1), sizeof(regv1_1) + sizeof(regv2_1), 
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1)}
  };
  dest1.size = sizeof(dest1);

  BrigOperandOpaque src1 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src1.size = sizeof(src1);

  BrigOperandReg reg1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  reg1.size = sizeof(reg1);

  BrigInstRead out1 = {
    0,
    BrigEInstRead,
    BrigRdImage,
    {sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1) + sizeof(src1),
     0, 0},
    Briggeom_1d,
    Brigf32,
    Brigs32,
    BrigNoPacking,
    0
  };
  out1.size = sizeof(out1); 
 
  Image_Test<BrigInstRead, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1, &src1, &reg1);
  TestCase1.Run_Test(&ImageRead);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "rd_image_v4_1da_s32_f32 ($s0,$s1,$s2,$s3), [%RWImg3<$s2 + 32>],($s6, $s5);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s5"); sbuf->append(op2);

  BrigOperandReg regv1_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_2_1.size = sizeof(regv1_2_1);

  BrigOperandReg regv2_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_2_1.size = sizeof(regv2_2_1);

  BrigOperandReg regv3_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_2_1.size = sizeof(regv3_2_1);

  BrigOperandReg regv4_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_2_1.size = sizeof(regv4_2_1);

  BrigOperandRegV4 dest2 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_2_1), sizeof(regv1_2_1) + sizeof(regv2_2_1), 
     sizeof(regv1_2_1) + sizeof(regv2_2_1) + sizeof(regv3_2_1)}
  };
  dest2.size = sizeof(dest2);

  BrigOperandOpaque src2 = {
    0,
    BrigEOperandOpaque,
    0,
    2 * (op1.size() + 1),
    32
  };
  src2.size = sizeof(src2);

  BrigOperandReg regv1_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_2_2.size = sizeof(regv1_2_2);

  BrigOperandReg regv2_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_2_2.size = sizeof(regv2_2_2);

  BrigOperandRegV2 regV2_2 = {
    0,
    BrigEOperandRegV2,
    Brigb32,
    0,
    {4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2), 
     5 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2)}
  };
  regV2_2.size = sizeof(regV2_2);

  BrigInstRead out2 = {
    0,
    BrigEInstRead,
    BrigRdImage,
    {4 * sizeof(regv1_2_1),
     4 * sizeof(regv1_2_1) + sizeof(dest2),
     4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2) + 2 * sizeof(regv1_2_2),
     0, 0},
    Briggeom_1da,
    Brigf32,
    Brigs32,
    BrigNoPacking,
    0
  };
  out2.size = sizeof(out2); 
 
  Image_Test<BrigInstRead, BrigOperandRegV2> TestCase2(in, sbuf, &out2, &dest2, &src2, &regV2_2);
  TestCase2.Run_Test(&ImageRead);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "rd_image_v4_2da_s32_f32 ($s0,$s1,$s3,$s4), [%RWImg3], ($s5, $s6, $s4, $s3);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s5"); sbuf->append(op2);
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s4"); sbuf->append(op2);
  op2.assign("$s3"); sbuf->append(op2);

  BrigOperandReg regv1_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_3_1.size = sizeof(regv1_3_1);

  BrigOperandReg regv2_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_3_1.size = sizeof(regv2_3_1);

  BrigOperandReg regv3_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_3_1.size = sizeof(regv3_3_1);

  BrigOperandReg regv4_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_3_1.size = sizeof(regv4_3_1);

  BrigOperandRegV4 dest3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_3_1), sizeof(regv1_3_1) + sizeof(regv2_3_1), 
     sizeof(regv1_3_1) + sizeof(regv2_3_1) + sizeof(regv3_3_1)}
  };
  dest3.size = sizeof(dest3);

  BrigOperandOpaque src3 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src3.size = sizeof(src3);

  BrigOperandReg regv1_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_3_2.size = sizeof(regv1_3_2);

  BrigOperandReg regv2_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_3_2.size = sizeof(regv2_3_2);

  BrigOperandReg regv3_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    6 * (op1.size() + 1)
  };
  regv3_3_2.size = sizeof(regv3_3_2);

  BrigOperandReg regv4_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    7 * (op1.size() + 1)
  };
  regv4_3_2.size = sizeof(regv4_3_2);

  BrigOperandRegV4 regV4_3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {4 * sizeof(regv1_3_1), 5 * sizeof(regv1_3_1), 6 * sizeof(regv1_3_1), 
     7 * sizeof(regv1_3_1)}
  };
  regV4_3.size = sizeof(regV4_3);

  BrigInstRead out3 = {
    0,
    BrigEInstRead,
    BrigRdImage,
    {4 * sizeof(regv1_3_1),
     4 * sizeof(regv1_3_1) + sizeof(dest3),
     4 * sizeof(regv1_3_1) + sizeof(dest3) + sizeof(src3) + 4 * sizeof(regv1_3_1),
     0, 0},
    Briggeom_2da,
    Brigf32,
    Brigs32,
    BrigNoPacking,
    0
  };
  out3.size = sizeof(out3); 
 
  Image_Test<BrigInstRead, BrigOperandRegV4> TestCase3(in, sbuf, &out3, &dest3, &src3, &regV4_3);
  TestCase3.Run_Test(&ImageRead);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "rd_image_v4_1d_s32_f32 ($s0,$s1,$s5,$s3), [%RWImg3], [%Samp3], ($s6);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s5"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);

  BrigOperandReg regv1_4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_4.size = sizeof(regv1_4);

  BrigOperandReg regv2_4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_4.size = sizeof(regv2_4);

  BrigOperandReg regv3_4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_4.size = sizeof(regv3_4);

  BrigOperandReg regv4_4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_4.size = sizeof(regv4_4);

  BrigOperandRegV4 dest4 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_4), sizeof(regv1_4) + sizeof(regv2_4), 
     sizeof(regv1_4) + sizeof(regv2_4) + sizeof(regv3_4)}
  };
  dest4.size = sizeof(dest4);

  BrigOperandOpaque src4_1 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src4_1.size = sizeof(src4_1);

  BrigOperandOpaque src4_2 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src4_2.size = sizeof(src4_2);

  BrigOperandReg reg4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  reg4.size = sizeof(reg4);

  BrigInstRead out4 = {
    0,
    BrigEInstRead,
    BrigRdImage,
    {sizeof(regv1_4) + sizeof(regv2_4) + sizeof(regv3_4) + sizeof(regv4_4),
     sizeof(regv1_4) + sizeof(regv2_4) + sizeof(regv3_4) + sizeof(regv4_4) + sizeof(dest4),
     sizeof(regv1_4) + sizeof(regv2_4) + sizeof(regv3_4) + sizeof(regv4_4) + sizeof(dest4) + sizeof(src4_1),
     sizeof(regv1_4) + sizeof(regv2_4) + sizeof(regv3_4) + sizeof(regv4_4) + sizeof(dest4) + sizeof(src4_1) + + sizeof(src4_2), 0},
    Briggeom_1d,
    Brigf32,
    Brigs32,
    BrigNoPacking,
    0
  };
  out4.size = sizeof(out4); 
 
  Image_Test<BrigInstRead, BrigOperandReg> TestCase4(in, sbuf, &out4, &dest4, &src4_1, &reg4, &src4_2);
  TestCase4.Run_Test(&ImageRead);  
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

/*********************** ImageLoad Test ***************************/
TEST(CodegenTest, ImageLoad_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "ld_image_v4_1d_s32_u32 ($s0,$s1,$s5,$s3), [%RWImg3], ($s6);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s5"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);

  BrigOperandReg regv1_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_1.size = sizeof(regv1_1);

  BrigOperandReg regv2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_1.size = sizeof(regv2_1);

  BrigOperandReg regv3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_1.size = sizeof(regv3_1);

  BrigOperandReg regv4_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_1.size = sizeof(regv4_1);

  BrigOperandRegV4 dest1 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_1), sizeof(regv1_1) + sizeof(regv2_1), 
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1)}
  };
  dest1.size = sizeof(dest1);

  BrigOperandOpaque src1 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src1.size = sizeof(src1);

  BrigOperandReg reg = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  reg.size = sizeof(reg);

  BrigInstImage out1 = {
    0,
    BrigEInstImage,
    BrigLdImage,
    {sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1) + sizeof(src1),
     0, 0},
    Briggeom_1d,
    Brigs32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out1.size = sizeof(out1); 
 
  Image_Test<BrigInstImage, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1, &src1, &reg);
  TestCase1.Run_Test(&ImageLoad);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_image_v4_1da_f32_u32 ($s0,$s1,$s2,$s3), [%RWImg3<$s2 + 32>],($s6, $s5);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s5"); sbuf->append(op2);

  BrigOperandReg regv1_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_2_1.size = sizeof(regv1_2_1);

  BrigOperandReg regv2_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_2_1.size = sizeof(regv2_2_1);

  BrigOperandReg regv3_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_2_1.size = sizeof(regv3_2_1);

  BrigOperandReg regv4_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_2_1.size = sizeof(regv4_2_1);

  BrigOperandRegV4 dest2 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_2_1), sizeof(regv1_2_1) + sizeof(regv2_2_1), 
     sizeof(regv1_2_1) + sizeof(regv2_2_1) + sizeof(regv3_2_1)}
  };
  dest2.size = sizeof(dest2);

  BrigOperandOpaque src2 = {
    0,
    BrigEOperandOpaque,
    0,
    2 * (op1.size() + 1),
    32
  };
  src2.size = sizeof(src2);

  BrigOperandReg regv1_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_2_2.size = sizeof(regv1_2_2);

  BrigOperandReg regv2_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_2_2.size = sizeof(regv2_2_2);

  BrigOperandRegV2 regV2_2 = {
    0,
    BrigEOperandRegV2,
    Brigb32,
    0,
    {4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2), 
     5 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2)}
  };
  regV2_2.size = sizeof(regV2_2);

  BrigInstImage out2 = {
    0,
    BrigEInstImage,
    BrigLdImage,
    {4 * sizeof(regv1_2_1),
     4 * sizeof(regv1_2_1) + sizeof(dest2),
     4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2) + 2 * sizeof(regv1_2_2),
     0, 0},
    Briggeom_1da,
    Brigf32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out2.size = sizeof(out2); 
 
  Image_Test<BrigInstImage, BrigOperandRegV2> TestCase2(in, sbuf, &out2, &dest2, &src2, &regV2_2);
  TestCase2.Run_Test(&ImageLoad);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_image_v4_2da_s32_u32 ($s0,$s1,$s3,$s4), [%RWImg3], ($s5, $s6, $s4, $s3);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s5"); sbuf->append(op2);
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s4"); sbuf->append(op2);
  op2.assign("$s3"); sbuf->append(op2);

  BrigOperandReg regv1_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_3_1.size = sizeof(regv1_3_1);

  BrigOperandReg regv2_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_3_1.size = sizeof(regv2_3_1);

  BrigOperandReg regv3_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_3_1.size = sizeof(regv3_3_1);

  BrigOperandReg regv4_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_3_1.size = sizeof(regv4_3_1);

  BrigOperandRegV4 dest3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_3_1), sizeof(regv1_3_1) + sizeof(regv2_3_1), 
     sizeof(regv1_3_1) + sizeof(regv2_3_1) + sizeof(regv3_3_1)}
  };
  dest3.size = sizeof(dest3);

  BrigOperandOpaque src3 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src3.size = sizeof(src3);

  BrigOperandReg regv1_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_3_2.size = sizeof(regv1_3_2);

  BrigOperandReg regv2_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_3_2.size = sizeof(regv2_3_2);

  BrigOperandReg regv3_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    6 * (op1.size() + 1)
  };
  regv3_3_2.size = sizeof(regv3_3_2);

  BrigOperandReg regv4_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    7 * (op1.size() + 1)
  };
  regv4_3_2.size = sizeof(regv4_3_2);

  BrigOperandRegV4 regV4_3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {4 * sizeof(regv1_3_1), 5 * sizeof(regv1_3_1), 6 * sizeof(regv1_3_1), 
     7 * sizeof(regv1_3_1)}
  };
  regV4_3.size = sizeof(regV4_3);

  BrigInstImage out3 = {
    0,
    BrigEInstImage,
    BrigLdImage,
    {4 * sizeof(regv1_3_1),
     4 * sizeof(regv1_3_1) + sizeof(dest3),
     4 * sizeof(regv1_3_1) + sizeof(dest3) + sizeof(src3) + 4 * sizeof(regv1_3_1),
     0, 0},
    Briggeom_2da,
    Brigs32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out3.size = sizeof(out3); 
 
  Image_Test<BrigInstImage, BrigOperandRegV4> TestCase3(in, sbuf, &out3, &dest3, &src3, &regV4_3);
  TestCase3.Run_Test(&ImageLoad);  
  sbuf->clear();

/******************************  End of tests *****************************************/
  delete sbuf;
}

/*********************** ImageLoad Test ***************************/
TEST(CodegenTest, ImageStore_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "st_image_v4_1d_s32_u32 ($s0,$s1,$s5,$s3), [%RWImg3], ($s6);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s5"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);

  BrigOperandReg regv1_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_1.size = sizeof(regv1_1);

  BrigOperandReg regv2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_1.size = sizeof(regv2_1);

  BrigOperandReg regv3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_1.size = sizeof(regv3_1);

  BrigOperandReg regv4_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_1.size = sizeof(regv4_1);

  BrigOperandRegV4 dest1 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_1), sizeof(regv1_1) + sizeof(regv2_1), 
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1)}
  };
  dest1.size = sizeof(dest1);

  BrigOperandOpaque src1 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src1.size = sizeof(src1);

  BrigOperandReg reg = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  reg.size = sizeof(reg);

  BrigInstImage out1 = {
    0,
    BrigEInstImage,
    BrigStImage,
    {sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1),
     sizeof(regv1_1) + sizeof(regv2_1) + sizeof(regv3_1) + sizeof(regv4_1) + sizeof(dest1) + sizeof(src1),
     0, 0},
    Briggeom_1d,
    Brigs32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out1.size = sizeof(out1); 
 
  Image_Test<BrigInstImage, BrigOperandReg> TestCase1(in, sbuf, &out1, &dest1, &src1, &reg);
  TestCase1.Run_Test(&ImageStore);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "st_image_v4_1da_f32_u32 ($s0,$s1,$s2,$s3), [%RWImg3<$s2 + 32>],($s6, $s5);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s5"); sbuf->append(op2);

  BrigOperandReg regv1_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_2_1.size = sizeof(regv1_2_1);

  BrigOperandReg regv2_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_2_1.size = sizeof(regv2_2_1);

  BrigOperandReg regv3_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_2_1.size = sizeof(regv3_2_1);

  BrigOperandReg regv4_2_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_2_1.size = sizeof(regv4_2_1);

  BrigOperandRegV4 dest2 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_2_1), sizeof(regv1_2_1) + sizeof(regv2_2_1), 
     sizeof(regv1_2_1) + sizeof(regv2_2_1) + sizeof(regv3_2_1)}
  };
  dest2.size = sizeof(dest2);

  BrigOperandOpaque src2 = {
    0,
    BrigEOperandOpaque,
    0,
    2 * (op1.size() + 1),
    32
  };
  src2.size = sizeof(src2);

  BrigOperandReg regv1_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_2_2.size = sizeof(regv1_2_2);

  BrigOperandReg regv2_2_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_2_2.size = sizeof(regv2_2_2);

  BrigOperandRegV2 regV2_2 = {
    0,
    BrigEOperandRegV2,
    Brigb32,
    0,
    {4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2), 
     5 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2)}
  };
  regV2_2.size = sizeof(regV2_2);

  BrigInstImage out2 = {
    0,
    BrigEInstImage,
    BrigStImage,
    {4 * sizeof(regv1_2_1),
     4 * sizeof(regv1_2_1) + sizeof(dest2),
     4 * sizeof(regv1_2_1) + sizeof(dest2) + sizeof(src2) + 2 * sizeof(regv1_2_2),
     0, 0},
    Briggeom_1da,
    Brigf32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out2.size = sizeof(out2); 
 
  Image_Test<BrigInstImage, BrigOperandRegV2> TestCase2(in, sbuf, &out2, &dest2, &src2, &regV2_2);
  TestCase2.Run_Test(&ImageStore);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "st_image_v4_2da_s32_u32 ($s0,$s1,$s3,$s4), [%RWImg3], ($s5, $s6, $s4, $s3);\n");
  op1.assign("$s0"); sbuf->append(op1); 
  op1.assign("$s1"); sbuf->append(op1); 
  op1.assign("$s3"); sbuf->append(op1); 
  op1.assign("$s4"); sbuf->append(op1); 
  op2.assign("$s5"); sbuf->append(op2);
  op2.assign("$s6"); sbuf->append(op2);
  op2.assign("$s4"); sbuf->append(op2);
  op2.assign("$s3"); sbuf->append(op2);

  BrigOperandReg regv1_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  regv1_3_1.size = sizeof(regv1_3_1);

  BrigOperandReg regv2_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  regv2_3_1.size = sizeof(regv2_3_1);

  BrigOperandReg regv3_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    2 * (op1.size() + 1)
  };
  regv3_3_1.size = sizeof(regv3_3_1);

  BrigOperandReg regv4_3_1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    3 * (op1.size() + 1)
  };
  regv4_3_1.size = sizeof(regv4_3_1);

  BrigOperandRegV4 dest3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {0, sizeof(regv1_3_1), sizeof(regv1_3_1) + sizeof(regv2_3_1), 
     sizeof(regv1_3_1) + sizeof(regv2_3_1) + sizeof(regv3_3_1)}
  };
  dest3.size = sizeof(dest3);

  BrigOperandOpaque src3 = {
    0,
    BrigEOperandOpaque,
    0,
    0,
    0
  };
  src3.size = sizeof(src3);

  BrigOperandReg regv1_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    4 * (op1.size() + 1)
  };
  regv1_3_2.size = sizeof(regv1_3_2);

  BrigOperandReg regv2_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    5 * (op1.size() + 1)
  };
  regv2_3_2.size = sizeof(regv2_3_2);

  BrigOperandReg regv3_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    6 * (op1.size() + 1)
  };
  regv3_3_2.size = sizeof(regv3_3_2);

  BrigOperandReg regv4_3_2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    7 * (op1.size() + 1)
  };
  regv4_3_2.size = sizeof(regv4_3_2);

  BrigOperandRegV4 regV4_3 = {
    0,
    BrigEOperandRegV4,
    Brigb32,
    0,
    {4 * sizeof(regv1_3_1), 5 * sizeof(regv1_3_1), 6 * sizeof(regv1_3_1), 
     7 * sizeof(regv1_3_1)}
  };
  regV4_3.size = sizeof(regV4_3);

  BrigInstImage out3 = {
    0,
    BrigEInstImage,
    BrigStImage,
    {4 * sizeof(regv1_3_1),
     4 * sizeof(regv1_3_1) + sizeof(dest3),
     4 * sizeof(regv1_3_1) + sizeof(dest3) + sizeof(src3) + 4 * sizeof(regv1_3_1),
     0, 0},
    Briggeom_2da,
    Brigs32,
    Brigu32,
    BrigNoPacking,
    0
  };
  out3.size = sizeof(out3); 
 
  Image_Test<BrigInstImage, BrigOperandRegV4> TestCase3(in, sbuf, &out3, &dest3, &src3, &regV4_3);
  TestCase3.Run_Test(&ImageStore);  
  sbuf->clear();


/******************************  End of tests *****************************************/
  delete sbuf;
}

}
}
