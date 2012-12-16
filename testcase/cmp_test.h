/* Copyright 2012 <MulticorewareInc> */

#ifndef CMP_TEST_H_
#define CMP_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

BrigAluModifier cmp_bam[8] = {
  {0, 0, 0, 0, 0, 0, 0},// no modifiers and all fields are all zeros
  {1, 1, 0, 1, 0, 0, 0},// ftz
  {1, 1, 1, 0, 0, 0, 0},// zero
  {1, 1, 2, 0, 0, 0, 0},// up
  {1, 1, 3, 0, 0, 0, 0},// down
  {1, 1, 1, 1, 0, 0, 0},// ftz_zero
  {1, 1, 2, 1, 0, 0, 0},// ftz_up
  {1, 1, 3, 1, 0, 0, 0} // ftz_down
};

class TestCmpEq : public ::testing::TestWithParam<int>
{

};
struct CmpEqTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpEqTest cmp_eq_pair[45] = {
// cmp_pair[0]
//supports eight control registers
  {
    "cmp_eq_b1_b1 $c7, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb1,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_eq_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_eq_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb64,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_eq_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu32,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_eq_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu64,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_eq_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs32,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_eq_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs64,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_ftz_eq_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf32,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_eq_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigf64,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_eq_b32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb1,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_eq_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_eq_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_eq_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_eq_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_eq_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_eq_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_ftz_eq_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_eq_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigf64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_eq_u32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb1,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_eq_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_eq_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb64,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_eq_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu32,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_eq_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu64,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_eq_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs32,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_eq_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs64,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_eq_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigf32,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_eq_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigf64,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_eq_s32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb1,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_eq_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_eq_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_eq_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_eq_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_eq_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_eq_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_ftz_eq_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_ftz_eq_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_eq_f32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb1,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_eq_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_eq_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigb64,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_eq_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu32,
      0
    }
  },
// cmp_pair[40]
  {
    "cmp_eq_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigu64,
      0
    }
  },
// cmp_pair[41]
  {
    "cmp_eq_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs32,
      0
    }
  },
// cmp_pair[42]
  {
    "cmp_eq_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEq,
      Brigs64,
      0
    }
  },
// cmp_pair[43]
  {
    "cmp_ftz_eq_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf32,
      0
    }
  },
// cmp_pair[44]
  {
    "cmp_ftz_eq_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigf64,
      0
    }
  }
};

class TestCmpEqInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_eq_invalid[56] = {
// false[0]
  "cmp_eq_b1_b1 $c1;",
// false[1]
  "cmp_eq_b1_b1 $c1, $c2;",
// false[2]
  "cmp_eq_b1_b1 $c1, $c2,;",
// false[3]
  "cmp_eq_b1_b1 $c1, $c2, $c3,",
// false[4]
  "cmp_eq_b1_b1 $c1, $c2, $c3, $c4;",
// false[5]
  "cmp_eq_b1_b1 $c1, $c2, $c100;",
// false[6]
  "cmp_eq_b1_b1 $c1, $c2, $100;",
// false[7]
  "cmp_eq_b1_b1 $c1, $c2, c100;",
// false[8]
  "cmmp_eq_b1_b1 $c1, $c2, $c100;",
// false[9]
  "cmp_eq_b32_b1 $d1;",
// false[10]
  "cmp_eq_b32_b1 $s1, $c2;",
// false[11]
  "cmp_eq_b32_b1 $s1, $c2,;",
// false[12]
  "cmp_eq_b32_b1 $s1, $c2, $c3,",
// false[13]
  "cmp_eq_b32_b1 $s1, $c2, $c3, $c4;",
// false[14]
  "cmp_eq_b32_b1 $s1, $c2, $c100;",
// false[15]
  "cmp_eq_b32_b1 $s1, $c2, $100;",
// false[16]
  "cmp_eq_b32_b1 $s1, $c2, c100;",
// false[17]
  "cmmp_eq_b32_b1 $s1, $c2, $c100;",
// false[18]
  "cmp_eq_u32_b1 $d1;",
// false[19]
  "cmp_eq_u32_b1 $s1, $c2;",
// false[20]
  "cmp_eq_u32_b1 $s1, $c2,;",
// false[21]
  "cmp_eq_u32_b1 $s1, $c2, $c3,",
// false[22]
  "cmp_eq_u32_b1 $s1, $c2, $c3, $c4;",
// false[23]
  "cmp_eq_u32_b1 $s1, $c2, $c100;",
// false[24]
  "cmp_eq_u32_b1 $s1, $c2, $100;",
// false[25]
  "cmp_eq_u32_b1 $s1, $c2, c100;",
// false[26]
  "cmmp_eq_u32_b1 $s1, $c2, $c100;",
// false[27]
  "cmp_eq_s32_b1 $d1;",
// false[28]
  "cmp_eq_s32_b1 $s1, $c2;",
// false[29]
  "cmp_eq_s32_b1 $s1, $c2,;",
// false[30]
  "cmp_eq_s32_b1 $s1, $c2, $c3,",
// false[31]
  "cmp_eq_s32_b1 $s1, $c2, $c3, $c4;",
// false[32]
  "cmp_eq_s32_b1 $s1, $c2, $c100;",
// false[33]
  "cmp_eq_s32_b1 $s1, $c2, $100;",
// false[34]
  "cmp_eq_s32_b1 $s1, $c2, c100;",
// false[35]
  "cmmp_eq_s32_b1 $s1, $c2, $c100;",
// false[36]
  "cmp_eq_f32_b1 $d1;",
// false[37]
  "cmp_eq_f32_b1 $s1, $c2;",
// false[38]
  "cmp_eq_f32_b1 $s1, $c2,;",
// false[39]
  "cmp_eq_f32_b1 $s1, $c2, $c3,",
// false[40]
  "cmp_eq_f32_b1 $s1, $c2, $c3, $c4;",
// false[41]
  "cmp_eq_f32_b1 $s1, $c2, $c100;",
// false[42]
  "cmp_eq_f32_b1 $s1, $c2, $100;",
// false[43]
  "cmp_eq_f32_b1 $s1, $c2, c100;",
// false[44]
  "cmmp_eq_f32_b1 $s1, $c2, $c100;",
// false[45]
  "cmp_eq_b32_b1 $d1;",
// false[46]
  "cmp_eq_b32_b32 $s1, $s2;",
// false[47]
  "cmp_eq_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_eq_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_eq_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_eq_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_eq_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_eq_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_eq_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_eq_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_eq_b32_u32 WAVESIZE, $s2, $s3;"
};

class TestCmpNe : public ::testing::TestWithParam<int>
{

};
struct CmpNeTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpNeTest cmp_ne_pair[45] = {
// cmp_pair[0]
  {
    "cmp_ne_b1_b1 $c1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb1,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_ne_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb32,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_ne_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb64,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_ne_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu32,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_ne_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu64,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_ne_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs32,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_ne_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs64,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_ne_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf32,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_ftz_ne_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigNe,
      Brigf64,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_ne_b32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb1,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_ne_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_ne_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_ne_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_ne_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ne_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ne_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_ne_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_ne_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_ne_u32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb1,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_ne_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb32,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_ne_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb64,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_ne_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu32,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_ne_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu64,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_ne_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs32,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_ne_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs64,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_ne_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf32,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_ne_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf64,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_ne_s32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb1,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_ne_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_ne_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_ne_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_ne_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_ne_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_ne_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_ne_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_ne_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_ne_f32_b1 $s1, $c2, $c3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb1,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_ne_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb32,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_ne_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigb64,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_ne_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu32,
      0
    }
  },
// cmp_pair[40]
  {
    "cmp_ne_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigu64,
      0
    }
  },
// cmp_pair[41]
  {
    "cmp_ne_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs32,
      0
    }
  },
// cmp_pair[42]
  {
    "cmp_ne_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigs64,
      0
    }
  },
// cmp_pair[43]
  {
    "cmp_ftz_ne_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigNe,
      Brigf32,
      0
    }
  },
// cmp_pair[44]
  {
    "cmp_ne_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf64,
      0
    }
  }
};

class TestCmpNeInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_ne_invalid[56] = {
// false[0]
  "cmp_ne_b1_b1 $c1;",
// false[1]
  "cmp_ne_b1_b1 $c1, $c2;",
// false[2]
  "cmp_ne_b1_b1 $c1, $c2,;",
// false[3]
  "cmp_ne_b1_b1 $c1, $c2, $c3,",
// false[4]
  "cmp_ne_b1_b1 $c1, $c2, $c3, $c4;",
// false[5]
  "cmp_ne_b1_b1 $c1, $c2, $c100;",
// false[6]
  "cmp_ne_b1_b1 $c1, $c2, $100;",
// false[7]
  "cmp_ne_b1_b1 $c1, $c2, c100;",
// false[8]
  "cmmp_ne_b1_b1 $c1, $c2, $c100;",
// false[9]
  "cmp_ne_b32_b1 $d1;",
// false[10]
  "cmp_ne_b32_b1 $s1, $c2;",
// false[11]
  "cmp_ne_b32_b1 $s1, $c2,;",
// false[12]
  "cmp_ne_b32_b1 $s1, $c2, $c3,",
// false[13]
  "cmp_ne_b32_b1 $s1, $c2, $c3, $c4;",
// false[14]
  "cmp_ne_b32_b1 $s1, $c2, $c100;",
// false[15]
  "cmp_ne_b32_b1 $s1, $c2, $100;",
// false[16]
  "cmp_ne_b32_b1 $s1, $c2, c100;",
// false[17]
  "cmmp_ne_b32_b1 $s1, $c2, $c100;",
// false[18]
  "cmp_ne_u32_b1 $d1;",
// false[19]
  "cmp_ne_u32_b1 $s1, $c2;",
// false[20]
  "cmp_ne_u32_b1 $s1, $c2,;",
// false[21]
  "cmp_ne_u32_b1 $s1, $c2, $c3,",
// false[22]
  "cmp_ne_u32_b1 $s1, $c2, $c3, $c4;",
// false[23]
  "cmp_ne_u32_b1 $s1, $c2, $c100;",
// false[24]
  "cmp_ne_u32_b1 $s1, $c2, $100;",
// false[25]
  "cmp_ne_u32_b1 $s1, $c2, c100;",
// false[26]
  "cmmp_ne_u32_b1 $s1, $c2, $c100;",
// false[27]
  "cmp_ne_s32_b1 $d1;",
// false[28]
  "cmp_ne_s32_b1 $s1, $c2;",
// false[29]
  "cmp_ne_s32_b1 $s1, $c2,;",
// false[30]
  "cmp_ne_s32_b1 $s1, $c2, $c3,",
// false[31]
  "cmp_ne_s32_b1 $s1, $c2, $c3, $c4;",
// false[32]
  "cmp_ne_s32_b1 $s1, $c2, $c100;",
// false[33]
  "cmp_ne_s32_b1 $s1, $c2, $100;",
// false[34]
  "cmp_ne_s32_b1 $s1, $c2, c100;",
// false[35]
  "cmmp_ne_s32_b1 $s1, $c2, $c100;",
// false[36]
  "cmp_ne_f32_b1 $d1;",
// false[37]
  "cmp_ne_f32_b1 $s1, $c2;",
// false[38]
  "cmp_ne_f32_b1 $s1, $c2,;",
// false[39]
  "cmp_ne_f32_b1 $s1, $c2, $c3,",
// false[40]
  "cmp_ne_f32_b1 $s1, $c2, $c3, $c4;",
// false[41]
  "cmp_ne_f32_b1 $s1, $c2, $c100;",
// false[42]
  "cmp_ne_f32_b1 $s1, $c2, $100;",
// false[43]
  "cmp_ne_f32_b1 $s1, $c2, c100;",
// false[44]
  "cmmp_ne_f32_b1 $s1, $c2, $c100;",
// false[45]
  "cmp_ne_b32_b1 $d1;",
// false[46]
  "cmp_ne_b32_b32 $s1, $s2;",
// false[47]
  "cmp_ne_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_ne_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_ne_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_ne_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_ne_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_ne_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_ne_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_ne_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_ne_b32_u32 WAVESIZE, $s2, $s3;"
};

class TestCmpLt : public ::testing::TestWithParam<int>
{

};
struct CmpLtTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpLtTest cmp_lt_pair[40] = {
// cmp_pair[0]
  {
    "cmp_lt_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_lt_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb64,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_lt_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_lt_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_lt_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_lt_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_ftz_lt_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigLt,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_lt_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_lt_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_lt_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_lt_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_lt_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_lt_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_lt_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_lt_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_lt_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_lt_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_lt_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_lt_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu32,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_lt_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu64,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_lt_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs32,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_lt_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs64,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_lt_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf32,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_lt_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf64,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_lt_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb32,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_lt_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb64,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_lt_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu32,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_lt_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu64,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_lt_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_lt_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_lt_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_lt_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_lt_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_lt_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigb64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_lt_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_lt_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigu64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_lt_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs32,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_lt_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigs64,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_ftz_lt_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigLt,
      Brigf32,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_lt_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf64,
      0
    }
  }
};

class TestCmpLtInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_lt_invalid[58] = {
// false[0]
  "cmp_lt_b1_b32 $c1;",
// false[1]
  "cmp_lt_b1_b32 $c1, $s2;",
// false[2]
  "cmp_lt_b1_b32 $c1, $s2,;",
// false[3]
  "cmp_lt_b1_b32 $c1, $s2, $s3,",
// false[4]
  "cmp_lt_b1_b32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_lt_b1_b32 $c1, $s2, $c100;",
// false[6]
  "cmp_lt_b1_b32 $c1, $s2, $100;",
// false[7]
  "cmp_lt_b1_b32 $c1, $s2, c100;",
// false[8]
  "cmmp_lt_b1_b32 $c1, $s2, $c100;",
// false[9]
  "cmp_lt_b32_b32 $d1;",
// false[10]
  "cmp_lt_b32_b32 $s1, $s2;",
// false[11]
  "cmp_lt_b32_b32 $s1, $s2,;",
// false[12]
  "cmp_lt_b32_b32 $s1, $s2, $s3,",
// false[13]
  "cmp_lt_b32_b32 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_lt_b32_b32 $s1, $s2, $c100;",
// false[15]
  "cmp_lt_b32_b32 $s1, $s2, $100;",
// false[16]
  "cmp_lt_b32_b32 $s1, $s2, c100;",
// false[17]
  "cmmp_lt_b32_b32 $s1, $s2, $c100;",
// false[18]
  "cmp_lt_u32_b32 $d1;",
// false[19]
  "cmp_lt_u32_b32 $s1, $s2;",
// false[20]
  "cmp_lt_u32_b32 $s1, $s2,;",
// false[21]
  "cmp_lt_u32_b32 $s1, $s2, $s3,",
// false[22]
  "cmp_lt_u32_b32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_lt_u32_b32 $s1, $s2, $c100;",
// false[24]
  "cmp_lt_u32_b32 $s1, $s2, $100;",
// false[25]
  "cmp_lt_u32_b32 $s1, $s2, c100;",
// false[26]
  "cmmp_lt_u32_b32 $s1, $s2, $c100;",
// false[27]
  "cmp_lt_s32_b32 $d1;",
// false[28]
  "cmp_lt_s32_b32 $s1, $s2;",
// false[29]
  "cmp_lt_s32_b32 $s1, $s2,;",
// false[30]
  "cmp_lt_s32_b32 $s1, $s2, $s3,",
// false[31]
  "cmp_lt_s32_b32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_lt_s32_b32 $s1, $s2, $c100;",
// false[33]
  "cmp_lt_s32_b32 $s1, $s2, $100;",
// false[34]
  "cmp_lt_s32_b32 $s1, $s2, c100;",
// false[35]
  "cmmp_lt_s32_b32 $s1, $s2, $c100;",
// false[36]
  "cmp_lt_f32_b32 $d1;",
// false[37]
  "cmp_lt_f32_b32 $s1, $s2;",
// false[38]
  "cmp_lt_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_lt_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_lt_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_lt_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_lt_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_lt_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_lt_f32_b32 $s1, $s2, $c100;",
// false[45]
  "cmp_lt_b32_b32 $d1;",
// false[46]
  "cmp_lt_b32_b32 $s1, $s2;",
// false[47]
  "cmp_lt_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_lt_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_lt_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_lt_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_lt_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_lt_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_lt_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_lt_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_lt_b32_u32 WAVESIZE, $s2, $s3;",
// false[56]
  "cmp_lt_b32_b1 $s1, $c2, $c3;",
// false[57]
  "cmp_ltt_b32_b32 $s1, $s2, $s3;"
};

class TestCmpLe : public ::testing::TestWithParam<int>
{

};
struct CmpLeTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpLeTest cmp_le_pair[40] = {
// cmp_pair[0]
  {
    "cmp_le_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_le_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb64,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_le_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_le_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_le_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_le_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_ftz_le_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigLe,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_le_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_le_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_le_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_le_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_le_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_le_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_le_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_le_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_le_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_le_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_le_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_le_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu32,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_le_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu64,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_le_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs32,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_le_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs64,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_le_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf32,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_le_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf64,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_le_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_le_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb64,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_le_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu32,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_le_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu64,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_le_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_le_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_le_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_le_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_le_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_le_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_le_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_le_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigu64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_le_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs32,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_le_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigs64,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_le_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigf32,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_ftz_le_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigLe,
      Brigf64,
      0
    }
  }
};

class TestCmpLeInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_le_invalid[58] = {
// false[0]
  "cmp_le_b1_b32 $c1;",
// false[1]
  "cmp_le_b1_b32 $c1, $s2;",
// false[2]
  "cmp_le_b1_b32 $c1, $s2,;",
// false[3]
  "cmp_le_b1_b32 $c1, $s2, $s3,",
// false[4]
  "cmp_le_b1_b32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_le_b1_b32 $c1, $s2, $c100;",
// false[6]
  "cmp_le_b1_b32 $c1, $s2, $100;",
// false[7]
  "cmp_le_b1_b32 $c1, $s2, c100;",
// false[8]
  "cmmp_le_b1_b32 $c1, $s2, $c100;",
// false[9]
  "cmp_le_b32_b32 $d1;",
// false[10]
  "cmp_le_b32_b32 $s1, $s2;",
// false[11]
  "cmp_le_b32_b32 $s1, $s2,;",
// false[12]
  "cmp_le_b32_b32 $s1, $s2, $s3,",
// false[13]
  "cmp_le_b32_b32 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_le_b32_b32 $s1, $s2, $c100;",
// false[15]
  "cmp_le_b32_b32 $s1, $s2, $100;",
// false[16]
  "cmp_le_b32_b32 $s1, $s2, c100;",
// false[17]
  "cmmp_le_b32_b32 $s1, $s2, $c100;",
// false[18]
  "cmp_le_u32_b32 $d1;",
// false[19]
  "cmp_le_u32_b32 $s1, $s2;",
// false[20]
  "cmp_le_u32_b32 $s1, $s2,;",
// false[21]
  "cmp_le_u32_b32 $s1, $s2, $s3,",
// false[22]
  "cmp_le_u32_b32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_le_u32_b32 $s1, $s2, $c100;",
// false[24]
  "cmp_le_u32_b32 $s1, $s2, $100;",
// false[25]
  "cmp_le_u32_b32 $s1, $s2, c100;",
// false[26]
  "cmmp_le_u32_b32 $s1, $s2, $c100;",
// false[27]
  "cmp_le_s32_b32 $d1;",
// false[28]
  "cmp_le_s32_b32 $s1, $s2;",
// false[29]
  "cmp_le_s32_b32 $s1, $s2,;",
// false[30]
  "cmp_le_s32_b32 $s1, $s2, $s3,",
// false[31]
  "cmp_le_s32_b32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_le_s32_b32 $s1, $s2, $c100;",
// false[33]
  "cmp_le_s32_b32 $s1, $s2, $100;",
// false[34]
  "cmp_le_s32_b32 $s1, $s2, c100;",
// false[35]
  "cmmp_le_s32_b32 $s1, $s2, $c100;",
// false[36]
  "cmp_le_f32_b32 $d1;",
// false[37]
  "cmp_le_f32_b32 $s1, $s2;",
// false[38]
  "cmp_le_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_le_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_le_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_le_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_le_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_le_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_le_f32_b32 $s1, $s2, $c100;",
// false[45]
  "cmp_le_b32_b32 $d1;",
// false[46]
  "cmp_le_b32_b32 $s1, $s2;",
// false[47]
  "cmp_le_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_le_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_le_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_le_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_le_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_le_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_le_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_le_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_le_b32_u32 WAVESIZE, $s2, $s3;",
// false[56]
  "cmp_le_b32_b1 $s1, $c2, $c3;",
// false[57]
  "cmp_let_b32_b32 $s1, $s2, $s3;"
};

class TestCmpGt : public ::testing::TestWithParam<int>
{

};
struct CmpGtTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpGtTest cmp_gt_pair[40] = {
// cmp_pair[0]
  {
    "cmp_gt_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_gt_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb64,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_gt_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_gt_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_gt_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_gt_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_gt_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_ftz_gt_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigGt,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_gt_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_gt_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_gt_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_gt_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_gt_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_gt_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_gt_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_gt_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_gt_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_gt_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_gt_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu32,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_gt_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu64,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_gt_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs32,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_gt_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs64,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_gt_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf32,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_gt_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf64,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_gt_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb32,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_gt_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb64,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_gt_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu32,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_gt_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu64,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_gt_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_gt_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_gt_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_gt_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_gt_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_gt_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigb64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_gt_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_gt_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigu64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_gt_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs32,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_gt_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigs64,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_ftz_gt_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigGt,
      Brigf32,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_gt_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGt,
      Brigf64,
      0
    }
  }
};

class TestCmpGtInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_gt_invalid[58] = {
// false[0]
  "cmp_gt_b1_b32 $c1;",
// false[1]
  "cmp_gt_b1_b32 $c1, $s2;",
// false[2]
  "cmp_gt_b1_b32 $c1, $s2,;",
// false[3]
  "cmp_gt_b1_b32 $c1, $s2, $s3,",
// false[4]
  "cmp_gt_b1_b32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_gt_b1_b32 $c1, $s2, $c100;",
// false[6]
  "cmp_gt_b1_b32 $c1, $s2, $100;",
// false[7]
  "cmp_gt_b1_b32 $c1, $s2, c100;",
// false[8]
  "cmmp_gt_b1_b32 $c1, $s2, $c100;",
// false[9]
  "cmp_gt_b32_b32 $d1;",
// false[10]
  "cmp_gt_b32_b32 $s1, $s2;",
// false[11]
  "cmp_gt_b32_b32 $s1, $s2,;",
// false[12]
  "cmp_gt_b32_b32 $s1, $s2, $s3,",
// false[13]
  "cmp_gt_b32_b32 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_gt_b32_b32 $s1, $s2, $c100;",
// false[15]
  "cmp_gt_b32_b32 $s1, $s2, $100;",
// false[16]
  "cmp_gt_b32_b32 $s1, $s2, c100;",
// false[17]
  "cmmp_gt_b32_b32 $s1, $s2, $c100;",
// false[18]
  "cmp_gt_u32_b32 $d1;",
// false[19]
  "cmp_gt_u32_b32 $s1, $s2;",
// false[20]
  "cmp_gt_u32_b32 $s1, $s2,;",
// false[21]
  "cmp_gt_u32_b32 $s1, $s2, $s3,",
// false[22]
  "cmp_gt_u32_b32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_gt_u32_b32 $s1, $s2, $c100;",
// false[24]
  "cmp_gt_u32_b32 $s1, $s2, $100;",
// false[25]
  "cmp_gt_u32_b32 $s1, $s2, c100;",
// false[26]
  "cmmp_gt_u32_b32 $s1, $s2, $c100;",
// false[27]
  "cmp_gt_s32_b32 $d1;",
// false[28]
  "cmp_gt_s32_b32 $s1, $s2;",
// false[29]
  "cmp_gt_s32_b32 $s1, $s2,;",
// false[30]
  "cmp_gt_s32_b32 $s1, $s2, $s3,",
// false[31]
  "cmp_gt_s32_b32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_gt_s32_b32 $s1, $s2, $c100;",
// false[33]
  "cmp_gt_s32_b32 $s1, $s2, $100;",
// false[34]
  "cmp_gt_s32_b32 $s1, $s2, c100;",
// false[35]
  "cmmp_gt_s32_b32 $s1, $s2, $c100;",
// false[36]
  "cmp_gt_f32_b32 $d1;",
// false[37]
  "cmp_gt_f32_b32 $s1, $s2;",
// false[38]
  "cmp_gt_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_gt_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_gt_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_gt_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_gt_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_gt_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_gt_f32_b32 $s1, $s2, $c100;",
// false[45]
  "cmp_gt_b32_b32 $d1;",
// false[46]
  "cmp_gt_b32_b32 $s1, $s2;",
// false[47]
  "cmp_gt_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_gt_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_gt_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_gt_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_gt_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_gt_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_gt_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_gt_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_gt_b32_u32 WAVESIZE, $s2, $s3;",
// false[56]
  "cmp_gt_b32_b1 $s1, $c2, $c3;",
// false[57]
  "cmp_gtt_b32_b32 $s1, $s2, $s3;"
};

class TestCmpGe : public ::testing::TestWithParam<int>
{

};
struct CmpGeTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpGeTest cmp_ge_pair[40] = {
// cmp_pair[0]
  {
    "cmp_ge_b1_b32 $c1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_ge_b1_b64 $c1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb64,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_ge_b1_u32 $c1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_ge_b1_u64 $c1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_ge_b1_s32 $c1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_ge_b1_s64 $c1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_ftz_ge_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigGe,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_ge_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_ge_b32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_ge_b32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_ge_b32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_ge_b32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_ge_b32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_ge_b32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ge_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ge_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf64,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_ge_u32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb32,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_ge_u32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb64,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_ge_u32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu32,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_ge_u32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu64,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_ge_u32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs32,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_ge_u32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs64,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_ge_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf32,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_ge_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf64,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_ge_s32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb32,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_ge_s32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb64,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_ge_s32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu32,
      0
    }
  },
// cmp_pair[27]
  {
    "cmp_ge_s32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu64,
      0
    }
  },
// cmp_pair[28]
  {
    "cmp_ge_s32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs32,
      0
    }
  },
// cmp_pair[29]
  {
    "cmp_ge_s32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs64,
      0
    }
  },
// cmp_pair[30]
  {
    "cmp_ge_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf32,
      0
    }
  },
// cmp_pair[31]
  {
    "cmp_ge_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf64,
      0
    }
  },
// cmp_pair[32]
  {
    "cmp_ge_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb32,
      0
    }
  },
