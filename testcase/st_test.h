/* Copyright 2012 <MulticorewareInc> */

#ifndef ST_TEST_H_
#define ST_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class OperationSt : public ::testing::TestWithParam<int>{
};

struct StTest{
  const char* str;
  BrigInstLdSt ref;
};

struct StTest st_pair[30] = {
//st_pair[0]
  {
  "st_u8 $s1, [&x];", 
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[1]
  {
    "st_u16 $s1, [0x6e];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[2]
  {
    "st_u32 $s1, [%g][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[3]
  {
    "st_s8 $s1, [%g];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[4]
  {
    "st_s32 $s1, [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[5]
  {
    "st_u64 $d1, [%g][$d2 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[6]
  {
    "st_s64 $d1, [&x][$d2 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[7]
  {
    "st_f64 $d1, [&x][$d2 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[8]
  {
    "st_b128 $q1, [&x][$d2 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[9]
  {
    "st_global_u8 $s1, [&x];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
    }
  },
//st_pair[10]
  {
    "st_group_s8 $s1, [%g];", 
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace,
    BrigRegular,
    0
    }
  },
//st_pair[11]
  {
    "st_kernarg_u16 $s1, [0x6e];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace,
    BrigRegular,
    0
    }
  },
//st_pair[12]
  {
    "st_global_u32 $s1, [%g][4];", 
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
    }
  },
//st_pair[13]
  {
    "st_group_s32 $s1, [&array][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
    BrigGroupSpace,
    BrigRegular,
    0
    }
  },
//st_pair[14]
  {
    "st_readonly_u64 $d1, [%g][$d2 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigReadonlySpace,
    BrigRegular,
    0
    }
  },
//st_pair[15]
  {
    "st_spill_s64 $d1, [&x][$d2 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigSpillSpace,
    BrigRegular,
    0
    }
  },
//st_pair[16]
  {
    "st_arg_f64 $d1, [&x][$d2 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigArgSpace,
    BrigRegular,
    0
    }
  },
//st_pair[17]
  {
    "st_global_b128 $q1, [&x][$d2 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
    }
  },
//st_pair[18]
  {
    "st_s16 $s1, [$s2 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[19]
  {
    "st_f16 $s1, [$s2 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[20]
  {
    "st_f16 $d1, [$s2 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[21]
  {
    "st_f32 $d1, [%g][$s2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[22]
  {
    "st_private_s8 $s1, [$s2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigPrivateSpace,
    BrigRegular,
    0
    }
  },
//st_pair[23]
  {
    "st_readonly_s16 $s1, [$s2 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigReadonlySpace,
    BrigRegular,
    0
    }
  },
//st_pair[24]
  {
    "st_spill_f16 $s1, [$s2 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigSpillSpace,
    BrigRegular,
    0
    }
  },
//st_pair[25]
  {
    "st_arg_f16 $d1, [$s2 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigArgSpace,
    BrigRegular,
    0
    }
  },
//st_pair[26]
  {
    "st_private_f32 $s1, [%g][$s2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigPrivateSpace,
    BrigRegular,
    0
    }
  },
//st_pair[27]
  {
    "st_kernarg_f32 $d1, [%g][$s2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigKernargSpace,
    BrigRegular,
    0
    }
  },
