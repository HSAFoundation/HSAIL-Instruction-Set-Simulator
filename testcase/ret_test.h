/* Copyright 2012 <MulticorewareInc> */

#ifndef RET_TEST_H_
#define RET_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestRet: public ::testing::TestWithParam<int>
{

};

struct RetTest{
  const char* str;
  BrigInstBase ref;
};

struct RetTest ret_pair[1] = {
  {
    "ret;",
     {
      brig_inst_base_size,
      BrigEInstBase,
      BrigRet,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
    }
  }
};

class TestRetInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_ret_invalid[] = {
  "ret",
  "Ret;"
};

}  // namespace brig
}  // namespace hsa
#endif //RET_TEST_H_
