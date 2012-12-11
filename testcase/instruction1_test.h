/* Copyright 2012 <MulticorewareInc> */

#ifndef INSTRUCTION1_TEST_H_
#define INSTRUCTION1_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

struct Instruction1Test{
  const char* str;
  BrigInstBase ref;
};

struct Instruction1NullptrTest{
  const char* str;
  BrigInstMem ref;
};

class TestInstruction1NullPtr: public ::testing::TestWithParam<int>{
};

struct Instruction1NullptrTest instruction1_nullptr_pair[9] = {
//instruction1_nullptr_pair[0]
  {
    "nullptr_u64 $d1;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu64,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigFlatSpace
    }
  },
 //instruction1_nullptr_pair[1]
  {
    "nullptr_u32 $d1;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigFlatSpace
    }
  },
  //instruction1_nullptr_pair[2]
  {
    "nullptr_readonly_u32 $d1;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigReadonlySpace
    }
  },
  //instruction1_nullptr_pair[3]
  {
    "nullptr_kernarg_u64 $d2;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu64,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigKernargSpace
    }
  },
  //instruction1_nullptr_pair[4]
  {
    "nullptr_global_u32 $d1;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGlobalSpace
    }
  },
  //instruction1_nullptr_pair[5]
  {
    "nullptr_private_u64 $d2;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu64,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigPrivateSpace
    }
  },
  //instruction1_nullptr_pair[6]
  {
    "nullptr_arg_u32 $d1;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigArgSpace
    }
  },
  //instruction1_nullptr_pair[7]
  {
    "nullptr_group_u64 $d2;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu64,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupSpace
    }
  },
  //instruction1_nullptr_pair[8]
  {
    "nullptr_spill_u64 $d2;",
    {
      brig_inst_mem_size,
      BrigEInstMem,
      BrigNullPtr,
      Brigu64,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigSpillSpace
    }
  }
};

class TestInstruction1LaneId: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_laneid_pair[1] = {
  {
  "laneid $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigLaneId,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1DynWaveId: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_dynwaveid_pair[1] = {
  {
  "dynwaveid $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigDynWaveId,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1DispatchId: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_dispatchid_pair[1] = {
  {
  "dispatchid $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigDispatchId,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1Cu: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_cu_pair[1] = {
  {
  "cu $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigCU,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1WorkDim: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_workdim_pair[1] = {
  {
  "workdim $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigWorkDim,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1WorkitemIdFlat: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_workitemidflat_pair[1] = {
  {
  "workitemid_flat $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigWorkItemIdFlat,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1WorkitemAbsidFlat: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_workitemabsidflat_pair[1] = {
  {
  "workitemabsid_flat $s1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigWorkItemAbsIdFlat,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1Clock: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_clock_pair[1] = {
  {
  "clock $d1;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigClock,
    Brigb64,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

class TestInstruction1DebugTrap: public ::testing::TestWithParam<int>{
};

struct Instruction1Test instruction1_debugtrap_pair[2] = {
//instruction1opnodt_debugtrap_pair[0]
  {
    "debugtrap 0;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigDebugTrap,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  },
//instruction1opnodt_debugtrap_pair[1]
  {
    "debugtrap 61;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigDebugTrap,
    Brigb32,
    BrigNoPacking,
    {operand_offset, 0, 0, 0, 0}
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //INSTRUCTION1_TEST_H_
