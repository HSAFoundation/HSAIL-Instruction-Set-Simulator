// Copyright 2012 MulticoreWare Inc.

#include <string>
#include <iostream>
#include "gtest/gtest.h"
#include "lexer.h"
#include "parser.h"
#include "parser_wrapper.h"
#include "error_reporter.h"
namespace hsa {
namespace brig {

extern Context* context;
ErrorReporter* main_reporter = ErrorReporter::get_instance();


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
}

TEST(ParserTest, ArrayDimensionSet) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

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
}

TEST(ParserTest, ArgumentDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

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
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentListBody(context));
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
  EXPECT_EQ(0, Codeblock(context));
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
};

TEST(ParserTest, BranchOperation) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

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

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 2
  input.clear();
  input.assign("version 1:0:$small;\n");
  input.append("function &return_true(arg_f32 %ret_val) () {\n");
  input.append(" ret;\n");
  input.append(" }; \n");

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 4
  input.clear();
  input.assign(" version 1:1:$small; \n");
  input.append("function &branch_ops (arg_u8x4 %x)() { \n");
  input.append(" cbr $c1, @then; \n");
  input.append(" abs_p_s8x4 $s1, $s2; \n");
  input.append(" brn @outof_IF; \n");
  input.append(" @then: add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append(" @outof_IF: ret; \n");
  input.append("}; \n");

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 5 - Call to simple function
  input.clear();
  input.assign("version 1:0:$small; \n");
  input.append("function &callee()() { \n");
  input.append("ret; \n");
  input.append("}; \n");
  input.append("function &caller()() { \n");
  input.append("{ call &callee; } \n");
  input.append(" }; \n");

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));

  // Example 6a - Call to a complex function
  input.clear();
  input.assign("version 1:0:$small;\n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) {\n");
  input.append("ret;\n");
  input.append("};\n");

  input.append(" function &caller()() {\n");
  input.append(" { arg_f32 %input; \n");
  input.append("  arg_f32 %output; \n");
  input.append("call &callee (%output)(%input);}\n");
  input.append(" }; \n");

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));
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
  context->clear_context();
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));
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

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));
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

  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Program(context));
};

TEST(ParserTest, FileDecl) {
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

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

  input.assign("syscall $s2, 0xff, $d1, $s3, $s4;\n");  // src must be s register
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
  context->token_to_scan = yylex();
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
  context->token_to_scan = yylex();
  EXPECT_NE(0, Label(context));
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
};

TEST(ParserTest, Extension) {
  // format:
  // extension := "extension" TOKEN_STRING ) ";"
  // correct cases
  // Create a lexer
  Lexer* lexer = new Lexer();
  // register error reporter with context
  context->set_error_reporter(main_reporter);

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

  input.assign("bitalign_b32 $s5, $s0, $s1, $s2, $s3;\n");  // redundent operand
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

  EXPECT_EQ(0, parser->parse());
};

}  // namespace brig
}  // namespace hsa
