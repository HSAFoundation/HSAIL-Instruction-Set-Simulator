// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;
TEST(CodegenTest, ExampleWithKernel) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$large; \n");
  input.append("kernel &demo(kernarg_f32 %x) { \n");
  input.append("private_u32 %z; \n");
  input.append("ret; \n");
  input.append("};\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Kernel(context));

  // test the size of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(144U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(32U, csize);

  BrigDirectiveKernel ref = {
    40,                       // size
    BrigEDirectiveKernel,   // kind
    0,                       // c_code
    0,                       // s_name
    1,                        // inParamCount
    140,                      // d_firstScopedDirective
    1,                        // operationCount
    140,                      // d_nextDirective
    BrigNone,                  // attribute
    0,                         // fbar
    0,                        // outParamCount
    60,                        // d_firstInParam
  };

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveKernel get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  BrigDirectiveSymbol kernarg = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    3,                        // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    6,                        // s_name
    Brigf32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  BrigDirectiveSymbol get_sym;
  context->get_directive(64, &get_sym);
  EXPECT_EQ(kernarg.size, get_sym.size);
  EXPECT_EQ(kernarg.kind, get_sym.kind);
  EXPECT_EQ(kernarg.s.storageClass, get_sym.s.storageClass);
  EXPECT_EQ(kernarg.s.s_name, get_sym.s.s_name);

  BrigDirectiveSymbol private_var = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    2,                        // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    9,                        // s_name
    Brigu32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  context->get_directive(104, &get_sym);
  EXPECT_EQ(private_var.size, get_sym.size);
  EXPECT_EQ(private_var.kind, get_sym.kind);
  EXPECT_EQ(private_var.s.storageClass, get_sym.s.storageClass);
  EXPECT_EQ(private_var.s.s_name, get_sym.s.s_name);

  delete lexer;
}

