/* Copyright 2012 <MulticorewareInc> */

#ifndef ATOMIC_TEST_H_
#define ATOMIC_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestAtomicAnd : public ::testing::TestWithParam<int>
{

};
struct AtomicTest{
  const char *str;
  BrigInstAtomic ref;
};

struct AtomicTest atomic_and_pair[6] = {
//atomic_and_pair[0]
  {
    "atomic_and_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAnd,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_and_pair[1]
  {
    "atomic_and_global_b64 $d1, [&x], $d2;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + address_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_and_pair[2]
  {
    "atomic_and_group_u32 $s1, [&x], WAVESIZE;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size + address_size,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_and_pair[3]
  {
    "atomic_and_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAnd,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_and_pair[4]
  {
    "atomic_and_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_and_pair[5]
  {
    "atomic_and_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicOr : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_or_pair[6] = {
//atomic_or_pair[0]
  {
    "atomic_or_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_or_pair[1]
  {
    "atomic_or_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_or_pair[2]
  {
    "atomic_or_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_or_pair[3]
  {
    "atomic_or_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_or_pair[4]
  {
    "atomic_or_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_or_pair[5]
  {
    "atomic_or_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicXor : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_xor_pair[6] = {
//atomic_xor_pair[0]
  {
    "atomic_xor_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_xor_pair[1]
  {
    "atomic_xor_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_xor_pair[2]
  {
    "atomic_xor_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_xor_pair[3]
  {
    "atomic_xor_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_xor_pair[4]
  {
    "atomic_xor_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_xor_pair[5]
  {
    "atomic_xor_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicExch : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_exch_pair[6] = {
//atomic_exch_pair[0]
  {
    "atomic_exch_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_exch_pair[1]
  {
    "atomic_exch_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_exch_pair[2]
  {
    "atomic_exch_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_exch_pair[3]
  {
    "atomic_exch_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_exch_pair[4]
  {
    "atomic_exch_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_exch_pair[5]
  {
    "atomic_exch_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicExch,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicAdd : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_add_pair[6] = {
//atomic_add_pair[0]
  {
    "atomic_add_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_add_pair[1]
  {
    "atomic_add_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_add_pair[2]
  {
    "atomic_add_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_add_pair[3]
  {
    "atomic_add_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_add_pair[4]
  {
    "atomic_add_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_add_pair[5]
  {
    "atomic_add_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicSub : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_sub_pair[6] = {
//atomic_sub_pair[0]
  {
    "atomic_sub_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_sub_pair[1]
  {
    "atomic_sub_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_sub_pair[2]
  {
    "atomic_sub_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_sub_pair[3]
  {
    "atomic_sub_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_sub_pair[4]
  {
    "atomic_sub_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_sub_pair[5]
  {
    "atomic_sub_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicInc : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_inc_pair[6] = {
//atomic_inc_pair[0]
  {
    "atomic_inc_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_inc_pair[1]
  {
    "atomic_inc_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_inc_pair[2]
  {
    "atomic_inc_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_inc_pair[3]
  {
    "atomic_inc_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_inc_pair[4]
  {
    "atomic_inc_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_inc_pair[5]
  {
    "atomic_inc_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicDec : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_dec_pair[6] = {
//atomic_dec_pair[0]
  {
    "atomic_dec_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_dec_pair[1]
  {
    "atomic_dec_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_dec_pair[2]
  {
    "atomic_dec_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_dec_pair[3]
  {
    "atomic_dec_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_dec_pair[4]
  {
    "atomic_dec_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_dec_pair[5]
  {
    "atomic_dec_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicMax : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_max_pair[6] = {
//atomic_max_pair[0]
  {
    "atomic_max_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_max_pair[1]
  {
    "atomic_max_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_max_pair[2]
  {
    "atomic_max_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_max_pair[3]
  {
    "atomic_max_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_max_pair[4]
  {
    "atomic_max_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_max_pair[5]
  {
    "atomic_max_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicMin : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_min_pair[6] = {
//atomic_min_pair[0]
  {
    "atomic_min_b32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_min_pair[1]
  {
    "atomic_min_global_b64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_min_pair[2]
  {
    "atomic_min_group_u32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_min_pair[3]
  {
    "atomic_min_group_acq_u64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_min_pair[4]
  {
    "atomic_min_global_ar_s32 $s1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_min_pair[5]
  {
    "atomic_min_global_part_ar_s64 $d1, [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicCas : public ::testing::TestWithParam<int>
{

};

struct AtomicTest atomic_cas_pair[6] = {
//atomic_cas_pair[0]
  {
    "atomic_cas_b32 $s1, [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(RoundUp8(operand_offset + reg_size + address_size) + immed_size),
      0
      },
      BrigAtomicCas,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomic_cas_pair[1]
  {
    "atomic_cas_global_b64 $d1, [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(RoundUp8(operand_offset + reg_size + address_size) + immed_size),
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomic_cas_pair[2]
  {
    "atomic_cas_group_u32 $s1, [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(RoundUp8(operand_offset + reg_size + address_size) + immed_size),
      0
      },
      BrigAtomicCas,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomic_cas_pair[3]
  {
    "atomic_cas_group_acq_u64 $d1, [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(RoundUp8(operand_offset + reg_size + address_size) + immed_size),
      0
      },
      BrigAtomicCas,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomic_cas_pair[4]
  {
    "atomic_cas_global_ar_s32 $s1, [&x], 23, $s2;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(operand_offset + reg_size + address_size) + immed_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomic_cas_pair[5]
  {
    "atomic_cas_global_part_ar_s64 $d1, [&x], 23, WAVESIZE;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomic,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size + address_size),
      RoundUp8(operand_offset + reg_size + address_size) + immed_size,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetAnd : public ::testing::TestWithParam<int>
{

};
struct AtomicNoRetTest{
  const char *str;
  BrigInstAtomic ref;
};

struct AtomicNoRetTest atomicnoret_and_pair[6] = {
//atomicNoRet_and_pair[0]
  {
    "atomicNoRet_and_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_and_pair[1]
  {
    "atomicNoRet_and_global_b64 [&x], $d2;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + address_size, 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_and_pair[2]
  {
    "atomicNoRet_and_group_u32 [&x], WAVESIZE;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      operand_offset + address_size, 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_and_pair[3]
  {
    "atomicNoRet_and_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_and_pair[4]
  {
    "atomicNoRet_and_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_and_pair[5]
  {
    "atomicNoRet_and_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAnd,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicNoRetOr : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_or_pair[6] = {
//atomicNoRet_or_pair[0]
  {
    "atomicNoRet_or_b32 [&x],23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_or_pair[1]
  {
    "atomicNoRet_or_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_or_pair[2]
  {
    "atomicNoRet_or_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_or_pair[3]
  {
    "atomicNoRet_or_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_or_pair[4]
  {
    "atomicNoRet_or_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_or_pair[5]
  {
    "atomicNoRet_or_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicOr,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicNoRetXor : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_xor_pair[6] = {
//atomicNoRet_xor_pair[0]
  {
    "atomicNoRet_xor_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_xor_pair[1]
  {
    "atomicNoRet_xor_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_xor_pair[2]
  {
    "atomicNoRet_xor_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_xor_pair[3]
  {
    "atomicNoRet_xor_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_xor_pair[4]
  {
    "atomicNoRet_xor_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_xor_pair[5]
  {
    "atomicNoRet_xor_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicXor,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetAdd : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_add_pair[6] = {
//atomicNoRet_add_pair[0]
  {
    "atomicNoRet_add_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_add_pair[1]
  {
    "atomicNoRet_add_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_add_pair[2]
  {
    "atomicNoRet_add_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_add_pair[3]
  {
    "atomicNoRet_add_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_add_pair[4]
  {
    "atomicNoRet_add_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_add_pair[5]
  {
    "atomicNoRet_add_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicAdd,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};


class TestAtomicNoRetSub : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_sub_pair[6] = {
//atomicNoRet_sub_pair[0]
  {
    "atomicNoRet_sub_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_sub_pair[1]
  {
    "atomicNoRet_sub_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_sub_pair[2]
  {
    "atomicNoRet_sub_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_sub_pair[3]
  {
    "atomicNoRet_sub_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_sub_pair[4]
  {
    "atomicNoRet_sub_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_sub_pair[5]
  {
    "atomicNoRet_sub_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicSub,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetInc : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_inc_pair[6] = {
//atomicNoRet_inc_pair[0]
  {
    "atomicNoRet_inc_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_inc_pair[1]
  {
    "atomicNoRet_inc_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_inc_pair[2]
  {
    "atomicNoRet_inc_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_inc_pair[3]
  {
    "atomicNoRet_inc_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_inc_pair[4]
  {
    "atomicNoRet_inc_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_inc_pair[5]
  {
    "atomicNoRet_inc_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicInc,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetDec : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_dec_pair[6] = {
//atomicNoRet_dec_pair[0]
  {
    "atomicNoRet_dec_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_dec_pair[1]
  {
    "atomicNoRet_dec_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_dec_pair[2]
  {
    "atomicNoRet_dec_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_dec_pair[3]
  {
    "atomicNoRet_dec_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_dec_pair[4]
  {
    "atomicNoRet_dec_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_dec_pair[5]
  {
    "atomicNoRet_dec_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicDec,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetMax : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_max_pair[6] = {
//atomicNoRet_max_pair[0]
  {
    "atomicNoRet_max_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_max_pair[1]
  {
    "atomicNoRet_max_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_max_pair[2]
  {
    "atomicNoRet_max_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_max_pair[3]
  {
    "atomicNoRet_max_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_max_pair[4]
  {
    "atomicNoRet_max_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_max_pair[5]
  {
    "atomicNoRet_max_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMax,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetMin : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_min_pair[6] = {
//atomicNoRet_min_pair[0]
  {
    "atomicNoRet_min_b32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_min_pair[1]
  {
    "atomicNoRet_min_global_b64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_min_pair[2]
  {
    "atomicNoRet_min_group_u32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_min_pair[3]
  {
    "atomicNoRet_min_group_acq_u64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_min_pair[4]
  {
    "atomicNoRet_min_global_ar_s32 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_min_pair[5]
  {
    "atomicNoRet_min_global_part_ar_s64 [&x], 23;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      0,
      0,
      0
      },
      BrigAtomicMin,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};

class TestAtomicNoRetCas : public ::testing::TestWithParam<int>
{

};

struct AtomicNoRetTest atomicnoret_cas_pair[6] = {
//atomicNoRet_cas_pair[0]
  {
    "atomicNoRet_cas_b32 [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigFlatSpace,
      BrigRegular
    }
  },
//atomicNoRet_cas_pair[1]
  {
    "atomicNoRet_cas_global_b64 [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigb64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigRegular
    }
  },
//atomicNoRet_cas_pair[2]
  {
    "atomicNoRet_cas_group_u32 [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigGroupSpace,
      BrigRegular
    }
  },
//atomicNoRet_cas_pair[3]
  {
    "atomicNoRet_cas_group_acq_u64 [&x], 23, 12;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigu64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigGroupSpace,
      BrigAcquire
    }
  },
//atomicNoRet_cas_pair[4]
  {
    "atomicNoRet_cas_global_ar_s32 [&x], 23, $s2;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs32,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigAcquireRelease
    }
  },
//atomicNoRet_cas_pair[5]
  {
    "atomicNoRet_cas_global_part_ar_s64 [&x], 23, WAVESIZE;",
    {
      brig_inst_atomic_size,
      BrigEInstAtomic,
      BrigAtomicNoRet,
      Brigs64,
      BrigNoPacking,
      {
      operand_offset, 
      RoundUp8(operand_offset + address_size), 
      RoundUp8(RoundUp8(operand_offset + address_size) + immed_size), 
      0,
      0
      },
      BrigAtomicCas,
      BrigGlobalSpace,
      BrigParAcquireRelease
    }
  }
};
}  // namespace brig
}  // namespace hsa
#endif //ATOMIC_TEST_H_
