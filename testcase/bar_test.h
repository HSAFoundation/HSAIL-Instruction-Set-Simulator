/* Copyright 2012 <MulticorewareInc> */

#ifndef BAR_TEST_H_
#define BAR_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestBar: public ::testing::TestWithParam<int>
{

};

struct BarTest{
  const char* str;
  BrigInstBar ref;
};

struct BarTest bar_pair[12] = {
//bar_pair[0]
  {
    "barrier;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel|BrigGlobalLevel
    }
  },
//bar_pair[1]
  {
    "barrier_global;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGlobalLevel
    }
  },
//bar_pair[2]
  {
    "barrier_group;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel
    }
  },
//bar_pair[3]
  {
    "barrier_partial;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigPartialLevel
    }
  },
//bar_pair[4]
  {
    "barrier_width(64);",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel|BrigGlobalLevel
    }
  },
//bar_pair[5]
  {
    "barrier_width(64)_global;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGlobalLevel
    }
  },
//bar_pair[6]
  {
    "barrier_width(64)_group;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel
    }
  },
//bar_pair[7]
  {
    "barrier_width(64)_partial;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigPartialLevel
    }
  },
//bar_pair[8]
  {
    "barrier_width(all);",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel|BrigGlobalLevel
    }
  },
//bar_pair[9]
  {
    "barrier_width(all)_global;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGlobalLevel
    }
  },
//bar_pair[10]
  {
    "barrier_width(all)_group;",
     {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigGroupLevel
    }
  },
//bar_pair[11]
  {
    "barrier_width(all)_partial;",
    {
      brig_inst_bar_size,
      BrigEInstBar,
      BrigBarrier,
      Brigb32,
      BrigNoPacking,
      {operand_offset, 0, 0, 0, 0},
      BrigPartialLevel
    }
  }
};

class TestBarInvalidInput: public ::testing::TestWithParam<int>
{

};

std::string inputarray_bar_invalid[] = {
  "barrier",
  "barrier_width;",
  "barrier_width(64.0f);",
  "barrier_width(test);"
};

}  // namespace brig
}  // namespace hsa
#endif //BAR_TEST_H_
