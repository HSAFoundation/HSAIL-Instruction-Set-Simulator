// Copyright 2012 MulticoreWare Inc.

#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "lexer.h"
#include "parser.h"
#include "parser_wrapper.h"

namespace hsa {
namespace brig {
extern Context* context;

// ------------------  BASIC PARSER TEST -----------------

TEST(ParserTest, OperandTest) {
  Lexer* lexer = new Lexer();
  std::string input("&a_global_id123");  // global id
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));

  input.assign("%a_local_id");  // local id
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));

  input.assign("$d7");  // register
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));

  input.assign("1352");  // Int constant
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));

  input.assign("WAVESIZE");  // TOKEN_WAVE_SIZE
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));

  input.assign("_u32(12, 13 ,14)");  // decimalListSingle
  lexer->set_source_string(input);
  EXPECT_EQ(0, Operand(lexer->get_next_token(), context));
}

TEST(ParserTest, AddressableOperandTest) {
  Lexer* lexer = new Lexer();
  std::string input("[%local_id]");  // Int constant
  lexer->set_source_string(input);
  EXPECT_EQ(0, AddressableOperand(lexer->get_next_token(), context));

  input.assign("[%local_id <100> ]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, AddressableOperand(lexer->get_next_token(), context));

  input.assign("[%local_id<$d7>]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, AddressableOperand(lexer->get_next_token(), context));

  input.assign("[%global_id<$q5 + 10 >]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, AddressableOperand(lexer->get_next_token(), context));

  input.assign("[%global_id<$d6 - 10 >]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, AddressableOperand(lexer->get_next_token(), context));
}

TEST(ParserTest, QueryTest) {
  Lexer* lexer = new Lexer();
  // test the Query types;
  std::string input("   query_order_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Query(lexer->get_next_token(), context));

  input.assign("    query_data_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Query(lexer->get_next_token(), context));

  input.assign("query_array_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_width_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_height_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_depth_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_normalized_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_filtering_u32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);

  // test the dataTypes;
  input.assign("query_order_s32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s64  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u64  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b1  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f32  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f64  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b64  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_b128  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u8x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s8x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u16x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s16x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f16x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f32x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u8x8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s8x8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u16x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s16x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f16x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u8x16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s8x16  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u16x8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s16x8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f16x8  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s32x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u32x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f64x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s64x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u64x2  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);

  // test for Operand
  // 1. Identifier
  input.assign("query_order_f32x4  $c1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u32x4  $s1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s64x2  %a1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u64x2  &a1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  // 2. BaseOperand


  // test for AddressableOperand
  input.assign("query_order_f32x4  $c1 , [%Test<100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s32x4  $d1 , [&Test<$d7  - 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_u32x4  $s1 , [&Test];");
  lexer->set_source_string(input);
  input.assign("query_order_f64x2  $q1 , [&Test<$d7  + 100>];");
  lexer->set_source_string(input);
  input.assign("query_order_s64x2  %a1 , [%Test<$d7>];");
  lexer->set_source_string(input);
}

TEST(ParserTest, ArrayOperandList) {
  Lexer* lexer = new Lexer();
  std::string input("($d4,&global_id, %local_id)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArrayOperandList(lexer->get_next_token(), context));
}

TEST(ParserTest, RoundingMode) {
  bool is_ftz = false;
  unsigned int current_token;
  Lexer* lexer = new Lexer();
  std::string input("_upi");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_downi");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_zeroi");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_neari");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_up");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_down");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_zero");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_near");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_ftz_up");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_ftz_down");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_ftz_zero");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_ftz_near");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));

  input.assign("_ftz");
  lexer->set_source_string(input);
  EXPECT_EQ(0, RoundingMode(lexer->get_next_token(),
                            &is_ftz,
                            &current_token,
                            context));
}

TEST(ParserTest, Instruction2) {
  // with packing
  Lexer* lexer = new Lexer();
  std::string input("abs_p_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  // with _ftz and packing
  input.assign("abs_ftz_p_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  // with _ftz floatRounding and packing
  input.assign("abs_ftz_up_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  // without roundingMode or packing
  input.assign("abs_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  // with _ftz
  input.assign("abs_ftz_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  // with _ftz floatRounding
  input.assign("abs_ftz_up_s8x4 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  input.assign("unpack2 $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  input.assign("unpack2_ftz $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  input.assign("unpack2_ftz_zero $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));


  input.assign("unpack2_neari $s1, $s2;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  input.assign("frsqrt_ftz_f32 $s1, $s0;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));

  input.assign("frsqrt_ftz_f32 $s1, $s0;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction2(lexer->get_next_token(), context));
}

TEST(ParserTest, VersionStatement) {
  Lexer* lexer = new Lexer();
  std::string input("version 1:0;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));


  input.assign("version 2:0:$large;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
}

TEST(ParserTest, AlignStatement) {
  Lexer* lexer = new Lexer();
  std::string input("align 8");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Alignment(lexer->get_next_token(), context));
}

TEST(ParserTest, DeclPrefix) {
  Lexer* lexer = new Lexer();
  bool recheck;
  unsigned int last_token;
  std::string input("align 8");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));

  input.assign("align 8 static");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));

  input.assign("align 8 extern const");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));

  input.assign("extern const");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));

  input.assign("extern const align 1");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));

  input.assign("const extern");
  lexer->set_source_string(input);
  EXPECT_EQ(0, DeclPrefix(lexer->get_next_token(),
                          &recheck,
                          &last_token,
                          context));
}

TEST(ParserTest, FBar) {
  Lexer* lexer = new Lexer();
  std::string input(":fbar(1)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, FBar(lexer->get_next_token(), context));
}

TEST(ParserTest, ArrayDimensionSet) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  unsigned int last_tok = 0;
  std::string input("[]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArrayDimensionSet(lexer->get_next_token(),
                                 &rescan,
                                 &last_tok,
                                 context));

  input.assign("[1]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArrayDimensionSet(lexer->get_next_token(),
                                 &rescan,
                                 &last_tok,
                                 context));

  input.assign("[1][2][][3]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArrayDimensionSet(lexer->get_next_token(),
                                 &rescan,
                                 &last_tok,
                                 context));
}

