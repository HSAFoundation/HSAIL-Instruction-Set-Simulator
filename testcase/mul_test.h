/* Copyright 2012 <MulticorewareInc> */

#ifndef MUL_TEST_H_
#define MUL_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class MulBase : public ::testing::TestWithParam<int>
{

};
struct MulBaseTest{
  const char *str;
  BrigInstBase ref;
};

struct MulBaseTest mulbase_pair[36] = {
// mulbase_pair[0]
  {
    "mul_s32 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[1]
  {
    "mul_s32 $s1, $s2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulbase_pair[2]
  {
    "mul_s32 $s1, $s2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[3]
  {
    "mul_s32 $s1, 1, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[4]
  {
    "mul_s32 $s1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      }
    }
  },
// mulbase_pair[5]
  {
    "mul_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[6]
  {
    "mul_s32 $s1, WAVESIZE, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      }
    }
  },
// mulbase_pair[7]
  {
    "mul_s32 $s1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      }
    }
  },
// mulbase_pair[8]
  {
    "mul_s32 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  },
// mulbase_pair[9]
  {
    "mul_s64 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[10]
  {
    "mul_s64 $d1, $d2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulbase_pair[11]
  {
    "mul_s64 $d1, $d2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[12]
  {
    "mul_s64 $d1, 1, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[13]
  {
    "mul_s64 $d1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      }
    }
  },
// mulbase_pair[14]
  {
    "mul_s64 $d1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[15]
  {
    "mul_s64 $d1, WAVESIZE, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      }
    }
  },
// mulbase_pair[16]
  {
    "mul_s64 $d1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      }
    }
  },
// mulbase_pair[17]
  {
    "mul_s64 $d1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  },
// mulbase_pair[18]
  {
    "mul_u32 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[19]
  {
    "mul_u32 $s1, $s2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulbase_pair[20]
  {
    "mul_u32 $s1, $s2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[21]
  {
    "mul_u32 $s1, 1, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[22]
  {
    "mul_u32 $s1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),              
        0,
        0
      }
    }
  },
// mulbase_pair[23]
  {
    "mul_u32 $s1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
        0,
        0
      }
    }
  },
// mulbase_pair[24]
  {
    "mul_u32 $s1, WAVESIZE, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      }
    }
  },
// mulbase_pair[25]
  {
    "mul_u32 $s1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      }
    }
  },
// mulbase_pair[26]
  {
    "mul_u32 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  },
// mulbase_pair[27]
  {
    "mul_u64 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[28]
  {
    "mul_u64 $d1, $d2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulbase_pair[29]
  {
    "mul_u64 $d1, $d2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulbase_pair[30]
  {
    "mul_u64 $d1, 1, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulbase_pair[31]
  {
    "mul_u64 $d1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),                           
        0,
        0
      }
    }
  },
// mulbase_pair[32]
  {
    "mul_u64 $d1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size, 
        0,
        0
      }
    }
  },
// mulbase_pair[33]
  {
    "mul_u64 $d1, WAVESIZE, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,              
        0,
        0
      }
    }
  },
// mulbase_pair[34]
  {
    "mul_u64 $d1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),             
        0,
        0
      }
    }
  },
// mulbase_pair[35]
  {
    "mul_u64 $d1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu64,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  }
};

class MulBaseInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mul_base_invalid[18] = {
// false[0]
  "mul_s32 $s1;",
// false[1]
  "mul_s32 $s1, $s2;",
// false[2]
  "mul_s32 1, $s2, $s3;",
// alse[3]
  "mul_s32 WAVESIZE, $s2, $s3;",
// false[4]
  "mul_s64 $s1;",
// false[5]
  "mul_s64 $s1, $s2;",
// false[6]
  "mul_s64 1, $s2, $s3;",
// false[7]
  "mul_s64 WAVESIZE, $s2, $s3;",
// false[8]
  "mul_s64 $s1, $s2, $s3, $s4, $s5;",
// false[9]
  "mul_u32 $s1;",
// false[10]
  "mul_u32 $s1, $s2;",
// false[11]
  "mul_u32 1, $s2, $s3;",
// false[12]
  "mul_u32 WAVESIZE, $s2, s3;",
// false[13]
  "mul_u64 $s1;",
// false[14]
  "mul_u64 $s1, $s2;",
// false[15]
  "mul_u64 1, $s2, $s3;",
// false[16]
  "mul_u64 WAVESIZE, $s2, $s3;",
// false[17]
  "mull_s64 1, $s2, $s3, $s4;"
};

class MulControl : public ::testing::TestWithParam<int>
{

};
struct MulControlTest{
  const char *str;
  BrigInstBase ref;
};

struct MulControlTest mulcontrol_pair[24] = {
// mulcontrol_pair[0]
  {
    "mul_pp_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu8x4,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[1]
  {
    "mul_pp_sat_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs8x4,
      BrigPackPPsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[2]
  {
    "mul_ps_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu16x2,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[3]
  {
    "mul_ps_sat_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs16x2,
      BrigPackPSsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[4]
  {
    "mul_sp_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu8x4,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[5]
  {
    "mul_sp_sat_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs8x4,
      BrigPackSPsat,
      {
       operand_offset,
       operand_offset + reg_size,
       operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[6]
  {
    "mul_ss_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu16x2,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[7]
  {
    "mul_ss_sat_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs16x2,
      BrigPackSSsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[8]
  {
    "mul_pp_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x2,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[9]
  {
    "mul_ps_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x2,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[10]
  {
    "mul_sp_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x2,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[11]
  {
    "mul_ss_f16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x2,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[12]
  {
    "mul_pp_u8x8 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu8x8,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[13]
  {
    "mul_pp_sat_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs8x8,
      BrigPackPPsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[14]
  {
    "mul_ps_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu16x4,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[15]
  {
    "mul_ps_sat_s16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs16x4,
      BrigPackPSsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[16]
  {
    "mul_sp_u8x8 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu8x8,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[17]
  {
    "mul_sp_sat_s8x8 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs8x8,
      BrigPackSPsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[18]
  {
    "mul_ss_u16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigu16x4,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[19]
  {
    "mul_ss_sat_s16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigs16x4,
      BrigPackSSsat,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[20]
  {
    "mul_pp_f16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x4,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
//  mulcontrol_pair[21]
  {
    "mul_ps_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf32x2,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[22]
  {
    "mul_sp_f16x4 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf16x4,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulcontrol_pair[23]
  {
    "mul_ss_f32x2 $d1, $d2, $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMul,
      Brigf32x2,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  }
};

class MulControlInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mul_control_invalid[16] = {
// false[0]
  "mul_pp_u8x4 $s1;",
// false[1]
  "mul_pp_s8x4 $s1, $s2;",
// false[2]
  "mul_pp_s8x4 $s1, $s2, $3;",
// false[3]
  "mul_pp_s8x4 $s1, $s2, s3;",
// false[4]
  "mul_pp_sat_u8x4 $s1, $s2, $s3, $s4;",
// false[5]
  "mul_ps_u8x4 $s1, $s2, $s3, $s4, $s5;",
// false[6]
  "mul_ps_sat_u8x4 1, $s2, $s3;",
// false[7]
  "mul_sp_u8x4 1, $s2, $s3;",
// false[8]
  "mul_sp_sat_s16x2 WAVESIZE, $s2, $s3;",
// false[9]
  "mul_ss_sat_u8x4 $s1, $s2, $s100;",
// false[10]
  "mul_ss_u8x4 $s1, $s2, $s3, 100;",
// false[11]
  "mul_ppp_f16x2 $s1, $s2, $s3;",
// false[12]
  "mul_ps_ff16x2 $s1, $s2, $s3;",
// false[13]
  "mul_ps_f32x22 $d1, $d2, $d3;",
// false[14]
  "mul_sp_f32x2 $s1, $s2, $s3, $s4;",
// false[15]
  "mull_ss_f32x2 $d1, $d2, $d3;"
};

class MulHis32Base : public ::testing::TestWithParam<int>
{

};
struct MulHis32BaseTest{
  const char *str;
  BrigInstBase ref;
};

struct MulHis32BaseTest mulhis32_pair[9] = {
// mulhis32_pair[0]
  {
    "mul_hi_s32 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhis32_pair[1]
  {
    "mul_hi_s32 $s1, $s2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulhis32_pair[2]
  {
    "mul_hi_s32 $s1, $s2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhis32_pair[3]
  {
    "mul_hi_s32 $s1, 1, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulhis32_pair[4]
  {
    "mul_hi_s32 $s1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      }
    }
  },
// mulhis32_pair[5]
  {
    "mul_hi_s32 $s1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulhis32_pair[6]
  {
    "mul_hi_s32 $s1, WAVESIZE, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      }
    }
  },
// mulhis32_pair[7]
  {
    "mul_hi_s32 $s1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      }
    }
  },
// mulhis32_pair[8]
  {
    "mul_hi_s32 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  }
};

class MulHis32BaseInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mul_hi_s32_base_invalid[13] = {
// false[0]
  "mul_hi_s32 $s1;",
// false[1]
  "mul_hi_s32 $s1, $s2;",
// false[2]
  "mul_hi_s32 WAVESIZE, $s2, $s3;",
// false[3]
  "mul_hi_s32 1, $s2, $s3;",
// false[4]
  "mul_hi_s32 $s1, $s2, $3",
// false[5]
  "mul_hi_s32 1, 1, 1;",
// false[6]
  "mul_hi_s32 $s1, $s2, s3",
// false[7]
  "mul_hi_s32 $s1, $s2, $s100",
// false[8]
  "mul_hi_s32 $s1, $s2, $s3, $s4",
// false[9]
  "mull_hi_s32 $s1, $s2, $s3",
// false[10]
  "mul_hii_s32 $s1, $s2, $s3",
// false[11]
  "mul_hi_ss32 $s1, $s2, $s3",
// false[12]
  "mul_hi_s322 $s1, $s2, $s3"
};

class MulHiu32Base : public ::testing::TestWithParam<int>
{

};

struct MulHiu32BaseTest{
  const char *str;
  BrigInstBase ref;
};

struct MulHiu32BaseTest mulhiu32_pair[9] = {
// mulhiu32_pair[0]
  {
    "mul_hi_u32 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhiu32_pair[1]
  {
    "mul_hi_u32 $s1, $s2, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size * 2),
        0,
        0
      }
    }
  },
// mulhiu32_pair[2]
  {
    "mul_hi_u32 $s1, $s2, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhiu32_pair[3]
  {
    "mul_hi_u32 $s1, 1, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulhiu32_pair[4]
  {
    "mul_hi_u32 $s1, 1, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
        0,
        0
      }
    }
  },
// mulhiu32_pair[5]
  {
    "mul_hi_u32 $s1, 1, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        RoundUp8(operand_offset + reg_size),
        RoundUp8(operand_offset + reg_size) + immed_size,
        0,
        0
      }
    }
  },
// mulhiu32_pair[6]
  {
    "mul_hi_u32 $s1, WAVESIZE, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size + wavesz_size,
        0,
        0
      }
    }
  },
// mulhiu32_pair[7]
  {
    "mul_hi_u32 $s1, WAVESIZE, 1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        RoundUp8(operand_offset + reg_size + wavesz_size),
        0,
        0
      }
    }
  },
// mulhiu32_pair[8]
  {
    "mul_hi_u32 $s1, WAVESIZE, WAVESIZE;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu32,
      BrigNoPacking,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size,
        0,
        0
      }
    }
  }
};

class MulHiu32BaseInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mul_hi_u32_base_invalid[13] = {
// false[0]
  "mul_hi_u32 $s1;",
// false[1]
  "mul_hi_u32 $s1, $s2;",
// false[2]
  "mul_hi_u32 WAVESIZE, $s2, $s3;",
// false[3]
  "mul_hi_u32 1, $s2, $s3;",
// false[4]
  "mul_hi_u32 $s1, $s2, $3",
// false[5]
  "mul_hi_u32 1, 1, 1;",
// false[6]
  "mul_hi_u32 $s1, $s2, s3",
// false[7]
  "mul_hi_u32 $s1, $s2, $s100",
// false[8]
  "mul_hi_u32 $s1, $s2, $s3, $s4",
// false[9]
  "mull_hi_u32 $s1, $s2, $s3",
// false[10]
  "mul_hii_u32 $s1, $s2, $s3",
// false[11]
  "mul_hi_su32 $s1, $s2, $s3",
// false[12]
  "mul_hi_u322 $s1, $s2, $s3"
};

class MulHiControl : public ::testing::TestWithParam<int>
{

};
struct MulHiControlTest{
  const char *str;
  BrigInstBase ref;
};

struct MulHiControlTest mulhicontrol_pair[16] = {
// mulhicontrol_pair[0]
  {
    "mul_hi_pp_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu8x4,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[1]
  {
    "mul_hi_pp_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs8x4,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[2]
  {
    "mul_hi_pp_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu16x2,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[3]
  {
    "mul_hi_pp_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs16x2,
      BrigPackPP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[4]
  {
    "mul_hi_ps_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu8x4,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[5]
  {
    "mul_hi_ps_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs8x4,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[6]
  {
    "mul_hi_ps_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu16x2,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[7]
  {
    "mul_hi_ps_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs16x2,
      BrigPackPS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[8]
  {
    "mul_hi_sp_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu8x4,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[9]
  {
    "mul_hi_sp_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs8x4,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[10]
  {
    "mul_hi_sp_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu16x2,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[11]
  {
    "mul_hi_sp_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs16x2,
      BrigPackSP,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[12]
  {
    "mul_hi_ss_u8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu8x4,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[13]
  {
    "mul_hi_ss_s8x4 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs8x4,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[14]
  {
    "mul_hi_ss_u16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigu16x2,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  },
// mulhicontrol_pair[15]
  {
    "mul_hi_ss_s16x2 $s1, $s2, $s3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMulHi,
      Brigs16x2,
      BrigPackSS,
      {
        operand_offset,
        operand_offset + reg_size,
        operand_offset + reg_size * 2,
        0,
        0
      }
    }
  }
};

class MulHiControlInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mul_hi_control_invalid[16] = {
// false[0]
  "mul_hi_pp_u8x4 $s1;",
// false[1]
  "mul_hi_pp_s8x4 $s1, $s2;",
// false[2]
  "mul_hi_pp_s8x4 $s1, $s2, $3;",
// false[3]
  "mul_hi_pp_s8x4 $s1, $s2, s3;",
// false[4]
  "mul_hi_pp_sat_u8x4 $s1, $s2, $s3, $s4;",
// false[5]
  "mul_hi_ps_u8x4 $s1, $s2, $s3, $s4, $s5;",
// false[6]
  "mul_hi_ps_sat_u8x4 1, $s2, $s3;",
// false[7]
  "mul_hi_sp_u8x4 1, $s2, $s3;",
// false[8]
  "mul_hi_sp_sat_s16x2 WAVESIZE, $s2, $s3;",
// false[9]
  "mul_hi_ss_sat_u8x4 $s1, $s2, $s100;",
// false[10]
  "mul_hi_ss_u8x4 $s1, $s2, $s3, 100;",
// false[11]
  "mul_hi_ppp_f16x2 $s1, $s2, $s3;",
// false[12]
  "mul_hi_ps_ff16x2 $s1, $s2, $s3;",
// false[13]
  "mul_hi_ps_f32x22 $d1, $d2, $d3;",
// false[14]
  "mul_hi_sp_f32x2 $s1, $s2, $s3, $s4;",
// false[15]
  "mull_hi_ss_f32x2 $d1, $d2, $d3;"
};

class MulFtzRound : public ::testing::TestWithParam<int>
{

};

struct MulFtzRoundTest{
  const char *str;
  BrigInstMod ref;
};

struct MulFtzRoundTest mulftzround_pair[24] = {
//mulftzround_pair[0]
  {
    "mul_ftz_f32 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//mulftzround_pair[1]
  {
    "mul_ftz_f64 $d1, $d2, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
   {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//mulftzround_pair[2]
  {
    "mul_near_f32 $s1, 1.0f, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//mulftzround_pair[3]
  {
    "mul_near_f64 $d1, 1.0, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0,
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//mulftzround_pair[4]
  {
    "mul_zero_f32 $s1, $s2, $s3;",
      {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0,
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//mulftzround_pair[5]
  {
    "mul_zero_f64 $d1, $d2, 1.0;",
      {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0,
      0
     },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//mulftzround_pair[6]
  {
    "mul_up_f32 $s1, 1.0f, $s3;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0,
      0
     },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//mulftzround_pair[7]
  {
    "mul_up_f64 $d1, 1.0, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0,
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//mulftzround_pair[8]
  {
    "mul_down_f32 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0,
      0
      },
     {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//mulftzround_pair[9]
  {
    "mul_down_f64 $d1, $d2, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//mulftzround_pair[10]
  {
    "mul_ftz_zero_f32 $s1, 1.0f, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//mulftzround_pair[11]
  {
    "mul_ftz_zero_f64 $d1, 1.0, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
     {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//mulftzround_pair[12]
  {
    "mul_ftz_up_f32 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
  }
  },
//mulftzround_pair[13]
  {
    "mul_ftz_up_f64 $d1, $d2, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0,
      0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//mulftzround_pair[14]
  {
    "mul_ftz_down_f32 $s1, 1.0f, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  },
//mulftzround_pair[15]
  {
    "mul_ftz_down_f64 $d1, 1.0, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0,
      0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  },
//mulftzround_pair[16]
  {
    "mul_ftz_f16 $s1, $s2, $s3;",
      {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//mulftzround_pair[17]
  {
    "mul_near_f16 $s1, $s2, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0,
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//mulftzround_pair[18]
  {
    "mul_zero_f16 $s1, 1.0f, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0,
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//mulftzround_pair[19]
  {
    "mul_up_f16 $s1, 1.0f, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//mulftzround_pair[20]
  {
    "mul_down_f16 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//mulftzround_pair[21]
  {
    "mul_ftz_zero_f16 $s1, $s2, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//mulftzround_pair[22]
  {
    "mul_ftz_up_f16 $s1, 1.0f, $s3;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0,
      0
     },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//mulftzround_pair[23]
  {
    "mul_ftz_down_f16 $s1, 1.0f, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0,
      0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  }
};

class MulFtzRoundInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_ftz_round_invalid[16] = {
  "mul_ftz_f32 $s1;",
  "mul_ftz_f64 $d1, $d2;",
  "mul_near_f32 $s1, 1.0f,;",
  "mul_near_f64 100, 1.0, 1.0;",
  "mul_zero_f32 WAVESIZE, $s2, $s3;",
  "mul_zero_f64 $d1, 1.0, 1.0, 1.0;",
  "mull_up_f32 $s1, 1.0f, $s3;",
  "mul_upp_f64 $d1, 1.0, 1.0;",
  "mul_down_ff32 $s1, $s2, $s3;",
  "mul_down_f644 $d1, $d2, 1.0;",
  "mul_ftz_zero_f32 $s1, , $s3;",
  "mul_ftz_zero_f64 $d1,;",
  "mul_ftz_up_f32 $s1, $s2;",
  "mul_ftz_up_f64 1.0, $d2, 1.0;",
  "mul_ftz_down_f32 $s1, 1.0f;",
  "mul_ftz_down_f64 $d1, 1.0, 1.0, 1.0;"
};

class Mul24 : public ::testing::TestWithParam<int>
{

};

struct MulBaseTest mul24_pair[12] = {
//mul24_pair[0]
  {
    "mul24_s32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      }
    }
  },
//mul24_pair[1]
  {
    "mul24_s32 $s1, $s2, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//mul24_pair[2]
  {
    "mul24_s32 $s1, $s2, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      }
    }
  },
//mul24_pair[3]
  {
    "mul24_s64 $d1, 1, $d3;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//mul24_pair[4]
  {
    "mul24_s64 $d1, 1, 1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//mul24_pair[5]
  {
    "mul24_s64 $d1, 1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//mul24_pair[6]
  {
    "mul24_u32 $s1, WAVESIZE, $s3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      0, 
      0
      }
    }
  },
//mul24_pair[7]
  {
    "mul24_u32 $s1, WAVESIZE, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      0, 
      0
      }
    }
  },
//mul24_pair[8]
  {
    "mul24_u32 $s1, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      0, 
      0
      }
    }
  },
//mul24_pair[9]
  {
    "mul24_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      }
    }
  },
//mul24_pair[10]
  {
    "mul24_u64 $d1, $d2, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//mul24_pair[11]
  {
    "mul24_u64 $d1, $d2, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      }
    }
  },
};

class Mul24Invalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul24_invalid[4] = {
  "mul24_s32 $s1;",
  "mul24_s64 $s1, $s2, s3;",
  "mul24_u32 1, 1, $s3;",
  "mull24_u64 WAVESIZE, 1, 1;"
};

class Mad24Hi : public ::testing::TestWithParam<int>
{

};

struct MulBaseTest mad24hi_pair[27] = {
//mad24hi_pair[0]
  {
    "mad24_hi_s32 $s1, $s2, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 3, 
      0
      }
    }
  },
//mad24hi_pair[1]
  {
    "mad24_hi_s32 $s1, $s2, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      RoundUp8(operand_offset + reg_size * 3), 
      0
      }
    }
  },
//mad24hi_pair[2]
  {
    "mad24_hi_s32 $s1, $s2, $s3, WAVESIZE;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 3, 
      0
      }
    }
  },
//mad24hi_pair[3]
  {
    "mad24_hi_s32 $s1, $s2, 1, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(operand_offset + reg_size * 2) + immed_size, 
      0
      }
    }
  },
//mad24hi_pair[4]
  {
    "mad24_hi_s32 $s1, $s2, 1, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size), 
      0
      }
    }
  },
//mad24hi_pair[5]
  {
    "mad24_hi_s32 $s1, $s2, 1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(operand_offset + reg_size * 2) + immed_size, 
      0
      }
    }
  },
//mad24hi_pair[6]
  {
    "mad24_hi_s32 $s1, $s2, WAVESIZE, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 2 + wavesz_size, 
      0
      }
    }
  },
//mad24hi_pair[7]
  {
    "mad24_hi_s32 $s1, $s2, WAVESIZE, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      RoundUp8(operand_offset + reg_size * 2 + wavesz_size), 
      0
      }
    }
  },
//mad24hi_pair[8]
  {
    "mad24_hi_s32 $s1, $s2, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 2, 
      0
      }
    }
  },
//mad24hi_pair[9]
  {
    "mad24_hi_s64 $d1, 1, $d3, $d4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size,  
      0
      }
    }
  },
//mad24hi_pair[10]
  {
    "mad24_hi_s64 $d1, 1, $d3, 1;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),  
      0
      }
    }
  },
//mad24hi_pair[11]
  {
    "mad24_hi_s64 $d1, 1, $d3, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size, 
      0
      }
    }
  },
//mad24hi_pair[12]
  {
    "mad24_hi_s64 $d1, 1, 1, $d4;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + immed_size, 
      0
      }
    }
  },
//mad24hi_pair[13]
  {
    "mad24_hi_s64 $d1, 1, 1, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
       immed_size), 
      0
      }
    }
  },
//mad24hi_pair[14]
  {
    "mad24_hi_s64 $d1, 1, 1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + immed_size, 
      0
      }
    }
  },
//mad24hi_pair[15]
  {
    "mad24_hi_s64 $d1, 1, WAVESIZE, $d4;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size + wavesz_size, 
      0
      }
    }
  },
//mad24hi_pair[16]
  {
    "mad24_hi_s64 $d1, 1, WAVESIZE, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + wavesz_size), 
      0
      }
    }
  },
//mad24hi_pair[17]
  {
    "mad24_hi_s64 $d1, 1, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigs64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0
      }
    } 
  },
//mad24hi_pair[18]
  {
    "mad24_hi_u32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      operand_offset + reg_size * 2 + wavesz_size, 
      0
      }
    }
  },
//mad24hi_pair[19]
  {
    "mad24_hi_u32 $s1, WAVESIZE, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      RoundUp8(operand_offset + reg_size * 2 + wavesz_size), 
      0
      }
    }
  },
//mad24hi_pair[20]
  {
    "mad24_hi_u32 $s1, WAVESIZE, $s3, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      operand_offset + reg_size, 
      0
      }
    }
  },
//mad24hi_pair[21]
  {
    "mad24_hi_u32 $s1, WAVESIZE, 1, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + wavesz_size), 
      RoundUp8(operand_offset + reg_size + wavesz_size) + immed_size, 
      0
      }
    }
  },
//mad24hi_pair[22]
  {
    "mad24_hi_u32 $s1, WAVESIZE, 1, 1;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + wavesz_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size + wavesz_size) + immed_size), 
      0
      }
    }
  },
//mad24hi_pair[23]
  {
    "mad24_hi_u64 $d1, WAVESIZE, 1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + wavesz_size), 
      operand_offset + reg_size, 
      0
      }
    }
  },
//mad24hi_pair[24]
  {
    "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, $d4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      operand_offset + reg_size + wavesz_size, 
      0
      }
    }
  },
//mad24hi_pair[25]
  {
    "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + wavesz_size), 
      0
      }
    }
  },
//mad24hi_pair[26]
  {
    "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMad24Hi,
    Brigu64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      0
      }
    }
  },
};

class Mad24HiInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad24_hi_invalid[] = {
  "mad24_hi_s32 $s1;",
  "mad24_hi_s64 $s1, $s2;",
  "mad24_hi_u32 1, 1, $s3;",
  "mad24_hi_u64 WAVESIZE, 1, 1;"
};

}  // namespace brig
}  // namespace hsa
#endif //MUL_TEST_H_
