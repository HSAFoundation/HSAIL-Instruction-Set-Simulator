#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../tokens.h"

extern int int_val;
extern float float_val;

TEST(LexTest, Bug1) {
  std::string input("foo");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(1,yylex());
}

TEST(LexTest, Bug2) {
  std::string input("12345");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(12345,int_val);
}

TEST(LexTest, Bug3) {
  std::string input("020");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(16,int_val);
}
TEST(LexTest, Bug4) {
  std::string input("$c7");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_CREGISTER,yylex());
}

TEST(LexTest, Bug5) {
  std::string input("$d7");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DREGISTER,yylex());
}

TEST(LexTest, Bug6) {
  std::string input("$s15");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SREGISTER,yylex());
}

TEST(LexTest, Bug11) {
  std::string input("0x11");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(17,int_val);
}
TEST(LexTest, Bug12) {
  std::string input("0.5e3f");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(0.5e3f, float_val);
}

TEST(LexTest, Bug23) {
  std::string input("0x1.0p0f");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(1.0f, float_val);
}

TEST(LexTest, Bug24) {
  std::string input("0f3F800000");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(1.0f, float_val);
}

TEST(LexTest, Bug26) {
  std::string input("0.5e3l");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(8,yylex());
}

TEST(LexTest, Bug28) {
  std::string input("0xA.0l");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(9,yylex());
}

TEST(LexTest, Bug29) {
  std::string input("0d0000000000000001");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(10,yylex());
}
