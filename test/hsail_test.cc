#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../tokens.h"


TEST(LexTest, Bug2) {
  std::string input("12345");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(2,yylex());
}

TEST(LexTest, Bug3) {
  std::string input("020");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(3,yylex());
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

TEST(LexTest, Bug7) {
  std::string input("$q5");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_QREGISTER,yylex());
}

TEST(LexTest, Bug8) {
  std::string input("@Go_to_this");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_LABEL,yylex());
}

TEST(LexTest, Bug9) {
  std::string input("/* this is a comment */");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_COMMENT,yylex());
}

TEST(LexTest, Bug10) {
  std::string input("//this is an inline comment\n");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_COMMENT,yylex());
}


TEST(LexTest, Bug11) {
  std::string input("0x2f");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(4,yylex());
}


TEST(LexTest, Bug17) {
  std::string input("%Test_id_123");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER,yylex());
}


TEST(LexTest, Bug18) {
  std::string input("&Test_global_id_123");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER,yylex());
}

TEST(LexTest, Bug19) {
  std::string input("\" This is a string\"");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_STRING,yylex());
}

TEST(LexTest, Bug20) {
  std::string input("snorm_int8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("unorm_int16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("rx");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("intensity");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("wrap");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());

  
}