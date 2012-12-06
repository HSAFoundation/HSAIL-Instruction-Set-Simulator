/* Copyright 2012 <MulticorewareInc> */

#ifndef INSTRUCTION2_TEST_H_
#define INSTRUCTION2_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

struct Instruction2Test{
  const char* str;
  BrigInstBase ref;
};

class TestInstruction2Abs : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2abs_pair[6] = {
//instruction2abs_pair[0]
  {
    "abs_s32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2abs_pair[1]
  {
    "abs_s32 $s1, 4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2abs_pair[2]
  {
    "abs_s32 $s1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2abs_pair[3]
  {
    "abs_s64 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2abs_pair[4]
  {
    "abs_s64 $d1, 4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2abs_pair[5]
  {
    "abs_s64 $d1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

class TestInstruction2PackAbs : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2packabs_pair[10] = {
//instruction2packabs_pair[0]
  {
    "abs_p_s8x4 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[1]
  {
    "abs_p_s16x2 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs16x2,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[2]
  {
    "abs_s_s8x4 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[3]
  {
    "abs_s_s16x2 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs16x2,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[4]
  {
    "abs_p_s8x8 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs8x8,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[5]
  {
    "abs_p_s16x4 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs16x4,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[6]
  {
    "abs_p_s32x2 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs32x2,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[7]
  {
    "abs_s_s8x8 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs8x8,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[8]
  {
    "abs_s_s16x4 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs16x4,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packabs_pair[9]
  {
    "abs_s_s32x2 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigAbs,
    Brigs32x2,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigNeg
class TestInstruction2Neg : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2neg_pair[6] = {
//instruction2neg_pair[0]
  {
    "neg_s32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2neg_pair[1]
  {
    "neg_s32 $s1, 4;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
  }
  },
//instruction2neg_pair[2]
  {
    "neg_s32 $s1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2neg_pair[3]
  {
    "neg_s64 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2neg_pair[4]
  {
    "neg_s64 $d1, 4;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2neg_pair[5]
  {
    "neg_s64 $d1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

class TestInstruction2PackNeg : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2packneg_pair[10] = {
//instruction2packneg_pair[0]
  {
    "neg_p_s8x4 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs8x4,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[1]
  {
    "neg_p_s16x2 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs16x2,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
   }
  },
//instruction2packneg_pair[2]
  {
    "neg_s_s8x4 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs8x4,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[3]
  {
    "neg_s_s16x2 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs16x2,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[4]
  {
    "neg_p_s8x8 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs8x8,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
   }
  },
//instruction2packneg_pair[5]
  {
    "neg_p_s16x4 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs16x4,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[6]
  {
    "neg_p_s32x2 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs32x2,
    BrigPackP,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[7]
  {
    "neg_s_s8x8 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs8x8,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[8]
  {
    "neg_s_s16x4 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs16x4,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2packneg_pair[9]
  {
    "neg_s_s32x2 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNeg,
    Brigs32x2,
    BrigPackS,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigNot
class TestInstruction2Not : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2not_pair[9] = {
//instruction2not_pair[0]
  {
    "not_b1 $c1, $c2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2not_pair[1]
  {
    "not_b32 $s0, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2not_pair[2]
  {
    "not_b64 $d0, $d1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2not_pair[3]
  {
    "not_b1 $c1, 0;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2not_pair[4]
  {
    "not_b32 $s0, 0x11;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2not_pair[5]
  {
    "not_b64 $d0, 0x11;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2not_pair[6]
  {
    "not_b1 $c1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2not_pair[7]
  {
    "not_b32 $s0, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2not_pair[8]
  {
    "not_b64 $d0, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNot,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigPopcount
class TestInstruction2Popcount : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2popcount_pair[6] = {
//instruction2popcount_pair[0]
  {
    "popcount_b32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2popcount_pair[1]
  {
    "popcount_b64 $s1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2popcount_pair[2]
  {
    "popcount_b32 $s1, 0x00010001;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2popcount_pair[3]
  {
    "popcount_b64 $s1, 0x00010001;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2popcount_pair[4]
  {
    "popcount_b32 $s1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2popcount_pair[5]
  {
    "popcount_b64 $s1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigPopCount,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigFirsrbit
class TestInstruction2Firstbit : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2firstbit_pair[6] = {
//instruction2firstbit_pair[0]
  {
    "firstbit_s32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[1]
  {
    "firstbit_s64 $s1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[2]
  {
    "firstbit_s32 $s1, 0x01;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2firstbit_pair[3]
  {
    "firstbit_s64 $s1, 0x01;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2firstbit_pair[4]
  {
    "firstbit_s32 $s1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[5]
  {
    "firstbit_s64 $s1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFirstBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigLastbit
class TestInstruction2Lastbit : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2lastbit_pair[6] = {
//instruction2firstbit_pair[0]
  {
    "lastbit_s32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[1]
  {
    "lastbit_s64 $s1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[2]
  {
    "lastbit_s32 $s1, 0x01;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2firstbit_pair[3]
  {
    "lastbit_s64 $s1, 0x01;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2firstbit_pair[4]
  {
    "lastbit_s32 $s1, WAVESIZE;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2firstbit_pair[5]
  {
    "lastbit_s64 $s1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLastBit,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
};

// BrigBitrev
class TestInstruction2Bitrev : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2bitrev_pair[6] = {
//instruction2bitrev_pair[0]
  {
    "bitrev_s32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2bitrev_pair[1]
  {
    "bitrev_s64 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2bitrev_pair[2]
  {
    "bitrev_s32 $s1, 0x01;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2bitrev_pair[3]
  {
    "bitrev_s64 $d1, 0x01;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2bitrev_pair[4]
  {
    "bitrev_s32 $s1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2bitrev_pair[5]
  {
    "bitrev_s64 $d1, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigBitRev,
    Brigs64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  }
};

// BrigMovslo
class TestInstruction2MovLo : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2movslo_pair[2] = {
//instruction2movs_pair[0]
  {
    "movs_lo_b32 $s1, $d1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMovsLo,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2movs_pair[1]
  {
    "movs_lo_b32 $s1, 0x000100010001;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMovsLo,
    Brigb32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

// BrigMovsHi
class TestInstruction2Movhi : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2movshi_pair[2] = {
//instruction2movshi_pair[0]
  {
    "movs_hi_b32 $s1, $d1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMovsHi,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2movshi_pair[1]
  {
    "movs_hi_b32 $s1, 0x000100010001;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMovsHi,
    Brigb32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

// BrigCount
class TestInstruction2Count : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2count_pair[3] = {
//instruction2count_pair[0]
  {
    "count_u32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2count_pair[1]
  {
    "count_u32 $s1, $c1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2count_pair[2]
  {
    "count_u32 $s1, 100;",
      {
    brig_inst_base_size,
    BrigEInstBase,
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

// BrigMask
class TestInstruction2Mask : public ::testing::TestWithParam<int>
{

};

struct Instruction2Test instruction2mask_pair[3] = {
//instruction2mask_pair[0]
  {
    "mask_b64 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2mask_pair[1]
  {
    "mask_b64 $d1, $c1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2mask_pair[2]
  {
    "mask_b64 $d1, 100;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
   }
  }
};

class TestInstruction2FTZsqrt : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ{
  const char* str;
  BrigInstBase ref;
};

struct Instruction2OpFTZ  instruction2opftz_sqrt_pair[6] = {
//instruction2opftz_sqrt_pair[0]
  {
    "sqrt_f16 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_sqrt_pair[1]
  {
    "sqrt_f32 $s1, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_sqrt_pair[2]
  {
    "sqrt_f64 $d1, $d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_sqrt_pair[3]
  {
    "sqrt_f32 $s1, 1.0f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2opftz_sqrt_pair[4]
  {
    "sqrt_f64 $d1, 1.0;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModSqrt : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZMod{
  const char* str;
  BrigInstMod ref;
};

BrigAluModifier sqrt_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 2, 0, 0, 0, 0},
  {1, 1, 3, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 2, 1, 0, 0, 0},
  {1, 1, 3, 1, 0, 0, 0},
  {1, 1, 1, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modsqrt_pair[42] = {
//instruction2opftz_modsqrt_pair[0]
  {
    "sqrt_ftz_f16 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[1]
  {
    "sqrt_up_f16 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[2]
  {
    "sqrt_down_f16 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[3]
  {
    "sqrt_zero_f16 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[4]
  {
    "sqrt_ftz_up_f16 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[5]
  {
    "sqrt_ftz_down_f16 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[6]
  {
    "sqrt_ftz_zero_f16 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[6]
    }
  },
//instruction2opftz_modsqrt_pair[7]
  {
    "sqrt_ftz_f32 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[8]
  {
    "sqrt_up_f32 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[9]
  {
    "sqrt_down_f32 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[10]
  {
    "sqrt_zero_f32 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[11]
  {
    "sqrt_ftz_up_f32 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[12]
  {
    "sqrt_ftz_down_f32 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[13]
  {
    "sqrt_ftz_zero_f32 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[6]
    }
  },
//instruction2opftz_modsqrt_pair[14]
  {
    "sqrt_ftz_f64 $d1, $d2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[15]
  {
    "sqrt_up_f64 $d1, $d2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[16]
  {
    "sqrt_down_f64 $d1, $d2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[17]
  {
    "sqrt_zero_f64 $d1, $d2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[18]
  {
    "sqrt_ftz_up_f64 $d1, $d2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[19]
  {
    "sqrt_ftz_down_f64 $d1, $d2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[20]
  {
    "sqrt_ftz_zero_f64 $d1, $d2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    sqrt_bam[6]
    }
  },
//instruction2opftz_modsqrt_pair[21]
  {
    "sqrt_ftz_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[22]
  {
    "sqrt_up_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[23]
  {
    "sqrt_down_f16 $s1, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[24]
  {
    "sqrt_zero_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[25]
  {
    "sqrt_ftz_up_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[26]
  {
    "sqrt_ftz_down_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[27]
  {
    "sqrt_ftz_zero_f16 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[6]
    }
  },
//instruction2opftz_modsqrt_pair[28]
  {
    "sqrt_ftz_f32 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[29]
  {
    "sqrt_up_f32 $s1, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[30]
  {
    "sqrt_down_f32 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[31]
  {
    "sqrt_zero_f32 $s1, 1.0f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[32]
  {
    "sqrt_ftz_up_f32 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[33]
  {
    "sqrt_ftz_down_f32 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[34]
  {
    "sqrt_ftz_zero_f32 $s1, 1.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[6]
    }
  },
//instruction2opftz_modsqrt_pair[35]
  {
    "sqrt_ftz_f64 $d1,1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[36]
  {
    "sqrt_up_f64 $d1,1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[37]
  {
    "sqrt_down_f64 $d1, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[38]
  {
    "sqrt_zero_f64 $d1, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[3]
    }
  },
//instruction2opftz_modsqrt_pair[39]
  {
    "sqrt_ftz_up_f64 $d1, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[4]
    }
  },
//instruction2opftz_modsqrt_pair[40]
  {
    "sqrt_ftz_down_f64 $d1, 1.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[5]
    }
  },
//instruction2opftz_modsqrt_pair[41]
  {
    "sqrt_ftz_zero_f64 $d1, 1.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigSqrt,
    Brigf64,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    sqrt_bam[6]
    }
  }
};



class TestInstruction2FtzSqrtInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_sqrt_invalid[27] = {
  "sqrt_f16 1.0, $s2;",
  "sqrt_f16 1.0, 1.0;",
  "sqrt_f16 ;",
  "sqrt_ftz_f16 ;",
  "sqrt_ftz_up_f16 ;",

  "sqrt_f16 $s1;",
  "sqrt_f16 $s1, ;",
  "sqrt_f16 $s1, $s2",
  "sqrt_f32 $s1  $s2;",
  "sqrt $s1, $s2;",
  "sqrt_f32 $s1, $s2, $s3;",

  // sqrt_ftz
  "sqrt_ftz_f16 $s1;",
  "sqrt_ftz_f16 $s1, ;",
  "sqrt_ftz_f16 $s1, $s2",
  "sqrt_ftz_f32 $s1  $s2;",
  "sqrt_ftz $s1, $s2;",
  //"sqrt_ftz_f64 $s1, $s2;",
  //"sqrt_ftz_f32 $d1, $d2;",
  "sqrt_ftz_f32 $s1, $s2, $s3;",

  // sqrt_ftz_round
  "sqrt_up_f16 $s1;",
  "sqrt_down_f16 $s1, ;",
  "sqrt_zero_f16 $s1, $s2",
  "sqrt_ftz_up_f32 $s1  $s2;",
  "sqrt_ftz_zero $s1, $s2;"
};

class TestInstruction2FTZfsin : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_fsin_pair[2] = {
//instruction2opftz_fsin_pair[0]
  {
    "fsin_f32 $s0, $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFsin,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fsin_pair[1]
  {
    "fsin_f32 $s0, 0.3f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFsin,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModfsin : public ::testing::TestWithParam<int>
{

};

BrigAluModifier fsin_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modsin_pair[2] = {
//instruction2opftz_modsin_pair[0]
  {
    "fsin_ftz_f32 $s0, $s1;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFsin,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fsin_bam[0]
    }
  },
//instruction2opftz_modsin_pair[1]
  {
    "fsin_ftz_f32 $s0, 0.3f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFsin,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fsin_bam[1]
    }
  }
};

class TestInstruction2FtzfsinInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_fsin_invalid[18] = {
   "fsin_f32 1.0, $s2;",
   "fsin_f32 1.0, 1.0;",
   "fsin_f32 ;",
   "fsin_ftz_f32 ;",

  // fsin
  "fsin_f32 $s1;",
  "fsin_f32 $s1, ;",
  "fsin_f32 $s1, $s2",
  "fsin_f32 $s1  $s2;",
  //"fsin_f32 $c1, $c2;",
  "fsin $s1, $s2;",
  "fsin_f32 $s1, $s2, $s3;",

  // fsin_ftz
  "fsin_ftz_f32 $s1;",
  "fsin_ftz_f32 $s1, ;",
  "fsin_ftz_f32 $s1, $s2",
  "fsin_ftz_f32 $s1  $s2;",
  //"fsin_ftz_f32 $c1, $c2;",
  "fsin_ftz $s1, $s2;",
  "fsin_ftz_f32 $s1, $s2, $s3;"
};

class TestInstruction2FTZfcos : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_fcos_pair[2] = {
//instruction2opftz_fcos_pair[0]
  {
    "fcos_f32 $s0, $s1;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFcos,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fcos_pair[1]
  {
    "fcos_f32 $s0, 0.3f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFcos,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModfcos : public ::testing::TestWithParam<int>
{

};

BrigAluModifier fcos_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modfcos_pair[2] = {
//instruction2opftz_modfcos_pair[0]
  {
    "fcos_ftz_f32 $s0, $s1;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFcos,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fcos_bam[0]
    }
  },
//instruction2opftz_modfcos_pair[1]
  {
    "fcos_ftz_f32 $s0, 0.3f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFcos,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fcos_bam[1]
    }
  }
};

class TestInstruction2FtzfcosInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_fcos_invalid[18] = {
   "fcos_f32 1.0, $s2;",
   "fcos_f32 1.0, 1.0;",
   "fcos_f32 ;",
   "fcos_ftz_f32 ;",

  // fcos
  "fcos_f32 $s1;",
  "fcos_f32 $s1, ;",
  "fcos_f32 $s1, $s2",
  "fcos_f32 $s1  $s2;",
  //"fcos_f32 $c1, $c2;",
  "fcos $s1, $s2;",
  "fcos_f32 $s1, $s2, $s3;",

  // fcos_ftz
  "fcos_ftz_f32 $s1;",
  "fcos_ftz_f32 $s1, ;",
  "fcos_ftz_f32 $s1, $s2",
  "fcos_ftz_f32 $s1  $s2;",
  //"fcos_ftz_f32 $c1, $c2;",
  "fcos_ftz $s1, $s2;",
  "fcos_ftz_f32 $s1, $s2, $s3;"
};

class TestInstruction2FTZfexp2 : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_fexp2_pair[2] = {
//instruction2opftz_fexp2_pair[0]
  {
    "fexp2_f32 $s0, $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFexp2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fexp2_pair[1]
  {
    "fexp2_f32 $s0, 0.3f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFexp2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModfexp2 : public ::testing::TestWithParam<int>
{

};

BrigAluModifier fexp2_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modexp2_pair[2] = {
//instruction2opftz_modfcos_pair[0]
  {
    "fexp2_ftz_f32 $s0, $s1;",
    {
    36,
    BrigEInstMod,
    BrigFexp2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fexp2_bam[0]
    }
  },
//instruction2opftz_modfcos_pair[1]
  {
    "fexp2_ftz_f32 $s0, 0.3f;",
    {
    36,
    BrigEInstMod,
    BrigFexp2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fexp2_bam[1]
    }
  }
};

class TestInstruction2Ftzfexp2Invalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_fexp2_invalid[] = {
   "fexp2_f32 1.0, $s2;",
   "fexp2_f32 1.0, 1.0;",
   "fexp2_f32 ;",
   "fexp2_ftz_f32 ;",

  // fexp2
  "fexp2_f32 $s1;",
  "fexp2_f32 $s1, ;",
  "fexp2_f32 $s1, $s2",
  "fexp2_f32 $s1  $s2;",
  //"fexp2_f32 $c1, $c2;",
  "fexp2 $s1, $s2;",
  "fexp2_f32 $s1, $s2, $s3;",

  // fexp2_ftz
  "fexp2_ftz_f32 $s1;",
  "fexp2_ftz_f32 $s1, ;",
  "fexp2_ftz_f32 $s1, $s2",
  "fexp2_ftz_f32 $s1  $s2;",
  //"fexp2_ftz_f32 $c1, $c2;",
  "fexp2_ftz $s1, $s2;",
  "fexp2_ftz_f32 $s1, $s2, $s3;"
};

class TestInstruction2FTZflog2 : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_flog2_pair[2] = {
//instruction2opftz_flog2_pair[0]
  {
    "flog2_f32 $s0, $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFlog2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_flog2_pair[1]
  {
    "flog2_f32 $s0, 0.3f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFlog2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModflog2 : public ::testing::TestWithParam<int>
{

};

BrigAluModifier flog2_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modflog2_pair[2] = {
//instruction2opftz_modflog2_pair[0]
  {
    "flog2_ftz_f32 $s0, $s1;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFlog2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    flog2_bam[0]
    }
  },
//instruction2opftz_modflog2_pair[1]
  {
    "flog2_ftz_f32 $s0, 0.3f;",
   {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFlog2,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    flog2_bam[1]
    }
  }
};

class TestInstruction2Ftzflog2Invalid : public ::testing::TestWithParam<int>
{

};

std::string input_instruction2opftz_flog2_invalid[] = {
   "flog2_f32 1.0, $s2;",
   "flog2_f32 1.0, 1.0;",
   "flog2_f32 ;",
   "flog2_ftz_f32 ;",

  // flog2
  "flog2_f32 $s1;",
  "flog2_f32 $s1, ;",
  "flog2_f32 $s1, $s2",
  "flog2_f32 $s1  $s2;",
  //"flog2_f32 $c1, $c2;",
  "flog2 $s1, $s2;",
  "flog2_f32 $s1, $s2, $s3;",

  // flog2_ftz
  "flog2_ftz_f32 $s1;",
  "flog2_ftz_f32 $s1, ;",
  "flog2_ftz_f32 $s1, $s2",
  "flog2_ftz_f32 $s1  $s2;",
  //"flog2_ftz_f32 $c1, $c2;",
  "flog2_ftz $s1, $s2;",
  "flog2_ftz_f32 $s1, $s2, $s3;"
};



class TestInstruction2FTZFrcp : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_frcp_pair[4] = {
//instruction2opftz_frcp_pair[0]
  {
    "frcp_f32 $s1,$s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrcp,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_frcp_pair[1]
  {
    "frcp_f64 $d1,$d2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrcp,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_frcp_pair[2]
  {
    "frcp_f32 $s1,4.0f;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrcp,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2opftz_frcp_pair[3]
  {
    "frcp_f64 $d1,4.0;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrcp,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModFrcp : public ::testing::TestWithParam<int>
{

};

BrigAluModifier frcp_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
};

struct Instruction2OpFTZMod  instruction2opftz_modfrcp_pair[4] = {
//instruction2opftz_modfrcp_pair[0]
  {
    "frcp_ftz_f32 $s1,$s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrcp,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    frcp_bam[0]
    }
  },
//instruction2opftz_modfrcp_pair[1]
  {
    "frcp_ftz_f64 $d1,$d2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrcp,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    frcp_bam[2]
    }
  },
//instruction2opftz_modfrcp_pair[2]
  {
    "frcp_ftz_f32 $s1,4.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrcp,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    frcp_bam[1]
    }
  },
//instruction2opftz_modfrcp_pair[3]
  {
    "frcp_ftz_f64 $d1,4.0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrcp,
    Brigf64,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    frcp_bam[3]
    }
  }
};

class TestInstruction2FtzfrcpInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_frcp_invalid[19] = {
   "frcp_f16 1.0, $s2;",
   "frcp_f16 1.0, 1.0;",
   "frcp_f16 ;",
   "frcp_ftz_f16 ;",

  // frcp
  "frcp_f32 $s1;",
  "frcp_f32 $s1, ;",
  "frcp_f32 $s1, $s2",
  "frcp_f32 $s1  $s2;",
  //"frcp_f32 $c1, $c2;",
  "frcp $s1, $s2;",
  //"frcp_f64 $s1, $s2;",
  "frcp_f32 $s1, $s2, $s3;",

  // frcp_ftz
  "frcp_ftz_f32 $s1;",
  "frcp_ftz_f32 $s1, ;",
  "frcp_ftz_f32 $s1, $s2",
  "frcp_ftz_f32 $s1  $s2;",
  //"frcp_ftz_f32 $c1, $c2;",
  "frcp_ftz $s1, $s2;",
  //"frcp_ftz_f64 $s1, $s2;",
  "frcp_ftz_f32 $s1, $s2, $s3;",
};

class TestInstruction2FTZfract : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_fract_pair[6] = {
//instruction2opftz_fract_pair[0]
  {
    "fract_f16 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fract_pair[1]
  {
    "fract_f32 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fract_pair[2]
  {
    "fract_f64 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_fract_pair[3]
  {
    "fract_f16 $s1, 3.2f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf16,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2opftz_fract_pair[4]
  {
    "fract_f32 $s1, 3.2f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf32,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2opftz_fract_pair[5]
  {
    "fract_f64 $d1, 3.2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFract,
    Brigf64,
    BrigNoPacking,
   {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModfract : public ::testing::TestWithParam<int>
{

};

BrigAluModifier fract_bam[] = {
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modfract_pair[6] = {
//instruction2opftz_modfract_pair[0]
  {
    "fract_ftz_f16 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf16,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fract_bam[0]
    }
  },
//instruction2opftz_modfract_pair[1]
  {
    "fract_ftz_f32 $s1, $s2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fract_bam[2]
    }
  },
//instruction2opftz_modfract_pair[2]
  {
    "fract_ftz_f64 $d1, $d2;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    fract_bam[4]
    }
  },
//instruction2opftz_modfract_pair[3]
  {
    "fract_ftz_f16 $s1, 3.2f;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf16,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fract_bam[1]
    }
  },
//instruction2opftz_modfract_pair[4]
  {
    "fract_ftz_f32 $s1, 3.2f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fract_bam[3]
    }
  },
//instruction2opftz_modfract_pair[5]
  {
    "fract_ftz_f64 $d1, 3.2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFract,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    fract_bam[5]
    }
  }
};

class TestInstruction2FtzfractInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_instruction2opftz_fract_invalid[] = {
   "fract_f16 1.0, $s2;",
   "fract_f16 1.0, 1.0;",
   "fract_f16 ;",
   "fract_ftz_f16 ;",

  // fract
  "fract_f16 $s1;",
  "fract_f32 $s1, ;",
  "fract_f16 $s1, $s2",
  "fract_f32 $s1  $s2;",
  //"fract_f32 $c1, $c2;",
  "fract $s1, $s2;",
  //"fract_f64 $s1, $s2;",
  "fract_f32 $s1, $s2, $s3;",

  // fract_ftz
  "fract_ftz_f16 $s1;",
  "fract_ftz_f32 $s1, ;",
  "fract_ftz_f32 $s1, $s2",
  "fract_ftz_f16 $s1  $s2;",
  //"fract_ftz_f32 $c1, $c2;",
  "fract_ftz $s1, $s2;",
  //"fract_ftz_f64 $s1, $s2;",
  "fract_ftz_f32 $s1, $s2, $s3;"
};

class TestInstruction2FTZfrsqrt : public ::testing::TestWithParam<int>
{

};

struct Instruction2OpFTZ  instruction2opftz_frsqrt_pair[4] = {
//instruction2opftz_frsqrt_pair[0]
  {
    "frsqrt_f32 $s1, $s2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrsqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_frsqrt_pair[1]
  {
    "frsqrt_f64 $d1, $d2;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrsqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//instruction2opftz_frsqrt_pair[2]
  {
    "frsqrt_f32 $s1, 4.0f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrsqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
//instruction2opftz_frsqrt_pair[3]
  {
    "frsqrt_f64 $d1, 4.0;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigFrsqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  }
};

class TestInstruction2FTZModfrsqrt : public ::testing::TestWithParam<int>
{

};

BrigAluModifier frsqrt_bam[] = {
  // Brigf32
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0},

  // Brigf64
  {1, 1, 0, 1, 0, 0, 0},
  {1, 1, 0, 1, 0, 0, 0}
};

struct Instruction2OpFTZMod  instruction2opftz_modsfrqrt_pair[4] = {
//instruction2opftz_modsqrt_pair[0]
  {
    "frsqrt_ftz_f32 $s1, $s2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrsqrt,
    Brigf32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0},
    frsqrt_bam[0]
    }
  },
//instruction2opftz_modsqrt_pair[1]
  {
    "frsqrt_ftz_f64 $d1, $d2;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrsqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    frsqrt_bam[2]
    }
  },
//instruction2opftz_modsqrt_pair[2]
  {
    "frsqrt_ftz_f32 $s1, 4.0f;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrsqrt,
    Brigf32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    frsqrt_bam[1]
    }
  },
//instruction2opftz_modsqrt_pair[3]
  {
    "frsqrt_ftz_f64 $d1, 4.0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigFrsqrt,
    Brigf64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    frsqrt_bam[3]
    }
  }
};

class TestInstruction2FtzfrsqrtInvalid : public ::testing::TestWithParam<int>
{

};
std::string input_instruction2opftz_frsqrt_invalid[] = {
   "frsqrt_f16 1.0, $s2;",
   "frsqrt_f16 1.0, 1.0;",
   "frsqrt_f16 ;",
   "frsqrt_ftz_f16 ;",

  // frsqrt
  "frsqrt_f32 $s1;",
  "frsqrt_f32 $s1, ;",
  "frsqrt_f32 $s1, $s2",
  "frsqrt_f32 $s1  $s2;",
  //"frsqrt_f32 $c1, $c2;",
  "frsqrt $s1, $s2;",
  //"frsqrt_f64 $s1, $s2;",
  "frsqrt_f32 $s1, $s2, $s3;",

  // frsqrt_ftz
  "frsqrt_ftz_f32 $s1;",
  "frsqrt_ftz_f32 $s1, ;",
  "frsqrt_ftz_f32 $s1, $s2",
  "frsqrt_ftz_f32 $s1  $s2;",
  //"frsqrt_ftz_f32 $c1, $c2;",
  "frsqrt_ftz $s1, $s2;",
  //"frsqrt_ftz_f64 $s1, $s2;",
  "frsqrt_ftz_f32 $s1, $s2, $s3;"
};

}  // namespace brig
}  // namespace hsa
#endif //INSTRUCTION2_TEST_H_
