/* Copyright 2012 <MulticorewareInc> */

#ifndef MOV_TEST_H_
#define MOV_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestMov : public ::testing::TestWithParam<int>
{

};
struct MovTest{
  const char *str;
  BrigInstBase ref;
};

struct MovTest mov_pair[10] = {
// mov_pair[0]
  {
    "mov_b1 $c1, $c2;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb1,
      BrigNoPacking,
      {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
// mov_pair[1]
  {
    "mov_b32 $s1, $s2;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb32,
      BrigNoPacking,
      {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
// mov_pair[2]
  {
    "mov_b32 $s1, 1.0f;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb32,
      BrigNoPacking,
      {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
// mov_pair[3]
  {
    "mov_b64 $d1, $d2;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb64,
      BrigNoPacking,
      {operand_offset, operand_offset + reg_size, 0, 0, 0}
    }
  },
// mov_pair[4]
  {
    "mov_b64 $d1, 1.0;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb64,
      BrigNoPacking,
      {operand_offset, RoundUp8(operand_offset + reg_size), 0, 0, 0}
    }
  },
// mov_pair[5]
  {
    "mov_b64 $d1, ($s1, $s2);",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb64,
      BrigNoPacking,
      {operand_offset, operand_offset + reg_size * 3, 0, 0, 0}
    }
  },
// mov_pair[6]
  {
    "mov_b64 ($s1, $s2), $d3;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb64,
      BrigNoPacking,
      {
        operand_offset + reg_size * 2,
        operand_offset + reg_size * 2 + regv2_size,
        0,
        0,
        0
      }
    }
  },
// mov_pair[7]
  {
    "mov_b64 ($s1, $s2), 1.0;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb64,
      BrigNoPacking,
      {
        operand_offset + reg_size * 2,
        RoundUp8(operand_offset + reg_size * 2 + regv2_size),
        0,
        0,
        0
      }
    }
  },
// mov_pair[8]
  {
    "mov_b128 $q1, ($s1, $s2, $s3, $s4);",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb128,
      BrigNoPacking,
      {operand_offset, operand_offset + reg_size * 5, 0, 0, 0}
    }
  },
// mov_pair[9]
  {
    "mov_b128 ($s1, $s2, $s3, $s4), $q1;",
    {
      brig_inst_base_size,
      BrigEInstBase,
      BrigMov,
      Brigb128,
      BrigNoPacking,
      {
        operand_offset + reg_size * 4,
        operand_offset + reg_size * 4 + regv4_size,
        0,
        0,
        0
      }
    }
  }
};

class TestMovInvalid : public ::testing::TestWithParam<int>
{

};

std::string input_array_mov_invalid[29] = {
// false[0]
  "mov_b1 $s1;",
// false[1]
  "mov_b32 $s1;",
// false[2]
  "mov_b64 $s1;",
// alse[3]
  "mov_b128 $s2;",
// false[4]
  "mov_b1 100, $s2;",
// false[5]
  "mov_b32 $s1, $s2, $s3;",
// false[6]
  "mov_b32 $s1, s2;",
// false[7]
  "mov_b64 $s1, $2;",
// false[8]
  "mov_b64 $s1, $s200;",
// false[9]
  "mov_b64 $d1, $d2",
// false[10]
  "mov_b64 $d1, $d2,;",
// false[11]
  "mov_b64 $d1, ($s2, $c3);",
// false[12]
  "mov_b64 $d1, ($d2, $s3);",
// false[13]
  "mov_b64 $d1, ($q2, $s3);",
// false[14]
  "mov_b64 $s1, ($s2, $d3);",
// false[15]
  "mov_b64 $c1, ($s2, $s3, $s4);",
// false[16]
  "mov_b64 ($s1, $s2, $s3), $s3;",
// false[17]
  "mov_b64 ($c1, $s2), $q3;",
// false[18]
  "mov_b64 ($s1, $d2), $d3;",
// false[19]
  "mov_b64 ($s1, $s2), $d3",
// false[20]
  "mov_b64 ($s1, $q2), $d3;",
// false[21]
  "mov_b64 ($d1, $s2), $d3;",
// false[22]
  "mov_b64 ($d1, $s2, $s3), $d3;",
// false[23]
  "mov_b64 ($s1, $d2), $c3;",
// false[24]
  "mov_b64 $d1, ($s2, $s3, $s4, $s5;",
// false[25]
  "mov_b128 $c1, ($s2, $s3, $s4, $s5);",
// false[26]
  "mov_b128 1.0, ($s2, $s3, $s4, $s5);",
// false[27]
  "mov_b2 $s1, $s2;",
// false[28]
  "movv_b32 $s1, $s2;"
};

}  // namespace brig
}  // namespace hsa
#endif //MOV_TEST_H_
