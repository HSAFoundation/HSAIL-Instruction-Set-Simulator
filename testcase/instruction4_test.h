/* Copyright 2012 <MulticorewareInc> */

#ifndef INSTRUCTION4_TEST_H_
#define INSTRUCTION4_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

struct Instruction4Test{
  const char* str;
  BrigInstBase ref;
};

class Instruction4MadTest : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4mad_pair[27] ={
//instruction4mad_pair[0]
  {
    "mad_s32 $s1, $s2, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
      {
     operand_offset, 
     operand_offset + reg_size, 
     operand_offset + reg_size *2, 
     operand_offset + reg_size *3, 
        0
      }
    }
  },
//instruction4mad_pair[1]
  {
    "mad_s32 $s1, $s2, $s3, 1;",
    {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[2]
  {
    "mad_s32 $s1, $s2, $s3, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size *2, 
      operand_offset + reg_size *3, 
      0
      }
    }
  },
//instruction4mad_pair[3]
  {
    "mad_s32 $s1, $s2, 1, $s4;",
    {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size *2), 
      RoundUp8(operand_offset + reg_size *2) + immed_size, 
      0
      }
    }
  },
//instruction4mad_pair[4]
  {
    "mad_s32 $s1, $s2, 1, 1;",
     {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size *2), 
      RoundUp8(RoundUp8(operand_offset + reg_size *2) + immed_size), 
      0
      }
    }
  },
//instruction4mad_pair[5]
  {
    "mad_s32 $s1, $s2, 1, WAVESIZE;",
      {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size *2), 
      RoundUp8(operand_offset + reg_size *2) + immed_size, 
      0
      }
    }
  },
//instruction4mad_pair[6]
  {
    "mad_s32 $s1, $s2, WAVESIZE, $s4;",
     {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size *2, 
      operand_offset + reg_size *2 + wavesz_size, 
      0
      }
    }
  },
//instruction4mad_pair[7]
  {
    "mad_s32 $s1, $s2, WAVESIZE, 1;",
    {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size *2, 
      RoundUp8(operand_offset + reg_size *2 + wavesz_size), 
      0
      }
    }
  },
//instruction4mad_pair[8]
  {
    "mad_s32 $s1, $s2, WAVESIZE, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigMad,
    Brigs32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size *2, 
      operand_offset + reg_size *2, 
      0
      }
    }
  },
