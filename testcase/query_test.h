/* Copyright 2012 <MulticorewareInc> */

#ifndef QUERY_TEST_H_
#define QUERY_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestQuery: public ::testing::TestWithParam<int>
{

};

struct QueryTest{
  const char* str;
  BrigInstBase ref;
};

struct QueryTest query_pair[8] = {
//query_pair[0]
  {
    "query_width_u32 $s1, [%RWImg3];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryWidth,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[1]
  {
    "query_height_u32 $s0, [&namedRWImg1];",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryHeight,
    Brigu32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[2]
  {
    "query_depth_u32 $s0, [&namedRWImg1];",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryDepth,
    Brigu32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[3]
  {
    "query_array_b32 $s1, [%RWImg3];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryArray,
    Brigb32,
    BrigNoPacking,
   {operand_offset, operand_offset + reg_size, 0, 0, 0}
  }
  },
//query_pair[4]
  {
    "query_normalized_b32 $s0, [&namedRWImg1];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryNormalized,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[5]
  {
    "query_filtering_b32  $s0, [&namedRWImg1];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryFiltering,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[6]
  {
    "query_order_b32 $s1, [%RWImg3];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryOrder,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
//query_pair[7]
  {
    "query_data_b32 $s1, [%RWImg3];",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigQueryData,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
};
class TestQueryInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_query_invalid[] = {
  "query_data_b32 $s1;",
  "query_test_b32 $s1, [%RWImg3];",
  "query_data $s1, [%RWImg3];"
};
}  // namespace brig
}  // namespace hsa
#endif //QUERY_TEST_H_
