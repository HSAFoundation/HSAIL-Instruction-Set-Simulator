/* Copyright 2012 <MulticorewareInc> */

#ifndef INSTRUCTION3_TEST_H_
#define INSTRUCTION3_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

struct Instruction3Test{
  const char* str;
  BrigInstBase ref;
};

class Instruction3OpAddInt: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3_add_pair[42] = {
//src0:reg u32
//instruction3_addint_pair[0]
  {
    "add_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[1]
  {
    "add_u32 $s1, $s2, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[2]
  {
    "add_u32 $s1, $s2, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAdd,
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
//src0:reg s32
//instruction3_addint_pair[3]
  {
    "add_s32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[4]
  {
    "add_s32 $s1, $s2, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[5]
  {
    "add_s32 $s1, $s2, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:reg u64	
//instruction3_addint_pair[6]
  {
    "add_u64 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[7]
  {
    "add_u64 $d1, $d2, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[8]
  {
    "add_u64 $d1, $d2, WAVESIZE;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:reg s64
//instruction3_addint_pair[9]
  {
    "add_s64 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[10]
  {
    "add_s64 $d1, $d2, 0x4356789;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[11]
  {
    "add_s64 $d1, $d2, WAVESIZE;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:immed value u32
//instruction3_addint_pair[12]
  {
    "add_u32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[13]
  {
    "add_u32 $s1, 0x100011, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[14]
  {
    "add_u32 $s1, 0x100011, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:immed value s32
//instruction3_addint_pair[15]
  {
    "add_s32 $s1, 0x100011, $s3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[16]
  {
    "add_s32 $s1, 0x100011, 0x4356789;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[17]
  {
    "add_s32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:immed value u64
//instruction3_addint_pair[18]
  {
    "add_u64 $d1, 0x1703a, $d3;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[19]
  {
    "add_u64 $d1, 0x1703a, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[20]
  {
    "add_u64 $d1, 0x1703a, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:immed value s64
//instruction3_addint_pair[21]
  {
    "add_s64 $d1, 0x1703a, $d3;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[22]
  {
    "add_s64 $d1, 0x1703a, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[23]
  {
    "add_s64 $d1, 0x1703a, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:wavesize u32
//instruction3_addint_pair[24]
  {
    "add_u32 $s1, WAVESIZE, $s3;",
     {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[25]
  {
    "add_u32 $s1, WAVESIZE, 0x4356789;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[26]
  {
    "add_u32 $s1, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//src0:wavesize s32
//instruction3_addint_pair[27]
  {
    "add_s32 $s1, WAVESIZE, $s3;",
    {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[28]
  {
    "add_s32 $s1, WAVESIZE, 0x4356789;",
     {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[29]
  {
    "add_s32 $s1, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//src0:wavesize u64
//instruction3_addint_pair[30]
  {
    "add_u64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[31]
  {
    "add_u64 $d1, WAVESIZE, 0x4356789;",
    {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[32]
  {
    "add_u64 $d1, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
  },
//src0:wavesize s64
//instruction3_addint_pair[33]
  {
    "add_s64 $d1, WAVESIZE, $d3;",
    {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[34]
  {
    "add_s64 $d1, WAVESIZE, 0x4356789;",
    {
    brig_inst_base_size,                                          
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addint_pair[35]
  {
    "add_s64 $d1, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,                                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_add_pair[36]
  {
    "add_f16 $s1, $s2, 0.5f;",
    {
    brig_inst_base_size,	                                              
    BrigEInstBase,
    BrigAdd,
    Brigf16,
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
//instruction3_add_pair[37]
  {
    "add_f32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,	                                                
    BrigEInstBase,
    BrigAdd,
    Brigf32,
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
//instruction3_addfloat_pair[38]
  {
    "add_f64 $d1, $d2, 0.5;", 
    {
    brig_inst_base_size,	                                                
    BrigEInstBase,
    BrigAdd,
    Brigf64,
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
//instruction3_addfloat_pair[39]
  {
    "add_f16 $s1, 72.0f, $s3;",
    {
    brig_inst_base_size,	                                                
    BrigEInstBase,
    BrigAdd,
    Brigf16,
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
//instruction3_addfloat_pair[40]
  {
    "add_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_base_size,	                                                
    BrigEInstBase,
    BrigAdd,
    Brigf32,
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
//instruction3_addfloat_pair[41]
  {
    "add_f64 $d1, 72.0, $d3;",
     {
    brig_inst_base_size,	                                                
    BrigEInstBase,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  }
};

class Instruction3OpAddPackedint: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3_addpacked_pair[10] = {
//instruction3_addpacked_pair[0]
  {
    "add_pp_u8x4 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[1]
  {
    "add_pp_sat_s8x4 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[2]
  {
    "add_ps_u16x2 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[3]
  {
    "add_ps_sat_s16x2 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[4]
  {
    "add_sp_u8x8 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[5]
  {
    "add_sp_sat_s8x8 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[6]
  {
    "add_ss_u16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[7]
  {
    "add_ss_sat_s16x4 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
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
//instruction3_addpacked_pair[8]
  {
    "add_ss_u32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigAdd,
    Brigu32x2,
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
//instruction3_addpacked_pair[9]
  {
    "add_ss_sat_s32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigAdd,
    Brigs32x2,
    BrigPackSSsat,
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


class Instruction3OpAddPackedFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_add_packed_float_pair[4] = {
//instruction3op_add_packed_float_pair[0]
  {
    "add_pp_f16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAdd,
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
//instruction3op_add_packed_float_pair[1]
  {
    "add_ps_f16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAdd,
    Brigf16x4,
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
//instruction3op_add_packed_float_pair[2]
  {
    "add_sp_f32x2 $d1, $d2, $d3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAdd,
    Brigf32x2,
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
//instruction3op_add_packed_float_pair[3]
  {
    "add_ss_f32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAdd,
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

class Instruction3OpCarry: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_carry_pair[8] = {
//instruction3op_carry_pair[0]
  {
    "carry_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[1]
  {
    "carry_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[2]
  {
    "carry_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[3]
  {
    "carry_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[4]
  {
    "carry_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[5]
  {
    "carry_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[6]
  {
    "carry_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
//instruction3op_carry_pair[7]
  {
    "carry_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigCarry,
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
  }
};

class Instruction3OpBorrow: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_borrow_pair[8] = {
//instruction3op_borrow_pair[0]
  {
    "borrow_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[1]
  {
    "borrow_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[2]
  {
    "borrow_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[3]
  {
    "borrow_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[4]
  {
    "borrow_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[5]
  {
    "borrow_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[6]
  {
    "borrow_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
//instruction3op_borrow_pair[7]
  {
    "borrow_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigBorrow,
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
  }
};

class Instruction3OpDivInt: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_div_pair[10] = {
//instruction3op_div_pair[0]
  {
    "div_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[1]
  {
    "div_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[2]
  {
    "div_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[3]
  {
    "div_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[4]
  {
    "div_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[5]
  {
    "div_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[6]
  {
    "div_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_pair[7]
  {
    "div_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigDiv,
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
//instruction3op_div_float_pair[8]
  {
    "div_f16 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                                      
    BrigEInstBase,
    BrigDiv,
    Brigf16,
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
//instruction3op_div_float_pair[9]
  {
    "div_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigDiv,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  }
};

class Instruction3OpRem: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_rem_pair[8] = {
//instruction3op_rem_pair[0]
  {
    "rem_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[1]
  {
    "rem_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[2]
  {
    "rem_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[3]
  {
    "rem_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[4]
  {
    "rem_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[5]
  {
    "rem_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[6]
  {
    "rem_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
//instruction3op_rem_pair[7]
  {
    "rem_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigRem,
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
  }
};

class Instruction3OpSubInt: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_sub_pair[11] = {
//instruction3op_sub_pair[0]
  {
    "sub_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[1]
  {
    "sub_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[2]
  {
    "sub_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[3]
  {
    "sub_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[4]
  {
    "sub_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[5]
  {
    "sub_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[6]
  {
    "sub_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_pair[7]
  {
    "sub_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_float_pair[8]
  {
    "sub_f32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
    Brigf32,
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
//instruction3op_sub_float_pair[9]
  {
    "sub_f64 $d1, $d2, 1.5;", 
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
    Brigf64,
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
//instruction3op_sub_float_pair[10]
  {
    "sub_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
    Brigf32,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  }
};

class Instruction3OpSubPackedInt: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_subpacked_pair[10] = {
//instruction3op_subpacked_pair[0]
  {
    "sub_pp_u8x4 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                                 
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[1]
  {
    "sub_pp_sat_s8x4 $s1, $s2, $s3;",
      {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[2]
  {
    "sub_ps_u16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                                 
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[3]
  {
    "sub_ps_sat_s16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[4]
  {
    "sub_sp_u8x8 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                                 
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[5]
  {
    "sub_sp_sat_s8x8 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[6]
  {
    "sub_ss_u16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[7]
  {
    "sub_ss_sat_s16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                                 
    BrigEInstBase,
    BrigSub,
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
//instruction3op_subpacked_pair[8]
  {
    "sub_ss_u32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                                 
    BrigEInstBase,
    BrigSub,
    Brigu32x2,
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
//instruction3op_subpacked_pair[9]
  {
    "sub_ss_sat_s32x2 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigSub,
    Brigs32x2,
    BrigPackSSsat,
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


class Instruction3OpSubPackedFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_sub_packed_float_pair[4] = {
//instruction3op_sub_packed_float_pair[0]
  {
    "sub_pp_f16x2 $s1, $s2, $s3;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSub,
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
//instruction3op_sub_packed_float_pair[1]
  {
    "sub_ps_f16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSub,
    Brigf16x4,
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
//instruction3op_sub_packed_float_pair[2]
  {
    "sub_sp_f32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSub,
    Brigf32x2,
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
//instruction3op_sub_packed_float_pair[3]
  {
    "sub_ss_f32x2 $d1, $d2, $d3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSub,
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

class Instruction3OpShl: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_shl_pair[8] = {
//instruction3op_shl_pair[0]
  {
    "shl_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[1]
  {
    "shl_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[2]
  {
    "shl_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[3]
  {
    "shl_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[4]
  {
    "shl_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[5]
  {
    "shl_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[6]
  {
    "shl_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_pair[7]
  {
    "shl_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShl,
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
  }
};

class Instruction3OpShlPacked: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_shl_packed_pair[16] = {
//instruction3op_shl_packed_pair[0]
  {
    "shl_u8x4 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_packed_pair[1]
  {
    "shl_s8x4 $s1, $s2, 0x34567a;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs8x4,
    BrigPackPS,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[2]
  {
    "shl_u8x8 $d1, $d2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigu8x8,
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
//instruction3op_shl_packed_pair[3]
  {
    "shl_s8x8 $d1, $d2, 0x34567a;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShl,
    Brigs8x8,
    BrigPackPS,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[4]
  {
    "shl_u16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_packed_pair[5]
  {
    "shl_s16x2 $s1, $s2, 0x34567a;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs16x2,
    BrigPackPS,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[6] 
  {
    "shl_u16x4 $d1, $d2, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
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
//instruction3op_shl_packed_pair[7]
  {
    "shl_s16x4 $d1, $d2, 0x34567a;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs16x4,
    BrigPackPS,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  }, 
//instruction3op_shl_packed_pair[8]
  {
    "shl_u8x4 $s1, 0x100011, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigu8x4,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[9]
  {
    "shl_s8x4 $s1, 0x100011, 0x34567a;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs8x4,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[10]
  {
    "shl_u8x8 $d1, 0x1703a, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigu8x8,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[11]
  {
    "shl_s8x8 $d1, 0x1703a, 0x34567a;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs8x8,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[12]
  {
    "shl_u16x2 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShl,
    Brigu16x2,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[13]
  {
    "shl_s16x2 $s1, 0x100011, 0x34567a;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShl,
    Brigs16x2,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[14]
  {
    "shl_u16x4 $d1, 0x1703a, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShl,
    Brigu16x4,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shl_packed_pair[15]
  {
    "shl_s16x4 $d1, 0x1703a, 0x34567a;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShl,
    Brigs16x4,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  }
};

class Instruction3OpShr: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_shr_pair[8] = {
//instruction3op_shr_pair[0]
  {
    "shr_u32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[1]
  {
    "shr_s32 $s1, $s2, 0x3456a;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[2]
  {
    "shr_u64 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[3]
  {
    "shr_s64 $d1, $d2, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[4]
  {
    "shr_u32 $s1, 0x100011, WAVESIZE;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[5]
  {
    "shr_s32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[6]
  {
    "shr_u64 $d1, 0x1703a, 0x3456a;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_pair[7]
  {
    "shr_s64 $d1, WAVESIZE, $d3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
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
  }
};

class Instruction3OpShrPacked: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_shr_packed_pair[16] = {
//instruction3op_shr_packed_pair[0]
  {
    "shr_u8x4 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_packed_pair[1]
  {
    "shr_s8x4 $s1, $s2, 0x345678b;",
    {
    brig_inst_base_size,                      
    BrigEInstBase,
    BrigShr,
    Brigs8x4,
    BrigPackPS,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[2]
  {
    "shr_u8x8 $d1, $d2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigShr,
    Brigu8x8,
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
//instruction3op_shr_packed_pair[3]
  {
    "shr_s8x8 $d1, $d2, 0x345678b;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigs8x8,
    BrigPackPS,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[4]
  {
    "shr_u16x2 $s1, $s2, $s3;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_packed_pair[5]
  {
    "shr_s16x2 $s1, $s2, 0x345678b;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigs16x2,
    BrigPackPS,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[6]
  {
    "shr_u16x4 $d1, $d2, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
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
//instruction3op_shr_packed_pair[7]
  {
    "shr_s16x4 $d1, $d2, 0x345678b;",
    {
    brig_inst_base_size,                      
    BrigEInstBase,
    BrigShr,
    Brigs16x4,
    BrigPackPS,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[8]
  {
    "shr_u8x4 $s1, 0x100011, $s3;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigu8x4,
    BrigPackPS,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[9]
  {
    "shr_s8x4 $s1, 0x100011, 0x345678b;",
     {
    brig_inst_base_size,                      
    BrigEInstBase,
    BrigShr,
    Brigs8x4,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[10]
  {
    "shr_u8x8 $d1, 0x1703a, $s3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigShr,
    Brigu8x8,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[11]
  {
    "shr_s8x8 $d1, 0x1703a, 0x345678b;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigs8x8,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[12]
  {
    "shr_u16x2 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigu16x2,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[13]
  {
    "shr_s16x2 $s1, 0x100011, 0x345678b;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigs16x2,
    BrigPackPS,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[14]
  {
    "shr_u16x4 $d1, 0x1703a, $s3;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigu16x4,
    BrigPackPS,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      }
    }
  },
//instruction3op_shr_packed_pair[15]
  {
    "shr_s16x4 $d1, 0x1703a, 0x345678b;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigShr,
    Brigs16x4,
    BrigPackPS,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  }
};

class Instruction3OpAnd: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_and_pair[9] = {
//instruction3op_and_pair[0]
  {
    "and_b1 $c1, $c2, $c3;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb1,
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
//instruction3op_and_pair[1]
  {
    "and_b32 $s1, $s2, 0x56123c;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb32,
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
//instruction3op_and_pair[2]
  {
    "and_b64 $d1, $d2, WAVESIZE;", 
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb64,
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
//instruction3op_and_pair[3]
  {
    "and_b1 $c1, 1, $c3;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb1,
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
//instruction3op_and_pair[4]
 {
    "and_b32 $s1, 0x100011, 0x56123c;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb32,
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
//instruction3op_and_pair[5]
 {
    "and_b64 $d1, 0x1703a, WAVESIZE;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb64,
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
//instruction3op_and_pair[6]
 {
    "and_b1 $c1, WAVESIZE, $c3;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb1,
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
//instruction3op_and_pair[7]
 {
    "and_b32 $s1, WAVESIZE, 0x56123c;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb32,
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
//instruction3op_and_pair[8]
  {
    "and_b64 $d1, WAVESIZE, WAVESIZE;",
      {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigAnd,
    Brigb64,
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
};

class Instruction3OpOr: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_or_pair[9] = {
//instruction3op_or_pair[0]
  {
    "or_b1 $c1, $c2, 1;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb1,
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
//instruction3op_or_pair[1]
  {
    "or_b32 $s1, $s2, WAVESIZE;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb32,
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
//instruction3op_or_pair[2]
  {
    "or_b64 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb64,
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
//instruction3op_or_pair[3]
  {
    "or_b1 $c1, 1, 1;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb1,
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
//instruction3op_or_pair[4]
  {
    "or_b32 $s1, 0x100011, WAVESIZE;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb32,
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
//instruction3op_or_pair[5]
  {
    "or_b64 $d1, 0x1703a, $d3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb64,
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
//instruction3op_or_pair[6]
  {
    "or_b1 $c1, WAVESIZE, 1;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb1,
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
//instruction3op_or_pair[7]
  {
    "or_b32 $s1, WAVESIZE, WAVESIZE;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb32,
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
//instruction3op_or_pair[8]
  {
    "or_b64 $d1, WAVESIZE, $d3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigOr,
    Brigb64,
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
};

class Instruction3OpXor: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_xor_pair[9] = {
//instruction3op_xor_pair[0]
  {
    "xor_b1 $c1, $c2, WAVESIZE;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb1,
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
//instruction3op_xor_pair[1]
  {
    "xor_b32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb32,
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
//instruction3op_xor_pair[2]
  {
    "xor_b64 $d1, $d2, 0x56789c;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb64,
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
//instruction3op_xor_pair[3]
  {
    "xor_b1 $c1, 1, WAVESIZE;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb1,
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
//instruction3op_xor_pair[4]
  {
    "xor_b32 $s1, 0x100011, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb32,
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
//instruction3op_xor_pair[5]
  {
    "xor_b64 $d1, 0x1703a, 0x56789c;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb64,
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
//instruction3op_xor_pair[6]
  {
    "xor_b1 $c1, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb1,
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
//instruction3op_xor_pair[7]
  {
    "xor_b32 $s1, WAVESIZE, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb32,
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
//instruction3op_xor_pair[8]
  {
    "xor_b64 $d1, WAVESIZE, 0x56789c;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigXor,
    Brigb64,
    BrigNoPacking,
   {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + wavesz_size),
      0, 
      0
      }
    }
  }
};

class Instruction3OpUnpacklo: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_unpacklo_pair[22] = {
//instruction3op_unpacklo_pair[0]
  {
    "unpacklo_s8x4 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs8x4,
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
//instruction3op_unpacklo_pair[1]
  {
    "unpacklo_u8x4 $s1, $s2, 72;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigu8x4,
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
//instruction3op_unpacklo_pair[2]
  {
    "unpacklo_s16x2 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs16x2,
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
//instruction3op_unpacklo_pair[3]
  {
    "unpacklo_u16x2 $s1, $s2, 72;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigu16x2,
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
//instruction3op_unpacklo_pair[4]
  {
    "unpacklo_f16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigUnpackLo,
    Brigf16x2,
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
//instruction3op_unpacklo_pair[5]
  {
    "unpacklo_s16x4 $d1, $d2, 72;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs16x4,
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
//instruction3op_unpacklo_pair[6]
  {
    "unpacklo_u16x4 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigu16x4,
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
//instruction3op_unpacklo_pair[7]
  {
    "unpacklo_f16x4 $d1, $d2, 72;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigf16x4,
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
//instruction3op_unpacklo_pair[8]
  {
    "unpacklo_s32x2 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs32x2,
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
//instruction3op_unpacklo_pair[9]
  {
    "unpacklo_u32x2 $d1, $d2, 72;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigu32x2,
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
//instruction3op_unpacklo_pair[10]
  {
    "unpacklo_f32x2 $d1, $d2, $d3;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigf32x2,
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
//instruction3op_unpacklo_pair[11]
  {
    "unpacklo_s8x4 $s1, 0x1703a, 72;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigs8x4,
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
//instruction3op_unpacklo_pair[12]
  {
    "unpacklo_u8x4 $s1, 0x1703a, $s3;",
     {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigu8x4,
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
//instruction3op_unpacklo_pair[13]
  {
    "unpacklo_s16x2 $s1, 0x1703a, 72;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs16x2,
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
//instruction3op_unpacklo_pair[14]
  {
    "unpacklo_u16x2 $s1, 0x1703a, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigu16x2,
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
//instruction3op_unpacklo_pair[15]
  {
    "unpacklo_f16x2 $s1, 1.0f, 72;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigf16x2,
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
//instruction3op_unpacklo_pair[16]
  {
    "unpacklo_s16x4 $d1, 0x1703a, $d3;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigs16x4,
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
//instruction3op_unpacklo_pair[17]
  {
    "unpacklo_u16x4 $d1, 0x1703a, 72;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigu16x4,
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
//instruction3op_unpacklo_pair[18]
  {
    "unpacklo_f16x4 $d1, 1.0f, $d3;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigf16x4,
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
//instruction3op_unpacklo_pair[19]
  {
    "unpacklo_s32x2 $d1, 0x1703a, 72;",
    {
    brig_inst_base_size,                         
    BrigEInstBase,
    BrigUnpackLo,
    Brigs32x2,
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
//instruction3op_unpacklo_pair[20]
  {
    "unpacklo_u32x2 $d1, 0x1703a, $d3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigu32x2,
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
//instruction3op_unpacklo_pair[21]
  {
    "unpacklo_f32x2 $d1, 1.0f, 72.0f;",
      {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigUnpackLo,
    Brigf32x2,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      0, 
      0
      }
    }
  }
};

class Instruction3OpUnpackhi: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_unpackhi_pair[22] = {
//instruction3op_unpackhi_pair[0]
  {
    "unpackhi_s8x4 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigUnpackHi,
    Brigs8x4,
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
//instruction3op_unpackhi_pair[1]
  {
    "unpackhi_u8x4 $s1, $s2, 72;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu8x4,
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
//instruction3op_unpackhi_pair[2]
  {
    "unpackhi_s16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs16x2,
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
//instruction3op_unpackhi_pair[3]
  {
    "unpackhi_u16x2 $s1, $s2, 72;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu16x2,
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
//instruction3op_unpackhi_pair[4]
  {
    "unpackhi_f16x2 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigf16x2,
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
//instruction3op_unpackhi_pair[5]
  {
    "unpackhi_s16x4 $d1, $d2, 72;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs16x4,
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
//instruction3op_unpackhi_pair[6]
  {
    "unpackhi_u16x4 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu16x4,
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
//instruction3op_unpackhi_pair[7]
  {
    "unpackhi_f16x4 $d1, $d2, 72;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigf16x4,
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
//instruction3op_unpackhi_pair[8]
  {
    "unpackhi_s32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs32x2,
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
//instruction3op_unpackhi_pair[9]
  {
    "unpackhi_u32x2 $d1, $d2, 72;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu32x2,
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
//instruction3op_unpackhi_pair[10]
  {
    "unpackhi_f32x2 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigUnpackHi,
    Brigf32x2,
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
//instruction3op_unpackhi_pair[11]
  {
    "unpackhi_s8x4 $s1, 0x1703a, 72;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs8x4,
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
//instruction3op_unpackhi_pair[12]
  {
    "unpackhi_u8x4 $s1, 0x1703a, $s3;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu8x4,
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
//instruction3op_unpackhi_pair[13]
  {
    "unpackhi_s16x2 $s1, 0x1703a, 72;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs16x2,
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
//instruction3op_unpackhi_pair[14]
  {
    "unpackhi_u16x2 $s1, 0x1703a, $s3;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu16x2,
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
//instruction3op_unpackhi_pair[15]
  {
    "unpackhi_f16x2 $s1, 1.0f, 72.0f;",
    {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigUnpackHi,
    Brigf16x2,
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
//instruction3op_unpackhi_pair[16]
  {
    "unpackhi_s16x4 $d1, 0x1703a, $d3;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs16x4,
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
//instruction3op_unpackhi_pair[17]
  {
    "unpackhi_u16x4 $d1, 0x1703a, 72;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu16x4,
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
//instruction3op_unpackhi_pair[18]
  {
    "unpackhi_f16x4 $d1, 1.0f, $d3;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigf16x4,
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
//instruction3op_unpackhi_pair[19]
  {
    "unpackhi_s32x2 $d1, 0x1703a, 72;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigs32x2,
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
//instruction3op_unpackhi_pair[20]
  {
    "unpackhi_u32x2 $d1, 0x1703a, $d3;",
     {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigu32x2,
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
//instruction3op_unpackhi_pair[21]
  {
    "unpackhi_f32x2 $d1, 1.0f, 72.0f;",
    {
    brig_inst_base_size,                               
    BrigEInstBase,
    BrigUnpackHi,
    Brigf32x2,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),  
      0, 
      0
      }
    }
  }
};

class Instruction3OpMovdlob64: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_movdlo_pair[4] = {
//instruction3op_movdlo_pair[0]
  {
    "movd_lo_b64 $d1, $d2, $s3;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdLo,
    Brigb64,
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
//instruction3op_movdlo_pair[1]
  {
    "movd_lo_b64 $d1, $d2, 0x1703a;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdLo,
    Brigb64,
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
//instruction3op_movdlo_pair[2]
  {
    "movd_lo_b64 $d1, 0x38002a, $s3;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdLo,
    Brigb64,
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
//instruction3op_movdlo_pair[3]
  {
    "movd_lo_b64 $d1, 0x38002a, 0x1703a;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdLo,
    Brigb64,
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
};

class Instruction3OpMovdhib64: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op__movdhi_pair[4] = {
//instruction3op__movdhi_pair[0]
  {
    "movd_hi_b64 $d1, $d2, $s3;",
     {
    brig_inst_base_size,                                
    BrigEInstBase,
    BrigMovdHi,
    Brigb64,
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
//instruction3op__movdhi_pair[1]
  {
    "movd_hi_b64 $d1, $d2, 0x1703a;",
     {
    brig_inst_base_size,                                
    BrigEInstBase,
    BrigMovdHi,
    Brigb64,
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
//instruction3op__movdhi_pair[2]
  {
    "movd_hi_b64 $d1, 0x38002a, $s3;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdHi,
    Brigb64,
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
//instruction3op__movdhi_pair[3]
  {
    "movd_hi_b64 $d1, 0x38002a, 0x1703a;",
     {
    brig_inst_base_size,                              
    BrigEInstBase,
    BrigMovdHi,
    Brigb64,
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
};

class Instruction3OpCopySignFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3_copysign_pair[12] = {
//src0:reg f16
//instruction3copysign_pair[0]
  {
    "copysign_f16 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf16,
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
//instruction3copysign_pair[1]
  {
    "copysign_f16 $s1, $s2, 7.2f;",
   {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf16,
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
//src0:reg f32
//instruction3copysign_pair[2]
  {
    "copysign_f32 $s1, $s2, $s3;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf32,
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
//instruction3copysign_pair[3]
  {
    "copysign_f32 $s1, $s2, 7.2f;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf32,
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
//src0:reg f64
//instruction3copysign_pair[4]
  {
    "copysign_f64 $d1, $d2, $d3;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf64,
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
//src0:reg f64
//instruction3copysign_pair[5]
  {
    "copysign_f64 $d1, $d2, 7.2;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf64,
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
//src0:immed value f16
//instruction3copysign_pair[6]
  {
    "copysign_f16 $s1, 1.0f, $s3;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf16,
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
//instruction3copysign_pair[7]
  {
    "copysign_f16 $s1, 1.0f, 7.2f;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf16,
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
//src0:immed value f32
//instruction3copysign_pair[8]
  {
    "copysign_f32 $s1, 1.0f, $s3;",
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigCopySign,
    Brigf32,
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
//instruction3copysign_pair[9]
 {
    "copysign_f32 $s1, 1.0f, 7.2f;",
    {
    brig_inst_base_size,                                  
    BrigEInstBase,
    BrigCopySign,
    Brigf32,
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
//src0:immed value f64
//instruction3copysign_pair[10]
  {
    "copysign_f64 $d1, 1.0, $d3;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf64,
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
//instruction3copysign_pair[11]
  {
    "copysign_f64 $d1, 1.0, 7.2;",
    {
    brig_inst_base_size,                                   
    BrigEInstBase,
    BrigCopySign,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  }
};

class Instruction3OpClassFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3_classfloat_pair[8] = {
//src0:reg f32
//instruction3classfloat_pair[0]
  {
    "class_f32 $c1, $s2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//instruction3classfloat_pair[1]
  {
    "class_f32 $c1, $s2, 0x004;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//src0:reg f64
//instruction3classfloat_pair[2]
  {
    "class_f64 $c1, $d2, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//instruction3classfloat_pair[3]
  {
    "class_f64 $c1, $d2, 0x020;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//src0:literal value f32
//instruction3classfloat_pair[4]
  {
    "class_f32 $c1, 10.1f, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//instruction3classfloat_pair[5]
  {
    "class_f32 $c1, 10.1f, 0x100;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//src0:literal value f64
//instruction3classfloat_pair[6]
  {
    "class_f64 $c1, 10.1, $s3;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
//instruction3classfloat_pair[7]
  {
    "class_f64 $c1, 10.1, 0x200;",
    {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigClass,
    Brigb1,
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
};

class Instruction3OpSend: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3_send_pair[4] = {
//src0:reg b32
//instruction3send_pair[0]
  {
    "send_b32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigSend,
    Brigb32,
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
//instruction3send_pair[1]
  {
    "send_b32 $s1, $s2, 3;",
     {
    brig_inst_base_size,                        
    BrigEInstBase,
    BrigSend,
    Brigb32,
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
//src0:literal value b32
//instruction3send_pair[2]
  {
    "send_b32 $s1, 0x100011, $s3;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigSend,
    Brigb32,
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
//instruction3send_pair[3]
  {
    "send_b32 $s1, 0x100011, 3;",
    {
    brig_inst_base_size,                       
    BrigEInstBase,
    BrigSend,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  }
};

class Instruction3OpReceive: public ::testing::TestWithParam<int>{
};

struct Instruction3Test instruction3op_receive_pair[4] = {
//src0:reg b32
//instruction3op_receive_pair[0]
  {
    "receive_b32 $s1, $s2, $s3;",
     {
    brig_inst_base_size,                           
    BrigEInstBase,
    BrigReceive,
    Brigb32,
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
//instruction3op_receive_pair[1]
  {
    "receive_b32 $s1, $s2, 3;",
     {
    brig_inst_base_size,                           
    BrigEInstBase,
    BrigReceive,
    Brigb32,
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
//src0:literal value b32
//instruction3op_receive_pair[2]
  {
    "receive_b32 $s1, 0x100011, $s3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigReceive,
    Brigb32,
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
//instruction3op_receive_pair[3]
  {
    "receive_b32 $s1, 0x100011, 3;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigReceive,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      }
    }
  }
};

struct Instruction3ModTest{
  const char* str;
  BrigInstMod ref;
};

class Instruction3OpAddFtzRoundFloat: public ::testing::TestWithParam<int>{
};

BrigAluModifier instruction3_bam[] = {
  {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved

  {1,1,1,0,0,0,0},//valid,float,zero,noftz,approx,fbar,reserved

  {1,1,2,0,0,0,0},//valid,float,up,noftz,approx,fbar,reserved

  {1,1,3,0,0,0,0},//valid,float,down,noftz,approx,fbar,reserved

  {1,1,0,1,0,0,0},//valid,float,near,ftz,approx,fbar,reserved

  {1,1,1,1,0,0,0},//valid,float,zero,ftz,approx,fbar,reserved

  {1,1,2,1,0,0,0},//valid,float,up,ftz,approx,fbar,reserved

  {1,1,3,1,0,0,0} //valid,float,down,ftz,approx,fbar,reserved
};

struct Instruction3ModTest instruction3_add_ftz_round_float_pair[8] = {
//instruction3_add_ftz_round_float_pair[0]
  {
    "add_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[1]
    }
  },
//instruction3_add_ftz_round_float_pair[1]
  {
    "add_up_f32 $s1, 0.5f, $s3;",
     {
    brig_inst_mod_size,                            
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[2]
    }
  },
//instruction3_add_ftz_round_float_pair[2]
  {
    "add_down_f64 $d1, 0.5, $d3;",
    {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[3]
    }
  },
//instruction3_add_ftz_round_float_pair[3]
  {
    "add_ftz_f16 $s1, $s2, 12.0f;",
    {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3_add_ftz_round_float_pair[4]
  {
    "add_ftz_near_f32 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3_add_ftz_round_float_pair[5]
  {
    "add_ftz_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[5]
    }
  },
//instruction3_add_ftz_round_float_pair[6]
  {
    "add_ftz_up_f32 $s1, $s2, 12.0f;",
    {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
    instruction3_bam[6]
    }
  },
//instruction3_add_ftz_round_float_pair[7]
  {
    "add_ftz_down_f64 $d1, $d2, $d3;",
     {
    brig_inst_mod_size,                          
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[7]
    }
  }
};



class Instruction3OpDivFtzRoundFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3ModTest instruction3op_div_ftz_round_float_pair[8] = {
//instruction3op_div_ftz_round_float_pair[0]
  {
    "div_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf16,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[1]
    }
  },
//instruction3op_div_ftz_round_float_pair[1]
  {
    "div_up_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[2]
    }
  },
//instruction3op_div_ftz_round_float_pair[2]
  {
    "div_down_f64 $d1, $d2, $d3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[3]
    }
  },
//instruction3op_div_ftz_round_float_pair[3]
  {
    "div_ftz_f16 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf16,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3op_div_ftz_round_float_pair[4]
  {
    "div_ftz_near_f32 $s1, 72.0f, 0.5f;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3op_div_ftz_round_float_pair[5]
  {
    "div_ftz_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf16,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[5]
    }
  },
//instruction3op_div_ftz_round_float_pair[6]
  {
    "div_ftz_up_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[6]
    }
  },
//instruction3op_div_ftz_round_float_pair[7]
  {
    "div_ftz_down_f64 $d1, $d2, $d3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigDiv,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[7]
    }
  }
};

class Instruction3OpSubFtzRoundFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3ModTest instruction3op_sub_ftz_round_float_pair[8] = {
//instruction3op_sub_ftz_round_float_pair[0]
  {
    "sub_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[1]
    }
  },
//instruction3op_sub_ftz_round_float_pair[1]
  {
    "sub_up_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[2]
    }
  },
//instruction3op_sub_ftz_round_float_pair[2]
  {
    "sub_down_f64 $d1, $d2, $d3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[3]
    }
  },
//instruction3op_sub_ftz_round_float_pair[3]
  {
    "sub_ftz_f32 $s1, 72.0f, $d3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf32,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3op_sub_ftz_round_float_pair[4]
  {
    "sub_ftz_near_f64 $d1, 72.0, 1.5;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
    instruction3_bam[4]
    }
  },
//instruction3op_sub_ftz_round_float_pair[5]
  {
    "sub_ftz_zero_f16 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[5]
    }
  },
//instruction3op_sub_ftz_round_float_pair[6]
  {
    "sub_ftz_up_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf32,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
    instruction3_bam[6]
    }
  },
//instruction3op_sub_ftz_round_float_pair[7]
  {
    "sub_ftz_down_f64 $d1, $d2, $d3;",
    {
    brig_inst_mod_size,                               
    BrigEInstMod,
    BrigSub,
    Brigf64,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
    instruction3_bam[7]
    }
  }
};

class Instruction3OpAddFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3ModTest instruction3_addfloat_pair[6] = {
//instruction3_addfloat_pair[0]
  {
    "add_near_f16 $s1, $s2, 0.5f;",
    {
    brig_inst_mod_size,	                                              
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3_addfloat_pair[1]
  {
    "add_near_f32 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,	                                                
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3_addfloat_pair[2]
  {
    "add_near_f64 $d1, $d2, 0.5;",
    {
    brig_inst_mod_size,	                                                
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3_addfloat_pair[3]
  {
    "add_near_f16 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,	                                                
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3_addfloat_pair[4]
  {
    "add_near_f32 $s1, 72.0f, $s3;",
    {
    brig_inst_mod_size,	                                                
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3_addfloat_pair[5]
  {
    "add_near_f64 $d1, 72.0, $d3;",
     {
    brig_inst_mod_size,	                                                
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
   {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  }
};
class Instruction3OpDivFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3ModTest instruction3op_div_float_pair[4] = {
//instruction3op_div_float_pair[0]
  {
    "div_near_f32 $s1, $s2, $s3;",
     {
    brig_inst_mod_size,                                     
    BrigEInstMod,
    BrigDiv,
    Brigf32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3op_div_float_pair[1]
  {
    "div_near_f64 $d1, $d2, 1.5;",
    {
    brig_inst_mod_size,                                    
    BrigEInstMod,
    BrigDiv,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3op_div_float_pair[2]
  {
    "div_near_f16 $s1, 72.0f, 1.5f;",
     {
    brig_inst_mod_size,                                   
    BrigEInstMod,
    BrigDiv,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3op_div_float_pair[3]
  {
    "div_near_f64 $d1, 72.0, $d3;",
     {
    brig_inst_mod_size,                                  
    BrigEInstMod,
    BrigDiv,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  }
};
class Instruction3OpSubFloat: public ::testing::TestWithParam<int>{
};

struct Instruction3ModTest instruction3op_sub_float_pair[3] = {
//instruction3op_sub_float_pair[0]
  {
    "sub_near_f32 $s1, $s2, $s3;",
    {
    brig_inst_mod_size,                                  
    BrigEInstMod,
    BrigSub,
    Brigf32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3op_sub_float_pair[1]
  {
    "sub_near_f16 $s1, 72.0f, 1.5f;",
     {
    brig_inst_mod_size,                                 
    BrigEInstMod,
    BrigSub,
    Brigf16,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size), 
      0, 
      0
      },
      instruction3_bam[0]
    }
  },
//instruction3op_sub_float_pair[2]
  {
    "sub_near_f64 $d1, 72.0, $d3;",
    {
    brig_inst_mod_size,                                  
    BrigEInstMod,
    BrigSub,
    Brigf64,
    BrigNoPacking,
    {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(operand_offset + reg_size) + immed_size, 
      0, 
      0
      },
      instruction3_bam[0]
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //INSTRUCTION3_TEST_H_
