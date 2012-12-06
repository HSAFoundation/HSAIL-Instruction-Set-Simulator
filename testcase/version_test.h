/* Copyright 2012 <MulticorewareInc> */

#ifndef VERSION_TEST_H_
#define VERSION_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestVersion: public ::testing::TestWithParam<int>{

};

struct VersionTest{
  const char * str;
  BrigDirectiveVersion ref;
};

struct VersionTest version_pair[34] = {
//version_pair[0]
  {
    "version 1:0; ",
    {
    brig_directive_version_size,   //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    1,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[1]
  {
  "version 2:0:$large;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[2]
  { 
    "version 2:0:$small;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[3]
  {
    "version 2:0:$reduced;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[4]
  {
    "version 2:0:$full;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[5]
  {
    "version 2:0:$sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[6]
  {
    "version 2:0:$nosftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[7]
  {
    "version 2:0:$reduced, $small;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[8]
  {
    "version 2:0:$reduced, $large;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[9]
  {
    "version 2:0:$full, $small;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[10]
  {
    "version 2:0:$full, $large;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[11]
  {
    "version 2:0:$sftz, $reduced;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[12]
  {
    "version 2:0:$nosftz, $reduced;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[13]
  {
    "version 2:0:$sftz, $full;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[14]
  {
    "version 2:0:$nosftz, $full;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[15]
{
    "version 2:0:$large, $sftz;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[16]
{
    "version 2:0:$large, $nosftz;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[17]
  {
    "version 2:0:$small, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[18]
  {
    "version 2:0:$small, $nosftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[19]
  {
    "version 2:0:$large, $reduced, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[20]
  {
    "version 2:0:$large, $reduced, $nosftz;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[21]
  {
    "version 2:0:$large, $full, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[22]
  {
    "version 2:0:$large, $full, $nosftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[23]
  {
    "version 2:0:$small, $reduced, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,              // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[24]
  {
    "version 2:0:$small, $reduced, $nosftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,              // profile
    BrigENosftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[25]
  {
    "version 2:0:$small, $full, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[26]
  {
    "version 3:2:$small, $full, $nosftz;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[27]
  {
    "version 2:0:$large, $small;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
    }
  },
//version_pair[28]
  {
    "version 2:0:$full, $reduced, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[29]
  {
    "version 3:2:$small, $full, $nosftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigENosftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[30]
  {
    "version 3:2:$small, $full, $nosftz, $sftz;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[31]
  {
    "version 3:2:$small, $full, $nosftz, $sftz, $large;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigELarge,             // machine
    BrigEFull,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[32]
  {
    "version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced;",
    {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  },
//version_pair[33]
  {
    "version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced, $small;",
     {
    brig_directive_version_size, //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
    }
  }
};

class TestVersionInvalid: public ::testing::TestWithParam<int>{

};

std::string inputarray_version_invalid[] = {
  "version ;",
  "version 1; ",
  "version 1:; ",
  "version 3:2:$test;",
  "version 2:0:$large ",
  "version 1.0f:2.0f:$small;",
  "version 2:0:$reduced, $small, $stfz, $test;"
};

}  // namespace brig
}  // namespace hsa
#endif //VERSION_TEST_H_