// cmp_pair[33]
  {
    "cmp_ge_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigb64,
      0
    }
  },
// cmp_pair[34]
  {
    "cmp_ge_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu32,
      0
    }
  },
// cmp_pair[35]
  {
    "cmp_ge_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigu64,
      0
    }
  },
// cmp_pair[36]
  {
    "cmp_ge_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs32,
      0
    }
  },
// cmp_pair[37]
  {
    "cmp_ge_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigs64,
      0
    }
  },
// cmp_pair[38]
  {
    "cmp_ftz_ge_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigGe,
      Brigf32,
      0
    }
  },
// cmp_pair[39]
  {
    "cmp_ge_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf64,
      0
    }
  }
};

class TestCmpGeInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_ge_invalid[58] = {
// false[0]
  "cmp_ge_b1_b32 $c1;",
// false[1]
  "cmp_ge_b1_b32 $c1, $s2;",
// false[2]
  "cmp_ge_b1_b32 $c1, $s2,;",
// false[3]
  "cmp_ge_b1_b32 $c1, $s2, $s3,",
// false[4]
  "cmp_ge_b1_b32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_ge_b1_b32 $c1, $s2, $c100;",
// false[6]
  "cmp_ge_b1_b32 $c1, $s2, $100;",
// false[7]
  "cmp_ge_b1_b32 $c1, $s2, c100;",
// false[8]
  "cmmp_ge_b1_b32 $c1, $s2, $c100;",
// false[9]
  "cmp_ge_b32_b32 $d1;",
// false[10]
  "cmp_ge_b32_b32 $s1, $s2;",
// false[11]
  "cmp_ge_b32_b32 $s1, $s2,;",
// false[12]
  "cmp_ge_b32_b32 $s1, $s2, $s3,",
// false[13]
  "cmp_ge_b32_b32 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_ge_b32_b32 $s1, $s2, $c100;",
// false[15]
  "cmp_ge_b32_b32 $s1, $s2, $100;",
// false[16]
  "cmp_ge_b32_b32 $s1, $s2, c100;",
// false[17]
  "cmmp_ge_b32_b32 $s1, $s2, $c100;",
// false[18]
  "cmp_ge_u32_b32 $d1;",
// false[19]
  "cmp_ge_u32_b32 $s1, $s2;",
// false[20]
  "cmp_ge_u32_b32 $s1, $s2,;",
// false[21]
  "cmp_ge_u32_b32 $s1, $s2, $s3,",
// false[22]
  "cmp_ge_u32_b32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_ge_u32_b32 $s1, $s2, $c100;",
// false[24]
  "cmp_ge_u32_b32 $s1, $s2, $100;",
// false[25]
  "cmp_ge_u32_b32 $s1, $s2, c100;",
// false[26]
  "cmmp_ge_u32_b32 $s1, $s2, $c100;",
// false[27]
  "cmp_ge_s32_b32 $d1;",
// false[28]
  "cmp_ge_s32_b32 $s1, $s2;",
// false[29]
  "cmp_ge_s32_b32 $s1, $s2,;",
// false[30]
  "cmp_ge_s32_b32 $s1, $s2, $s3,",
// false[31]
  "cmp_ge_s32_b32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_ge_s32_b32 $s1, $s2, $c100;",
// false[33]
  "cmp_ge_s32_b32 $s1, $s2, $100;",
// false[34]
  "cmp_ge_s32_b32 $s1, $s2, c100;",
// false[35]
  "cmmp_ge_s32_b32 $s1, $s2, $c100;",
// false[36]
  "cmp_ge_f32_b32 $d1;",
// false[37]
  "cmp_ge_f32_b32 $s1, $s2;",
// false[38]
  "cmp_ge_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_ge_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_ge_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_ge_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_ge_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_ge_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_ge_f32_b32 $s1, $s2, $c100;",
// false[45]
  "cmp_ge_b32_b32 $d1;",
// false[46]
  "cmp_ge_b32_b32 $s1, $s2;",
// false[47]
  "cmp_ge_b32_b64 $s1, $s2,;",
// false[48]
  "cmp_ge_b32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_ge_b32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_ge_b32_s32 $s1, $s2, $s100;",
// false[51]
  "cmp_ge_b32_s64 $s1, $s2, $100;",
// false[52]
  "cmp_ge_b32_f32 $s1, $s2, s100;",
// false[53]
  "cmmp_ge_b32_f64 $s1, $d2, $d100;",
// false[54]
  "cmp_ge_b32_u32 1, $s2, $s3;",
// false[55]
  "cmp_ge_b32_u32 WAVESIZE, $s2, $s3;",
// false[56]
  "cmp_ge_b32_b1 $s1, $c2, $c3;",
// false[57]
  "cmp_get_b32_b32 $s1, $s2, $s3;"
};

class TestCmpEqu : public ::testing::TestWithParam<int>
{

};
struct CmpEquTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpEquTest cmp_equ_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_equ_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEqu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_equ_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_equ_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_equ_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_equ_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_equ_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_equ_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_equ_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_equ_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_equ_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_equ_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_equ_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_equ_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_equ_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ftz_equ_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigEqu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_equ_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf64,
      0
    }
  }
};

class TestCmpEquInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_equ_invalid[51] = {
// false[0]
  "cmp_equ_f32_f32 $c1;",
// false[1]
  "cmp_equ_f32_f32 $c1, $s2;",
// false[2]
  "cmp_equ_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_equ_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_equ_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_equ_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_equ_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_equ_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_equ_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_equ_f32_f64 $d1;",
// false[10]
  "cmp_equ_f32_f64 $s1, $s2;",
// false[11]
  "cmp_equ_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_equ_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_equ_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_equ_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_equ_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_equ_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_equ_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_equ_u32_f32 $d1;",
// false[19]
  "cmp_equ_u32_f32 $s1, $s2;",
// false[20]
  "cmp_equ_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_equ_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_equ_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_equ_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_equ_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_equ_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_equ_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_equ_s32_f32 $d1;",
// false[28]
  "cmp_equ_s32_f32 $s1, $s2;",
// false[29]
  "cmp_equ_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_equ_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_equ_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_equ_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_equ_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_equ_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_equ_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_equ_f32_b32 $d1;",
// false[37]
  "cmp_equ_f32_b32 $s1, $s2;",
// false[38]
  "cmp_equ_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_equ_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_equ_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_equ_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_equ_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_equ_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_equ_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_equ_f32_s32 $d1;",
// false[46]
  "cmp_equ_f32_s32 $s1, $s2;",
// false[47]
  "cmp_equ_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_equ_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_equ_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_equ_f32_b1 $s1, $c2, $c3;"
};

class TestCmpNeu : public ::testing::TestWithParam<int>
{

};
struct CmpNeuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpNeuTest cmp_neu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_neu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigNeu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_neu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_neu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_neu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_neu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_neu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_neu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_neu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_neu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_neu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_neu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_neu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_neu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_neu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_neu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_neu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigNeu,
      Brigf64,
      0
    }
  }
};

class TestCmpNeuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_neu_invalid[51] = {
// false[0]
  "cmp_neu_f32_f32 $c1;",
// false[1]
  "cmp_neu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_neu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_neu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_neu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_neu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_neu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_neu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_neu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_neu_f32_f64 $d1;",
// false[10]
  "cmp_neu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_neu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_neu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_neu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_neu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_neu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_neu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_neu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_neu_u32_f32 $d1;",
// false[19]
  "cmp_neu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_neu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_neu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_neu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_neu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_neu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_neu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_neu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_neu_s32_f32 $d1;",
// false[28]
  "cmp_neu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_neu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_neu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_neu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_neu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_neu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_neu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_neu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_neu_f32_b32 $d1;",
// false[37]
  "cmp_neu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_neu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_neu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_neu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_neu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_neu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_neu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_neu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_neu_f32_s32 $d1;",
// false[46]
  "cmp_neu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_neu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_neu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_neu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_neu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpLeu : public ::testing::TestWithParam<int>
{

};
struct CmpLeuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpLeuTest cmp_leu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_leu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigLeu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_leu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_leu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_leu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_leu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_leu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_leu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_leu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_leu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_leu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_leu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_leu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_leu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_leu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_leu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_leu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigLeu,
      Brigf64,
      0
    }
  }
};

class TestCmpLeuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_leu_invalid[51] = {
// false[0]
  "cmp_leu_f32_f32 $c1;",
// false[1]
  "cmp_leu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_leu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_leu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_leu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_leu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_leu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_leu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_leu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_leu_f32_f64 $d1;",
// false[10]
  "cmp_leu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_leu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_leu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_leu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_leu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_leu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_leu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_leu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_leu_u32_f32 $d1;",
// false[19]
  "cmp_leu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_leu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_leu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_leu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_leu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_leu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_leu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_leu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_leu_s32_f32 $d1;",
// false[28]
  "cmp_leu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_leu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_leu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_leu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_leu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_leu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_leu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_leu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_leu_f32_b32 $d1;",
// false[37]
  "cmp_leu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_leu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_leu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_leu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_leu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_leu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_leu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_leu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_leu_f32_s32 $d1;",
// false[46]
  "cmp_leu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_leu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_leu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_leu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_leu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpLtu : public ::testing::TestWithParam<int>
{

};
struct CmpLtuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpLtuTest cmp_ltu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_ltu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigLtu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_ltu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_ltu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_ltu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_ltu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_ltu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_ltu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_ltu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_ltu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_ltu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_ltu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_ltu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_ltu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_ltu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ltu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_ltu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigLtu,
      Brigf64,
      0
    }
  }
};

class TestCmpLtuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_ltu_invalid[51] = {
// false[0]
  "cmp_ltu_f32_f32 $c1;",
// false[1]
  "cmp_ltu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_ltu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_ltu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_ltu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_ltu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_ltu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_ltu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_ltu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_ltu_f32_f64 $d1;",
// false[10]
  "cmp_ltu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_ltu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_ltu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_ltu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_ltu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_ltu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_ltu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_ltu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_ltu_u32_f32 $d1;",
// false[19]
  "cmp_ltu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_ltu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_ltu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_ltu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_ltu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_ltu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_ltu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_ltu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_ltu_s32_f32 $d1;",
// false[28]
  "cmp_ltu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_ltu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_ltu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_ltu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_ltu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_ltu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_ltu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_ltu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_ltu_f32_b32 $d1;",
// false[37]
  "cmp_ltu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_ltu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_ltu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_ltu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_ltu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_ltu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_ltu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_ltu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_ltu_f32_s32 $d1;",
// false[46]
  "cmp_ltu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_ltu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_ltu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_ltu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_ltu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpGtu : public ::testing::TestWithParam<int>
{

};
struct CmpGtuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpGtuTest cmp_gtu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_gtu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigGtu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_gtu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_gtu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_gtu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_gtu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_gtu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_gtu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_gtu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_gtu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_gtu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_gtu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_gtu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_gtu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_gtu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_gtu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_gtu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigGtu,
      Brigf64,
      0
    }
  }
};

class TestCmpGtuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_gtu_invalid[51] = {
// false[0]
  "cmp_gtu_f32_f32 $c1;",
// false[1]
  "cmp_gtu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_gtu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_gtu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_gtu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_gtu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_gtu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_gtu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_gtu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_gtu_f32_f64 $d1;",
// false[10]
  "cmp_gtu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_gtu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_gtu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_gtu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_gtu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_gtu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_gtu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_gtu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_gtu_u32_f32 $d1;",
// false[19]
  "cmp_gtu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_gtu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_gtu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_gtu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_gtu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_gtu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_gtu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_gtu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_gtu_s32_f32 $d1;",
// false[28]
  "cmp_gtu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_gtu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_gtu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_gtu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_gtu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_gtu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_gtu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_gtu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_gtu_f32_b32 $d1;",
// false[37]
  "cmp_gtu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_gtu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_gtu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_gtu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_gtu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_gtu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_gtu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_gtu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_gtu_f32_s32 $d1;",
// false[46]
  "cmp_gtu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_gtu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_gtu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_gtu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_gtu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpGeu : public ::testing::TestWithParam<int>
{

};
struct CmpGeuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpGeuTest cmp_geu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_geu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigGeu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_geu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_geu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_geu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_geu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_geu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_geu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_geu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_geu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_geu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_geu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_geu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_geu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_geu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_geu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_geu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigGeu,
      Brigf64,
      0
    }
  }
};

class TestCmpGeuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_geu_invalid[51] = {
// false[0]
  "cmp_geu_f32_f32 $c1;",
// false[1]
  "cmp_geu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_geu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_geu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_geu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_geu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_geu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_geu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_geu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_geu_f32_f64 $d1;",
// false[10]
  "cmp_geu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_geu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_geu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_geu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_geu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_geu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_geu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_geu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_geu_u32_f32 $d1;",
// false[19]
  "cmp_geu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_geu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_geu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_geu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_geu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_geu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_geu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_geu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_geu_s32_f32 $d1;",
// false[28]
  "cmp_geu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_geu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_geu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_geu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_geu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_geu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_geu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_geu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_geu_f32_b32 $d1;",
// false[37]
  "cmp_geu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_geu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_geu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_geu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_geu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_geu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_geu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_geu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_geu_f32_s32 $d1;",
// false[46]
  "cmp_geu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_geu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_geu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_geu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_geu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpNum : public ::testing::TestWithParam<int>
{

};
struct CmpNumTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpNumTest cmp_num_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_num_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigNum,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_num_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_num_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_num_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_num_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_num_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_num_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_num_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_num_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_num_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_num_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_num_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_num_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_num_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_num_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_num_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigNum,
      Brigf64,
      0
    }
  }
};

class TestCmpNumInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_num_invalid[51] = {
// false[0]
  "cmp_num_f32_f32 $c1;",
// false[1]
  "cmp_num_f32_f32 $c1, $s2;",
// false[2]
  "cmp_num_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_num_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_num_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_num_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_num_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_num_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_num_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_num_f32_f64 $d1;",
// false[10]
  "cmp_num_f32_f64 $s1, $s2;",
// false[11]
  "cmp_num_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_num_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_num_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_num_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_num_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_num_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_num_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_num_u32_f32 $d1;",
// false[19]
  "cmp_num_u32_f32 $s1, $s2;",
// false[20]
  "cmp_num_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_num_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_num_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_num_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_num_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_num_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_num_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_num_s32_f32 $d1;",
// false[28]
  "cmp_num_s32_f32 $s1, $s2;",
// false[29]
  "cmp_num_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_num_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_num_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_num_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_num_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_num_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_num_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_num_f32_b32 $d1;",
// false[37]
  "cmp_num_f32_b32 $s1, $s2;",
// false[38]
  "cmp_num_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_num_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_num_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_num_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_num_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_num_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_num_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_num_f32_s32 $d1;",
// false[46]
  "cmp_num_f32_s32 $s1, $s2;",
// false[47]
  "cmp_num_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_num_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_num_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_num_f32_b1 $s1, $c2, $c3;"
};

class TestCmpNan : public ::testing::TestWithParam<int>
{

};
struct CmpNanTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpNanTest cmp_nan_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_nan_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigNan,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_nan_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_nan_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_nan_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_nan_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_nan_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_nan_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_nan_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_nan_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_nan_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_nan_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_nan_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigu64,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_nan_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigs32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_nan_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigs64,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_nan_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigf32,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_nan_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigNan,
      Brigf64,
      0
    }
  }
};

class TestCmpNanInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_nan_invalid[51] = {
// false[0]
  "cmp_nan_f32_f32 $c1;",
// false[1]
  "cmp_nan_f32_f32 $c1, $s2;",
// false[2]
  "cmp_nan_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_nan_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_nan_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_nan_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_nan_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_nan_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_nan_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_nan_f32_f64 $d1;",
// false[10]
  "cmp_nan_f32_f64 $s1, $s2;",
// false[11]
  "cmp_nan_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_nan_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_nan_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_nan_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_nan_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_nan_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_nan_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_nan_u32_f32 $d1;",
// false[19]
  "cmp_nan_u32_f32 $s1, $s2;",
// false[20]
  "cmp_nan_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_nan_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_nan_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_nan_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_nan_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_nan_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_nan_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_nan_s32_f32 $d1;",
// false[28]
  "cmp_nan_s32_f32 $s1, $s2;",
// false[29]
  "cmp_nan_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_nan_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_nan_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_nan_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_nan_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_nan_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_nan_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_nan_f32_b32 $d1;",
// false[37]
  "cmp_nan_f32_b32 $s1, $s2;",
// false[38]
  "cmp_nan_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_nan_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_nan_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_nan_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_nan_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_nan_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_nan_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_nan_f32_s32 $d1;",
// false[46]
  "cmp_nan_f32_s32 $s1, $s2;",
// false[47]
  "cmp_nan_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_nan_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_nan_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_nan_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSeq : public ::testing::TestWithParam<int>
{

};
struct CmpSeqTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSeqTest cmp_seq_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_seq_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSeq,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_seq_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_seq_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_seq_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_seq_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_seq_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_seq_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_seq_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_seq_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_seq_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_seq_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_seq_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_seq_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_seq_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_seq_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_seq_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSeq,
      Brigu64,
      0
    }
  }
};

class TestCmpSeqInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_seq_invalid[51] = {
// false[0]
  "cmp_seq_f32_f32 $c1;",
// false[1]
  "cmp_seq_f32_f32 $c1, $s2;",
// false[2]
  "cmp_seq_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_seq_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_seq_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_seq_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_seq_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_seq_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_seq_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_seq_f32_f64 $d1;",
// false[10]
  "cmp_seq_f32_f64 $s1, $s2;",
// false[11]
  "cmp_seq_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_seq_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_seq_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_seq_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_seq_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_seq_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_seq_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_seq_u32_f32 $d1;",
// false[19]
  "cmp_seq_u32_f32 $s1, $s2;",
// false[20]
  "cmp_seq_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_seq_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_seq_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_seq_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_seq_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_seq_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_seq_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_seq_s32_f32 $d1;",
// false[28]
  "cmp_seq_s32_f32 $s1, $s2;",
// false[29]
  "cmp_seq_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_seq_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_seq_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_seq_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_seq_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_seq_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_seq_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_seq_f32_b32 $d1;",
// false[37]
  "cmp_seq_f32_b32 $s1, $s2;",
// false[38]
  "cmp_seq_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_seq_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_seq_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_seq_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_seq_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_seq_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_seq_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_seq_f32_s32 $d1;",
// false[46]
  "cmp_seq_f32_s32 $s1, $s2;",
// false[47]
  "cmp_seq_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_seq_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_seq_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_seq_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSne : public ::testing::TestWithParam<int>
{

};
struct CmpSneTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSneTest cmp_sne_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sne_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSne,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sne_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sne_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sne_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sne_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sne_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sne_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sne_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sne_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sne_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sne_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sne_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sne_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sne_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sne_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_sne_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSne,
      Brigu64,
      0
    }
  }
};

class TestCmpSneInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sne_invalid[51] = {
// false[0]
  "cmp_sne_f32_f32 $c1;",
// false[1]
  "cmp_sne_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sne_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sne_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sne_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sne_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sne_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sne_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sne_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sne_f32_f64 $d1;",
// false[10]
  "cmp_sne_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sne_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sne_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sne_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sne_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sne_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sne_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sne_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sne_u32_f32 $d1;",
// false[19]
  "cmp_sne_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sne_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sne_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sne_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sne_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sne_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sne_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sne_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sne_s32_f32 $d1;",
// false[28]
  "cmp_sne_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sne_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sne_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sne_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sne_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sne_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sne_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sne_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sne_f32_b32 $d1;",
// false[37]
  "cmp_sne_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sne_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sne_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sne_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sne_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sne_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sne_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sne_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sne_f32_s32 $d1;",
// false[46]
  "cmp_sne_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sne_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sne_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sne_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sne_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSlt : public ::testing::TestWithParam<int>
{

};
struct CmpSltTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSltTest cmp_slt_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_slt_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSlt,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_slt_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_slt_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_slt_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_slt_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_slt_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_slt_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_slt_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_slt_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_slt_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_slt_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_slt_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_slt_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_slt_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ftz_slt_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigSlt,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_slt_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigu64,
      0
    }
  }
};

class TestCmpSltInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_slt_invalid[51] = {
// false[0]
  "cmp_slt_f32_f32 $c1;",
// false[1]
  "cmp_slt_f32_f32 $c1, $s2;",
// false[2]
  "cmp_slt_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_slt_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_slt_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_slt_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_slt_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_slt_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_slt_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_slt_f32_f64 $d1;",
// false[10]
  "cmp_slt_f32_f64 $s1, $s2;",
// false[11]
  "cmp_slt_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_slt_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_slt_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_slt_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_slt_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_slt_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_slt_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_slt_u32_f32 $d1;",
// false[19]
  "cmp_slt_u32_f32 $s1, $s2;",
// false[20]
  "cmp_slt_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_slt_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_slt_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_slt_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_slt_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_slt_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_slt_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_slt_s32_f32 $d1;",
// false[28]
  "cmp_slt_s32_f32 $s1, $s2;",
// false[29]
  "cmp_slt_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_slt_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_slt_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_slt_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_slt_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_slt_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_slt_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_slt_f32_b32 $d1;",
// false[37]
  "cmp_slt_f32_b32 $s1, $s2;",
// false[38]
  "cmp_slt_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_slt_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_slt_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_slt_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_slt_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_slt_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_slt_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_slt_f32_s32 $d1;",
// false[46]
  "cmp_slt_f32_s32 $s1, $s2;",
// false[47]
  "cmp_slt_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_slt_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_slt_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_slt_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSle : public ::testing::TestWithParam<int>
{

};
struct CmpSleTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSleTest cmp_sle_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sle_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSle,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sle_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sle_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sle_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sle_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sle_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sle_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sle_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sle_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sle_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sle_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sle_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sle_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sle_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sle_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_sle_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSle,
      Brigu64,
      0
    }
  }
};

class TestCmpSleInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sle_invalid[51] = {
// false[0]
  "cmp_sle_f32_f32 $c1;",
// false[1]
  "cmp_sle_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sle_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sle_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sle_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sle_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sle_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sle_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sle_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sle_f32_f64 $d1;",
// false[10]
  "cmp_sle_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sle_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sle_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sle_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sle_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sle_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sle_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sle_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sle_u32_f32 $d1;",
// false[19]
  "cmp_sle_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sle_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sle_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sle_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sle_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sle_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sle_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sle_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sle_s32_f32 $d1;",
// false[28]
  "cmp_sle_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sle_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sle_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sle_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sle_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sle_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sle_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sle_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sle_f32_b32 $d1;",
// false[37]
  "cmp_sle_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sle_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sle_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sle_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sle_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sle_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sle_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sle_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sle_f32_s32 $d1;",
// false[46]
  "cmp_sle_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sle_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sle_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sle_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sle_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSgt : public ::testing::TestWithParam<int>
{

};
struct CmpSgtTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSgtTest cmp_sgt_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sgt_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSgt,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sgt_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sgt_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sgt_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sgt_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sgt_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sgt_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sgt_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sgt_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sgt_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sgt_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sgt_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sgt_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sgt_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sgt_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_sgt_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSgt,
      Brigu64,
      0
    }
  }
};

class TestCmpSgtInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sgt_invalid[51] = {
// false[0]
  "cmp_sgt_f32_f32 $c1;",
// false[1]
  "cmp_sgt_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sgt_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sgt_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sgt_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sgt_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sgt_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sgt_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sgt_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sgt_f32_f64 $d1;",
// false[10]
  "cmp_sgt_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sgt_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sgt_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sgt_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sgt_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sgt_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sgt_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sgt_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sgt_u32_f32 $d1;",
// false[19]
  "cmp_sgt_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sgt_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sgt_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sgt_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sgt_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sgt_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sgt_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sgt_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sgt_s32_f32 $d1;",
// false[28]
  "cmp_sgt_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sgt_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sgt_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sgt_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sgt_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sgt_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sgt_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sgt_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sgt_f32_b32 $d1;",
// false[37]
  "cmp_sgt_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sgt_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sgt_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sgt_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sgt_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sgt_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sgt_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sgt_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sgt_f32_s32 $d1;",
// false[46]
  "cmp_sgt_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sgt_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sgt_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sgt_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sgt_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSge : public ::testing::TestWithParam<int>
{

};
struct CmpSgeTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSgeTest cmp_sge_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sge_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSge,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sge_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sge_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sge_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sge_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sge_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sge_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sge_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sge_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sge_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sge_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sge_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sge_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sge_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sge_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_sge_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSge,
      Brigu64,
      0
    }
  }
};

class TestCmpSgeInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sge_invalid[51] = {
// false[0]
  "cmp_sge_f32_f32 $c1;",
// false[1]
  "cmp_sge_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sge_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sge_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sge_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sge_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sge_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sge_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sge_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sge_f32_f64 $d1;",
// false[10]
  "cmp_sge_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sge_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sge_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sge_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sge_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sge_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sge_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sge_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sge_u32_f32 $d1;",
// false[19]
  "cmp_sge_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sge_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sge_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sge_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sge_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sge_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sge_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sge_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sge_s32_f32 $d1;",
// false[28]
  "cmp_sge_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sge_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sge_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sge_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sge_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sge_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sge_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sge_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sge_f32_b32 $d1;",
// false[37]
  "cmp_sge_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sge_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sge_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sge_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sge_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sge_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sge_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sge_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sge_f32_s32 $d1;",
// false[46]
  "cmp_sge_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sge_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sge_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sge_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sge_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSequ : public ::testing::TestWithParam<int>
{

};
struct CmpSequTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSequTest cmp_sequ_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sequ_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSequ,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sequ_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sequ_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sequ_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sequ_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sequ_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sequ_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sequ_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sequ_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sequ_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sequ_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sequ_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sequ_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sequ_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_ftz_sequ_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_sequ_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSequ,
      Brigu64,
      0
    }
  }
};

class TestCmpSequInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sequ_invalid[51] = {
// false[0]
  "cmp_sequ_f32_f32 $c1;",
// false[1]
  "cmp_sequ_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sequ_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sequ_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sequ_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sequ_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sequ_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sequ_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sequ_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sequ_f32_f64 $d1;",
// false[10]
  "cmp_sequ_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sequ_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sequ_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sequ_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sequ_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sequ_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sequ_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sequ_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sequ_u32_f32 $d1;",
// false[19]
  "cmp_sequ_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sequ_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sequ_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sequ_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sequ_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sequ_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sequ_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sequ_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sequ_s32_f32 $d1;",
// false[28]
  "cmp_sequ_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sequ_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sequ_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sequ_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sequ_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sequ_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sequ_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sequ_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sequ_f32_b32 $d1;",
// false[37]
  "cmp_sequ_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sequ_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sequ_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sequ_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sequ_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sequ_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sequ_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sequ_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sequ_f32_s32 $d1;",
// false[46]
  "cmp_sequ_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sequ_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sequ_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sequ_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sequ_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSneu : public ::testing::TestWithParam<int>
{

};
struct CmpSneuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSneuTest cmp_sneu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sneu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSneu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sneu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sneu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sneu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sneu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sneu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sneu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sneu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sneu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sneu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sneu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sneu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sneu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sneu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sneu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_sneu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigu64,
      0
    }
  }
};

class TestCmpSneuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sneu_invalid[51] = {
// false[0]
  "cmp_sneu_f32_f32 $c1;",
// false[1]
  "cmp_sneu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sneu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sneu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sneu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sneu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sneu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sneu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sneu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sneu_f32_f64 $d1;",
// false[10]
  "cmp_sneu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sneu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sneu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sneu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sneu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sneu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sneu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sneu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sneu_u32_f32 $d1;",
// false[19]
  "cmp_sneu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sneu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sneu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sneu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sneu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sneu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sneu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sneu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sneu_s32_f32 $d1;",
// false[28]
  "cmp_sneu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sneu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sneu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sneu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sneu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sneu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sneu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sneu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sneu_f32_b32 $d1;",
// false[37]
  "cmp_sneu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sneu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sneu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sneu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sneu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sneu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sneu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sneu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sneu_f32_s32 $d1;",
// false[46]
  "cmp_sneu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sneu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sneu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sneu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sneu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSleu : public ::testing::TestWithParam<int>
{

};
struct CmpSleuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSleuTest cmp_sleu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sleu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSleu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sleu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sleu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sleu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sleu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sleu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sleu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sleu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sleu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sleu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sleu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sleu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sleu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sleu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sleu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_sleu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigu64,
      0
    }
  }
};

class TestCmpSleuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sleu_invalid[51] = {
// false[0]
  "cmp_sleu_f32_f32 $c1;",
// false[1]
  "cmp_sleu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sleu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sleu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sleu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sleu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sleu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sleu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sleu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sleu_f32_f64 $d1;",
// false[10]
  "cmp_sleu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sleu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sleu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sleu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sleu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sleu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sleu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sleu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sleu_u32_f32 $d1;",
// false[19]
  "cmp_sleu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sleu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sleu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sleu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sleu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sleu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sleu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sleu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sleu_s32_f32 $d1;",
// false[28]
  "cmp_sleu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sleu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sleu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sleu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sleu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sleu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sleu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sleu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sleu_f32_b32 $d1;",
// false[37]
  "cmp_sleu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sleu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sleu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sleu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sleu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sleu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sleu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sleu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sleu_f32_s32 $d1;",
// false[46]
  "cmp_sleu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sleu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sleu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sleu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sleu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSltu : public ::testing::TestWithParam<int>
{

};
struct CmpSltuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSltuTest cmp_sltu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sltu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSltu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sltu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sltu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sltu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sltu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sltu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sltu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sltu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sltu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sltu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sltu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sltu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sltu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sltu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sltu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_ftz_sltu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSltu,
      Brigu64,
      0
    }
  }
};

class TestCmpSltuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sltu_invalid[51] = {
// false[0]
  "cmp_sltu_f32_f32 $c1;",
// false[1]
  "cmp_sltu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sltu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sltu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sltu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sltu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sltu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sltu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sltu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sltu_f32_f64 $d1;",
// false[10]
  "cmp_sltu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sltu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sltu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sltu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sltu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sltu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sltu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sltu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sltu_u32_f32 $d1;",
// false[19]
  "cmp_sltu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sltu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sltu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sltu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sltu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sltu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sltu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sltu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sltu_s32_f32 $d1;",
// false[28]
  "cmp_sltu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sltu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sltu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sltu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sltu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sltu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sltu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sltu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sltu_f32_b32 $d1;",
// false[37]
  "cmp_sltu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sltu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sltu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sltu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sltu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sltu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sltu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sltu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sltu_f32_s32 $d1;",
// false[46]
  "cmp_sltu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sltu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sltu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sltu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sltu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSgtu : public ::testing::TestWithParam<int>
{

};
struct CmpSgtuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSgtuTest cmp_sgtu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sgtu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSgtu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sgtu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sgtu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sgtu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sgtu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sgtu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sgtu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sgtu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sgtu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sgtu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sgtu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sgtu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sgtu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sgtu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sgtu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_sgtu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigu64,
      0
    }
  }
};

class TestCmpSgtuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sgtu_invalid[51] = {
// false[0]
  "cmp_sgtu_f32_f32 $c1;",
// false[1]
  "cmp_sgtu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sgtu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sgtu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sgtu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sgtu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sgtu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sgtu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sgtu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sgtu_f32_f64 $d1;",
// false[10]
  "cmp_sgtu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sgtu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sgtu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sgtu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sgtu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sgtu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sgtu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sgtu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sgtu_u32_f32 $d1;",
// false[19]
  "cmp_sgtu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sgtu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sgtu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sgtu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sgtu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sgtu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sgtu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sgtu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sgtu_s32_f32 $d1;",
// false[28]
  "cmp_sgtu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sgtu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sgtu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sgtu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sgtu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sgtu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sgtu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sgtu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sgtu_f32_b32 $d1;",
// false[37]
  "cmp_sgtu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sgtu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sgtu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sgtu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sgtu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sgtu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sgtu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sgtu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sgtu_f32_s32 $d1;",
// false[46]
  "cmp_sgtu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sgtu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sgtu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sgtu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sgtu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSgeu : public ::testing::TestWithParam<int>
{

};
struct CmpSgeuTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSgeuTest cmp_sgeu_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_sgeu_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSgeu,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_sgeu_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_sgeu_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_sgeu_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_sgeu_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_sgeu_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_sgeu_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_sgeu_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_sgeu_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_sgeu_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_sgeu_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_sgeu_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_sgeu_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_sgeu_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sgeu_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_sgeu_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigu64,
      0
    }
  }
};

class TestCmpSgeuInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_sgeu_invalid[51] = {
// false[0]
  "cmp_sgeu_f32_f32 $c1;",
// false[1]
  "cmp_sgeu_f32_f32 $c1, $s2;",
// false[2]
  "cmp_sgeu_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_sgeu_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_sgeu_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_sgeu_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_sgeu_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_sgeu_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_sgeu_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_sgeu_f32_f64 $d1;",
// false[10]
  "cmp_sgeu_f32_f64 $s1, $s2;",
// false[11]
  "cmp_sgeu_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_sgeu_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_sgeu_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_sgeu_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_sgeu_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_sgeu_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_sgeu_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_sgeu_u32_f32 $d1;",
// false[19]
  "cmp_sgeu_u32_f32 $s1, $s2;",
// false[20]
  "cmp_sgeu_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_sgeu_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_sgeu_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_sgeu_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_sgeu_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_sgeu_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_sgeu_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_sgeu_s32_f32 $d1;",
// false[28]
  "cmp_sgeu_s32_f32 $s1, $s2;",
// false[29]
  "cmp_sgeu_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_sgeu_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_sgeu_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_sgeu_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_sgeu_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_sgeu_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_sgeu_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_sgeu_f32_b32 $d1;",
// false[37]
  "cmp_sgeu_f32_b32 $s1, $s2;",
// false[38]
  "cmp_sgeu_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_sgeu_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_sgeu_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_sgeu_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_sgeu_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_sgeu_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_sgeu_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_sgeu_f32_s32 $d1;",
// false[46]
  "cmp_sgeu_f32_s32 $s1, $s2;",
// false[47]
  "cmp_sgeu_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_sgeu_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_sgeu_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_sgeu_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSnum : public ::testing::TestWithParam<int>
{

};
struct CmpSnumTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSnumTest cmp_snum_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_snum_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSnum,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_snum_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_snum_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_snum_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_snum_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_snum_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_snum_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_snum_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_snum_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_snum_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_snum_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_snum_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_snum_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_snum_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_snum_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_snum_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigu64,
      0
    }
  }
};

class TestCmpSnumInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_snum_invalid[51] = {
// false[0]
  "cmp_snum_f32_f32 $c1;",
// false[1]
  "cmp_snum_f32_f32 $c1, $s2;",
// false[2]
  "cmp_snum_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_snum_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_snum_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_snum_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_snum_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_snum_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_snum_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_snum_f32_f64 $d1;",
// false[10]
  "cmp_snum_f32_f64 $s1, $s2;",
// false[11]
  "cmp_snum_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_snum_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_snum_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_snum_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_snum_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_snum_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_snum_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_snum_u32_f32 $d1;",
// false[19]
  "cmp_snum_u32_f32 $s1, $s2;",
// false[20]
  "cmp_snum_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_snum_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_snum_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_snum_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_snum_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_snum_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_snum_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_snum_s32_f32 $d1;",
// false[28]
  "cmp_snum_s32_f32 $s1, $s2;",
// false[29]
  "cmp_snum_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_snum_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_snum_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_snum_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_snum_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_snum_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_snum_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_snum_f32_b32 $d1;",
// false[37]
  "cmp_snum_f32_b32 $s1, $s2;",
// false[38]
  "cmp_snum_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_snum_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_snum_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_snum_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_snum_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_snum_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_snum_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_snum_f32_s32 $d1;",
// false[46]
  "cmp_snum_f32_s32 $s1, $s2;",
// false[47]
  "cmp_snum_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_snum_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_snum_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_snum_f32_b1 $s1, $c2, $c3;"
};

class TestCmpSnan : public ::testing::TestWithParam<int>
{

};
struct CmpSnanTest{
  const char *str;
  BrigInstCmp ref;
};

struct CmpSnanTest cmp_snan_pair[16] = {
// cmp_pair[0]
  {
    "cmp_ftz_snan_b1_f32 $c1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[1],
      BrigSnan,
      Brigf32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_snan_b1_f64 $c1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb1,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf64,
      0
    }
  },

// cmp_pair[2]
  {
    "cmp_snan_b32_f32 $s1, WAVESIZE, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf32,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_snan_b32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigb32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf64,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_snan_u32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf32,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_snan_u32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf64,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_snan_s32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_snan_s32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf64,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_snan_f32_b32 $s1, $s2, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigb32,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_snan_f32_b64 $s1, $d2, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigb64,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_snan_f32_u32 $s1, 1, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigu32,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_snan_f32_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigs32,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_snan_f32_s64 $s1, WAVESIZE, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigs64,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_snan_f32_f32 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf32,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_snan_f32_f64 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf64,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_snan_f32_u64 $s1, 1, 0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigu64,
      0
    }
  }
};

class TestCmpSnanInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_cmp_snan_invalid[51] = {
// false[0]
  "cmp_snan_f32_f32 $c1;",
// false[1]
  "cmp_snan_f32_f32 $c1, $s2;",
// false[2]
  "cmp_snan_f32_f32 $c1, $s2,;",
// false[3]
  "cmp_snan_f32_f32 $c1, $s2, $s3,",
// false[4]
  "cmp_snan_f32_f32 $c1, $s2, $s3, $c4;",
// false[5]
  "cmp_snan_f32_f32 $c1, $s2, $c100;",
// false[6]
  "cmp_snan_f32_f32 $c1, $s2, $100;",
// false[7]
  "cmp_snan_f32_f32 $c1, $s2, c100;",
// false[8]
  "cmmp_snan_f32_f32 $c1, $s2, $c100;",
// false[9]
  "cmp_snan_f32_f64 $d1;",
// false[10]
  "cmp_snan_f32_f64 $s1, $s2;",
// false[11]
  "cmp_snan_f32_f64 $s1, $s2,;",
// false[12]
  "cmp_snan_f32_f64 $s1, $s2, $s3,",
// false[13]
  "cmp_snan_f32_f64 $s1, $s2, $s3, $c4;",
// false[14]
  "cmp_snan_f32_f64 $s1, $s2, $c100;",
// false[15]
  "cmp_snan_f32_f64 $s1, $s2, $100;",
// false[16]
  "cmp_snan_f32_f64 $s1, $s2, c100;",
// false[17]
  "cmmp_snan_f32_f64 $s1, $s2, $c100;",
// false[18]
  "cmp_snan_u32_f32 $d1;",
// false[19]
  "cmp_snan_u32_f32 $s1, $s2;",
// false[20]
  "cmp_snan_u32_f32 $s1, $s2,;",
// false[21]
  "cmp_snan_u32_f32 $s1, $s2, $s3,",
// false[22]
  "cmp_snan_u32_f32 $s1, $s2, $s3, $c4;",
// false[23]
  "cmp_snan_u32_f32 $s1, $s2, $c100;",
// false[24]
  "cmp_snan_u32_f32 $s1, $s2, $100;",
// false[25]
  "cmp_snan_u32_f32 $s1, $s2, c100;",
// false[26]
  "cmmp_snan_u32_f32 $s1, $s2, $c100;",
// false[27]
  "cmp_snan_s32_f32 $d1;",
// false[28]
  "cmp_snan_s32_f32 $s1, $s2;",
// false[29]
  "cmp_snan_s32_f32 $s1, $s2,;",
// false[30]
  "cmp_snan_s32_f32 $s1, $s2, $s3,",
// false[31]
  "cmp_snan_s32_f32 $s1, $s2, $s3, $c4;",
// false[32]
  "cmp_snan_s32_f32 $s1, $s2, $c100;",
// false[33]
  "cmp_snan_s32_f32 $s1, $s2, $100;",
// false[34]
  "cmp_snan_s32_f32 $s1, $s2, c100;",
// false[35]
  "cmmp_snan_s32_f32 $s1, $s2, $c100;",
// false[36]
  "cmp_snan_f32_b32 $d1;",
// false[37]
  "cmp_snan_f32_b32 $s1, $s2;",
// false[38]
  "cmp_snan_f32_b32 $s1, $s2,;",
// false[39]
  "cmp_snan_f32_b32 $s1, $s2, $s3,",
// false[40]
  "cmp_snan_f32_b32 $s1, $s2, $s3, $c4;",
// false[41]
  "cmp_snan_f32_b32 $s1, $s2, $c100;",
// false[42]
  "cmp_snan_f32_b32 $s1, $s2, $100;",
// false[43]
  "cmp_snan_f32_b32 $s1, $s2, c100;",
// false[44]
  "cmmp_snan_f32_s32 $s1, $s2, $c100;",
// false[45]
  "cmp_snan_f32_s32 $d1;",
// false[46]
  "cmp_snan_f32_s32 $s1, $s2;",
// false[47]
  "cmp_snan_f32_b64 $s1, $s2,;",
// false[48]
  "cmp_snan_f32_u32 $s1, $s2, $s3,",
// false[49]
  "cmp_snan_f32_u64 $s1, $s2, $s3, $s4;",
// false[50]
  "cmp_snan_f32_b1 $s1, $c2, $c3;"
};

