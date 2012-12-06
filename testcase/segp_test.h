/* Copyright 2012 <MulticorewareInc> */

#ifndef SEGP_TEST_H_
#define SEGP_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {
class SegpSegmentpb1: public ::testing::TestWithParam<int>{
};

struct SegpTest{
  const char* str;
  BrigInstMem ref;
};

struct SegpTest segpsegmentpb1_pair[28] = {
//global
//segpsegmentpb1_pair[0]
  {
    "segmentp_global_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpsegmentpb1_pair[1]
  {
    "segmentp_global_b1 $c1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpsegmentpb1_pair[2]
  {
    "segmentp_global_b1 $c1,1;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpsegmentpb1_pair[3]
  {
    "segmentp_global_b1 $c1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
 //group
//segpsegmentpb1_pair[4]
  {
    "segmentp_group_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpsegmentpb1_pair[5]
  {
    "segmentp_group_b1 $c1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpsegmentpb1_pair[6]
  {
    "segmentp_group_b1 $c1,1;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpsegmentpb1_pair[7]
  {
    "segmentp_group_b1 $c1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//private
//segpsegmentpb1_pair[8]
  {
    "segmentp_private_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpsegmentpb1_pair[9]
  {
    "segmentp_private_b1 $c1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpsegmentpb1_pair[10]
  {
    "segmentp_private_b1 $c1,1;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpsegmentpb1_pair[11]
  {
    "segmentp_private_b1 $c1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//kernarg
//segpsegmentpb1_pair[12]
  {
    "segmentp_kernarg_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpsegmentpb1_pair[13]
  {
    "segmentp_kernarg_b1 $c1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpsegmentpb1_pair[14]
  {
    "segmentp_kernarg_b1 $c1,1;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpsegmentpb1_pair[15]
  {
    "segmentp_kernarg_b1 $c1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//readonly
//segpsegmentpb1_pair[16]
  {
    "segmentp_readonly_b1 $c1,$s2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpsegmentpb1_pair[17]
  {
    "segmentp_readonly_b1 $c1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpsegmentpb1_pair[18]
  {
    "segmentp_readonly_b1 $c1,1;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpsegmentpb1_pair[19]
  {
    "segmentp_readonly_b1 $c1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//spill
//segpsegmentpb1_pair[20]
  {
    "segmentp_spill_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpsegmentpb1_pair[21]
  {
    "segmentp_spill_b1 $c1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpsegmentpb1_pair[22]
  {
    "segmentp_spill_b1 $c1,1;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpsegmentpb1_pair[23]
  {
    "segmentp_spill_b1 $c1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//arg
//segpsegmentpb1_pair[24]
  {
    "segmentp_arg_b1 $c1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpsegmentpb1_pair[25]
  {
    "segmentp_arg_b1 $c1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpsegmentpb1_pair[26]
  {
    "segmentp_arg_b1 $c1,1;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigArgSpace
    }
  },
//segpsegmentpb1_pair[27]
  {
    "segmentp_arg_b1 $c1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigSegmentp,
    Brigb1,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  }
};

class SegpFtos: public ::testing::TestWithParam<int>
{
};

struct SegpTest segpftos_pair[42] = {
//global u32
//segpftos_pair[0]
  {
    "ftos_global_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpftos_pair[1]
  {
    "ftos_global_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpftos_pair[2]
  {
    "ftos_global_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//group u32
//segpftos_pair[3]
  {
    "ftos_group_u32 $s1,$s2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpftos_pair[4]
  {
    "ftos_group_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpftos_pair[5]
  {
    "ftos_group_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//private u32
//segpftos_pair[6]
  {
    "ftos_private_u32 $s1,$s2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpftos_pair[7]
  {
    "ftos_private_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpftos_pair[8]
  {
    "ftos_private_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//kernarg u32
//segpftos_pair[9]
  {
    "ftos_kernarg_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpftos_pair[10]
  {
    "ftos_kernarg_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpftos_pair[11]
  {
    "ftos_kernarg_u32 $s1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//readonly u32
//segpftos_pair[12]
  {
    "ftos_readonly_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpftos_pair[13]
  {
    "ftos_readonly_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpftos_pair[14]
  {
    "ftos_readonly_u32 $s1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//spill u32
//segpftos_pair[15]
  {
    "ftos_spill_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpftos_pair[16]
  {
    "ftos_spill_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpftos_pair[17]
  {
    "ftos_spill_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//arg u32
//segpftos_pair[18]
  {
    "ftos_arg_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpftos_pair[19]
  {
    "ftos_arg_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigArgSpace
    }
  },
//segpftos_pair[20]
  {
    "ftos_arg_u32 $s1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//global u64
//segpftos_pair[21]
  {
    "ftos_global_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpftos_pair[22]
  {
    "ftos_global_u64 $d1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpftos_pair[23]
  {
    "ftos_global_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//group u64
//segpftos_pair[24]
  {
    "ftos_group_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpftos_pair[25]
  {
    "ftos_group_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpftos_pair[26]
  {
    "ftos_group_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//private u64
//segpftos_pair[27]
  {
    "ftos_private_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpftos_pair[28]
  {
    "ftos_private_u64 $d1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpftos_pair[29]
  {
    "ftos_private_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//kernarg u64
//segpftos_pair[30]
  {
    "ftos_kernarg_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpftos_pair[31]
  {
    "ftos_kernarg_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpftos_pair[32]
  {
    "ftos_kernarg_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//readonly u64
//segpftos_pair[33]
  {
    "ftos_readonly_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpftos_pair[34]
  {
    "ftos_readonly_u64 $d1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpftos_pair[35]
  {
    "ftos_readonly_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//spill u64
//segpftos_pair[36]
  {
    "ftos_spill_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpftos_pair[37]
  {
    "ftos_spill_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpftos_pair[38]
  {
    "ftos_spill_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//arg u64
//segpftos_pair[39]
  {
    "ftos_arg_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpftos_pair[40]
  {
    "ftos_arg_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigArgSpace
    }
  },
//segpftos_pair[41]
  {
    "ftos_arg_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigFtoS,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  }
};

class SegpStof : public ::testing::TestWithParam<int>{
};

struct SegpTest segpstof_pair[] = {
//global u32
//segpstof_pair[0]
  {
    "stof_global_u32 $s1,$s2;",
   {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpstof_pair[1]
  {
    "stof_global_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpstof_pair[2]
  {
    "stof_global_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//group u32
//segpstof_pair[3]
  {
    "stof_group_u32 $s1,$s2;",
   {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpstof_pair[4]
  {
   "stof_group_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpstof_pair[5]
  {
    "stof_group_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//private u32
//segpstof_pair[6]
  {
    "stof_private_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpstof_pair[7]
  {
    "stof_private_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpstof_pair[8]
  {
    "stof_private_u32 $s1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//kernarg u32
//segpstof_pair[9]
  {
    "stof_kernarg_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpstof_pair[10]
  {
    "stof_kernarg_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpstof_pair[11]
  {
    "stof_kernarg_u32 $s1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//readonly u32
//segpstof_pair[12]
  {
    "stof_readonly_u32 $s1,$s2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpstof_pair[13]
  {
    "stof_readonly_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpstof_pair[14]
  {
    "stof_readonly_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//spill u32
//segpstof_pair[15]
  {
    "stof_spill_u32 $s1,$s2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpstof_pair[16]
  {
    "stof_spill_u32 $s1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpstof_pair[17]
  {
    "stof_spill_u32 $s1,WAVESIZE;",
   {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//arg u32
//segpstof_pair[18]
  {
    "stof_arg_u32 $s1,$s2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpstof_pair[19]
  {
    "stof_arg_u32 $s1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigArgSpace
    }
  },
//segpstof_pair[20]
  {
    "stof_arg_u32 $s1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//global u64
//segpstof_pair[21]
  {
    "stof_global_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpstof_pair[22]
  {
    "stof_global_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGlobalSpace
    }
  },
//segpstof_pair[23]
  {
    "stof_global_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGlobalSpace
    }
  },
//group u64
//segpstof_pair[24]
  {
    "stof_group_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpstof_pair[25]
  {
    "stof_group_u64 $d1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigGroupSpace
    }
  },
//segpstof_pair[26]
  {
    "stof_group_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigGroupSpace
    }
  },
//private u64
//segpstof_pair[27]
  {
    "stof_private_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpstof_pair[28]
  {
    "stof_private_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigPrivateSpace
    }
  },
//segpstof_pair[29]
  {
    "stof_private_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigPrivateSpace
    }
  },
//kernarg u64
//segpstof_pair[30]
  {
    "stof_kernarg_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpstof_pair[31]
  {
    "stof_kernarg_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigKernargSpace
    }
  },
//segpstof_pair[32]
  {
    "stof_kernarg_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigKernargSpace
    }
  },
//readonly u64
//segpstof_pair[33]
  {
    "stof_readonly_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpstof_pair[34]
  {
    "stof_readonly_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigReadonlySpace
    }
  },
//segpstof_pair[35]
  {
    "stof_readonly_u64 $d1,WAVESIZE;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigReadonlySpace
    }
  },
//spill u64
//segpstof_pair[36]
  {
    "stof_spill_u64 $d1,$d2;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpstof_pair[37]
  {
    "stof_spill_u64 $d1,61;",
     {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigSpillSpace
    }
  },
//segpstof_pair[38]
  {
    "stof_spill_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigSpillSpace
    }
  },
//arg u64
//segpstof_pair[39]
  {
    "stof_arg_u64 $d1,$d2;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  },
//segpstof_pair[40]
  {
    "stof_arg_u64 $d1,61;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0},
    BrigArgSpace
    }
  },
//segpstof_pair[41]
  {
    "stof_arg_u64 $d1,WAVESIZE;",
    {
    brig_inst_mem_size,
    BrigEInstMem,
    BrigStoF,
    Brigu64,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
    BrigArgSpace
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //SEGP_TEST_H_
