/* Copyright 2012 <MulticorewareInc> */

#ifndef LOCATION_TEST_H_
#define LOCATION_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestLocation: public ::testing::TestWithParam<int>
{

};

struct LocationTest{
  const char * str;
  BrigDirectiveLoc ref;
};

struct LocationTest location_pair[1] = {
  {
    "loc 12 22 32 ;",
     {
    brig_directive_loc_size, //size
    BrigEDirectiveLoc,    //kind
    code_offset,     //c_code
    12,                   //sourceFile
    22,                   //sourceLine
    32                    //sourceColumn
  }
  }
};

class TestLocationInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_location_invalid[] = {
  "locq 12 22 32 ;",
  "loc 12 22 32 ",
  "loc 12 22 ; ",
  "loc -12 22 32 ;",
  "loc 12 22.0f 32 ;"
};

}  // namespace brig
}  // namespace hsa
#endif //LOCATION_TEST_H_
