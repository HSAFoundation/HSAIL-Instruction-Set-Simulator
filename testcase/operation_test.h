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

}  // namespace brig
}  // namespace hsa
#endif //CODEGEN_TEST_H_
