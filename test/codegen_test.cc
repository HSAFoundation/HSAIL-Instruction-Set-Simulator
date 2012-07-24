// Copyright 2012 MulticoreWare Inc.

#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"
#include "../include/brig.h"
/*
TEST(CodegenTest, Version) {
  std::string input("version 1:0;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  BrigDirectiveVersion version_brig;
  EXPECT_EQ(0, Version(yylex(), &version_brig));
  // test returned structure
  EXPECT_EQ(BrigEDirectiveVersion, version_brig.kind);
  EXPECT_EQ(1, version_brig.major);
  EXPECT_EQ(0, version_brig.minor);
  EXPECT_EQ(BrigESmall, version_brig.machine);
  EXPECT_EQ(BrigEFull, version_brig.profile);
  EXPECT_EQ(BrigENosftz, version_brig.ftz);
  EXPECT_EQ(0, version_brig.reserved);

  input.assign("version 2:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), &version_brig));
    // test returned structure
  EXPECT_EQ(BrigEDirectiveVersion, version_brig.kind);
  EXPECT_EQ(2, version_brig.major);
  EXPECT_EQ(0, version_brig.minor);
  EXPECT_EQ(BrigELarge, version_brig.machine);
  EXPECT_EQ(BrigEFull, version_brig.profile);
  EXPECT_EQ(BrigENosftz, version_brig.ftz);
  EXPECT_EQ(0, version_brig.reserved);
}
*/
