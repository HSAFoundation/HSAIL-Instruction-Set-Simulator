// Copyright 2012 MulticoreWare Inc.

#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"
#include "../include/brig.h"
#include "../include/brig_buffer.h"

using namespace hsa::brig;

Context* context = new Context();
TEST(CodegenTest, Version) {
  BrigDirectiveVersion version_brig;
  
  std::string input("version 1:0;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), &version_brig));
  
  // append directive 
  context->append_d(&version_brig);
  // get structure back
  BrigDirectiveVersion *get  = context->get_d<BrigDirectiveVersion>();
 
  // compare two structs      
  EXPECT_EQ(version_brig.kind, get->kind);
  EXPECT_EQ(version_brig.major, get->major);
  EXPECT_EQ(version_brig.minor, get->minor);
  EXPECT_EQ(version_brig.machine, get->machine);
  EXPECT_EQ(version_brig.profile, get->profile);
  EXPECT_EQ(version_brig.ftz, get->ftz);

    /* TEST 2 */
  input.assign("version 2:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), &version_brig));
  
  // append directive 
  context->append_d(&version_brig);
  // get structure back
  get  = context->get_d<BrigDirectiveVersion>();
 
  // compare two structs      
  EXPECT_EQ(version_brig.kind, get->kind);
  EXPECT_EQ(version_brig.major, get->major);
  EXPECT_EQ(version_brig.minor, get->minor);
  EXPECT_EQ(version_brig.machine, get->machine);
  EXPECT_EQ(version_brig.profile, get->profile);
  EXPECT_EQ(version_brig.ftz, get->ftz);

}