class TestCmpF16 : public ::testing::TestWithParam<int>
{

};
struct CmpF16Test{
  const char *str;
  BrigInstCmp ref;
};

struct CmpF16Test cmp_f16_pair[27] = {
// cmp_pair[0]
  {
    "cmp_ftz_eq_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[1],
      BrigEq,
      Brigb32,
      0
    }
  },
// cmp_pair[1]
  {
    "cmp_ne_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNe,
      Brigf16,
      0
    }
  },
// cmp_pair[2]
  {
    "cmp_lt_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLt,
      Brigf16,
      0
    }
  },
// cmp_pair[3]
  {
    "cmp_le_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigLe,
      Brigb32,
      0
    }
  },
// cmp_pair[4]
  {
    "cmp_gt_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGe,
      Brigf16,
      0
    }
  },
// cmp_pair[5]
  {
    "cmp_equ_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigEqu,
      Brigf16,
      0
    }
  },
// cmp_pair[6]
  {
    "cmp_neu_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNeu,
      Brigb32,
      0
    }
  },
// cmp_pair[7]
  {
    "cmp_leu_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLeu,
      Brigf16,
      0
    }
  },
// cmp_pair[8]
  {
    "cmp_ltu_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigLtu,
      Brigf16,
      0
    }
  },
// cmp_pair[9]
  {
    "cmp_gtu_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigGtu,
      Brigb32,
      0
    }
  },
// cmp_pair[10]
  {
    "cmp_geu_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigGeu,
      Brigf16,
      0
    }
  },
// cmp_pair[11]
  {
    "cmp_num_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigNum,
      Brigf16,
      0
    }
  },
// cmp_pair[12]
  {
    "cmp_nan_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigNan,
      Brigb32,
      0
    }
  },
// cmp_pair[13]
  {
    "cmp_seq_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSeq,
      Brigf16,
      0
    }
  },
// cmp_pair[14]
  {
    "cmp_sne_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSne,
      Brigf16,
      0
    }
  },
// cmp_pair[15]
  {
    "cmp_slt_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSlt,
      Brigb32,
      0
    }
  },
// cmp_pair[16]
  {
    "cmp_sle_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSle,
      Brigf16,
      0
    }
  },
// cmp_pair[17]
  {
    "cmp_sgt_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgt,
      Brigf16,
      0
    }
  },
// cmp_pair[18]
  {
    "cmp_sge_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSge,
      Brigb32,
      0
    }
  },
// cmp_pair[19]
  {
    "cmp_sequ_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSequ,
      Brigf16,
      0
    }
  },
// cmp_pair[20]
  {
    "cmp_sneu_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSneu,
      Brigf16,
      0
    }
  },
// cmp_pair[21]
  {
    "cmp_sleu_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSleu,
      Brigb32,
      0
    }
  },
// cmp_pair[22]
  {
    "cmp_sltu_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSltu,
      Brigf16,
      0
    }
  },
// cmp_pair[23]
  {
    "cmp_sgtu_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSgtu,
      Brigf16,
      0
    }
  },
// cmp_pair[24]
  {
    "cmp_sgeu_f16_b32 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + 2 * reg_size,
        0,
        0
      },
      cmp_bam[0],
      BrigSgeu,
      Brigb32,
      0
    }
  },
// cmp_pair[25]
  {
    "cmp_snum_s32_f16 $s1, $s2, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + 2 * reg_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnum,
      Brigf16,
      0
    }
  },
// cmp_pair[26]
  {
    "cmp_snan_f16_f16 $s1, WAVESIZE, 0.0f;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigCmp,
      Brigf16,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      },
      cmp_bam[0],
      BrigSnan,
      Brigf16,
      0
    }
  }
};

BrigAluModifier packedcmp_bam[9] = {
  {0, 0, 0, 0, 0, 0, 0},//integer
  {1, 1, 0, 1, 0, 0, 0},// ftz
  {1, 1, 0, 0, 0, 0, 0},// near
  {1, 1, 1, 0, 0, 0, 0},// zero
  {1, 1, 2, 0, 0, 0, 0},// up
  {1, 1, 3, 0, 0, 0, 0},// down
  {1, 1, 1, 1, 0, 0, 0},// ftz_zero
  {1, 1, 2, 1, 0, 0, 0},// ftz_up
  {1, 1, 3, 1, 0, 0, 0} // ftz_down
};

class TestPackedCmpInteger : public ::testing::TestWithParam<int>
{

};
struct PackedCmpIntegerTest{
  const char *str;
  BrigInstCmp ref;
};

