/* Copyright 2012 <MulticorewareInc> */

#ifndef IMAGE_TEST_H_
#define IMAGE_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestLdImage : public ::testing::TestWithParam<int>
{

};
struct LdImageTest{
  const char *str;
  BrigInstImage ref;
};

struct LdImageTest imageload_pair[12] = {
// imageload_pair[0]
  {
    "ld_image_v4_1d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1d,
      Brigu32, 
       0
    }
  },
// imageload_pair[1]
  {
    "ld_image_v4_1d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1d,
      Brigu32, 
      0
    }
  },
// imageload_pair[2]
  {
    "ld_image_v4_2d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2d,
      Brigu32, 
      0
    }
  },
// imageload_pair[3]
  {
    "ld_image_v4_2d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2d,
      Brigu32, 
      0
    }
  },
// imageload_pair[4]
  {
    "ld_image_v4_3d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_3d,
      Brigu32, 
      0
    }
  },
// imageload_pair[5]
  {
    "ld_image_v4_3d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_3d,
      Brigu32, 
      0
    }
  },
// imageload_pair[6]
  {
    "ld_image_v4_1da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1da,
      Brigu32, 
      0
    }
  },
// imageload_pair[7]
  {
    "ld_image_v4_1da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1da,
      Brigu32, 
       0
    }
  },
// imageload_pair[8]
  {
    "ld_image_v4_2da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2da,
      Brigu32, 
      0
    }
  },
// imageload_pair[9]
  {
    "ld_image_v4_2da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2da,
      Brigu32, 
      0
    }
  },
// imageload_pair[10]
  {
    "ld_image_v4_1db_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1db,
      Brigu32, 
      0
    }
  },
// imageload_pair[11]
  {
    "ld_image_v4_1db_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigLdImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1db,
      Brigu32, 
      0
    }
  }
};

class TestLdImageInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageload_invalid[60] = {
//false[0]
  "ld_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "ld_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "ld_image_v4_1d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[3]
  "ld_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[4]
  "ld_image_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5),;",
//false[5]
  "ld_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5)",
//false[6]
  "ld_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[7]
  "ld_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[8]
  "ld_image_v4_1d_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[9]
  "ld_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);",
//false[10]
  "ld_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s500);",
//false[11]
  "ld_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, ($s5, $s6);",
//false[12]
  "ld_image_v4_2d_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[13]
  "ld_image_v4_2d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[14]
  "ld_image_v4_2d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], (s5);",
//false[15]
  "ld_image_v2_2d_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, ($s5, $s6);",
//false[16]
  "ld_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], (100);",
//false[17]
  "ld_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6);",
//false[18]
  "ldd_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[19]
  "ld_image_v4_2d_f32_u32 $s1, [%RWImg3], $s5, $s6;",
//false[20]
  "ld_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[21]
  "ld_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[22]
  "ld_image_v4_3d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[23]
  "ld_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[24]
  "ld_image_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[25]
  "ld_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[26]
  "ld_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[27]
  "ld_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[28]
  "ld_image_v4_3d_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[29]
  "ld_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5, $s6 $s7, $s8);",
//false[30]
  "ld_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[31]
  "ld_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[32]
  "ld_image_v4_1da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[33]
  "ld_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[34]
  "ld_image_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[35]
  "ld_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[36]
  "ld_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[37]
  "ld_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[38]
  "ld_image_v4_1da_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[39]
  "ld_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);",
//false[40]
  "ld_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[41]
  "ld_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[42]
  "ld_image_v4_2da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[43]
  "ld_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[44]
  "ld_image_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[45]
  "ld_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[46]
  "ld_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[47]
  "ld_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[48]
  "ld_image_v4_2da_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[49]
  "ld_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5, $s6 $s7, $s8);",
//false[50]
  "ld_image_v4_1db_u32_s32 ($s1, $c2, $s3, $s4);",
//false[51]
  "ld_image_v4_1db_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[52]
  "ld_image_v4_1db_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[53]
  "ld_image_v4_1db_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[54]
  "ld_image_v4_1db_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5),;",
//false[55]
  "ld_image_v4_1db_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5)",
//false[56]
  "ld_image_v4_1db_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[57]
  "ld_image_v4_1db_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[58]
  "ld_image_v4_1db_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[59]
  "ld_image_v4_1db_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);"
};

class TestImageNoRetAdd : public ::testing::TestWithParam<int>
{

};
struct ImageNoRetTest{
  const char *str;
  BrigInstAtomicImage ref;
};