//st_pair[28]
  {
    "st_s8 $s1, [$s2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//st_pair[29]
  {
    "st_f32 $s1, [%g][$s2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
};

class OperationStIm : public ::testing::TestWithParam<int>{
};

struct StTest stim_pair[30] = {
//stim_pair[0]
  {
    "st_rel_u8 1, [&x];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[1]
  {
    "st_part_rel_u16 1, [0x6e];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[2]
  {
    "st_part_rel_u32 1, [%g][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[3]
  {
    "st_part_rel_s8 1, [%g];", 
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[4]
  {
    "st_rel_s32 1, [&array][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[5]
  {
    "st_part_rel_u64 61, [%g][$d3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[6]
  {
    "st_rel_s64 61, [&x][$d3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
   {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[7]
  {
    "st_part_rel_f64 61, [&x][$d3 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
   {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[8]
  {
    "st_rel_b128 121, [&x][$d3 - 2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[9]
  {
    "st_equiv(0)_u8 1, [&x];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//stim_pair[10]
  {
    "st_equiv(1)_s8 1, [%g];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
    }
  },
//stim_pair[11]
  {
    "st_equiv(0)_u16 1, [0x6e];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//stim_pair[12]
  {
    "st_equiv(1)_u32 1, [%g][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
    }
  },
//stim_pair[13]
  {
    "st_equiv(2)_s32 1, [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    2
    }
  },
//stim_pair[14]
  {
    "st_equiv(2)_u64 61, [%g][$d3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    2
    }
  },
//stim_pair[15]
  {
    "st_equiv(0)_s64 61, [&x][$d3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//stim_pair[16]
  {
    "st_equiv(1)_f64 61, [&x][$d3 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
   {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
    }
  },
//stim_pair[17]
  {
    "st_equiv(2)_b128 121, [&x][$d3 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    2
    }
  },
//stim_pair[18]
 {
    "st_rel_f16 61, [$s3 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[19]
  {
    "st_part_rel_f16 1, [$s3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[20]
  {
    "st_rel_f32 61, [%g][$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[21]
  {
    "st_equiv(2)_s8 1, [$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    2
    }
  },
//stim_pair[22]
  {
    "st_equiv(1)_s16 1, [$s3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
    }
  },
//stim_pair[23]
  {
    "st_equiv(2)_f16 1, [$s3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    2
    }
  },
//stim_pair[24]
  {
    "st_equiv(0)_f16 61, [$s3 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//stim_pair[25]
  {
    "st_equiv(0)_f32 1, [%g][$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    0
    }
  },
//stim_pair[26]
  {
    "st_equiv(1)_f32 61, [%g][$s3];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRegular,
    1
    }
  },
//stim_pair[27]
  {
    "st_part_rel_f32 1, [%g][$s3];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + address_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stim_pair[28]
  {
    "st_rel_s16 1, [$s3 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stim_pair[29]
  {
    "st_rel_s8 1, [$s3];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size + reg_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  }
};

class OperationStV2 : public ::testing::TestWithParam<int>{
};

struct StTest stv2_pair[30] = {
//stv2_pair[0]
  {
    "st_v2_global_rel_u8 ($s1,$s2), [&x];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigGlobalSpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[1]
  {
    "st_v2_kernarg_part_rel_u16 ($s1,$s2), [0x6e];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigKernargSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[2]
  {
    "st_v2_global_part_rel_u32 ($s1,$s2), [%g][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[3]
  {
    "st_v2_global_part_rel_b128 ($q1,$q2), [&x][$d3 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[4]
  {
    "st_v2_group_part_rel_s8 ($s1,$s2), [%g];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigGroupSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[5]
  {
    "st_v2_group_rel_s32 ($s1,$s2), [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size +
       address_size, 0, 0, 0},
    BrigGroupSpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[6]
  {
    "st_v2_readonly_part_rel_u64 ($d1,$d2), [%g][$d3 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigReadonlySpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[7]
  {
    "st_v2_spill_rel_s64 ($d1,$d2), [&x][$d3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigSpillSpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[8]
  {
    "st_v2_arg_part_rel_f64 ($d1,$d2), [&x][$d3 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigArgSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[9]
  {
    "st_v2_global_equiv(0)_u8 ($s1,$s2), [&x];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    0
    }
  },
//stv2_pair[10]
  {
    "st_v2_group_equiv(1)_s8 ($s1,$s2), [%g];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigGroupSpace,
    BrigRegular,
    1
    }
  },
//stv2_pair[11]
  {
    "st_v2_kernarg_equiv(0)_u16 ($s1,$s2), [0x6e];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size,
       0, 0, 0},
    BrigKernargSpace,
    BrigRegular,
    0
    }
  },
//stv2_pair[12]
  {
    "st_v2_global_equiv(1)_u32 ($s1,$s2), [%g][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    1
    }
  },
//stv2_pair[13]
  {
    "st_v2_group_equiv(2)_s32 ($s1,$s2), [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 2 + regv2_size +
       address_size, 0, 0, 0},
    BrigGroupSpace,
    BrigRegular,
    2
    }
  },
//stv2_pair[14]
  {
    "st_v2_readonly_equiv(2)_u64 ($d1,$d2), [%g][$d3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigReadonlySpace,
    BrigRegular,
    2
    }
  },
//stv2_pair[15]
  {
    "st_v2_spill_equiv(0)_s64 ($d1,$d2), [&x][$d3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigSpillSpace,
    BrigRegular,
    0
    }
  },
//stv2_pair[16]
  {
    "st_v2_arg_equiv(1)_f64 ($d1,$d2), [&x][$d3 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigArgSpace,
    BrigRegular,
    1
    }
  },
//stv2_pair[17]
  {
    "st_v2_global_equiv(2)_b128 ($q1,$q2), [&x][$d3 - 2];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRegular,
    2
    }
  },
//stv2_pair[18]
  {
    "st_v2_readonly_rel_s16 ($s1,$s2), [$s3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigReadonlySpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[19]
  {
    "st_v2_spill_part_rel_f16 ($s1,$s2), [$s3 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigSpillSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[20]
  {
    "st_v2_arg_rel_f16 ($d1,$d2), [$s3 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigArgSpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[21]
  {
    "st_v2_kernarg_rel_f32 ($d1,$d2), [%g][$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigKernargSpace,
    BrigRelease,
    0
    }
  },
//stv2_pair[22]
  {
    "st_v2_private_equiv(2)_s8 ($s1,$s2), [$s3];", 
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigPrivateSpace,
    BrigRegular,
    2
    }
  },
//stv2_pair[23]
  {
    "st_v2_readonly_equiv(1)_s16 ($s1,$s2), [$s3 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigReadonlySpace,
    BrigRegular,
    1
    }
  },
//stv2_pair[24]
  {
    "st_v2_spill_equiv(2)_f16 ($s1,$s2), [$s3 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigSpillSpace,
    BrigRegular,
    2
    }
  },
//stv2_pair[25]
  {
    "st_v2_arg_equiv(0)_f16 ($d1,$d2), [$s3 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigArgSpace,
    BrigRegular,
    0
    }
  },
//stv2_pair[26]
  {
    "st_v2_private_equiv(0)_f32 ($s1,$s2), [%g][$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigPrivateSpace,
    BrigRegular,
    0
    }
  },
//stv2_pair[27]
  {
    "st_v2_kernarg_equiv(1)_f32 ($d1,$d2), [%g][$s3];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigKernargSpace,
    BrigRegular,
    1
    }
  },
//stv2_pair[28]
  {
    "st_v2_private_part_rel_f32 ($s1,$s2), [%g][$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
   {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size +
       address_size, 0, 0, 0},
    BrigPrivateSpace,
    BrigParRelease,
    0
    }
  },
//stv2_pair[29]
  {
    "st_v2_private_rel_s8 ($s1,$s2), [$s3];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset + reg_size * 2, operand_offset + reg_size * 3 + regv2_size,
       0, 0, 0},
    BrigPrivateSpace,
    BrigRelease,
    0
    }
  }
};

class OperationStV4 : public ::testing::TestWithParam<int>{
};

struct StTest stv4_pair[30] = {
//stv4_pair[0]
  {
    "st_v4_rel_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[1]
  {
    "st_v4_part_rel_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stv4_pair[2]
  {
    "st_v4_part_rel_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[3]
  {
    "st_v4_part_rel_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[4]
  {
    "st_v4_rel_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[5]
  {
    "st_v4_part_rel_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    2
    }
  },
//stv4_pair[6]
  {
    "st_v4_rel_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[7]
  {
    "st_v4_part_rel_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[8]
  {
    "st_v4_rel_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$d5 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[9]
  {
    "st_v4_global_rel_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];", 
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu8,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigGlobalSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[10]
  {
    "st_v4_group_part_rel_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigGroupSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[11]
  {
    "st_v4_kernarg_part_rel_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size,
       0, 0, 0},
    BrigKernargSpace,
    BrigParRelease,
    0
    }
  },
//stv4_pair[12]
  {
    "st_v4_global_part_rel_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu32,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[13] 
  {
    "st_v4_group_rel_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs32,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 4 + regv4_size +
       address_size, 0, 0, 0},
    BrigGroupSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[14]
  {
    "st_v4_readonly_part_rel_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigu64,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigReadonlySpace,
    BrigParRelease,
    2
    }
  },
//stv4_pair[15]
  {
    "st_v4_spill_rel_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs64,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigSpillSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[16]
  {
    "st_v4_arg_part_rel_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];", 
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf64,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigArgSpace,
    BrigParRelease,
    1
    }
  },
//stv4_pair[17]
  {
    "st_v4_global_rel_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$d5 - 2];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigb128,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigGlobalSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[18]
  {
    "st_v4_part_rel_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    2
    }
  },
//stv4_pair[19]
  {
    "st_v4_rel_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[20]
  {
    "st_v4_part_rel_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigParRelease,
    0
    }
  },
//stv4_pair[21]
  {
    "st_v4_rel_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    1
    }
  },
//stv4_pair[22]
  {
    "st_v4_private_rel_equiv(2)_s8 ($s1,$s2,$s3,$s4), [$s5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigPrivateSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[23]
  {
    "st_v4_readonly_rel_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigReadonlySpace,
    BrigRelease,
    1
    }
  },
//stv4_pair[24]
  {
    "st_v4_spill_part_rel_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigSpillSpace,
    BrigParRelease,
    2
    }
  },
//stv4_pair[25]
  {
    "st_v4_arg_rel_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigArgSpace,
    BrigRelease,
    0
    }
  },
//stv4_pair[26] 
  {
    "st_v4_private_part_rel_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigPrivateSpace,
    BrigParRelease,
    0
    }
  },
//stv4_pair[27]
  {
    "st_v4_kernarg_rel_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigf32,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size +
       address_size, 0, 0, 0},
    BrigKernargSpace,
    BrigRelease,
    1
    }
  }, 
//stv4_pair[28]
  {
    "st_v4_rel_equiv(2)_s8 ($s1,$s2,$s3,$s4), [$s5];",
    {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs8,
    BrigNoPacking,
   {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    2
    }
  },
//stv4_pair[29]
  {
    "st_v4_rel_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
     {
    brig_inst_ldst_size,
    BrigEInstLdSt,
    BrigSt,
    Brigs16,
    BrigNoPacking,
    {operand_offset + reg_size * 4, operand_offset + reg_size * 5 + regv4_size,
       0, 0, 0},
    BrigFlatSpace,
    BrigRelease,
    1
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //ST_TEST_H_
