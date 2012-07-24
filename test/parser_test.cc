// Copyright 2012 MulticoreWare Inc.

#include "./gtest/gtest.h"
#include "./lexer.h"
#include "../parser.h"


// ------------------ Parser TESTS -----------------

TEST(ParserTest, IdentifierTest) {
  std::string input("&a_global_id123");  // global id
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Identifier(yylex()));

  input.assign("%a_local_id");  // local id
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Identifier(yylex()));

  input.assign("$d7");  // register
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Identifier(yylex()));
}

TEST(ParserTest, BaseOperandTest) {
  std::string input("1352");  // Int constant
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, BaseOperand(yylex()));

  input.assign("WAVESIZE");  // TOKEN_WAVE_SIZE
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, BaseOperand(yylex()));

  input.assign("_u32(12, 13 ,14)");  // decimalListSingle
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, BaseOperand(yylex()));
}

TEST(ParserTest, AddressableOperandTest) {
  std::string input("[%local_id]");  // Int constant
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%local_id <100> ]");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%local_id<$d7>]");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%global_id<$q5 + 10 >]");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, AddressableOperand(yylex()));

  input.assign("[%global_id<$d6 - 10 >]");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, AddressableOperand(yylex()));
}

TEST(ParserTest, QueryTest) {
  // test the Query types;
  std::string input("query_order_u32  $c1 , [&Test<$d7  + 100>];");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Query(yylex()));

  input.assign("query_data_u32  $c1 , [&Test<$d7  + 100>];");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Query(yylex()));

  input.assign("query_array_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_width_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_height_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_depth_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_normalized_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_filtering_u32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));

  // test the dataTypes;
  input.assign("query_order_s32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s64  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u64  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b1  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f32  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f64  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b64  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_b128  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u8x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s8x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u16x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s16x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f16x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f32x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u8x8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s8x8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u16x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s16x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f16x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u8x16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s8x16  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u16x8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s16x8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f16x8  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s32x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u32x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f64x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s64x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u64x2  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));

  // test for Operand
  // 1. Identifier
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u32x4  $s1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s64x2  %a1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u64x2  &a1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  // 2. BaseOperand


  // test for AddressableOperand
  input.assign("query_order_f32x4  $c1 , [%Test<100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  - 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_u32x4  $s1 , [&Test];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  input.assign("query_order_s64x2  %a1 , [%Test<$d7>];");
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
}

TEST(ParserTest, Bug57) {
  std::string input("($d4,&global_id, %local_id)");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, ArrayOperandList(yylex()));
}

TEST(ParserTest, RoundingMode) {
  bool is_ftz = false;
  int current_token;
  std::string input("_upi");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_downi");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_zeroi");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_neari");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_up");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_down");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_zero");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_near");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_ftz_up");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_ftz_down");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_ftz_zero");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_ftz_near");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));

  input.assign("_ftz");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, RoundingMode(yylex(), &is_ftz, &current_token));
}


TEST(ParserTest, Instruction2) {
  // with packing

  std::string input("abs_p_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  // with _ftz and packing
  input.assign("abs_ftz_p_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  // with _ftz floatRounding and packing
  input.assign("abs_ftz_up_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  // without roundingMode or packing
  input.assign("abs_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  // with _ftz
  input.assign("abs_ftz_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  // with _ftz floatRounding
  input.assign("abs_ftz_up_s8x4 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  input.assign("unpack2 $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  input.assign("unpack2_ftz $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  input.assign("unpack2_ftz_zero $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));


  input.assign("unpack2_neari $s1, $s2;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  input.assign("frsqrt_ftz_f32 $s1, $s0;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));

  input.assign("frsqrt_ftz_f32 $s1, $s0;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Instruction2(yylex()));
}

TEST(ParserTest, VersionStatement) {
  std::string input("version 1:0;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex()));

  input.assign("version 1:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex()));
}

TEST(ParserTest, AlignStatement) {
  std::string input("align 8");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Alignment(yylex()));
}

TEST(ParserTest, DeclPrefix) {
  bool recheck;
  int last_token;
  std::string input("align 8");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));

  input.assign("align 8 static");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));

  input.assign("align 8 extern const");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));

  input.assign("extern const");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));

  input.assign("extern const align 1");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));

  input.assign("const extern");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, DeclPrefix(yylex(), &recheck, &last_token));
}

TEST(ParserTest, FBar) {
  std::string input(":fbar(1)");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, FBar(yylex()));
}

TEST(ParserTest, ArrayDimensionSet) {
  bool rescan = false;
  int last_tok = 0;
  std::string input("[]");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArrayDimensionSet(yylex(), &rescan, &last_tok));

  input.assign("[1]");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArrayDimensionSet(yylex(), &rescan, &last_tok));

  input.assign("[1][2][][3]");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArrayDimensionSet(yylex(), &rescan, &last_tok));
}

