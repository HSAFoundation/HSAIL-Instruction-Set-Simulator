#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../tokens.h"

extern int int_val;
extern float float_val;
extern double double_val;


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
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(0.5e3, double_val);
}

TEST(LexTest, Bug28) {
  std::string input("0x1.0l");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(1.0, double_val);
}

TEST(LexTest, Bug29) {
  std::string input("0d3FF0000000000000");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(1.0, double_val);
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

TEST(LexTest, Bug21) {
  std::string input("WAVESIZE");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_WAVESIZE,yylex());
}

TEST(LexTest, Bug22) {			// common keywords


  std::string input("workgroupid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKGROUPID,yylex());
  
  input.assign("version");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(VERSION,yylex());
  
  input.assign("global");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(GLOBAL,yylex());
  
  input.assign("_Samp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SAMP,yylex());
  
  input.assign("_RWImg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_RWIMG,yylex());
  
  input.assign("_ROImg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ROIMG,yylex());
  
  input.assign("align");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ALIGN,yylex());
  
  input.assign("file");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FILE,yylex());
  
  input.assign("arg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ARG,yylex());
  
    input.assign("kernarg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(KERNARG,yylex());
  
  input.assign("function");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FUNCTION,yylex());
  
  input.assign(":fbar");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FBAR,yylex());
  
  input.assign("signature");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SIGNATURE,yylex());
  
    input.assign("block");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCK,yylex());
  
  input.assign("endblock");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ENDBLOCK,yylex());
  
  input.assign("blocknumeric");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCKNUMERIC,yylex());
  
    input.assign("blockstring");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCKSTRING,yylex());
  
  input.assign("kernel");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(KERNEL,yylex());
  
  input.assign("pragma");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(PRAGMA,yylex());
  
  input.assign("labeltargets");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LABELTARGETS,yylex());
  
    input.assign("extension");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(EXTENSION,yylex());
  
  input.assign("extern");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(EXTERN,yylex());
  
  input.assign("static");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(STATIC,yylex());
  
    input.assign("const");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CONST,yylex());
  
  input.assign("private");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(PRIVATE,yylex());
  
  input.assign("spill");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SPILL,yylex());
  
  input.assign("group");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(GROUP,yylex());
 
   input.assign("readonly");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(READONLY,yylex());
  
  input.assign("loc");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LOC,yylex());
}

TEST(LexTest, Bug25) {			// addressSpaceIdentifier keywords
  std::string input("_readonly");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_READONLY,yylex());
  
  input.assign("_kernarg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_KERNARG,yylex());
  
  input.assign("_global");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GLOBAL,yylex());
  
  input.assign("_private");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PRIVATE,yylex());
  
  input.assign("_arg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ARG,yylex());
  
  input.assign("_group");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GROUP,yylex());
  
  input.assign("_spill");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SPILL,yylex());
  
 }
 
 TEST(LexTest, Bug27) {				// keywords _v2 and _v4
  std::string input("_v2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V2,yylex());
  
  input.assign("_v4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V4,yylex());
}

 TEST(LexTest, Bug30) {				// keywords _v2 and _v4
  std::string input("format");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FORMAT,yylex());
  
  input.assign("order");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ORDER,yylex());
  
  input.assign("coord");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(COORD,yylex());
  
  input.assign("filter");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FILTER,yylex());
  
  input.assign("boundaryU");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYU,yylex());
  
  input.assign("boundaryV");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYV,yylex());
  
  input.assign("boundaryW");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYW,yylex());
}

 TEST(LexTest, Bug45) {				// keywords for queryOp
  std::string input("query_order");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_ORDER,yylex());
  
  input.assign("query_data");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_DATA,yylex());
  
  input.assign("query_array");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_ARRAY,yylex());
  
  input.assign("query_width");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_WIDTH,yylex());
  
  input.assign("query_depth");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_DEPTH,yylex());
  
  input.assign("query_height");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_HEIGHT,yylex());
  
  input.assign("query_normalized");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_NORMALIZED,yylex());

  input.assign("query_filtering");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_FILTERING,yylex());

}

 TEST(LexTest, Bug48) {				// keywords for queryOp
  std::string input("_1d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1D,yylex());
  
  input.assign("_2d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_2D,yylex());
  
  input.assign("_3d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_3D,yylex());
  
  input.assign("_1db");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1DB,yylex());
  
  input.assign("_1da");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1DA,yylex());
  
  input.assign("_2da");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_2DA,yylex());
  
  input.assign("rd_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(RD_IMAGE,yylex());

  input.assign("ld_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LD_IMAGE,yylex());

  input.assign("st_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ST_IMAGE,yylex());

}