TEST(ParserTest, ArgumentDecl) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  unsigned int last_tok = 0;

  // test 1
  std::string input("const static arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

  // test 2
  input.assign("align 8 const static arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

  // test 3
  input.assign("align 8 arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

    // test 4
  input.assign("extern arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

    // test 5
  input.assign("const align 8 arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

    // test 6
  input.assign("const static align 8 arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

  // test 7
  input.assign("const align 8 static arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

      // test 8
  input.assign("static const align 8 arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));

      // test 9
  input.assign("static align 8 arg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentDecl(lexer->get_next_token(),
                            &rescan,
                            &last_tok,
                            context));
}

TEST(ParserTest, ArgumentListBody) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  unsigned int last_tok = 0;

  // test 1
  std::string input("const static arg_u32 %local_id[2][2],");
  input.append("static arg_f16 %local_id[], align 8 arg_u64 %test ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgumentListBody(lexer->get_next_token(),
                                &rescan,
                                &last_tok,
                                context));
}

TEST(ParserTest, FunctionDefinition) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  unsigned int last_tok = 0;

  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0) :fbar(1)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, FunctionDefinition(lexer->get_next_token(),
                                  &rescan,
                                  &last_tok,
                                  context));
}

TEST(ParserTest, FunctionDecl) {
  Lexer* lexer = new Lexer();
  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0) :fbar(1);");
  lexer->set_source_string(input);
  EXPECT_EQ(0, FunctionDecl(lexer->get_next_token(), context));
}

TEST(ParserTest, Codeblock) {
  Lexer* lexer = new Lexer();
  // test 1
  std::string input("{ abs_p_s8x4 $s1, $s2; abs_s8x4 $s1, $s2; }; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Codeblock(lexer->get_next_token(), context));
}

TEST(ParserTest, Function) {
  Lexer* lexer = new Lexer();
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append(" (arg_u32 %arg_val0) :fbar(1)");
  input.append("{ abs_p_s8x4 $s1, $s2; abs_s8x4 $s1, $s2; };");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Function(lexer->get_next_token(), context));
}