TEST(ParserTest, ArgumentDecl) {
  bool rescan = false;
  int last_tok = 0;

  // test 1
  std::string input("const static arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

  // test 2
  input.assign("align 8 const static arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

  // test 3
  input.assign("align 8 arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

    // test 4
  input.assign("extern arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

    // test 5
  input.assign("const align 8 arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

    // test 6
  input.assign("const static align 8 arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

  // test 7
  input.assign("const align 8 static arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

      // test 8
  input.assign("static const align 8 arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));

      // test 9
  input.assign("static align 8 arg_u32 %local_id[2][2] ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentDecl(yylex(), &rescan, &last_tok));
}


TEST(ParserTest, ArgumentListBody) {
  bool rescan = false;
  int last_tok = 0;

  // test 1
  std::string input("const static arg_u32 %local_id[2][2],");
  input.append("static arg_f16 %local_id[], align 8 arg_u64 %test ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, ArgumentListBody(yylex(), &rescan, &last_tok));
}

TEST(ParserTest, FunctionDefinition) {
  bool rescan = false;
  int last_tok = 0;

  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0) :fbar(1)");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, FunctionDefinition(yylex(), &rescan, &last_tok));
}

TEST(ParserTest, FunctionDecl) {
  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0) :fbar(1);");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, FunctionDecl(yylex()));
}

TEST(ParserTest, Codeblock) {
  // test 1
  std::string input("{ abs_p_s8x4 $s1, $s2; abs_s8x4 $s1, $s2; }; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Codeblock(yylex()));
}

TEST(ParserTest, Function) {
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append(" (arg_u32 %arg_val0) :fbar(1)");
  input.append("{ abs_p_s8x4 $s1, $s2; abs_s8x4 $s1, $s2; };");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Function(yylex()));
}

TEST(ParserTest, SimpleProg) {
  std::string input("version 1:0:$small;");
  input.append("function &get_global_id(arg_u32 %ret_val)");
  input.append(" (arg_u32 %arg_val0);");
  input.append("function &abort() (); ");

  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));
};

TEST(ParserTest, ProgWithFunctionDefinition) {
  // Example 3
  std::string input("version 1:0:$small;");
  input.append("function &packed_ops (arg_u8x4 %x)() {");
  input.append(" abs_p_s8x4 $s1, $s2; ");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; ");
  input.append(" query_order_u32  $c1 , [&Test<$d7  + 100>];");
  input.append(" }; ");

  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));

  // Example 2
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &return_true(arg_f32 %ret_val) () {");
  input.append(" ret;");
  input.append(" }; ");

  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));

  // Example 4
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &branch_ops (arg_u8x4 %x)() {");
  input.append("cbr $c1, @then;");
  input.append("abs_p_s8x4 $s1, $s2;");
  input.append(" brn @outof_IF;");
  input.append("@then: add_pp_sat_u16x2 $s1, $s0, $s3;");
  input.append(" @outof_IF: ret;");
  input.append(" }; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));

  // Example 5 - Call to simple function
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &callee()() {");
  input.append("ret;");
  input.append("};");

  input.append(" function &caller()() {");
  input.append("{call &callee;}");
  input.append(" }; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));

  // Example 6 - Call to a complex function
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {");
  input.append("ret;");
  input.append("};");

  input.append(" function &caller()() {");
  input.append("{call &callee (%output)(%input);}");
  input.append(" }; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Program(yylex()));
};

TEST(ParserTest, Instruction3) {
  std::string input(" add_pp_sat_u16x2 $s1, $s0, $s3;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Instruction3(yylex()));
};

TEST(ParserTest, OptionalWidth) {
  std::string input("_width(all)");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, OptionalWidth(yylex()));

  input.assign("_width(32)");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, OptionalWidth(yylex()));
};

TEST(ParserTest, BranchOperation) {
  bool rescan = false;
  int last_tok = 0;

  std::string input("cbr_width(all)_fbar $s1, @then;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("cbr_width(all)_fbar $c1, 10 , [@first, @then];");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("cbr_width(all)_fbar $c1, &global;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("cbr_width(all)_fbar $c1, 5, [%local]; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("cbr_width(all)_fbar $c1, 10, @label; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("brn_width(all)_fbar &global; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("brn_width(all)_fbar @goto; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("brn_width(all)_fbar &global, [%local]; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("brn_width(all)_fbar &global, [@goto]; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));

  input.assign("cbr $s1, @then; ");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Branch(yylex()));
};

TEST(ParserTest, ParseCallTargets) {
  std::string input("[&global, %local]");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, CallTargets(yylex()));
};

TEST(ParserTest, ParseCallArgs) {
  std::string input("()");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, CallArgs(yylex()));

  input.assign("(&a,%b,%c)");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, CallArgs(yylex()));

  input.assign("(1,2,3)");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, CallArgs(yylex()));
};

TEST(ParserTest, Call) {
  std::string input("call &callee (%output)(%input);");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Call(yylex()));

  input.assign("call_width(all) &callee ");
  input.append("(%output1,&output2)(%input1, $d7) [&id1, &id2];");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, Call(yylex()));
};
