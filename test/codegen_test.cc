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
  EXPECT_EQ(148U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(40U, csize);

  BrigDirectiveKernel ref = {
    40,                       // size
    BrigEDirectiveKernel,   // kind
    8,                       // c_code
    8,                       // s_name
    1,                        // inParamCount
    108,                      // d_firstScopedDirective
    1,                        // operationCount
    148,                      // d_nextDirective
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
    BrigKernargSpace,         // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    14,                       // s_name
    Brigf32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  BrigDirectiveSymbol get_sym;
  context->get_directive(68, &get_sym);
  EXPECT_EQ(kernarg.size, get_sym.size);
  EXPECT_EQ(kernarg.kind, get_sym.kind);
  EXPECT_EQ(kernarg.s.storageClass, get_sym.s.storageClass);
  EXPECT_EQ(kernarg.s.s_name, get_sym.s.s_name);

  BrigDirectiveSymbol private_var = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    BrigPrivateSpace,         // storag class kernarg
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    17,                        // s_name
    Brigu32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };

  context->get_directive(108, &get_sym);
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
  EXPECT_EQ(324U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(72U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(84, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(69U, ssize);

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    40,                       // c_code
    40,                       // s_name
    0,                        // inParamCount
    228,                      // d_firstScopedDirective
    1,                        // operationCount
    324,                      // d_nextDirective
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
  context->get_directive(228, &arg_scope);
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(40U, arg_scope.c_code);

  context->get_directive(316, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(72U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(40, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(24, cbr_op.o_operands[0]);
  EXPECT_EQ(56, cbr_op.o_operands[1]);
  EXPECT_EQ(48, cbr_op.o_operands[2]);
  EXPECT_EQ(68, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(48, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(28U, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(196U+40U, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(236U+40U, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(56, &arg_l);
  EXPECT_EQ(12U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16U, arg_l.o_args[0]);

  context->get_operand(68, &arg_l);
  EXPECT_EQ(16U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8U, arg_l.o_args[0]);

  BrigoOffset32_t arg_test = 0;
  context->get_operand(80, &arg_test);
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
  EXPECT_EQ(284, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(72, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(80, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(60, ssize);

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    40,                       // c_code
    31,                       // s_name
    0,                        // inParamCount
    188,                      // d_firstScopedDirective
    1,                        // operationCount
    284,                      // d_nextDirective
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
  context->get_directive(188, &arg_scope);
  EXPECT_EQ(8U, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(40U, arg_scope.c_code);

  context->get_directive(276, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(72U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(40, &cbr_op);
  EXPECT_EQ(32U, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(24, cbr_op.o_operands[0]);
  EXPECT_EQ(56, cbr_op.o_operands[1]);
  EXPECT_EQ(48, cbr_op.o_operands[2]);
  EXPECT_EQ(68, cbr_op.o_operands[3]);
  EXPECT_EQ(0U, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(48, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(28U, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(196U, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8U, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(236U, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(56, &arg_l);
  EXPECT_EQ(12U, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16U, arg_l.o_args[0]);

  context->get_operand(68, &arg_l);
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
  EXPECT_EQ(124U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(72U, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(52, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(24U, ssize);

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    40,                       // c_code
    16,                        // s_name
    0,                        // inParamCount
    108,                      // d_firstScopedDirective
    1,                        // operationCount
    124,                      // d_nextDirective
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
  context->get_directive(108, &arg_scope);
  EXPECT_EQ(8U, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(40U, arg_scope.c_code);

  context->get_directive(116, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(72U, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(40, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(8, cbr_op.o_operands[0]);
  EXPECT_EQ(40, cbr_op.o_operands[1]);
  EXPECT_EQ(32, cbr_op.o_operands[2]);
  EXPECT_EQ(40, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(32, &func_o);
  EXPECT_EQ(8U, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(28U, func_o.fn);

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
  EXPECT_EQ(132, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(168, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(136, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(59U, ssize);

  BrigDirectiveFunction ref = {
      40,                       // size
      BrigEDirectiveFunction,   // kind
      8,                        // c_code
      8,                        // s_name
      0,                        // inParamCount
      108,                      // d_firstScopedDirective
      5,                        // operationCount
      132,                      // d_nextDirective
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
  context->get_directive(108, &label1);
  EXPECT_EQ(12U, label1.size);
  EXPECT_EQ(104, label1.c_code);
  EXPECT_EQ(35U, label1.s_name);

  context->get_directive(120, &label1);
  EXPECT_EQ(136, label1.c_code);
  EXPECT_EQ(49U, label1.s_name);

  // test BrigCbr
  BrigInstBase cbr_op;
  context->get_code(8, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCbr, cbr_op.opcode);
  EXPECT_EQ(Brigb32, cbr_op.type);
  EXPECT_EQ(8, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(44, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigBrn
  BrigInstBar br_op;
  context->get_code(72, &br_op);
  EXPECT_EQ(32, br_op.size);
  EXPECT_EQ(BrigBrn, br_op.opcode);
  EXPECT_EQ(80, br_op.o_operands[0]);
  EXPECT_EQ(104, br_op.o_operands[1]);
  EXPECT_EQ(0, br_op.o_operands[2]);
  EXPECT_EQ(0, br_op.o_operands[3]);
  EXPECT_EQ(0, br_op.o_operands[4]);

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
  EXPECT_EQ(108U, dsize);

  // test the .directive section
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    8,                        // c_code
    8,                        // s_name
    0,                        // inParamCount
    104+4,                       // d_firstScopedDirective
    2,                        // operationCount
    104+4,                       // d_nextDirective
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
  EXPECT_EQ(39U, size);

  // test .code section.
  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(72U, csize);

  BrigInstBase get_c;
  context->get_code(40, &get_c);
  EXPECT_EQ(BrigAdd, get_c.opcode);
  EXPECT_EQ(BrigPackPPsat, get_c.packing);
  EXPECT_EQ(Brigu16x2, get_c.type);
  EXPECT_EQ(8U, get_c.o_operands[0]);
  EXPECT_EQ(32U, get_c.o_operands[1]);
  EXPECT_EQ(44U, get_c.o_operands[2]);

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
  EXPECT_EQ(108U, dsize);

  // test the offset to the .string section
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    8,                        // c_code
    8,                        // s_name
    0,                        // inParamCount
    96+4+8,                       // d_firstScopedDirective
    1,                        // operationCount
    96+4+8,                       // d_nextDirective
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
  EXPECT_EQ(30U, size);

  // find the string.
  std::string func_name("&return_true");
  int str_offset = context->lookup_symbol(func_name);
  EXPECT_EQ(8, str_offset);

  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(40U, csize);

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
  EXPECT_EQ(BrigEAlignment_4, Context::dir_alignment_check(bbs));

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

  EXPECT_EQ(BrigEAlignment_8, Context::dir_alignment_check(bbn));
  // this is a 8-byte aligned item and has a size of multiple of 8.
  // so the offset after appending this item should be a multiple of 8.
  EXPECT_EQ(0U, curr_offset%8);
}

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
    24                     // name
  };

  EXPECT_EQ(ref.size, getReg.size);
  EXPECT_EQ(ref.kind, getReg.kind);
  EXPECT_EQ(ref.type, getReg.type);
  EXPECT_EQ(ref.reserved, getReg.reserved);
  EXPECT_EQ(ref.s_name, getReg.s_name);

  delete lexer;
}

TEST(CodegenTest, PairAddressableOperand_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("[&array][$s1-16]\n");

  Lexer* lexer = new Lexer(input);

  BrigOperandCompound ref = {
    20,                    // size
    BrigEOperandCompound,  // kind
    Brigb64,               // type
    0,                     // reserved
    8,                     // name
    20,                    // reg
    -16                    // offset
  };

  BrigOperandReg getReg;
  BrigOperandAddress getAddr;
  BrigOperandCompound getComp;

  context->add_symbol("&array");

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, PairAddressableOperand(context));

  context->get_operand(8, &getAddr);
  context->get_operand(20, &getReg);
  context->get_operand(32, &getComp);

  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
 // EXPECT_EQ(0, getAddr.offset);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(15, getReg.s_name);
  // BrigOperandCompoud
  EXPECT_EQ(ref.size, getComp.size);
  EXPECT_EQ(ref.kind, getComp.kind);
  EXPECT_EQ(ref.type, getComp.type);
  EXPECT_EQ(ref.reserved, getComp.reserved);
  EXPECT_EQ(ref.name, getComp.name);
  EXPECT_EQ(ref.reg, getComp.reg);
  EXPECT_EQ(ref.offset, getComp.offset);

  delete lexer;
}

TEST(CodegenTest, Label_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase refCbrLab3 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigCbr,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {40, 64, 76, 0, 0}        // o_operands[5]
  };

  BrigInstBase refBrnLab1 = {
    32,                  // size
    BrigEInstBase,        // kind
    BrigBrn,             // opcode
    Brigb32,             // type
    BrigNoPacking,       // packing
    {8, 32, 0, 0, 0}     // o_operands[5]
  };
  BrigInstBase refCbrLab1 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigCbr,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {120, 64, 32, 0, 0}        // o_operands[5]
  };
  BrigInstBase refBrnLab2 = {
    32,                  // size
    BrigEInstBase,        // kind
    BrigBrn,             // opcode
    Brigb32,             // type
    BrigNoPacking,       // packing
    {88, 112, 0, 0, 0}   // o_operands[5]
  };

  BrigDirectiveLabel ref1 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    40,                     // c_code
    8                       // s_name
  };
  BrigDirectiveLabel ref2 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    72,                     // c_code
    18                      // s_name
  };
  BrigDirectiveLabel ref3 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    72,                     // c_code
    24                      // s_name
  };
  BrigDirectiveLabel ref4 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    104,                    // c_code
    30                      // s_name
  };

  BrigDirectiveLabel get1, get2, get3, get4;
  BrigOperandLabelRef getLabRef;
  BrigInstBase getCbrCode;
  BrigInstBase getBrnCode;

  std::string input("brn @lab1;\n");  // brn lab1
  input.append("@lab1:\n");
  input.append("cbr $c1, @lab3;\n");  // cbr lab3
  input.append("@lab2:");
  input.append("@lab3:");
  input.append("brn @lab2;\n");       // brn lab2
  input.append("@lab4:\n");
  input.append("cbr $c1, @lab1;\n");  // cbr lab1

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  // brn lab1
  EXPECT_EQ(0, Branch(context));
  context->get_code(8, &getBrnCode);


  EXPECT_EQ(refBrnLab1.size, getBrnCode.size);
  EXPECT_EQ(refBrnLab1.kind, getBrnCode.kind);
  EXPECT_EQ(refBrnLab1.opcode, getBrnCode.opcode);
  EXPECT_EQ(refBrnLab1.type, getBrnCode.type);
  EXPECT_EQ(refBrnLab1.packing, getBrnCode.packing);
  EXPECT_EQ(refBrnLab1.o_operands[0], getBrnCode.o_operands[0]);
  EXPECT_EQ(refBrnLab1.o_operands[1], getBrnCode.o_operands[1]);
  EXPECT_EQ(refBrnLab1.o_operands[2], getBrnCode.o_operands[2]);
  EXPECT_EQ(refBrnLab1.o_operands[3], getBrnCode.o_operands[3]);
  EXPECT_EQ(refBrnLab1.o_operands[4], getBrnCode.o_operands[4]);


  BrigoOffset32_t curOpOffset = 8;

  BrigOperandImmed getImm;

  curOpOffset += curOpOffset & 0x7;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  context->get_operand(curOpOffset, &getLabRef);

  EXPECT_EQ(sizeof(BrigOperandLabelRef), getLabRef.size);
  EXPECT_EQ(BrigEOperandLabelRef, getLabRef.kind);
  // When the label isn't declared.
  // the value of labeldirective is 0 now.
  EXPECT_EQ(0, getLabRef.labeldirective);

  // lab1
  EXPECT_EQ(0, Label(context));
  context->get_operand(curOpOffset, &getLabRef);
  EXPECT_EQ(8, getLabRef.labeldirective);
  curOpOffset += sizeof(BrigOperandLabelRef);

  // cbr lab3
  EXPECT_EQ(0, Branch(context));

  context->get_code(40, &getCbrCode);

  EXPECT_EQ(refCbrLab3.size, getCbrCode.size);
  EXPECT_EQ(refCbrLab3.kind, getCbrCode.kind);
  EXPECT_EQ(refCbrLab3.opcode, getCbrCode.opcode);
  EXPECT_EQ(refCbrLab3.type, getCbrCode.type);
  EXPECT_EQ(refCbrLab3.packing, getCbrCode.packing);
  EXPECT_EQ(refCbrLab3.o_operands[0], getCbrCode.o_operands[0]);
  EXPECT_EQ(refCbrLab3.o_operands[1], getCbrCode.o_operands[1]);
  EXPECT_EQ(refCbrLab3.o_operands[2], getCbrCode.o_operands[2]);
  EXPECT_EQ(refCbrLab3.o_operands[3], getCbrCode.o_operands[3]);
  EXPECT_EQ(refCbrLab3.o_operands[4], getCbrCode.o_operands[4]);

  curOpOffset += curOpOffset & 0x7;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  BrigOperandReg getReg;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb1, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(14, getReg.s_name);

  context->get_operand(curOpOffset, &getLabRef);
  EXPECT_EQ(sizeof(BrigOperandLabelRef), getLabRef.size);
  EXPECT_EQ(BrigEOperandLabelRef, getLabRef.kind);
  // When the label isn't declared.
  // the value of labeldirective is 0 now.
  EXPECT_EQ(0, getLabRef.labeldirective);

  // lab2
  EXPECT_EQ(0, Label(context));

  // lab3
  EXPECT_EQ(0, Label(context));
  context->get_operand(curOpOffset, &getLabRef);
  EXPECT_EQ(32, getLabRef.labeldirective);
  curOpOffset += sizeof(BrigOperandLabelRef);

  // brn lab2
  EXPECT_EQ(0, Branch(context));

  context->get_code(72, &getBrnCode);

  EXPECT_EQ(refBrnLab2.size, getBrnCode.size);
  EXPECT_EQ(refBrnLab2.kind, getBrnCode.kind);
  EXPECT_EQ(refBrnLab2.opcode, getBrnCode.opcode);
  EXPECT_EQ(refBrnLab2.type, getBrnCode.type);
  EXPECT_EQ(refBrnLab2.packing, getBrnCode.packing);
  EXPECT_EQ(refBrnLab2.o_operands[0], getBrnCode.o_operands[0]);
  EXPECT_EQ(refBrnLab2.o_operands[1], getBrnCode.o_operands[1]);
  EXPECT_EQ(refBrnLab2.o_operands[2], getBrnCode.o_operands[2]);
  EXPECT_EQ(refBrnLab2.o_operands[3], getBrnCode.o_operands[3]);
  EXPECT_EQ(refBrnLab2.o_operands[4], getBrnCode.o_operands[4]);
  // TODO(Chuang) set the value of .syncFlags
  // EXPECT_EQ(refBrn.syncFlags, getBrnCode.syncFlags);

  curOpOffset += curOpOffset & 0x7;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  context->get_operand(curOpOffset, &getLabRef);
  curOpOffset += sizeof(BrigOperandLabelRef);

  EXPECT_EQ(sizeof(BrigOperandLabelRef), getLabRef.size);
  EXPECT_EQ(BrigEOperandLabelRef, getLabRef.kind);
  EXPECT_EQ(20, getLabRef.labeldirective);

  // lab4
  EXPECT_EQ(0, Label(context));

  // cbr lab1
  EXPECT_EQ(0, Branch(context));

  context->get_code(104, &getCbrCode);

  EXPECT_EQ(refCbrLab1.size, getCbrCode.size);
  EXPECT_EQ(refCbrLab1.kind, getCbrCode.kind);
  EXPECT_EQ(refCbrLab1.opcode, getCbrCode.opcode);
  EXPECT_EQ(refCbrLab1.type, getCbrCode.type);
  EXPECT_EQ(refCbrLab1.packing, getCbrCode.packing);
  EXPECT_EQ(refCbrLab1.o_operands[0], getCbrCode.o_operands[0]);
  EXPECT_EQ(refCbrLab1.o_operands[1], getCbrCode.o_operands[1]);
  EXPECT_EQ(refCbrLab1.o_operands[2], getCbrCode.o_operands[2]);
  EXPECT_EQ(refCbrLab1.o_operands[3], getCbrCode.o_operands[3]);
  EXPECT_EQ(refCbrLab1.o_operands[4], getCbrCode.o_operands[4]);

  curOpOffset += curOpOffset & 0x7;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);


  EXPECT_EQ(curOpOffset, context->get_operand_offset());

  context->get_directive(8, &get1);

  // lab1
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.c_code, get1.c_code);
  EXPECT_EQ(ref1.s_name, get1.s_name);

  context->get_directive(20, &get2);

  // lab2
  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.c_code, get2.c_code);
  EXPECT_EQ(ref2.s_name, get2.s_name);

  context->get_directive(32, &get3);

  // lab3
  EXPECT_EQ(ref3.size, get3.size);
  EXPECT_EQ(ref3.kind, get3.kind);
  EXPECT_EQ(ref3.c_code, get3.c_code);
  EXPECT_EQ(ref3.s_name, get3.s_name);

  context->get_directive(44, &get4);

  // lab4
  EXPECT_EQ(ref4.size, get4.size);
  EXPECT_EQ(ref4.kind, get4.kind);
  EXPECT_EQ(ref4.c_code, get4.c_code);
  EXPECT_EQ(ref4.s_name, get4.s_name);

  delete lexer;
}

TEST(CodegenTest, ArrayOperand_CodeGen_Test) {
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
  BrigoOffset32_t getoOffset;
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandPart2(context, &getoOffset));
  context->get_operand(getoOffset, &getRegV4);
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
  EXPECT_EQ(0, ArrayOperandPart2(context, &getoOffset));
  context->get_operand(getoOffset, &getRegV2);
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
  EXPECT_EQ(0, ArrayOperandPart2(context, &getoOffset));
  context->get_operand(getoOffset, &getReg);

  BrigOperandReg refRegD1 = {
    12,                    // size
    BrigEOperandReg,       // kind
    Brigb64,               // type
    0,                     // reserved
    24                     // name
  };

  EXPECT_EQ(refRegD1.size, getReg.size);
  EXPECT_EQ(refRegD1.kind, getReg.kind);
  EXPECT_EQ(refRegD1.type, getReg.type);
  EXPECT_EQ(refRegD1.reserved, getReg.reserved);
  EXPECT_EQ(refRegD1.s_name, getReg.s_name);

  EXPECT_EQ(108, context->get_operand_offset());

  input.assign("($s3)\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandPart2(context, &getoOffset));
  context->get_operand(getoOffset, &getReg);

  BrigOperandReg refRegS3 = {
    12,                    // size
    BrigEOperandReg,       // kind
    Brigb32,               // type
    0,                     // reserved
    16                      // name
  };

  EXPECT_EQ(refRegS3.size, getReg.size);
  EXPECT_EQ(refRegS3.kind, getReg.kind);
  EXPECT_EQ(refRegS3.type, getReg.type);
  EXPECT_EQ(refRegS3.reserved, getReg.reserved);
  EXPECT_EQ(refRegS3.s_name, getReg.s_name);

  input.assign("$s2\n");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArrayOperandPart2(context, &getoOffset));
  context->get_operand(getoOffset, &getReg);

  BrigOperandReg refRegS2 = {
    12,                    // size
    BrigEOperandReg,       // kind
    Brigb32,               // type
    0,                     // reserved
    12                      // name
  };

  EXPECT_EQ(refRegS2.size, getReg.size);
  EXPECT_EQ(refRegS2.kind, getReg.kind);
  EXPECT_EQ(refRegS2.type, getReg.type);
  EXPECT_EQ(refRegS2.reserved, getReg.reserved);
  EXPECT_EQ(refRegS2.s_name, getReg.s_name);

  EXPECT_EQ(108, context->get_operand_offset());

  delete lexer;
}

TEST(CodegenTest, ArrayDimensionSetCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  // case for decimal
  std::string input("[9][9] ");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,ArrayDimensionSet(context));

  EXPECT_EQ(81, context->get_dim());
  EXPECT_EQ(BrigArray, context->get_symbol_modifier());


  input.assign("global_u16 &x[] ;");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,InitializableDecl(context));
  EXPECT_EQ(BrigFlex,context->get_symbol_modifier());
  EXPECT_EQ(0, context->get_dim());

  input.assign("global_u8 &y[] = {1,2,3,4,5,6,7,8,9};");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,InitializableDecl(context));
  EXPECT_EQ(BrigFlex | BrigArray,context->get_symbol_modifier());
  EXPECT_EQ(9, context->get_dim());

  delete lexer;
}

TEST(CodegenTest, ArgumentDeclCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("align 8 arg_u8 %last[] ;\n");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ArgumentDecl(context));

  BrigDirectiveSymbol ref = {
        sizeof(BrigDirectiveSymbol),                 // size
        BrigEDirectiveSymbol,             // kind
        {
          8,       // c_code
          BrigArgSpace,                    // storageClass
          BrigNone,         // attribute
          0,                                // reserved
          BrigFlex,   // symbol modifier
          0,               // dim
          8,                  // s_name
          Brigu8,              // data type
          8,         // alignment
        },
        0,                                // d_init = 0 for arg
        0                                 // reserved
        };

	BrigDirectiveSymbol get;
	context->get_directive(8, &get);
	EXPECT_EQ(ref.size, get.size);
	EXPECT_EQ(ref.kind, get.kind);
	EXPECT_EQ(ref.s.c_code, get.s.c_code);
	EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
	EXPECT_EQ(ref.s.attribute, get.s.attribute);
	EXPECT_EQ(ref.s.reserved, get.s.reserved);
	EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
	EXPECT_EQ(ref.s.dim, get.s.dim);
	EXPECT_EQ(ref.s.s_name, get.s.s_name);
	EXPECT_EQ(ref.s.type, get.s.type);
	EXPECT_EQ(ref.s.align, get.s.align);
	EXPECT_EQ(ref.d_init, get.d_init);
	EXPECT_EQ(ref.reserved, get.reserved);

	delete lexer;
}

TEST(CodegenTest, ImageStore_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstImage ref2da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {56, 80, 108, 0, 0},   // o_operands[5]
    Briggeom_2da          // geom
  };

  BrigInstImage ref1da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {132, 156, 172, 0, 0},   // o_operands[5]
    Briggeom_1da          // geom
  };

  BrigInstImage ref1db = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {188, 212, 44, 0, 0},   // o_operands[5]
    Briggeom_1db          // geom
  };

  BrigInstImage get;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage;

  std::string input("st_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s2,$s3,$s4,$s5);\n");
  input.append("st_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5);\n");
  input.append("st_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4);\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  context->symbol_map["%RWImg3"] = 30;

  EXPECT_EQ(0, ImageStore(context));
  EXPECT_EQ(0, ImageStore(context));
  EXPECT_EQ(0, ImageStore(context));

  context->get_code(8, &get);

  EXPECT_EQ(ref2da.size, get.size);
  EXPECT_EQ(ref2da.kind, get.kind);
  EXPECT_EQ(ref2da.opcode, get.opcode);
  EXPECT_EQ(ref2da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref2da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref2da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref2da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref2da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref2da.geom, get.geom);
  EXPECT_EQ(ref2da.type, get.type);
  EXPECT_EQ(ref2da.stype, get.stype);
  EXPECT_EQ(ref2da.packing, get.packing);
  EXPECT_EQ(ref2da.reserved, get.reserved);

  context->get_code(48, &get);

  EXPECT_EQ(ref1da.size, get.size);
  EXPECT_EQ(ref1da.kind, get.kind);
  EXPECT_EQ(ref1da.opcode, get.opcode);
  EXPECT_EQ(ref1da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1da.geom, get.geom);
  EXPECT_EQ(ref1da.type, get.type);
  EXPECT_EQ(ref1da.stype, get.stype);
  EXPECT_EQ(ref1da.packing, get.packing);
  EXPECT_EQ(ref1da.reserved, get.reserved);

  context->get_code(88, &get);

  EXPECT_EQ(ref1db.size, get.size);
  EXPECT_EQ(ref1db.kind, get.kind);
  EXPECT_EQ(ref1db.opcode, get.opcode);
  EXPECT_EQ(ref1db.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1db.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1db.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1db.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1db.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1db.geom, get.geom);
  EXPECT_EQ(ref1db.type, get.type);
  EXPECT_EQ(ref1db.stype, get.stype);
  EXPECT_EQ(ref1db.packing, get.packing);
  EXPECT_EQ(ref1db.reserved, get.reserved);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.s_name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.s_name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.s_name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.s_name);

  context->get_operand(96, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.s_name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(156, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(212, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);


  context->get_operand(172, &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(16, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(44, getRegV2.regs[0]);
  EXPECT_EQ(96, getRegV2.regs[1]);

  context->get_operand(56, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(108, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(20, getRegV4.regs[0]);
  EXPECT_EQ(32, getRegV4.regs[1]);
  EXPECT_EQ(44, getRegV4.regs[2]);
  EXPECT_EQ(96, getRegV4.regs[3]);

  context->get_operand(132, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(188, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);


  delete lexer;
}

TEST(CodegenTest, ImageLoad_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstImage ref2da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {56, 80, 108, 0, 0},   // o_operands[5]
    Briggeom_2da          // geom
  };

  BrigInstImage ref1da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {132, 156, 172, 0, 0},   // o_operands[5]
    Briggeom_1da          // geom
  };

  BrigInstImage ref1db = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {188, 212, 44, 0, 0},   // o_operands[5]
    Briggeom_1db          // geom
  };

  BrigInstImage get;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage;

  std::string input("ld_image_v4_2da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5,$s2,$s3);\n");
  input.append("ld_image_v4_1da_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4,$s5);\n");
  input.append("ld_image_v4_1db_f32_u32 ($s1,$s2,$s3,$s4), [%RWImg3], ($s4);\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  context->symbol_map["%RWImg3"] = 30;

  EXPECT_EQ(0, ImageLoad(context));
  EXPECT_EQ(0, ImageLoad(context));
  EXPECT_EQ(0, ImageLoad(context));

  context->get_code(8, &get);

  EXPECT_EQ(ref2da.size, get.size);
  EXPECT_EQ(ref2da.kind, get.kind);
  EXPECT_EQ(ref2da.opcode, get.opcode);
  EXPECT_EQ(ref2da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref2da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref2da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref2da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref2da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref2da.geom, get.geom);
  EXPECT_EQ(ref2da.type, get.type);
  EXPECT_EQ(ref2da.stype, get.stype);
  EXPECT_EQ(ref2da.packing, get.packing);
  EXPECT_EQ(ref2da.reserved, get.reserved);

  context->get_code(48, &get);

  EXPECT_EQ(ref1da.size, get.size);
  EXPECT_EQ(ref1da.kind, get.kind);
  EXPECT_EQ(ref1da.opcode, get.opcode);
  EXPECT_EQ(ref1da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1da.geom, get.geom);
  EXPECT_EQ(ref1da.type, get.type);
  EXPECT_EQ(ref1da.stype, get.stype);
  EXPECT_EQ(ref1da.packing, get.packing);
  EXPECT_EQ(ref1da.reserved, get.reserved);

  context->get_code(88, &get);

  EXPECT_EQ(ref1db.size, get.size);
  EXPECT_EQ(ref1db.kind, get.kind);
  EXPECT_EQ(ref1db.opcode, get.opcode);
  EXPECT_EQ(ref1db.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1db.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1db.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1db.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1db.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1db.geom, get.geom);
  EXPECT_EQ(ref1db.type, get.type);
  EXPECT_EQ(ref1db.stype, get.stype);
  EXPECT_EQ(ref1db.packing, get.packing);
  EXPECT_EQ(ref1db.reserved, get.reserved);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.s_name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.s_name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.s_name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.s_name);

  context->get_operand(96, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.s_name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(156, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(212, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);


  context->get_operand(172, &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(16, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(44, getRegV2.regs[0]);
  EXPECT_EQ(96, getRegV2.regs[1]);

  context->get_operand(56, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(108, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(44, getRegV4.regs[0]);
  EXPECT_EQ(96, getRegV4.regs[1]);
  EXPECT_EQ(20, getRegV4.regs[2]);
  EXPECT_EQ(32, getRegV4.regs[3]);

  context->get_operand(132, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(188, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);


  delete lexer;
}

TEST(CodegenTest, ImageRead_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstRead ref1d = {
    sizeof(BrigInstRead),                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    Brigs32,               // type
    Brigf32,               // stype
    BrigNoPacking,         // packing
    {56, 80, 96, 112, 0},   // o_operands[5]
    Briggeom_1d,           // geom
    0                      // reserved
  };

  BrigInstRead ref1da = {
    sizeof(BrigInstRead),                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    Brigs32,               // type
    Brigf32,               // stype
    BrigNoPacking,         // packing
    {136, 160, 176, 192, 0}, // o_operands[5]
    Briggeom_1da,          // geom
    0                      // reserved
  };

  BrigInstRead ref2da = {
    sizeof(BrigInstRead),                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    Brigs32,               // type
    Brigf32,               // stype
    BrigNoPacking,         // packing
    {220, 244, 260, 276, 0},  // o_operands[5]
    Briggeom_2da,          // geom
    0                      // reserved
  };

  BrigInstRead get;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage;

  std::string input("rd_image_v4_1d_s32_f32 ($s0,$s1,$s5,$s3), ");
  input.append("[%RWImg3], [%Samp3], ($s6);\n");
  input.append("rd_image_v4_1da_s32_f32 ($s0,$s1,$s2,$s3), [%RWImg3],");
  input.append("[%Samp3],($s6, $s5);\n");
  input.append("rd_image_v4_2da_s32_f32 ($s0,$s1,$s3,$s4), [%RWImg3],");
  input.append("[%Samp3],($s5, $s6, $s4, $s3);\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  context->symbol_map["%RWImg3"] = 0xf7;
  context->symbol_map["%Samp3"] = 0xf1;

  EXPECT_EQ(0, ImageRead(context));
  EXPECT_EQ(0, ImageRead(context));
  EXPECT_EQ(0, ImageRead(context));

  context->get_code(8, &get);

  EXPECT_EQ(ref1d.size, get.size);
  EXPECT_EQ(ref1d.kind, get.kind);
  EXPECT_EQ(ref1d.opcode, get.opcode);
  EXPECT_EQ(ref1d.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1d.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1d.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1d.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1d.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1d.geom, get.geom);
  EXPECT_EQ(ref1d.type, get.type);
  EXPECT_EQ(ref1d.stype, get.stype);
  EXPECT_EQ(ref1d.packing, get.packing);
  EXPECT_EQ(ref1d.reserved, get.reserved);
  context->get_code(8 + sizeof(BrigInstRead), &get);

  EXPECT_EQ(ref1da.size, get.size);
  EXPECT_EQ(ref1da.kind, get.kind);
  EXPECT_EQ(ref1da.opcode, get.opcode);
  EXPECT_EQ(ref1da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref1da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref1da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref1da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref1da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref1da.geom, get.geom);
  EXPECT_EQ(ref1da.type, get.type);
  EXPECT_EQ(ref1da.stype, get.stype);
  EXPECT_EQ(ref1da.packing, get.packing);
  EXPECT_EQ(ref1da.reserved, get.reserved);

  context->get_code(8 + sizeof(BrigInstRead) * 2, &get);

  EXPECT_EQ(ref2da.size, get.size);
  EXPECT_EQ(ref2da.kind, get.kind);
  EXPECT_EQ(ref2da.opcode, get.opcode);
  EXPECT_EQ(ref2da.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref2da.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref2da.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref2da.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref2da.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref2da.geom, get.geom);
  EXPECT_EQ(ref2da.type, get.type);
  EXPECT_EQ(ref2da.stype, get.stype);
  EXPECT_EQ(ref2da.packing, get.packing);
  EXPECT_EQ(ref2da.reserved, get.reserved);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.s_name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.s_name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.s_name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.s_name);

  context->get_operand(112, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.s_name);

  context->get_operand(124, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(28, getReg.s_name);

  context->get_operand(208, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(32, getReg.s_name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(96, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(160, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(176, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(244, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(260, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.directive);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(192, &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(16, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(112, getRegV2.regs[0]);
  EXPECT_EQ(32, getRegV2.regs[1]);

  context->get_operand(56, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(32, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(136, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(124, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);

  context->get_operand(220, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(8, getRegV4.regs[0]);
  EXPECT_EQ(20, getRegV4.regs[1]);
  EXPECT_EQ(44, getRegV4.regs[2]);
  EXPECT_EQ(208, getRegV4.regs[3]);

  context->get_operand(276, &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(24, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(Brigb32, getRegV4.type);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(32, getRegV4.regs[0]);
  EXPECT_EQ(112, getRegV4.regs[1]);
  EXPECT_EQ(208, getRegV4.regs[2]);
  EXPECT_EQ(44, getRegV4.regs[3]);


  delete lexer;
}

TEST(CodegenTest,ControlCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("memopt_on;");

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,Control(context));

  BrigDirectiveControl ref = {
    24,
    BrigEDirectiveControl,
    8,
    BrigEMemOpt,
    {1,0,0}
  };
  BrigDirectiveControl get;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.controlType,get.controlType);
  EXPECT_EQ(ref.values[0],get.values[0]);
  EXPECT_EQ(ref.values[1],get.values[1]);
  EXPECT_EQ(ref.values[2],get.values[2]);

  input.assign("workgroupspercu 6;");
  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,Control(context));

  BrigDirectiveControl ref1 = {
    24,
    BrigEDirectiveControl,
    8,
    BrigEMaxGperC,
    {6,0,0}
  };
  context->get_directive(8,&get);

  EXPECT_EQ(ref1.size,get.size);
  EXPECT_EQ(ref1.kind,get.kind);
  EXPECT_EQ(ref1.c_code,get.c_code);
  EXPECT_EQ(ref1.controlType,get.controlType);
  EXPECT_EQ(ref1.values[0],get.values[0]);
  EXPECT_EQ(ref1.values[1],get.values[1]);
  EXPECT_EQ(ref1.values[2],get.values[2]);

  input.assign("itemsperworkgroup 2,3,4;");
  context->clear_context();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,Control(context));

  BrigDirectiveControl ref2 = {
    24,
    BrigEDirectiveControl,
    8,
    BrigEMaxWIperG,
    {2,3,4}
  };
  context->get_directive(8,&get);

  EXPECT_EQ(ref2.size,get.size);
  EXPECT_EQ(ref2.kind,get.kind);
  EXPECT_EQ(ref2.c_code,get.c_code);
  EXPECT_EQ(ref2.controlType,get.controlType);
  EXPECT_EQ(ref2.values[0],get.values[0]);
  EXPECT_EQ(ref2.values[1],get.values[1]);
  EXPECT_EQ(ref2.values[2],get.values[2]);

  delete lexer;
}

TEST(CodegenTest,BlockCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("block \"debug\"");
  // input.append("blocknumeric_b8 255, 23, 10, 23;");
  input.append("blocknumeric_b32 1255, 0x323, 10, 23;");
  input.append("blocknumeric_b64 0x12345678, 0x323, 10, 23;");

  input.append("blockstring \"this is a string\";");
  input.append("endblock;");

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,Block(context));

  // block end
  BrigBlockEnd bbe = {
    sizeof(BrigBlockEnd),
    BrigEDirectiveBlockEnd
  };
  BrigBlockEnd get_bbe;
  BrigdOffset32_t bbe_d_offset = context->get_directive_offset() - sizeof(BrigBlockEnd);
  context->get_directive(bbe_d_offset, &get_bbe);

  EXPECT_EQ(bbe.size,get_bbe.size);
  EXPECT_EQ(bbe.kind,get_bbe.kind);

  // block string
  uint32_t bbs_len = strlen("\"this is a string\"") + 1 ;
  BrigsOffset32_t bbs_s_offset = context->get_string_offset() - bbs_len;
  BrigBlockString bbs = {
    sizeof(BrigBlockString),
    BrigEDirectiveBlockString,
    bbs_s_offset
  };
  BrigBlockString get_bbs;
  BrigdOffset32_t bbs_d_offset = bbe_d_offset - sizeof(BrigBlockString);
  context->get_directive(bbs_d_offset, &get_bbs);

  EXPECT_EQ(bbs.size,get_bbs.size);
  EXPECT_EQ(bbs.kind,get_bbs.kind);
  EXPECT_EQ(bbs.s_name,get_bbs.s_name);
/*
  // blocknumeric
  size_t arraySize = sizeof(BrigBlockNumeric) + 3 * sizeof(uint64_t);
  BrigdOffset32_t bbn1_d_offset = bbs_d_offset - arraySize ;

  uint8_t *array = new uint8_t[arraySize];
  BrigBlockNumeric *bbn1 =
        reinterpret_cast<BrigBlockNumeric*>(array);
  bbn1->size = arraySize;
  bbn1->kind = BrigEDirectiveBlockNumeric;
  bbn1->type = Brigb64;
  bbn1->elementCount = 4;
  bbn1->u64[0] = 0x12345678;
  bbn1->u64[1] = 0x323;
  bbn1->u64[2] = 10;
  bbn1->u64[3] = 23;

  array = new uint8_t[arraySize];
  BrigBlockNumeric *get = reinterpret_cast<BrigBlockNumeric*>(array);
  char *get_charp = reinterpret_cast<char *>(get);
  context->get_directive_bytes(get_charp,bbn1_d_offset,arraySize);

  EXPECT_EQ(bbn1->size,get->size);
  EXPECT_EQ(bbn1->kind,get->kind);
  EXPECT_EQ(bbn1->type,get->type);
  EXPECT_EQ(bbn1->elementCount,get->elementCount);
  EXPECT_EQ(bbn1->u64[0],get->u64[0]);
  EXPECT_EQ(bbn1->u64[1],get->u64[1]);
  EXPECT_EQ(bbn1->u64[2],get->u64[2]);
  EXPECT_EQ(bbn1->u64[3],get->u64[3]);
  delete[] reinterpret_cast<char *>(bbn1);
  delete[] reinterpret_cast<char *>(get);

 // blocknumeric
  arraySize = sizeof(BrigBlockNumeric) + sizeof(uint64_t);
  BrigdOffset32_t bbn2_d_offset = bbn1_d_offset - arraySize;
  array = new uint8_t[arraySize];
  BrigBlockNumeric *bbn2 =
        reinterpret_cast<BrigBlockNumeric*>(array);
  bbn2->size = arraySize;
  bbn2->kind =  BrigEDirectiveBlockNumeric;
  bbn2->type = Brigb32;
  bbn2->elementCount = 4;
  bbn2->u32[0] = 1255;
  bbn2->u32[1] = 0x323;
  bbn2->u32[2] = 10;
  bbn2->u32[3] = 23;

  array = new uint8_t[arraySize];
  get = reinterpret_cast<BrigBlockNumeric*>(array);
  get_charp = reinterpret_cast<char *>(get);
  context->get_directive_bytes(get_charp,bbn2_d_offset,arraySize);

  EXPECT_EQ(bbn2->size,get->size);
  EXPECT_EQ(bbn2->kind,get->kind);
  EXPECT_EQ(bbn2->type,get->type);
  EXPECT_EQ(bbn2->elementCount,get->elementCount);
  EXPECT_EQ(bbn2->u32[0],get->u32[0]);
  EXPECT_EQ(bbn2->u32[1],get->u32[1]);
  EXPECT_EQ(bbn2->u32[2],get->u32[2]);
  EXPECT_EQ(bbn2->u32[3],get->u32[3]);
  delete[] reinterpret_cast<char *>(bbn2);
  delete[] reinterpret_cast<char *>(get);

  // blockstart
  uint32_t str2_len = strlen("\"debug\"") + 1;
  BrigBlockStart start = {
    sizeof(BrigBlockStart),
    BrigEDirectiveBlockStart,
    context->get_code_offset(),
    bbs_s_offset - str2_len
  };
  BrigdOffset32_t bbs2_d_offset = bbn2_d_offset - sizeof(BrigBlockStart)
             - sizeof(BrigDirectivePad);
  BrigBlockStart get_start;
  context->get_directive(bbs2_d_offset,&get_start);

  EXPECT_EQ(start.size,get_start.size);
  EXPECT_EQ(start.kind,get_start.kind);
  EXPECT_EQ(start.c_code,get_start.c_code);
  EXPECT_EQ(start.s_name,get_start.s_name);
*/
  delete lexer;
}

TEST(CodegenTest,FunctionSignatureCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("signature &test(arg_u32)(arg_f32) ;");
  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,FunctionSignature(context));

  size_t arraySize = sizeof(BrigDirectiveSignature) +
      sizeof(BrigDirectiveSignature::BrigProtoType);
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveSignature *ref =
        reinterpret_cast<BrigDirectiveSignature *>(array);

  ref->size = arraySize;
  ref->kind = BrigEDirectiveSignature;
  ref->c_code = context->get_code_offset();
  ref->s_name = context->get_string_offset()-(strlen("&test") + 1);
  ref->outCount = 1;
  ref->inCount = 1;
  ref->types[0].type = Brigu32;
  ref->types[0].align = 1;
  ref->types[0].hasDim = 0;
  ref->types[0].dim = 0;

  ref->types[1].type = Brigf32;
  ref->types[1].align = 1;
  ref->types[1].hasDim = 0;
  ref->types[1].dim = 0;

// Buffer *dbuf = context->get_directive();
  uint32_t offset = context->get_directive_offset() - arraySize;
//  BrigDirectiveSignature *get =
  //     reinterpret_cast<BrigDirectiveSignature*>(&dbuf->get()[offset]);

  array = new uint8_t[arraySize];
  BrigDirectiveSignature *get =
      reinterpret_cast<BrigDirectiveSignature*>(array);
  char *get_charp =  reinterpret_cast<char *>(get);

  context->get_directive_bytes(get_charp,offset,arraySize);
  EXPECT_EQ(ref->size,get->size);
  EXPECT_EQ(ref->kind,get->kind);
  EXPECT_EQ(ref->c_code,get->c_code);
  EXPECT_EQ(ref->s_name,get->s_name);
  EXPECT_EQ(ref->outCount,get->outCount);
  EXPECT_EQ(ref->inCount,get->inCount);
  EXPECT_EQ(ref->types[0].type,get->types[0].type);
  EXPECT_EQ(ref->types[0].align,get->types[0].align);
  EXPECT_EQ(ref->types[0].hasDim,get->types[0].hasDim);
  EXPECT_EQ(ref->types[0].dim,get->types[0].dim);
  EXPECT_EQ(ref->types[1].type,get->types[1].type);
  EXPECT_EQ(ref->types[1].align,get->types[1].align);
  EXPECT_EQ(ref->types[1].hasDim,get->types[1].hasDim);
  EXPECT_EQ(ref->types[1].dim,get->types[1].dim);

  delete[] reinterpret_cast<char *>(ref);
  delete[] reinterpret_cast<char *>(get);

  delete lexer;
}

TEST(CodegenTest, Kernel_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("kernel &_kernel( \n");
  input.append("kernarg_u32 %arg0, \n");
  input.append("kernarg_u32 %arg1) \n");
  input.append("{ \n");
  input.append("@begin: \n");
  input.append("  private_s32 %arg2;\n");
  input.append("  workitemaid $s0, 0; \n");
  input.append("  ld_kernarg_u32 $s2, [%arg0]; \n");
  input.append("  mad_u32 $s5, $s4, $s2, $s3; \n");
  input.append("  ret; \n");
  input.append("}; \n");

  Lexer* lexer = new Lexer(input);
  BrigdOffset32_t curDirOffset = 0;

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Kernel(context));
  EXPECT_EQ(180, context->get_directive_offset());
  EXPECT_EQ(132, context->get_operand_offset());
  EXPECT_EQ(62, context->get_string_offset());
  EXPECT_EQ(148, context->get_code_offset());


  BrigDirectiveKernel ref = {
    sizeof(BrigDirectiveKernel),    // size
    BrigEDirectiveKernel,           // kind
    8,                              // c_code
    8,                              // s_name
    2,                              // inParamCount
    128,                             // d_firstScopedDirective
    4,                              // operationCount
    180,                             // d_nextDirective
    BrigNone,                       // attribute
    2,                              // fbar
    0,                              // outParamCount
    48,                             // d_firstInParam
  };

  BrigDirectiveKernel get;
  curDirOffset = context->current_bdf_offset;
  context->get_directive(curDirOffset, &get);
  curDirOffset += sizeof(BrigDirectiveKernel);

  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  BrigDirectiveSymbol kernarg0 = {
    sizeof(BrigDirectiveSymbol),   // size
    BrigEDirectiveSymbol ,        // kind
    {
      0,                         // c_code
      BrigKernargSpace,          // storag class kernarg
      BrigNone ,                 // attribut
      0,                         // reserved
      0,                         // symbolModifier
      0,                         // dim
      17,                         // s_name
      Brigu32,                   // type
      1,                         // align
    },
    0,                          // d_init
    0                          // reserved
  };

  BrigDirectiveSymbol getArg;
  context->get_directive(curDirOffset, &getArg);
  curDirOffset += sizeof(BrigDirectiveSymbol);

  EXPECT_EQ(kernarg0.size, getArg.size);
  EXPECT_EQ(kernarg0.kind, getArg.kind);
  EXPECT_EQ(kernarg0.s.storageClass, getArg.s.storageClass);
  EXPECT_EQ(kernarg0.s.s_name, getArg.s.s_name);

  BrigDirectiveSymbol kernarg1 = {
    sizeof(BrigDirectiveSymbol),  // size
    BrigEDirectiveSymbol ,    // kind
    {
      8,                         // c_code
      BrigKernargSpace,         // storag class kernarg
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      23,                        // s_name
      Brigu32,                  // type
      1,                        // align
    },
    0,                        // d_init
    0                         // reserved
  };

  context->get_directive(curDirOffset, &getArg);
  curDirOffset += sizeof(BrigDirectiveSymbol);
  EXPECT_EQ(kernarg1.size, getArg.size);
  EXPECT_EQ(kernarg1.kind, getArg.kind);
  EXPECT_EQ(kernarg1.s.storageClass, getArg.s.storageClass);
  EXPECT_EQ(kernarg1.s.s_name, getArg.s.s_name);

  BrigDirectiveLabel labRef = {
    sizeof(BrigDirectiveLabel),
    BrigEDirectiveLabel,
    8,
    29
  };
  BrigDirectiveLabel getLab;

  context->get_directive(curDirOffset, &getLab);
  curDirOffset += sizeof(BrigDirectiveLabel);
  EXPECT_EQ(labRef.size, getLab.size);
  EXPECT_EQ(labRef.kind, getLab.kind);
  EXPECT_EQ(labRef.c_code, getLab.c_code);
  EXPECT_EQ(labRef.s_name, getLab.s_name);

  BrigDirectiveSymbol arg2 = {
    sizeof(BrigDirectiveSymbol),  // size
    BrigEDirectiveSymbol ,    // kind
    {
      0,                         // c_code
      BrigPrivateSpace,         // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      36,                       // s_name
      Brigs32,                  // type
      1,                        // align
    },
    0,                          // d_init
    0                           // reserved
  };

  context->get_directive(curDirOffset, &getArg);
  curDirOffset += sizeof(BrigDirectiveSymbol);
  EXPECT_EQ(arg2.size, getArg.size);
  EXPECT_EQ(arg2.kind, getArg.kind);
  EXPECT_EQ(arg2.s.storageClass, getArg.s.storageClass);
  EXPECT_EQ(arg2.s.s_name, getArg.s.s_name);


  delete lexer;
}

TEST(CodegenTest, Comment_Test){
	context->set_error_reporter(main_reporter);
	context->clear_context();

	std::string input("nop;\n /*This is a comment*/\n nop;");
	Lexer* lexer = new Lexer(input);
	lexer->set_source_string(input);
	context->token_to_scan = lexer->get_next_token();
	EXPECT_EQ(0, Instruction0(context));
	EXPECT_EQ(0, Comment(context));
	EXPECT_EQ(0, Instruction0(context));

	BrigDirectiveComment get;
	context->get_directive(8, &get);

	BrigDirectiveComment ref = {
	sizeof(BrigDirectiveComment),
	BrigEDirectiveComment,
	8 + sizeof(BrigInstBase),
	8
	};

	EXPECT_EQ(ref.size, get.size);
	EXPECT_EQ(ref.kind, get.kind);
	EXPECT_EQ(ref.c_code, get.c_code);
	EXPECT_EQ(ref.s_name, get.s_name);

	context->clear_context();
	input.assign("nop;\n //This is a comment\n nop;");
	lexer->set_source_string(input);
	context->token_to_scan = lexer->get_next_token();
	EXPECT_EQ(0, Instruction0(context));
	EXPECT_EQ(0, Comment(context));
	EXPECT_EQ(0, Instruction0(context));

	context->get_directive(8, &get);

	BrigDirectiveComment ref2 = {
	sizeof(BrigDirectiveComment),
	BrigEDirectiveComment,
	8 + sizeof(BrigInstBase),
	8
	};

	EXPECT_EQ(ref2.size, get.size);
	EXPECT_EQ(ref2.kind, get.kind);
	EXPECT_EQ(ref2.c_code, get.c_code);
	EXPECT_EQ(ref2.s_name, get.s_name);

    delete lexer;
}

TEST(CodegenTest, Example6_CodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("version 1:0:$small;\n");
  input.append("function &callee(arg_f32 %output)(arg_f32 %input)\n");
  input.append("{\n");
  input.append("  ld_arg_f32 $s0, [%input];\n");
  input.append("  st_arg_f32 $s0, [%output];\n");
  input.append("  ret;\n");
  input.append("};\n");
  input.append("function &caller()()\n");
  input.append("{\n");
  input.append("  {\n");
  input.append("    arg_f32 %an_input;\n");
  input.append("    st_arg_f32 $s1, [%an_input];\n");
  input.append("    arg_f32 %an_output;\n");
  input.append("    call &callee (%an_output)(%an_input);\n");
  input.append("    ld_arg_f32 $s0, [%an_output];\n");
  input.append("  }\n");
  input.append("};\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  BrigcOffset32_t curCodOffset = context->get_code_offset();
  BrigoOffset32_t curOpeOffset = context->get_operand_offset();
  BrigdOffset32_t curDirOffset = context->get_directive_offset();


  EXPECT_EQ(0, Program(context));

  BrigDirectiveVersion verRef = {
    sizeof(BrigDirectiveVersion),
    BrigEDirectiveVersion,
    curCodOffset,
    1,                    //  major
    0,                    //  minor
    BrigESmall,
    BrigEFull,
    BrigENosftz,
    0
  };
  BrigDirectiveVersion verGet;
  context->get_directive(curDirOffset, &verGet);
  curDirOffset += sizeof(BrigDirectiveVersion);

  EXPECT_EQ(verRef.size, verGet.size);
  EXPECT_EQ(verRef.kind, verGet.kind);
  EXPECT_EQ(verRef.c_code, verGet.c_code);
  EXPECT_EQ(verRef.major, verGet.major);
  EXPECT_EQ(verRef.minor, verGet.minor);
  EXPECT_EQ(verRef.machine, verGet.machine);
  EXPECT_EQ(verRef.profile, verGet.profile);
  EXPECT_EQ(verRef.ftz, verGet.ftz);
  EXPECT_EQ(verRef.reserved, verGet.reserved);

  BrigDirectiveFunction calleeFunRef = {
    sizeof(BrigDirectiveFunction),               // size
    BrigEDirectiveFunction,                      // kind
    8,                                           // c_code
    8,                                           // s_name
    1,                                           // inParamCount
    148,                                         // d_firstScopedDirective
    3,                                           // operationCount
    148,                                         // d_nextDirective
    BrigNone,                                    // attribute
    0,                                           // fbarCount
    1,                                           // outParamCount
    108                                          // d_firstInParam
  };

  BrigDirectiveFunction funGet;
  context->get_directive(curDirOffset, &funGet);
  curDirOffset += sizeof(BrigDirectiveFunction);

  EXPECT_EQ(calleeFunRef.size, funGet.size);
  EXPECT_EQ(calleeFunRef.kind, funGet.kind);
  EXPECT_EQ(calleeFunRef.s_name, funGet.s_name);
  EXPECT_EQ(calleeFunRef.c_code, funGet.c_code);
  EXPECT_EQ(calleeFunRef.outParamCount, funGet.outParamCount);
  EXPECT_EQ(calleeFunRef.inParamCount, funGet.inParamCount);
  EXPECT_EQ(calleeFunRef.operationCount, funGet.operationCount);
  EXPECT_EQ(calleeFunRef.d_nextDirective, funGet.d_nextDirective);
  EXPECT_EQ(calleeFunRef.d_firstScopedDirective, funGet.d_firstScopedDirective);
  EXPECT_EQ(calleeFunRef.d_firstInParam, funGet.d_firstInParam);
  EXPECT_EQ(calleeFunRef.reserved, funGet.reserved);
  EXPECT_EQ(calleeFunRef.attribute, funGet.attribute);

  BrigDirectiveSymbol outputSymbol = {
  sizeof(BrigDirectiveSymbol),   // size
  BrigEDirectiveSymbol ,         // kind
  {
    curCodOffset,                // c_code
    BrigArgSpace,                // storag class kernarg
    BrigNone ,                   // attribut
    0,                           // reserved
    0,                           // symbolModifier
    0,                           // dim
    16,                          // s_name
    Brigf32,                     // type
    1                            // align
  },
  0,                             // d_init
  0,                             // reserved
  };

  BrigDirectiveSymbol symGet;

  context->get_directive(curDirOffset, &symGet);
  curDirOffset += sizeof(BrigDirectiveSymbol);

  EXPECT_EQ(outputSymbol.size, symGet.size);
  EXPECT_EQ(outputSymbol.kind, symGet.kind);
  EXPECT_EQ(outputSymbol.s.storageClass, symGet.s.storageClass);
  EXPECT_EQ(outputSymbol.s.s_name, symGet.s.s_name);
  EXPECT_EQ(outputSymbol.s.c_code, symGet.s.c_code);
  EXPECT_EQ(outputSymbol.s.attribute, symGet.s.attribute);
  EXPECT_EQ(outputSymbol.s.dim, symGet.s.dim);
  EXPECT_EQ(outputSymbol.s.type, symGet.s.type);
  EXPECT_EQ(outputSymbol.s.reserved, symGet.s.reserved);
  EXPECT_EQ(outputSymbol.s.symbolModifier, symGet.s.symbolModifier);
  EXPECT_EQ(outputSymbol.s.align, symGet.s.align);
  EXPECT_EQ(outputSymbol.d_init, symGet.d_init);
  EXPECT_EQ(outputSymbol.reserved, symGet.reserved);
  BrigDirectiveSymbol inputSymbol = {
  sizeof(BrigDirectiveSymbol),   // size
  BrigEDirectiveSymbol ,         // kind
  {
    curCodOffset,                // c_code
    BrigArgSpace,                // storag class kernarg
    BrigNone ,                   // attribut
    0,                           // reserved
    0,                           // symbolModifier
    0,                           // dim
    24,                          // s_name
    Brigf32,                     // type
    1                            // align
  },
  0,                             // d_init
  0,                             // reserved
  };

  context->get_directive(curDirOffset, &symGet);
  curDirOffset += sizeof(BrigDirectiveSymbol);

  EXPECT_EQ(inputSymbol.size, symGet.size);
  EXPECT_EQ(inputSymbol.kind, symGet.kind);
  EXPECT_EQ(inputSymbol.s.storageClass, symGet.s.storageClass);
  EXPECT_EQ(inputSymbol.s.s_name, symGet.s.s_name);
  EXPECT_EQ(inputSymbol.s.c_code, symGet.s.c_code);
  EXPECT_EQ(inputSymbol.s.attribute, symGet.s.attribute);
  EXPECT_EQ(inputSymbol.s.dim, symGet.s.dim);
  EXPECT_EQ(inputSymbol.s.type, symGet.s.type);
  EXPECT_EQ(inputSymbol.s.reserved, symGet.s.reserved);
  EXPECT_EQ(inputSymbol.s.symbolModifier, symGet.s.symbolModifier);
  EXPECT_EQ(inputSymbol.s.align, symGet.s.align);
  EXPECT_EQ(inputSymbol.d_init, symGet.d_init);
  EXPECT_EQ(inputSymbol.reserved, symGet.reserved);

  BrigDirectiveFunction callerFunRef = {
    sizeof(BrigDirectiveFunction),               // size
    BrigEDirectiveFunction,                      // kind
    128,                                         // c_code
    35,                                          // s_name
    0,                                           // inParamCount
    188,                                         // d_firstScopedDirective
    3,                                           // operationCount
    284,                                         // d_nextDirective
    BrigNone,                                    // attribute
    0,                                           // fbarCount
    0,                                           // outParamCount
    0                                            // d_firstInParam
  };

  context->get_directive(curDirOffset, &funGet);
  curDirOffset += sizeof(BrigDirectiveFunction);

  EXPECT_EQ(callerFunRef.size, funGet.size);
  EXPECT_EQ(callerFunRef.kind, funGet.kind);
  EXPECT_EQ(callerFunRef.s_name, funGet.s_name);
  EXPECT_EQ(callerFunRef.c_code, funGet.c_code);
  EXPECT_EQ(callerFunRef.outParamCount, funGet.outParamCount);
  EXPECT_EQ(callerFunRef.inParamCount, funGet.inParamCount);
  EXPECT_EQ(callerFunRef.operationCount, funGet.operationCount);
  EXPECT_EQ(callerFunRef.d_nextDirective, funGet.d_nextDirective);
  EXPECT_EQ(callerFunRef.d_firstScopedDirective, funGet.d_firstScopedDirective);
  EXPECT_EQ(callerFunRef.d_firstInParam, funGet.d_firstInParam);
  EXPECT_EQ(callerFunRef.reserved, funGet.reserved);
  EXPECT_EQ(callerFunRef.attribute, funGet.attribute);

  BrigDirectiveScope argStart = {
    sizeof(BrigDirectiveScope),
    BrigEDirectiveArgStart,
    128
  };
  BrigDirectiveScope getScope;
  context->get_directive(curDirOffset, &getScope);
  curDirOffset += sizeof(BrigDirectiveScope);
  EXPECT_EQ(argStart.size, getScope.size);
  EXPECT_EQ(argStart.kind, getScope.kind);
  EXPECT_EQ(argStart.c_code, getScope.c_code);

  BrigDirectiveSymbol anInputSymbol = {
  sizeof(BrigDirectiveSymbol),   // size
  BrigEDirectiveSymbol ,         // kind
  {
    128,                         // c_code
    BrigArgSpace,                // storag class kernarg
    BrigNone ,                   // attribut
    0,                           // reserved
    0,                           // symbolModifier
    0,                           // dim
    43,                          // s_name
    Brigf32,                     // type
    1                            // align
  },
  0,                             // d_init
  0,                             // reserved
  };

  context->get_directive(curDirOffset, &symGet);
  curDirOffset += sizeof(BrigDirectiveSymbol);

  EXPECT_EQ(anInputSymbol.size, symGet.size);
  EXPECT_EQ(anInputSymbol.kind, symGet.kind);
  EXPECT_EQ(anInputSymbol.s.storageClass, symGet.s.storageClass);
  EXPECT_EQ(anInputSymbol.s.s_name, symGet.s.s_name);
  EXPECT_EQ(anInputSymbol.s.c_code, symGet.s.c_code);
  EXPECT_EQ(anInputSymbol.s.attribute, symGet.s.attribute);
  EXPECT_EQ(anInputSymbol.s.dim, symGet.s.dim);
  EXPECT_EQ(anInputSymbol.s.type, symGet.s.type);
  EXPECT_EQ(anInputSymbol.s.reserved, symGet.s.reserved);
  EXPECT_EQ(anInputSymbol.s.symbolModifier, symGet.s.symbolModifier);
  EXPECT_EQ(anInputSymbol.s.align, symGet.s.align);
  EXPECT_EQ(anInputSymbol.d_init, symGet.d_init);
  EXPECT_EQ(anInputSymbol.reserved, symGet.reserved);

  BrigDirectiveSymbol anOutputSymbol = {
    sizeof(BrigDirectiveSymbol),
    BrigEDirectiveSymbol,
    {
      172,
      BrigArgSpace,
      BrigNone,
      0,
      0,
      0,
      57,
      Brigf32,
      1
    },
    0,
    0
  };
  context->get_directive(curDirOffset, &symGet);
  curDirOffset += sizeof(BrigDirectiveSymbol);

  EXPECT_EQ(anOutputSymbol.size, symGet.size);
  EXPECT_EQ(anOutputSymbol.kind, symGet.kind);
  EXPECT_EQ(anOutputSymbol.s.storageClass, symGet.s.storageClass);
  EXPECT_EQ(anOutputSymbol.s.s_name, symGet.s.s_name);
  EXPECT_EQ(anOutputSymbol.s.c_code, symGet.s.c_code);
  EXPECT_EQ(anOutputSymbol.s.attribute, symGet.s.attribute);
  EXPECT_EQ(anOutputSymbol.s.dim, symGet.s.dim);
  EXPECT_EQ(anOutputSymbol.s.type, symGet.s.type);
  EXPECT_EQ(anOutputSymbol.s.reserved, symGet.s.reserved);
  EXPECT_EQ(anOutputSymbol.s.symbolModifier, symGet.s.symbolModifier);
  EXPECT_EQ(anOutputSymbol.s.align, symGet.s.align);
  EXPECT_EQ(anOutputSymbol.d_init, symGet.d_init);
  EXPECT_EQ(anOutputSymbol.reserved, symGet.reserved);


  BrigDirectiveScope argEnd = {
    sizeof(BrigDirectiveScope),
    BrigEDirectiveArgEnd,
    248
  };

  context->get_directive(curDirOffset, &getScope);
  curDirOffset += sizeof(BrigDirectiveScope);
  EXPECT_EQ(argEnd.size, getScope.size);
  EXPECT_EQ(argEnd.kind, getScope.kind);
  EXPECT_EQ(argEnd.c_code, getScope.c_code);

  BrigInstLdSt instLd1 = {
    sizeof(BrigInstLdSt), // size
    BrigEInstLdSt,       // kind
    BrigLd,              // opcode
    Brigf32,             // type
    BrigNoPacking,       // packing
    {8, 32, 44, 0, 0},    // operand[5]
    BrigArgSpace,        // storageClass
    BrigRegular,         // memorySemantic
    0                    // equivClass
  };


  BrigInstLdSt ldStGet;
  context->get_code(curCodOffset, &ldStGet);
  curCodOffset += sizeof(BrigInstLdSt);

  EXPECT_EQ(instLd1.size, ldStGet.size);
  EXPECT_EQ(instLd1.kind, ldStGet.kind);
  EXPECT_EQ(instLd1.opcode, ldStGet.opcode);
  EXPECT_EQ(instLd1.type, ldStGet.type);
  EXPECT_EQ(instLd1.packing, ldStGet.packing);
  EXPECT_EQ(instLd1.o_operands[0], ldStGet.o_operands[0]);
  EXPECT_EQ(instLd1.o_operands[1], ldStGet.o_operands[1]);
  EXPECT_EQ(instLd1.o_operands[2], ldStGet.o_operands[2]);
  EXPECT_EQ(instLd1.o_operands[3], ldStGet.o_operands[3]);
  EXPECT_EQ(instLd1.o_operands[4], ldStGet.o_operands[4]);
  EXPECT_EQ(instLd1.storageClass, ldStGet.storageClass);
  EXPECT_EQ(instLd1.memorySemantic, ldStGet.memorySemantic);
  EXPECT_EQ(instLd1.equivClass, ldStGet.equivClass);

  BrigInstLdSt instSt1 = {
    sizeof(BrigInstLdSt), // size
    BrigEInstLdSt,       // kind
    BrigSt,              // opcode
    Brigf32,             // type
    BrigNoPacking,       // packing
    {32, 56, 0, 0, 0},    // operand[5]
    BrigArgSpace,        // storageClass
    BrigRegular,         // memorySemantic
    0                    // equivClass
  };


  context->get_code(curCodOffset, &ldStGet);
  curCodOffset += sizeof(BrigInstLdSt);

  EXPECT_EQ(instSt1.size, ldStGet.size);
  EXPECT_EQ(instSt1.kind, ldStGet.kind);
  EXPECT_EQ(instSt1.opcode, ldStGet.opcode);
  EXPECT_EQ(instSt1.type, ldStGet.type);
  EXPECT_EQ(instSt1.packing, ldStGet.packing);
  EXPECT_EQ(instSt1.o_operands[0], ldStGet.o_operands[0]);
  EXPECT_EQ(instSt1.o_operands[1], ldStGet.o_operands[1]);
  EXPECT_EQ(instSt1.o_operands[2], ldStGet.o_operands[2]);
  EXPECT_EQ(instSt1.o_operands[3], ldStGet.o_operands[3]);
  EXPECT_EQ(instSt1.o_operands[4], ldStGet.o_operands[4]);
  EXPECT_EQ(instSt1.storageClass, ldStGet.storageClass);
  EXPECT_EQ(instSt1.memorySemantic, ldStGet.memorySemantic);
  EXPECT_EQ(instSt1.equivClass, ldStGet.equivClass);

  BrigInstBase instRet = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigRet,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase baseGet;
  context->get_code(curCodOffset, &baseGet);
  curCodOffset += sizeof(BrigInstBase);

  EXPECT_EQ(instRet.size, baseGet.size);
  EXPECT_EQ(instRet.kind, baseGet.kind);
  EXPECT_EQ(instRet.opcode, baseGet.opcode);
  EXPECT_EQ(instRet.type, baseGet.type);
  EXPECT_EQ(instRet.packing, baseGet.packing);

  EXPECT_EQ(instRet.o_operands[0], baseGet.o_operands[0]);
  EXPECT_EQ(instRet.o_operands[1], baseGet.o_operands[1]);
  EXPECT_EQ(instRet.o_operands[2], baseGet.o_operands[2]);
  EXPECT_EQ(instRet.o_operands[3], baseGet.o_operands[3]);
  EXPECT_EQ(instRet.o_operands[4], baseGet.o_operands[4]);

  BrigInstLdSt instSt2 = {
    sizeof(BrigInstLdSt), // size
    BrigEInstLdSt,       // kind
    BrigSt,              // opcode
    Brigf32,             // type
    BrigNoPacking,       // packing
    {76, 88, 0, 0, 0},    // operand[5]
    BrigArgSpace,        // storageClass
    BrigRegular,         // memorySemantic
    0                    // equivClass
  };


  context->get_code(curCodOffset, &ldStGet);
  curCodOffset += sizeof(BrigInstLdSt);


  EXPECT_EQ(instSt2.size, ldStGet.size);
  EXPECT_EQ(instSt2.kind, ldStGet.kind);
  EXPECT_EQ(instSt2.opcode, ldStGet.opcode);
  EXPECT_EQ(instSt2.type, ldStGet.type);
  EXPECT_EQ(instSt2.packing, ldStGet.packing);
  EXPECT_EQ(instSt2.o_operands[0], ldStGet.o_operands[0]);
  EXPECT_EQ(instSt2.o_operands[1], ldStGet.o_operands[1]);
  EXPECT_EQ(instSt2.o_operands[2], ldStGet.o_operands[2]);
  EXPECT_EQ(instSt2.o_operands[3], ldStGet.o_operands[3]);
  EXPECT_EQ(instSt2.o_operands[4], ldStGet.o_operands[4]);
  EXPECT_EQ(instSt2.storageClass, ldStGet.storageClass);
  EXPECT_EQ(instSt2.memorySemantic, ldStGet.memorySemantic);
  EXPECT_EQ(instSt2.equivClass, ldStGet.equivClass);

  BrigInstBase instCall = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigCall,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {112, 144, 136, 156, 0}        // o_operands[5]
  };

  context->get_code(curCodOffset, &baseGet);
  curCodOffset += sizeof(BrigInstBase);
  EXPECT_EQ(instCall.size, baseGet.size);
  EXPECT_EQ(instCall.kind, baseGet.kind);
  EXPECT_EQ(instCall.opcode, baseGet.opcode);
  EXPECT_EQ(instCall.type, baseGet.type);
  EXPECT_EQ(instCall.packing, baseGet.packing);

  EXPECT_EQ(instCall.o_operands[0], baseGet.o_operands[0]);
  EXPECT_EQ(instCall.o_operands[1], baseGet.o_operands[1]);
  EXPECT_EQ(instCall.o_operands[2], baseGet.o_operands[2]);
  EXPECT_EQ(instCall.o_operands[3], baseGet.o_operands[3]);
  EXPECT_EQ(instCall.o_operands[4], baseGet.o_operands[4]);

  BrigInstLdSt instLd2 = {
    sizeof(BrigInstLdSt), // size
    BrigEInstLdSt,       // kind
    BrigLd,              // opcode
    Brigf32,             // type
    BrigNoPacking,       // packing
    {168, 32, 192, 0, 0},    // operand[5]
    BrigArgSpace,        // storageClass
    BrigRegular,         // memorySemantic
    0                    // equivClass
  };


  context->get_code(curCodOffset, &ldStGet);
  curCodOffset += sizeof(BrigInstLdSt);

  EXPECT_EQ(instLd2.size, ldStGet.size);
  EXPECT_EQ(instLd2.kind, ldStGet.kind);
  EXPECT_EQ(instLd2.opcode, ldStGet.opcode);
  EXPECT_EQ(instLd2.type, ldStGet.type);
  EXPECT_EQ(instLd2.packing, ldStGet.packing);
  EXPECT_EQ(instLd2.o_operands[0], ldStGet.o_operands[0]);
  EXPECT_EQ(instLd2.o_operands[1], ldStGet.o_operands[1]);
  EXPECT_EQ(instLd2.o_operands[2], ldStGet.o_operands[2]);
  EXPECT_EQ(instLd2.o_operands[3], ldStGet.o_operands[3]);
  EXPECT_EQ(instLd2.o_operands[4], ldStGet.o_operands[4]);
  EXPECT_EQ(instLd2.storageClass, ldStGet.storageClass);
  EXPECT_EQ(instLd2.memorySemantic, ldStGet.memorySemantic);
  EXPECT_EQ(instLd2.equivClass, ldStGet.equivClass);

  BrigOperandImmed getImm;

  curOpeOffset += curOpeOffset & 0x7;
  context->get_operand(curOpeOffset, &getImm);
  curOpeOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  BrigOperandReg s0Reg = {
    sizeof(BrigOperandReg), // size
    BrigEOperandReg,  // kind
    Brigb32,          // type
    0,                // reserved
    31                 // name
  };

  BrigOperandReg regGet;
  context->get_operand(curOpeOffset, &regGet);
  curOpeOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(s0Reg.size, regGet.size);
  EXPECT_EQ(s0Reg.kind, regGet.kind);
  EXPECT_EQ(s0Reg.type, regGet.type);
  EXPECT_EQ(s0Reg.reserved, regGet.reserved);
  EXPECT_EQ(s0Reg.s_name, regGet.s_name);

  BrigOperandAddress inputAddr = {
    sizeof(BrigOperandAddress),
    BrigEOperandAddress,
    Brigb32,
    0,
    108
  };

  BrigOperandAddress getAddr;
  context->get_operand(curOpeOffset, &getAddr);
  curOpeOffset += sizeof(BrigOperandAddress);
  // BrigOperandAddress
  EXPECT_EQ(inputAddr.size, getAddr.size);
  EXPECT_EQ(inputAddr.kind, getAddr.kind);
  EXPECT_EQ(inputAddr.type, getAddr.type);
  EXPECT_EQ(inputAddr.reserved, getAddr.reserved);
  EXPECT_EQ(inputAddr.directive, getAddr.directive);

  BrigOperandAddress outputAddr = {
    sizeof(BrigOperandAddress),
    BrigEOperandAddress,
    Brigb32,
    0,
    68
  };

  context->get_operand(curOpeOffset, &getAddr);
  curOpeOffset += sizeof(BrigOperandAddress);
  // BrigOperandAddress
  EXPECT_EQ(outputAddr.size, getAddr.size);
  EXPECT_EQ(outputAddr.kind, getAddr.kind);
  EXPECT_EQ(outputAddr.type, getAddr.type);
  EXPECT_EQ(outputAddr.reserved, getAddr.reserved);
  EXPECT_EQ(outputAddr.directive, getAddr.directive);

  BrigOperandArgumentRef anInput = {
    sizeof(BrigOperandArgumentRef),
    BrigEOperandArgumentRef,
    196
  };

  BrigOperandArgumentRef getRef;
  context->get_operand(curOpeOffset, &getRef);
  curOpeOffset += sizeof(BrigOperandArgumentRef);

  EXPECT_EQ(anInput.size, getRef.size);
  EXPECT_EQ(anInput.kind, getRef.kind);
  EXPECT_EQ(anInput.arg, getRef.arg);

  BrigOperandReg s1Reg = {
    sizeof(BrigOperandReg), // size
    BrigEOperandReg,  // kind
    Brigb32,          // type
    0,                // reserved
    53                 // name
  };

  context->get_operand(curOpeOffset, &regGet);
  curOpeOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(s1Reg.size, regGet.size);
  EXPECT_EQ(s1Reg.kind, regGet.kind);
  EXPECT_EQ(s1Reg.type, regGet.type);
  EXPECT_EQ(s1Reg.reserved, regGet.reserved);
  EXPECT_EQ(s1Reg.s_name, regGet.s_name);

  BrigOperandAddress anInputAddr = {
    sizeof(BrigOperandAddress),
    BrigEOperandAddress,
    Brigb32,
    0,
    196
  };

  context->get_operand(curOpeOffset, &getAddr);
  curOpeOffset += sizeof(BrigOperandAddress);
  // BrigOperandAddress
  EXPECT_EQ(anInputAddr.size, getAddr.size);
  EXPECT_EQ(anInputAddr.kind, getAddr.kind);
  EXPECT_EQ(anInputAddr.type, getAddr.type);
  EXPECT_EQ(anInputAddr.reserved, getAddr.reserved);
  EXPECT_EQ(anInputAddr.directive, getAddr.directive);

  BrigOperandArgumentRef anOutput = {
    sizeof(BrigOperandArgumentRef),
    BrigEOperandArgumentRef,
    236
  };

  context->get_operand(curOpeOffset, &getRef);
  curOpeOffset += sizeof(BrigOperandArgumentRef);

  EXPECT_EQ(anOutput.size, getRef.size);
  EXPECT_EQ(anOutput.kind, getRef.kind);
  EXPECT_EQ(anOutput.arg, getRef.arg);

  curOpeOffset += curOpeOffset & 0x7;
  context->get_operand(curOpeOffset, &getImm);
  curOpeOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  BrigOperandFunctionRef calleeOpFunRef = {
    sizeof(BrigOperandFunctionRef),
    BrigEOperandFunctionRef,
    28
  };

  BrigOperandFunctionRef getFunRef;
  context->get_operand(curOpeOffset, &getFunRef);
  curOpeOffset += sizeof(BrigOperandFunctionRef);

  EXPECT_EQ(calleeOpFunRef.size, getFunRef.size);
  EXPECT_EQ(calleeOpFunRef.kind, getFunRef.kind);
  EXPECT_EQ(calleeOpFunRef.fn, getFunRef.fn);

  BrigOperandArgumentList outputList = {
    sizeof(BrigOperandArgumentList),
    BrigEOperandArgumentList,
    1,
    { 100 }
  };

  BrigOperandArgumentList getArgList;
  context->get_operand(curOpeOffset, &getArgList);
  curOpeOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(outputList.size, getArgList.size);
  EXPECT_EQ(outputList.kind, getArgList.kind);
  EXPECT_EQ(outputList.elementCount, getArgList.elementCount);
  EXPECT_EQ(outputList.o_args[0], getArgList.o_args[0]);

  BrigOperandArgumentList inputList = {
    sizeof(BrigOperandArgumentList),
    BrigEOperandArgumentList,
    1,
    { 68 }
  };

  context->get_operand(curOpeOffset, &getArgList);
  curOpeOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(inputList.size, getArgList.size);
  EXPECT_EQ(inputList.kind, getArgList.kind);
  EXPECT_EQ(inputList.elementCount, getArgList.elementCount);
  EXPECT_EQ(inputList.o_args[0], getArgList.o_args[0]);

  curOpeOffset += curOpeOffset & 0x7;
  context->get_operand(curOpeOffset, &getImm);
  curOpeOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  BrigOperandAddress anOutputAddr = {
    sizeof(BrigOperandAddress),
    BrigEOperandAddress,
    Brigb32,
    0,
    236
  };

  context->get_operand(curOpeOffset, &getAddr);
  curOpeOffset += sizeof(BrigOperandAddress);
  // BrigOperandAddress
  EXPECT_EQ(anOutputAddr.size, getAddr.size);
  EXPECT_EQ(anOutputAddr.kind, getAddr.kind);
  EXPECT_EQ(anOutputAddr.type, getAddr.type);
  EXPECT_EQ(anOutputAddr.reserved, getAddr.reserved);
  EXPECT_EQ(anOutputAddr.directive, getAddr.directive);

  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(284, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(248, csize);
  BrigsOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(68, ssize);
  BrigoOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(204, osize);

  delete lexer;
}

}  // namespace brig
}  // namespace hsa
