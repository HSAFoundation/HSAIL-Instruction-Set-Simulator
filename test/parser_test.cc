// Copyright 2012 MulticoreWare Inc.

#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "lexer.h"
#include "parser.h"
#include "parser_wrapper.h"
#include "error_reporter.h"
#include "gmock/gmock.h"
#include "mock_error_reporter.h"
namespace hsa {
namespace brig {

extern Context* context;
ErrorReporter* main_reporter = ErrorReporter::get_instance();

using ::testing::AtLeast;
using ::testing::_;

// ------------------  BASIC PARSER TEST -----------------

TEST(ParserTest, OperandTest) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("&a_global_id123 \n");  // global id
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  input.assign("%a_local_id \n");  // local id
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  input.assign("$d7 \n");  // register
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  input.assign("1352 \n");  // Int constant
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  input.assign("_u32(12, 13 ,14) \n");  // decimalListSingle
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  delete lexer;
}

TEST(ParserTest, AddressableOperandTest) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("[%local_id] \n");  // Int constant
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AddressableOperand(context));

  input.assign("[%local_id<100>] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AddressableOperand(context));

  input.assign("[%local_id<$d7>] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AddressableOperand(context));

  input.assign("[%global_id<$q5 + 10>] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AddressableOperand(context));

  input.assign("[%global_id<$d6 - 10 >]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AddressableOperand(context));

  delete lexer;
}

TEST(ParserTest, QueryTest) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // test the Query types;
  std::string input("query_order_u32  $c1 , [&Test<$d7  + 100>]; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Query(context));

  lexer->get_next_token();
  input.assign("query_data_u32  $c1 , [&Test<$d7  + 100>]; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Query(context));

  delete lexer;
}

TEST(ParserTest, ArrayOperandList) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("($d4, &global_id, %local_id) \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandList(context));

  delete lexer;
}

TEST(ParserTest, RoundingMode) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("_upi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_downi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_zeroi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_neari \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_up \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_down \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_zero \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_near \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_ftz_up \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_ftz_down \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_ftz_zero \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_ftz_near \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  input.assign("_ftz \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, RoundingMode(context));

  delete lexer;
}

TEST(ParserTest, Instruction2) {
  // with packing
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("abs_p_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  // with _ftz and packing
  input.assign("abs_ftz_p_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  // with _ftz floatRounding and packing
  input.assign("abs_ftz_up_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  // without roundingMode or packing
  input.assign("abs_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  // with _ftz
  input.assign("abs_ftz_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  // with _ftz floatRounding
  input.assign("abs_ftz_up_s8x4 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  input.assign("unpack2 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  input.assign("unpack2_ftz $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  input.assign("unpack2_ftz_zero $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));


  input.assign("unpack2_neari $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  input.assign("frsqrt_ftz_f32 $s1, $s0;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  input.assign("frsqrt_ftz_f32 $s1, $s0;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  delete lexer;
}

TEST(ParserTest, VersionStatement) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("version 1:0;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));


  input.assign("version 2:0:$large;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  input.clear();
  delete lexer;
}

TEST(ParserTest, AlignStatement) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("align 8\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Alignment(context));

  delete lexer;
}

TEST(ParserTest, DeclPrefix) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);
  std::string input("align 8\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  input.assign("align 8 static\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  input.assign("align 8 extern const\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  input.assign("extern const\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  input.assign("extern const align 1\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  input.assign("const extern\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, DeclPrefix(context));

  delete lexer;
}

TEST(ParserTest, FBar) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input(":fbar(1)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FBar(context));

  delete lexer;
}

TEST(ParserTest, ArrayDimensionSet) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  bool rescan = false;
  unsigned int last_tok = 0;
  std::string input("[]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayDimensionSet(context));

  input.assign("[1]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayDimensionSet(context));

  input.assign("[1][2][][3]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayDimensionSet(context));

  delete lexer;
}

TEST(ParserTest, ArgumentDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  bool rescan = false;
  unsigned int last_tok = 0;
  // test 1
  std::string input("const static arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

  // test 2
  input.assign("align 8 const static arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

  // test 3
  input.assign("align 8 arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

    // test 4
  input.assign("extern arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

    // test 5
  input.assign("const align 8 arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

    // test 6
  input.assign("const static align 8 arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

  // test 7
  input.assign("const align 8 static arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

      // test 8
  input.assign("static const align 8 arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

      // test 9
  input.assign("static align 8 arg_u32 %local_id[2][2] \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

  delete lexer;
}

TEST(ParserTest, ArgumentListBody) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // test 1
  std::string input("const static arg_u32 %local_id[2][2],\n");
  input.append("static arg_f16 %local_id[], align 8 arg_u64 %test \n");
  lexer->set_source_string(input);
  context->clear_context();
  // initialize fake values
  // which should be set in real case when parser parses a function def
  context->current_bdf_offset = 0;
  context->set_arg_output(false);
  // append a fake BDF to directive buffer
  BrigDirectiveFunction fake = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      32,                       // c_code
      32,                       // s_name
      0,                        // inParamCount
      220,                      // d_firstScopedDirective
      1,                        // operationCount
      316,                      // d_nextDirective
      BrigNone,
      0,
      0,                        // outParamCount
      0,
  };
  context->append_directive(&fake);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentListBody(context));

  delete lexer;
}

TEST(ParserTest, FunctionDefinition) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val) ");
  input.append("(arg_u32 %arg_val0):fbar(1)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionDefinition(context));

  delete lexer;
}

TEST(ParserTest, FunctionDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // test 1
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0) :fbar(1);\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionDecl(context));

  delete lexer;
}

TEST(ParserTest, Codeblock) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // test 1
  std::string input("{ \n");
  input.append("abs_p_s8x4 $s1, $s2; \n");
  input.append("abs_s8x4 $s1, $s2; \n");
  input.append("}; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

// initialize fake values
  // which should be set in real case when parser parses a function def
  context->current_bdf_offset = 0;
  context->set_arg_output(false);
  // append a fake BDF to directive buffer
  BrigDirectiveFunction fake = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      32,                       // c_code
      32,                       // s_name
      0,                        // inParamCount
      220,                      // d_firstScopedDirective
      1,                        // operationCount
      316,                      // d_nextDirective
      BrigNone,
      0,
      0,                        // outParamCount
      0,
  };
  context->append_directive(&fake);

  EXPECT_EQ(0, Codeblock(context));

  delete lexer;
}

TEST(ParserTest, Function) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("function &get_global_id(arg_u32 %ret_val) ");
  input.append(" (arg_u32 %arg_val0) :fbar(1) { \n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" abs_s8x4 $s1, $s2; \n");
  input.append("};\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Function(context));

  delete lexer;
}

TEST(ParserTest, SimpleProg) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("version 1:0:$small;\n");
  input.append("function &get_global_id(arg_u32 %ret_val)\n");
  input.append(" (arg_u32 %arg_val0);\n");
  input.append("function &abort() (); \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  delete lexer;
};

TEST(ParserTest, Instruction3) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input(" add_pp_sat_u16x2 $s1, $s0, $s3;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction3(context));

  delete lexer;
};

TEST(ParserTest, OptionalWidth) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("_width(all)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OptionalWidth(context));

  input.assign("_width(32)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OptionalWidth(context));

  delete lexer;
};

TEST(ParserTest, BranchOperation) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // initialize fake values
  // which should be set in real case when parser parses a function def
  context->current_bdf_offset = 0;
  context->set_arg_output(false);
  // append a fake BDF to directive buffer
  BrigDirectiveFunction fake = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      32,                       // c_code
      32,                       // s_name
      0,                        // inParamCount
      220,                      // d_firstScopedDirective
      1,                        // operationCount
      316,                      // d_nextDirective
      BrigNone,
      0,
      0,                        // outParamCount
      0,
  };
  context->append_directive(&fake);


  std::string input("cbr_width(all)_fbar $s1, @then;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("\n cbr_width(all)_fbar $c1, 10 , [@first, @then];\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("cbr_width(all)_fbar $c1, &global;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("cbr_width(all)_fbar $c1, 5, [%local]; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("cbr_width(all)_fbar $c1, 10, @label; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("brn_width(all)_fbar &global; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("brn_width(all)_fbar @goto; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("brn_width(all)_fbar &global, [%local]; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("brn_width(all)_fbar &global, [@goto]; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  input.assign("cbr $s1, @then; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Branch(context));

  delete lexer;
};

TEST(ParserTest, ParseCallTargets) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("[&global, %local]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CallTargets(context));

  delete lexer;
};

TEST(ParserTest, ParseCallArgs) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("()\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CallArgs(context));

  input.assign("(&a,%b,%c)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CallArgs(context));

  input.assign("(1,2,3)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CallArgs(context));

  delete lexer;
};

TEST(ParserTest, Call) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("call &callee (%output)(%input);\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Call(context));

  input.assign("call_width(all) &callee \n");
  input.append("(%output1,&output2)(%input1, $d7) [&id1, &id2];\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Call(context));

  delete lexer;
};

TEST(ParserTest, Initializers) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("= {12, 13,14, -13}\n");  // DecimalInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("= 12, 13,14 \n");  // DecimalInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("={ 1.2f, 1.3f,-1.4f }\n");  // SingleInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("= 1.2f, 1.3f,-1.4f \n");  // SingleInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("={ 1.2L, 1.3L,-1.4L }\n");  // FloatInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("= 1.2L, 1.3L,-1.4L \n");  // FloatInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  input.assign("= {@a, @b, @c} \n");  // LabelInitializer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Initializer(context));

  delete lexer;
};

TEST(ParserTest, InitializableDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // DecimalInitializer
  std::string input("readonly_s32 &x[4]= {12, 13,14, -13};\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  input.assign("global_u32 &x[3] = 12, 13,14 ; \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  // SingleInitializer
  input.assign("readonly_f32 %f[3] = { 1.2f, 1.3f,-1.4f };\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  input.assign("global_f32 &c[3] = 1.2f, 1.3f,-1.4f ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  // FloatInitializer
  input.assign("readonly_f64 %d[3] ={ 1.2L, 1.3L,-1.4L; }\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  input.assign("global_f64 %g[3] = 1.2L, 1.3L,-1.4L ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, InitializableDecl(context));

  delete lexer;
};

TEST(ParserTest, ProgWithFunctionDefinition) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // Example 3
  std::string input("version 1:0:$small;\n");
  input.append("function &packed_ops (arg_u8x4 %x)() {\n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 2
  input.assign("version 1:0:$small;\n");
  input.append("function &return_true(arg_f32 %ret_val) () {\n");
  input.append(" ret;\n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 4
  input.assign(" version 1:1:$small; \n");
  input.append("function &branch_ops (arg_u8x4 %x)() { \n");
  input.append(" cbr $c1, @then; \n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" brn @outof_IF; \n");
  input.append(" @then: add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append(" @outof_IF: ret; \n");
  input.append("}; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 5 - Call to simple function
  input.assign("version 1:0:$small; \n");
  input.append("function &callee()() { \n");
  input.append("ret; \n");
  input.append("}; \n");
  input.append("function &caller()() { \n");
  input.append("{ call &callee; } \n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 6a - Call to a complex function
  input.assign("version 1:0:$small;\n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {\n");
  input.append("ret;\n");
  input.append("};\n");

  input.append(" function &caller()() {\n");
  input.append(" { arg_f32 %input; \n");
  input.append("  arg_f32 %output; \n");
  input.append("call &callee (%output)(%input);}\n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  delete lexer;
};

TEST(ParserTest, ProgWithGlobalDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("version 1:0:$small;\n");
  input.append("readonly_f32 %f[3] = { 1.2f, 1.3f,-1.4f };\n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {\n");
  input.append("ret;\n");
  input.append("};\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  delete lexer;
};

TEST(ParserTest, ProgWithUninitializableDecl ) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  // Example 1 - PRM 20.8.2
  std::string input("version 1:0:$large; \n");
  input.append("global_f32 &x = 2; \n");
  input.append("function &test()() { \n");
  input.append("{ private_u32 %z; } \n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  delete lexer;
};

TEST(ParserTest, UninitializableDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("private_f32 %f[3];\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, UninitializableDecl(context));

  delete lexer;
};

TEST(ParserTest, ArgUninitializableDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("arg_f32 %f[3];\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgUninitializableDecl(context));

  delete lexer;
};

TEST(ParserTest, ProgWithArgUninitializableDecl ) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("version 1:0:$large;\n");
  input.append("global_f32 &x = 2;\n");
  input.append("function &test()() {\n");
  input.append("{arg_u32 %z;}\n");
  input.append(" }; \n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  delete lexer;
};

TEST(ParserTest, FileDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("file 1 \"this is a file\";\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FileDecl(context));

  // wrong case
  input.assign("file 2 ;\n");  // lack of file string
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file \"this is a file\";\n");  // lack of file string
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file 2 \"this is a file\"\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file \"this is a file\" 2;\n");  // reverse order
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file 1 2;\n");  // two integer number
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file \"file1\" \"file2\";\n");  // two file string
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file 1 \"file1\" \"file2\";\n");  // redundant file string
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file 1 2 \"file\";\n");  // redundant integer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file 1.2 \"file\";\n");  // not integer
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file;\n");  // lack of number , file string
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  input.assign("file $s1 \"file\";\n");  // register not allowed
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  delete lexer;
};

TEST(ParserTest, VectorToken) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("_v2\n");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, VectorToken(context));

  // input.clear() ;
  input.assign("_v4\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, VectorToken(context));

  delete lexer;
};

TEST(ParserTest, SysCall) {
  // syscall dest, n, src0, src1, src2;
  // dest: must be a 32-bit register
  // n: An integer literal
  // src: must be a s reg, imm, WAVESIZE
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("syscall $s1, 3, $s2, $s3, $s4;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  input.assign("syscall $s1, 0xff, 1, 2, 3;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  input.assign("syscall $s1, 0xff, 1, $s3, 2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  input.assign("syscall $s1, 0xff, 1, $s3, WAVESIZE;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  input.assign("syscall $s11, 0xff, WAVESIZE, $s3, WAVESIZE;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  // wrong case
  input.assign("syscall $d2, 0xff, $s1, $s3, $s4;\n");  // d register is 64-bit
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $c2, 0xff, $s1, $s3, $s4;\n");  // c register is 1-bit
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $q2, 0xff, $s1, $s3, $s4;\n");  // q register is 128-bit
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  // src must be s register
  input.assign("syscall $s2, 0xff, $d1, $s3, $s4;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s2, $s4, $s1, $s3, $s4;\n");
  // n must be integer literal
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));


  input.assign("syscall $s3, 1.1, $s1, $s3, $s4;\n");
  // n must be integer literal
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s3, 3, $s1, $s3, $s4\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s3 3, $s1, $s3, $s4;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s3, 3 $s1, $s3, $s4;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s3, 3, $s1 $s3, $s4;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  input.assign("syscall $s3, 3, $s1, $s3 $s4;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  delete lexer;
};

TEST(ParserTest, Label) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("@_test_label_1:\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Label(context));

  input.assign("@_test_label_2: \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Label(context));

  // wrong case

  input.assign("@_test_label_3 @wrong  : \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Label(context));

  input.assign("@_test_label_4 \n");  // lack of colon ':'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Label(context));

  input.assign("$_test_label_5 :\n");
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Label(context));

  delete lexer;
};

TEST(ParserTest, LabelTargets) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("@tab: labeltargets @a1, @a2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, LabelTargets(context));

  input.assign("@targets: labeltargets @label;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, LabelTargets(context));

  input.assign("@targets: labeltargets @label1, @label2, @label3, @label4,\n");
  input.append("@label5, @label6, @label7, @label8, @label9,\n");
  input.append("@label10, @label11;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, LabelTargets(context));

  // wrong case
  // redundant ','
  input.assign("@targets: labeltargets @label1, @label2, @label3, ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, LabelTargets(context));

  // redundant ','
  input.assign("@targets: ,labeltargets @label1, @label2, @label3;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, LabelTargets(context));

  input.assign("@targets: labeltargets;\n");  // number of label is zero
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, LabelTargets(context));

  input.assign("@targets: labeltargets @label\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, LabelTargets(context));

  delete lexer;
};

TEST(ParserTest, Extension) {
  // format:
  // extension := "extension" TOKEN_STRING ) ";"
  // correct cases
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("extension \"abc\" ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Extension(context));

  input.assign("extension \"\t\nabcd\" ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Extension(context));

// wrong cases
  input.assign("extension \"\asdfjl\"\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Extension(context));

  delete lexer;
};

TEST(ParserTest, SignatureType) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("arg_u32\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureType(context));

  input.assign("arg_u32 %a\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureType(context));

  input.assign("arg_ROImg\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureType(context));

  input.assign("arg_RWImg\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureType(context));

  input.assign("arg_Samp\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureType(context));

  input.clear();
  delete lexer;
};


TEST(ParserTest, FunctionSignature) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("signature &test()();\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  input.assign("signature &test()(arg_u32) ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  input.assign("signature &test()(arg_u32,arg_u32) ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  input.assign("signature &test()(arg_u32) :fbar(2) ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  input.assign("signature &test(arg_u32)(arg_u32,arg_u32) ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  input.assign("signature &test(arg_u32)(arg_u32) :fbar(2) ;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FunctionSignature(context));

  delete lexer;
};


TEST(ParserTest, SignatureArgumentList) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("arg_u32,arg_ROImg\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureArgumentList(context));

  input.assign("arg_u32,arg_RWImg\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureArgumentList(context));

  input.assign("arg_u32,arg_Samp\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SignatureArgumentList(context));

  delete lexer;
};

TEST(ParserTest, Instruction4) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("mad_ftz_u64 $d1, $d2, $d3, $d4;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("extract_b32 $s1, $s1, 2, 3;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("insert_s32 $s1, $s1, 2, 3;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("shuffle_u8x4 $s10, $s12, $s12, 0x55;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("cmov_u8x4 $s1, $s0, $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("fma_ftz_up_f32 $s3, 1.0f, $s1, 23f;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("bitalign_b32 $s5, $s0, $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("bytealign_b32 $s5, $s0, $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("lerp_b32 $s5, $s0, $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("sad_b32 $s5, $s0, $s1, $s6;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("sad2_b32 $s5, $s0, $s1, $s6;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("sad4_b32 $s5, $s0, $s1, $s6;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("sad4hi_b32 $s5, $s0, $s1, $s6;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  input.assign("bitselect_u32 $s5, $s0, $s1, $s6;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  // wrong case
  input.assign("sad2_b32 ,$s5, $s0, $s1, $s6;\n");  // redundant ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("sad4_b32 $s5, $s0 $s1, $s6;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("sad4hi_b32 $s5, $s0, $s1 $s6;\n");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("bitselect $s5, $s0, $s1, $s6\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("cmov_u8x4;\n");  // no one operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("fma_f32 $s3;\n");  // only one operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("bitalign_b32 $s5, $s0, $s1, $s2, $s3;\n");  // redundant operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("bytealign_b32 $s5, $s0, $s1;\n");  // lack of one operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("lerp_b32 $s5, , $s1, $s2;\n");  // lack of one operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  input.assign("sad $s5, $s0, $s1, $s6;\n");  // lack of data type
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));

  delete lexer;
};
// ------------------  Test for ldc rule -------------------
// format:
// ldc ::= "ldc" dataTypeId operand "," ( TOKEN_LABEL ";" | identifier ";" )
// correct cases
TEST(ParserTest, Ldc) {
  Lexer* lexer = new Lexer();

  std::string input("ldc_b32 $s1, &bar;");  // identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ldc(context));

  input.assign("ldc_b64 $s2, @lab;");  // label
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ldc(context));

  input.assign("ldc_b64 $s2, %label;");  // identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ldc(context));

  // wrong cases
  input.assign("ldc_b64 $s1, &some_function");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ldc(context));

  input.assign("ldc $s1, &function;");  // lack of dataTypeId
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ldc(context));

  input.assign("ldc_b32 , $s1, &function;");  // redundant ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ldc(context));

  input.assign("ldc_b64 $s1, e123;");  // unrecognized identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ldc(context));

  delete lexer;
};

// -----------------  Test for Instruction5 rule -----------------
// format:
// Instruction5 ::= "f2u4" dataTypeId operand "," operand
//                  "," operand "," operand "," operand ";"
// correct cases
TEST(ParserTest, Instruction5) {
  Lexer* lexer = new Lexer();

  std::string input("f2u4_u32 $s1, $s2, $s3, $s9, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  input.assign("f2u4_u64 $d4, $d6, $d3, $d1, $d5;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  input.assign("f2u4_u64 $s4, $s6, $s3, 364, 113;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  input.assign("f2u4_u32 $s1, $s2, $s3, 0xD41, 0xF4;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  input.assign("f2u4_u64 $d1, $d2, $d3, 1.0f, 2.0f;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  // wrong cases
  input.assign("f2u4_u64 $d4, $d6, $d3, $d1, $d5");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction5(context));

  input.assign("f2u4_u32 $s0, $s6, $s3, $s4 $s2;");  // lack of ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction5(context));

  input.assign("f2u4 $d1, $d2, $d3, 4.0f, 6.0f;");  // lack of datetypeId
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction5(context));

  input.assign("f2u4_u64 $s1, $s2, $s3, 0xD41;");  // lack of operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction5(context));

  input.assign("f2u4_u64 , $s1, $s2, $s3, $s4, 0xD41;");  // redundant ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction5(context));

  delete lexer;
};

// -----------------  Test for cvtModifier1 rule -----------------
// format:
// cvtModifier1 ::= floatRounding
//                 | "_ftz"
//                 | "_ftz" floatRounding
//                 | intRounding
// correct cases
TEST(ParserTest, CvtModifier1) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("_upi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_downi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_zeroi \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_neari \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_up \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_down \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_zero \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_near \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_ftz_up \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_ftz_down \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_ftz_zero \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_ftz_near \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  input.assign("_ftz \n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, CvtModifier1(context));

  delete lexer;
};

// -----------------  Test for mov rule -------------------
// format:
// mov ::= "mov" dataTypeId arrayOperand ","
//         arrayOperand ";"
TEST(ParserTest, Mov) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
  // correct cases
  std::string input("mov_b64 $d1, $d4;\n");  // D register
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  input.assign("mov_b128 $s5, $s6;\n");  // S register
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  input.assign("mov_b128 $c1, $c2;\n");  // C register
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  input.assign("mov_b128 $q3, $q5;\n");  // Q register
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  input.assign("mov_b32 $s1, 0;\n");  // Immediate
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  input.assign("mov_b32 $s4, (&global_id, %local_id);\n");  // Arrayoperandlist
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));
  // wrong cases
  input.assign("mov $q1, $q2;\n");  // lack of modifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  input.assign("mov_b32 $c7;\n");  // lack of operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  input.assign("mov_b32 $s2, $s1\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  input.assign("mov_b128 $s6, &global_id, %local_id);\n");  // lack of '('
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  input.assign("mov_b32 $s2, $s3, $s1;\n");  // redundant operand
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  input.assign("mov_b32 , $q3, $q1;\n");  // redundant ','
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));

  delete lexer;
};

TEST(ParserTest, KernelArgumentList) {
  Lexer* lexer = new Lexer();
  bool rescan_last_token = false;
  unsigned int last_token = 0;

  // test 1
  std::string input("const static kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  // test 2
  input.assign("align 8 const static kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  // test 3
  input.assign("align 8 kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

    // test 4
  input.assign("extern kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

    // test 5
  input.assign("const align 8 kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

    // test 6
  input.assign("const static align 8 kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  // test 7
  input.assign("const align 8 static kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  // test 8
  input.assign("static const align 8 kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  // test 9
  input.assign("static align 8 kernarg_u32 %local_id[2][2] ");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, KernelArgumentDecl(context));

  delete lexer;
};

TEST(ParserTest, KernelArgumentListBody) {
  Lexer *lexer = new Lexer();


  std::string input("kernarg_f32 %x");

  lexer->set_source_string(input);
  context->clear_context();
  // initialize fake values
  // which should be set in real case when parser parses a function def
  context->current_bdf_offset = 0;
  context->set_arg_output(false);
  // append a fake BDF to directive buffer
  BrigDirectiveFunction fake = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      32,                       // c_code
      32,                       // s_name
      0,                        // inParamCount
      220,                      // d_firstScopedDirective
      1,                        // operationCount
      316,                      // d_nextDirective
      BrigNone,
      0,
      0,                        // outParamCount
      0,
  };
  context->append_directive(&fake);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0 , KernelArgumentListBody(context));



  input.assign("kernarg_u32 %y , kernarg_f32 %x");
  lexer->set_source_string(input);

  context->clear_context();
  // initialize fake values
  // which should be set in real case when parser parses a function def
  context->current_bdf_offset = 0;
  context->set_arg_output(false);
  context->append_directive(&fake);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0 , KernelArgumentListBody(context));

  delete lexer;
};

TEST(ParserTest, Kernel) {
  Lexer *lexer = new Lexer();

  std::string input("kernel &demo (kernarg_f32 %x)");
  input.append("{private_u32 %z ;");
  input.append("ret ;} ;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0 , Kernel(context));


  delete lexer;
};

TEST(ParserTest, OperandList) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("$s1, $c4, $d4, $q2 \n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, OperandList(context));

  input.assign("&g, %l, $s1 \n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OperandList(context));

  input.assign("&g\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OperandList(context));

  input.assign("&g1, &g2, %l, $s1,$s1, $c4, $d4,$q2,-77, $c4, $d4,$q2,\n");
  input.append("$s1, $c4, $d4,$q2, 1.2l, 2.33f , $c4, $d4, 5, $c4, $d4,$q2\n");
  // the number of operands is 24
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OperandList(context));

  input.assign(", $s5, $s0 ,$s6\n");  // redundant ','
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OperandList(context));

  input.assign("$s5, $s0 ,$s6,\n");  // redundant ','
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OperandList(context));

  input.assign("\n");  // NULL
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OperandList(context));

  delete lexer;
};

TEST(ParserTest, Cmp) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("cmp_eq_b1_b1 $c1, $c2, 0;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_eq_b32_b1 $s1, $c2, 0;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_eq_f32_b1 $s1, $c2, 0.0f;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_ne_b1_b1 $c1, $c2, 0;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_sltu_b1_f32 $c1, $s2, 0;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_gt_f32_b32 $s1, $s2, 0.0f;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("packedcmp_lt_f32x2 $d1, $d2, $d3;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_sltu_b1_f64 $c1, $d1, $d2;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_lt_f32_f32 $s1, $s2, 0.0f;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  input.assign("cmp_equ_b1_f64 $c1, $d1, $d2;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  // wrong case
  input.assign("cmp_equ_b1 $c1, $d1, $d2;");  // lack of data type
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("cmp_b1_f64 $c1, $d1, $d2;");  // lack of comparsionId
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("packedcmp_lt $d1, $d2, $d3;");  // lack of data type
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("packedcmp_f32x2 $d1, $d2, $d3;");  // lack of comparsionId
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("cmp_eq_f32_b1 $s1, $c2;");  // lack of operands
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("cmp_eq_f32_b1 $s1, $c2, 0.0f");  // lack of ';'
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  input.assign("cmp_eq_f32_b1 $s1, $c2 0.0f;");  // lack of ','
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));

  delete lexer;
};


TEST(ParserTest, GlobalPrivateDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("private_u32 &tmp[2][2];\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalPrivateDecl(context));

  input.assign("private_s32 &tmp;\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalPrivateDecl(context));

  input.assign("private_b32 &tmp[2];\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalPrivateDecl(context));

  // wrong case
  input.assign("private_s32 %tmp;\n");  // %tmp is not global identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  input.assign("private_u32 &tmp\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  input.assign("private_u32;\n");  // lack of identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  delete lexer;
};

TEST(ParserTest, OffsetAddressableOperand) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("[$s1 + 0xf7]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OffsetAddressableOperand(context));

  input.assign("[$s1]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OffsetAddressableOperand(context));

  input.assign("[$s2 - 0xf7]\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OffsetAddressableOperand(context));

  input.assign("[0xf7]\n");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OffsetAddressableOperand(context));

  // wrong case
  input.assign("[0xf7\n");  // lack of ']'
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OffsetAddressableOperand(context));

  input.assign("$s1]\n");  // lack of '['
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OffsetAddressableOperand(context));

  input.assign("[]\n");  // the content in square brackets is empty
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OffsetAddressableOperand(context));

  input.assign("[$s1 * 0xf7]\n");  // '*' is the illegal operation
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OffsetAddressableOperand(context));

  input.assign("[0xf7 + 0xf7]\n");  // the operation is illegal
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, OffsetAddressableOperand(context));

  delete lexer;
};

TEST(ParserTest, MemoryOperand) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);
  context->clear_context();


  std::string input("[%local_id]");  // Int constant
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));

  input.assign("[$s2 - 0xf7]");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));

  input.assign("[%local_id<$d7>][$s1 + 0xf7]");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));

  delete lexer;
};

TEST(ParserTest, IntegerLiteral) {
  // Case 1 Start reg '+' int
  std::string input("$s1+5");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());
  EXPECT_EQ('+', lexer->get_next_token());

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(5, context->token_value.int_val);
  // Case 1 End

  // Case 2 Start reg '+' int
  input.assign("$s11+-11");
  lexer->set_source_string(input);

  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());
  EXPECT_EQ('+', lexer->get_next_token());

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(-11, context->token_value.int_val);
  // Case 2 End

  // Case 3 Start  double
  input.assign("-11.7777l");
  lexer->set_source_string(input);

  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(-11.7777, context->token_value.double_val);
  // Case 3 End

  // Case 4 Start int '-' int
  input.assign("11-7");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(11, context->token_value.int_val);

  EXPECT_EQ('-', lexer->get_next_token());
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(7, context->token_value.int_val);
  // Case 4 End

  // Case 5 Start int ',' int
  input.assign("+11,-11");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(11, context->token_value.int_val);
  EXPECT_EQ(',', lexer->get_next_token());
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(-11, context->token_value.int_val);
  // Case 5 End

  // Case 6 Start int
  input.assign("+7");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(7, context->token_value.int_val);
  // Case 6 End

  // Case 7 Start '(' int ')'
  input.assign("(+7)");
  lexer->set_source_string(input);


  EXPECT_EQ('(', lexer->get_next_token());
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(7, context->token_value.int_val);
  EXPECT_EQ(')', lexer->get_next_token());
  // Case 7 End

  // Case 9 Start int + int
  input.assign("5+-3");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(5, context->token_value.int_val);
  EXPECT_EQ('+', lexer->get_next_token());
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(-3, context->token_value.int_val);
  // Case 9 End

  // Case 10 Start int '+' int
  input.assign("-5++3");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(-5, context->token_value.int_val);
  EXPECT_EQ('+', lexer->get_next_token());
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, IntegerLiteral(context));
  EXPECT_EQ(3, context->token_value.int_val);
  // Case 10 End


  // Case 11
  input.assign("+a");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_NE(0, IntegerLiteral(context));

  delete lexer;
}

TEST(ParserTest, GlobalGroupDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("group_u32 &tmp[2][2];");
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_EQ(0, GlobalGroupDecl(context));

  input.assign("group_s32 &tmp;");
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_EQ(0, GlobalGroupDecl(context));

  input.assign("group_b32 &tmp[2];");
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_EQ(0, GlobalGroupDecl(context));

  // wrong case
  input.assign("group_s32 %tmp;");  // %tmp is not global identifier
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_NE(0, GlobalGroupDecl(context));

  input.assign("group_u32 &tmp");  // lack of ';'
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_NE(0, GlobalGroupDecl(context));

  input.assign("group_u32;");  // lack of identifier
  lexer->set_source_string(input);

  context->token_to_scan = yylex();
  EXPECT_NE(0, GlobalGroupDecl(context));

  delete lexer;
};

TEST(ParserTest, Mul) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);
  // test mul_datatype case
  std::string input("mul_u32 $s1, $s2, 0x23;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul_pp_u16x4 $d1, $d0, $d3;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul_hi_u32 $s1, $s3, $s9;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul24_u32 $s1, $s2, 12;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul24_hi_s32 $s1, $s2, -12;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mad24_u64 $d1, $d2, 12, 2;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mad24_hi_s32 $s1, $s2, -12, 23;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul_ftz_s32 $s1, $s3, $s9;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  input.assign("mul_ftz_pp_s32 $s1, $s3, $s9;");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Mul(context));

  delete lexer;
};

TEST(ParserTest, Ld) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("ld_f32 $s1, [&x];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(64)_f16 $d1, [&x];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_acq_f32 $s1, [&x];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_acq_equiv(2)_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_acq_equiv(2)_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_arg_acq_equiv(2)_f32 $s1, [&y];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_arg_dep_equiv(2)_f32 $s1, [&y];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_private_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_spill_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_v4_f32 ($s1,$s1,$s6,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_v2_equiv(9)_f32 ($s1,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_group_equiv(0)_u32 $s0, [$s2];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_equiv(1)_u64 $d3, [$s4+32];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_v2_equiv(1)_u64 ($d1,$d2), [$s0+32];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(8)_v4_f32 ($s1,$s1,$s6,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_equiv(1)_u64 $d6, [128];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(4)_v2_equiv(9)_f32 ($s1,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(64)_u32 $s0, [$s2];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(1024)_equiv(1)_u64 $d6, [128];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  input.assign("ld_width(all)_equiv(1)_u64 $d6, [128];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));

  delete lexer;
};

TEST(ParserTest, St) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("st_f32 $s1, [&x];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_rel_f32 $s1, [&x];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_rel_equiv(2)_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_private_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_f32 $s1, [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_v4_f32 ($s1,$s1,$s6,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_v2_equiv(9)_f32 ($s1,$s2), [$s3+4];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_group_equiv(0)_u32 $s0, [$s2];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_equiv(1)_u64 $d3, [$s4+32];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_v2_equiv(1)_u64 ($d1,$d2), [$s0+32];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  input.assign("st_equiv(1)_u64 $d6, [128];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St(context));

  delete lexer;
};

TEST(ParserTest, Lda) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("lda_u64 $d2, [&z];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Lda(context));

  input.assign("lda_u64 $d1, [$d0+10];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Lda(context));

  input.assign("lda_group_b32 $s1, [%g];");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Lda(context));

  delete lexer;
};

TEST(ParserTest, ImageRet) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("atomic_image_and_ar_3d_b32 $s1, [&namedRWImg2],");
  input.append("($s0,$s3,$s1,$s10), $s1;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_and_2d_b32 $s2, [&namedRWImg1], ($s0,$s3), $s2;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_and_1d_b32 $s3, [&namedRWImg2], $s1, $s10;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_and_3d_u32 $s4, [&namedRWImg2],");
  input.append("($s0,$s3,$s1,$s10), $s2;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_and_ar_2d_s32 $s1, [&namedRWImg1],");
  input.append("($s0,$s3), $s2;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_and_1d_b32 $s2, [&namedRWImg2], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_or_1d_b32 $s3, [&namedRWImg2], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_xor_1d_b32 $s0, [&namedRWImg2], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  input.assign("atomic_image_cas_1d_b32 $s10, [&namedRWImg2], $s1, $s3, $s4;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  delete lexer;
};

TEST(ParserTest, ImageNoRet) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("atomicNoRet_image_and_ar_3d_b32 [&namedRWImg],");
  input.append("($s1,$s2,$s3,$s10), $s12;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_1d_b32 [&namedRWImg], $s10,$s1;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_3d_u32 [&namedRWImg],");
  input.append("($s0,$s3,$s1,$s10), $s2;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_ar_2d_s32 [&namedRWImg],");
  input.append("($s0,$s3), $s2;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_1d_b32 [&namedRWImg], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_or_1d_b32 [&namedRWImg], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_xor_1d_b32 [&namedRWImg], $s1, $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_1da_b32 [&namedRWImg], ($s1,$s2), $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_and_1d_b32 [&namedRWImg], ($s1), $s3;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  input.assign("atomicNoRet_image_cas_1d_b32 [&namedRWImg], $s1, $s3, $s4;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  delete lexer;
};

// -----------------  Test for cvt rule -------------------
// format:
// cvt ::= "cvt" optcvtModifier dataTypeId
//         dataTypeId operand "," operand ";"
TEST(ParserTest, Cvt) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);
// correct cases
  std::string input("cvt_upi_u32_f32 $s1, $s2;\n");  // intRounding modifier
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

  input.assign("cvt_near_f32_f32 $d1, $d2;\n");  // floatRounding modifier
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

  input.assign("cvt_ftz_u32_f32 $s2, $d1;\n");  // ftz modifier
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

  // ftz floatingRounding modifier
  input.assign("cvt_ftz_up_u32_u32 $s1, $d1;\n");
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

  input.assign("cvt_u32_f64 $d2, $d1;\n");  // without modifier
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cvt(context));

// wrong cases
  input.assign("cvt_f64 $s1, $d1;\n");  // lack of one datatypeId
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cvt(context));

  input.assign("cvt_f32_f64 $d1;\n");  // lack of operand
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cvt(context));

  input.assign("cvt_up_f32_f64 $s1, $d1\n");  // lack of ';'
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cvt(context));

  input.assign("cvt_f32_f64 , $s1, $d1;\n");  // redundant ','
  lexer->set_source_string(input);
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cvt(context));

  input.clear();
  delete lexer;
};


TEST(ParserTest, ImageLoad) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("ld_image_v4_3d_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3],");
  input.append("($s4,$s5,$s6,$s7);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld_image(context));

  input.assign("ld_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3],");
  input.append("($s4,$s5);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld_image(context));

  input.assign("ld_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld_image(context));

  input.assign("ld_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3],");
  input.append("($s4,$s1,$s2,$s3);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld_image(context));

  delete lexer;
};

TEST(ParserTest, ImageStore) {
  // Create a lexer
  Lexer* lexer = new Lexer();

  // register error reporter with context
  context->set_error_reporter(main_reporter);

  std::string input("st_image_v4_3d_f32_u32 ($s1,$s2,$s3,$s4),");
  input.append("[%RWImg3], ($s4,$s5,$s6,$s7);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St_image(context));

  input.assign("st_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4),");
  input.append("[%RWImg3], ($s4,$s5,$s6,$s7);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St_image(context));

  input.assign("st_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4),");
  input.append("[%RWImg3], ($s4,$s5);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St_image(context));

  input.assign("st_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4);");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, St_image(context));

  delete lexer;
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

  if (0) {
    // Print out string buffer content:
    unsigned int index = 0;
    std::string temp;
    std::cout << "Buffer content: " << std::endl;
    while (index < context->get_string_offset()) {
      temp = context->get_string(index);
      std::cout << "Index " << index << ": " << temp << std::endl;
      index+=temp.length()+1;
    }
  }

  delete parser;
};

TEST(ParserWrapperTest, ParseSimpleProgram) {
  // Example 3
  std::string input("\n version 1:0:$small; \n");
  input.append("function &packed_ops (arg_u8x4 %x)() { \n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append(" }; \n");

  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  EXPECT_EQ(0, parser->parse());

  delete parser;
};

TEST(ParserWrapperTest, ParseSequenceOfPrograms) {
  // Example 3
  std::string input("version 1:0:$small;\n");
  input.append("function &packed_ops (arg_u8x4 %x)() {\n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append(" }; \n");

  // Example 4
  input.append("version 1:1:$small;\n");
  input.append("function &branch_ops (arg_u8x4 %x)() {\n");
  input.append("cbr $c1, @then;\n");
  input.append("abs_p_s8x4 $s1, $s2;\n");
  input.append(" brn @outof_IF;\n");
  input.append("@then: add_pp_sat_u16x2 $s1, $s0, $s3;\n");
  input.append(" @outof_IF: ret;\n");
  input.append(" }; \n");

  Parser* parser = new Parser(context);
  parser->set_source_string(input);

  context->clear_context();

  // test size of various buffer
  std::cout << "Directive buffer size before parsing = ";
  std::cout << context->get_directive_offset() << std::endl;
  std::cout << "Operand buffer size before parsing = ";
  std::cout << context->get_operand_offset() << std::endl;
  std::cout << "Code buffer size before parsing = ";
  std::cout << context->get_code_offset() << std::endl;
  std::cout << "String buffer size before parsing = ";
  std::cout << context->get_string_offset() << std::endl;

  EXPECT_EQ(0, parser->parse());

  // test size of various buffer
  std::cout << "Directive buffer size after parsing = ";
  std::cout << context->get_directive_offset() << std::endl;
  std::cout << "Operand buffer size after parsing = ";
  std::cout << context->get_operand_offset() << std::endl;
  std::cout << "Code buffer size after parsing = ";
  std::cout << context->get_code_offset() << std::endl;
  std::cout << "String buffer size after parsing = ";
  std::cout << context->get_string_offset() << std::endl;


  delete parser;
};

// Error reporting tests

TEST(ErrorReporting, UseMockErrorReporter) {
  MockErrorReporter mer;

  // forward call to FakeErrorReporter
  // to record error history
  mer.DelegateToFake();
  ErrorReporterInterface* old_rpt = context->get_error_reporter();
  context->set_error_reporter(&mer);
  context->clear_context();

  std::string input("version 1:0;");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_CALL(mer, report_error(ErrorReporterInterface::OK, _, _));
  EXPECT_EQ(0, Version(context));

  EXPECT_CALL(mer, show_all_error());
  mer.show_all_error();

  // return the true reporter to context
  context->set_error_reporter(old_rpt);

  delete lexer;
}

TEST(ErrorReporting, CheckErrorHistory) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  ErrorReporterInterface* old_rpt = context->get_error_reporter();
  MockErrorReporter mer;
  // forward call to FakeErrorReporter
  // to record error history
  mer.DelegateToFake();

  EXPECT_CALL(mer, report_error(_, _, _))
      .Times(AtLeast(1));

  // register error reporter with context
  context->set_error_reporter(&mer);
  context->clear_context();

  // wrong case
  std::string input("private_s32 %tmp;\n");  // %tmp is not global identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  input.assign("private_u32 &tmp\n");  // lack of ';'
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  input.assign("private_u32;\n");  // lack of identifier
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, GlobalPrivateDecl(context));

  EXPECT_CALL(mer, show_all_error());

  // test reported error
  ErrorReporterInterface::error_t error_code;
  unsigned int number_of_errors = mer.get_number_of_errors();

  EXPECT_EQ(3, number_of_errors);

  error_code = mer.get_error_at(0);
  EXPECT_EQ(ErrorReporterInterface::MISSING_IDENTIFIER, error_code);

  error_code = mer.get_error_at(1);
  EXPECT_EQ(ErrorReporterInterface::MISSING_SEMICOLON, error_code);

  error_code = mer.get_error_at(2);
  EXPECT_EQ(ErrorReporterInterface::MISSING_IDENTIFIER, error_code);

  mer.show_all_error();


  context->set_error_reporter(old_rpt);
  delete lexer;
};

}  // namespace brig
}  // namespace hsa