TEST(CodegenTest, CallwMultiArgs) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small; \n");
  input.append("function &callee(arg_f32 %output) ");
  input.append("(arg_f32 %input1, arg_f32 %input2) { \n");
  input.append("ret; \n");
  input.append("};\n");
  input.append("function &caller()(){ \n");
  input.append(" { arg_f32 %an_input; \n");
  input.append("   arg_f32 %an_output; \n");
  input.append("   call &callee(%an_output)(%an_input, %an_input);\n");
  input.append(" } \n");
  input.append("}; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));
  EXPECT_EQ(0, Function(context));

  // test the size of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(320U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(56U+4U, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(61U, ssize);

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,                       // c_code
    32,                       // s_name
    0,                        // inParamCount
    224,                      // d_firstScopedDirective
    1,                        // operationCount
    320,                      // d_nextDirective
    BrigNone,
    0,
    0,                        // outParamCount
    0,
  };

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context->get_directive(224, &arg_scope);
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32U, arg_scope.c_code);

  context->get_directive(312, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0U, cbr_op.o_operands[0]);
  EXPECT_EQ(32U, cbr_op.o_operands[1]);
  EXPECT_EQ(24U, cbr_op.o_operands[2]);
  EXPECT_EQ(44U, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(24, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20U, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(192U+40U, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(232U+40U, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(32, &arg_l);
  EXPECT_EQ(12U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16U, arg_l.o_args[0]);

  context->get_operand(44, &arg_l);
  EXPECT_EQ(16U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8U, arg_l.o_args[0]);

  BrigoOffset32_t arg_test = 0;
  context->get_operand(56, &arg_test);
  EXPECT_EQ(8U, arg_test);

  delete lexer;
}

TEST(CodegenTest, Example6_CallwArgs) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small; \n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input) { \n");
  input.append(" ret; \n");
  input.append("}; \n");

  input.append("function &caller()(){ \n");
  input.append(" { arg_f32 %an_input; \n");
  input.append("   arg_f32 %an_output; \n");
  input.append("  call &callee(%an_output)(%an_input); \n");
  input.append(" } \n");
  input.append("}; \n");

  // test the rule
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));
  EXPECT_EQ(0, Function(context));

  // test the sizes of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(276U+4U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(56U, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(52U, ssize);

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,                       // c_code
    23,                       // s_name
    0,                        // inParamCount
    180+4,                      // d_firstScopedDirective
    1,                        // operationCount
    276+4,                      // d_nextDirective
    BrigNone,
    0,
    0,                        // outParamCount
    0,
  };

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context->get_directive(184, &arg_scope);
  EXPECT_EQ(8U, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32U, arg_scope.c_code);

  context->get_directive(272, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32U, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0U, cbr_op.o_operands[0]);
  EXPECT_EQ(32U, cbr_op.o_operands[1]);
  EXPECT_EQ(24U, cbr_op.o_operands[2]);
  EXPECT_EQ(44U, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(24, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20U, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(188U+4U, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(228U+4U, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(32, &arg_l);
  EXPECT_EQ(12U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16U, arg_l.o_args[0]);

  context->get_operand(44, &arg_l);
  EXPECT_EQ(12U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8U, arg_l.o_args[0]);

  delete lexer;
}

TEST(CodegenTest, Example5_SimpleCall) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small; \n");
  input.append("function &callee()(){ \n");
  input.append(" ret; \n");
  input.append("}; \n");
  input.append(" function &caller()(){ \n");
  input.append(" { \n");
  input.append("   call &callee; \n");
  input.append(" } \n");
  input.append("}; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));
  EXPECT_EQ(0, Function(context));

  // test the sizes of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(116U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(16U, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(16U, ssize);

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,                       // c_code
    8,                        // s_name
    0,                        // inParamCount
    100,                      // d_firstScopedDirective
    1,                        // operationCount
    116,                      // d_nextDirective
    BrigNone,
    0,
    0,                        // outParamCount
    0,
  };

  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context->get_directive(100, &arg_scope);
  EXPECT_EQ(8U, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32U, arg_scope.c_code);

  context->get_directive(108, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32U, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0U, cbr_op.o_operands[0]);
  EXPECT_EQ(0U, cbr_op.o_operands[1]);
  EXPECT_EQ(8U, cbr_op.o_operands[2]);
  EXPECT_EQ(0U, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(8, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20U, func_o.fn);

  delete lexer;
}

TEST(CodegenTest, Example4_Branch) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small;\n");
  input.append("function &branch_ops (arg_u8x4 %x)() { \n");
  input.append("  cbr $c1, @then; \n");
  input.append("  abs_p_s8x4 $s1, $s2; \n");
  input.append("  brn @outof_IF; \n");
  input.append("  @then: \n");
  input.append("  add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append("  @outof_IF: \n");
  input.append("  ret; \n");
  input.append("}; \n");

  // test the rule
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));

  // test the sizes of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(124U+4U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(164U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(84U, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(51U, ssize);

  // test BrigDirectiveFunction
  BrigDirectiveFunction ref = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      0,                        // c_code
      0,                        // s_name
      0,                        // inParamCount
      100+4,                      // d_firstScopedDirective
      5,                        // operationCount
      124+4,                      // d_nextDirective
      BrigNone,
      0,
      1,                        // outParamCount
      0,
    };

  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveLabel
  BrigDirectiveLabel label1;
  context->get_directive(104, &label1);
  EXPECT_EQ(12U, label1.size);
  EXPECT_EQ(100U, label1.c_code);
  EXPECT_EQ(27U, label1.s_name);

  context->get_directive(116, &label1);
  EXPECT_EQ(132U, label1.c_code);
  EXPECT_EQ(41U, label1.s_name);

  // test BrigCbr
  BrigInstBase cbr_op;
  context->get_code(0, &cbr_op);
  EXPECT_EQ(32U, cbr_op.size);
  EXPECT_EQ(BrigCbr, cbr_op.opcode);
  EXPECT_EQ(Brigb1, cbr_op.type);
  EXPECT_EQ(0U, cbr_op.o_operands[0]);
  EXPECT_EQ(8U, cbr_op.o_operands[1]);
  EXPECT_EQ(44U, cbr_op.o_operands[2]);
  EXPECT_EQ(0U, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigBrn
  BrigInstBar br_op;
  context->get_code(64, &br_op);
  EXPECT_EQ(36U, br_op.size);
  EXPECT_EQ(BrigBrn, br_op.opcode);
  EXPECT_EQ(0U, br_op.o_operands[0]);
  EXPECT_EQ(76U, br_op.o_operands[1]);
  EXPECT_EQ(0U, br_op.o_operands[2]);
  EXPECT_EQ(0U, br_op.o_operands[3]);
  EXPECT_EQ(0U, br_op.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Example3_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small; \n");
  input.append("static function &packed_ops (extern arg_u8x4 %x)() {");
  input.append("  abs_p_s8x4 $s1, $s2; \n");
  input.append("  add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append("}; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));

  // test the .directive section size
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(100U+4U, dsize);

  // test the .directive section
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    0,                        // c_code
    0,                        // s_name
    0,                        // inParamCount
    96+4,                       // d_firstScopedDirective
    2,                        // operationCount
    96+4,                       // d_nextDirective
    BrigStatic,
    0,
    1,                        // outParamCount
    0,
  };

  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);
  EXPECT_EQ(ref.attribute, get.attribute);

  // test the .string size
  BrigsOffset32_t size = context->get_string_offset();
  EXPECT_EQ(31U, size);

  // test .code section.
  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64U, csize);

  BrigInstBase get_c;
  context->get_code(32, &get_c);
  EXPECT_EQ(BrigAdd, get_c.opcode);
  EXPECT_EQ(BrigPackPPsat, get_c.packing);
  EXPECT_EQ(Brigu16x2, get_c.type);
  EXPECT_EQ(8U, get_c.o_operands[0]);
  EXPECT_EQ(32U, get_c.o_operands[1]);
  EXPECT_EQ(44U, get_c.o_operands[2]);

  delete lexer;
}