struct ImageNoRetTest imagenoret_add_pair[6] = {
//imagenoret_add_pair[0]
  {
    "atomicNoRet_image_add_1d_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_add_pair[1]
  {
    "atomicNoRet_image_add_2d_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_add_pair[2]
  {
    "atomicNoRet_image_add_3d_u64 [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_add_pair[3]
  {
    "atomicNoRet_image_add_acq_1db_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_add_pair[4]
  {
    "atomicNoRet_image_add_ar_1da_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_add_pair[5]
  {
    "atomicNoRet_image_add_2da_u64 [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetSub : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_sub_pair[6] = {
//imagenoret_sub_pair[0]
  {
    "atomicNoRet_image_sub_1d_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_sub_pair[1]
  {
    "atomicNoRet_image_sub_2d_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_sub_pair[2]
  {
    "atomicNoRet_image_sub_3d_u64 [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_sub_pair[3]
  {
    "atomicNoRet_image_sub_acq_1db_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_sub_pair[4]
  {
    "atomicNoRet_image_sub_ar_1da_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_sub_pair[5]
  {
    "atomicNoRet_image_sub_2da_u64 [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};


class TestImageNoRetOr : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_or_pair[6] = {
//imagenoret_or_pair[0]
  {
    "atomicNoRet_image_or_1d_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_or_pair[1]
  {
    "atomicNoRet_image_or_2d_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_or_pair[2]
  {
    "atomicNoRet_image_or_3d_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_or_pair[3]
  {
    "atomicNoRet_image_or_acq_1db_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_or_pair[4]
  {
    "atomicNoRet_image_or_ar_1da_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_or_pair[5]
  {
    "atomicNoRet_image_or_2da_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
       BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetAnd : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_and_pair[6] = {
//imagenoret_and_pair[0]
  {
    "atomicNoRet_image_and_1d_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_and_pair[1]
  {
    "atomicNoRet_image_and_2d_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_and_pair[2]
  {
    "atomicNoRet_image_and_3d_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_and_pair[3]
  {
    "atomicNoRet_image_and_acq_1db_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_and_pair[4]
  {
    "atomicNoRet_image_and_ar_1da_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_and_pair[5]
  {
    "atomicNoRet_image_and_2da_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
       BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetXor : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_xor_pair[6] = {
//imagenoret_xor_pair[0]
  {
    "atomicNoRet_image_xor_1d_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_xor_pair[1]
  {
    "atomicNoRet_image_xor_2d_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_xor_pair[2]
  {
    "atomicNoRet_image_xor_3d_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_xor_pair[3]
  {
    "atomicNoRet_image_xor_acq_1db_b32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_xor_pair[4]
  {
    "atomicNoRet_image_xor_ar_1da_b32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_xor_pair[5]
  {
    "atomicNoRet_image_xor_2da_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetInc : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_inc_pair[6] = {
//imagenoret_inc_pair[0]
  {
    "atomicNoRet_image_inc_1d_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_inc_pair[1]
  {
    "atomicNoRet_image_inc_2d_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_inc_pair[2]
  {
    "atomicNoRet_image_inc_3d_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_inc_pair[3]
  {
    "atomicNoRet_image_inc_acq_1db_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_inc_pair[4]
  {
    "atomicNoRet_image_inc_ar_1da_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_inc_pair[5]
  {
    "atomicNoRet_image_inc_2da_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetDec : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_dec_pair[6] = {
//imagenoret_dec_pair[0]
  {
    "atomicNoRet_image_dec_1d_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_dec_pair[1]
  {
    "atomicNoRet_image_dec_2d_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_dec_pair[2]
  {
    "atomicNoRet_image_dec_3d_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_dec_pair[3]
  {
    "atomicNoRet_image_dec_acq_1db_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_dec_pair[4]
  {
    "atomicNoRet_image_dec_ar_1da_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_dec_pair[5]
  {
    "atomicNoRet_image_dec_2da_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetMax : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_max_pair[6] = {
//imagenoret_max_pair[0]
  {
    "atomicNoRet_image_max_1d_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_max_pair[1]
  {
    "atomicNoRet_image_max_2d_u32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_max_pair[2]
  {
    "atomicNoRet_image_max_3d_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_max_pair[3]
  {
    "atomicNoRet_image_max_acq_1db_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_max_pair[4]
  {
    "atomicNoRet_image_max_ar_1da_s32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_max_pair[5]
  {
    "atomicNoRet_image_max_2da_u32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetMin : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_min_pair[6] = {
//imagenoret_min_pair[0]
  {
    "atomicNoRet_image_min_1d_s32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_min_pair[1]
  {
    "atomicNoRet_image_min_2d_u32 [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_min_pair[2]
  {
    "atomicNoRet_image_min_3d_s32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_min_pair[3]
  {
    "atomicNoRet_image_min_acq_1db_u32 [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_min_pair[4]
  {
    "atomicNoRet_image_min_ar_1da_s32 [&namedRWImg2], ($s2, $s3), 1;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      RoundUp8(operand_offset + reg_size * 2 + opaque_size + regv2_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_min_pair[5]
  {
    "atomicNoRet_image_min_2da_u32 [&namedRWImg2], ($s2, $s3, $s4, $s5), WAVESIZE;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageNoRetCas : public ::testing::TestWithParam<int>
{

};

struct ImageNoRetTest imagenoret_cas_pair[6] = {
//imagenoret_cas_pair[0]
  {
    "atomicNoRet_image_cas_1d_b32 [&namedRWImg2], $s2, $s3, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imagenoret_cas_pair[1]
  {
    "atomicNoRet_image_cas_2d_b32 [&namedRWImg2], ($s2, $s3), $s4, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size + reg_size * 2,
      operand_offset + opaque_size + reg_size * 2 + regv2_size,
      operand_offset + opaque_size + reg_size * 3 + regv2_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imagenoret_cas_pair[2]
  {
    "atomicNoRet_image_cas_3d_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imagenoret_cas_pair[3]
  {
    "atomicNoRet_image_cas_acq_1db_b32 [&namedRWImg2], $s2, $s3, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + opaque_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imagenoret_cas_pair[4]
  {
    "atomicNoRet_image_cas_ar_1da_b32 [&namedRWImg2], ($s2, $s3), $s4, 1;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 2 + opaque_size + regv2_size,
      RoundUp8(operand_offset + reg_size * 3 + opaque_size + regv2_size),
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imagenoret_Cas_pair[5]
  {
    "atomicNoRet_image_cas_2da_b32 [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6, WAVESIZE;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicNoRetImage,
      Brigb32,
       BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size * 4 + opaque_size,
      operand_offset + reg_size * 4 + opaque_size + regv4_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestRdImage1d : public ::testing::TestWithParam<int>
{

};
struct RdImageTest{
  const char *str;
  BrigInstRead ref;
};

struct RdImageTest imageread_1d_pair[6] = {
// imageread_1d_pair[0]
  {
    "rd_image_v4_1d_u32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, // size
      BrigEInstRead, // kind
      BrigRdImage, // opcode
      Brigu32, // type
      BrigNoPacking, //packing
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, // o_operands[5]
      Briggeom_1d, // geom
      Brigu32, // stype
      0 // reserved
    }
  },
// imageread_1d_pair[1]
  {
    "rd_image_v4_1d_u32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigu32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1d, 
      Brigf32,
      0
    }
  },
// imageread_1d_pair[2]
  {
    "rd_image_v4_1d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1d, 
      Brigu32, 
      0
    }
  },
// imageread_1d_pair[3]
  {
    "rd_image_v4_1d_s32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1d, 
      Brigf32, 
      0
    }
  },
// imageread_1d_pair[4]
  {
    "rd_image_v4_1d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1d, 
      Brigu32, 
      0
    }
  },
// imageread_1d_pair[5]
  {
    "rd_image_v4_1d_f32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 4*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1d, 
      Brigf32, 
      0
    }
  }
};

class TestRdImage1dInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageread_1d_invalid[21] = {
//false[0]
  "rd_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "rd_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "rd_image_v4_1d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3];",
//false[3]
  "rd_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],;",
//false[4]
  "rd_image_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5),;",
//false[5]
  "rd_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5)",
//false[6]
  "rd_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], $s5;",
//false[7]
  "rd_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[8]
  "rd_image_v4_1d_s32_u32 ($s1), [%RWImg3], [%Samp3], ($s5);",
//false[9]
  "rd_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],$s5,",
//false[10]
  "rd_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s500);",
//false[11]
  "rd_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, [%Samp3], ($s5);",
//false[12]
  "rd_image_v4_1d_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], %Samp3, ($s5);",
//false[13]
  "rd_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], Samp3, ($s5);",
//false[14]
  "rd_image_v4_1d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
//false[15]
  "rd_image_v2_1d_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, [%Samp3], ($s5);",
//false[16]
  "rd_image_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], (100);",
//false[17]
  "rd_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5);",
//false[18]
  "rdd_image_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
//false[19]
  "rd_imagee_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5);",
//false[20]
  "rd_image_v4_1d_f32_u32 $s1, [%RWImg3], [%Samp3], ($s5);"
};

class TestRdImage2d : public ::testing::TestWithParam<int>
{

};

struct RdImageTest imageread_2d_pair[6] = {
// imageread_2d_pair[0]
  {
    "rd_image_v4_2d_u32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, // size
      BrigEInstRead, // kind
      BrigRdImage, // opcode
      Brigu32, // type
      BrigNoPacking, //packing
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, // o_operands[5]
      Briggeom_2d, // geom
      Brigu32, // stype
      0 // reserved
    }
  },
// imageread_2d_pair[1]
  {
    "rd_image_v4_2d_u32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigu32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2d, 
      Brigf32, 
      0
    }
  },
// imageread_2d_pair[2]
  {
    "rd_image_v4_2d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2d, 
      Brigu32, 
      0
    }
  },
// imageread_2d_pair[3]
  {
    "rd_image_v4_2d_s32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2d, 
      Brigf32, 
      0
    }
  },
// imageread_2d_pair[4]
  {
    "rd_image_v4_2d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2d, 
      Brigu32, 
      0
    }
  },
// imageread_2d_pair[5]
  {
    "rd_image_v4_2d_f32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2d, 
      Brigf32, 
      0
    }
  }
};

class TestRdImage2dInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageread_2d_invalid[21] = {
//false[0]
  "rd_image_v4_2d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "rd_image_v4_2d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "rd_image_v4_2d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3];",
//false[3]
  "rd_image_v4_2d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],;",
//false[4]
  "rd_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6),;",
//false[5]
  "rd_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6)",
//false[6]
  "rd_image_v4_2d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], $s5;",
//false[7]
  "rd_image_v4_2d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[8]
  "rd_image_v4_2d_s32_u32 ($s1), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[9]
  "rd_image_v4_2d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],$s5,",
//false[10]
  "rd_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s500);",
//false[11]
  "rd_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, [%Samp3], ($s5, $s6);",
//false[12]
  "rd_image_v4_2d_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], %Samp3, ($s5, $s6);",
//false[13]
  "rd_image_v4_2d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], Samp3, ($s5, $s6);",
//false[14]
  "rd_image_v4_2d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[15]
  "rd_image_v2_2d_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, [%Samp3], ($s5, $s6);",
//false[16]
  //"rd_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], (100);",
//false[17]
  "rd_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6);",
//false[18]
  "rdd_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[19]
  "rd_imagee_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[20]
  "rd_image_v4_2d_f32_u32 $s1, [%RWImg3], [%Samp3], ($s5, $s6);"
};

class TestRdImage3d : public ::testing::TestWithParam<int>
{

};

struct RdImageTest imageread_3d_pair[6] = {
// imageread_3d_pair[0]
  {
    "rd_image_v4_3d_u32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, // size
      BrigEInstRead, // kind
      BrigRdImage, // opcode
      Brigu32, // type
      BrigNoPacking, //packing
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, // o_operands[5]
      Briggeom_3d, // geom
      Brigu32, // stype
      0 // reserved
    }
  },
// imageread_3d_pair[1]
  {
    "rd_image_v4_3d_u32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigu32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_3d, 
      Brigf32, 
      0
    }
  },
// imageread_3d_pair[2]
  {
    "rd_image_v4_3d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32,  
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_3d, 
      Brigu32,
      0
    }
  },
// imageread_3d_pair[3]
  {
    "rd_image_v4_3d_s32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_3d, 
      Brigf32, 
      0
    }
  },
// imageread_3d_pair[4]
  {
    "rd_image_v4_3d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_3d, 
      Brigu32, 
      0
    }
  },
// imageread_3d_pair[5]
  {
    "rd_image_v4_3d_f32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_3d, 
      Brigf32, 
      0
    }
  }
};

class TestRdImage3dInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageread_3d_invalid[21] = {
//false[0]
  "rd_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "rd_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "rd_image_v4_3d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3];",
//false[3]
  "rd_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],;",
//false[4]
  "rd_image_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8),;",
//false[5]
  "rd_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8)",
//false[6]
  "rd_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], $s5;",
//false[7]
  "rd_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7);",
//false[8]
  "rd_image_v4_3d_s32_u32 ($s1), [%RWImg3], [%Samp3], ($s5);",
//false[9]
  "rd_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], [%Samp3],$s5, $s6,",
//false[10]
  "rd_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s500);",
//false[11]
  "rd_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, [%Samp3], ($s5, $s6, $s7, $s8);",
//false[12]
  "rd_image_v4_3d_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], %Samp3, ($s5, $s6, $s7, $s8);",
//false[13]
  "rd_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], Samp3, ($s5, $s6, $s7, $s8);",
//false[14]
  "rd_image_v4_3d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[15]
  "rd_image_v2_3d_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, [%Samp3], ($s5, $s6, $s7, $s8);",
//false[16]
  //"rd_image_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], (100);",
//false[17]
  "rd_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6, $s7, $s8);",
//false[18]
  "rdd_image_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[19]
  "rd_imagee_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[20]
  "rd_image_v4_3d_f32_u32 $s1, [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);"
};

class TestRdImage1da : public ::testing::TestWithParam<int>
{

};

struct RdImageTest imageread_1da_pair[6] = {
// imageread_1da_pair[0]
  {
    "rd_image_v4_1da_u32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, // size
      BrigEInstRead, // kind
      BrigRdImage, // opcode
      Brigu32, // type
      BrigNoPacking, //packing
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, // o_operands[5]
      Briggeom_1da, // geom
      Brigu32, // stype
      0 // reserved
    }
  },
// imageread_1da_pair[1]
  {
    "rd_image_v4_1da_u32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigu32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1da, 
      Brigf32, 
      0
    }
  },
// imageread_1da_pair[2]
  {
    "rd_image_v4_1da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1da, 
      Brigu32, 
      0
    }
  },
// imageread_1da_pair[3]
  {
    "rd_image_v4_1da_s32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1da, 
      Brigf32, 
      0
    }
  },
// imageread_1da_pair[4]
  {
    "rd_image_v4_1da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1da, 
      Brigu32, 
      0
    }
  },
// imageread_1da_pair[5]
  {
    "rd_image_v4_1da_f32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 6*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_1da, 
      Brigf32, 
      0
    }
  }
};

class TestRdImage1daInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageread_1da_invalid[21] = {
//false[0]
  "rd_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "rd_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "rd_image_v4_1da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3];",
//false[3]
  "rd_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],;",
//false[4]
  "rd_image_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6),;",
//false[5]
  "rd_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6)",
//false[6]
  "rd_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], $s5;",
//false[7]
  "rd_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[8]
  "rd_image_v4_1da_s32_u32 ($s1), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[9]
  "rd_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],$s5,",
//false[10]
  "rd_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s500);",
//false[11]
  "rd_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, [%Samp3], ($s5, $s6);",
//false[12]
  "rd_image_v4_1da_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], %Samp3, ($s5, $s6);",
//false[13]
  "rd_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], Samp3, ($s5, $s6);",
//false[14]
  "rd_image_v4_1da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[15]
  "rd_image_v2_1da_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, [%Samp3], ($s5, $s6);",
//false[16]
  "rd_image_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], (100);",
//false[17]
  "rd_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6);",
//false[18]
  "rdd_image_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[19]
  "rd_imagee_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6);",
//false[20]
  "rd_image_v4_1da_f32_u32 $s1, [%RWImg3], [%Samp3], ($s5, $s6);"
};

class TestRdImage2da : public ::testing::TestWithParam<int>
{

};

struct RdImageTest imageread_2da_pair[6] = {
// imageread_2da_pair[0]
  {
    "rd_image_v4_2da_u32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, // size
      BrigEInstRead, // kind
      BrigRdImage, // opcode
      Brigu32, // type
      BrigNoPacking, //packing
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, // o_operands[5]
      Briggeom_2da, // geom
      Brigu32, // stype
      0 // reserved
    }
  },
// imageread_2da_pair[1]
  {
    "rd_image_v4_2da_u32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigu32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2da, 
      Brigf32, 
      0
    }
  },
// imageread_2da_pair[2]
  {
    "rd_image_v4_2da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2da, 
      Brigu32, 
      0
    }
  },
// imageread_2da_pair[3]
  {
    "rd_image_v4_2da_s32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigs32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2da, 
      Brigf32, 
      0
    }
  },
// imageread_2da_pair[4]
  {
    "rd_image_v4_2da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2da, 
      Brigu32, 
      0
    }
  },
// imageread_2da_pair[5]
  {
    "rd_image_v4_2da_f32_f32 ($s1, $s2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_read_size, 
      BrigEInstRead, 
      BrigRdImage, 
      Brigf32, 
      BrigNoPacking, 
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        operand_offset + 8*reg_size + regv4_size + 2*opaque_size,
        0
      }, 
      Briggeom_2da, 
      Brigf32, 
      0
    }
  }
};

class TestRdImage2daInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imageread_2da_invalid[21] = {
//false[0]
  "rd_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "rd_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "rd_image_v4_2da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3];",
//false[3]
  "rd_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3],;",
//false[4]
  "rd_image_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8),;",
//false[5]
  "rd_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8)",
//false[6]
  "rd_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], $s5;",
//false[7]
  "rd_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7);",
//false[8]
  "rd_image_v4_2da_s32_u32 ($s1), [%RWImg3], [%Samp3], ($s5);",
//false[9]
  "rd_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], [%Samp3],$s5, $s6,",
//false[10]
  "rd_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s500);",
//false[11]
  "rd_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, [%Samp3], ($s5, $s6, $s7, $s8);",
//false[12]
  "rd_image_v4_2da_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], %Samp3, ($s5, $s6, $s7, $s8);",
//false[13]
  "rd_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], Samp3, ($s5, $s6, $s7, $s8);",
//false[14]
  "rd_image_v4_2da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[15]
  "rd_image_v2_2da_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, [%Samp3], ($s5, $s6, $s7, $s8);",
//false[16]
  "rd_image_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], (100);",
//false[17]
  "rd_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6, $s7, $s8);",
//false[18]
  "rdd_image_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[19]
  "rd_imagee_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);",
//false[20]
  "rd_image_v4_2da_f32_u32 $s1, [%RWImg3], [%Samp3], ($s5, $s6, $s7, $s8);"
};

class TestImageRetAdd : public ::testing::TestWithParam<int>
{

};
struct ImageRetTest{
  const char *str;
  BrigInstAtomicImage ref;
};

struct ImageRetTest imageret_add_pair[6] = {
//imageret_add_pair[0]
  {
    "atomic_image_add_1d_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_add_pair[1]
  {
    "atomic_image_add_2d_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_add_pair[2]
  {
    "atomic_image_add_3d_u64 $d1, [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_add_pair[3]
  {
    "atomic_image_add_acq_1db_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_add_pair[4]
  {
    "atomic_image_add_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_add_pair[5]
  {
    "atomic_image_add_2da_u64 $d1, [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetSub : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_sub_pair[6] = {
//imageret_sub_pair[0]
  {
    "atomic_image_sub_1d_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_sub_pair[1]
  {
    "atomic_image_sub_2d_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_sub_pair[2]
  {
    "atomic_image_sub_3d_u64 $d1, [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_sub_pair[3]
  {
    "atomic_image_sub_acq_1db_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_sub_pair[4]
  {
    "atomic_image_sub_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_sub_pair[5]
  {
    "atomic_image_sub_2da_u64 $d1, [&namedRWImg2], ($d2, $d3, $d4, $d5), $d6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};


class TestImageRetOr : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_or_pair[6] = {
//imageret_or_pair[0]
  {
    "atomic_image_or_1d_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_or_pair[1]
  {
    "atomic_image_or_2d_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_or_pair[2]
  {
    "atomic_image_or_3d_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_or_pair[3]
  {
    "atomic_image_or_acq_1db_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_or_pair[4]
  {
    "atomic_image_or_ar_1da_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_or_pair[5]
  {
    "atomic_image_or_2da_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetAnd : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_and_pair[6] = {
//imageret_and_pair[0]
  {
    "atomic_image_and_1d_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_and_pair[1]
  {
    "atomic_image_and_2d_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_and_pair[2]
  {
    "atomic_image_and_3d_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_and_pair[3]
  {
    "atomic_image_and_acq_1db_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_and_pair[4]
  {
    "atomic_image_and_ar_1da_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_and_pair[5]
  {
    "atomic_image_and_2da_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetXor : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_xor_pair[6] = {
//imageret_xor_pair[0]
  {
    "atomic_image_xor_1d_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_xor_pair[1]
  {
    "atomic_image_xor_2d_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_xor_pair[2]
  {
    "atomic_image_xor_3d_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_xor_pair[3]
  {
    "atomic_image_xor_acq_1db_b32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_xor_pair[4]
  {
    "atomic_image_xor_ar_1da_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_xor_pair[5]
  {
    "atomic_image_xor_2da_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetInc : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_inc_pair[6] = {
//imageret_inc_pair[0]
  {
    "atomic_image_inc_1d_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_inc_pair[1]
  {
    "atomic_image_inc_2d_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_inc_pair[2]
  {
    "atomic_image_inc_3d_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_inc_pair[3]
  {
    "atomic_image_inc_acq_1db_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_inc_pair[4]
  {
    "atomic_image_inc_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_inc_pair[5]
  {
    "atomic_image_inc_2da_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetDec : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_dec_pair[6] = {
//imageret_dec_pair[0]
  {
    "atomic_image_dec_1d_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_dec_pair[1]
  {
    "atomic_image_dec_2d_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_dec_pair[2]
  {
    "atomic_image_dec_3d_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_dec_pair[3]
  {
    "atomic_image_dec_acq_1db_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_dec_pair[4]
  {
    "atomic_image_dec_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_dec_pair[5]
  {
    "atomic_image_dec_2da_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetMax : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_max_pair[6] = {
//imageret_max_pair[0]
  {
    "atomic_image_max_1d_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_max_pair[1]
  {
    "atomic_image_max_2d_u32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_max_pair[2]
  {
    "atomic_image_max_3d_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_max_pair[3]
  {
    "atomic_image_max_acq_1db_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_max_pair[4]
  {
    "atomic_image_max_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_max_pair[5]
  {
    "atomic_image_max_2da_u32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetMin : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_min_pair[6] = {
//imageret_max_pair[0]
  {
    "atomic_image_min_1d_s32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_min_pair[1]
  {
    "atomic_image_min_2d_u32 $s1, [&namedRWImg2], ($s2, $s3), $s4;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_min_pair[2]
  {
    "atomic_image_min_3d_s32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_min_pair[3]
  {
    "atomic_image_min_acq_1db_u32 $s1, [&namedRWImg2], $s2, $s3;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_min_pair[4]
  {
    "atomic_image_min_ar_1da_s32 $s1, [&namedRWImg2], ($s2, $s3), 1;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      RoundUp8(operand_offset + reg_size * 3 + opaque_size + regv2_size),
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_min_pair[5]
  {
    "atomic_image_min_2da_u32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), WAVESIZE;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageRetCas : public ::testing::TestWithParam<int>
{

};

struct ImageRetTest imageret_cas_pair[6] = {
//imageret_cas_pair[0]
  {
    "atomic_image_cas_1d_b32 $s1, [&namedRWImg2], $s2, $s3, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_1d
    }
  },
//imageret_cas_pair[1]
  {
    "atomic_image_cas_2d_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      operand_offset + reg_size * 4 + opaque_size + regv2_size
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2d
    }
  },
//imageret_cas_pair[2]
  {
    "atomic_image_cas_3d_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      operand_offset + reg_size * 6 + opaque_size + regv4_size
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_3d
    }
  },
//imageret_cas_pair[3]
  {
    "atomic_image_cas_acq_1db_b32 $s1, [&namedRWImg2], $s2, $s3, $s10;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + opaque_size,
      operand_offset + reg_size * 2 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquire,
      Briggeom_1db
    }
  },
//imageret_cas_pair[4]
  {
    "atomic_image_cas_ar_1da_b32 $s1, [&namedRWImg2], ($s2, $s3), $s4, 1;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 3 + opaque_size,
      operand_offset + reg_size * 3 + opaque_size + regv2_size,
      RoundUp8(operand_offset + reg_size * 4 + opaque_size + regv2_size)
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquireRelease,
      Briggeom_1da
    }
  },
//imageret_Cas_pair[5]
  {
    "atomic_image_cas_2da_b32 $s1, [&namedRWImg2], ($s2, $s3, $s4, $s5), $s6, WAVESIZE;",
    {
      brig_inst_atomic_image_size,
      BrigEInstAtomicImage,
      BrigAtomicImage,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 5 + opaque_size,
      operand_offset + reg_size * 5 + opaque_size + regv4_size,
      operand_offset + reg_size * 6 + opaque_size + regv4_size
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular,
      Briggeom_2da
    }
  }
};

class TestImageStore : public ::testing::TestWithParam<int>
{

};
struct ImageStoreTest{
  const char *str;
  BrigInstImage ref;
};

struct ImageStoreTest imagestore_pair[12] = {
// imagestore_pair[0]
{
    "st_image_v4_1d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1d,
      Brigu32, 
      0
    }
  },
// imagestore_pair[1]
  {
    "st_image_v4_1d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1d,
      Brigu32,
      0
    }
  },
// imagestore_pair[2]
  {
    "st_image_v4_2d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2d,
      Brigu32,
       0
    }
  },
// imagestore_pair[3]
  {
    "st_image_v4_2d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2d,
      Brigu32,
      0
    }
  },
// imagestore_pair[4]
  {
    "st_image_v4_3d_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_3d,
      Brigu32,
      0
    }
  },
// imagestore_pair[5]
  {
    "st_image_v4_3d_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      },  
      Briggeom_3d,
      Brigu32, 
      0
    }
  },
// imagestore_pair[6]
  {
    "st_image_v4_1da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1da,
      Brigu32,
      0
    }
  },
// imagestore_pair[7]
  {
    "st_image_v4_1da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 6*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1da,
      Brigu32,
      0
    }
  },
// imagestore_pair[8]
  {
    "st_image_v4_2da_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2da,
      Brigu32,
      0
    }
  },
// imagestore_pair[9]
  {
    "st_image_v4_2da_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5, $s6, $s7, $s8);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 8*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_2da,
      Brigu32,
      0
    }
  },
// imagestore_pair[10]
  {
    "st_image_v4_1db_f32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigf32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1db,
      Brigu32,
      0
    }
  },
// imagestore_pair[11]
  {
    "st_image_v4_1db_s32_u32 ($s1, $s2, $s3, $s4), [%RWImg3], ($s5);",
    {
      brig_inst_image_size, 
      BrigEInstImage, 
      BrigStImage, 
      Brigs32, 
      BrigNoPacking,
      {
        operand_offset + 4*reg_size,
        operand_offset + 4*reg_size + regv4_size,
        operand_offset + 4*reg_size + regv4_size + opaque_size,
        0,
        0
      }, 
      Briggeom_1db,
      Brigu32,
      0
    }
  }
};

class TestImageStoreInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_imagestore_invalid[60] = {
//false[0]
  "st_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[1]
  "st_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[2]
  "st_image_v4_1d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[3]
  "st_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[4]
  "st_image_v4_1d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5),;",
//false[5]
  "st_image_v4_1d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5)",
//false[6]
  "st_image_v4_1d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[7]
  "st_image_v4_1d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[8]
  "st_image_v4_1d_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[9]
  "st_image_v4_1d_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);",
//false[10]
  "st_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s500);",
//false[11]
  "st_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), %RWImg3, ($s5, $s6);",
//false[12]
  "st_image_v4_2d_u32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[13]
  "st_image_v4_2d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[14]
  "st_image_v4_2d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], (s5);",
//false[15]
  "st_image_v2_2d_s32_f32 ($s1, $c2, $s3, $s4), RWImg3, ($s5, $s6);",
//false[16]
  "st_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], (100);",
//false[17]
  "st_image_v4_2d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], 100, ($s5, $s6);",
//false[18]
  "ldd_image_v4_2d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[19]
  "st_image_v4_2d_f32_u32 $s1, [%RWImg3], $s5, $s6;",
//false[20]
  "st_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4);",
//false[21]
  "st_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[22]
  "st_image_v4_3d_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[23]
  "st_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[24]
  "st_image_v4_3d_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[25]
  "st_image_v4_3d_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[26]
  "st_image_v4_3d_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[27]
  "st_image_v4_3d_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[28]
  "st_image_v4_3d_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[29]
  "st_image_v4_3d_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5, $s6 $s7, $s8);",
//false[30]
  "st_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[31]
  "st_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[32]
  "st_image_v4_1da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[33]
  "st_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[34]
  "st_image_v4_1da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[35]
  "st_image_v4_1da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[36]
  "st_image_v4_1da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[37]
  "st_image_v4_1da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[38]
  "st_image_v4_1da_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[39]
  "st_image_v4_1da_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);",
//false[40]
  "st_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4);",
//false[41]
  "st_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[42]
  "st_image_v4_2da_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[43]
  "st_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[44]
  "st_image_v4_2da_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8),;",
//false[45]
  "st_image_v4_2da_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6 $s7, $s8)",
//false[46]
  "st_image_v4_2da_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[47]
  "st_image_v4_2da_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[48]
  "st_image_v4_2da_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[49]
  "st_image_v4_2da_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5, $s6 $s7, $s8);",
//false[50]
  "st_image_v4_1db_u32_s32 ($s1, $c2, $s3, $s4);",
//false[51]
  "st_image_v4_1db_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[52]
  "st_image_v4_1db_s32_u32 ($s1, $c2, $s3, $s4), [%RWImg3];",
//false[53]
  "st_image_v4_1db_s32_f32 ($s1, $c2, $s3, $s4), [%RWImg3],;",
//false[54]
  "st_image_v4_1db_f32_u32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5),;",
//false[55]
  "st_image_v4_1db_f32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5)",
//false[56]
  "st_image_v4_1db_u32_s32 ($s1, $c2, $s3, $s4), [%RWImg3], $s5;",
//false[57]
  "st_image_v4_1db_u32_f32 ($s1, $c2, $s3, $s4), [%RWImg3], ($s5, $s6);",
//false[58]
  "st_image_v4_1db_s32_s32 ($s1), [%RWImg3], ($s5, $s6);",
//false[59]
  "st_image_v4_1db_s32_f32 ($s1, $c2, $s3, $s4, $s6), [%RWImg3], ($s5);"
};

}  // namespace brig
}  // namespace hsa
#endif //IMAGE_TEST_H_
