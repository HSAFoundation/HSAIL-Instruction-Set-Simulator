/* Copyright 2012 <MulticorewareInc> */

#ifndef LD_TEST_H_
#define LD_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {
  class OperationLd : public ::testing::TestWithParam<int>{
  };

  struct LdTest{
    const char* str;
    BrigInstLdSt ref;
  };

  struct LdTest ld_pair[80] = {
    //ld_pair[0]
    {
      "ld_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[1]
    {
      "ld_u16 $s1, [0x6e];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[2]
    {
      "ld_u32 $s1, [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[3]
    {
      "ld_u64 $d1, [%g][$d2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[4]
    {
      "ld_s64 $d1, [&x][$d2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[5]
    {
      "ld_f64 $d1, [&x][$d2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[6]
    {
      "ld_s8 $s1, [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[7]
    {
      "ld_s32 $s1, [&array][4];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[8]
    {
      "ld_width(1)_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[9]
    {
      "ld_width(2)_s8 $s1, [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[10]
    {
      "ld_width(8)_u16 $s1, [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[11]
    {
      "ld_width(128)_u32 $s1, [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[12]
    {
      "ld_width(256)_s32 $s1, [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[13]
    {
      "ld_width(all)_u64 $d1, [%g][$d2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[14]
    {
      "ld_width(1)_s64 $d1, [&x][$d2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[15]
    {
      "ld_width(2)_f64 $d1, [&x][$d2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[16]
    {
      "ld_global_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[17]
    {
      "ld_group_s8 $s1, [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[18]
    {
      "ld_kernarg_u16 $s1, [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[19]
    {
      "ld_global_u32 $s1, [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[20]
    {
      "ld_group_s32 $s1, [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[21]
    {
      "ld_readonly_u64 $d1, [%g][$d2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[22]
    {
      "ld_spill_s64 $d1, [&x][$d2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[23]
    {
      "ld_arg_f64 $d1, [&x][$d2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[24]
    {
      "ld_acq_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ld_pair[25]
    {
      "ld_s8 $s1, [%g];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //ld_pair[26]
    {
      "ld_acq_u16 $s1, [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ld_pair[27]
    {
      "ld_u32 $s1, [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //ld_pair[28]
    {
      "ld_part_acq_s32 $s1, [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //ld_pair[29]
    {
      "ld_part_acq_u64 $d1, [%g][$d2 + 10];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //ld_pair[30]
    {
      "ld_acq_s64 $d1, [&x][$d2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ld_pair[31]
    {
      "ld_f64 $d1, [&x][$d2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //ld_pair[32]
    {
      "ld_equiv(0)_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[33]
    {
      "ld_equiv(1)_s8 $s1, [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //ld_pair[34]
    {
      "ld_equiv(0)_u16 $s1, [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[35]
    {
      "ld_equiv(1)_u32 $s1, [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //ld_pair[36]
    {
      "ld_equiv(2)_s32 $s1, [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },

    //ld_pair[37]
    {
      "ld_equiv(2)_u64 $d1, [%g][$d2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //ld_pair[38]
    {
      "ld_equiv(0)_s64 $d1, [&x][$d2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[39]
    {
      "ld_equiv(1)_f64 $d1, [&x][$d2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //ld_pair[40]
    {
      "ld_width(1)_global_u8 $s1, [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[41]
    {
      "ld_width(2)_group_s8 $s1, [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[42]
    {
      "ld_width(8)_kernarg_u16 $s1, [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[43]
    {
      "ld_width(16)_readonly_s16 $s1, [$s2 + 10];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        0
      }
    },

    //ld_pair[44]
    {
      "ld_f16 $s1, [$s2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[45]
    {
      "ld_f16 $s1, [$s2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[46]
    {
      "ld_f32 $s1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[47]
    {
      "ld_f32 $d1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[48]
    {
      "ld_b128 $q1, [&x][$s2 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[49]
    {
      "ld_width(4)_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[50]
    {
      "ld_width(16)_s16 $s1, [$s2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[51]
    {
      "ld_width(32)_f16 $s1, [$s2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[52]
    {
      "ld_width(64)_f16 $d1, [$s2 + 1];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[53]
    {
      "ld_width(512)_f32 $s1, [%g][$s2];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[54]
    {
      "ld_width(1024)_f32 $d1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[55]
    {
      "ld_width(4)_b128 $q1, [&x][$s2 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[56]
    {
      "ld_private_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[57]
    {
      "ld_readonly_s16 $s1, [$s2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[58]
    {
      "ld_spill_f16 $s1, [$s2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[59]
    {
      "ld_arg_f16 $d1, [$s2 + 1];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[60]
    {
      "ld_private_f32 $s1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[61]
    {
      "ld_kernarg_f32 $d1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[62]
    {
      "ld_global_b128 $q1, [&x][$s2 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[63]
    {
      "ld_part_acq_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //ld_pair[64]
    {
      "ld_s16 $s1, [$s2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //ld_pair[65]
    {
      "ld_part_acq_f16 $s1, [$s2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //ld_pair[66]
    {
      "ld_acq_f16 $d1, [$s2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ld_pair[67]
    {
      "ld_acq_f32 $s1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ld_pair[68]
    {
      "ld_f32 $d1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //ld_pair[69]
    {
      "ld_part_acq_b128 $q1, [&x][$s2 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //ld_pair[70]
    {
      "ld_equiv(2)_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //ld_pair[71]
    {
      "ld_equiv(1)_s16 $s1, [$s2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //ld_pair[72]
    {
      "ld_equiv(2)_f16 $s1, [$s2 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //ld_pair[73]
    {
      "ld_equiv(0)_f16 $d1, [$s2 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[74]
    {
      "ld_equiv(0)_f32 $s1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[75]
    {
      "ld_equiv(1)_f32 $d1, [%g][$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //ld_pair[76]
    {
      "ld_equiv(2)_b128 $q1, [&x][$s2 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2 + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //ld_pair[77]
    {
      "ld_width(4)_private_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[78]
    {
      "ld_u8 $s1, [$s2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //ld_pair[79]
    {
      "ld_s16 $s1, [$s2 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size, 
          operand_offset + immed_size + reg_size * 2, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
  };

  class OperationLdV2 : public ::testing::TestWithParam<int>{
  };

  struct LdTest ldv2_pair[80] = {
    // ldv2_pair[0]
    {
      "ld_width(128)_v2_global_u32 ($s1,$s2), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[1]
    {
      "ld_width(2)_v2_arg_f64 ($d1,$d2), [&x][$d3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[2]
    {
      "ld_width(1)_v2_acq_u8 ($s1,$s2), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    // ldv2_pair[3]
    {
      "ld_width(8)_v2_acq_u16 ($s1,$s2), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },

    // ldv2_pair[4]
    {
      "ld_width(256)_v2_group_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        0
      }
    },

    // ldv2_pair[5]
    {
      "ld_width(all)_v2_readonly_u64 ($d1,$d2), [%g][$d3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[6]
    {
      "ld_width(1)_v2_spill_s64 ($d1,$d2), [&x][$d3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[7]
    {
      "ld_width(2)_v2_s8 ($s1,$s2), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    // ldv2_pair[8]
    {
      "ld_width(128)_v2_u32 ($s1,$s2), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    // ldv2_pair[9]
    {
      "ld_width(256)_v2_part_acq_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    // ldv2_pair[10]
    {
      "ld_width(all)_v2_part_acq_u64 ($d1,$d2), [%g][$d3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    // ldv2_pair[11]
    {
      "ld_width(1)_v2_acq_s64 ($d1,$d2), [&x][$d3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    // ldv2_pair[12]
    {
      "ld_width(2)_v2_f64 ($d1,$d2), [&x][$d3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    // ldv2_pair[13]
    {
      "ld_width(1)_v2_equiv(0)_u8 ($s1,$s2), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[14]
    {
      "ld_width(2)_v2_equiv(1)_s8 ($s1,$s2), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[15]
    {
      "ld_width(8)_v2_equiv(0)_u16 ($s1,$s2), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[16]
    {
      "ld_width(128)_v2_equiv(1)_u32 ($s1,$s2), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[17]
    {
      "ld_width(256)_v2_equiv(2)_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    // ldv2_pair[18]
    {
      "ld_width(all)_v2_equiv(2)_u64 ($d1,$d2), [%g][$d3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    // ldv2_pair[19]
    {
      "ld_width(1)_v2_equiv(0)_s64 ($d1,$d2), [&x][$d3 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[20]
    {
      "ld_width(2)_v2_equiv(1)_f64 ($d1,$d2), [&x][$d3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[21]
    {
      "ld_v2_global_acq_u8 ($s1,$s2), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigAcquire,
        0
      }
    },
    // ldv2_pair[22]
    {
      "ld_v2_group_s8 ($s1,$s2), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigGroupSpace,
        0,
        0
      }
    },
    // ldv2_pair[23]
    {
      "ld_v2_kernarg_acq_u16 ($s1,$s2), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigAcquire,
        0
      }
    },
    // ldv2_pair[24]
    {
      "ld_v2_global_u32 ($s1,$s2), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        0,
        0
      }
    },
    // ldv2_pair[25]
    {
      "ld_v2_group_part_acq_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigParAcquire,
        0
      }
    },
    // ldv2_pair[26]
    {
      "ld_v2_readonly_part_acq_u64 ($d1,$d2), [%g][$d3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigParAcquire,
        0
      }
    },
    // ldv2_pair[27]
    {
      "ld_v2_spill_acq_s64 ($d1,$d2), [&x][$d3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigAcquire,
        0
      }
    },
    // ldv2_pair[28]
    {
      "ld_v2_arg_f64 ($d1,$d2), [&x][$d3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        0,
        0
      }
    },
    // ldv2_pair[29]
    {
      "ld_v2_global_equiv(0)_u8 ($s1,$s2), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[30]
    {
      "ld_v2_group_equiv(1)_s8 ($s1,$s2), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[31]
    {
      "ld_v2_kernarg_equiv(0)_u16 ($s1,$s2), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[32]
    {
      "ld_v2_global_equiv(1)_u32 ($s1,$s2), [%g][4];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[33]
    {
      "ld_v2_group_equiv(2)_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        2
      }
    },
    // ldv2_pair[34]
    {
      "ld_v2_readonly_equiv(2)_u64 ($d1,$d2), [%g][$d3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        2
      }
    },
    // ldv2_pair[35]
    {
      "ld_v2_spill_equiv(0)_s64 ($d1,$d2), [&x][$d3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    },
    // ldv2_pair[36]
    {
      "ld_v2_arg_equiv(1)_f64 ($d1,$d2), [&x][$d3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        1
      }
    },
    // ldv2_pair[37]
    {
      "ld_v2_acq_equiv(0)_u8 ($s1,$s2), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[38]
    {
      "ld_v2_equiv(1)_s8 ($s1,$s2), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    // ldv2_pair[39]
    {
      "ld_v2_acq_equiv(0)_u16 ($s1,$s2), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[40]
    {
      "ld_v2_equiv(1)_u32 ($s1,$s2), [%g][4];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //LdTest ldv2_pair[41]
    {
      "ld_v2_part_acq_equiv(2)_s32 ($s1,$s2), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 2 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //LdTest ldv2_pair[42]
    {
      "ld_v2_acq_equiv(0)_f32 ($s1,$s2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[43]
    {
      "ld_width(64)_v2_arg_f16 ($d1,$d2), [$s3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[44]
    {
      "ld_width(512)_v2_private_f32 ($s1,$s2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[45]
    {
      "ld_width(1024)_v2_kernarg_f32 ($d1,$d2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[46]
    {
      "ld_width(4)_v2_global_b128 ($q1,$q2), [&x][$s3 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[47]
    {
      "ld_width(4)_v2_part_acq_u8 ($s1,$s2), [$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[48]
    {
      "ld_width(32)_v2_part_acq_f16 ($s1,$s2), [$s3 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[49]
    {
      "ld_width(64)_v2_acq_f16 ($d1,$d2), [$s3 + 1];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[50]
    {
      "ld_width(512)_v2_acq_f32 ($s1,$s2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[51]
    {
      "ld_width(1024)_v2_f32 ($d1,$d2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    //LdTest ldv2_pair[52]
    {
      "ld_width(4)_v2_part_acq_b128 ($q1,$q2), [&x][$s3 - 2];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[53]
    {
      "ld_width(4)_v2_equiv(2)_u8 ($s1,$s2), [$s3];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[54]
    {
      "ld_width(16)_v2_equiv(1)_s16 ($s1,$s2), [$s3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //LdTest ldv2_pair[55]
    {
      "ld_width(32)_v2_equiv(2)_f16 ($s1,$s2), [$s3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[56]
    {
      "ld_width(64)_v2_equiv(0)_f16 ($d1,$d2), [$s3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[57]
    {
      "ld_width(512)_v2_equiv(0)_f32 ($s1,$s2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[58]
    {
      "ld_width(1024)_v2_equiv(1)_f32 ($d1,$d2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        1
      }
    },
    //LdTest ldv2_pair[59]
    {
      "ld_width(4)_v2_equiv(2)_b128 ($q1,$q2), [&x][$s3 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[60]
    {
      "ld_v2_private_part_acq_u8 ($s1,$s2), [$s3];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[61]
    {
      "ld_v2_readonly_s16 ($s1,$s2), [$s3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        0,
        0
      }
    },
    //LdTest ldv2_pair[62]
    {
      "ld_v2_spill_part_acq_f16 ($s1,$s2), [$s3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[63]
    {
      "ld_v2_arg_acq_f16 ($d1,$d2), [$s3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[64]
    {
      "ld_v2_private_acq_f32 ($s1,$s2), [%g][$s3];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[65]
    {
      "ld_v2_kernarg_f32 ($d1,$d2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        0,
        0
      }
    },
    //LdTest ldv2_pair[66]
    {
      "ld_v2_global_part_acq_b128 ($q1,$q2), [&x][$s3 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigParAcquire,
        0
      }
    },
    //LdTest ldv2_pair[67]
    {
      "ld_v2_private_equiv(2)_u8 ($s1,$s2), [$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[68]
    {
      "ld_v2_readonly_equiv(1)_s16 ($s1,$s2), [$s3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        1
      }
    },
    //LdTest ldv2_pair[69]
    {
      "ld_v2_spill_equiv(2)_f16 ($s1,$s2), [$s3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[70]
    {
      "ld_v2_arg_equiv(0)_f16 ($d1,$d2), [$s3 + 1];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[71]
    {
      "ld_v2_private_equiv(0)_f32 ($s1,$s2), [%g][$s3];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //LdTest ldv2_pair[72]
    {
      "ld_v2_kernarg_equiv(1)_f32 ($d1,$d2), [%g][$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        1
      }
    },
    //LdTest ldv2_pair[73]
    {
      "ld_v2_global_equiv(2)_b128 ($q1,$q2), [&x][$s3 - 2];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        2
      }
    },
    //LdTest ldv2_pair[74]
    {
      "ld_v2_part_acq_equiv(2)_u8 ($s1,$s2), [$s3];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //LdTest ldv2_pair[75]
    {
      "ld_v2_equiv(1)_s16 ($s1,$s2), [$s3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //LdTest ldv2_pair[76]
    {
      "ld_v2_part_acq_equiv(2)_f16 ($s1,$s2), [$s3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //LdTest ldv2_pair[77]
    {
      "ld_v2_acq_equiv(0)_f16 ($d1,$d2), [$s3 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //LdTest ldv2_pair[78]
    {
      "ld_width(16)_v2_s16 ($s1,$s2), [$s3 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        0
      }
    },
    // LdTest ldv2_pair[79]
    {
      "ld_width(32)_v2_spill_f16 ($s1,$s2), [$s3 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 2, 
          operand_offset + immed_size + reg_size * 3 + regv2_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    }
  };

  class OperationLdV4 : public ::testing::TestWithParam<int>{
  };

  struct LdTest ldv4_pair[80] = {
    //ldv4_pair[0]
    {
      "ld_v4_part_acq_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[1]
    {
      "ld_v4_acq_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[2]
    {
      "ld_v4_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[3]
    {
      "ld_width(8)_v4_kernarg_acq_u16 ($s1,$s2,$s3,$s4), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[4]
    {
      "ld_width(128)_v4_global_u32 ($s1,$s2,$s3,$s4), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        0,
        0
      }
    },
    //ldv4_pair[5]
    {
      "ld_width(all)_v4_readonly_part_acq_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigParAcquire,
        0
      }
    },
    //ldv4_pair[6]
    {
      "ld_width(1)_v4_global_acq_u8 ($s1,$s2,$s3,$s4), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[7]
    {
      "ld_width(2)_v4_group_s8 ($s1,$s2,$s3,$s4), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGroupSpace,
        0,
        0
      }
    },
    //ldv4_pair[8]
    {
      "ld_width(256)_v4_group_part_acq_s32 ($s1,$s2,$s3,$s4), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigParAcquire,
        0
      }
    },
    //ldv4_pair[9]
    {
      "ld_width(1)_v4_spill_acq_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[10]
    {
      "ld_width(2)_v4_arg_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        0,
        0
      }
    },
    //ldv4_pair[11]
    {
      "ld_width(1)_v4_global_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        0
      }
    },
    //ldv4_pair[12]
    {
      "ld_width(2)_v4_group_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        1
      }
    },
    //ldv4_pair[13]
    {
      "ld_width(8)_v4_kernarg_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        0
      }
    },
    //ldv4_pair[14]
    {
      "ld_width(128)_v4_global_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        1
      }
    },
    //ldv4_pair[15]
    {
      "ld_width(256)_v4_group_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigRegular,
        2
      }
    },
    //ldv4_pair[16]
    {
      "ld_width(all)_v4_readonly_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        2
      }
    },
    //ldv4_pair[17]
    {
      "ld_width(1)_v4_spill_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        0
      }
    },
    //ldv4_pair[18]
    {
      "ld_width(2)_v4_arg_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        1
      }
    },
    //ldv4_pair[19]
    {
      "ld_width(1)_v4_acq_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[20]
    {
      "ld_width(2)_v4_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[21]
    {
      "ld_width(8)_v4_acq_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[22]
    {
      "ld_width(128)_v4_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[23]
    {
      "ld_width(256)_v4_part_acq_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[24]
    {
      "ld_width(all)_v4_part_acq_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[25]
    {
      "ld_width(1)_v4_acq_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[26]
    {
      "ld_width(2)_v4_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[27]
    {
      "ld_v4_global_acq_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[28]
    {
      "ld_v4_group_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigGroupSpace,
        0,
        1
      }
    },
    //ldv4_pair[29]
    {
      "ld_v4_kernarg_acq_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[30]
    {
      "ld_v4_global_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        0,
        1
      }
    },
    //ldv4_pair[31]
    {
      "ld_v4_group_part_acq_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGroupSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[32]
    {
      "ld_v4_readonly_part_acq_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[33]
    {
      "ld_v4_spill_acq_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[34]
    {
      "ld_v4_arg_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigArgSpace,
        0,
        1
      }
    },
    //ldv4_pair[35]
    {
      "ld_width(1)_v4_global_acq_equiv(0)_u8 ($s1,$s2,$s3,$s4), [&x];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size , 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[36]
    {
      "ld_width(2)_v4_group_equiv(1)_s8 ($s1,$s2,$s3,$s4), [%g];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size , 
          0, 
          0
        },
        BrigGroupSpace,
        0,
        1
      }
    },
    //ldv4_pair[37]
    {
      "ld_width(8)_v4_kernarg_acq_equiv(0)_u16 ($s1,$s2,$s3,$s4), [0x6e];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size , 
          0, 
          0
        },
        BrigKernargSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[38]
    {
      "ld_width(128)_v4_global_equiv(1)_u32 ($s1,$s2,$s3,$s4), [%g][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size,
          0, 
          0
        },
        BrigGlobalSpace,
        0,
        1
      }
    },
    //ldv4_pair[39]
    {
      "ld_width(256)_v4_group_part_acq_equiv(2)_s32 ($s1,$s2,$s3,$s4), [&array][4];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 4 + regv4_size + address_size,
          0, 
          0
        },
        BrigGroupSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[40]
    {
      "ld_width(all)_v4_readonly_part_acq_equiv(2)_u64 ($d1,$d2,$d3,$d4), [%g][$d5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigReadonlySpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[41]
    {
      "ld_width(1)_v4_spill_acq_equiv(0)_s64 ($d1,$d2,$d3,$d4), [&x][$d5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigSpillSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[42]
    {
      "ld_width(2)_v4_arg_equiv(1)_f64 ($d1,$d2,$d3,$d4), [&x][$d5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf64,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigArgSpace,
        0,
        1
      }
    },
    //ldv4_pair[43]
    {
      "ld_v4_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[44]
    {
      "ld_v4_spill_part_acq_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[45]
    {
      "ld_v4_arg_acq_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[46]
    {
      "ld_v4_part_acq_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$s5 - 2];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[47]
    {
      "ld_width(4)_v4_private_part_acq_equiv(2)_u8 ($s1,$s2,$s3,$s4), [$s5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size , 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[48]
    {
      "ld_v4_readonly_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        0,
        1
      }
    },

    //ldv4_pair[49]
    {
      "ld_width(512)_v4_acq_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[50]
    {
      "ld_width(1024)_v4_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[51]
    {
      "ld_width(16)_v4_readonly_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size , 
          0, 
          0
        },
        BrigReadonlySpace,
        0,
        1
      }
    },
    //ldv4_pair[52]
    {
      "ld_width(16)_v4_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        0,
        1
      }
    },
    //ldv4_pair[53]
    {
      "ld_width(32)_v4_part_acq_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[54]
    {
      "ld_v4_private_part_acq_equiv(2)_u8 ($s1,$s2,$s3,$s4), [$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[55]
    {
      "ld_width(64)_v4_acq_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[56]
    {
      "ld_width(32)_v4_spill_part_acq_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size , 
          0, 
          0
        },
        BrigSpillSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[57]
    {
      "ld_width(64)_v4_arg_acq_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size , 
          0, 
          0
        },
        BrigArgSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[58]
    {
      "ld_width(4)_v4_private_part_acq_u8 ($s1,$s2,$s3,$s4), [$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigParAcquire,
        0
      }
    },
    //ldv4_pair[59]
    {
      "ld_width(16)_v4_readonly_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        0,
        0
      }
    },
    //ldv4_pair[60]
    {
      "ld_v4_private_acq_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[61]
    {
      "ld_v4_kernarg_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        0,
        1
      }
    },
    //ldv4_pair[62]
    {
      "ld_width(32)_v4_spill_part_acq_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigParAcquire,
        0
      }
    },
    //ldv4_pair[63]
    {
      "ld_width(64)_v4_arg_acq_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[64]
    {
      "ld_width(512)_v4_private_acq_f32 ($s1,$s2,$s3,$s4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[65]
    {
      "ld_width(1024)_v4_kernarg_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        0,
        0
      }
    },
    //ldv4_pair[66]
    {
      "ld_width(4)_v4_global_part_acq_b128 ($q1,$q2,$q3,$q4), [&x][$s5 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigParAcquire,
        0
      }
    },
    //ldv4_pair[67]
    {
      "ld_width(4)_v4_private_equiv(2)_u8 ($s1,$s2,$s3,$s4), [$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        2
      }
    },
    //ldv4_pair[68]
    {
      "ld_width(16)_v4_readonly_equiv(1)_s16 ($s1,$s2,$s3,$s4), [$s5 + 10];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigs16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigReadonlySpace,
        BrigRegular,
        1
      }
    },
    //ldv4_pair[69]
    {
      "ld_width(32)_v4_spill_equiv(2)_f16 ($s1,$s2,$s3,$s4), [$s5 - 5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigSpillSpace,
        BrigRegular,
        2
      } 
    },
    //ldv4_pair[70]
    {
      "ld_width(64)_v4_arg_equiv(0)_f16 ($d1,$d2,$d3,$d4), [$s5 + 1];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf16,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigArgSpace,
        BrigRegular,
        0
      }
    },
    //ldv4_pair[71]
    {
      "ld_width(512)_v4_private_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigPrivateSpace,
        BrigRegular,
        0
      }
    },
    //ldv4_pair[72]
    {
      "ld_width(1024)_v4_kernarg_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];", 
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigKernargSpace,
        BrigRegular,
        1
      }
    },
    //ldv4_pair[73]
    {
      "ld_width(4)_v4_global_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$s5 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigRegular,
        2
      }
    },
    //ldv4_pair[74]
    {
      "ld_width(4)_v4_global_part_acq_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$s5 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigGlobalSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[75]
    {
      "ld_width(512)_v4_private_acq_equiv(0)_f32 ($s1,$s2,$s3,$s4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigPrivateSpace,
        BrigAcquire,
        0
      }
    },
    //ldv4_pair[76]
    {
      "ld_width(1024)_v4_kernarg_equiv(1)_f32 ($d1,$d2,$d3,$d4), [%g][$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigf32,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size,
          0, 
          0
        },
        BrigKernargSpace,
        0,
        1
      }
    },
    //ldv4_pair[77]
    {
      "ld_v4_global_part_acq_equiv(2)_b128 ($q1,$q2,$q3,$q4), [&x][$s5 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigGlobalSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[78]
    {
      "ld_width(4)_v4_part_acq_equiv(2)_u8 ($s1,$s2,$s3,$s4), [$s5];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigu8,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    },
    //ldv4_pair[79]
    //supports 32 128-bit registers
    {
      "ld_width(4)_v4_part_acq_equiv(2)_b128 ($q9,$q2,$q3,$q4), [&x][$s5 - 2];",
      {
        brig_inst_ldst_size,
        BrigEInstLdSt,
        BrigLd,
        Brigb128,
        BrigNoPacking,
        {
          operand_offset, 
          operand_offset + immed_size + reg_size * 4, 
          operand_offset + immed_size + reg_size * 5 + regv4_size + address_size, 
          0, 
          0
        },
        BrigFlatSpace,
        BrigParAcquire,
        2
      }
    }

  };

  class TestLdInvalid: public ::testing::TestWithParam<int>
  {

  };

  std::string inputarray_ld_invalid[12] = {
    //if type is b, length must be 128
    "ld_b64 $d0, [$d1];",
    "ld_b32 $s0, [$d1];",
    "ld_b8 $s0, [$d1];",
    "ld_b16 $s0, [$d1];",
    "ld_v2_b64 ($d0,$d1), [$d2];",
    "ld_v2_b32 ($s0,$s1), [$d1];",
    "ld_v2_b8 ($s0,$s1), [$d1];",
    "ld_v2_b16 ($s0,$s1), [$d1];",
    "ld_v4_b64 ($d0,$d1,$d2,$d3), [$d5];",
    "ld_v4_b32 ($s0,$s1,$s2,$s3), [$d1];",
    "ld_v4_b8 ($s0,$s1,$s2,$s3), [$d1];",
    "ld_v4_b16 ($s0,$s1,$s2,$s3), [$d1];"
  };

  class OperationLda : public ::testing::TestWithParam<int>{
  };

  struct LdaTest{
    const char* str;
    BrigInstMem ref;
  };

  struct LdaTest lda_pair[22] = {
    //lda_pair[0]
    {
      "lda_u32 $s1, [&x];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[1]
    {
      "lda_global_u32 $s1, [%g];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigGlobalSpace
      }
    },
    //lda_pair[2]
    {
      "lda_group_u32 $s1, [$s2];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigGroupSpace
      }
    },
    //lda_pair[3]
    {
      "lda_private_u32 $s1, [$s2 + 10];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigPrivateSpace
      }
    },
    //lda_pair[4]
    {
      "lda_kernarg_u32 $s1, [$s2 - 5];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigKernargSpace
      }
    },
    //lda_pair[5]
    {
      "lda_readonly_u32 $s1, [%g][$s2];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigReadonlySpace
      }
    },
    //lda_pair[6]
    {
      "lda_spill_u32 $s1, [%g][$s2 + 10];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigSpillSpace
      }
    },
    //lda_pair[7]
    {
      "lda_arg_u32 $s1, [%g][$s2 - 5];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigArgSpace
      }
    },
    //lda_pair[8]
    {
      "lda_u32 $s1, [0x6e];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigFlatSpace
      }
    },

    //lda_pair[9]
    {
      "lda_u32 $s1, [%g][4];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[10]
    {
      "lda_u32 $s1, [&array][4];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[11]
    {
      "lda_u64 $d1, [&x];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[12]
    {
      "lda_u64 $d1, [%g];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[13]
    {
      "lda_u64 $d1, [$d2];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[14]
    {
      "lda_u64 $d1, [0x6e];", 
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[15]
    {
      "lda_u64 $d1, [$d2 + 10];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[16]
    {
      "lda_u64 $d1, [$d2 - 5];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[17]
    {
      "lda_u64 $d1, [%g][4];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[18]
    {
      "lda_u64 $d1, [&array][4];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[19]
    {
      "lda_u64 $d1, [%g][$d2];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[20]
    {
      "lda_u64 $d1, [%g][$d2 + 10];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    },
    //lda_pair[21]
    {
      "lda_u64 $d1, [%g][$d2 - 5];",
      {
        brig_inst_mem_size,
        BrigEInstMem,
        BrigLda,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size * 2 + address_size, 0, 0, 0},
        BrigFlatSpace
      }
    }
  };

  class OperationLdc : public ::testing::TestWithParam<int>{
  };

  struct LdcTest{
    const char* str;
    BrigInstBase ref;
  };

  struct LdcTest ldc_pair[3] = {
    //ldc_pair[0]
    {
      "ldc_u32 $s1, @lab;",
      {
        brig_inst_base_size,
        BrigEInstBase,
        BrigLdc,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0}
      }
    },
    //ldc_pair[1]
    {
      "ldc_u32 $s1, &foo1;",
      {
        brig_inst_base_size,
        BrigEInstBase,
        BrigLdc,
        Brigu32,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0}
      }
    },
    //ldc_pair[2]
    {
      "ldc_u64 $d1, &foo2;",
      {
        brig_inst_base_size,
        BrigEInstBase,
        BrigLdc,
        Brigu64,
        BrigNoPacking,
        {operand_offset, operand_offset + reg_size, 0, 0, 0}
      }
    }
  };

}  // namespace brig
}  // namespace hsa
#endif //LD_TEST_H_