TEST(CodegenTest, Instrustion3Op_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("add_pp_sat_u16x2 $s1, $s0, $s3; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAdd,
    Brigu16x2,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  };

  BrigInstBase get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instrustion2Op_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("abs_p_s8x4 $s1, $s2; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  BrigInstBase get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instrustion2Op_FTZ_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("sqrt_s8x4 $s1, $s2; \n");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigSqrt,
    Brigs8x4,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  BrigInstBase get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instrustion2Op_FTZ_With_Modifier_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("sqrt_ftz_s8x4 $s1, $s2; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigAluModifier bam;
  bam.ftz = 1;

  BrigInstMod ref = {
    sizeof(ref),       // size
    BrigEInstMod,      // kind
    BrigSqrt,          // opcode
    Brigs8x4,          // type
    BrigNoPacking,     // packing
    {8, 20, 0, 0, 0},  // operand
    bam
  };

  BrigInstMod get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);

  delete lexer;
}

TEST(CodegenTest, Instrustion2Op_with_Modifier_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("abs_ftz_p_s8x4 $s1, $s2; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigAluModifier bam;
  bam.ftz = 1;

  BrigInstMod ref = {
    sizeof(ref),       // size
    BrigEInstMod,      // kind
    BrigAbs,           // opcode
    Brigs8x4,          // type
    BrigPackP,         // packing
    {8, 20, 0, 0, 0},  // operand
    bam
  };

  BrigInstMod get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);

  delete lexer;
}

TEST(CodegenTest, SimplestFunction_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small; \n");
  input.append("function &return_true(arg_f32 %ret_val)(){ \n");
  input.append(" ret; \n");
  input.append("};");

  // test the rule
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));
  EXPECT_EQ(0, Function(context));

  // test the .directive section size
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(100U+4U, dsize);

  // test the offset to the .string section
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    0,                        // c_code
    0,                        // s_name
    0,                        // inParamCount
    96+4,                       // d_firstScopedDirective
    1,                        // operationCount
    96+4,                       // d_nextDirective
    BrigNone,
    0,
    1,                        // outParamCount
    0,
  };

  BrigDirectiveFunction get;
  context->get_directive(context->current_bdf_offset, &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test the .string size
  BrigsOffset32_t size = context->get_string_offset();
  EXPECT_EQ(22U, size);

  // find the string.
  std::string func_name("&return_true");
  int str_offset = context->lookup_symbol(func_name);
  EXPECT_EQ(0, str_offset);

  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(32U, csize);

  delete lexer;
}

TEST(CodegenTest, AlignmentCheck) {
  // Try the situation in PRM 20.2 (pg. 226)

  // use a new context object to ensure the problem happen
  // since if at beginning the offset is a multiple of 4 but not a multiple of 8
  // then appending a 4-byte aligned item will lead to a multiple-of-8 offset
  context->clear_context();

  // First append a 4-byte aligned item BrigBlockStart
  uint32_t curr_offset = context->get_directive_offset();

  BrigBlockStart bbs = {
    12,                        // size
    BrigEDirectiveBlockStart,  // kind
    0,                         // c_code
    0                          // s_name;
  };

  context->append_directive(&bbs);    // append_directiveirective
  curr_offset = context->get_directive_offset();

  EXPECT_EQ(0U, curr_offset%4);
  EXPECT_EQ(BrigEAlignment_4, Context::alignment_check(bbs));

  // Next append a 8-byte aligned item  such as BrigBlockNumeric
  BrigBlockNumeric bbn = {
    16,                          // size
    BrigEDirectiveBlockNumeric,  // kind
    Brigb64,                     // type
    1,                           // elementCount
    { { 0 } },                   // u64
  };
  bbn.u64[0] = 1;

  context->append_directive(&bbn);
  curr_offset = context->get_directive_offset();

  EXPECT_EQ(BrigEAlignment_8, Context::alignment_check(bbn));
  // this is a 8-byte aligned item and has a size of multiple of 8.
  // so the offset after appending this item should be a multiple of 8.
  EXPECT_EQ(0U, curr_offset%8);
}

TEST(CodegenTest, VersionCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  std::string input("\n version 1:0; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  uint32_t curr_d_offset = context->get_directive_offset();

  BrigDirectiveVersion ref = {
    sizeof(ref),
    BrigEDirectiveVersion,
    0,            // unknown c_code
    1,            // major
    0,            // minor
    BrigELarge,   // machine
    BrigEFull,    // profile
    BrigENosftz,  // ftz
    0             // reserved
  };


  // get structure back
  BrigDirectiveVersion get;
  context->get_directive(curr_d_offset-sizeof(get), &get);

  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

  /* ---------- TEST 2 ---------*/
  context->clear_context();

  input.assign("version 2:0:$large;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;

  // get structure back
  curr_d_offset = context->get_directive_offset();
  context->get_directive(curr_d_offset-sizeof(get), &get);

  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

        /* TEST 3, Multi Target */
  context->clear_context();
  input.assign("version 2:0:$large, $reduced, $sftz;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Version(context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;
  ref.profile = BrigEReduced;
  ref.ftz = BrigESftz;

  // get structure back
  curr_d_offset = context->get_directive_offset();
  context->get_directive(curr_d_offset-sizeof(get), &get);

  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);
  context->clear_context();

  delete lexer;
}

TEST(CodegenTest, RegisterOperandCodeGen) {
  std::string name;
  std::string input("$d7");  // register

  Lexer* lexer = new Lexer();
  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  parser->clear_context();
  // scan symbols
  parser->scan_symbols();

  // rescan
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Operand(context));

  // reference struct
  BrigOperandReg ref = {
    sizeof(ref),      // size
    BrigEOperandReg,  // kind
    Brigb64,          // type
    0,                // reserved
    0                 // name
  };

  name.assign("$d7");
  ref.name = context->lookup_symbol(name);
  EXPECT_EQ(0U, ref.name);

  // get structure from context and compare
  BrigOperandReg get;
  uint32_t curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);

  // second register
  input.assign("$q7");
  parser->set_source_string(input);
  // scan symbols
  parser->scan_symbols();

  // rescan
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  name.assign("$q7");
  ref.name = context->lookup_symbol(name);
  ref.type = Brigb128;
  curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);

  delete lexer;
  delete parser;
}