TEST(ParserTest, SimpleProg) {
  Lexer* lexer = new Lexer();
  std::string input("version 1:0:$small;");
  input.append("function &get_global_id(arg_u32 %ret_val)");
  input.append(" (arg_u32 %arg_val0);");
  input.append("function &abort() (); ");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
};

TEST(ParserTest, Instruction3) {
  Lexer* lexer = new Lexer();
  std::string input(" add_pp_sat_u16x2 $s1, $s0, $s3;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Instruction3(lexer->get_next_token(), context));
};

TEST(ParserTest, OptionalWidth) {
  Lexer* lexer = new Lexer();
  std::string input("_width(all)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, OptionalWidth(lexer->get_next_token(), context));

  input.assign("_width(32)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, OptionalWidth(lexer->get_next_token(), context));
};

TEST(ParserTest, BranchOperation) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  int last_tok = 0;

  std::string input("cbr_width(all)_fbar $s1, @then;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("cbr_width(all)_fbar $c1, 10 , [@first, @then];");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("cbr_width(all)_fbar $c1, &global;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("cbr_width(all)_fbar $c1, 5, [%local]; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("cbr_width(all)_fbar $c1, 10, @label; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("brn_width(all)_fbar &global; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("brn_width(all)_fbar @goto; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("brn_width(all)_fbar &global, [%local]; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("brn_width(all)_fbar &global, [@goto]; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));

  input.assign("cbr $s1, @then; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Branch(lexer->get_next_token(), context));
};

TEST(ParserTest, ParseCallTargets) {
  Lexer* lexer = new Lexer();
  std::string input("[&global, %local]");
  lexer->set_source_string(input);
  EXPECT_EQ(0, CallTargets(lexer->get_next_token(), context));
};

TEST(ParserTest, ParseCallArgs) {
  Lexer* lexer = new Lexer();
  std::string input("()");
  lexer->set_source_string(input);
  EXPECT_EQ(0, CallArgs(lexer->get_next_token(), context));

  input.assign("(&a,%b,%c)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, CallArgs(lexer->get_next_token(), context));

  input.assign("(1,2,3)");
  lexer->set_source_string(input);
  EXPECT_EQ(0, CallArgs(lexer->get_next_token(), context));
};

TEST(ParserTest, Call) {
  Lexer* lexer = new Lexer();
  std::string input("call &callee (%output)(%input);");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Call(lexer->get_next_token(), context));

  input.assign("call_width(all) &callee ");
  input.append("(%output1,&output2)(%input1, $d7) [&id1, &id2];");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Call(lexer->get_next_token(), context));
};

TEST(ParserTest, Initializers) {
  Lexer* lexer = new Lexer();
  bool rescan = false;
  unsigned int last_token = 0;
  std::string input("= {12, 13,14, -13}");  // DecimalInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("= 12, 13,14 ");  // DecimalInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("={ 1.2f, 1.3f,-1.4f }");  // SingleInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("= 1.2f, 1.3f,-1.4f ");  // SingleInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("={ 1.2L, 1.3L,-1.4L }");  // FloatInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("= 1.2L, 1.3L,-1.4L ");  // FloatInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));

  input.assign("= {@a, @b, @c} ");  // LabelInitializer
  lexer->set_source_string(input);
  EXPECT_EQ(0, Initializer(lexer->get_next_token(),
                           &rescan,
                           &last_token,
                           context));
};

TEST(ParserTest, InitializableDecl) {
  Lexer* lexer = new Lexer();
  // DecimalInitializer
  std::string input("readonly_s32 &x[4]= {12, 13,14, -13};");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));

  input.assign("global_u32 &x[3] = 12, 13,14 ; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));

  // SingleInitializer
  input.assign("readonly_f32 %f[3] = { 1.2f, 1.3f,-1.4f };");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));

  input.assign("global_f32 &c[3] = 1.2f, 1.3f,-1.4f ;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));

  // FloatInitializer
  input.assign("readonly_f64 %d[3] ={ 1.2L, 1.3L,-1.4L; }");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));

  input.assign("global_f64 %g[3] = 1.2L, 1.3L,-1.4L ;");
  lexer->set_source_string(input);
  EXPECT_EQ(0, InitializableDecl(lexer->get_next_token(), context));
};

