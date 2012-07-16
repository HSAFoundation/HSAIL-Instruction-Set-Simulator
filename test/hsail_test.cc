#include "gtest/gtest.h"
#include "lexer.h"
#include <string>



TEST(LexTest, Bug1) {
  std::string input("foo");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(1,yylex());
}

TEST(LexTest, Bug2) {
  std::string input("12345");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(2,yylex());
}