TEST(CodegenTest, NumericValueOperandCodeGen) {
  /* Integer */
  std::string input("5");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  BrigOperandImmed ref = {
    sizeof(ref),        // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  };

  ref.bits.u = 5;
  // get structure from context and compare
  BrigOperandImmed get;
  uint32_t curr_o_offset = context->get_operand_offset();
  // to overcome padding
  context->get_operand<BrigOperandImmed>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);

    /* Negative Integer */
  input.assign("-5");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.u = -5;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  /* float single */
  input.assign("5.0f");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.f = 5;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.f, get.bits.f);

  /* double */
  input.assign("5.0l");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.bits.d = 5;
  ref.type = Brigb64;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.d, get.bits.d);

  /* Integer List */
  input.assign("_b32(5,6,8)");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.u = 8;
  // get last structure from context and compare
  curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  delete lexer;
}

TEST(CodegenTest, LookupStringTest) {
  std::string input("&test_string1");

  StringBuffer* strBuf = new StringBuffer();

  strBuf->append(input);

  uint32_t offset = strBuf->size();
  input.assign("&test_string2");
  strBuf->append(input);

  // lookup first string
  input.assign("&test_string1");
  uint32_t loc = strBuf->lookup(input);
  EXPECT_EQ(0U, loc);

  input.assign("&test_string2");
  loc = strBuf->lookup(input);
  EXPECT_EQ(offset, loc);

  delete strBuf;
}

TEST(CodegenTest, AddSymbolTest) {
  std::string symbol("&symbol1");
  uint32_t offset = context->get_string_offset();

  // add symbol
  uint32_t sym1_offset = context->add_symbol(symbol);
  EXPECT_EQ(offset, sym1_offset);

  offset = context->get_string_offset();
  symbol.assign("%symbol2");
  uint32_t sym2_offset = context->add_symbol(symbol);
  EXPECT_EQ(offset, sym2_offset);

  // try to add symbol 1 again
  symbol.assign("&symbol1");
  uint32_t sym1b_offset = context->add_symbol(symbol);
  EXPECT_EQ(sym1_offset, sym1b_offset);

  // lookup
  symbol.assign("%symbol2");
  uint32_t lookup_sym2 = context->lookup_symbol(symbol);

  EXPECT_EQ(sym2_offset, lookup_sym2);
}

TEST(CodegenTest, LookupStringBugTest) {
  std::string input("&previous_test_string1");

  StringBuffer* strBuf = new StringBuffer();

  strBuf->append(input);

  uint32_t offset = strBuf->size();
  input.assign("test_string1");
  strBuf->append(input);


  // lookup second string
  input.assign("test_string1");
  uint32_t loc = strBuf->lookup(input);
  EXPECT_EQ(offset, loc);

  delete strBuf;
}

TEST(CodegenTest, BrigOperandAddressGeneration) {
  std::string name;
  std::string input("[&test]");  // [name]

  Lexer* lexer = new Lexer();
  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  // scan symbols
  parser->scan_symbols();

  // rescan

  lexer->set_source_string(input);
  // get 2 tokens pass '['
  context->token_to_scan = lexer->get_next_token();
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, AddressableOperand(context));

  name.assign(input.c_str());
  BrigOperandAddress ref = {
    sizeof(ref),          // size
    BrigEOperandAddress,  // kind
    Brigb32,              // Data Type
    0,                    // reserved
    context->symbol_map[name],                    // directive
    0,                    // offset -> ??
  };

  if (context->get_machine() == BrigELarge)
    ref.type = Brigb64;

    // get structure from context and compare
  BrigOperandAddress get;
  uint32_t curr_o_offset = context->get_operand_offset();
  context->get_operand(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.directive, get.directive);

  delete lexer;
  delete parser;
}


TEST(CodegenTest, Instruction2Op_CodeGen_abs_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_s64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_p_s8x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_p_s8x4 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}