TEST(ParserTest, ProgWithFunctionDefinition) {
  Lexer* lexer = new Lexer();
  // Example 3
  std::string input("version 1:0:$small;");
  input.append("function &packed_ops (arg_u8x4 %x)() {");
  input.append(" abs_p_s8x4 $s1, $s2; ");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; ");
  input.append(" }; ");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));

  // Example 2
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &return_true(arg_f32 %ret_val) () {");
  input.append(" ret;");
  input.append(" }; ");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));

  // Example 4
  input.clear();
  input.assign("version 1:1:$small;");
  input.append("function &branch_ops (arg_u8x4 %x)() {");
  input.append("cbr $c1, @then;");
  input.append("abs_p_s8x4 $s1, $s2;");
  input.append(" brn @outof_IF;");
  input.append("@then: add_pp_sat_u16x2 $s1, $s0, $s3;");
  input.append(" @outof_IF: ret;");
  input.append(" }; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));

  // Example 5 - Call to simple function
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &callee()() {");
  input.append("ret;");
  input.append("};");

  input.append(" function &caller()() {");
  input.append("{call &callee;}");
  input.append(" }; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));

  // Example 6a - Call to a complex function
  input.clear();
  input.assign("version 1:0:$small;");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {");
  input.append("ret;");
  input.append("};");

  input.append(" function &caller()() {");
  input.append(" { arg_f32 %input; ");
  input.append("  arg_f32 %output; ");
  input.append("call &callee (%output)(%input);}");
  input.append(" }; ");
  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
};

TEST(ParserTest, ProgWithGlobalDecl) {
  Lexer* lexer = new Lexer();
  std::string input("version 1:0:$small;");
  input.append("readonly_f32 %f[3] = { 1.2f, 1.3f,-1.4f };");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {");
  input.append("ret;");
  input.append("};");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
};

TEST(ParserTest, ProgWithUninitializableDecl ) {
  Lexer* lexer = new Lexer();
  // Example 1 - PRM 20.8.2
  std::string input("version 1:0:$large;");
  input.append("global_f32 &x = 2;");
  input.append("function &test()() {");
  input.append("{private_u32 %z;}");
  input.append(" }; ");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
};

TEST(ParserTest, UninitializableDecl) {
  Lexer* lexer = new Lexer();
  std::string input("private_f32 %f[3];");

  lexer->set_source_string(input);
  EXPECT_EQ(0, UninitializableDecl(lexer->get_next_token(), context));
};

TEST(ParserTest, ArgUninitializableDecl) {
  Lexer* lexer = new Lexer();
  std::string input("arg_f32 %f[3];");

  lexer->set_source_string(input);
  EXPECT_EQ(0, ArgUninitializableDecl(lexer->get_next_token(), context));
};

TEST(ParserTest, ProgWithArgUninitializableDecl ) {
  Lexer* lexer = new Lexer();
  std::string input("version 1:0:$large;");
  input.append("global_f32 &x = 2;");
  input.append("function &test()() {");
  input.append("{arg_u32 %z;}");
  input.append(" }; ");

  lexer->set_source_string(input);
  EXPECT_EQ(0, Program(lexer->get_next_token(), context));
};

TEST(ParserTest, FileDecl) {
  Lexer* lexer = new Lexer();
  std::string input("file 1 \"this is a file\";");
  lexer->set_source_string(input);
  EXPECT_EQ(0, FileDecl(lexer->get_next_token(), context));

  // wrong case
  input.assign("file 2 ;");  // lack of file string
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file \"this is a file\";");  // lack of file string
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file 2 \"this is a file\"");  // lack of ';'
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file \"this is a file\" 2;");  // reverse order
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file 1 2;");  // two integer number
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file \"file1\" \"file2\";");  // two file string
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file 1 \"file1\" \"file2\";");  // redundant file string
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file 1 2 \"file\";");  // redundant integer
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file 1.2 \"file\";");  // not integer
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file;");  // lack of number , file string
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));

  input.assign("file $s1 \"file\";");  // register not allowed
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, FileDecl(yylex(), context));
};