//instruction4mad_pair[9]
  {
    "mad_s64 $d1, 1, $d3, $d4;",
      {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[10]
  {
    "mad_s64 $d1, 1, $d3, 1;",
    {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[11]
  {
    "mad_s64 $d1, 1, $d3, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[12]
  {
    "mad_s64 $d1, 1, 1, $d4;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[13]
  {
    "mad_s64 $d1, 1, 1, 1;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[14]
  {
    "mad_s64 $d1, 1, 1, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[15]
  {
     "mad_s64 $d1, 1, WAVESIZE, $d4;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[16]
  {
    "mad_s64 $d1, 1, WAVESIZE, 1;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[17]
  {
    "mad_s64 $d1, 1, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[18]
  {
    "mad_u32 $s1, WAVESIZE, $s3, $s4;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[19]
  {
    "mad_u32 $s1, WAVESIZE, $s3, 1;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[20]
  {
    "mad_u32 $s1, WAVESIZE, $s3, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[21]
  {
    "mad_u32 $s1, WAVESIZE, 1, $s4;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[22]
  {
    "mad_u32 $s1, WAVESIZE, 1, 1;",
     {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[23]
  {
    "mad_u64 $d1, WAVESIZE, 1, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[24]
  {
    "mad_u64 $d1, WAVESIZE, WAVESIZE, $s4;",
    {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[25]
  {
    "mad_u64 $d1, WAVESIZE, WAVESIZE, 1;",
   {
    32,
    BrigEInstBase,
    BrigMad,
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
//instruction4mad_pair[26]
  {
    "mad_u64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;",
      {
    32,
    BrigEInstBase,
    BrigMad,
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
  }
};

class Instruction4MadInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad_invalid[4] = {
  "mad_s32 $s1;",
  "mad_s64 $s1, $s2;",
  "mad_u32 1, 1, $s3, $s4;",
  "mad_u64 WAVESIZE, 1, 1, 1;"
};

class Instruction4MadFtzRound : public ::testing::TestWithParam<int>
{

};

struct Instruction4ModTest{
  const char* str;
  BrigInstMod ref;
};

struct Instruction4ModTest instruction4madftzround_pair[16] ={
//instruction4mad_pair[0]
  {
    "mad_ftz_f32 $s1, $s2, $s3, 1.0f;",
    {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      RoundUp8(operand_offset + reg_size * 3), 
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//instruction4mad_pair[1]
  {
    "mad_ftz_f64 $d1, $d2, 1.0, $d4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(operand_offset + reg_size * 2) + immed_size, 
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//instruction4mad_pair[2]
  {
    "mad_near_f32 $s1, 1.0f, $s3, $s4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size, 
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//instruction4mad_pair[3]
  {
    "mad_near_f64 $d1, 1.0, $d3, 1.0;",
    {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size), 
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//instruction4mad_pair[4]
  {
    "mad_zero_f32 $s1, 1.0f, 1.0f, 1.0f;",
      {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size), 
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//instruction4mad_pair[5]
  {
    "mad_zero_f64 $d1, $d2, $d3, $d4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 3, 
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//instruction4mad_pair[6]
  {
    "mad_up_f32 $s1, $s2, 1.0f, $s4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(operand_offset + reg_size * 2) + immed_size, 
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//instruction4mad_pair[7]
  {
    "mad_up_f64 $d1, $d2, 1.0, 1.0;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size), 
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//instruction4mad_pair[8]
  {
    "mad_down_f32 $s1, 1.0f, $s3, 1.0f;",
      {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size), 
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//instruction4mad_pair[9]
  {
    "mad_down_f64 $d1, 1.0, 1.0, $d4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + immed_size, 
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//instruction4mad_pair[10]
  {
    "mad_ftz_zero_f32 $s1, $s2, $s3, $s4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
    {
     operand_offset,
     operand_offset + reg_size, 
     operand_offset + reg_size * 2, 
     operand_offset + reg_size * 3, 
             0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//instruction4mad_pair[11]
  {
    "mad_ftz_zero_f64 $d1, $d2, $d3, 1.0;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
      {
     operand_offset,
     operand_offset + reg_size, 
     operand_offset + reg_size * 2, 
     RoundUp8(operand_offset + reg_size * 3), 
             0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//instruction4mad_pair[12]
  {
    "mad_ftz_up_f32 $s1, $s2, 1.0f, 1.0f;",
      {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
     {
     operand_offset,
     operand_offset + reg_size, 
     RoundUp8(operand_offset + reg_size * 2), 
     RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size), 
             0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//instruction4mad_pair[13]
  {
    "mad_ftz_up_f64 $d1, 1.0, $d3, $d4;",
     {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
      {
     operand_offset,
     RoundUp8(operand_offset + reg_size), 
     RoundUp8(operand_offset + reg_size) + immed_size,  
     RoundUp8(operand_offset + reg_size) + immed_size + reg_size, 
             0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//instruction4mad_pair[14]
  {
    "mad_ftz_down_f32 $s1, 1.0f, 1.0f, $s4;",
      {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf32,
    BrigNoPacking,
      {
     operand_offset,
     RoundUp8(operand_offset + reg_size), 
     RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),  
     RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + immed_size,  
             0
      },
   {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  },
//instruction4mad_pair[15]
  {
    "mad_ftz_down_f64 $d1, 1.0, 1.0, 1.0;",
    {
    36,
    BrigEInstMod,
    BrigMad,
    Brigf64,
    BrigNoPacking,
     {
     operand_offset,
     RoundUp8(operand_offset + reg_size), 
     RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),  
     RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
       immed_size),  
             0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  },
};

class Instruction4MadFtzRoundInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad_ftz_round_invalid[9] = {
  "mad_ftz_f16 $s1;",
  "mad_near_f32 $s1, $s2,;",
  "mad_zero_f64 $s1, $s2, $s3",
  "mad_up_f16 WAVESIZE, $s2, $s3, $s4;",
  "mad_down_f32 1, WAVESIZE, $s3, $s4;",
  "mad_ftz_zero_f64 $s1, $s2, WAVESIZE, $s4;",
  "madd_ftz_up_f16 $s1, $s2, $s3, $s4;",
  "mad_ftzz_down_f32 1, 1, 1, 1;",
  "mad_ff64 1, 1,WAVESIZE,WAVESIZE;"
};

class Instruction4Extract : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4extract_pair[10] ={
//instruction4extract_pair[0]
  {
    "extract_b32 $s1, $s2, $s3, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
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
//instruction4extract_pair[1]
  {
    "extract_b32 $s1, $s2, 1, $s4;",
    {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
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
//instruction4extract_pair[2]
  {
    "extract_b32 $s1, $s2, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
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
//instruction4extract_pair[3]
  {
    "extract_b64 $d1, 1, $s3, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
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
//instruction4extract_pair[4]
  {
    "extract_b64 $d1, 1, 1, $s4;",
     {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
    BrigNoPacking,
   {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      }
    }
  },
//instruction4extract_pair[5]
  {
    "extract_b64 $d1, 1, WAVESIZE, $s4;",
    {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
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
//instruction4extract_pair[6]
  {
     "extract_b64 $d1, 1, WAVESIZE, 1;",
     {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
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
//instruction4extract_pair[7]
  {
    "extract_b64 $d1, 1, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb64,
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
//instruction4extract_pair[8]
  {
    "extract_b32 $s1, WAVESIZE, 1, $s4;",
     {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
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
//instruction4extract_pair[9]
  {
    "extract_b32 $s1, WAVESIZE, 1, 1;",
     {
    32,
    BrigEInstBase,
    BrigExtract,
    Brigb32,
    BrigNoPacking,
   {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size + wavesz_size),
      RoundUp8(RoundUp8(operand_offset + reg_size + wavesz_size) + immed_size),
      0
      }
    }
  }
};

class Instruction4ExtractInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_extract_invalid[23] = {
  "extract_b32 $s1;",
  "extract_b32 $s1, $s2,;",
  "extract_b32 $s1, $s2, $s3",
  "extract_b32 $s1, $s2, $s3,,$s4;",
  "extract_b32 $s1, $s2, $s3, $4;",
  "extract_b32 $s1, $s2, $s3, s4;",
  "extract_b32 $s1, $s2, $s3, $s130;",
  "extract_b32 $s1, $s2, $s3, $s4, $s5;",
  "extract_b32 $s1, $s2, $s3, $s4, 100;",
  "extract_b32 100, $s2, $s3, $s4;",
  "extract_b32 WAVESIZE, $s2, $s3, $s4;",
  "extract_b64 $s1;",
  "extract_b64 $s1, $s2,;",
  "extract_b64 $s1, $s2, $s3",
  "extract_b64 $s1, $s2, $s3,,$s4;",
  "extract_b64 $s1, $s2, $s3, $4;",
  "extract_b64 $s1, $s2, $s3, s4;",
  "extract_b64 $s1, $s2, $s3, $s128;",
  "extract_b64 $s1, $s2, $s3, $s4, $s5;",
  "extract_b64 $s1, $s2, $s3, $s4, 100;",
  "extract_b64 100, $s2, $s3, $s4;",
  "extract_b64 WAVESIZE, $s2, $s3, $s4;",
  "extract_b64 $d1, $d2, $d3, $d4;"
};

class Instruction4Insert : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4insert_pair[10] ={
//instruction4insert_pair[0]
  {
    "insert_b32 $s1, $s2, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
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
//instruction4insert_pair[1]
  {
    "insert_b32 $s1, $s2, $s3, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
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
//instruction4insert_pair[2]
  {
    "insert_b32 $s1, $s2, 1, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
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
//instruction4insert_pair[3]
  {
    "insert_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
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
//instruction4insert_pair[4]
  {
    "insert_b64 $d1, 1, 1, $d4;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      }
    }
  },
//instruction4insert_pair[5]
  {
    "insert_b64 $d1, 1, WAVESIZE, 1;",
     {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
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
//instruction4insert_pair[6]
  {
    "insert_b64 $d1, 1, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
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
//instruction4insert_pair[7]
  {
    "insert_b32 $s1, WAVESIZE, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb32,
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
//instruction4insert_pair[8]
  {
    "insert_b64 $d1, WAVESIZE, 1, 1;",
    {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
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
//instruction4insert_pair[9]
  {
    "insert_b64 $d1, WAVESIZE, WAVESIZE, 1;",
     {
    32,
    BrigEInstBase,
    BrigInsert,
    Brigb64,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size + wavesz_size),
      0
      }
    }
  }
};

class Instruction4InsertInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_insert_invalid[22] = {
  "insert_b32 $s1;",
  "insert_b32 $s1, $s2,;",
  "insert_b32 $s1, $s2, $s3",
  "insert_b32 $s1, $s2, $s3,,$s4;",
  "insert_b32 $s1, $s2, $s3, $4;",
  "insert_b32 $s1, $s2, $s3, s4;",
  "insert_b32 $s1, $s2, $s3, $s128;",
  "insert_b32 $s1, $s2, $s3, $s4, $s5;",
  "insert_b32 $s1, $s2, $s3, $s4, 100;",
  "insert_b32 100, $s2, $s3, $s4;",
  "insert_b32 WAVESIZE, $s2, $s3, $s4;",
  "insert_b64 $d1;",
  "insert_b64 $d1, $d2,;",
  "insert_b64 $d1, $d2, $d3",
  "insert_b64 $d1, $d2, $d3,,$d4;",
  "insert_b64 $d1, $d2, $d3, $4;",
  "insert_b64 $d1, $d2, $d3, d4;",
  "insert_b64 $d1, $d2, $d3, $s128;",
  "insert_b64 $d1, $d2, $d3, $d4, $s5;",
  "insert_b64 $d1, $d2, $d3, $d4, 100;",
  "insert_b64 100, $d2, $d3, $d4;",
  "insert_b64 WAVESIZE, $d2, $d3, $d4;"
};

class Instruction4Bitselect : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4bitselect_pair[10] ={
//instruction4bitselect_pair[0]
  {
    "bitselect_b32 $s1, $s2, $s3, $s4;",
     {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
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

//instruction4bitselect_pair[1]
  {
    "bitselect_b32 $s1, $s2, 1, WAVESIZE;",
     {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
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

//instruction4bitselect_pair[2]
  {
    "bitselect_b32 $s1, $s2, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
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
//instruction4bitselect_pair[3]
  {
    "bitselect_b64 $d1, 1, $d3, $d4;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
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
//instruction4bitselect_pair[4]
  {
    "bitselect_b64 $d1, 1, $d3, 1;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
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
//instruction4bitselect_pair[5]
  {
    "bitselect_b64 $d1, 1, $d3, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
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
//instruction4bitselect_pair[6]
  {
    "bitselect_b64 $d1, 1, 1, $d4;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
    BrigNoPacking,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      }
    }
  },

//instruction4bitselect_pair[7]
  {
    "bitselect_b64 $d1, 1, WAVESIZE, WAVESIZE;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb64,
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
//instruction4bitselect_pair[8]
  {
    "bitselect_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
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
//instruction4bitselect_pair[9]
  {
    "bitselect_b32 $s1, WAVESIZE, $s3, 1;",
    {
    32,
    BrigEInstBase,
    BrigBitSelect,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      RoundUp8(operand_offset + reg_size * 2 + wavesz_size),
      0
      }
    }
  }
};

class Instruction4BitselectInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_bitselect_invalid[22] = {
  "bitselect_b32 $s1;",
  "bitselect_b32 $s1, $s2,;",
  "bitselect_b32 $s1, $s2, $s3",
  "bitselect_b32 $s1, $s2, $s3,,$s4;",
  "bitselect_b32 $s1, $s2, $s3, $4;",
  "bitselect_b32 $s1, $s2, $s3, s4;",
  "bitselect_b32 $s1, $s2, $s3, $s130;",
  "bitselect_b32 $s1, $s2, $s3, $s4, $s5;",
  "bitselect_b32 $s1, $s2, $s3, $s4, 100;",
  "bitselect_b32 100, $s2, $s3, $s4;",
  "bitselect_b32 WAVESIZE, $s2, $s3, $s4;",
  "bitselect_b64 $d1;",
  "bitselect_b64 $d1, $d2,;",
  "bitselect_b64 $d1, $d2, $d3",
  "bitselect_b64 $d1, $d2, $d3,,$d4;",
  "bitselect_b64 $d1, $d2, $d3, $4;",
  "bitselect_b64 $d1, $d2, $d3, d4;",
  "bitselect_b64 $d1, $d2, $d3, $s130;",
  "bitselect_b64 $d1, $d2, $d3, $d4, $s5;",
  "bitselect_b64 $d1, $d2, $d3, $d4, 100;",
  "bitselect_b64 100, $d2, $d3, $d4;",
  "bitselect_b64 WAVESIZE, $d2, $d3, $d4;"
};

class Instruction4ShuffleTest : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4shuffle_pair[11] ={
//instruction4shuffle_pair[0]
  {
    "shuffle_s8x4 $s1, $s2, $s3, 0x55;",
     {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigs8x4,
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
//instruction4shuffle_pair[1]
  {
    "shuffle_u8x4 $s1, $s2, 1, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigu8x4,
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
//instruction4shuffle_pair[2]
  {
    "shuffle_s16x2 $s1, 1, $s3, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigs16x2,
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
//instruction4shuffle_pair[3]
  {
    "shuffle_u16x2 $s1, 1, 1, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigu16x2,
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
//instruction4shuffle_pair[4]
  {
    "shuffle_s8x8 $d1, $d2, $d3, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigs8x8,
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
//instruction4shuffle_pair[5]
  {
    "shuffle_u8x8 $d1, $d2, 1, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigu8x8,
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
//instruction4shuffle_pair[6]
  {
    "shuffle_s16x4 $d1, 1, $d3, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigs16x4,
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
//instruction4shuffle_pair[7]
  {
    "shuffle_u16x4 $d1, 1, 1, 0x55;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigu16x4,
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
//instruction4shuffle_pair[8]
  {
    "shuffle_f32x2 $d1, $d2, $d3, 2.0;",
     {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigf32x2,
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
//instruction4shuffle_pair[9]
  {
    "shuffle_f16x2 $s1, $s2, 1.0f, 2.0f;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigf16x2,
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
//instruction4shuffle_pair[10]
  {
    "shuffle_f16x4 $d1, 1.0, $d3, 2.0;",
    {
    32,
    BrigEInstBase,
    BrigShuffle,
    Brigf16x4,
    BrigNoPacking,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),
      0
      }
    }
  }
};

class Instruction4ShuffleInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_shuffle_invalid[18] = {
  "shuffle_s8x4 $s1;",
  "shuffle_s8x4 $s1, $s2;",
  "shuffle_s8x4 $s1, $s2, $s3;",
  "shuffle_u8x4 $s1, $s2, $s3, s4;",
  "shuffle_u8x4 $s1, $s2, $s3, $4;",
  "shuffle_u8x4 $s1, $s2, $s3, $s4,;",
  "shuffle_s8x8 $s1, $s2, $s3, $s4, $5;",
  "shuffle_s8x8 $s1, $s2, $s3, $s130;",
  "shuffle_u8x8 $s1, $s2, $s3, $s4, $5;",
  "shuffle_s16x2 $s1, $s2, $s3, $s4;",
  "shuffle_s16x2 $s1, $s2, $s3, ;",
  "shuffle_s16x2 $s1, $s2, $s3, , ;",
  "shuffle_u16x2 100, $s2, $s3, $s4;",
  "shuffle_u16x2 WAVESIZE, $s2, $s3, $s4;",
  "shuffle_u16x2 $s1, WAVESIZE, $s3, $s4;",
  "shuffle_f16x2 $s1, $s2, WAVESIZE, $s4;",
  "shuffle_s16x4 $d1, $d2, $d3, WAVESIZE;",
  "shufflee_f32x2 $d1, $d2, $d3, $d4;"
};

class Instruction4CmovTest : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4cmov_pair[21] ={
//instruction4cmov_pair[0]
  {
    "cmov_b1 $c1, $c2, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb1,
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
//instruction4cmov_pair[1]
  {
    "cmov_b1 $c1, $c2, $s3, 1;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb1,
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
//instruction4cmov_pair[2]
  {
    "cmov_b32 $s1, $c2, 1, $s4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb32,
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
//instruction4cmov_pair[3]
  {
    "cmov_b32 $s1, $c2, 1, 1;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb32,
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
//instruction4cmov_pair[4]
  {
    "cmov_b32 $s1, 1, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb32,
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
//instruction4cmov_pair[5]
  {
    "cmov_b64 $d1, 0, $d2, 0;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb64,
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
//instruction4cmov_pair[6]
  {
    "cmov_b64 $d1, 1, 1, $d4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb64,
    BrigNoPacking,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      }
    }
  },
//instruction4cmov_pair[7]
  {
    "cmov_b64 $d1, 1, 1, 1;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigb64,
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
//instruction4cmov_pair[8]
  {
    "cmov_s8x4 $s1, $s2, $s3, $s4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigs8x4,
    BrigPackPP,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      0
      }
    }
  },
//instruction4cmov_pair[9]
  {
    "cmov_u8x4 $s1, $s2, $s3, 1;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigu8x4,
    BrigPackPP,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      RoundUp8(operand_offset + reg_size * 3),
      0
      }
    }
  },
//instruction4cmov_pair[10]
  {
    "cmov_s16x2 $s1, $s2, 1, $s4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigs16x2,
    BrigPackPP,
    {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(operand_offset + reg_size * 2) + immed_size,
      0
      }
    }
  },
//instruction4cmov_pair[11]
  {
    "cmov_u16x2 $s1, $s2, 1, 1;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigu16x2,
    BrigPackPP,
     {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      0
      }
    }
  },
//instruction4cmov_pair[12]
  {
    "cmov_s8x8 $d1, 1, $d3, $d4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigs8x8,
    BrigPackPP,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size,
      0
      }
    }
  },
//instruction4cmov_pair[13]
  {
    "cmov_u8x8 $d1, 1, $d3, 1;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigu8x8,
    BrigPackPP,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),
      0
      }
    }
  },
//instruction4cmov_pair[14]
  {
    "cmov_s16x4 $d1, 1, 1, $d4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigs16x4,
    BrigPackPP,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      }
    }
  },
//instruction4cmov_pair[15]
  {
    "cmov_u16x4 $d1, 1, 1, 1;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigu16x4,
    BrigPackPP,
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
//instruction4cmov_pair[16]
  {
    "cmov_s32x2 $d1, $d2, $d3, $d4;",
    {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigs32x2,
    BrigPackPP,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      0
      }
    }
  },
//instruction4cmov_pair[17]
  {
    "cmov_u32x2 $d1, $d2, $d3, 1;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigu32x2,
    BrigPackPP,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      RoundUp8(operand_offset + reg_size * 3),
      0
      }
    }
  },
//instruction4cmov_pair[18]
  {
    "cmov_f16x2 $s1, $s2, 1.0f, $s4;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigf16x2,
    BrigPackPP,
    {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(operand_offset + reg_size * 2) + immed_size,
      0
      }
    }
  },
//instruction4cmov_pair[19]
  {
    "cmov_f16x4 $d1, $d2, 1.0, 1.0;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigf16x4,
    BrigPackPP,
     {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      0
      }
    }
  },
//instruction4cmov_pair[20]
  {
    "cmov_f32x2 $d1, 1.0, $d3, $d4;",
     {
    32,
    BrigEInstBase,
    BrigCmov,
    Brigf32x2,
    BrigPackPP,
     {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size,
      0
      }
    }
  }
};

class Instruction4CmovInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_cmov_invalid[18] = {
  "cmov_s8x4 $s1;",
  "cmov_s8x4 $s1, $s2;",
  "cmov_s8x4 $s1, $s2, $s3;",
  "cmov_u8x4 $s1, $s2, $s3, s4;",
  "cmov_u8x4 $s1, $s2, $s3, $4;",
  "cmov_u8x4 $s1, $s2, $s3, $s4,;",
  "cmov_s8x8 $s1, $s2, $s3, $s4, $5;",
  "cmov_s8x8 $s1, $s2, $s3, $s130;",
  "cmov_u8x8 $s1, $s2, $s3, $s4, $5;",
  "cmo_s16x2 $s1, $s2, $s3, $s4;,",
  "cmov_s16x2 $s1, $s2, $s3, ;",
  "cmov_s16x2 $s1, $s2, $s3, , ;",
  "cmov_u16x2 100, $s2, $s3, $s4;",
  "cmov_u16x2 WAVESIZE, $s2, $s3, $s4;",
  "cmov_u16x2 $s1, WAVESIZE, $s3, $s4;",
  "cmov_f16x2 $s1, $s2, WAVESIZE, $s4;",
  "cmov_s16x4 $d1, $d2, $d3, WAVESIZE;",
  "cmove_f32x2 $d1, $d2, $d3, $d4;"
};

class Instruction4FmaTest : public ::testing::TestWithParam<int>
{

};

struct Instruction4ModTest instruction4fma_pair[16] ={
//instruction4fma_pair[0]
  {
    "fma_ftz_f32 $s1, $s2, $s3, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      RoundUp8(operand_offset + reg_size * 3),
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//instruction4fma_pair[1]
  {
    "fma_ftz_f64 $d1, $d2, 1.0, $d4;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(operand_offset + reg_size * 2) + immed_size,
      0
      },
    {1, 1, 0, 1, 0, 0, 0}// ftz
    }
  },
//instruction4fma_pair[2]
  {
    "fma_near_f32 $s1, 1.0f, $s3, $s4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size,
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//instruction4fma_pair[3]
  {
    "fma_near_f64 $d1, 1.0, $d3, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),
      0
      },
    {1, 1, 0, 0, 0, 0, 0}// near
    }
  },
//instruction4fma_pair[4]
  {
    "fma_zero_f32 $s1, 1.0f, 1.0f, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size),
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//instruction4fma_pair[5]
  {
    "fma_zero_f64 $d1, $d2, $d3, $d4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      0
      },
    {1, 1, 1, 0, 0, 0, 0}// zero
    }
  },
//instruction4fma_pair[6]
  {
    "fma_up_f32 $s1, $s2, 1.0f, $s4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(operand_offset + reg_size * 2) + immed_size,
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//instruction4fma_pair[7]
  {
    "fma_up_f64 $d1, $d2, 1.0, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      0
      },
    {1, 1, 2, 0, 0, 0, 0}// up
    }
  },
//instruction4fma_pair[8]
  {
    "fma_down_f32 $s1, 1.0f, $s3, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size + reg_size),
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//instruction4fma_pair[9]
  {
    "fma_down_f64 $d1, 1.0, 1.0, $d4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      },
    {1, 1, 3, 0, 0, 0, 0}// down
    }
  },
//instruction4fma_pair[10]
  {
    "fma_ftz_zero_f32 $s1, $s2, $s3, $s4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//instruction4fma_pair[11]
  {
    "fma_ftz_zero_f64 $d1, $d2, $d3, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size * 2,
      RoundUp8(operand_offset + reg_size * 3),
      0
      },
    {1, 1, 1, 1, 0, 0, 0}// ftz_zero
    }
  },
//instruction4fma_pair[12]
  {
    "fma_ftz_up_f32 $s1, $s2, 1.0f, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//instruction4fma_pair[13]
  {
    "fma_ftz_up_f64 $d1, 1.0, $d3, $d4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
   {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(operand_offset + reg_size) + immed_size,
      RoundUp8(operand_offset + reg_size) + immed_size + reg_size,
      0
      },
    {1, 1, 2, 1, 0, 0, 0}// ftz_up
    }
  },
//instruction4fma_pair[14]
  {
    "fma_ftz_down_f32 $s1, 1.0f, 1.0f, $s4;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf32,
    BrigNoPacking,
      {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size,
      0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  },
//instruction4fma_pair[15]
  {
    "fma_ftz_down_f64 $d1, 1.0, 1.0, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFma,
    Brigf64,
    BrigNoPacking,
   {
      operand_offset,
      RoundUp8(operand_offset + reg_size),
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size),
      0
      },
    {1, 1, 3, 1, 0, 0, 0} // ftz_down
    }
  }
};

class Instruction4FmaFtzRoundInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_fma_ftz_round_invalid[9] = {
  "fma_ftz_f16 $s1;",
  "fma_near_f32 $s1, $s2,;",
  "fma_zero_f64 $s1, $s2, $s3",
  "fma_up_f16 WAVESIZE, $s2, $s3, $s4;",
  "fma_down_f32 1, WAVESIZE, $s3, $s4;",
  "fma_ftz_zero_f64 $s1, $s2, WAVESIZE, $s4;",
  "fma_ftz_up_f16 $s1, $s2, $s3, $s4;",
  "fma_ftzz_down_f32 1, 1, 1, 1;",
  "fma_ff64 1, 1,WAVESIZE,WAVESIZE;"
};

class Instruction4BitAlign : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4bitalign_pair[7] ={
//instruction4bitalign_pair[0]
  {
    "bitalign_b32 $s1, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[1]
  {
    "bitalign_b32 $s1, $s2, $s3, 8;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[2]
  {
    "bitalign_b32 $s1, $s2, $s3, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[3]
  {
    "bitalign_b32 $s1, $s2, 8, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[4]
  {
    "bitalign_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[5]
  {
    "bitalign_b32 $s1, 8, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
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
//instruction4bitalign_pair[6]
  {
    "bitalign_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitAlign,
    Brigb32,
    BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4BitAlignInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_bitalign_invalid[9]={
  "bitalign_b32 $s1;",
  "bitalign_b32 $s1, $s2, $s3, $s130;",
  "bitalign_b32 $s1, $s2, $s3, $s4, 100;",
  "bitalignn_b32 $s1, $s2, $s3, $s4;",
  "bitalign_b64 $s1, $s2, $s3, $s4;",
  "bitalign_b32 $s1, $s2, $s3, s4;",
  "bitalign_b32 $s1, WAVESIZE, $s3, $4;",
  "bitalign_b32 100, $s2, $s3, $s4;",
  "bitalign_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4ByteAlign : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4bytealign_pair[7] ={
//instruction4bytealign_pair[0]
  {
    "bytealign_b32 $s1, $s2, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[1]
  {
    "bytealign_b32 $s1, $s2, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[2]
  {
    "bytealign_b32 $s1, $s2, $s3, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[3]
  {
    "bytealign_b32 $s1, $s2, 1, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[4]
  {
    "bytealign_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[5]
  {
    "bytealign_b32 $s1, 1, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
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
//instruction4bytealign_pair[6]
  {
    "bytealign_b32 $s1, WAVESIZE, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigByteAlign,
    Brigb32,
    BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4ByteAlignInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_bytealign_invalid[9]={
  "bytealign_b32 $s1;",
  "bytealign_b32 $s1, $s2, $s3, $s130;",
  "bytealign_b32 $s1, $s2, $s3, $s4, 100;",
  "bytealignn_b32 $s1, $s2, $s3, $s4;",
  "bytealign_b64 $s1, $s2, $s3, $s4;",
  "bytealign_b32 $s1, $s2, $s3, s4;",
  "bytealign_b32 $s1, WAVESIZE, $s3, $4;",
  "bytealign_b32 100, $s2, $s3, $s4;",
  "bytealign_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4Lerp : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4lerp_pair[7] ={
//instruction4lerp_pair[0]
  {
    "lerp_b32 $s1, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[1]
  {
    "lerp_b32 $s1, $s2, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[2]
  {
    "lerp_b32 $s1, $s2, $s3, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[3]
  {
    "lerp_b32 $s1, $s2, 1, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[4]
  {
    "lerp_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[5]
  {
    "lerp_b32 $s1, 1, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
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
//instruction4lerp_pair[6]
  {
    "lerp_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLerp,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4LerpInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_lerp_invalid[9]={
  "lerp_b32 $s1;",
  "lerp_b32 $s1, $s2, $s3, $s130;",
  "lerp_b32 $s1, $s2, $s3, $s4, 100;",
  "lerpn_b32 $s1, $s2, $s3, $s4;",
  "lerp_b64 $s1, $s2, $s3, $s4;",
  "lerp_b32 $s1, $s2, $s3, s4;",
  "lerp_b32 $s1, WAVESIZE, $s3, $4;",
  "lerp_b32 100, $s2, $s3, $s4;",
  "lerp_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4Sad : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4sad_pair[7] ={
//instruction4sad_pair[0]
  {
    "sad_b32 $s1, $s2, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[1]
  {
    "sad_b32 $s1, $s2, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[2]
  {
    "sad_b32 $s1, $s2, $s3, WAVESIZE;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[3]
  {
    "sad_b32 $s1, $s2, 1, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[4]
  {
    "sad_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[5]
  {
    "sad_b32 $s1, 1, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
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
//instruction4sad_pair[6]
  {
    "sad_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad,
    Brigb32,
    BrigNoPacking,
      {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4SadInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_sad_invalid[9]={
  "sad_b32 $s1;",
  "sad_b32 $s1, $s2, $s3, $s130;",
  "sad_b32 $s1, $s2, $s3, $s4, 100;",
  "sadn_b32 $s1, $s2, $s3, $s4;",
  "sad_b64 $s1, $s2, $s3, $s4;",
  "sad_b32 $s1, $s2, $s3, s4;",
  "sad_b32 $s1, WAVESIZE, $s3, $4;",
  "sad_b32 100, $s2, $s3, $s4;",
  "sad_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4Sad2 : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4sad2_pair[7] ={
//instruction4sad2_pair[0]
  {
    "sad2_b32 $s1, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[1]
  {
    "sad2_b32 $s1, $s2, $s3, 1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[2]
  {
    "sad2_b32 $s1, $s2, $s3, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[3]
  {
    "sad2_b32 $s1, $s2, 1, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[4]
  {
    "sad2_b32 $s1, $s2, WAVESIZE, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[5]
  {
    "sad2_b32 $s1, 1, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
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
//instruction4sad2_pair[6]
  {
    "sad2_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad2,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4Sad2Invalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_sad2_invalid[9]={
  "sad2_b32 $s1;",
  "sad2_b32 $s1, $s2, $s3, $s130;",
  "sad2_b32 $s1, $s2, $s3, $s4, 100;",
  "sad2n_b32 $s1, $s2, $s3, $s4;",
  "sad2_b64 $s1, $s2, $s3, $s4;",
  "sad2_b32 $s1, $s2, $s3, s4;",
  "sad2_b32 $s1, WAVESIZE, $s3, $4;",
  "sad2_b32 100, $s2, $s3, $s4;",
  "sad2_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4Sad4 : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4sad4_pair[7] ={
//instruction4sad4_pair[0]
  {
    "sad4_b32 $s1, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[1]
  {
    "sad4_b32 $s1, $s2, $s3, 1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[2]
  {
    "sad4_b32 $s1, $s2, $s3, WAVESIZE;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[3]
  {
    "sad4_b32 $s1, $s2, 1, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[4]
  {
    "sad4_b32 $s1, $s2, WAVESIZE, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[5]
  {
    "sad4_b32 $s1, 1, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
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
//instruction4sad4_pair[6]
  {
    "sad4_b32 $s1, WAVESIZE, $s3, $s4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4Sad4Invalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_sad4_invalid[9]={
  "sad4_b32 $s1;",
  "sad4_b32 $s1, $s2, $s3, $s130;",
  "sad4_b32 $s1, $s2, $s3, $s4, 100;",
  "sad4n_b32 $s1, $s2, $s3, $s4;",
  "sad4_b64 $s1, $s2, $s3, $s4;",
  "sad4_b32 $s1, $s2, $s3, s4;",
  "sad4_b32 $s1, WAVESIZE, $s3, $4;",
  "sad4_b32 100, $s2, $s3, $s4;",
  "sad4_b32 WAVESIZE, $s2, $s3, $s4;"
};

class Instruction4Sad4Hi : public ::testing::TestWithParam<int>
{

};

struct Instruction4Test instruction4sad4hi_pair[7] ={
//instruction4sad4hi_pair[0]
  {
    "sad4hi_b32 $s1, $s2, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[1]
  {
    "sad4hi_b32 $s1, $s2, $s3, 1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[2]
  {
    "sad4hi_b32 $s1, $s2, $s3, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[3]
  {
    "sad4hi_b32 $s1, $s2, 1, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[4]
  {
    "sad4hi_b32 $s1, $s2, WAVESIZE, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[5]
  {
    "sad4hi_b32 $s1, 1, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
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
//instruction4sad4hi_pair[6]
  {
    "sad4hi_b32 $s1, WAVESIZE, $s3, $s4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSad4Hi,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset,
      operand_offset + reg_size,
      operand_offset + reg_size + wavesz_size,
      operand_offset + reg_size * 2 + wavesz_size,
      0
      }
    }
  }
};

class Instruction4Sad4HiInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_array_sad4hi_invalid[9]={
  "sad4hi_b32 $s1;",
  "sad4hi_b32 $s1, $s2, $s3, $s130;",
  "sad4hi_b32 $s1, $s2, $s3, $s4, 100;",
  "sad4hin_b32 $s1, $s2, $s3, $s4;",
  "sad4hi_b64 $s1, $s2, $s3, $s4;",
  "sad4hi_b32 $s1, $s2, $s3, s4;",
  "sad4hi_b32 $s1, WAVESIZE, $s3, $4;",
  "sad4hi_b32 100, $s2, $s3, $s4;",
  "sad4hi_b32 WAVESIZE, $s2, $s3, $s4;"
};


}  // namespace brig
}  // namespace hsa
#endif //INSTRUCTION4_TEST_H_