TEST(CodegenTest, Instruction2Op_CodeGen_abs_p_s16x2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs16x2,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_p_s16x2 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_p_s8x8) {
  Lexer* lexer = new Lexer();

  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs8x8,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_p_s8x8 $d1, $d2;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_p_s16x4) {
  Lexer* lexer = new Lexer();
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs16x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_p_s16x4 $d1, $d2;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_s_s8x4) {
  Lexer* lexer = new Lexer();
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs8x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s_s8x4 $s1, $s2;");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_s_s16x2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs16x2,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s_s16x2 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_s_s8x8) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs8x8,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s_s8x8 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_abs_s_s16x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAbs,
    Brigs16x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("abs_s_s16x4 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_p_s8x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs8x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_p_s8x4 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}


TEST(CodegenTest, Instruction2Op_CodeGen_neg_p_s16x2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs16x2,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_p_s16x2 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_p_s8x8) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs8x8,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_p_s8x8 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_p_s16x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs16x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_p_s16x4 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s_s8x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs8x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s_s8x4 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s_s16x2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs16x2,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s_s16x2 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s_s8x8) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs8x8,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s_s8x8 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_neg_s_s16x4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNeg,
    Brigs16x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };

  std::string input("neg_s_s16x4 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_not_b1) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNot,
    Brigb1,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("not_b1 $c1, $c2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_not_b32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNot,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("not_b32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_not_b64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNot,
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("not_b64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}


TEST(CodegenTest, Instruction2Op_CodeGen_popcount_b32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigPopcount,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("popcount_b32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_popcount_b64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigPopcount,
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("popcount_b64 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_bitrev_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigBitRev,
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("bitrev_s32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_bitrev_s64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigBitRev,
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("bitrev_s64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_bitrev_u32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigBitRev,
    Brigu32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("bitrev_u32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_bitrev_u64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigBitRev,
    Brigu64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("bitrev_u64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_mask_b64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigMask,
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("mask_b64 $d1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_firstbit_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFirstbit,
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("firstbit_s32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_firstbit_s64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFirstbit,
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("firstbit_s64 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_firstbit_u32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFirstbit,
    Brigu32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("firstbit_u32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_firstbit_u64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFirstbit,
    Brigu64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("firstbit_u64 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_lastbit_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigLastbit,
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("lastbit_s32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_lastbit_s64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigLastbit,
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("lastbit_s64 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_lastbit_u32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigLastbit,
    Brigu32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("lastbit_u32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_lastbit_u64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigLastbit,
    Brigu64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("lastbit_u64 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_count_u32) {
  context->set_error_reporter(main_reporter);

  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigCount,
    Brigu32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("count_u32 $s1, $s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_movs_lo_b32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    Brigmovslo,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("movs_lo_b32 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}

TEST(CodegenTest, Instruction2Op_CodeGen_movs_hi_b32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    Brigmovshi,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("movs_hi_b32 $s1, $d2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  delete lexer;
}
TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_unpack3) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigUnpack3,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("unpack3 $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_unpack2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
     32,
    BrigEInstBase,
    BrigUnpack2,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("unpack2 $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}
TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_unpack1) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
     32,
    BrigEInstBase,
    BrigUnpack1,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("unpack1 $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}
TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_unpack0) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
     32,
    BrigEInstBase,
    BrigUnpack0,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("unpack0 $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_alloca) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
     32,
    BrigEInstBase,
    BrigAlloca,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("alloca $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_workitemid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigWorkItemId,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("workitemid $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_workitemaid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigWorkItemAId,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("workitemaid $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_workgroupid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigWorkGroupId,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("workgroupid $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_workgroupsize) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigWorkGroupSize,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("workgroupsize $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_NDRangesize) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNDRangesize,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("NDRangesize $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Instrustion2Op_NODT_CodeGen_NDRangegroups) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigNDRangegroups,
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  std::string input("NDRangegroups $s1,$s2;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, RetCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigRet,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  std::string input("ret ;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ret(context));

  BrigInstBase get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, SyncCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBar ref = {
    36,
    BrigEInstBar,
    BrigSync,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGlobalLevel
  };

  std::string input("sync_global ;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Sync(context));

  BrigInstBar get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref.syncFlags, get.syncFlags);

  delete lexer;
}

TEST(CodegenTest, BarCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBar ref = {
    36,
    BrigEInstBar,
    BrigBarrier,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0},
    BrigGroupLevel
  };

  std::string input("barrier_group ;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Bar(context));

  BrigInstBar get;
  context->get_code(0, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref.syncFlags, get.syncFlags);

  //case 2
  context->clear_context();
  input.assign("barrier_width(all)_group ;");
  
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Bar(context));

  ref.o_operands[0] = 8 ;//update

  BrigInstBar get_bib;
  context->get_code(0, &get_bib);

  EXPECT_EQ(ref.size,get_bib.size);
  EXPECT_EQ(ref.kind,get_bib.kind);
  EXPECT_EQ(ref.opcode, get_bib.opcode);
  EXPECT_EQ(ref.packing, get_bib.packing);
  EXPECT_EQ(ref.type, get_bib.type);
  EXPECT_EQ(ref.o_operands[0], get_bib.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get_bib.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get_bib.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get_bib.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get_bib.o_operands[4]);
  EXPECT_EQ(ref.syncFlags, get_bib.syncFlags);

  BrigOperandImmed boi = {
    24,
    BrigEOperandImmed,
    Brigb32,
    0,
    0
  };

  BrigOperandImmed get_boi;
  context->get_operand(8, &get_boi);

  EXPECT_EQ(boi.size,get_boi.size);
  EXPECT_EQ(boi.kind,get_boi.kind);
  EXPECT_EQ(boi.type, get_boi.type);
  EXPECT_EQ(boi.reserved, get_boi.reserved);
  EXPECT_EQ(boi.bits.u, get_boi.bits.u);

  delete lexer;
}

TEST(CodegenTest, OptionalWidthCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandImmed ref = {
    24,
    BrigEOperandImmed,
    Brigb32,
    0,
    2
  };

  std::string input("_width(2);");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, OptionalWidth(context));

  BrigOperandImmed get;
  context->get_operand(8, &get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  delete lexer;
}

TEST(CodegenTest, LdSt_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstLdSt ref = {
    44,                // size
    BrigEInstLdSt,     // kind
    BrigLd,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, 8, 20, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  BrigInstLdSt get;  

  std::string input("ld_arg_f32 $s0, [%input];\n");
  input.append("st_arg_f32 $s0, [%output];\n");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Ld(context));
  EXPECT_EQ(0, St(context));
  context->get_code(0, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic);
  EXPECT_EQ(ref.equivClass, get.equivClass);

  BrigInstLdSt tmp = {
    44,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {8, 36, 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  ref = tmp;  

  context->get_code(44, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic);
  EXPECT_EQ(ref.equivClass, get.equivClass);

  delete lexer;
};

TEST(CodegenTest, MemoryOperand_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandIndirect ref = {
    16,                    // size
    BrigEOperandIndirect,  // kind
    8,                     // reg
    Brigb32,               // type
    0,                     // reserved
    8                      // offset
  };
  BrigOperandIndirect get;
  BrigOperandReg getReg;
  BrigOperandAddress getAddr;
  std::string input("[$s1+8]\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));
  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  context->get_operand(20, &get);
  // BrigOperandIndirect
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.reg, get.reg);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.offset, get.offset);

  context->clear_context();
  input.assign("[0x7f]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));
  context->get_operand(8, &get);
 
  ref.reg = 0;
  ref.offset = 0x7f;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.reg, get.reg);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.offset, get.offset);

  BrigOperandCompound ref2 = {
    20,                    // size
    BrigEOperandCompound,  // kind
    Brigb32,               // type
    0,                     // reserved
    8,                     // name
    24,                    // reg
    -16                    // offset
  };

  BrigOperandCompound get2;
  memset(&getAddr, 0, sizeof(getAddr));
  memset(&getReg, 0, sizeof(getReg));

  context->clear_context();
  input.assign("[&array][$s1-16]\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  context->add_symbol("&array");
  EXPECT_EQ(0, MemoryOperand(context));
  
  context->get_operand(8, &getAddr);
  context->get_operand(24, &getReg);
  context->get_operand(36, &get2);
  
  // BrigOperandAddress
  EXPECT_EQ(16, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb32, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  EXPECT_EQ(0, getAddr.offset);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(7, getReg.name);
  // BrigOperandCompoud
  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.type, get2.type);
  EXPECT_EQ(ref2.reserved, get2.reserved);
  EXPECT_EQ(ref2.name, get2.name);
  EXPECT_EQ(ref2.reg, get2.reg);
  EXPECT_EQ(ref2.offset, get2.offset);


  delete lexer;
};

TEST(CodegenTest, ArrayOperandList_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandRegV4 refV4 = {
    24,                    // size
    BrigEOperandRegV4,     // kind
    Brigb32,               // type
    0,                     // reserved
    {8, 20, 32, 8}         // regs
  };

  BrigOperandRegV4 getRegV4;
  BrigOperandRegV2 getRegV2;
  BrigOperandReg getReg;

  std::string input("( $s1,$s2, $s3 , $s1)\n");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandList(context));
  context->get_operand(44, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(refV4.size, getRegV4.size);
  EXPECT_EQ(refV4.kind, getRegV4.kind);
  EXPECT_EQ(refV4.type, getRegV4.type);
  EXPECT_EQ(refV4.reserved, getRegV4.reserved);
  EXPECT_EQ(refV4.regs[0], getRegV4.regs[0]);
  EXPECT_EQ(refV4.regs[1], getRegV4.regs[1]);
  EXPECT_EQ(refV4.regs[2], getRegV4.regs[2]);
  EXPECT_EQ(refV4.regs[3], getRegV4.regs[3]);

  BrigOperandRegV2 refV2 = {
    16,                    // size
    BrigEOperandRegV2,     // kind
    Brigb1,                // type
    0,                     // reserved
    {68, 68}               // regs
  };

  input.assign("($c2, $c2)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandList(context));
  context->get_operand(80, &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(refV2.size, getRegV2.size);
  EXPECT_EQ(refV2.kind, getRegV2.kind);
  EXPECT_EQ(refV2.type, getRegV2.type);
  EXPECT_EQ(refV2.reserved, getRegV2.reserved);
  EXPECT_EQ(refV2.regs[0], getRegV2.regs[0]);
  EXPECT_EQ(refV2.regs[1], getRegV2.regs[1]);

  input.assign("($d1)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandList(context));
  context->get_operand(96, &getReg);
 
  BrigOperandReg ref = {
    12,                    // size
    BrigEOperandReg,       // kind
    Brigb64,               // type
    0,                     // reserved
    16                     // name
  };

  EXPECT_EQ(ref.size, getReg.size);
  EXPECT_EQ(ref.kind, getReg.kind);
  EXPECT_EQ(ref.type, getReg.type);
  EXPECT_EQ(ref.reserved, getReg.reserved);
  EXPECT_EQ(ref.name, getReg.name);


  delete lexer;
};

TEST(CodegenTest, PairAddressableOperand_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("[&array][$s1-16]\n");

  Lexer* lexer = new Lexer(input);

  BrigOperandCompound ref = {
    20,                    // size
    BrigEOperandCompound,  // kind
    Brigb32,               // type
    0,                     // reserved
    8,                     // name
    24,                    // reg
    -16                    // offset
  };

  BrigOperandReg getReg;
  BrigOperandAddress getAddr;
  BrigOperandCompound getComp;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  context->add_symbol("&array");

  EXPECT_EQ(0, PairAddressableOperand(context));
  
  context->get_operand(8, &getAddr);
  context->get_operand(24, &getReg);
  context->get_operand(36, &getComp);
  
  // BrigOperandAddress
  EXPECT_EQ(16, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb32, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  EXPECT_EQ(0, getAddr.offset);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(7, getReg.name);
  // BrigOperandCompoud
  EXPECT_EQ(ref.size, getComp.size);
  EXPECT_EQ(ref.kind, getComp.kind);
  EXPECT_EQ(ref.type, getComp.type);
  EXPECT_EQ(ref.reserved, getComp.reserved);
  EXPECT_EQ(ref.name, getComp.name);
  EXPECT_EQ(ref.reg, getComp.reg);
  EXPECT_EQ(ref.offset, getComp.offset);

  delete lexer;
};

TEST(CodegenTest, Lda_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("lda_group_b32 $s1, [%loc];\n");

  Lexer* lexer = new Lexer(input);

  BrigInstMem ref = {
    36,                    // size
    BrigEInstMem,           // kind
    BrigLda,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {8, 20, 0, 0, 0},      // o_operands[5]
    BrigGroupSpace         // storageClass
  };
  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandAddress getAddr;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  context->add_symbol("%loc");

  EXPECT_EQ(0, Lda(context)); 

  context->get_operand(8, &getReg);
  context->get_operand(20, &getAddr);
  context->get_code(0, &getMem);
  
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(5, getReg.name);  
  // BrigOperandAddress
  EXPECT_EQ(16, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb32, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  EXPECT_EQ(0, getAddr.offset);

  // BrigInstMem
  EXPECT_EQ(ref.size, getMem.size);
  EXPECT_EQ(ref.kind, getMem.kind);
  EXPECT_EQ(ref.opcode, getMem.opcode);
  EXPECT_EQ(ref.type, getMem.type);
  EXPECT_EQ(ref.packing, getMem.packing);

  EXPECT_EQ(ref.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getMem.o_operands[4]);
  EXPECT_EQ(ref.storageClass, getMem.storageClass);

  delete lexer;
};

TEST(CodegenTest, Segp_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("stof_private_u64 $d2, $d1;\n");
  input.append("segmentp_group_b1 $c1, $d0;\n");
  
  Lexer* lexer = new Lexer(input);

  BrigInstMem ref1 = {
    36,                    // size
    BrigEInstMem,          // kind
    BrigStoF,              // opcode
    Brigu64,               // type
    BrigNoPacking,         // packing
    {8, 20, 0, 0, 0},      // o_operands[5]           
    BrigPrivateSpace       // storageClass
  };

  BrigInstMem ref2 = {
    36,                    // size
    BrigEInstMem,          // kind
    BrigSegmentp,          // opcode
    Brigb1,                // type
    BrigNoPacking,         // packing
    {32, 44, 0, 0, 0},     // o_operands[5]           
    BrigGroupSpace         // storageClass
  };

  BrigInstMem getMem;
  BrigOperandReg getReg1, getReg2;
  BrigOperandAddress getAddr;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context)); 
  EXPECT_EQ(0, Segp(context));

  context->get_operand(8, &getReg1);
  context->get_operand(20, &getReg2);
  context->get_code(0, &getMem);
  
  // BrigOperandReg
  EXPECT_EQ(12, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb64, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(0, getReg1.name);  
  // BrigOperandReg
  EXPECT_EQ(12, getReg2.size);
  EXPECT_EQ(BrigEOperandReg, getReg2.kind);
  EXPECT_EQ(Brigb64, getReg2.type);
  EXPECT_EQ(0, getReg2.reserved);
  EXPECT_EQ(4, getReg2.name);  
  
  // BrigInstMem
  EXPECT_EQ(ref1.size, getMem.size);
  EXPECT_EQ(ref1.kind, getMem.kind);
  EXPECT_EQ(ref1.opcode, getMem.opcode);
  EXPECT_EQ(ref1.type, getMem.type);
  EXPECT_EQ(ref1.packing, getMem.packing);

  EXPECT_EQ(ref1.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref1.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref1.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref1.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref1.o_operands[4], getMem.o_operands[4]);
  EXPECT_EQ(ref1.storageClass, getMem.storageClass);


  context->get_operand(32, &getReg1);
  context->get_operand(44, &getReg2);
  context->get_code(36, &getMem);
  
  // BrigOperandReg
  EXPECT_EQ(12, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb1, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(8, getReg1.name);  
  // BrigOperandReg
  EXPECT_EQ(12, getReg2.size);
  EXPECT_EQ(BrigEOperandReg, getReg2.kind);
  EXPECT_EQ(Brigb64, getReg2.type);
  EXPECT_EQ(0, getReg2.reserved);
  EXPECT_EQ(12, getReg2.name);  
  
  // BrigInstMem
  EXPECT_EQ(ref2.size, getMem.size);
  EXPECT_EQ(ref2.kind, getMem.kind);
  EXPECT_EQ(ref2.opcode, getMem.opcode);
  EXPECT_EQ(ref2.type, getMem.type);
  EXPECT_EQ(ref2.packing, getMem.packing);

  EXPECT_EQ(ref2.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref2.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref2.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref2.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref2.o_operands[4], getMem.o_operands[4]);
  EXPECT_EQ(ref2.storageClass, getMem.storageClass);

  delete lexer;
};

TEST(CodegenTest, Ldc_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("ldc_b64 $s1, &foo;\n");
  input.append("ldc_b32 $s2, @lab;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstBase ref1 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigLdc,               // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {24, 8, 0, 0, 0}       // o_operands[5]
  };
  BrigInstBase ref2 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigLdc,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {36, 16, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase getBase;
  BrigOperandReg getReg;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  BrigOperandFunctionRef fn_pad_op = {
    8,                        // size
    BrigEOperandFunctionRef,  // kind
    0                         // fn
  };
  BrigOperandLabelRef lab_pad_op = {
    8,                     // size
    BrigEOperandLabelRef,  // kind
    0                      // labeldirective
  };
  
  context->add_symbol("&foo");
  context->add_symbol("@lab");
  context->operand_map["&foo"] = context->get_operand_offset();
  context->append_operand(&fn_pad_op);
  context->operand_map["@lab"] = context->get_operand_offset();
  context->append_operand(&lab_pad_op);
  
  
  EXPECT_EQ(0, Ldc(context)); 
  EXPECT_EQ(0, Ldc(context));

  context->get_operand(24, &getReg);  
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(10, getReg.name);  

  context->get_operand(36, &getReg);  
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(14, getReg.name);  

  context->get_code(0, &getBase);
  // BrigInstBase
  EXPECT_EQ(ref1.size, getBase.size);
  EXPECT_EQ(ref1.kind, getBase.kind);
  EXPECT_EQ(ref1.opcode, getBase.opcode);
  EXPECT_EQ(ref1.type, getBase.type);
  EXPECT_EQ(ref1.packing, getBase.packing);

  EXPECT_EQ(ref1.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref1.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref1.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref1.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref1.o_operands[4], getBase.o_operands[4]);

  context->get_code(32, &getBase);
  // BrigInstBase
  EXPECT_EQ(ref2.size, getBase.size);
  EXPECT_EQ(ref2.kind, getBase.kind);
  EXPECT_EQ(ref2.opcode, getBase.opcode);
  EXPECT_EQ(ref2.type, getBase.type);
  EXPECT_EQ(ref2.packing, getBase.packing);
  EXPECT_EQ(ref2.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref2.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref2.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref2.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref2.o_operands[4], getBase.o_operands[4]);

  delete lexer;
};

TEST(CodegenTest, Atom_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("atomic_cas_global_ar_b64 $d1, [&x1], 23, 12;\n");
  input.append("atomic_and_global_ar_u32 $s1, [&x2], 24;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstAtomic ref1 = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomic,            // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {8, 20, 40, 64, 0},    // o_operands[5]
    BrigAtomicCas,         // atomicOperation;
    BrigGlobalSpace,       // storageClass;
    BrigAcquireRelease     // memorySemantic;
  };

  BrigInstAtomic ref2 = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomic,            // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {88, 100, 120, 0, 0},   // o_operands[5]
    BrigAtomicAnd,         // atomicOperation;
    BrigGlobalSpace,       // storageClass;
    BrigAcquireRelease     // memorySemantic;
  };
  
  BrigInstAtomic getAtom;
  BrigOperandReg getReg;
  BrigOperandAddress getAddr;
  BrigOperandImmed getImm;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  
  context->add_symbol("&x1");
  context->add_symbol("&x2");
  
  EXPECT_EQ(0, Atom(context));
  EXPECT_EQ(0, Atom(context));

  context->get_operand(8, &getReg);  
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);  

  context->get_operand(20, &getAddr);  
  // BrigOperandAddress
  EXPECT_EQ(16, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb32, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  EXPECT_EQ(0, getAddr.offset); 

  context->get_operand(40, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(23, getImm.bits.u);

  context->get_operand(64, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(12, getImm.bits.u);

  context->get_code(0, &getAtom);
  // BrigInstAtomic
  EXPECT_EQ(ref1.size, getAtom.size);
  EXPECT_EQ(ref1.kind, getAtom.kind);
  EXPECT_EQ(ref1.opcode, getAtom.opcode);
  EXPECT_EQ(ref1.type, getAtom.type);
  EXPECT_EQ(ref1.packing, getAtom.packing);

  EXPECT_EQ(ref1.o_operands[0], getAtom.o_operands[0]);
  EXPECT_EQ(ref1.o_operands[1], getAtom.o_operands[1]);
  EXPECT_EQ(ref1.o_operands[2], getAtom.o_operands[2]);
  EXPECT_EQ(ref1.o_operands[3], getAtom.o_operands[3]);
  EXPECT_EQ(ref1.o_operands[4], getAtom.o_operands[4]);
  EXPECT_EQ(ref1.atomicOperation, getAtom.atomicOperation);
  EXPECT_EQ(ref1.storageClass, getAtom.storageClass);
  EXPECT_EQ(ref1.memorySemantic, getAtom.memorySemantic);

  context->get_operand(88, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(100, &getAddr);  
  // BrigOperandAddress
  EXPECT_EQ(16, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb32, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  EXPECT_EQ(0, getAddr.offset); 

  context->get_operand(120, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(24, getImm.bits.u);

  context->get_code(44, &getAtom);
  // BrigInstAtomic
  EXPECT_EQ(ref2.size, getAtom.size);
  EXPECT_EQ(ref2.kind, getAtom.kind);
  EXPECT_EQ(ref2.opcode, getAtom.opcode);
  EXPECT_EQ(ref2.type, getAtom.type);
  EXPECT_EQ(ref2.packing, getAtom.packing);

  EXPECT_EQ(ref2.o_operands[0], getAtom.o_operands[0]);
  EXPECT_EQ(ref2.o_operands[1], getAtom.o_operands[1]);
  EXPECT_EQ(ref2.o_operands[2], getAtom.o_operands[2]);
  EXPECT_EQ(ref2.o_operands[3], getAtom.o_operands[3]);
  EXPECT_EQ(ref2.o_operands[4], getAtom.o_operands[4]);
  EXPECT_EQ(ref2.atomicOperation, getAtom.atomicOperation);
  EXPECT_EQ(ref2.storageClass, getAtom.storageClass);
  EXPECT_EQ(ref2.memorySemantic, getAtom.memorySemantic);

  delete lexer;
};


}  // namespace brig
}  // namespace hsa