TEST(ParserTest, VectorToken) {
  Lexer* lexer = new Lexer();
  std::string input("_v2");

  lexer->set_source_string(input);
  EXPECT_EQ(0, VectorToken(lexer->get_next_token(), context));

  // input.clear() ;
  input.assign("_v4");
  lexer->set_source_string(input);
  EXPECT_EQ(0, VectorToken(lexer->get_next_token(), context));
};

TEST(ParserTest, SysCall) {
  // syscall dest, n, src0, src1, src2;
  // dest: must be a 32-bit register
  // n: An integer literal
  // src: must be a s reg, imm, WAVESIZE
  std::string input("syscall $s1, 3, $s2, $s3, $s4;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, SysCall(yylex(), context));

  input.assign("syscall $s1, 0xff, 1, 2, 3;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, SysCall(yylex(), context));

  input.assign("syscall $s1, 0xff, 1, $s3, 2;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, SysCall(yylex(), context));

  input.assign("syscall $s1, 0xff, 1, $s3, WAVESIZE;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, SysCall(yylex(), context));

  input.assign("syscall $s11, 0xff, WAVESIZE, $s3, WAVESIZE;");
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_EQ(0, SysCall(yylex(), context));

  // wrong case
  input.assign("syscall $d2, 0xff, $s1, $s3, $s4;");  // d register is 64-bit
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $c2, 0xff, $s1, $s3, $s4;");  // c register is 1-bit
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $q2, 0xff, $s1, $s3, $s4;");  // q register is 128-bit
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s2, 0xff, $d1, $s3, $s4;");  // src must be s register
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s2, $s4, $s1, $s3, $s4;");
  // n must be integer literal
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3, 1.1, $s1, $s3, $s4;");
  // n must be integer literal
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3, 3, $s1, $s3, $s4");  // lack of ';'
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3 3, $s1, $s3, $s4;");  // lack of ','
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3, 3 $s1, $s3, $s4;");  // lack of ','
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3, 3, $s1 $s3, $s4;");  // lack of ','
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));

  input.assign("syscall $s3, 3, $s1, $s3 $s4;");  // lack of ','
  yy_scan_string(reinterpret_cast<const char*>(input.c_str()));
  EXPECT_NE(0, SysCall(yylex(), context));
};

// ------------------  PARSER WRAPPER TEST -----------------
TEST(ParserWrapperTest, ScanSymbolsWithParser) {
  std::string input("version 1:0:$large;\n");
  input.append("global_f32 &x = 2;\n");
  input.append("function &test()() {\n");
  input.append("{arg_u32 %z;}\n");
  input.append(" }; \n");

  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  parser->scan_symbols();


  // Print out string buffer content:
  unsigned int index = 0;
  std::string temp;
  std::cout << "Buffer content: " << std::endl;
  while (index < context->get_string_offset()) {
    temp = context->get_string(index);
    std::cout << "Index " << index << ": " << temp << std::endl;
    index+=temp.length()+1;
  }
};

TEST(ParserWrapperTest, ParseSimpleProgram) {
  // Example 3
  std::string input("version 1:0:$small;");
  input.append("function &packed_ops (arg_u8x4 %x)() {");
  input.append(" abs_p_s8x4 $s1, $s2; ");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; ");
  input.append(" }; ");

  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  int result =  parser->parse();

  EXPECT_EQ(0, result);
};

TEST(ParserWrapperTest, ParseSequenceOfPrograms) {
  // Example 3
  std::string input("version 1:0:$small;");
  input.append("function &packed_ops (arg_u8x4 %x)() {");
  input.append(" abs_p_s8x4 $s1, $s2; ");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; ");
  input.append(" }; ");

  // Example 4
  input.append("version 1:1:$small;");
  input.append("function &branch_ops (arg_u8x4 %x)() {");
  input.append("cbr $c1, @then;");
  input.append("abs_p_s8x4 $s1, $s2;");
  input.append(" brn @outof_IF;");
  input.append("@then: add_pp_sat_u16x2 $s1, $s0, $s3;");
  input.append(" @outof_IF: ret;");
  input.append(" }; ");

  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  int result =  parser->parse();

  EXPECT_EQ(0, result);
};


}  // namespace brig
}  // namespace hsa