struct PackedCmpIntegerTest packedcmp_integer_pair[78] = {
// packedcmp_pair[0]
  {
    "packedcmp_eq_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[1]
  {
    "packedcmp_eq_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[2]
  {
    "packedcmp_eq_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[3]
  {
    "packedcmp_eq_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[4]
  {
    "packedcmp_eq_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[5]
  {
    "packedcmp_eq_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[6]
  {
    "packedcmp_eq_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[7]
  {
    "packedcmp_eq_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[8]
  {
    "packedcmp_eq_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[9]
  {
    "packedcmp_eq_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[10]
  {
    "packedcmp_ftz_eq_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[1],
      BrigEq,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[11]
  {
    "packedcmp_eq_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[12]
  {
    "packedcmp_eq_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEq,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[13]
  {
    "packedcmp_ne_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[14]
  {
    "packedcmp_ne_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[15]
  {
    "packedcmp_ne_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[16]
  {
    "packedcmp_ne_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[17]
  {
    "packedcmp_ne_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[18]
  {
    "packedcmp_ne_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[19]
  {
    "packedcmp_ne_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[20]
  {
    "packedcmp_ne_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[21]
  {
    "packedcmp_ne_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[22]
  {
    "packedcmp_ne_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[23]
  {
    "packedcmp_ne_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[24]
  {
    "packedcmp_ne_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[25]
  {
    "packedcmp_ne_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNe,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[26]
  {
    "packedcmp_lt_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[27]
  {
    "packedcmp_lt_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[28]
  {
    "packedcmp_lt_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[29]
  {
    "packedcmp_lt_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[30]
  {
    "packedcmp_lt_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[31]
  {
    "packedcmp_lt_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[32]
  {
    "packedcmp_lt_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[33]
  {
    "packedcmp_lt_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[34]
  {
    "packedcmp_lt_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[35]
  {
    "packedcmp_lt_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[36]
  {
    "packedcmp_ftz_lt_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[1],
      BrigLt,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[37]
  {
    "packedcmp_lt_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[38]
  {
    "packedcmp_lt_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLt,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[39]
  {
    "packedcmp_le_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[40]
  {
    "packedcmp_le_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[41]
  {
    "packedcmp_le_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[42]
  {
    "packedcmp_le_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[43]
  {
    "packedcmp_le_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[44]
  {
    "packedcmp_le_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[45]
  {
    "packedcmp_le_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[46]
  {
    "packedcmp_le_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[47]
  {
    "packedcmp_le_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[48]
  {
    "packedcmp_le_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[49]
  {
    "packedcmp_ftz_le_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[1],
      BrigLe,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[50]
  {
    "packedcmp_le_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[51]
  {
    "packedcmp_le_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLe,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[52]
  {
    "packedcmp_ge_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[53]
  {
    "packedcmp_ge_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[54]
  {
    "packedcmp_ge_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[55]
  {
    "packedcmp_ge_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[56]
  {
    "packedcmp_ge_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[57]
  {
    "packedcmp_ge_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[58]
  {
    "packedcmp_ge_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[59]
  {
    "packedcmp_ge_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[60]
  {
    "packedcmp_ge_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[61]
  {
    "packedcmp_ge_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[62]
  {
    "packedcmp_ge_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[63]
  {
    "packedcmp_ge_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[64]
  {
    "packedcmp_ge_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGe,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[65]
  {
    "packedcmp_gt_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigs8x4,
      0
    }
  },
// packedcmp_pair[66]
  {
    "packedcmp_gt_s16x2 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigs16x2,
      0
    }
  },
// packedcmp_pair[67]
  {
    "packedcmp_gt_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigs8x8,
      0
    }
  },
// packedcmp_pair[68]
  {
    "packedcmp_gt_s16x4 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigs16x4,
      0
    }
  },
// packedcmp_pair[69]
  {
    "packedcmp_gt_s32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigs32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigs32x2,
      0
    }
  },
// packedcmp_pair[70]
  {
    "packedcmp_gt_u8x4 $s1, $s2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigu8x4,
      0
    }
  },
// packedcmp_pair[71]
  {
    "packedcmp_gt_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigu16x2,
      0
    }
  },
// packedcmp_pair[72]
  {
    "packedcmp_gt_u8x8 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu8x8,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigu8x8,
      0
    }
  },
// packedcmp_pair[73]
  {
    "packedcmp_gt_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigu16x4,
      0
    }
  },
// packedcmp_pair[74]
  {
    "packedcmp_gt_u32x2 $d1, $d2, 1;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigu32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigu32x2,
      0
    }
  },
// packedcmp_pair[75]
  {
    "packedcmp_gt_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[76]
  {
    "packedcmp_gt_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[77]
  {
    "packedcmp_gt_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGt,
      Brigf32x2,
      0
    }
  }
};

class TestPackedCmpIntegerInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_packedcmp_integer_invalid[60] = {
// false[0]
  "packedcmp_ne_s8x4 $c1;",
// false[1]
  "packedcmp_ne_s8x4 $c1, $c2;",
// false[2]
  "packedcmp_ne_u8x4 $c1, $c2,;",
// false[3]
  "packedcmp_ne_u8x4 $c1, $c2, $c3,",
// false[4]
  "packedcmp_ne_s8x8 $c1, $c2, $c3, $c4;",
// false[5]
  "packedcmp_ne_u8x8 $d1, $d2, $d100;",
// false[6]
  "packedcmp_ne_u8x8 $d1, $d2, $100;",
// false[7]
  "packedcmp_ne_s16x2 $c1, $c2, c100;",
// false[8]
  "packedcmmp_ne_s16x2 $c1, $c2, $c100;",
// false[9]
  "packedcmp_ne_u16x2 $d1;",
// false[10]
  "packedcmp_lt_u16x2 $s1, $c2;",
// false[11]
  "packedcmp_lt_u16x2 $s1, $c2,;",
// false[12]
  "packedcmp_lt_u16x2 $s1, $c2, $c3,",
// false[13]
  "packedcmp_lt_f16x2 $s1, $c2, $c3, $c4;",
// false[14]
  "packedcmp_lt_f16x2 $s1, $c2, $c100;",
// false[15]
  "packedcmp_lt_f16x2 $s1, $c2, $100;",
// false[16]
  "packedcmp_lt_u16x4 $d1, $d2, d100;",
// false[17]
  "packedcmmp_lt_u16x4 $s1, $c2, $c100;",
// false[18]
  "packedcmp_lt_f16x4_b1 $d1;",
// false[19]
  "packedcmp_lt_f16x4 $d1, $d2;",
// false[20]
  "packedcmp_le_f16x4 $d1, $d2,;",
// false[21]
  "packedcmp_le_f16x4 $d1, $d2, $d3,",
// false[22]
  "packedcmp_le_u32x2 $s1, $c2, $c3, $c4;",
// false[23]
  "packedcmp_le_u32x2 $d1, $d2, $d;",
// false[24]
  "packedcmp_le_u32x2 $d1, $d2, $100;",
// false[25]
  "packedcmp_le_u32x2 $d1, $d2, d100;",
// false[26]
  "packedcmpp_le_u32x2 $d1, $d2, $d100;",
// false[27]
  "packedcmp_le_s32x2 $d1;",
// false[28]
  "packedcmp_le_s32x2 $d1, $d2;",
// false[29]
  "packedcmp_le_s32x2 $d1, $d2,;",
// false[30]
  "packedcmp_gt_s32x2 $d1, $d2, $d3,",
// false[31]
  "packedcmp_gt_s32x2 $d1, $d2, $d3, $d4;",
// false[32]
  "packedcmp_gt_s32x2 $d1, $d2, $d100;",
// false[33]
  "packedcmp_gt_s32x2 $d1, $d2, $100;",
// false[34]
  "packedcmp_gt_s32x2 $d1, $d2, d100;",
// false[35]
  "packedcmmp_gt_s32x2 $d1, $d2, $d100;",
// false[36]
  "packedcmp_gt_f32x2 $d1;",
// false[37]
  "packedcmp_gt_f32x2 $d1, $c2;",
// false[38]
  "packedcmp_gt_f32x2 $d1, $c2,;",
// false[39]
  "packedcmp_gt_f32x2 $d1, $c2, $c3,",
// false[40]
  "packedcmp_ge_f32x2 $d1, $c2, $c3, $c4;",
// false[41]
  "packedcmp_ge_f32x2 $d1, $c2, $c100;",
// false[42]
  "packedcmp_ge_f32x2 $d1, $c2, $100;",
// false[43]
  "packedcmp_ge_f32x2 $d1, $c2, c100;",
// false[44]
  "packedcmmp_ge_f32x2 $s1, $c2, $c100;",
// false[45]
  "packedcmp_ge_s8x4_b1 $d1;",
// false[46]
  "packedcmp_ge_u8x4 $s1, $s2;",
// false[47]
  "packedcmp_ge_f8x4 $s1, $s2,;",
// false[48]
  "packedcmp_ge_s8x8 $s1, $s2, $s3,",
// false[49]
  "packedcmp_ge_u8x8 $s1, $s2, $s3, $s4;",
// false[50]
  "packedcmp_ge_f8x8 $s1, $s2, $s100;",
// false[51]
  "packedcmp_eq_f8x8 $s1, $s2, $100;",
// false[52]
  "packedcmp_eq_u16x2 $s1, $s2, s100;",
// false[53]
  "packedcmmp_eq_b32_s16x4 $s1, $d2, $d1;",
// false[54]
  "packedcmp_eq_f16x2 1, $s2, $s3;",
// false[55]
  "packedcmp_eq_f32x2 WAVESIZE, $s2, $s3;",
// false[56]
  "packedcmp_eq_f32x2 $d1;",
// false[57]
  "packedcmp_eq_f32x2 $d1, $c2;",
// false[58]
  "packedcmp_eq_f32x2 $d1, $c2,;",
// false[59]
  "packedcmp_eq_f32x2 $d1, $c2, $c3,"
};

class TestPackedCmpFloats : public ::testing::TestWithParam<int>
{

};
struct PackedCmpFloatsTest{
  const char *str;
  BrigInstCmp ref;
};

struct PackedCmpFloatsTest packedcmp_floats_pair[24] = {
// packedcmp_pair[0]
  {
    "packedcmp_ftz_equ_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[1],
      BrigEqu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[1]
  {
    "packedcmp_equ_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigEqu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[2]
  {
    "packedcmp_equ_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigEqu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[3]
  {
    "packedcmp_neu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNeu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[4]
  {
    "packedcmp_neu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNeu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[5]
  {
    "packedcmp_neu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNeu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[6]
  {
    "packedcmp_ltu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLtu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[7]
  {
    "packedcmp_ltu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLtu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[8]
  {
    "packedcmp_ltu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLtu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[9]
  {
    "packedcmp_leu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLeu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[10]
  {
    "packedcmp_leu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigLeu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[11]
  {
    "packedcmp_leu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigLeu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[12]
  {
    "packedcmp_gtu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGtu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[13]
  {
    "packedcmp_gtu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGtu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[14]
  {
    "packedcmp_gtu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGtu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[15]
  {
    "packedcmp_geu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGeu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[16]
  {
    "packedcmp_geu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigGeu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[17]
  {
    "packedcmp_geu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigGeu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[18]
  {
    "packedcmp_num_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNum,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[19]
  {
    "packedcmp_num_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNum,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[20]
  {
    "packedcmp_num_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNum,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[21]
  {
    "packedcmp_nan_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + reg_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNan,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[22]
  {
    "packedcmp_nan_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigNan,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[23]
  {
    "packedcmp_nan_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigNan,
      Brigf32x2,
      0
    }
  }
};

class TestPackedCmpFloatsInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_packedcmp_floats_invalid[80] = {
// false[0]
  "packedcmp_neu_s8x4 $c1;",
// false[1]
  "packedcmp_neu_s8x4 $c1, $c2;",
// false[2]
  "packedcmp_neu_u8x4 $c1, $c2,;",
// false[3]
  "packedcmp_neu_u8x4 $c1, $c2, $c3,",
// false[4]
  "packedcmp_neu_s8x8 $c1, $c2, $c3, $c4;",
// false[5]
  "packedcmp_neu_u8x8 $d1, $d2, $d100;",
// false[6]
  "packedcmp_neu_u8x8 $d1, $d2, $100;",
// false[7]
  "packedcmp_neu_s16x2 $c1, $c2, c100;",
// false[8]
  "packedcmmp_neu_s16x2 $c1, $c2, $c100;",
// false[9]
  "packedcmp_neu_u16x2 $d1;",
// false[10]
  "packedcmp_ltu_u16x2 $s1, $c2;",
// false[11]
  "packedcmp_ltu_u16x2 $s1, $c2,;",
// false[12]
  "packedcmp_ltu_u16x2 $s1, $c2, $c3,",
// false[13]
  "packedcmp_ltu_f16x2 $s1, $c2, $c3, $c4;",
// false[14]
  "packedcmp_ltu_f16x2 $s1, $c2, $c100;",
// false[15]
  "packedcmp_ltu_f16x2 $s1, $c2, $100;",
// false[16]
  "packedcmp_ltu_u16x4 $d1, $d2, d100;",
// false[17]
  "packedcmmp_ltu_u16x4 $s1, $c2, $c100;",
// false[18]
  "packedcmp_ltu_f16x4_b1 $d1;",
// false[19]
  "packedcmp_ltu_f16x4 $d1, $d2;",
// false[20]
  "packedcmp_leu_f16x4 $d1, $d2,;",
// false[21]
  "packedcmp_leu_f16x4 $d1, $d2, $d3,",
// false[22]
  "packedcmp_leu_u32x2 $s1, $c2, $c3, $c4;",
// false[23]
  "packedcmp_leu_u32x2 $d1, $d2, $d;",
// false[24]
  "packedcmp_leu_u32x2 $d1, $d2, $100;",
// false[25]
  "packedcmp_leu_u32x2 $d1, $d2, d100;",
// false[26]
  "packedcmpp_leu_u32x2 $d1, $d2, $d100;",
// false[27]
  "packedcmp_leu_s32x2 $d1;",
// false[28]
  "packedcmp_leu_s32x2 $d1, $d2;",
// false[29]
  "packedcmp_leu_s32x2 $d1, $d2,;",
// false[30]
  "packedcmp_gtu_s32x2 $d1, $d2, $d3,",
// false[31]
  "packedcmp_gtu_s32x2 $d1, $d2, $d3, $d4;",
// false[32]
  "packedcmp_gtu_s32x2 $d1, $d2, $d100;",
// false[33]
  "packedcmp_gtu_s32x2 $d1, $d2, $100;",
// false[34]
  "packedcmp_gtu_s32x2 $d1, $d2, d100;",
// false[35]
  "packedcmmp_gtu_s32x2 $d1, $d2, $d100;",
// false[36]
  "packedcmp_gtu_f32x2 $d1;",
// false[37]
  "packedcmp_gtu_f32x2 $d1, $c2;",
// false[38]
  "packedcmp_gtu_f32x2 $d1, $c2,;",
// false[39]
  "packedcmp_gtu_f32x2 $d1, $c2, $c3,",
// false[40]
  "packedcmp_geu_f32x2 $d1, $c2, $c3, $c4;",
// false[41]
  "packedcmp_geu_f32x2 $d1, $c2, $c100;",
// false[42]
  "packedcmp_geu_f32x2 $d1, $c2, $100;",
// false[43]
  "packedcmp_geu_f32x2 $d1, $c2, c100;",
// false[44]
  "packedcmmp_geu_f32x2 $s1, $c2, $c100;",
// false[45]
  "packedcmp_geu_s8x4_b1 $d1;",
// false[46]
  "packedcmp_geu_u8x4 $s1, $s2;",
// false[47]
  "packedcmp_geu_f8x4 $s1, $s2,;",
// false[48]
  "packedcmp_geu_s8x8 $s1, $s2, $s3,",
// false[49]
  "packedcmp_geu_u8x8 $s1, $s2, $s3, $s4;",
// false[50]
  "packedcmp_geu_f8x8 $s1, $s2, $s100;",
// false[51]
  "packedcmp_equ_f8x8 $s1, $s2, $100;",
// false[52]
  "packedcmp_equ_u16x2 $s1, $s2, s100;",
// false[53]
  "packedcmmp_equ_b32_s16x4 $s1, $d2, $d1;",
// false[54]
  "packedcmp_equ_f16x2 1, $s2, $s3;",
// false[55]
  "packedcmp_equ_f32x2 WAVESIZE, $s2, $s3;",
// false[56]
  "packedcmp_equ_f32x2 $d1;",
// false[57]
  "packedcmp_equ_f32x2 $d1, $c2;",
// false[58]
  "packedcmp_equ_f32x2 $d1, $c2,;",
// false[59]
  "packedcmp_equ_f32x2 $d1, $c2, $c3,",
// false[60]
  "packedcmp_num_s8x4 $c1;",
// false[61]
  "packedcmp_num_s8x4 $c1, $c2;",
// false[62]
  "packedcmp_num_u8x4 $c1, $c2,;",
// false[63]
  "packedcmp_num_u8x4 $c1, $c2, $c3,",
// false[64]
  "packedcmp_num_s8x8 $c1, $c2, $c3, $c4;",
// false[65]
  "packedcmp_num_u8x8 $d1, $d2, $d100;",
// false[66]
  "packedcmp_num_u8x8 $d1, $d2, $100;",
// false[67]
  "packedcmp_num_s16x2 $c1, $c2, c100;",
// false[68]
  "packedcmmp_num_s16x2 $c1, $c2, $c100;",
// false[69]
  "packedcmp_num_u16x2 $d1;",
// false[70]
  "packedcmp_nan_s8x4 $c1;",
// false[71]
  "packedcmp_nan_s8x4 $c1, $c2;",
// false[72]
  "packedcmp_nan_u8x4 $c1, $c2,;",
// false[73]
  "packedcmp_nan_u8x4 $c1, $c2, $c3,",
// false[74]
  "packedcmp_nan_s8x8 $c1, $c2, $c3, $c4;",
// false[75]
  "packedcmp_nan_u8x8 $d1, $d2, $d100;",
// false[76]
  "packedcmp_nan_u8x8 $d1, $d2, $100;",
// false[77]
  "packedcmp_nan_s16x2 $c1, $c2, c100;",
// false[78]
  "packedcmmp_nan_s16x2 $c1, $c2, $c100;",
// false[79]
  "packedcmp_nan_u16x2 $d1;"
};

class TestPackedCmpSnan : public ::testing::TestWithParam<int>
{

};
struct PackedCmpSnanTest{
  const char *str;
  BrigInstCmp ref;
};

struct PackedCmpSnanTest packedcmp_snan_pair[42] = {
// packedcmp_pair[0]
  {
    "packedcmp_ftz_sequ_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[1],
      BrigSequ,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[1]
  {
    "packedcmp_sequ_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSequ,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[2]
  {
    "packedcmp_sequ_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSequ,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[3]
  {
    "packedcmp_sneu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSneu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[4]
  {
    "packedcmp_sneu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSneu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[5]
  {
    "packedcmp_sneu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSneu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[6]
  {
    "packedcmp_sltu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSltu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[7]
  {
    "packedcmp_sltu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSltu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[8]
  {
    "packedcmp_sltu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSltu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[9]
  {
    "packedcmp_sleu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSleu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[10]
  {
    "packedcmp_sleu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSleu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[11]
  {
    "packedcmp_sleu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSleu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[12]
  {
    "packedcmp_sgtu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgtu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[13]
  {
    "packedcmp_sgtu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgtu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[14]
  {
    "packedcmp_sgtu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgtu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[15]
  {
    "packedcmp_sgeu_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgeu,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[16]
  {
    "packedcmp_sgeu_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgeu,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[17]
  {
    "packedcmp_sgeu_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgeu,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[18]
  {
    "packedcmp_snum_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnum,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[19]
  {
    "packedcmp_snum_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnum,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[20]
  {
    "packedcmp_snum_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnum,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[21]
  {
    "packedcmp_snan_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnan,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[22]
  {
    "packedcmp_snan_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnan,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[23]
  {
    "packedcmp_snan_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSnan,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[24]
  {
    "packedcmp_seq_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSeq,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[25]
  {
    "packedcmp_seq_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSeq,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[26]
  {
    "packedcmp_seq_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSeq,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[27]
  {
    "packedcmp_sne_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSne,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[28]
  {
    "packedcmp_sne_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSne,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[29]
  {
    "packedcmp_sne_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSne,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[30]
  {
    "packedcmp_slt_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSlt,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[31]
  {
    "packedcmp_slt_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSlt,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[32]
  {
    "packedcmp_slt_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSlt,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[33]
  {
    "packedcmp_sle_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSle,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[34]
  {
    "packedcmp_sle_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSle,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[35]
  {
    "packedcmp_sle_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSle,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[36]
  {
    "packedcmp_sgt_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgt,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[37]
  {
    "packedcmp_sgt_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgt,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[38]
  {
    "packedcmp_sgt_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSgt,
      Brigf32x2,
      0
    }
  },
// packedcmp_pair[39]
  {
    "packedcmp_sge_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x2,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSge,
      Brigf16x2,
      0
    }
  },
// packedcmp_pair[40]
  {
    "packedcmp_sge_f16x4 $d1, $d2, 1.0;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf16x4,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      },
      packedcmp_bam[0],
      BrigSge,
      Brigf16x4,
      0
    }
  },
// packedcmp_pair[41]
  {
    "packedcmp_sge_f32x2 $d1, 1.0, $d3;",
    {
      brig_inst_cmp_size,
      BrigEInstCmp,
      BrigPackedCmp,
      Brigf32x2,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      },
      packedcmp_bam[0],
      BrigSge,
      Brigf32x2,
      0
    }
  },
};

class TestPackedCmpSnanInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_packedcmp_snan_invalid[140] = {
// false[0]
  "packedcmp_sneu_s8x4 $c1;",
// false[1]
  "packedcmp_sneu_s8x4 $c1, $c2;",
// false[2]
  "packedcmp_sneu_u8x4 $c1, $c2,;",
// false[3]
  "packedcmp_sneu_u8x4 $c1, $c2, $c3,",
// false[4]
  "packedcmp_sneu_s8x8 $c1, $c2, $c3, $c4;",
// false[5]
  "packedcmp_sneu_u8x8 $d1, $d2, $d100;",
// false[6]
  "packedcmp_sneu_u8x8 $d1, $d2, $100;",
// false[7]
  "packedcmp_sneu_s16x2 $c1, $c2, c100;",
// false[8]
  "packedcmmp_sneu_s16x2 $c1, $c2, $c100;",
// false[9]
  "packedcmp_sneu_u16x2 $d1;",
// false[10]
  "packedcmp_sltu_u16x2 $s1, $c2;",
// false[11]
  "packedcmp_sltu_u16x2 $s1, $c2,;",
// false[12]
  "packedcmp_sltu_u16x2 $s1, $c2, $c3,",
// false[13]
  "packedcmp_sltu_f16x2 $s1, $c2, $c3, $c4;",
// false[14]
  "packedcmp_sltu_f16x2 $s1, $c2, $c100;",
// false[15]
  "packedcmp_sltu_f16x2 $s1, $c2, $100;",
// false[16]
  "packedcmp_sltu_u16x4 $d1, $d2, d100;",
// false[17]
  "packedcmmp_sltu_u16x4 $s1, $c2, $c100;",
// false[18]
  "packedcmp_sltu_f16x4_b1 $d1;",
// false[19]
  "packedcmp_sltu_f16x4 $d1, $d2;",
// false[20]
  "packedcmp_sleu_f16x4 $d1, $d2,;",
// false[21]
  "packedcmp_sleu_f16x4 $d1, $d2, $d3,",
// false[22]
  "packedcmp_sleu_u32x2 $s1, $c2, $c3, $c4;",
// false[23]
  "packedcmp_sleu_u32x2 $d1, $d2, $d;",
// false[24]
  "packedcmp_sleu_u32x2 $d1, $d2, $100;",
// false[25]
  "packedcmp_sleu_u32x2 $d1, $d2, d100;",
// false[26]
  "packedcmpp_sleu_u32x2 $d1, $d2, $d100;",
// false[27]
  "packedcmp_sleu_s32x2 $d1;",
// false[28]
  "packedcmp_sleu_s32x2 $d1, $d2;",
// false[29]
  "packedcmp_sleu_s32x2 $d1, $d2,;",
// false[30]
  "packedcmp_sgtu_s32x2 $d1, $d2, $d3,",
// false[31]
  "packedcmp_sgtu_s32x2 $d1, $d2, $d3, $d4;",
// false[32]
  "packedcmp_sgtu_s32x2 $d1, $d2, $d100;",
// false[33]
  "packedcmp_sgtu_s32x2 $d1, $d2, $100;",
// false[34]
  "packedcmp_sgtu_s32x2 $d1, $d2, d100;",
// false[35]
  "packedcmmp_sgtu_s32x2 $d1, $d2, $d100;",
// false[36]
  "packedcmp_sgtu_f32x2 $d1;",
// false[37]
  "packedcmp_sgtu_f32x2 $d1, $c2;",
// false[38]
  "packedcmp_sgtu_f32x2 $d1, $c2,;",
// false[39]
  "packedcmp_sgtu_f32x2 $d1, $c2, $c3,",
// false[40]
  "packedcmp_sgeu_f32x2 $d1, $c2, $c3, $c4;",
// false[41]
  "packedcmp_sgeu_f32x2 $d1, $c2, $c100;",
// false[42]
  "packedcmp_sgeu_f32x2 $d1, $c2, $100;",
// false[43]
  "packedcmp_sgeu_f32x2 $d1, $c2, c100;",
// false[44]
  "packedcmmp_sgeu_f32x2 $s1, $c2, $c100;",
// false[45]
  "packedcmp_sgeu_s8x4_b1 $d1;",
// false[46]
  "packedcmp_sgeu_u8x4 $s1, $s2;",
// false[47]
  "packedcmp_sgeu_f8x4 $s1, $s2,;",
// false[48]
  "packedcmp_sgeu_s8x8 $s1, $s2, $s3,",
// false[49]
  "packedcmp_sgeu_u8x8 $s1, $s2, $s3, $s4;",
// false[50]
  "packedcmp_sgeu_f8x8 $s1, $s2, $s100;",
// false[51]
  "packedcmp_sequ_f8x8 $s1, $s2, $100;",
// false[52]
  "packedcmp_sequ_u16x2 $s1, $s2, s100;",
// false[53]
  "packedcmmp_sequ_b32_s16x4 $s1, $d2, $d1;",
// false[54]
  "packedcmp_sequ_f16x2 1, $s2, $s3;",
// false[55]
  "packedcmp_sequ_f32x2 WAVESIZE, $s2, $s3;",
// false[56]
  "packedcmp_sequ_f32x2 $d1;",
// false[57]
  "packedcmp_sequ_f32x2 $d1, $c2;",
// false[58]
  "packedcmp_sequ_f32x2 $d1, $c2,;",
// false[59]
  "packedcmp_sequ_f32x2 $d1, $c2, $c3,",
// false[60]
  "packedcmp_snum_s8x4 $c1;",
// false[61]
  "packedcmp_snum_s8x4 $c1, $c2;",
// false[62]
  "packedcmp_snum_u8x4 $c1, $c2,;",
// false[63]
  "packedcmp_snum_u8x4 $c1, $c2, $c3,",
// false[64]
  "packedcmp_snum_s8x8 $c1, $c2, $c3, $c4;",
// false[65]
  "packedcmp_snum_u8x8 $d1, $d2, $d100;",
// false[66]
  "packedcmp_snum_u8x8 $d1, $d2, $100;",
// false[67]
  "packedcmp_snum_s16x2 $c1, $c2, c100;",
// false[68]
  "packedcmmp_snum_s16x2 $c1, $c2, $c100;",
// false[69]
  "packedcmp_snum_u16x2 $d1;",
// false[70]
  "packedcmp_snan_s8x4 $c1;",
// false[71]
  "packedcmp_snan_s8x4 $c1, $c2;",
// false[72]
  "packedcmp_snan_u8x4 $c1, $c2,;",
// false[73]
  "packedcmp_snan_u8x4 $c1, $c2, $c3,",
// false[74]
  "packedcmp_snan_s8x8 $c1, $c2, $c3, $c4;",
// false[75]
  "packedcmp_snan_u8x8 $d1, $d2, $d100;",
// false[76]
  "packedcmp_snan_u8x8 $d1, $d2, $100;",
// false[77]
  "packedcmp_snan_s16x2 $c1, $c2, c100;",
// false[78]
  "packedcmmp_snan_s16x2 $c1, $c2, $c100;",
// false[79]
  "packedcmp_snan_u16x2 $d1;",
// false[80]
  "packedcmp_sne_s8x4 $c1;",
// false[81]
  "packedcmp_sne_s8x4 $c1, $c2;",
// false[82]
  "packedcmp_sne_u8x4 $c1, $c2,;",
// false[83]
  "packedcmp_sne_u8x4 $c1, $c2, $c3,",
// false[84]
  "packedcmp_sne_s8x8 $c1, $c2, $c3, $c4;",
// false[85]
  "packedcmp_sne_u8x8 $d1, $d2, $d100;",
// false[86]
  "packedcmp_sne_u8x8 $d1, $d2, $100;",
// false[87]
  "packedcmp_sne_s16x2 $c1, $c2, c100;",
// false[88]
  "packedcmmp_sne_s16x2 $c1, $c2, $c100;",
// false[89]
  "packedcmp_sne_u16x2 $d1;",
// false[90]
  "packedcmp_slt_u16x2 $s1, $c2;",
// false[91]
  "packedcmp_slt_u16x2 $s1, $c2,;",
// false[92]
  "packedcmp_slt_u16x2 $s1, $c2, $c3,",
// false[93]
  "packedcmp_slt_f16x2 $s1, $c2, $c3, $c4;",
// false[94]
  "packedcmp_slt_f16x2 $s1, $c2, $c100;",
// false[95]
  "packedcmp_slt_f16x2 $s1, $c2, $100;",
// false[96]
  "packedcmp_slt_u16x4 $d1, $d2, d100;",
// false[97]
  "packedcmmp_slt_u16x4 $s1, $c2, $c100;",
// false[98]
  "packedcmp_slt_f16x4_b1 $d1;",
// false[99]
  "packedcmp_slt_f16x4 $d1, $d2;",
// false[100]
  "packedcmp_sle_f16x4 $d1, $d2,;",
// false[101]
  "packedcmp_sle_f16x4 $d1, $d2, $d3,",
// false[102]
  "packedcmp_sle_u32x2 $s1, $c2, $c3, $c4;",
// false[103]
  "packedcmp_sle_u32x2 $d1, $d2, $d;",
// false[104]
  "packedcmp_sle_u32x2 $d1, $d2, $100;",
// false[105]
  "packedcmp_sle_u32x2 $d1, $d2, d100;",
// false[106]
  "packedcmpp_sle_u32x2 $d1, $d2, $d100;",
// false[107]
  "packedcmp_sle_s32x2 $d1;",
// false[108]
  "packedcmp_sle_s32x2 $d1, $d2;",
// false[109]
  "packedcmp_sle_s32x2 $d1, $d2,;",
// false[110]
  "packedcmp_sgt_s32x2 $d1, $d2, $d3,",
// false[111]
  "packedcmp_sgt_s32x2 $d1, $d2, $d3, $d4;",
// false[112]
  "packedcmp_sgt_s32x2 $d1, $d2, $d100;",
// false[113]
  "packedcmp_sgt_s32x2 $d1, $d2, $100;",
// false[114]
  "packedcmp_sgt_s32x2 $d1, $d2, d100;",
// false[115]
  "packedcmmp_sgt_s32x2 $d1, $d2, $d100;",
// false[116]
  "packedcmp_sgt_f32x2 $d1;",
// false[117]
  "packedcmp_sgt_f32x2 $d1, $c2;",
// false[118]
  "packedcmp_sgt_f32x2 $d1, $c2,;",
// false[119]
  "packedcmp_sgt_f32x2 $d1, $c2, $c3,",
// false[120]
  "packedcmp_sge_f32x2 $d1, $c2, $c3, $c4;",
// false[121]
  "packedcmp_sge_f32x2 $d1, $c2, $c100;",
// false[122]
  "packedcmp_sge_f32x2 $d1, $c2, $100;",
// false[123]
  "packedcmp_sge_f32x2 $d1, $c2, c100;",
// false[124]
  "packedcmmp_sge_f32x2 $s1, $c2, $c100;",
// false[125]
  "packedcmp_sge_s8x4_b1 $d1;",
// false[126]
  "packedcmp_sge_u8x4 $s1, $s2;",
// false[127]
  "packedcmp_sge_f8x4 $s1, $s2,;",
// false[128]
  "packedcmp_sge_s8x8 $s1, $s2, $s3,",
// false[129]
  "packedcmp_sge_u8x8 $s1, $s2, $s3, $s4;",
// false[130]
  "packedcmp_sge_f8x8 $s1, $s2, $s100;",
// false[131]
  "packedcmp_seq_f8x8 $s1, $s2, $100;",
// false[132]
  "packedcmp_seq_u16x2 $s1, $s2, s100;",
// false[133]
  "packedcmmp_seq_b32_s16x4 $s1, $d2, $d1;",
// false[134]
  "packedcmp_seq_f16x2 1, $s2, $s3;",
// false[135]
  "packedcmp_seq_f32x2 WAVESIZE, $s2, $s3;",
// false[136]
  "packedcmp_seq_f32x2 $d1;",
// false[137]
  "packedcmp_seq_f32x2 $d1, $c2;",
// false[138]
  "packedcmp_seq_f32x2 $d1, $c2,;",
// false[139]
  "packedcmp_seq_f32x2 $d1, $c2, $c3,"
};

}  // namespace brig
}  // namespace hsa
#endif //CMP_TEST_H_
