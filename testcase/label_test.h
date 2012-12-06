/* Copyright 2012 <MulticorewareInc> */

#ifndef LABEL_TEST_H_
#define LABEL_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestLabel: public ::testing::TestWithParam<int>
{

};

struct LabelTest{
  const char * str;
  BrigDirectiveLabel ref;
};

struct LabelTest label_pair[2] = {
//label_pair[0]
  {
     "@a:",
     {
    brig_directive_label_size,
    BrigEDirectiveLabel,
    code_offset,
    string_offset,
  }
  },
//label_pair[1]
{
    "@xyz:",
     {
    brig_directive_label_size,
    BrigEDirectiveLabel,
    code_offset,
    string_offset,
  }
  }
};

class TestLabelInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_label_invalid[] = {
 "@xyz",
 "@ :",
  "xyz:"
};

}  // namespace brig
}  // namespace hsa
#endif //LABEL_TEST_H_
