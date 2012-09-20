/* Copyright 2012 <MulticorewareInc> */

#ifndef CODEGEN_TEST_H_
#define CODEGEN_TEST_H_

namespace hsa {
namespace brig {

class Instruction1opNoDTLaneid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_laneid[] = {
  "laneid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_laneid[] = {
  {
    32,
    BrigEInstBase,
    BrigLaneId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDynwaveid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_dynwaveid[] = {
  "dynwaveid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_dynwaveid[] = {
  {
    32,
    BrigEInstBase,
    BrigDynWaveId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDispatchid: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_dispatchid[] = {
  "dispatchid $s1;"
};
BrigInstBase outputarray_instruction1opnodt_dispatchid[] = {
  {
    32,
    BrigEInstBase,
    BrigDispatchId,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTCu: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_cu[] = {
  "cu $s1;"
};
BrigInstBase outputarray_instruction1opnodt_cu[] = {
  {
    32,
    BrigEInstBase,
    BrigCU,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkdim: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workdim[] = {
  "workdim $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workdim[] = {
  {
    32,
    BrigEInstBase,
    BrigWorkDim,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkitemidFlat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workitemidflat[] = {
  "workitemid_flat $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workitemidflat[] = {
  {
    32,
    BrigEInstBase,
    Brigworkitemidflat,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTWorkitemaidFlat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_workitemaidflat[] = {
  "workitemaid_flat $s1;"
};
BrigInstBase outputarray_instruction1opnodt_workitemaidflat[] = {
  {
    32,
    BrigEInstBase,
    Brigworkitemaidflat,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTClock: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_clock[] = {
  "clock $d1;"
};
BrigInstBase outputarray_instruction1opnodt_clock[] = {
  {
    32,
    BrigEInstBase,
    BrigClock,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};

class Instruction1opNoDTDebugtrap: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction1opnodt_debugtrap[] = {
  "debugtrap $s0;",
  "debugtrap $s4;",
  "debugtrap 0;",
  "debugtrap 61;",
  "debugtrap WAVESIZE;"
};
BrigInstBase outputarray_instruction1opnodt_debugtrap[] = {
  {
    32,
    BrigEInstBase,
    BrigDebugtrap,
    Brigb32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  }
};
class MulBase : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_base[] = {
  // s32
  "mul_s32 $s1, $s2, $s3;",
  "mul_s32 $s1, $s2, 1;",
  "mul_s32 $s1, $s2, WAVESIZE;",
  "mul_s32 $s1, 1, $s3;",
  "mul_s32 $s1, 1, 1;",
  "mul_s32 $s1, 1, WAVESIZE;",
  "mul_s32 $s1, WAVESIZE, $s3;",
  "mul_s32 $s1, WAVESIZE, 1;",
  "mul_s32 $s1, WAVESIZE, WAVESIZE;",
  // s64
  "mul_s64 $d1, $d2, $d3;",
  "mul_s64 $d1, $d2, 1;",
  "mul_s64 $d1, $d2, WAVESIZE;",
  "mul_s64 $d1, 1, $d3;",
  "mul_s64 $d1, 1, 1;",
  "mul_s64 $d1, 1, WAVESIZE;",
  "mul_s64 $d1, WAVESIZE, $d3;",
  "mul_s64 $d1, WAVESIZE, 1;",
  "mul_s64 $d1, WAVESIZE, WAVESIZE;",
  // u32
  "mul_u32 $s1, $s2, $s3;",
  "mul_u32 $s1, $s2, 1;",
  "mul_u32 $s1, $s2, WAVESIZE;",
  "mul_u32 $s1, 1, $s3;",
  "mul_u32 $s1, 1, 1;",
  "mul_u32 $s1, 1, WAVESIZE;",
  "mul_u32 $s1, WAVESIZE, $s3;",
  "mul_u32 $s1, WAVESIZE, 1;",
  "mul_u32 $s1, WAVESIZE, WAVESIZE;",
  // u64
  "mul_u64 $d1, $d2, $d3;",
  "mul_u64 $d1, $d2, 1;",
  "mul_u64 $d1, $d2, WAVESIZE;",
  "mul_u64 $d1, 1, $d3;",
  "mul_u64 $d1, 1, 1;",
  "mul_u64 $d1, 1, WAVESIZE;",
  "mul_u64 $d1, WAVESIZE, $d3;",
  "mul_u64 $d1, WAVESIZE, 1;",
  "mul_u64 $d1, WAVESIZE, WAVESIZE;"
};
class MulBaseFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_base_false[] = {
  "mul_s32 $s1;",
  "mul_s32 $s1, $s2;",
  "mul_s32 1, $s2, $s3;",
  "mul_s32 WAVESIZE, $s2, $s3;",
  "mul_s64 $s1;",
  "mul_s64 $s1, $s2;",
  "mul_s64 1, $s2, $s3;",
  "mul_s64 WAVESIZE, $s2, $s3;",
  "mul_s64 $s1, $s2, $s3;",
  "mul_u32 $s1;",
  "mul_u32 $s1, $s2;",
  "mul_u32 1, $s2, $s3;",
  "mul_u32 WAVESIZE, $s2, $s3;",
  "mul_u64 $s1;",
  "mul_u64 $s1, $s2;",
  "mul_u64 1, $s2, $s3;",
  "mul_u64 WAVESIZE, $s2, $s3;"
};
BrigInstBase output_array_mul_base[] = {
  // s32
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // s64
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u32
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u64
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};
class MulFtzRound : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_ftz_round[] = {
  // ftz_f16
  "mul_ftz_f16 $s1, $s2, $s3;",
  "mul_ftz_f16 $s1, $s2, 1;",
  "mul_ftz_f16 $s1, 1, $s3;",
  "mul_ftz_f16 $s1, 1, 1;",
  // ftz_f32
  "mul_ftz_f32 $s1, $s2, $s3;",
  "mul_ftz_f32 $s1, $s2, 1;",
  "mul_ftz_f32 $s1, 1, $s3;",
  "mul_ftz_f32 $s1, 1, 1;",
  // ftz_f64
  "mul_ftz_f64 $d1, $d2, $d3;",
  "mul_ftz_f64 $d1, $d2, 1;",
  "mul_ftz_f64 $d1, 1, $d3;",
  "mul_ftz_f64 $d1, 1, 1;",
  // near_f16
  "mul_near_f16 $s1, $s2, $s3;",
  "mul_near_f16 $s1, $s2, 1;",
  "mul_near_f16 $s1, 1, $s3;",
  "mul_near_f16 $s1, 1, 1;",
  // near_f32
  "mul_near_f32 $s1, $s2, $s3;",
  "mul_near_f32 $s1, $s2, 1;",
  "mul_near_f32 $s1, 1, $s3;",
  "mul_near_f32 $s1, 1, 1;",
  // near_f64
  "mul_near_f64 $d1, $d2, $d3;",
  "mul_near_f64 $d1, $d2, 1;",
  "mul_near_f64 $d1, 1, $d3;",
  "mul_near_f64 $d1, 1, 1;",
  // zero_f16
  "mul_zero_f16 $s1, $s2, $s3;",
  "mul_zero_f16 $s1, $s2, 1;",
  "mul_zero_f16 $s1, 1, $s3;",
  "mul_zero_f16 $s1, 1, 1;",
  // zero_f32
  "mul_zero_f32 $s1, $s2, $s3;",
  "mul_zero_f32 $s1, $s2, 1;",
  "mul_zero_f32 $s1, 1, $s3;",
  "mul_zero_f32 $s1, 1, 1;",
  // zero_f64
  "mul_zero_f64 $d1, $d2, $d3;",
  "mul_zero_f64 $d1, $d2, 1;",
  "mul_zero_f64 $d1, 1, $d3;",
  "mul_zero_f64 $d1, 1, 1;",
  // up_f16
  "mul_up_f16 $s1, $s2, $s3;",
  "mul_up_f16 $s1, $s2, 1;",
  "mul_up_f16 $s1, 1, $s3;",
  "mul_up_f16 $s1, 1, 1;",
  // up_f32
  "mul_up_f32 $s1, $s2, $s3;",
  "mul_up_f32 $s1, $s2, 1;",
  "mul_up_f32 $s1, 1, $s3;",
  "mul_up_f32 $s1, 1, 1;",
  // up_f64
  "mul_up_f64 $d1, $d2, $d3;",
  "mul_up_f64 $d1, $d2, 1;",
  "mul_up_f64 $d1, 1, $d3;",
  "mul_up_f64 $d1, 1, 1;",
  // down_f16
  "mul_down_f16 $s1, $s2, $s3;",
  "mul_down_f16 $s1, $s2, 1;",
  "mul_down_f16 $s1, 1, $s3;",
  "mul_down_f16 $s1, 1, 1;",
  // down_f32
  "mul_down_f32 $s1, $s2, $s3;",
  "mul_down_f32 $s1, $s2, 1;",
  "mul_down_f32 $s1, 1, $s3;",
  "mul_down_f32 $s1, 1, 1;",
  // down_f64
  "mul_down_f64 $d1, $d2, $d3;",
  "mul_down_f64 $d1, $d2, 1;",
  "mul_down_f64 $d1, 1, $d3;",
  "mul_down_f64 $d1, 1, 1;",
  // ftz_zero_f16
  "mul_ftz_zero_f16 $s1, $s2, $s3;",
  "mul_ftz_zero_f16 $s1, $s2, 1;",
  "mul_ftz_zero_f16 $s1, 1, $s3;",
  "mul_ftz_zero_f16 $s1, 1, 1;",
  // ftz_zero_f32
  "mul_ftz_zero_f32 $s1, $s2, $s3;",
  "mul_ftz_zero_f32 $s1, $s2, 1;",
  "mul_ftz_zero_f32 $s1, 1, $s3;",
  "mul_ftz_zero_f32 $s1, 1, 1;",
  // ftz_zero_f64
  "mul_ftz_zero_f64 $d1, $d2, $d3;",
  "mul_ftz_zero_f64 $d1, $d2, 1;",
  "mul_ftz_zero_f64 $d1, 1, $d3;",
  "mul_ftz_zero_f64 $d1, 1, 1;",
  // ftz_up_f16
  "mul_ftz_up_f16 $s1, $s2, $s3;",
  "mul_ftz_up_f16 $s1, $s2, 1;",
  "mul_ftz_up_f16 $s1, 1, $s3;",
  "mul_ftz_up_f16 $s1, 1, 1;",
  // ftz_up_f32
  "mul_ftz_up_f32 $s1, $s2, $s3;",
  "mul_ftz_up_f32 $s1, $s2, 1;",
  "mul_ftz_up_f32 $s1, 1, $s3;",
  "mul_ftz_up_f32 $s1, 1, 1;",
  // ftz_up_f64
  "mul_ftz_up_f64 $d1, $d2, $d3;",
  "mul_ftz_up_f64 $d1, $d2, 1;",
  "mul_ftz_up_f64 $d1, 1, $d3;",
  "mul_ftz_up_f64 $d1, 1, 1;",
  // ftz_down_f16
  "mul_ftz_down_f16 $s1, $s2, $s3;",
  "mul_ftz_down_f16 $s1, $s2, 1;",
  "mul_ftz_down_f16 $s1, 1, $s3;",
  "mul_ftz_down_f16 $s1, 1, 1;",
  // ftz_down_f32
  "mul_ftz_down_f32 $s1, $s2, $s3;",
  "mul_ftz_down_f32 $s1, $s2, 1;",
  "mul_ftz_down_f32 $s1, 1, $s3;",
  "mul_ftz_down_f32 $s1, 1, 1;",
  // ftz_down_f64
  "mul_ftz_down_f64 $d1, $d2, $d3;",
  "mul_ftz_down_f64 $d1, $d2, 1;",
  "mul_ftz_down_f64 $d1, 1, $d3;",
  "mul_ftz_down_f64 $d1, 1, 1;",
  // f16
  "mul_f16 $s1, $s2, $s3;",
  "mul_f16 $s1, $s2, 1.0;",
  "mul_f16 $s1, 1.0, $s3;",
  "mul_f16 $s1, 1.0, 1.0;",
  // f32
  "mul_f32 $s1, $s2, $s3;",
  "mul_f32 $s1, $s2, 1.0f;",
  "mul_f32 $s1, 1.0f, $s3;",
  "mul_f32 $s1, 1.0f, 1.0f;",
  // f64
  "mul_f64 $d1, $d2, $d3;",
  "mul_f64 $d1, $d2, 1.0;",
  "mul_f64 $d1, 1.0, $d3;",
  "mul_f64 $d1, 1.0, 1.0;"
};
class MulFtzRoundFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_ftz_round_false[] = {
  "mul_ftz_f16 $s1;",
  "mul_ftz_f16 $s1, $s2;",
  "mul_ftz_f16 1, $s2, $s3;",
  "mul_ftz_f16 WAVESIZE, $s2, $s3;",
  "mul_ftz_f16 $s1, WAVESIZE, $s3;",
  "mul_ftz_f16 $s1, $s2,WAVESIZE;",
  "mul_ftz_f16 $s1, 1,WAVESIZE;"
};
BrigAluModifier mul_bam[] = {
  {1,0,0,1,0,0,0},// ftz
  {1,0,0,0,0,0,0},// near
  {1,1,0,0,0,0,0},// zero
  {1,2,0,0,0,0,0},// up
  {1,3,0,0,0,0,0},// down
  {1,1,0,1,0,0,0},// ftz_zero
  {1,2,0,1,0,0,0},// ftz_up
  {1,3,0,1,0,0,0} // ftz_down
};
BrigInstMod output_array_mul_ftz_round[] = {
  // ftz
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[0]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[0]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[0]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[0]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[0]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[0]
  },
  // near
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[1]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[1]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[1]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[1]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[1]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[1]
  },
  // zero
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[2]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[2]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[2]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[2]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[2]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[2]
  },
  // up
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[3]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[3]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[3]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[3]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[3]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[3]
  },
  // down
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[4]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[4]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[4]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[4]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[4]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[4]
  },
  // ftz_zero
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[5]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[5]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[5]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[5]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[5]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[5]
  },
  // ftz_up
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[6]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[6]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[6]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[6]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[6]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[6]
  },
  // ftz_down
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[7]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[7]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[7]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[7]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    mul_bam[7]
  },
  {
    36,
    BrigEInstMod,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    mul_bam[7]
  }
};

BrigInstBase output_array_mul_floating[] = {
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  }
};


class SegpSegmentpb1: public ::testing::TestWithParam<int>{
};
std::string inputarray_segp_segmentpb1[] = {
  //global
  "segmentp_global_b1 $c1,$s2;",
  "segmentp_global_b1 $c1,$d2;",
  "segmentp_global_b1 $c1,61;",
  //group
  "segmentp_group_b1 $c1,$s2;",
  "segmentp_group_b1 $c1,$d2;",
  "segmentp_group_b1 $c1,61;",
  //private
  "segmentp_private_b1 $c1,$s2;",
  "segmentp_private_b1 $c1,$d2;",
  "segmentp_private_b1 $c1,61;",
  //kernarg
  "segmentp_kernarg_b1 $c1,$s2;",
  "segmentp_kernarg_b1 $c1,$d2;",
  "segmentp_kernarg_b1 $c1,61;",
  //readonly
  "segmentp_readonly_b1 $c1,$s2;",
  "segmentp_readonly_b1 $c1,$d2;",
  "segmentp_readonly_b1 $c1,61;",
  //spill
  "segmentp_spill_b1 $c1,$s2;",
  "segmentp_spill_b1 $c1,$d2;",
  "segmentp_spill_b1 $c1,61;",
  //arg
  "segmentp_arg_b1 $c1,$s2;",
  "segmentp_arg_b1 $c1,$d2;",
  "segmentp_arg_b1 $c1,61;"
};

BrigInstMem outputarray_segp_segmentpb1[] = {
  {//0
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGlobalSpace
  },
  {//1
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGroupSpace
  },
  {//2
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigPrivateSpace
  },
  {//3
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigKernargSpace
  },
  {//4
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigReadonlySpace
  },
  {//5
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigSpillSpace
  },
  {//6
    36,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigArgSpace
  }
};


class SegpFtos: public ::testing::TestWithParam<int>
{
};
std::string inputarray_segp_ftos[] = {
  //global u32
  "ftos_global_u32 $d1,$s2;",
  "ftos_global_u32 $d1,61;",
  //group u32
  "ftos_group_u32 $d1,$s2;",
  "ftos_group_u32 $d1,61;",
  //private u32
  "ftos_private_u32 $d1,$s2;",
  "ftos_private_u32 $d1,61;",
  //kernarg u32
  "ftos_kernarg_u32 $d1,$s2;",
  "ftos_kernarg_u32 $d1,61;",
  //readonly u32
  "ftos_readonly_u32 $d1,$s2;",
  "ftos_readonly_u32 $d1,61;",
  //spill u32
  "ftos_spill_u32 $d1,$s2;",
  "ftos_spill_u32 $d1,61;",
  //arg u32
  "ftos_arg_u32 $d1,$s2;",
  "ftos_arg_u32 $d1,61;",
  //global u64
  "ftos_global_u64 $d1,$d2;",
  "ftos_global_u64 $d1,1;",
  //group u64
  "ftos_group_u64 $d1,$d2;",
  "ftos_group_u64 $d1,1;",
  //private u64
  "ftos_private_u64 $d1,$d2;",
  "ftos_private_u64 $d1,1;",
  //kernarg u64
  "ftos_kernarg_u64 $d1,$d2;",
  "ftos_kernarg_u64 $d1,1;",
  //readonly u64
  "ftos_readonly_u64 $d1,$d2;",
  "ftos_readonly_u64 $d1,1;",
  //spill u64
  "ftos_spill_u64 $d1,$d2;",
  "ftos_spill_u64 $d1,1;",
  //arg u64
  "ftos_arg_u64 $d1,$d2;",
  "ftos_arg_u64 $d1,1;"
};

BrigInstMem outputarray_segp_ftos[] = {
  {//0
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGlobalSpace
  },
  {//1
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGroupSpace
  },
  {//2
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigPrivateSpace
  },
  {//3
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigKernargSpace
  },
  {//4
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigReadonlySpace
  },
  {//5
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigSpillSpace
  },
  {//6
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigArgSpace
  },
  {//7
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGlobalSpace
  },
  {//8
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGroupSpace
  },
  {//9
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigPrivateSpace
  },
  {//10
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigKernargSpace
  },
  {//11
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigReadonlySpace
  },
  {//12
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigSpillSpace
  },
  {//13
    36,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigArgSpace
  }
};


class SegpStof : public ::testing::TestWithParam<int>{
};
std::string inputarray_segp_stof[] = {
  //global u32
  "stof_global_u32 $d1,$s2;",
  "stof_global_u32 $d1,61;",
  //group u32
  "stof_group_u32 $d1,$s2;",
  "stof_group_u32 $d1,61;",
  //private u32
  "stof_private_u32 $d1,$s2;",
  "stof_private_u32 $d1,61;",
  //kernarg u32
  "stof_kernarg_u32 $d1,$s2;",
  "stof_kernarg_u32 $d1,61;",
  //readonly u32
  "stof_readonly_u32 $d1,$s2;",
  "stof_readonly_u32 $d1,61;",
  //spill u32
  "stof_spill_u32 $d1,$s2;",
  "stof_spill_u32 $d1,61;",
  //arg u32
  "stof_arg_u32 $d1,$s2;",
  "stof_arg_u32 $d1,61;",
  //global u64
  "stof_global_u64 $d1,$d2;",
  "stof_global_u64 $d1,1;",
  //group u64
  "stof_group_u64 $d1,$d2;",
  "stof_group_u64 $d1,1;",
  //private u64
  "stof_private_u64 $d1,$d2;",
  "stof_private_u64 $d1,1;",
  //kernarg u64
  "stof_kernarg_u64 $d1,$d2;",
  "stof_kernarg_u64 $d1,1;",
  //readonly u64
  "stof_readonly_u64 $d1,$d2;",
  "stof_readonly_u64 $d1,1;",
  //spill u64
  "stof_spill_u64 $d1,$d2;",
  "stof_spill_u64 $d1,1;",
  //arg u64
  "stof_arg_u64 $d1,$d2;",
  "stof_arg_u64 $d1,1;"
};

BrigInstMem outputarray_segp_stof[] = {
  {//0
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGlobalSpace
  },
  {//1
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGroupSpace
  },
  {//2
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigPrivateSpace
  },
  {//3
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigKernargSpace
  },
  {//4
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigReadonlySpace
  },
  {//5
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigSpillSpace
  },
  {//6
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigArgSpace
  },
  {//7
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGlobalSpace
  },
  {//8
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigGroupSpace
  },
  {//9
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigPrivateSpace
  },
  {//10
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigKernargSpace
  },
  {//11
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigReadonlySpace
  },
  {//12
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigSpillSpace
  },
  {//13
    36,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {8,20,0,0,0},
    BrigArgSpace
  }
};

class MulControl : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_control[] = {
  // 32-bit
  "mul_pp_u8x4 $s1, $s2, $s3;",
  "mul_pp_s8x4 $s1, $s2, $s3;",
  "mul_pp_u16x2 $s1, $s2, $s3;",
  "mul_pp_s16x2 $s1, $s2, $s3;",
  "mul_pp_sat_u8x4 $s1, $s2, $s3;",
  "mul_pp_sat_s8x4 $s1, $s2, $s3;",
  "mul_pp_sat_u16x2 $s1, $s2, $s3;",
  "mul_pp_sat_s16x2 $s1, $s2, $s3;",
  "mul_ps_u8x4 $s1, $s2, $s3;",
  "mul_ps_s8x4 $s1, $s2, $s3;",
  "mul_ps_u16x2 $s1, $s2, $s3;",
  "mul_ps_s16x2 $s1, $s2, $s3;",
  "mul_ps_sat_u8x4 $s1, $s2, $s3;",
  "mul_ps_sat_s8x4 $s1, $s2, $s3;",
  "mul_ps_sat_u16x2 $s1, $s2, $s3;",
  "mul_ps_sat_s16x2 $s1, $s2, $s3;",
  "mul_sp_u8x4 $s1, $s2, $s3;",
  "mul_sp_s8x4 $s1, $s2, $s3;",
  "mul_sp_u16x2 $s1, $s2, $s3;",
  "mul_sp_s16x2 $s1, $s2, $s3;",
  "mul_sp_sat_u8x4 $s1, $s2, $s3;",
  "mul_sp_sat_s8x4 $s1, $s2, $s3;",
  "mul_sp_sat_u16x2 $s1, $s2, $s3;",
  "mul_sp_sat_s16x2 $s1, $s2, $s3;",
  "mul_ss_u8x4 $s1, $s2, $s3;",
  "mul_ss_s8x4 $s1, $s2, $s3;",
  "mul_ss_u16x2 $s1, $s2, $s3;",
  "mul_ss_s16x2 $s1, $s2, $s3;",
  "mul_ss_sat_u8x4 $s1, $s2, $s3;",
  "mul_ss_sat_s8x4 $s1, $s2, $s3;",
  "mul_ss_sat_u16x2 $s1, $s2, $s3;",
  "mul_ss_sat_s16x2 $s1, $s2, $s3;",
  "mul_pp_f16x2 $s1, $s2, $s3;",
  "mul_ps_f16x2 $s1, $s2, $s3;",
  "mul_sp_f16x2 $s1, $s2, $s3;",
  "mul_ss_f16x2 $s1, $s2, $s3;",
  // 64-bit
  "mul_pp_u8x8 $d1, $d2, $d3;",
  "mul_pp_s8x8 $d1, $d2, $d3;",
  "mul_pp_u16x4 $d1, $d2, $d3;",
  "mul_pp_s16x4 $d1, $d2, $d3;",
  "mul_pp_sat_u8x8 $d1, $d2, $d3;",
  "mul_pp_sat_s8x8 $d1, $d2, $d3;",
  "mul_pp_sat_u16x4 $d1, $d2, $d3;",
  "mul_pp_sat_s16x4 $d1, $d2, $d3;",
  "mul_ps_u8x8 $d1, $d2, $d3;",
  "mul_ps_s8x8 $d1, $d2, $d3;",
  "mul_ps_u16x4 $d1, $d2, $d3;",
  "mul_ps_s16x4 $d1, $d2, $d3;",
  "mul_ps_sat_u8x8 $d1, $d2, $d3;",
  "mul_ps_sat_s8x8 $d1, $d2, $d3;",
  "mul_ps_sat_u16x4 $d1, $d2, $d3;",
  "mul_ps_sat_s16x4 $d1, $d2, $d3;",
  "mul_sp_u8x8 $d1, $d2, $d3;",
  "mul_sp_s8x8 $d1, $d2, $d3;",
  "mul_sp_u16x4 $d1, $d2, $d3;",
  "mul_sp_s16x4 $d1, $d2, $d3;",
  "mul_sp_sat_u8x8 $d1, $d2, $d3;",
  "mul_sp_sat_s8x8 $d1, $d2, $d3;",
  "mul_sp_sat_u16x4 $d1, $d2, $d3;",
  "mul_sp_sat_s16x4 $d1, $d2, $d3;",
  "mul_ss_u8x8 $d1, $d2, $d3;",
  "mul_ss_s8x8 $d1, $d2, $d3;",
  "mul_ss_u16x4 $d1, $d2, $d3;",
  "mul_ss_s16x4 $d1, $d2, $d3;",
  "mul_ss_sat_u8x8 $d1, $d2, $d3;",
  "mul_ss_sat_s8x8 $d1, $d2, $d3;",
  "mul_ss_sat_u16x4 $d1, $d2, $d3;",
  "mul_ss_sat_s16x4 $d1, $d2, $d3;",
  "mul_pp_f16x4 $d1, $d2, $d3;",
  "mul_pp_f32x2 $d1, $d2, $d3;",
  "mul_ps_f16x4 $d1, $d2, $d3;",
  "mul_ps_f32x2 $d1, $d2, $d3;",
  "mul_sp_f16x4 $d1, $d2, $d3;",
  "mul_sp_f32x2 $d1, $d2, $d3;",
  "mul_ss_f16x4 $d1, $d2, $d3;",
  "mul_ss_f32x2 $d1, $d2, $d3;"
};
BrigInstBase output_array_mul_control[] = {
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x8,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x8,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x4,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x4,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf16x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigf32x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  }
};
class MulControlFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_control_false[] = {
  "mul_pp_u8x4 $s1;",
  "mul_pp_u8x4 $s1, $s2;",
  "mul_pp_u8x4 1;",
  "mul_pp_u8x4 WAVESIZE, $s2;",
  "mul_pp_s8x4 $s1;",
  "mul_pp_s8x4 $s1, $s2;",
  "mul_pp_s8x4 1, $s2, $s3;",
  "mul_pp_s8x4 WAVESIZE, $s2, $s3;",
  "mul_pp_u16x2 $s1;",
  "mul_pp_u16x2 $s1,$s2;",
  "mul_pp_u16x2 1, $s2, $s3;",
  "mul_pp_u16x2 WAVESIZE, $s2, $s3;",
  "mul_pp_s16x2 $s1;",
  "mul_pp_s16x2 $s1, $s2;",
  "mul_pp_s16x2 1, $s2, $s3;",
  "mul_pp_s16x2 WAVESIZE, $s2, $s3;",
  "mul_pp_sat_u8x4 $s1;",
  "mul_pp_sat_u8x4 $s1, $s2;",
  "mul_pp_sat_u8x4 1, $s2, $s3;",
  "mul_pp_sat_u8x4 WAVESIZE, $s2, $s3;",
  "mul_pp_sat_s8x4 $s1;",
  "mul_pp_sat_s8x4 $s1, $s2;",
  "mul_pp_sat_s8x4 1, $s2, $s3;",
  "mul_pp_sat_s8x4 WAVESIZE, $s2, $s3;",
  "mul_pp_sat_u16x2 $s1;",
  "mul_pp_sat_u16x2 $s1, $s2;",
  "mul_pp_sat_u16x2 1, $s2, $s3;",
  "mul_pp_sat_u16x2 WAVESIZE, $s2, $s3;",
  "mul_pp_sat_s16x2 $s1;",
  "mul_pp_sat_s16x2 $s1, $s2;",
  "mul_pp_sat_s16x2 1, $s2, $s3;",
  "mul_pp_sat_s16x2 WAVESIZE, $s2, $s3;",
  "mul_ps_u8x4 $s1;",
  "mul_ps_u8x4 $s1, $s2;",
  "mul_ps_u8x4 1;",
  "mul_ps_u8x4 WAVESIZE, $s2;",
  "mul_ps_s8x4 $s1;",
  "mul_ps_s8x4 $s1, $s2;",
  "mul_ps_s8x4 1, $s2, $s3;",
  "mul_ps_s8x4 WAVESIZE, $s2, $s3;",
  "mul_ps_u16x2 $s1;",
  "mul_ps_u16x2 $s1,$s2;",
  "mul_ps_u16x2 1, $s2, $s3;",
  "mul_ps_u16x2 WAVESIZE, $s2, $s3;",
  "mul_ps_s16x2 $s1;",
  "mul_ps_s16x2 $s1, $s2;",
  "mul_ps_s16x2 1, $s2, $s3;",
  "mul_ps_s16x2 WAVESIZE, $s2, $s3;",
  "mul_ps_sat_u8x4 $s1;",
  "mul_ps_sat_u8x4 $s1, $s2;",
  "mul_ps_sat_u8x4 1, $s2, $s3;",
  "mul_ps_sat_u8x4 WAVESIZE, $s2, $s3;",
  "mul_ps_sat_s8x4 $s1;",
  "mul_ps_sat_s8x4 $s1, $s2;",
  "mul_ps_sat_s8x4 1, $s2, $s3;",
  "mul_ps_sat_s8x4 WAVESIZE, $s2, $s3;",
  "mul_ps_sat_u16x2 $s1;",
  "mul_ps_sat_u16x2 $s1, $s2;",
  "mul_ps_sat_u16x2 1, $s2, $s3;",
  "mul_ps_sat_u16x2 WAVESIZE, $s2, $s3;",
  "mul_ps_sat_s16x2 $s1;",
  "mul_ps_sat_s16x2 $s1, $s2;",
  "mul_ps_sat_s16x2 1, $s2, $s3;",
  "mul_ps_sat_s16x2 WAVESIZE, $s2, $s3;",
  "mul_sp_u8x4 $s1;",
  "mul_sp_u8x4 $s1, $s2;",
  "mul_sp_u8x4 1;",
  "mul_sp_u8x4 WAVESIZE, $s2;",
  "mul_sp_s8x4 $s1;",
  "mul_sp_s8x4 $s1, $s2;",
  "mul_sp_s8x4 1, $s2, $s3;",
  "mul_sp_s8x4 WAVESIZE, $s2, $s3;",
  "mul_sp_u16x2 $s1;",
  "mul_sp_u16x2 $s1,$s2;",
  "mul_sp_u16x2 1, $s2, $s3;",
  "mul_sp_u16x2 WAVESIZE, $s2, $s3;",
  "mul_sp_s16x2 $s1;",
  "mul_sp_s16x2 $s1, $s2;",
  "mul_sp_s16x2 1, $s2, $s3;",
  "mul_sp_s16x2 WAVESIZE, $s2, $s3;",
  "mul_sp_sat_u8x4 $s1;",
  "mul_sp_sat_u8x4 $s1, $s2;",
  "mul_sp_sat_u8x4 1, $s2, $s3;",
  "mul_sp_sat_u8x4 WAVESIZE, $s2, $s3;",
  "mul_sp_sat_s8x4 $s1;",
  "mul_sp_sat_s8x4 $s1, $s2;",
  "mul_sp_sat_s8x4 1, $s2, $s3;",
  "mul_sp_sat_s8x4 WAVESIZE, $s2, $s3;",
  "mul_sp_sat_u16x2 $s1;",
  "mul_sp_sat_u16x2 $s1, $s2;",
  "mul_sp_sat_u16x2 1, $s2, $s3;",
  "mul_sp_sat_u16x2 WAVESIZE, $s2, $s3;",
  "mul_sp_sat_s16x2 $s1;",
  "mul_sp_sat_s16x2 $s1, $s2;",
  "mul_sp_sat_s16x2 1, $s2, $s3;",
  "mul_sp_sat_s16x2 WAVESIZE, $s2, $s3;",
  "mul_ss_u8x4 $s1;",
  "mul_ss_u8x4 $s1, $s2;",
  "mul_ss_u8x4 1;",
  "mul_ss_u8x4 WAVESIZE, $s2;",
  "mul_ss_s8x4 $s1;",
  "mul_ss_s8x4 $s1, $s2;",
  "mul_ss_s8x4 1, $s2, $s3;",
  "mul_ss_s8x4 WAVESIZE, $s2, $s3;",
  "mul_ss_u16x2 $s1;",
  "mul_ss_u16x2 $s1,$s2;",
  "mul_ss_u16x2 1, $s2, $s3;",
  "mul_ss_u16x2 WAVESIZE, $s2, $s3;",
  "mul_ss_s16x2 $s1;",
  "mul_ss_s16x2 $s1, $s2;",
  "mul_ss_s16x2 1, $s2, $s3;",
  "mul_ss_s16x2 WAVESIZE, $s2, $s3;",
  "mul_ss_sat_u8x4 $s1;",
  "mul_ss_sat_u8x4 $s1, $s2;",
  "mul_ss_sat_u8x4 1, $s2, $s3;",
  "mul_ss_sat_u8x4 WAVESIZE, $s2, $s3;",
  "mul_ss_sat_s8x4 $s1;",
  "mul_ss_sat_s8x4 $s1, $s2;",
  "mul_ss_sat_s8x4 1, $s2, $s3;",
  "mul_ss_sat_s8x4 WAVESIZE, $s2, $s3;",
  "mul_ss_sat_u16x2 $s1;",
  "mul_ss_sat_u16x2 $s1, $s2;",
  "mul_ss_sat_u16x2 1, $s2, $s3;",
  "mul_ss_sat_u16x2 WAVESIZE, $s2, $s3;",
  "mul_ss_sat_s16x2 $s1;",
  "mul_ss_sat_s16x2 $s1, $s2;",
  "mul_ss_sat_s16x2 1, $s2, $s3;",
  "mul_ss_sat_s16x2 WAVESIZE, $s2, $s3;",
  "mul_pp_f16x2 $s1;",
  "mul_pp_f16x2 $s1, $s2;",
  "mul_pp_f16x2 1, $s2, $s3;",
  "mul_ps_f16x2 $s1;",
  "mul_ps_f16x2 $s1, $s2;",
  "mul_ps_f16x2 1, $s2, $s3;",
  "mul_sp_f16x2 $s1;",
  "mul_sp_f16x2 $s1, $s2;",
  "mul_sp_f16x2 1, $s2, $s3;",
  "mul_ss_f16x2 $s1;",
  "mul_ss_f16x2 $s1, $s2;",
  "mul_ss_f16x2 1, $s2, $s3;",
  "mul_pp_u8x8 $d1;",
  "mul_pp_s8x8 $d1, $d2;",
  "mul_pp_u16x4 $d1;",
  "mul_pp_s16x4 1, $d2, $d3;",
  "mul_pp_sat_u8x8 $d1;",
  "mul_pp_sat_s8x8 $d1, $d2;",
  "mul_pp_sat_u16x4 1, $d2, $d3;",
  "mul_pp_sat_s16x4 $d1, $d2;",
  "mul_ps_u8x8 $d1;",
  "mul_ps_s8x8 1, $d2, $d3;",
  "mul_ps_u16x4 $d1;",
  "mul_ps_s16x4 $d1, $d2;",
  "mul_ps_sat_u8x8 1, $d2, $d3;",
  "mul_ps_sat_s8x8 $d1;",
  "mul_ps_sat_u16x4 $d1, $d2;",
  "mul_ps_sat_s16x4 1, $d2, $d3;",
  "mul_sp_u8x8 $d1;",
  "mul_sp_s8x8 $d1, $d2;",
  "mul_sp_u16x4 1, $d2, $d3;",
  "mul_sp_s16x4 $d1;",
  "mul_sp_sat_u8x8 $d1, $d2;",
  "mul_sp_sat_s8x8 1, $d2, $d3;",
  "mul_sp_sat_u16x4 $d1;",
  "mul_sp_sat_s16x4 $d1, $d2;",
  "mul_ss_u8x8 1, $d2, $d3;",
  "mul_ss_s8x8 $d1;",
  "mul_ss_u16x4 $d1, $d2;",
  "mul_ss_s16x4 1, $d2, $d3;",
  "mul_ss_sat_u8x8 $d1;",
  "mul_ss_sat_s8x8 $d1, $d2;",
  "mul_ss_sat_u16x4 1, $d2, $d3;",
  "mul_ss_sat_s16x4 $d1;",
  "mul_pp_f16x4 $d1, $d2;",
  "mul_pp_f32x2 1, $d2, $d3;",
  "mul_ps_f16x4 $d1;",
  "mul_ps_f32x2 $d1, $d2;",
  "mul_sp_f16x4 1, $d2, $d3;",
  "mul_sp_f32x2 $d1;",
  "mul_ss_f16x4 $d1, $d2;",
  "mul_ss_f32x2 1, $d2, $d3;"
};
class MulHis32Base : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_s32_base[] = {
  "mul_hi_s32 $s1, $s2, $s3;",
  "mul_hi_s32 $s1, $s2, 1;",
  "mul_hi_s32 $s1, $s2, WAVESIZE;",
  "mul_hi_s32 $s1, 1, $s3;",
  "mul_hi_s32 $s1, 1, 1;",
  "mul_hi_s32 $s1, 1, WAVESIZE;",
  "mul_hi_s32 $s1, WAVESIZE, $s3;",
  "mul_hi_s32 $s1, WAVESIZE, 1;",
  "mul_hi_s32 $s1, WAVESIZE, WAVESIZE;"
};
BrigInstBase output_array_mul_hi_s32_base[] = {
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};
class MulHis32BaseFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_s32_base_false[] = {
  "mul_hi_s32 $s1;",
  "mul_hi_s32 $s1, $s2;",
  "mul_hi_s32 WAVESIZE,$s1, $s2;",
  "mul_hi_s32 1, $s1, $s2;",
  "mul_hi_s32 1, 1, 1;"
};

class MulHiu32Base : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_u32_base[] = {
  "mul_hi_u32 $s1, $s2, $s3;",
  "mul_hi_u32 $s1, $s2, 1;",
  "mul_hi_u32 $s1, $s2, WAVESIZE;",
  "mul_hi_u32 $s1, 1, $s3;",
  "mul_hi_u32 $s1, 1, 1;",
  "mul_hi_u32 $s1, 1, WAVESIZE;",
  "mul_hi_u32 $s1, WAVESIZE, $s3;",
  "mul_hi_u32 $s1, WAVESIZE, 1;",
  "mul_hi_u32 $s1, WAVESIZE, WAVESIZE;"
};
BrigInstBase output_array_mul_hi_u32_base[] = {
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};
class MulHiu32BaseFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_u32_base_false[] = {
  "mul_hi_u32 $s1;",
  "mul_hi_u32 $s1, $s2;",
  "mul_hi_u32 WAVESIZE,$s1, $s2;",
  "mul_hi_u32 1, $s1, $s2;",
  "mul_hi_u32 1, 1, 1;"
};

class MulHiControl : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_control[] = {
  "mul_hi_pp_u8x4 $s1, $s2, $s3;",
  "mul_hi_pp_s8x4 $s1, $s2, $s3;",
  "mul_hi_pp_u16x2 $s1, $s2, $s3;",
  "mul_hi_pp_s16x2 $s1, $s2, $s3;",
  "mul_hi_ps_u8x4 $s1, $s2, $s3;",
  "mul_hi_ps_s8x4 $s1, $s2, $s3;",
  "mul_hi_ps_u16x2 $s1, $s2, $s3;",
  "mul_hi_ps_s16x2 $s1, $s2, $s3;",
  "mul_hi_sp_u8x4 $s1, $s2, $s3;",
  "mul_hi_sp_s8x4 $s1, $s2, $s3;",
  "mul_hi_sp_u16x2 $s1, $s2, $s3;",
  "mul_hi_sp_s16x2 $s1, $s2, $s3;",
  "mul_hi_ss_u8x4 $s1, $s2, $s3;",
  "mul_hi_ss_s8x4 $s1, $s2, $s3;",
  "mul_hi_ss_u16x2 $s1, $s2, $s3;",
  "mul_hi_ss_s16x2 $s1, $s2, $s3;"
};
BrigInstBase output_array_mul_hi_control[] = {
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu8x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs8x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigu16x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul,
    Brigs16x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  }
};
class MulHiControlFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul_hi_control_false[] = {
  "mul_hi_pp_u8x4 $s1;",
  "mul_hi_pp_s8x4 $s1, $s2;",
  "mul_hi_pp_u16x2 $s2, $s3;",
  "mul_hi_pp_s16x2 $s3;",
  "mul_hi_ps_u8x4 $s1;",
  "mul_hi_ps_s8x4 $s1, $s2;",
  "mul_hi_ps_u16x2 $s1;",
  "mul_hi_ps_s16x2 $s1;",
  "mul_hi_sp_u8x4 $s1, $s2;",
  "mul_hi_sp_s8x4 $s1;",
  "mul_hi_sp_u16x2 $s1, $s2;",
  "mul_hi_sp_s16x2 $s1;",
  "mul_hi_ss_u8x4 $s1;",
  "mul_hi_ss_s8x4 $s1;",
  "mul_hi_ss_u16x2 1, $s2, $s3;",
  "mul_hi_ss_s16x2 WAVESIZE, $s2, $s3;"
};

class Mul24 : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul24[] = {
  // s32
  "mul24_s32 $s1, $s2, $s3;",
  "mul24_s32 $s1, $s2, 1;",
  "mul24_s32 $s1, $s2, WAVESIZE;",
  "mul24_s32 $s1, 1, $s3;",
  "mul24_s32 $s1, 1, 1;",
  "mul24_s32 $s1, 1, WAVESIZE;",
  "mul24_s32 $s1, WAVESIZE, $s3;",
  "mul24_s32 $s1, WAVESIZE, 1;",
  "mul24_s32 $s1, WAVESIZE, WAVESIZE;",
  // s64
  "mul24_s64 $d1, $d2, $d3;",
  "mul24_s64 $d1, $d2, 1;",
  "mul24_s64 $d1, $d2, WAVESIZE;",
  "mul24_s64 $d1, 1, $d3;",
  "mul24_s64 $d1, 1, 1;",
  "mul24_s64 $d1, 1, WAVESIZE;",
  "mul24_s64 $d1, WAVESIZE, $d3;",
  "mul24_s64 $d1, WAVESIZE, 1;",
  "mul24_s64 $d1, WAVESIZE, WAVESIZE;",
  // u32
  "mul24_u32 $s1, $s2, $s3;",
  "mul24_u32 $s1, $s2, 1;",
  "mul24_u32 $s1, $s2, WAVESIZE;",
  "mul24_u32 $s1, 1, $s3;",
  "mul24_u32 $s1, 1, 1;",
  "mul24_u32 $s1, 1, WAVESIZE;",
  "mul24_u32 $s1, WAVESIZE, $s3;",
  "mul24_u32 $s1, WAVESIZE, 1;",
  "mul24_u32 $s1, WAVESIZE, WAVESIZE;",
  // u64
  "mul24_u64 $d1, $d2, $d3;",
  "mul24_u64 $d1, $d2, 1;",
  "mul24_u64 $d1, $d2, WAVESIZE;",
  "mul24_u64 $d1, 1, $d3;",
  "mul24_u64 $d1, 1, 1;",
  "mul24_u64 $d1, 1, WAVESIZE;",
  "mul24_u64 $d1, WAVESIZE, $d3;",
  "mul24_u64 $d1, WAVESIZE, 1;",
  "mul24_u64 $d1, WAVESIZE, WAVESIZE;"
};

BrigInstBase output_array_mul24[] = {
  // s32
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // s64
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u32
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u64
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};
class Mul24False : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul24_false[] = {
  "mul24_s32 $s1;",
  "mul24_s64 $s1, $s2;",
  "mul24_u32 1, 1, $s3;",
  "mul24_u64 WAVESIZE, 1, 1;"
};

class Mul24Hi : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul24_hi[] = {
  // s32
  "mul24_hi_s32 $s1, $s2, $s3;",
  "mul24_hi_s32 $s1, $s2, 1;",
  "mul24_hi_s32 $s1, $s2, WAVESIZE;",
  "mul24_hi_s32 $s1, 1, $s3;",
  "mul24_hi_s32 $s1, 1, 1;",
  "mul24_hi_s32 $s1, 1, WAVESIZE;",
  "mul24_hi_s32 $s1, WAVESIZE, $s3;",
  "mul24_hi_s32 $s1, WAVESIZE, 1;",
  "mul24_hi_s32 $s1, WAVESIZE, WAVESIZE;",
  // s64
  "mul24_hi_s64 $d1, $d2, $d3;",
  "mul24_hi_s64 $d1, $d2, 1;",
  "mul24_hi_s64 $d1, $d2, WAVESIZE;",
  "mul24_hi_s64 $d1, 1, $d3;",
  "mul24_hi_s64 $d1, 1, 1;",
  "mul24_hi_s64 $d1, 1, WAVESIZE;",
  "mul24_hi_s64 $d1, WAVESIZE, $d3;",
  "mul24_hi_s64 $d1, WAVESIZE, 1;",
  "mul24_hi_s64 $d1, WAVESIZE, WAVESIZE;",
  // u32
  "mul24_hi_u32 $s1, $s2, $s3;",
  "mul24_hi_u32 $s1, $s2, 1;",
  "mul24_hi_u32 $s1, $s2, WAVESIZE;",
  "mul24_hi_u32 $s1, 1, $s3;",
  "mul24_hi_u32 $s1, 1, 1;",
  "mul24_hi_u32 $s1, 1, WAVESIZE;",
  "mul24_hi_u32 $s1, WAVESIZE, $s3;",
  "mul24_hi_u32 $s1, WAVESIZE, 1;",
  "mul24_hi_u32 $s1, WAVESIZE, WAVESIZE;",
  // u64
  "mul24_hi_u64 $d1, $d2, $d3;",
  "mul24_hi_u64 $d1, $d2, 1;",
  "mul24_hi_u64 $d1, $d2, WAVESIZE;",
  "mul24_hi_u64 $d1, 1, $d3;",
  "mul24_hi_u64 $d1, 1, 1;",
  "mul24_hi_u64 $d1, 1, WAVESIZE;",
  "mul24_hi_u64 $d1, WAVESIZE, $d3;",
  "mul24_hi_u64 $d1, WAVESIZE, 1;",
  "mul24_hi_u64 $d1, WAVESIZE, WAVESIZE;"
};

BrigInstBase output_array_mul24_hi[] = {
  // s32
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // s64
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u32
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  // u64
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMul24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};
class Mul24HiFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mul24_hi_false[] = {
  "mul24_hi_s32 $s1;",
  "mul24_hi_s64 $s1, $s2;",
  "mul24_hi_u32 1, 1, $s3;",
  "mul24_hi_u64 WAVESIZE, 1, 1;"
};

class Mad24 : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad24[] = {
  // s32
  "mad24_s32 $s1, $s2, $s3, $s4;",
  "mad24_s32 $s1, $s2, $s3, 1;",
  "mad24_s32 $s1, $s2, $s3, WAVESIZE;",
  "mad24_s32 $s1, $s2, 1, $s4;",
  "mad24_s32 $s1, $s2, 1, 1;",
  "mad24_s32 $s1, $s2, 1, WAVESIZE;",
  "mad24_s32 $s1, $s2, WAVESIZE, $s4;",
  "mad24_s32 $s1, $s2, WAVESIZE, 1;",
  "mad24_s32 $s1, $s2, WAVESIZE, WAVESIZE;",
  "mad24_s32 $s1, 1, $s3, $s4;",
  "mad24_s32 $s1, 1, $s3, 1;",
  "mad24_s32 $s1, 1, $s3, WAVESIZE;",
  "mad24_s32 $s1, 1, 1, $s4;",
  "mad24_s32 $s1, 1, 1, 1;",
  "mad24_s32 $s1, 1, 1, WAVESIZE;",
  "mad24_s32 $s1, 1, WAVESIZE, $s4;",
  "mad24_s32 $s1, 1, WAVESIZE, 1;",
  "mad24_s32 $s1, 1, WAVESIZE, WAVESIZE;",
  "mad24_s32 $s1, WAVESIZE, $s3, $s4;",
  "mad24_s32 $s1, WAVESIZE, $s3, 1;",
  "mad24_s32 $s1, WAVESIZE, $s3, WAVESIZE;",
  "mad24_s32 $s1, WAVESIZE, 1, $s4;",
  "mad24_s32 $s1, WAVESIZE, 1, 1;",
  "mad24_s32 $s1, WAVESIZE, 1, WAVESIZE;",
  "mad24_s32 $s1, WAVESIZE, WAVESIZE, $s4;",
  "mad24_s32 $s1, WAVESIZE, WAVESIZE, 1;",
  "mad24_s32 $s1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // s64
  "mad24_s64 $d1, $d2, $d3, $d4;",
  "mad24_s64 $d1, $d2, $d3, 1;",
  "mad24_s64 $d1, $d2, $d3, WAVESIZE;",
  "mad24_s64 $d1, $d2, 1, $d4;",
  "mad24_s64 $d1, $d2, 1, 1;",
  "mad24_s64 $d1, $d2, 1, WAVESIZE;",
  "mad24_s64 $d1, $d2, WAVESIZE, $d4;",
  "mad24_s64 $d1, $d2, WAVESIZE, 1;",
  "mad24_s64 $d1, $d2, WAVESIZE, WAVESIZE;",
  "mad24_s64 $d1, 1, $d3, $d4;",
  "mad24_s64 $d1, 1, $d3, 1;",
  "mad24_s64 $d1, 1, $d3, WAVESIZE;",
  "mad24_s64 $d1, 1, 1, $d4;",
  "mad24_s64 $d1, 1, 1, 1;",
  "mad24_s64 $d1, 1, 1, WAVESIZE;",
  "mad24_s64 $d1, 1, WAVESIZE, $d4;",
  "mad24_s64 $d1, 1, WAVESIZE, 1;",
  "mad24_s64 $d1, 1, WAVESIZE, WAVESIZE;",
  "mad24_s64 $d1, WAVESIZE, $d3, $d4;",
  "mad24_s64 $d1, WAVESIZE, $d3, 1;",
  "mad24_s64 $d1, WAVESIZE, $d3, WAVESIZE;",
  "mad24_s64 $d1, WAVESIZE, 1, $d4;",
  "mad24_s64 $d1, WAVESIZE, 1, 1;",
  "mad24_s64 $d1, WAVESIZE, 1, WAVESIZE;",
  "mad24_s64 $d1, WAVESIZE, WAVESIZE, $d4;",
  "mad24_s64 $d1, WAVESIZE, WAVESIZE, 1;",
  "mad24_s64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // u32
  "mad24_u32 $s1, $s2, $s3, $s4;",
  "mad24_u32 $s1, $s2, $s3, 1;",
  "mad24_u32 $s1, $s2, $s3, WAVESIZE;",
  "mad24_u32 $s1, $s2, 1, $s4;",
  "mad24_u32 $s1, $s2, 1, 1;",
  "mad24_u32 $s1, $s2, 1, WAVESIZE;",
  "mad24_u32 $s1, $s2, WAVESIZE, $s4;",
  "mad24_u32 $s1, $s2, WAVESIZE, 1;",
  "mad24_u32 $s1, $s2, WAVESIZE, WAVESIZE;",
  "mad24_u32 $s1, 1, $s3, $s4;",
  "mad24_u32 $s1, 1, $s3, 1;",
  "mad24_u32 $s1, 1, $s3, WAVESIZE;",
  "mad24_u32 $s1, 1, 1, $s4;",
  "mad24_u32 $s1, 1, 1, 1;",
  "mad24_u32 $s1, 1, 1, WAVESIZE;",
  "mad24_u32 $s1, 1, WAVESIZE, $s4;",
  "mad24_u32 $s1, 1, WAVESIZE, 1;",
  "mad24_u32 $s1, 1, WAVESIZE, WAVESIZE;",
  "mad24_u32 $s1, WAVESIZE, $s3, $s4;",
  "mad24_u32 $s1, WAVESIZE, $s3, 1;",
  "mad24_u32 $s1, WAVESIZE, $s3, WAVESIZE;",
  "mad24_u32 $s1, WAVESIZE, 1, $s4;",
  "mad24_u32 $s1, WAVESIZE, 1, 1;",
  "mad24_u32 $s1, WAVESIZE, 1, WAVESIZE;",
  "mad24_u32 $s1, WAVESIZE, WAVESIZE, $s4;",
  "mad24_u32 $s1, WAVESIZE, WAVESIZE, 1;",
  "mad24_u32 $s1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // u64
  "mad24_u64 $d1, $d2, $d3, $d4;",
  "mad24_u64 $d1, $d2, $d3, 1;",
  "mad24_u64 $d1, $d2, $d3, WAVESIZE;",
  "mad24_u64 $d1, $d2, 1, $d4;",
  "mad24_u64 $d1, $d2, 1, 1;",
  "mad24_u64 $d1, $d2, 1, WAVESIZE;",
  "mad24_u64 $d1, $d2, WAVESIZE, $d4;",
  "mad24_u64 $d1, $d2, WAVESIZE, 1;",
  "mad24_u64 $d1, $d2, WAVESIZE, WAVESIZE;",
  "mad24_u64 $d1, 1, $d3, $d4;",
  "mad24_u64 $d1, 1, $d3, 1;",
  "mad24_u64 $d1, 1, $d3, WAVESIZE;",
  "mad24_u64 $d1, 1, 1, $d4;",
  "mad24_u64 $d1, 1, 1, 1;",
  "mad24_u64 $d1, 1, 1, WAVESIZE;",
  "mad24_u64 $d1, 1, WAVESIZE, $d4;",
  "mad24_u64 $d1, 1, WAVESIZE, 1;",
  "mad24_u64 $d1, 1, WAVESIZE, WAVESIZE;",
  "mad24_u64 $d1, WAVESIZE, $d3, $d4;",
  "mad24_u64 $d1, WAVESIZE, $d3, 1;",
  "mad24_u64 $d1, WAVESIZE, $d3, WAVESIZE;",
  "mad24_u64 $d1, WAVESIZE, 1, $d4;",
  "mad24_u64 $d1, WAVESIZE, 1, 1;",
  "mad24_u64 $d1, WAVESIZE, 1, WAVESIZE;",
  "mad24_u64 $d1, WAVESIZE, WAVESIZE, $d4;",
  "mad24_u64 $d1, WAVESIZE, WAVESIZE, 1;",
  "mad24_u64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;",
};
BrigInstBase output_array_mad24[] = {
  // s32
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // s64
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // u32
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // u64
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  }
};
class Mad24False : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad24_false[] = {
  "mad24_s32 $s1;",
  "mad24_s64 $s1, $s2;",
  "mad24_u32 1, 1, $s3;",
  "mad24_u64 WAVESIZE, 1, 1;"
};

class Mad24Hi : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad24_hi[] = {
  // s32
  "mad24_hi_s32 $s1, $s2, $s3, $s4;",
  "mad24_hi_s32 $s1, $s2, $s3, 1;",
  "mad24_hi_s32 $s1, $s2, $s3, WAVESIZE;",
  "mad24_hi_s32 $s1, $s2, 1, $s4;",
  "mad24_hi_s32 $s1, $s2, 1, 1;",
  "mad24_hi_s32 $s1, $s2, 1, WAVESIZE;",
  "mad24_hi_s32 $s1, $s2, WAVESIZE, $s4;",
  "mad24_hi_s32 $s1, $s2, WAVESIZE, 1;",
  "mad24_hi_s32 $s1, $s2, WAVESIZE, WAVESIZE;",
  "mad24_hi_s32 $s1, 1, $s3, $s4;",
  "mad24_hi_s32 $s1, 1, $s3, 1;",
  "mad24_hi_s32 $s1, 1, $s3, WAVESIZE;",
  "mad24_hi_s32 $s1, 1, 1, $s4;",
  "mad24_hi_s32 $s1, 1, 1, 1;",
  "mad24_hi_s32 $s1, 1, 1, WAVESIZE;",
  "mad24_hi_s32 $s1, 1, WAVESIZE, $s4;",
  "mad24_hi_s32 $s1, 1, WAVESIZE, 1;",
  "mad24_hi_s32 $s1, 1, WAVESIZE, WAVESIZE;",
  "mad24_hi_s32 $s1, WAVESIZE, $s3, $s4;",
  "mad24_hi_s32 $s1, WAVESIZE, $s3, 1;",
  "mad24_hi_s32 $s1, WAVESIZE, $s3, WAVESIZE;",
  "mad24_hi_s32 $s1, WAVESIZE, 1, $s4;",
  "mad24_hi_s32 $s1, WAVESIZE, 1, 1;",
  "mad24_hi_s32 $s1, WAVESIZE, 1, WAVESIZE;",
  "mad24_hi_s32 $s1, WAVESIZE, WAVESIZE, $s4;",
  "mad24_hi_s32 $s1, WAVESIZE, WAVESIZE, 1;",
  "mad24_hi_s32 $s1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // s64
  "mad24_hi_s64 $d1, $d2, $d3, $d4;",
  "mad24_hi_s64 $d1, $d2, $d3, 1;",
  "mad24_hi_s64 $d1, $d2, $d3, WAVESIZE;",
  "mad24_hi_s64 $d1, $d2, 1, $d4;",
  "mad24_hi_s64 $d1, $d2, 1, 1;",
  "mad24_hi_s64 $d1, $d2, 1, WAVESIZE;",
  "mad24_hi_s64 $d1, $d2, WAVESIZE, $d4;",
  "mad24_hi_s64 $d1, $d2, WAVESIZE, 1;",
  "mad24_hi_s64 $d1, $d2, WAVESIZE, WAVESIZE;",
  "mad24_hi_s64 $d1, 1, $d3, $d4;",
  "mad24_hi_s64 $d1, 1, $d3, 1;",
  "mad24_hi_s64 $d1, 1, $d3, WAVESIZE;",
  "mad24_hi_s64 $d1, 1, 1, $d4;",
  "mad24_hi_s64 $d1, 1, 1, 1;",
  "mad24_hi_s64 $d1, 1, 1, WAVESIZE;",
  "mad24_hi_s64 $d1, 1, WAVESIZE, $d4;",
  "mad24_hi_s64 $d1, 1, WAVESIZE, 1;",
  "mad24_hi_s64 $d1, 1, WAVESIZE, WAVESIZE;",
  "mad24_hi_s64 $d1, WAVESIZE, $d3, $d4;",
  "mad24_hi_s64 $d1, WAVESIZE, $d3, 1;",
  "mad24_hi_s64 $d1, WAVESIZE, $d3, WAVESIZE;",
  "mad24_hi_s64 $d1, WAVESIZE, 1, $d4;",
  "mad24_hi_s64 $d1, WAVESIZE, 1, 1;",
  "mad24_hi_s64 $d1, WAVESIZE, 1, WAVESIZE;",
  "mad24_hi_s64 $d1, WAVESIZE, WAVESIZE, $d4;",
  "mad24_hi_s64 $d1, WAVESIZE, WAVESIZE, 1;",
  "mad24_hi_s64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // u32
  "mad24_hi_u32 $s1, $s2, $s3, $s4;",
  "mad24_hi_u32 $s1, $s2, $s3, 1;",
  "mad24_hi_u32 $s1, $s2, $s3, WAVESIZE;",
  "mad24_hi_u32 $s1, $s2, 1, $s4;",
  "mad24_hi_u32 $s1, $s2, 1, 1;",
  "mad24_hi_u32 $s1, $s2, 1, WAVESIZE;",
  "mad24_hi_u32 $s1, $s2, WAVESIZE, $s4;",
  "mad24_hi_u32 $s1, $s2, WAVESIZE, 1;",
  "mad24_hi_u32 $s1, $s2, WAVESIZE, WAVESIZE;",
  "mad24_hi_u32 $s1, 1, $s3, $s4;",
  "mad24_hi_u32 $s1, 1, $s3, 1;",
  "mad24_hi_u32 $s1, 1, $s3, WAVESIZE;",
  "mad24_hi_u32 $s1, 1, 1, $s4;",
  "mad24_hi_u32 $s1, 1, 1, 1;",
  "mad24_hi_u32 $s1, 1, 1, WAVESIZE;",
  "mad24_hi_u32 $s1, 1, WAVESIZE, $s4;",
  "mad24_hi_u32 $s1, 1, WAVESIZE, 1;",
  "mad24_hi_u32 $s1, 1, WAVESIZE, WAVESIZE;",
  "mad24_hi_u32 $s1, WAVESIZE, $s3, $s4;",
  "mad24_hi_u32 $s1, WAVESIZE, $s3, 1;",
  "mad24_hi_u32 $s1, WAVESIZE, $s3, WAVESIZE;",
  "mad24_hi_u32 $s1, WAVESIZE, 1, $s4;",
  "mad24_hi_u32 $s1, WAVESIZE, 1, 1;",
  "mad24_hi_u32 $s1, WAVESIZE, 1, WAVESIZE;",
  "mad24_hi_u32 $s1, WAVESIZE, WAVESIZE, $s4;",
  "mad24_hi_u32 $s1, WAVESIZE, WAVESIZE, 1;",
  "mad24_hi_u32 $s1, WAVESIZE, WAVESIZE, WAVESIZE;",
  // u64
  "mad24_hi_u64 $d1, $d2, $d3, $d4;",
  "mad24_hi_u64 $d1, $d2, $d3, 1;",
  "mad24_hi_u64 $d1, $d2, $d3, WAVESIZE;",
  "mad24_hi_u64 $d1, $d2, 1, $d4;",
  "mad24_hi_u64 $d1, $d2, 1, 1;",
  "mad24_hi_u64 $d1, $d2, 1, WAVESIZE;",
  "mad24_hi_u64 $d1, $d2, WAVESIZE, $d4;",
  "mad24_hi_u64 $d1, $d2, WAVESIZE, 1;",
  "mad24_hi_u64 $d1, $d2, WAVESIZE, WAVESIZE;",
  "mad24_hi_u64 $d1, 1, $d3, $d4;",
  "mad24_hi_u64 $d1, 1, $d3, 1;",
  "mad24_hi_u64 $d1, 1, $d3, WAVESIZE;",
  "mad24_hi_u64 $d1, 1, 1, $d4;",
  "mad24_hi_u64 $d1, 1, 1, 1;",
  "mad24_hi_u64 $d1, 1, 1, WAVESIZE;",
  "mad24_hi_u64 $d1, 1, WAVESIZE, $d4;",
  "mad24_hi_u64 $d1, 1, WAVESIZE, 1;",
  "mad24_hi_u64 $d1, 1, WAVESIZE, WAVESIZE;",
  "mad24_hi_u64 $d1, WAVESIZE, $d3, $d4;",
  "mad24_hi_u64 $d1, WAVESIZE, $d3, 1;",
  "mad24_hi_u64 $d1, WAVESIZE, $d3, WAVESIZE;",
  "mad24_hi_u64 $d1, WAVESIZE, 1, $d4;",
  "mad24_hi_u64 $d1, WAVESIZE, 1, 1;",
  "mad24_hi_u64 $d1, WAVESIZE, 1, WAVESIZE;",
  "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, $d4;",
  "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, 1;",
  "mad24_hi_u64 $d1, WAVESIZE, WAVESIZE, WAVESIZE;"
};
BrigInstBase output_array_mad24_hi[] = {
  // s32
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // s64
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // u32
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  },
  // u64
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 44, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 56, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 68, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 36, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 48, 0}
  },
  {
    32,
    BrigEInstBase,
    BrigMad24,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 28, 0}
  }
};
class Mad24HiFalse : public ::testing::TestWithParam<int>
{

};
std::string input_array_mad24_hi_false[] = {
  "mad24_hi_s32 $s1;",
  "mad24_hi_s64 $s1, $s2;",
  "mad24_hi_u32 1, 1, $s3;",
  "mad24_hi_u64 WAVESIZE, 1, 1;"
};


class Instruction3OpAddInt: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_add_int[] = {
//src0:reg u32
  "add_u32 $s1, $s2, $s3;",
  //"add_u32 $s1, $s2, 1;",
  "add_u32 $s1, $s2, WAVESIZE;",
  //src0:reg s32
  "add_s32 $s1, $s2, $s3;",
  //"add_s32 $s1, $s2, 1;",
  "add_s32 $s1, $s2, WAVESIZE;",
  //src0:reg u64	
  "add_u64 $d1, $d2, $d3;",
  //"add_u64 $d1, $d2, 1;",
  "add_u64 $d1, $d2, WAVESIZE;",
  //src0:reg s64
  "add_s64 $d1, $d2, $d3;",
  //"add_s64 $d1, $d2, 1;",
  "add_s64 $d1, $d2, WAVESIZE;",
  //src0:immed value u32
  //"add_u32 $s1, 61, $s3;",
  //"add_u32 $s1, 61, 1;",
  //"add_u32 $s1, 61, WAVESIZE;",
  //src0:immed value s32
  //"add_s32 $d1, 61, $d3;",
  //"add_s32 $s1, 61, 1;",
  //"add_s32 $s1, 61, WAVESIZE;",
  //src0:immed value u64
  //"add_u64 $d1, 61, $d3;",
  //"add_u64 $d1, 61, 1;",
  //"add_u64 $d1, 61, WAVESIZE;",
  //src0:immed value s64
  //"add_s64 $d1, 61, $d3;",
  //"add_s64 $d1, 61, 1;",
  //"add_s64 $d1, 61, WAVESIZE;",
  //src0:wavesize u32
  "add_u32 $s1, WAVESIZE, $s3;",
  //"add_u32 $s1, WAVESIZE, 61;",
  "add_u32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s32
  "add_s32 $s1, WAVESIZE, $s3;",
  //"add_s32 $s1, WAVESIZE, 61;",
  "add_s32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize u64
  "add_u64 $d1, WAVESIZE, $d3;",
  //"add_u64 $d1, WAVESIZE, 61;",
  "add_u64 $d1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s64
  "add_s64 $d1, WAVESIZE, $d3;",
  //"add_s64 $d1, WAVESIZE, 61;",
  "add_s64 $d1, WAVESIZE, WAVESIZE;"
};

BrigInstBase outputarray_instruction3op_add_int[] = {
 {//0
    32,                                         //src0:reg u32
    BrigEInstBase,
    BrigAdd,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,                                         //src0:reg s32
    BrigEInstBase,
    BrigAdd,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,                                         //src0:reg u64
    BrigEInstBase,
    BrigAdd,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//3
    32,                                         //src0:reg s64
    BrigEInstBase,
    BrigAdd,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },/*
  {//4
    32,                                         //src0:immed value u32
    BrigEInstBase,
    BrigAdd,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//5
    32,                                         //src0:immed value s32
    BrigEInstBase,
    BrigAdd,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//6
    32,                                         //src0:immed value u64
    BrigEInstBase,
    BrigAdd,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//7
    32,                                         //src0:immed value s64
    BrigEInstBase,
    BrigAdd,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },*/
  {//8
    32,                                         //src0:wavesize u32
    BrigEInstBase,
    BrigAdd,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//9
    32,                                          //src0:wavesize s32
    BrigEInstBase,
    BrigAdd,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//10
    32,                                          //src0:wavesize u64
    BrigEInstBase,
    BrigAdd,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//11
    32,                                          //src0:wavesize s64
    BrigEInstBase,
    BrigAdd,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};

class Instruction3OpAddPackedint: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_add_packed_int[] = {
  "add_pp_u8x4 $s1, $s2, $s3;",
  "add_pp_sat_s8x4 $s1, $s2, $s3;",
  "add_ps_u16x2 $s1, $s2, $s3;",
  "add_ps_sat_s16x2 $s1, $s2, $s3;",
  "add_sp_u8x8 $d1, $d2, $d3;",
  "add_sp_sat_s8x8 $d1, $d2, $d3;",
  "add_ss_u16x4 $d1, $d2, $d3;",
  "add_ss_sat_s16x4 $d1, $d2, $d3;"/*,
  "add_ss_u32x2 $d1, $d2, $d3;",
  "add_ss_sat_s32x2 $d1, $d2, $d3;"*/
};

BrigInstBase outputarray_instruction3op_add_packed_int[] = {
  {//0
    32,                         //pp_u8x4
    BrigEInstBase,
    BrigAdd,
    Brigu8x4,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,                         //pp_sat_s8x4
    BrigEInstBase,
    BrigAdd,
    Brigs8x4,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,                         //ps_u16x2
    BrigEInstBase,
    BrigAdd,
    Brigu16x2,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {//3
    32,                         //ps_sat_s16x2
    BrigEInstBase,
    BrigAdd,
    Brigs16x2,
    BrigPackPSsat,
    {8, 20, 32, 0, 0}
  },
  {//4
    32,                         //sp_u8x8
    BrigEInstBase,
    BrigAdd,
    Brigu8x8,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {//5
    32,                         //sp_sat_s8x8
    BrigEInstBase,
    BrigAdd,
    Brigs8x8,
    BrigPackSPsat,
    {8, 20, 32, 0, 0}
  },
  {//6
    32,                         //ss_u16x4
    BrigEInstBase,
    BrigAdd,
    Brigu16x4,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {//7
    32,                         //ss_sat_s16x4
    BrigEInstBase,
    BrigAdd,
    Brigs16x4,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  }/*,
  {//8
    32,                         //ss_u32x2
    BrigEInstBase,
    BrigAdd,
    Brigu32x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  },
  {//9
    32,                         //ss_sat_s32x2
    BrigEInstBase,
    BrigAdd,
    Brigs32x2,
    BrigPackSSsat,
    {8, 20, 32, 0, 0}
  }*/
};


class Instruction3OpAddFloat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_add_float[] = {
  //src0:reg f16
  "add_f16 $s1, $s2, $s3;",
  "add_near_f16 $s1, $s2, $s3;",
  //"add_f16 $s1, $s2, 0.2;", 
  //"add_near_f16 $s1, $s2, 0.2;",
  //src0:reg f32
  "add_f32 $s1, $s2, $s3;",
  "add_near_f32 $s1, $s2, $s3;",
  //"add_f32 $s1, $s2, 0.2;", 
  //"add_near_f32 $s1, $s2, 0.2;",
  //src0:reg f64
  "add_f64 $d1, $d2, $d3;",
  "add_near_f64 $d1, $d2, $d3;"//,
  //"add_f64 $d1, $d2, 0.2;", 
  //"add_near_f64 $d1, $d2, 0.2;",
  //src0:immed f16
  //"add_f16 $s1, 0.1, $s3;",
  //"add_near_f16 $s1, 0.1, $s3;",
  //"add_f16 $s1, 0.1, 0.2;", 
  //"add_near_f16 $s1, 0.1, 0.2;",
  //src0:immed f32
  //"add_f32 $s1, 0.1, $s3;",
  //"add_near_f32 $s1, 0.1, $s3;",
  //"add_f32 $s1, 0.1, 0.2;", 
  //"add_near_f32 $s1, 0.1, 0.2;",
  //src0:immed f64
  //"add_f64 $d1, 0.1, $d3;",
  //"add_near_f64 $d1, 0.1, $d3;",
  //"add_f64 $d1, 0.1, 0.2;",
  //"add_near_f64 $d1, 0.1, 0.2;"
};

BrigInstBase outputarray_instruction3op_add_float[] = {
  {//0
    32,	                                               //src0:reg f16
    BrigEInstBase,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,	                                                //src0:reg f32
    BrigEInstBase,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,	                                                //src0:reg f64
    BrigEInstBase,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  }/*,
  {//3
    32,	                                                //src0:immed f16
    BrigEInstBase,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//4
    32,	                                                //src0:immed f32
    BrigEInstBase,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//5
    32,	                                                //src0:immed f64
    BrigEInstBase,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  }*/
};


class Instruction3OpAddFtzRoundFloat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_add_ftz_round_float[] = {
  //src0:reg f16 noftz zero
  "add_zero_f16 $s1, $s2, $s3;",//0
  //"add_zero_f16 $s1, $s2, 0.2;",
  //src0:reg f32 noftz zero
  "add_zero_f32 $s1, $s2, $s3;",
  //"add_zero_f32 $s1, $s2, 0.2;",
  //src0:reg f64 noftz zero
  "add_zero_f64 $d1, $d2, $d3;",
  //"add_zero_f64 $d1, $d2, 0.2;",
  //src0:immed f16 noftz zero
  //"add_zero_f16 $s1, 0.1, $s3;",//6
  //"add_zero_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 noftz zero
  //"add_zero_f32 $s1, 0.1, $s3;",
  //"add_zero_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 noftz zero
  //"add_zero_f64 $d1, 0.1, $d3;",
  //"add_zero_f64 $d1, 0.1, 0.2;",
  //src0:reg f16 noftz up
  "add_up_f16 $s1, $s2, $s3;",//12
  //"add_up_f16 $s1, $s2, 0.2;",
  //src0:reg f32 noftz up
  "add_up_f32 $s1, $s2, $s3;",
  //"add_up_f32 $s1, $s2, 0.2;",
  //src0:reg f64 noftz up
  "add_up_f64 $d1, $d2, $d3;",
  //"add_up_f64 $d1, $d2, 0.2;",
  //src0:immed f16 noftz up
  //"add_up_f16 $s1, 0.1, $s3;",//18
  //"add_up_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 noftz up
  //"add_up_f32 $s1, 0.1, $s3;",
  //"add_up_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 noftz up
  //"add_up_f64 $d1, 0.1, $d3;",
  //"add_up_f64 $d1, 0.1, 0.2;",
  //src0:reg f16 noftz down
  "add_down_f16 $s1, $s2, $s3;",//24
  //"add_down_f16 $s1, $s2, 0.2;", 
  //src0:reg f32 noftz down
  "add_down_f32 $s1, $s2, $s3;",
  //"add_down_f32 $s1, $s2, 0.2;", 
  //src0:reg f64 noftz down
  "add_down_f64 $d1, $d2, $d3;",
  //"add_down_f64 $d1, $d2, 0.2;", 
  //src0:immed f16 noftz down
  //"add_down_f16 $s1, 0.1, $s3;",//30
  //"add_down_f16 $s1, 0.1, 0.2;", 
  //src0:immed f32 noftz down
  //"add_down_f32 $s1, 0.1, $s3;",
  //"add_down_f32 $s1, 0.1, 0.2;", 
  //src0:immed f64 noftz down
  //"add_down_f64 $d1, 0.1, $d3;",
  //"add_down_f64 $d1, 0.1, 0.2;", 
  //src0:reg f16 ftz near
  "add_ftz_f16 $s1, $s2, $s3;",//36
  "add_ftz_near_f16 $s1, $s2, $s3;",
  //"add_ftz_f16 $s1, $s2, 0.2;",
  //"add_ftz_near_f16 $s1, $s2, 0.2;",
  //src0:reg f32 ftz near
  "add_ftz_f32 $s1, $s2, $s3;",
  "add_ftz_near_f32 $s1, $s2, $s3;",
  //"add_ftz_f32 $s1, $s2, 0.2;",
  //"add_ftz_near_f32 $s1, $s2, 0.2;",
  //src0:reg f64 ftz near
  "add_ftz_f64 $d1, $d2, $d3;",//44
  "add_ftz_near_f64 $d1, $d2, $d3;",
  //"add_ftz_f64 $d1, $d2, 0.2;",
  //"add_ftz_near_f64 $d1, $d2, 0.2;",
  //src0:immed f16 ftz near
  //"add_ftz_f16 $s1, 0.1, $s3;",
  //"add_ftz_near_f16 $s1, 0.1, $s3;",
  //"add_ftz_f16 $s1, 0.1, 0.2;",
  //"add_ftz_near_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 ftz near
  //"add_ftz_f32 $s1, 0.1, $d3;",//52
  //"add_ftz_near_f32 $s1, 0.1, $d3;",
  //"add_ftz_f32 $s1, 0.1, 0.2;",
  //"add_ftz_near_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 ftz near
  //"add_ftz_f64 $d1, 0.1, $d3;",
  //"add_ftz_near_f64 $d1, 0.1, $d3;",
  //"add_ftz_f64 $d1, 0.1, 0.2;",
  //"add_ftz_near_f64 $d1, 0.1, 0.2;",
  //src0:reg f16 ftz zero
  "add_ftz_zero_f16 $s1, $s2, $s3;",//60
  //"add_ftz_zero_f16 $s1, $s2, 0.2;",
  //src0:reg f32 ftz zero
  "add_ftz_zero_f32 $s1, $s2, $s3;", 
  //"add_ftz_zero_f32 $s1, $s2, 0.2;",
  //src0:reg f64 ftz zero
  "add_ftz_zero_f64 $d1, $d2, $d3;", 
  //"add_ftz_zero_f64 $d1, $d2, 0.2;",
  //src0:immed f16 ftz zero
  //"add_ftz_zero_f16 $s1, 0.1, $s3;",
  //"add_ftz_zero_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 ftz zero
  //"add_ftz_zero_f32 $s1, 0.1, $s3;", 
  //"add_ftz_zero_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 ftz zero
  //"add_ftz_zero_f64 $d1, 0.1, $d3;",//70
  //"add_ftz_zero_f64 $d1, 0.1, 0.2;",
  //src0:reg f16 ftz up
  "add_ftz_up_f16 $s1, $s2, $s3;", 
  //"add_ftz_up_f16 $s1, $s2, 0.2;",
  //src0:reg f32 ftz up
  "add_ftz_up_f32 $s1, $s2, $s3;", 
  //"add_ftz_up_f32 $s1, $s2, 0.2;",
  //src0:reg f64 ftz up
  "add_ftz_up_f64 $d1, $d2, $d3;", 
  //"add_ftz_up_f64 $d1, $d2, 0.2;",
  //src0:immed f16 ftz up
  //"add_ftz_up_f16 $s1, 0.1, $s3;", 
  //"add_ftz_up_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 ftz up
  //"add_ftz_up_f32 $s1, 0.1, $s3;", //80
  //"add_ftz_up_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 ftz up
  //"add_ftz_up_f64 $d1, 0.1, $d3;", 
  //"add_ftz_up_f64 $d1, 0.1, 0.2;",
  //src0:reg f16 ftz down
  "add_ftz_down_f16 $s1, $s2, $s3;",
  //"add_ftz_down_f16 $s1, $s2, 0.2;",
  //src0:reg f32 ftz down
  "add_ftz_down_f32 $s1, $s2, $s3;",
  //"add_ftz_down_f32 $s1, $s2, 0.2;",
  //src0:reg f64 ftz down
  "add_ftz_down_f64 $d1, $d2, $d3;"//, //88
  //"add_ftz_down_f64 $d1, $d2, 0.2;",
  //src0:immed f16 ftz down
  //"add_ftz_down_f16 $s1, 0.1, $s3;",
  //"add_ftz_down_f16 $s1, 0.1, 0.2;",
  //src0:immed f32 ftz down
  //"add_ftz_down_f32 $s1, 0.1, $s3;",
  //"add_ftz_down_f32 $s1, 0.1, 0.2;",
  //src0:immed f64 ftz down
  //"add_ftz_down_f64 $d1, 0.1, $d3;",
  //"add_ftz_down_f64 $d1, 0.1, 0.2;"
};

BrigAluModifier add_bam[] = {
  {1,0,0,0,0,0,0},//float,near,noint,noftz,approx,fbar,reserved

  {1,1,0,0,0,0,0},//float,zero,noint,noftz,approx,fbar,reserved

  {1,2,0,0,0,0,0},//float,up,noint,noftz,approx,fbar,reserved

  {1,3,0,0,0,0,0},//float,down,noint,noftz,approx,fbar,reserved

  {1,0,0,1,0,0,0},//float,near,noint,ftz,approx,fbar,reserved

  {1,1,0,1,0,0,0},//float,zero,noint,ftz,approx,fbar,reserved

  {1,2,0,1,0,0,0},//float,up,noint,ftz,approx,fbar,reserved

  {1,3,0,1,0,0,0} //float,down,noint,ftz,approx,fbar,reserved
};

BrigInstMod outputarray_instruction3op_add_ftz_round_float[] = {
  {//0
    36,                               //src0:reg f16 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[1]
  },
  {//2
    36,                                //src0:reg f32 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[1]
  },
  {//3
    36,                                //src0:reg f64 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[1]
  },/*
  {//4
    36,                              //src0:immed value f16 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[1]
  },
  {//5
    36,                              //src0:immed value f32 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[1]
  },
  {//6
    36,                             //src0:immed value f32 noftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[1]
  },*/
  {//7
    36,                             //src0:reg f16 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[2]
  },
  {//8
    36,                               //src0:reg f32 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[2]
  },
  {//9
    36,                               //src0:reg f64 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[2]
  },/*
  {//10
    36,                              //src0:immed value f16 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[2]
  },
  {//11
    36,                             //src0:immed value f32 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[2]
  },
  {//12
    36,                            //src0:immed value f64 noftz up
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[2]
  },*/
  {//13
    36,                           //src0:reg f16 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[3]
  },
  {//14
    36,                           //src0:reg f32 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[3]
  },
  {//15
    36,                          //src0:reg f64 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[3]
  },/*
  {//16
    36,                          //src0:immed value f16 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[3]
  },
  {//17
    36,                          //src0:immed value f32 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[3]
  },
  {//18
    36,                          //src0:immed value f64 noftz down
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[3]
  },*/
  {//19
    36,                          //src0:reg f16 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },
  {//20
    36,                          //src0:reg f16 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },
  {//21
    36,                          //src0:reg f32 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },
  {//22
    36,                          //src0:reg f32 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },
  {//23
    36,                           //src0:reg f64 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },
  {//24
    36,                          //src0:reg f64 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[4]
  },/*
  {//25
    36,                          //src0:immed value f16 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },
  {//26
    36,                          //src0:immed value f16 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },
  {//27
    36,                          //src0:immed value f32 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },
  {//28
    36,                          //src0:immed value f32 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },
  {//29
    36,                          //src0:immed value f64 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },
  {//30
    36,                          //src0:immed value f64 ftz near
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[4]
  },*/
  {//31
    36,                          //src0:reg f16 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[5]
  },
  {//32
    36,                          //src0:reg f32 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[5]
  },
  {//33
    36,                          //src0:reg f64 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[5]
  },/*
  {//34
    36,                          //src0:immed value f16 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[5]
  },
  {//35
    36,                          //src0:immed value f32 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[5]
  },
  {//36
    36,                          //src0:immed value f32 ftz zero
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[5]
  },*/
  {//37
    36,                          //src0:reg f16 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[6]
  },
  {//38
    36,                          //src0:reg f32 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[6]
  },
  {//39
    36,                          //src0:reg f64 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[6]
  },/*
  {//40
    36,                          //src0:immed value f16 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[6]
  },
  {//41
    36,                          //src0:immed value f32 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[6]
  },
  {//42
    36,                          //src0:immed value f64 ftz up
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[6]
  },*/
  {//43
    36,                          //src0:reg f16 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[7]
  },
  {//44
    36,                          //src0:reg f32 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[7]
  },
  {//45
    36,                          //src0:reg f64 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 32, 0, 0},
    add_bam[7]
  }/*,
  {//46
    36,                          //src0:immed value f16 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf16,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[7]
  },
  {//47
    36,                          //src0:immed value f32 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf32,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[7]
  },
  {//48
    36,                          //src0:immed value f64 ftz down
    BrigEInstMod,
    BrigAdd,
    Brigf64,
    BrigNoPacking,
    {8, 20, 44, 0, 0},
    add_bam[7]
  }*/
};


class Instruction3OpAddPackedFloat: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_add_packed_float[] = {
  "add_pp_f16x2 $s1, $s2, $s3;",
  "add_ps_f16x4 $d1, $d2, $d3;",
  "add_sp_f32x2 $d1, $d2, $d3;",
  "add_ss_f32x2 $d1, $d2, $d3;"
};

BrigInstBase outputarray_instruction3op_add_packed_float[] = {
  {//0
    32,
    BrigEInstBase,
    BrigAdd,
    Brigf16x2,
    BrigPackPP,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,
    BrigEInstBase,
    BrigAdd,
    Brigf16x4,
    BrigPackPS,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,
    BrigEInstBase,
    BrigAdd,
    Brigf32x2,
    BrigPackSP,
    {8, 20, 32, 0, 0}
  },
  {//3
    32,
    BrigEInstBase,
    BrigAdd,
    Brigf32x2,
    BrigPackSS,
    {8, 20, 32, 0, 0}
  }
};


class Instruction3OpCarry: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_carry[] = {
  //src0:reg u32
  "carry_u32 $s1, $s2, $s3;",
  //"carry_u32 $s1, $s2, 1;",
  "carry_u32 $s1, $s2, WAVESIZE;",
  //src0:reg s32
  "carry_s32 $s1, $s2, $s3;",
  //"carry_s32 $s1, $s2, 1;",
  "carry_s32 $s1, $s2, WAVESIZE;",
  //src0:reg u64
  "carry_u64 $d1, $d2, $d3;",//6
  //"carry_u64 $d1, $d2, 1;",
  "carry_u64 $d1, $d2, WAVESIZE;",
  //src0:reg s64
  "carry_s64 $d1, $d2, $d3;",
  //"carry_s64 $d1, $d2, 1;",
  "carry_s64 $d1, $d2, WAVESIZE;",
  //src0:immed u32
  //"carry_u32 $s1, 61, $s3;",
  //"carry_u32 $s1, 61, 1;",
  //"carry_u32 $s1, 61, WAVESIZE;",
  //src0:immed s32
  //"carry_s32 $s1, 61, $d3;",//15
  //"carry_s32 $s1, 61, 1;",
  //"carry_s32 $s1, 61, WAVESIZE;",
  //src0:immed u64
  //"carry_u64 $d1, 61, $d3;",
  //"carry_u64 $d1, 61, 1;",
  //"carry_u64 $d1, 61, WAVESIZE;",
  //src0:immed s64
  //"carry_s64 $d1, 61, $d3;",
  //"carry_s64 $d1, 61, 1;",
  //"carry_s64 $d1, 61, WAVESIZE;",
  //src0:wavesize u32
  "carry_u32 $s1, WAVESIZE, $s3;",//24
  //"carry_u32 $s1, WAVESIZE, 61;",
  "carry_u32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s32
  "carry_s32 $s1, WAVESIZE, $s3;",
  //"carry_s32 $s1, WAVESIZE, 61;",
  "carry_s32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize u64
  "carry_u64 $d1, WAVESIZE, $d3;",
  //"carry_u64 $d1, WAVESIZE, 61;",//31
  "carry_u64 $d1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s64
  "carry_s64 $d1, WAVESIZE, $d3;",
  //"carry_s64 $d1, WAVESIZE, 61;",
  "carry_s64 $d1, WAVESIZE, WAVESIZE;"
};

BrigInstBase outputarray_instruction3op_carry[] = {
  {//0
    32,                          //src0:reg u32
    BrigEInstBase,
    BrigCarry,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,                          //src0:reg s32
    BrigEInstBase,
    BrigCarry,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,                          //src0:reg u64
    BrigEInstBase,
    BrigCarry,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//3
    32,                          //src0:reg s64
    BrigEInstBase,
    BrigCarry,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },/*
  {//4
    32,                          //src0:immed value u32
    BrigEInstBase,
    BrigCarry,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//5
    32,                          //src0:immed value s32
    BrigEInstBase,
    BrigCarry,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//6
    32,                          //src0:immed value u64
    BrigEInstBase,
    BrigCarry,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//7
    32,                          //src0:immed value s64
    BrigEInstBase,
    BrigCarry,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },*/
  {//8
    32,                          //src0:wavesize u32
    BrigEInstBase,
    BrigCarry,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//9
    32,                          //src0:wavesize s32
    BrigEInstBase,
    BrigCarry,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//10
    32,                          //src0:wavesize u64
    BrigEInstBase,
    BrigCarry,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//11
    32,                          //src0:wavesize s64
    BrigEInstBase,
    BrigCarry,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};


class Instruction3OpBorrow: public ::testing::TestWithParam<int>{
};
std::string inputarray_instruction3op_borrow[] = {
  //src0:reg u32
  "borrow_u32 $s1, $s2, $s3;",
  //"borrow_u32 $s1, $s2, 1;",
  "borrow_u32 $s1, $s2, WAVESIZE;",
  //src0:reg s32
  "borrow_s32 $s1, $s2, $s3;",
  //"borrow_s32 $s1, $s2, 1;",
  "borrow_s32 $s1, $s2, WAVESIZE;",
  //src0:reg u64
  "borrow_u64 $d1, $d2, $d3;",
  //"borrow_u64 $d1, $d2, 1;",
  "borrow_u64 $d1, $d2, WAVESIZE;",
  //src0:reg s64
  "borrow_s64 $d1, $d2, $d3;",
  //"borrow_s64 $d1, $d2, 1;",
  "borrow_s64 $d1, $d2, WAVESIZE;",
  //src0:immed u32
  //"borrow_u32 $s1, 61, $s3;",
  //"borrow_u32 $s1, 61, 1;",
  //"borrow_u32 $s1, 61, WAVESIZE;",
  //src0:immed s32
  //"borrow_s32 $s1, 61, $d3;",
  //"borrow_s32 $s1, 61, 1;",
  //"borrow_s32 $s1, 61, WAVESIZE;",
  //src0:immed u64
  //"borrow_u64 $d1, 61, $d3;",
  //"borrow_u64 $d1, 61, 1;",
  //"borrow_u64 $d1, 61, WAVESIZE;",
  //src0:immed s64
  //"borrow_s64 $d1, 61, $d3;",
  //"borrow_s64 $d1, 61, 1;",
  //"borrow_s64 $d1, 61, WAVESIZE;",
  //src0:wavesize u32
  "borrow_u32 $s1, WAVESIZE, $s3;",
  //"borrow_u32 $s1, WAVESIZE, 61;",
  "borrow_u32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s32
  "borrow_s32 $s1, WAVESIZE, $s3;",
  //"borrow_s32 $s1, WAVESIZE, 61;",
  "borrow_s32 $s1, WAVESIZE, WAVESIZE;",
  //src0:wavesize u64
  "borrow_u64 $d1, WAVESIZE, $d3;",
  //"borrow_u64 $d1, WAVESIZE, 61;",
  "borrow_u64 $d1, WAVESIZE, WAVESIZE;",
  //src0:wavesize s64
  "borrow_s64 $d1, WAVESIZE, $d3;",
  //"borrow_s64 $d1, WAVESIZE, 61;",
  "borrow_s64 $d1, WAVESIZE, WAVESIZE;" 
};

BrigInstBase outputarray_instruction3op_borrow[] = {
  {//0
    32,                        //src0:reg u32
    BrigEInstBase,
    BrigBorrow,
    Brigu32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//1
    32,                        //src0:reg s32
    BrigEInstBase,
    BrigBorrow,
    Brigs32,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//2
    32,                        //src0:reg u64
    BrigEInstBase,
    BrigBorrow,
    Brigu64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },
  {//3
    32,                        //src0:reg s64
    BrigEInstBase,
    BrigBorrow,
    Brigs64,
    BrigNoPacking,
    {8, 20, 32, 0, 0}
  },/*
  {//4
    32,                        //src0:immed value u32
    BrigEInstBase,
    BrigBorrow,
    Brigu32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//5
    32,                        //src0:immed value s32
    BrigEInstBase,
    BrigBorrow,
    Brigs32,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//6
    32,                        //src0:immed value u64
    BrigEInstBase,
    BrigBorrow,
    Brigu64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },
  {//7
    32,                        //src0:immed value s64
    BrigEInstBase,
    BrigBorrow,
    Brigs64,
    BrigNoPacking,
    {8, 20, 44, 0, 0}
  },*/
  {//8
    32,                         //src0:wavesize u32
    BrigEInstBase,
    BrigBorrow,
    Brigu32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//9
    32,                         //src0:wavesize s32
    BrigEInstBase,
    BrigBorrow,
    Brigs32,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//10
    32,                         //src0:wavesize u64
    BrigEInstBase,
    BrigBorrow,
    Brigu64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  },
  {//11
    32,                         //src0:wavesize s64
    BrigEInstBase,
    BrigBorrow,
    Brigs64,
    BrigNoPacking,
    {8, 20, 24, 0, 0}
  }
};


}  // namespace brig
}  // namespace hsa
#endif //CODEGEN_TEST_H_
