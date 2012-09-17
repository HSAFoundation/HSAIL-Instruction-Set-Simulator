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
  "debugtrap 61;"
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
  "mul_s32 $s1;"
  "mul_s32 $s1, $s2;",
  "mul_s32 1, $s2, $s3;",
  "mul_s32 WAVESIZE, $s2, $s3;",
  "mul_s64 $s1;"
  "mul_s64 $s1, $s2;",
  "mul_s64 1, $s2, $s3;",
  "mul_s64 WAVESIZE, $s2, $s3;",
  "mul_s64 $s1, $s2, $s3;",
  "mul_u32 $s1;"
  "mul_u32 $s1, $s2;",
  "mul_u32 1, $s2, $s3;",
  "mul_u32 WAVESIZE, $s2, $s3;"
  "mul_u64 $s1;"
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
}  // namespace brig
}  // namespace hsa
#endif //CODEGEN_TEST_H_
