/* Copyright 2012 <MulticorewareInc> */

#ifndef DIRECTIVE_TEST_H_
#define DIRECTIVE_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestPragma: public ::testing::TestWithParam<int>
{

};

struct PragmaTest{
  const char * str;
  BrigDirectivePragma ref;
};

struct PragmaTest pragma_pair[1] = {
  {
    "pragma \"test\";",
     {
  brig_directive_pragma_size, //size
  BrigEDirectivePragma, //kind
  code_offset,     //c_code
  string_offset,   //s_name
  }
  }
};

class TestPragmaInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_pragma_invalid[] = {
  "pragma \"test\"",
  "pragma \"test;",
  "pragma test\";",
  "pragm \"test\";",
  "pragma;"
};

class TestExtension: public ::testing::TestWithParam<int>
{

};

struct ExtensionTest{
  const char * str;
  BrigDirectiveExtension ref;
};

struct ExtensionTest extension_pair[2] = {
//extension_pair[0]
  {
    "extension \"amd:newext\";",
   {
  brig_directive_extension_size, //size
  BrigEDirectiveExtension, //kind
  code_offset,        //c_code
  string_offset,      //s_name
  }
  },
//extension_pair[1]
  {
    "gcn_region_alloc 9;",
    {
  brig_directive_extension_size, //size
  BrigEDirectiveExtension, //kind
  code_offset,        //c_code
  string_offset,      //s_name
    }
  }
};

class TestExtensionInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_extension_invalid[] = {
  "extension \"amd:newext\"",
  "extension \"amd:newext;",
  "extension amd:newext\";",
  "extensiona \"amd:newext\";",
  "gcn_region_alloc 9",
  "gcn_region_alloc ;",
  "gcn_region_allo 9;"
};

class TestControl: public ::testing::TestWithParam<int>
{

};

struct ControlTest{
  const char * str;
  BrigDirectiveControl ref;
};

struct ControlTest control_pair[4] = {
//control_pair[0]
  {
    "itemsperworkgroup 7, 8, 9;",
    {
    brig_directive_control_size, //size
    BrigEDirectiveControl, //kind
    code_offset,      //c_code
    BrigEMaxWIperG,           //control type
    {7,8,9}                //values[3]
  }
  },
//control_pair[1]
  {
    "workgroupspercu 8;",
      {
    brig_directive_control_size, //size
    BrigEDirectiveControl, //kind
    code_offset,       //c_code
    BrigEMaxGperC,           //control type
    {8,0,0}                //values[3]
    }
  },
//control_pair[2]
  {
     "memopt_on;",
      {
    brig_directive_control_size,//size
    BrigEDirectiveControl, //kind
    code_offset,      //c_code
    BrigEMemOpt,           //control type
    {1,0,0}                //values[3]
    }
  },
//control_pair[3]
  {
    "memopt_off;" ,
      {
    brig_directive_control_size, //size
    BrigEDirectiveControl, //kind
    code_offset,      //c_code
    BrigEMemOpt,           //control type
    {0,0,0}                //values[3]
    }
  }
};

class TestControlInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_control_invalid[] = {
  "itemsperworkgroup 7, 8, 9",
  "itemsperworkgroup 7, 8, ",
  "itemsperworkgroup 7, 8 9;",
  "workgroupspercu ;",
  "memopt_on 9;",
  "memopt_off 10;" 
};

class TestFileDecl: public ::testing::TestWithParam<int>
{

};

struct FileDeclTest{
  const char * str;
  BrigDirectiveFile ref;
};

struct FileDeclTest filedecl_pair[2] = {
//filedecl_pair[0]
  {
     "file 1 \"\" ;",
     {
    brig_directive_file_size, //size
    BrigEDirectiveFile,   //kind
    code_offset,     //c_code
    1,                    //fileid
    0                     //s_filename
  }
  },
//filedecl_pair[1]
  {
     "file 2 \"this is a file\";",
     {
    brig_directive_file_size, //size
    BrigEDirectiveFile,   //kind
    code_offset,     //c_code
    2,                    //fileid
    string_offset    //s_filename
  }
  }
};

class TestFileDeclInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_filedecl_invalid[] = {
  "file 1 \";",
  "file 2 this is a file\";",
  "file  \"this is a file\";",
  "fil 3 \"this is a file\";",
  "file 3 \"this is a file\" ",
  "file 1.0f \"this is a file\";",
  "file -1 \"this is a file\";"
};


}  // namespace brig
}  // namespace hsa
#endif //DIRECTIVE_TEST_H_
