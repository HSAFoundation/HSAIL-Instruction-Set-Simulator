// Copyright 2012 MulticoreWare Inc.
#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"


// ------------------ Parser TESTS -----------------

TEST(ParserTest, IdentifierTest) {
  std::string input("&a_global_id123");  // global id
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, Identifier(yylex()));

  input.assign("%a_local_id");  // local id
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, Identifier(yylex()));

  input.assign("$d7");  // register
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, Identifier(yylex()));
}

TEST(ParserTest, BaseOperandTest) {
  std::string input("1352");  // Int constant
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, BaseOperand(yylex()));

  input.assign("WAVESIZE");  // TOKEN_WAVE_SIZE
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, BaseOperand(yylex()));

  input.assign("_u32(12, 13 ,14)");  // decimalListSingle
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, BaseOperand(yylex()));
}

TEST(ParserTest, AddressableOperandTest) {
  std::string input("[%local_id]");  // Int constant
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%local_id <100> ]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%local_id<$d7>]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%global_id<$q5 + 10 >]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%global_id<$d6 - 10 >]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, AddressableOperand(yylex()));
}

TEST(ParserTest, QueryTest) {
  // test the Query types;
  std::string input("query_order_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_data_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_array_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_width_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_height_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_depth_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_normalized_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_filtering_u32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));

  // test the dataTypes;
  input.assign("query_order_s32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s64  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u64  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b1  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f32  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f64  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b64  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_b128  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u8x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s8x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u16x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s16x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f16x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f32x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u8x8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s8x8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u16x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s16x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f16x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u8x16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s8x16  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u16x8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s16x8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f16x8  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s32x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u32x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f64x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s64x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u64x2  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));

  // test for Operand
  // 1. Identifier
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u32x4  $s1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s64x2  %a1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u64x2  &a1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  // 2. BaseOperand


  // test for AddressableOperand
  input.assign("query_order_f32x4  $c1 , [%Test<100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  - 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_u32x4  $s1 , [&Test]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>]");
  ASSERT_EQ(0, Parse(input));
  input.assign("query_order_s64x2  %a1 , [%Test<$d7>]");
  ASSERT_EQ(0, Parse(input));
}

TEST(ParserTest, Bug55) {
  std::string input("query_order_u32 _u32(1,2,3), [%a_local_id]");
  EXPECT_EQ(0, Parse(input));
}

TEST(ParserTest, Bug57) {
  std::string input("($d4,&global_id, %local_id)");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(0, ArrayOperandList(yylex()));
}

TEST(ParserTest, Bug58) {
  std::string input("query_order_u32 _f32(1.2,.2,3.4, 5.6), [%a_local_id]");
  EXPECT_EQ(0, Parse(input));
}
