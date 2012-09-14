/* Copyright 2012 <MulticorewareInc> */

#ifndef VERSION_TEST_H_
#define VERSION_TEST_H_

namespace hsa {
namespace brig {

class CodegenTestVersion: public ::testing::TestWithParam<int>
{

};

std::string inputarray_version[] = {
  "version 1:0; ",
  "version 2:0:$large;",
  "version 2:0:$small;",
  "version 2:0:$reduced;",
  "version 2:0:$full;",
  "version 2:0:$sftz;",
  "version 2:0:$nosftz;",
  "version 2:0:$reduced, $small;",
  "version 2:0:$reduced, $large;",
  "version 2:0:$full, $small;",
  "version 2:0:$full, $large;",
  "version 2:0:$sftz, $reduced;",
  "version 2:0:$nosftz, $reduced;",
  "version 2:0:$sftz, $full;",
  "version 2:0:$nosftz, $full;",
  "version 2:0:$large, $sftz;",
  "version 2:0:$large, $nosftz;",
  "version 2:0:$small, $sftz;",
  "version 2:0:$small, $nosftz;",
  "version 2:0:$large, $reduced, $sftz;",
  "version 2:0:$large, $reduced, $nosftz;",
  "version 2:0:$large, $full, $sftz;",
  "version 2:0:$large, $full, $nosftz;",
  "version 2:0:$small, $reduced, $sftz;",
  "version 2:0:$small, $reduced, $nosftz;",
  "version 2:0:$small, $full, $sftz;",
  "version 3:2:$small, $full, $nosftz;",
  "version 2:0:$large, $small;",
  "version 2:0:$full, $reduced, $sftz;",
  "version 3:2:$small, $full, $nosftz;",
  "version 3:2:$small, $full, $nosftz, $sftz;",
  "version 3:2:$small, $full, $nosftz, $sftz, $large;",
  "version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced;",
  "version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced, $small;"
};

BrigDirectiveVersion outputarray_version[] = {
//"version 1:0; "
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    1,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$large;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$small;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$reduced"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$full;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$reduced, $small;"
 {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
// "version 2:0:$reduced, $large;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
// "version 2:0:$full, $small;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$full, $large;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$sftz, $reduced;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$nosftz, $reduced;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$sftz, $full;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$nosftz, $full;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$large, $stfz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$large, $nostfz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$small, $stfz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigESftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$small, $nostfz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$large, $reduced, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$large, $reduced, $nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigENosftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$large, $full, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$large, $full, $nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEFull,              // profile
    BrigENosftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$small, $reduced, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,              // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$small, $reduced, $nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEReduced,              // profile
    BrigENosftz,              // ftz
    0                       // reserved
  },
//"version 2:0:$small, $full, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$large, $small;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigESmall,             // machine
    BrigEFull,              // profile
    BrigENosftz,            // ftz
    0                       // reserved
  },
//"version 2:0:$full, $reduced, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    2,                      // major
    0,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigENosftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz, $sftz;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEFull,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz, $sftz, $large;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigELarge,             // machine
    BrigEFull,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigELarge,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
//"version 3:2:$small, $full, $nosftz, $sftz, $large, $reduced, $small;"
  {
    20,                     //size
    BrigEDirectiveVersion,  //kind
    0,                      // c_code
    3,                      // major
    2,                      // minor
    BrigESmall,             // machine
    BrigEReduced,           // profile
    BrigESftz,              // ftz
    0                       // reserved
  },
};

class TestVersionFalseInput: public ::testing::TestWithParam<int>
{

};

std::string inputarray_version_false[] = {
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
