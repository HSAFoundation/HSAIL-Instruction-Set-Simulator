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
#include "Instruction1_test.h"
#include "Instruction2_test.h"
#include "Instruction3_test.h"
#include "Instruction4_test.h"
#include "Ld_test.h"
#include "St_test.h"
#include "GlobalDecl_test.h"
#include "Syscall_test.h"

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
  EXPECT_EQ(132U, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(172U, csize);
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
  EXPECT_EQ(108U, label1.c_code);
  EXPECT_EQ(35U, label1.s_name);

  context->get_directive(120, &label1);
  EXPECT_EQ(140U, label1.c_code);
  EXPECT_EQ(49U, label1.s_name);

  // test BrigCbr
  BrigInstBase cbr_op;
  context->get_code(8, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCbr, cbr_op.opcode);
  EXPECT_EQ(Brigb1, cbr_op.type);
  EXPECT_EQ(8, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(44, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigBrn
  BrigInstBar br_op;
  context->get_code(72, &br_op);
  EXPECT_EQ(36U, br_op.size);
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
  EXPECT_EQ(8U, ref.name);

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
  ref.bits.u = (unsigned int) (-5);
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
  BrigoOffset32_t opOffset;
  EXPECT_EQ(0, AddressableOperandPart2(context, &opOffset, false));

  name.assign(input.c_str());
  BrigOperandAddress ref = {
    sizeof(ref),          // size
    BrigEOperandAddress,  // kind
    Brigb32,              // Data Type
    0,                    // reserved
    context->symbol_map[name]                    // directive
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
  context->get_code(8, &get);

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
  context->get_code(8, &get);

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
    {8, 0, 0, 0, 0},
    BrigGroupLevel
  };

  std::string input("barrier_group ;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Bar(context));

  BrigInstBar get;
  context->get_code(8, &get);

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

  ref.o_operands[0] = 0 ;//update

  BrigInstBar get_bib;
  context->get_code(8, &get_bib);

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
    { 2 }
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

TEST(CodegenTest, MemoryOperand_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandIndirect ref = {
    16,                    // size
    BrigEOperandIndirect,  // kind
    8,                     // reg
    Brigb64,               // type
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
  EXPECT_EQ(8, getReg.name);

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
    Brigb64,               // type
    0,                     // reserved
    8,                     // name
    20,                    // reg
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
  context->get_operand(20, &getReg);
  context->get_operand(32, &get2);

  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
  //EXPECT_EQ(0, getAddr.offset);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(15, getReg.name);
  // BrigOperandCompoud
  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.type, get2.type);
  EXPECT_EQ(ref2.reserved, get2.reserved);
  EXPECT_EQ(ref2.name, get2.name);
  EXPECT_EQ(ref2.reg, get2.reg);
  EXPECT_EQ(ref2.offset, get2.offset);

  BrigOperandCompound ref3 = {
    20,                    // size
    BrigEOperandCompound,  // kind
    Brigb64,               // type
    0,                     // reserved
    20,                    // name
    32,                    // reg
    0                      // offset
  };
  BrigOperandCompound get3;

  context->clear_context();
  input.assign("[%loc],\n");
  input.append("[&array][$s1]\n");
  input.append("[$d3-5]");
  lexer->set_source_string(input);

  context->token_to_scan = lexer->get_next_token();

  context->add_symbol("&array");
  context->add_symbol("%loc");

  EXPECT_EQ(0, MemoryOperand(context));
  EXPECT_EQ(',', context->token_to_scan);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));
  EXPECT_EQ(0, MemoryOperand(context));

  context->get_operand(8, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
//  EXPECT_EQ(0, getAddr.offset);

  context->get_operand(20, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
//  EXPECT_EQ(0, getAddr.offset);


  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(44, &get3);
  // BrigOperandCompoud
  EXPECT_EQ(ref3.size, get3.size);
  EXPECT_EQ(ref3.kind, get3.kind);
  EXPECT_EQ(ref3.type, get3.type);
  EXPECT_EQ(ref3.reserved, get3.reserved);
  EXPECT_EQ(ref3.name, get3.name);
  EXPECT_EQ(ref3.reg, get3.reg);
  EXPECT_EQ(ref3.offset, get3.offset);

  BrigOperandIndirect refInd = {
    16,                    // size
    BrigEOperandIndirect,  // kind
    64,                    // reg
    Brigb64,               // type
    0,                     // reserved
    -5                     // offset
  };
  BrigOperandIndirect getInd;

  context->get_operand(76, &getInd);
  // BrigOperandIndirect
  EXPECT_EQ(refInd.size, getInd.size);
  EXPECT_EQ(refInd.kind, getInd.kind);
  EXPECT_EQ(refInd.reg, getInd.reg);
  EXPECT_EQ(refInd.type, getInd.type);
  EXPECT_EQ(refInd.reserved, getInd.reserved);
  EXPECT_EQ(refInd.offset, getInd.offset);


  delete lexer;
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
  EXPECT_EQ(ref.name, getReg.name);

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
  EXPECT_EQ(15, getReg.name);
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

TEST(CodegenTest, Instruction1_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("laneid $s1;\n");
  input.append("clock $d6;\n");
  input.append("fbar_wait_b64 $d1;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstBase ref1 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigLaneId,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {8, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase ref2 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigClock,             // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {20, 0, 0, 0, 0},      // o_operands[5]
  };

  BrigInstBase ref3 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigFbarWait,          // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {32, 0, 0, 0, 0},      // o_operands[5]
  };

  BrigInstBase getMem;
  BrigOperandReg getReg;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));
  EXPECT_EQ(0, Instruction1(context));
  EXPECT_EQ(0, Instruction1(context));

  context->get_operand(8, &getReg);
  context->get_code(8, &getMem);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

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

  context->get_operand(20, &getReg);
  context->get_code(40, &getMem);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

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

  context->get_operand(32, &getReg);
  context->get_code(72, &getMem);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  // BrigInstMem
  EXPECT_EQ(ref3.size, getMem.size);
  EXPECT_EQ(ref3.kind, getMem.kind);
  EXPECT_EQ(ref3.opcode, getMem.opcode);
  EXPECT_EQ(ref3.type, getMem.type);
  EXPECT_EQ(ref3.packing, getMem.packing);

  EXPECT_EQ(ref3.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref3.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref3.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref3.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref3.o_operands[4], getMem.o_operands[4]);

  delete lexer;
}

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
    {8, 20, 32, 56, 0},    // o_operands[5]
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
    {80, 92, 104, 0, 0},   // o_operands[5]
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
  EXPECT_EQ(16, getReg.name);

  context->get_operand(20, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
 // EXPECT_EQ(0, getAddr.offset);

  context->get_operand(32, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(23, getImm.bits.u);

  context->get_operand(56, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(12, getImm.bits.u);

  context->get_code(8, &getAtom);
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

  context->get_operand(80, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(92, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);
 // EXPECT_EQ(0, getAddr.offset);

  context->get_operand(104, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(24, getImm.bits.u);

  context->get_code(52, &getAtom);
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
}

TEST(CodegenTest,GlobalReadOnlyImageDeclCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_ROImg &demo={format = signed_int32 ,order = r,width = 4,height = 5,depth = 6 } ;");

  Lexer *lexer = new Lexer(input);
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveImage ref = {
    56,                     //size
    BrigEDirectiveImage,    //kind
    {
      8,                        // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      8,                        // s_name
      BrigROImg,                // type
      1,                        // align
    },
    4,                      //width
    5,                      //height
    6,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigImageFormatUnknown  //format
  };
  EXPECT_EQ(0,GlobalReadOnlyImageDecl(context));

  BrigDirectiveImage get ;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);

  delete lexer ;
}

TEST(CodegenTest,GlobalImageDeclCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_RWImg &demo[9]={format = signed_int32 ,order = r,width = 2,height = 3,depth = 4 } ;");

  Lexer *lexer = new Lexer(input);
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveImage ref = {
    sizeof(BrigDirectiveImage),                     //size
    BrigEDirectiveImage,    //kind
    {
      8,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      8,                        // s_name
      BrigRWImg,                  // type
      1,                        // align
    },
    2,                      //width
    3,                      //height
    4,                      //depth
    9,                      //array
    BrigImageOrderUnknown,  //order
    BrigImageFormatUnknown  //format
  };
  EXPECT_EQ(0,GlobalImageDecl(context));

  BrigDirectiveImage get ;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  EXPECT_EQ(ref.array, get.array);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);

  delete lexer ;
}

TEST(CodegenTest,GlobalSamplerDeclCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_Samp &demo={coord = normalized, filter = linear, ");
  input.append("boundaryU = clamp, boundaryV = wrap, boundaryW = mirror } ;");

  Lexer *lexer = new Lexer(input);
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveSampler ref = {
    sizeof(BrigDirectiveSampler),//size
    BrigEDirectiveSampler,       //kind
    {
      8,                        // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                        // symbolModifier
      0,                        // dim
      8,                        // s_name
      BrigSamp,                 // type
      1,                        // align
    },
    1,                      //valid
    1,                      //normalized
    BrigSamplerFilterLinear,//filter
    BrigSamplerClamp,       //boundaryU
    BrigSamplerWrap,        //boundaryV
    BrigSamplerMirror,      //boundaryW
    0                       //reserved1
  };
  EXPECT_EQ(0,GlobalSamplerDecl(context));

  BrigDirectiveSampler get ;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);
  EXPECT_EQ(ref.reserved1, get.reserved1);


  context->set_error_reporter(main_reporter);
  context->clear_context();
  input.assign("global_Samp &demo={coord = unnormalized, filter = nearest, ");
  input.append("boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,GlobalSamplerDecl(context));

  context->get_directive(8,&get);
  ref.normalized=0;
  ref.filter=BrigSamplerFilterNearest;
  ref.boundaryU=BrigSamplerMirrorOnce;
  ref.boundaryV=BrigSamplerBorder;
  ref.boundaryW=BrigSamplerClamp;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);

  context->set_error_reporter(main_reporter);
  context->clear_context();
  input.assign("global_Samp &demo={coord = unnormalized, filter = nearest, ");
  input.append("boundaryU = wrap, boundaryV = clamp, boundaryW = wrap } ;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,GlobalSamplerDecl(context));

  context->get_directive(8,&get);
  ref.normalized=0;
  ref.filter=BrigSamplerFilterNearest;
  ref.boundaryU=BrigSamplerWrap;
  ref.boundaryV=BrigSamplerClamp;
  ref.boundaryW=BrigSamplerWrap;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);

  context->set_error_reporter(main_reporter);
  context->clear_context();
  input.assign("global_Samp &demo={coord = unnormalized, filter = nearest, ");
  input.append("boundaryU = mirror, boundaryV = mirror, boundaryW = border } ;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,GlobalSamplerDecl(context));

  context->get_directive(8,&get);
  ref.normalized=0;
  ref.filter=BrigSamplerFilterNearest;
  ref.boundaryU=BrigSamplerMirror;
  ref.boundaryV=BrigSamplerMirror;
  ref.boundaryW=BrigSamplerBorder;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);

  context->set_error_reporter(main_reporter);
  context->clear_context();
  input.assign("global_Samp &demo={coord = unnormalized, filter = nearest, ");
  input.append("boundaryU = border, boundaryV = mirroronce, boundaryW = mirroronce } ;");

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,GlobalSamplerDecl(context));

  context->get_directive(8,&get);
  ref.normalized=0;
  ref.filter=BrigSamplerFilterNearest;
  ref.boundaryU=BrigSamplerBorder;
  ref.boundaryV=BrigSamplerMirrorOnce;
  ref.boundaryW=BrigSamplerMirrorOnce;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);


  delete lexer ;
}



TEST(CodegenTest, OffsetAddressableOperand_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandIndirect ref1 = {
    16,                     // size
    BrigEOperandIndirect,   // kind
    8,                      // reg
    Brigb64,               // type
    0,                      // reserved
    11                      // offset
  };
  BrigOperandIndirect ref2 = {
    16,                     // size
    BrigEOperandIndirect,   // kind
    36,                     // reg
    Brigb64,                // type
    0,                      // reserved
    -5                      // offset
  };
  BrigOperandIndirect ref3 = {
    16,                     // size
    BrigEOperandIndirect,   // kind
    0,                      // reg
    Brigb64,                // type
    0,                      // reserved
    77                      // offset
  };
  BrigOperandIndirect ref4 = {
    16,                     // size
    BrigEOperandIndirect,   // kind
    80,                     // reg
    Brigb64,                // type
    0,                      // reserved
    0                       // offset
  };

  BrigOperandIndirect get1, get2, get3, get4;
  // The register must be an s or d register (c registers are not allowed).
  std::string input("[$s2+11]\n");
  input.append("[$d2-5]\n");
  input.append("[77]\n");
  input.append("[$d5]\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  // pass the first '['
  if (context->token_to_scan == '[') {
    context->token_to_scan = lexer->get_next_token();
  }

  EXPECT_EQ(0, OffsetAddressableOperand(context));
  if (context->token_to_scan == '[') {
    context->token_to_scan = lexer->get_next_token();
    EXPECT_EQ(0, OffsetAddressableOperand(context));
  }

  if (context->token_to_scan == '[') {
    context->token_to_scan = lexer->get_next_token();
    EXPECT_EQ(0, OffsetAddressableOperand(context));
  }

  if (context->token_to_scan == '[') {
    context->token_to_scan = lexer->get_next_token();
    context->set_machine(BrigELarge);
    EXPECT_EQ(0, OffsetAddressableOperand(context));
  }

  context->get_operand(20, &get1);

  // BrigOperandIndirect
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.reg, get1.reg);
  EXPECT_EQ(ref1.type, get1.type);
  EXPECT_EQ(ref1.reserved, get1.reserved);
  EXPECT_EQ(ref1.offset, get1.offset);

  context->get_operand(48, &get2);

  // BrigOperandIndirect
  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.reg, get2.reg);
  EXPECT_EQ(ref2.type, get2.type);
  EXPECT_EQ(ref2.reserved, get2.reserved);
  EXPECT_EQ(ref2.offset, get2.offset);

  context->get_operand(64, &get3);

  // BrigOperandIndirect
  EXPECT_EQ(ref3.size, get3.size);
  EXPECT_EQ(ref3.kind, get3.kind);
  EXPECT_EQ(ref3.reg, get3.reg);
  EXPECT_EQ(ref3.type, get3.type);
  EXPECT_EQ(ref3.reserved, get3.reserved);
  EXPECT_EQ(ref3.offset, get3.offset);

  context->get_operand(92, &get4);

  // BrigOperandIndirect
  EXPECT_EQ(ref4.size, get4.size);
  EXPECT_EQ(ref4.kind, get4.kind);
  EXPECT_EQ(ref4.reg, get4.reg);
  EXPECT_EQ(ref4.type, get4.type);
  EXPECT_EQ(ref4.reserved, get4.reserved);
  EXPECT_EQ(ref4.offset, get4.offset);

  delete lexer;
}

TEST(CodegenTest, GlobalPrivateDeclCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("private_f32  &demo; ");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveSymbol ref = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    8,                         // c_code
    BrigPrivateSpace,         // storag class
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    8,                        // s_name
    Brigf32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };
  EXPECT_EQ(0,GlobalPrivateDecl(context));
  BrigDirectiveSymbol get ;

  context->get_directive(8, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);

  delete lexer;
}

TEST(CodegenTest, GlobalGroupDeclCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("group_f32  &demo; ");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveSymbol ref = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    8,                         // c_code
    BrigGroupSpace,         // storag class
    BrigNone ,                // attribut
    0,                        // reserved
    0,                        // symbolModifier
    0,                        // dim
    8,                        // s_name
    Brigf32,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };
  EXPECT_EQ(0,GlobalGroupDecl(context));
  BrigDirectiveSymbol get ;

  context->get_directive(8, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);

  delete lexer;
}

TEST(CodegenTest, Label_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase refCbrLab3 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigCbr,               // opcode
    Brigb1,               // type
    BrigNoPacking,         // packing
    {40, 64, 76, 0, 0}        // o_operands[5]
  };

  BrigInstBar refBrnLab1 = {
    36,                  // size
    BrigEInstBar,        // kind
    BrigBrn,             // opcode
    Brigb32,             // type
    BrigNoPacking,       // packing
    {8, 32, 0, 0, 0},     // o_operands[5]
    0                    // syncFlags
  };
  BrigInstBase refCbrLab1 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigCbr,               // opcode
    Brigb1,               // type
    BrigNoPacking,         // packing
    {120, 64, 32, 0, 0}        // o_operands[5]
  };
  BrigInstBar refBrnLab2 = {
    36,                  // size
    BrigEInstBar,        // kind
    BrigBrn,             // opcode
    Brigb32,             // type
    BrigNoPacking,       // packing
    {88, 112, 0, 0, 0},     // o_operands[5]
    0                    // syncFlags
  };

  BrigDirectiveLabel ref1 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    44,                     // c_code
    8                       // s_name
  };
  BrigDirectiveLabel ref2 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    76,                     // c_code
    18                      // s_name
  };
  BrigDirectiveLabel ref3 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    76,                     // c_code
    24                      // s_name
  };
  BrigDirectiveLabel ref4 = {
    12,                     // size
    BrigEDirectiveLabel,    // kind
    112,                    // c_code
    30                      // s_name
  };

  BrigDirectiveLabel get1, get2, get3, get4;
  BrigOperandLabelRef getLabRef;
  BrigInstBase getCbrCode;
  BrigInstBar getBrnCode;

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
  // TODO(Chuang) set the value of .syncFlags
  // EXPECT_EQ(refBrn.syncFlags, getBrnCode.syncFlags);

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

  context->get_code(44, &getCbrCode);

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
  EXPECT_EQ(14, getReg.name);

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

  context->get_code(76, &getBrnCode);

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

  context->get_code(112, &getCbrCode);

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

TEST(CodegenTest, ImageRet_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstAtomicImage ref1 = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicImage,        // opcode
    Brigb32,                    // type
    BrigNoPacking,          // packing
    {8, 20, 36, 48, 0},         // o_operands[5]
    BrigAtomicXor,          // atomicOperation
    BrigGlobalSpace,        // storageClass
    BrigRegular,            // memorySemantic
    Briggeom_1d             // geom

  };

  BrigInstAtomicImage ref2 = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicImage,        // opcode
    Brigb32,                    // type
    BrigNoPacking,          // packing
    {60, 72, 36, 48, 88},         // o_operands[5]
    BrigAtomicCas,          // atomicOperation
    BrigGlobalSpace,        // storageClass
    BrigRegular,            // memorySemantic
    Briggeom_1d             // geom
  };

  BrigInstAtomicImage get1, get2;
  BrigOperandReg getReg;
  BrigOperandOpaque getImg;

  // The register must be an s or d register (c registers are not allowed).
  std::string input("atomic_image_xor_1d_b32 $s0, [&namedRWImg1], $s1, $s3;\n");
  input.append("atomic_image_cas_1d_b32 $s10, [&namedRWImg2], $s1, $s3, $s4;\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, ImageRet(context));
  EXPECT_EQ(0, ImageRet(context));

  context->get_code(8, &get1);

  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.opcode, get1.opcode);
  EXPECT_EQ(ref1.type, get1.type);
  EXPECT_EQ(ref1.packing, get1.packing);
  EXPECT_EQ(ref1.o_operands[0], get1.o_operands[0]);
  EXPECT_EQ(ref1.o_operands[1], get1.o_operands[1]);
  EXPECT_EQ(ref1.o_operands[2], get1.o_operands[2]);
  EXPECT_EQ(ref1.o_operands[3], get1.o_operands[3]);
  EXPECT_EQ(ref1.o_operands[4], get1.o_operands[4]);

  EXPECT_EQ(ref1.atomicOperation, get1.atomicOperation);
  EXPECT_EQ(ref1.storageClass, get1.storageClass);
  EXPECT_EQ(ref1.memorySemantic, get1.memorySemantic);
  EXPECT_EQ(ref1.geom, get1.geom);

  context->get_code(56, &get2);

  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.opcode, get2.opcode);
  EXPECT_EQ(ref2.type, get2.type);
  EXPECT_EQ(ref2.packing, get2.packing);
  EXPECT_EQ(ref2.o_operands[0], get2.o_operands[0]);
  EXPECT_EQ(ref2.o_operands[1], get2.o_operands[1]);
  EXPECT_EQ(ref2.o_operands[2], get2.o_operands[2]);
  EXPECT_EQ(ref2.o_operands[3], get2.o_operands[3]);
  EXPECT_EQ(ref2.o_operands[4], get2.o_operands[4]);

  EXPECT_EQ(ref2.atomicOperation, get2.atomicOperation);
  EXPECT_EQ(ref2.storageClass, get2.storageClass);
  EXPECT_EQ(ref2.memorySemantic, get2.memorySemantic);
  EXPECT_EQ(ref2.geom, get2.geom);

  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(36, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);
  context->get_operand(48, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);
  context->get_operand(60, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);
  context->get_operand(88, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(25, getReg.name);


  context->get_operand(20, &getImg);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImg.size);
  EXPECT_EQ(BrigEOperandOpaque, getImg.kind);
  EXPECT_EQ(0, getImg.name);
  EXPECT_EQ(0, getImg.reg);
  EXPECT_EQ(0, getImg.offset);

  context->get_operand(72, &getImg);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImg.size);
  EXPECT_EQ(BrigEOperandOpaque, getImg.kind);
  EXPECT_EQ(0, getImg.name);
  EXPECT_EQ(0, getImg.reg);
  EXPECT_EQ(0, getImg.offset);

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
  EXPECT_EQ(refRegD1.name, getReg.name);

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
  EXPECT_EQ(refRegS3.name, getReg.name);

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
  EXPECT_EQ(refRegS2.name, getReg.name);

  EXPECT_EQ(108, context->get_operand_offset());

  delete lexer;
}

TEST(CodegenTest, ImageNoRet_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstAtomicImage ref1 = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicNoRetImage,   // opcode
    Brigb32,                // type
    BrigNoPacking,          // packing
    {8, 24, 36, 48, 0},     // o_operands[5]
    BrigAtomicCas,          // atomicOperation
    BrigGlobalSpace,        // storageClass
    BrigRegular,            // memorySemantic
    Briggeom_1d             // geom
  };

  BrigInstAtomicImage ref2 = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicNoRetImage,   // opcode
    Brigs32,                // type
    BrigNoPacking,          // packing
    {60, 88, 104, 0, 0},    // o_operands[5]
    BrigAtomicAnd,          // atomicOperation
    BrigGlobalSpace,        // storageClass
    BrigAcquireRelease,     // memorySemantic
    Briggeom_2d             // geom
  };

  BrigInstAtomicImage get1, get2;
  BrigOperandReg getReg;
  BrigOperandOpaque getImg;

  // The register must be an s or d register (c registers are not allowed).
  std::string input("atomicNoRet_image_cas_1d_b32 [&namedRWImg], $s1, $s3, $s4;\n");
  input.append("atomicNoRet_image_and_ar_2d_s32 [&namedRWImg], ($s0,$s3), $s2;\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, ImageNoRet(context));
  EXPECT_EQ(0, ImageNoRet(context));

  context->get_code(8, &get1);

  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.opcode, get1.opcode);
  EXPECT_EQ(ref1.type, get1.type);
  EXPECT_EQ(ref1.packing, get1.packing);
  EXPECT_EQ(ref1.o_operands[0], get1.o_operands[0]);
  EXPECT_EQ(ref1.o_operands[1], get1.o_operands[1]);
  EXPECT_EQ(ref1.o_operands[2], get1.o_operands[2]);
  EXPECT_EQ(ref1.o_operands[3], get1.o_operands[3]);
  EXPECT_EQ(ref1.o_operands[4], get1.o_operands[4]);

  EXPECT_EQ(ref1.atomicOperation, get1.atomicOperation);
  EXPECT_EQ(ref1.storageClass, get1.storageClass);
  EXPECT_EQ(ref1.memorySemantic, get1.memorySemantic);
  EXPECT_EQ(ref1.geom, get1.geom);

  context->get_code(56, &get2);

  EXPECT_EQ(ref2.size, get2.size);
  EXPECT_EQ(ref2.kind, get2.kind);
  EXPECT_EQ(ref2.opcode, get2.opcode);
  EXPECT_EQ(ref2.type, get2.type);
  EXPECT_EQ(ref2.packing, get2.packing);
  EXPECT_EQ(ref2.o_operands[0], get2.o_operands[0]);
  EXPECT_EQ(ref2.o_operands[1], get2.o_operands[1]);
  EXPECT_EQ(ref2.o_operands[2], get2.o_operands[2]);
  EXPECT_EQ(ref2.o_operands[3], get2.o_operands[3]);
  EXPECT_EQ(ref2.o_operands[4], get2.o_operands[4]);

  EXPECT_EQ(ref2.atomicOperation, get2.atomicOperation);
  EXPECT_EQ(ref2.storageClass, get2.storageClass);
  EXPECT_EQ(ref2.memorySemantic, get2.memorySemantic);
  EXPECT_EQ(ref2.geom, get2.geom);

  context->get_operand(24, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(36, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);
  context->get_operand(48, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);
  context->get_operand(76, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);
  BrigOperandRegV2 getRegV2;
  context->get_operand(88, &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(16, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(76, getRegV2.regs[0]);
  EXPECT_EQ(36, getRegV2.regs[1]);


  context->get_operand(8, &getImg);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImg.size);
  EXPECT_EQ(BrigEOperandOpaque, getImg.kind);
  EXPECT_EQ(0, getImg.name);
  EXPECT_EQ(0, getImg.reg);
  EXPECT_EQ(0, getImg.offset);

  context->get_operand(60, &getImg);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImg.size);
  EXPECT_EQ(BrigEOperandOpaque, getImg.kind);
  EXPECT_EQ(0, getImg.name);
  EXPECT_EQ(0, getImg.reg);
  EXPECT_EQ(0, getImg.offset);

  delete lexer;
}


TEST(CodegenTest, Query_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase refOrder = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigQueryOrder,        // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {8, 20, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase refData = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigQueryData,         // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {36, 60, 0, 0, 0}      // o_operands[5]
  };

  BrigInstBase refWidth = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigQueryWidth,        // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {8, 76, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase get;
  BrigOperandReg getReg;
  BrigOperandOpaque getImage;

  std::string input("query_order_b32 $s0, [&namedRWImg1];\n");
  input.append("query_data_b32 $s1, [&namedRWImg1<$s2 - 4>];\n");
  input.append("query_width_u32 $s0, [&namedSamp<10>];\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  context->symbol_map["&namedRWImg1"] = 30;
  context->symbol_map["&namedSamp"] = 0;

  EXPECT_EQ(0, Query(context));
  EXPECT_EQ(0, Query(context));
  EXPECT_EQ(0, Query(context));

  context->get_code(8, &get);

  EXPECT_EQ(refOrder.size, get.size);
  EXPECT_EQ(refOrder.kind, get.kind);
  EXPECT_EQ(refOrder.opcode, get.opcode);
  EXPECT_EQ(refOrder.type, get.type);
  EXPECT_EQ(refOrder.packing, get.packing);
  EXPECT_EQ(refOrder.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(refOrder.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(refOrder.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(refOrder.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(refOrder.o_operands[4], get.o_operands[4]);

  context->get_code(40, &get);

  EXPECT_EQ(refData.size, get.size);
  EXPECT_EQ(refData.kind, get.kind);
  EXPECT_EQ(refData.opcode, get.opcode);
  EXPECT_EQ(refData.type, get.type);
  EXPECT_EQ(refData.packing, get.packing);
  EXPECT_EQ(refData.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(refData.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(refData.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(refData.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(refData.o_operands[4], get.o_operands[4]);

  context->get_code(72, &get);

  EXPECT_EQ(refWidth.size, get.size);
  EXPECT_EQ(refWidth.kind, get.kind);
  EXPECT_EQ(refWidth.opcode, get.opcode);
  EXPECT_EQ(refWidth.type, get.type);
  EXPECT_EQ(refWidth.packing, get.packing);
  EXPECT_EQ(refWidth.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(refWidth.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(refWidth.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(refWidth.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(refWidth.o_operands[4], get.o_operands[4]);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(36, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);
  context->get_operand(48, &getReg);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(20, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(60, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(48, getImage.reg);
  EXPECT_EQ(-4, getImage.offset);

  context->get_operand(76, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(10, getImage.offset);

  delete lexer;
}

TEST(CodegenTest, Instruction1Op_CodeGen_fbar_Wait_b64) {

  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFbarWait,
    Brigb64,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  };

  std::string input("fbar_wait_b64 $d0;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_operand(8, &getReg);
  context->get_code(8, &getBase);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigInstBase
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.packing, getBase.packing);

  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Instruction1Op_CodeGen_fbar_Arrive_b64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFbarArrive,
    Brigb64,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  };

  std::string input("fbar_arrive_b64 $d0;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_operand(8, &getReg);
  context->get_code(8, &getBase);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigInstBase
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.packing, getBase.packing);

  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Instruction1Op_CodeGen_fbar_skip_b64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigFbarSkip,
    Brigb64,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  };

  std::string input("fbar_skip_b64 $d0;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_operand(8, &getReg);
  context->get_code(8, &getBase);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigInstBase
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.packing, getBase.packing);

  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Instruction1Op_CodeGen_Countup_u32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigCountup,
    Brigu32,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  };

  std::string input("countup_u32 $s1;");
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_operand(8, &getReg);
  context->get_code(8, &getBase);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigInstBase
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.packing, getBase.packing);

  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest, Instruction1op_CodeGen_fbar_Release) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("fbar_release_b64 $d0;");
  input.append("fbar_release_b64 $d0,$d1;");

  Lexer* lexer = new Lexer(input);

  BrigInstBase ref1={
    32,
    BrigEInstBase,
    BrigFbarRelease,
    Brigb64,
    BrigNoPacking,
    {8, 0, 0, 0, 0}
  };
  BrigInstBase ref2={
    32,
    BrigEInstBase,
    BrigFbarRelease,
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };

  BrigInstBase getBase;
  BrigOperandReg getReg1,getReg2;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));
  EXPECT_EQ(0, Instruction1(context));

  context->get_code(8, &getBase);
  context->get_operand(8, &getReg1);

  // BrigOperandReg
  EXPECT_EQ(12, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb64, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(8, getReg1.name);

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

  context->get_operand(8, &getReg1);
  context->get_operand(20, &getReg2);
  context->get_code(40, &getBase);

  // BrigOperandReg1
  EXPECT_EQ(12, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb64, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(8, getReg1.name);

 // BrigOperandReg2
  EXPECT_EQ(12, getReg2.size);
  EXPECT_EQ(BrigEOperandReg, getReg2.kind);
  EXPECT_EQ(Brigb64, getReg2.type);
  EXPECT_EQ(0, getReg2.reserved);
  EXPECT_EQ(12, getReg2.name);

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

TEST(CodegenTest,FileDeclCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("file 1 \"math.c\" ;");

  BrigDirectiveFile ref = {
    16,                   //size
    BrigEDirectiveFile,   //kind
    8,                    //c_code
    1,                    //fileid
    8                     //s_filename
  };

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0,FileDecl(context));

  BrigDirectiveFile get;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.fileid,get.fileid);
  EXPECT_EQ(ref.s_filename,get.s_filename);
  delete lexer;
}

TEST(CodegenTest,LocationCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("loc 1 10 5 ;");

  BrigDirectiveLoc ref = {
    20,                   //size
    BrigEDirectiveLoc,    //kind
    8,                    //c_code
    1,                    //sourceFile
    10,                   //sourceLine
    5                     //sourceColumn
  };

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0,Location(context));

  BrigDirectiveLoc get;
  context->get_directive(8,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.sourceFile,get.sourceFile);
  EXPECT_EQ(ref.sourceLine,get.sourceLine);
  EXPECT_EQ(ref.sourceColumn,get.sourceColumn);

  delete lexer;
}

TEST(CodegenTest, ImageStore_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstImage ref2da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    {56, 80, 108, 0, 0},   // o_operands[5]
    Briggeom_2da,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstImage ref1da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    {132, 156, 172, 0, 0},   // o_operands[5]
    Briggeom_1da,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstImage ref1db = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    {188, 212, 44, 0, 0},   // o_operands[5]
    Briggeom_1db,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
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
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(96, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(156, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(212, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
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
    {56, 80, 108, 0, 0},   // o_operands[5]
    Briggeom_2da,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstImage ref1da = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    {132, 156, 172, 0, 0},   // o_operands[5]
    Briggeom_1da,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstImage ref1db = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    {188, 212, 44, 0, 0},   // o_operands[5]
    Briggeom_1db,          // geom
    Brigf32,               // type
    Brigu32,               // stype
    BrigNoPacking,         // packing
    0                      // reserved
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
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(96, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(156, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(212, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(30, getImage.name);
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
    40,                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    {56, 80, 96, 112, 0},   // o_operands[5]
    Briggeom_1d,           // geom
    Brigf32,               // stype
    Brigs32,               // type
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstRead ref1da = {
    40,                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    {136, 160, 176, 192, 0}, // o_operands[5]
    Briggeom_1da,          // geom
    Brigf32,               // stype
    Brigs32,               // type
    BrigNoPacking,         // packing
    0                      // reserved
  };

  BrigInstRead ref2da = {
    40,                    // size
    BrigEInstRead,        // kind
    BrigRdImage,           // opcode
    {220, 244, 260, 276, 0},  // o_operands[5]
    Briggeom_2da,          // geom
    Brigf32,               // stype
    Brigs32,               // type
    BrigNoPacking,         // packing
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
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(112, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_operand(124, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(28, getReg.name);

  context->get_operand(208, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(32, getReg.name);

  context->get_operand(80, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(96, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(160, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(176, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(244, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf7, getImage.name);
  EXPECT_EQ(0, getImage.reg);
  EXPECT_EQ(0, getImage.offset);

  context->get_operand(260, &getImage);
  // BrigOperandOpaque
  EXPECT_EQ(16, getImage.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
  EXPECT_EQ(0xf1, getImage.name);
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

TEST(CodegenTest, Cmp_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();


  // TODO(Chuang): set a correct value to aluModifier.
  BrigInstCmp cmpNeRef = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigCmp,            // opcode
    Brigf32,            // type
    BrigNoPacking,      // packing
    {8, 20, 32, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    BrigNe,             // comparisonOperator
    Brigb1,             // sourceType
    0                   // reserved
  };

  BrigInstCmp cmpGtRef = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigCmp,            // opcode
    Brigf32,            // type
    BrigNoPacking,      // packing
    {8, 56, 72, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    BrigGt,             // comparisonOperator
    Brigb32,             // sourceType
    0                   // reserved
  };

  BrigInstCmp cmpEquRef = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigCmp,            // opcode
    Brigb1,             // type
    BrigNoPacking,      // packing
    {96, 56, 112, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    BrigEqu,            // comparisonOperator
    Brigf32,            // sourceType
    0                   // reserved
  };

  BrigInstCmp cmpSltuRef = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigCmp,            // opcode
    Brigb1,             // type
    BrigNoPacking,      // packing
    {96, 136, 148, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    BrigSltu,           // comparisonOperator
    Brigf64,            // sourceType
    0                   // reserved
  };

  // TODO(Chuang): set a correct value to sourceType.
  BrigInstCmp packedCmpLtRef = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigPackedCmp,      // opcode
    Brigf32x2,          // type
    BrigNoPacking,      // packing
    {136, 148, 160, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    BrigLt,             // comparisonOperator
    0,                  // sourceType
    0                   // reserved
  };

  BrigInstCmp get;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;

  std::string input("cmp_ne_f32_b1 $s1, $c2, 0.0f;\n");
  input.append("cmp_gt_f32_b32 $s1, $s2, 0.0f;\n");
  input.append("cmp_equ_b1_f32 $c1, $s2, 0;\n");
  input.append("cmp_sltu_b1_f64 $c1, $d1, $d2;\n");
  input.append("packedcmp_lt_f32x2 $d1, $d2, $d3;\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Cmp(context));
  EXPECT_EQ(0, Cmp(context));
  EXPECT_EQ(0, Cmp(context));
  EXPECT_EQ(0, Cmp(context));
  EXPECT_EQ(0, Cmp(context));

  context->get_code(8, &get);

  EXPECT_EQ(cmpNeRef.size, get.size);
  EXPECT_EQ(cmpNeRef.kind, get.kind);
  EXPECT_EQ(cmpNeRef.opcode, get.opcode);
  EXPECT_EQ(cmpNeRef.type, get.type);
  EXPECT_EQ(cmpNeRef.packing, get.packing);
  EXPECT_EQ(cmpNeRef.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(cmpNeRef.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(cmpNeRef.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(cmpNeRef.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(cmpNeRef.o_operands[4], get.o_operands[4]);

  // EXPECT_EQ(cmpNeRef.aluModifier, get.aluModifier);
  unsigned int *pAluModRef = reinterpret_cast<unsigned int*>(&cmpNeRef.aluModifier);
  unsigned int *pAluModGet = reinterpret_cast<unsigned int*>(&get.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cmpNeRef.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(cmpNeRef.sourceType, get.sourceType);
  EXPECT_EQ(cmpNeRef.reserved, get.reserved);

  context->get_code(52, &get);

  EXPECT_EQ(cmpGtRef.size, get.size);
  EXPECT_EQ(cmpGtRef.kind, get.kind);
  EXPECT_EQ(cmpGtRef.opcode, get.opcode);
  EXPECT_EQ(cmpGtRef.type, get.type);
  EXPECT_EQ(cmpGtRef.packing, get.packing);
  EXPECT_EQ(cmpGtRef.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(cmpGtRef.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(cmpGtRef.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(cmpGtRef.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(cmpGtRef.o_operands[4], get.o_operands[4]);

  // EXPECT_EQ(cmpGtRef.aluModifier, get.aluModifier);
  pAluModRef = reinterpret_cast<unsigned int*>(&cmpGtRef.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&get.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cmpGtRef.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(cmpGtRef.sourceType, get.sourceType);
  EXPECT_EQ(cmpGtRef.reserved, get.reserved);

  context->get_code(96, &get);

  EXPECT_EQ(cmpEquRef.size, get.size);
  EXPECT_EQ(cmpEquRef.kind, get.kind);
  EXPECT_EQ(cmpEquRef.opcode, get.opcode);
  EXPECT_EQ(cmpEquRef.type, get.type);
  EXPECT_EQ(cmpEquRef.packing, get.packing);
  EXPECT_EQ(cmpEquRef.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(cmpEquRef.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(cmpEquRef.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(cmpEquRef.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(cmpEquRef.o_operands[4], get.o_operands[4]);

  //  EXPECT_EQ(cmpEquRef.aluModifier, get.aluModifier);
  pAluModRef = reinterpret_cast<unsigned int*>(&cmpEquRef.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&get.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cmpEquRef.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(cmpEquRef.sourceType, get.sourceType);
  EXPECT_EQ(cmpEquRef.reserved, get.reserved);

  context->get_code(140, &get);

  EXPECT_EQ(cmpSltuRef.size, get.size);
  EXPECT_EQ(cmpSltuRef.kind, get.kind);
  EXPECT_EQ(cmpSltuRef.opcode, get.opcode);
  EXPECT_EQ(cmpSltuRef.type, get.type);
  EXPECT_EQ(cmpSltuRef.packing, get.packing);
  EXPECT_EQ(cmpSltuRef.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(cmpSltuRef.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(cmpSltuRef.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(cmpSltuRef.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(cmpSltuRef.o_operands[4], get.o_operands[4]);

  //  EXPECT_EQ(cmpSltuRef.aluModifier, get.aluModifier);
  pAluModRef = reinterpret_cast<unsigned int*>(&cmpSltuRef.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&get.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cmpSltuRef.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(cmpSltuRef.sourceType, get.sourceType);
  EXPECT_EQ(cmpSltuRef.reserved, get.reserved);

  context->get_code(184, &get);

  EXPECT_EQ(packedCmpLtRef.size, get.size);
  EXPECT_EQ(packedCmpLtRef.kind, get.kind);
  EXPECT_EQ(packedCmpLtRef.opcode, get.opcode);
  EXPECT_EQ(packedCmpLtRef.type, get.type);
  EXPECT_EQ(packedCmpLtRef.packing, get.packing);
  EXPECT_EQ(packedCmpLtRef.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(packedCmpLtRef.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(packedCmpLtRef.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(packedCmpLtRef.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(packedCmpLtRef.o_operands[4], get.o_operands[4]);

  //  EXPECT_EQ(packedCmpLtRef.aluModifier, get.aluModifier);
  pAluModRef = reinterpret_cast<unsigned int*>(&packedCmpLtRef.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&get.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(packedCmpLtRef.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(packedCmpLtRef.sourceType, get.sourceType);
  EXPECT_EQ(packedCmpLtRef.reserved, get.reserved);

  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb1, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(56, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(96, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb1, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(136, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_operand(148, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(28, getReg.name);

  context->get_operand(160, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(32, getReg.name);

  context->get_operand(32, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  // TODO(Chuang): confirm the type of Immediate number.
  // EXPECT_EQ(Brigb1, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.c);

  context->get_operand(72, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  context->get_operand(112, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0.0f, getImm.bits.f);

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
    BrigEMaxTid,
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

TEST(CodegenTest, Mov_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("mov_b1 $c1, 0;\n");
  input.append("mov_b32 $s1, 0.0f;\n");
  input.append("mov_b64 $d1, 0.0;\n");
  input.append("mov_b64 $d1, ($s4,$s9);\n");
  input.append("mov_b128 ($s9,$s8,$s4,$s1), $q1;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstBase movB1Ref = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    Brigb1,               // type
    BrigNoPacking,         // packing
    {8, 24, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase movB32Ref = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {48, 64, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase movB64Ref1 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {88, 104, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase movB64Ref2 = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {88, 152, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase movB128Ref = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    Brigb128,               // type
    BrigNoPacking,         // packing
    {180, 204, 0, 0, 0}       // o_operands[5]
  };


  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Mov(context));
  EXPECT_EQ(0, Mov(context));
  EXPECT_EQ(0, Mov(context));
  EXPECT_EQ(0, Mov(context));
  EXPECT_EQ(0, Mov(context));

  context->get_code(8, &getBase);
  // BrigInstBase
  EXPECT_EQ(movB1Ref.size, getBase.size);
  EXPECT_EQ(movB1Ref.kind, getBase.kind);
  EXPECT_EQ(movB1Ref.opcode, getBase.opcode);
  EXPECT_EQ(movB1Ref.type, getBase.type);
  EXPECT_EQ(movB1Ref.packing, getBase.packing);

  EXPECT_EQ(movB1Ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(movB1Ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(movB1Ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(movB1Ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(movB1Ref.o_operands[4], getBase.o_operands[4]);

  context->get_code(40, &getBase);
  // BrigInstBase
  EXPECT_EQ(movB32Ref.size, getBase.size);
  EXPECT_EQ(movB32Ref.kind, getBase.kind);
  EXPECT_EQ(movB32Ref.opcode, getBase.opcode);
  EXPECT_EQ(movB32Ref.type, getBase.type);
  EXPECT_EQ(movB32Ref.packing, getBase.packing);
  EXPECT_EQ(movB32Ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(movB32Ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(movB32Ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(movB32Ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(movB32Ref.o_operands[4], getBase.o_operands[4]);

  context->get_code(72, &getBase);
  // BrigInstBase
  EXPECT_EQ(movB64Ref1.size, getBase.size);
  EXPECT_EQ(movB64Ref1.kind, getBase.kind);
  EXPECT_EQ(movB64Ref1.opcode, getBase.opcode);
  EXPECT_EQ(movB64Ref1.type, getBase.type);
  EXPECT_EQ(movB64Ref1.packing, getBase.packing);
  EXPECT_EQ(movB64Ref1.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(movB64Ref1.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(movB64Ref1.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(movB64Ref1.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(movB64Ref1.o_operands[4], getBase.o_operands[4]);

  context->get_code(104, &getBase);
  // BrigInstBase
  EXPECT_EQ(movB64Ref2.size, getBase.size);
  EXPECT_EQ(movB64Ref2.kind, getBase.kind);
  EXPECT_EQ(movB64Ref2.opcode, getBase.opcode);
  EXPECT_EQ(movB64Ref2.type, getBase.type);
  EXPECT_EQ(movB64Ref2.packing, getBase.packing);
  EXPECT_EQ(movB64Ref2.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(movB64Ref2.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(movB64Ref2.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(movB64Ref2.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(movB64Ref2.o_operands[4], getBase.o_operands[4]);

  context->get_code(136, &getBase);
  // BrigInstBase
  EXPECT_EQ(movB128Ref.size, getBase.size);
  EXPECT_EQ(movB128Ref.kind, getBase.kind);
  EXPECT_EQ(movB128Ref.opcode, getBase.opcode);
  EXPECT_EQ(movB128Ref.type, getBase.type);
  EXPECT_EQ(movB128Ref.packing, getBase.packing);
  EXPECT_EQ(movB128Ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(movB128Ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(movB128Ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(movB128Ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(movB128Ref.o_operands[4], getBase.o_operands[4]);

  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb1, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(48, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(88, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(128, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_operand(140, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_operand(168, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(28, getReg.name);

  context->get_operand(204, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb128, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(32, getReg.name);

  context->get_operand(24, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  context->get_operand(64, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0.0f, getImm.bits.f);

  context->get_operand(104, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb64, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0.0, getImm.bits.d);

  delete lexer;
}


TEST(CodegenTest, AtomicNoRet_CodeGen_Test) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstAtomic refCas = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomicNoRet,       // opcode
    Brigb64,               // type
    BrigNoPacking,         // packing
    {8, 24, 48, 0, 0},     // o_operands[5]
    BrigAtomicCas,         // atomicOperation
    BrigGlobalSpace,       // storageClass
    BrigAcquireRelease     // memorySemantic
  };

  BrigInstAtomic refMax = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomicNoRet,       // opcode
    Brigf64,               // type
    BrigNoPacking,         // packing
    {72, 88, 0, 0, 0},     // o_operands[5]
    BrigAtomicMax,         // atomicOperation
    BrigGlobalSpace,       // storageClass
    BrigAcquireRelease     // memorySemantic
  };

  BrigInstAtomic getAtom;
  BrigOperandAddress getAddr;
  BrigOperandImmed getImm;


  std::string input("atomicNoRet_cas_global_ar_b64 [&x], 23, 12;\n");
  input.append("atomicNoRet_max_global_ar_f64 [&x], 23.0;\n");

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, AtomicNoRet(context));
  EXPECT_EQ(0, AtomicNoRet(context));

  context->get_code(8, &getAtom);
  // BrigInstAtomic
  EXPECT_EQ(refCas.size, getAtom.size);
  EXPECT_EQ(refCas.kind, getAtom.kind);
  EXPECT_EQ(refCas.opcode, getAtom.opcode);
  EXPECT_EQ(refCas.type, getAtom.type);
  EXPECT_EQ(refCas.packing, getAtom.packing);

  EXPECT_EQ(refCas.o_operands[0], getAtom.o_operands[0]);
  EXPECT_EQ(refCas.o_operands[1], getAtom.o_operands[1]);
  EXPECT_EQ(refCas.o_operands[2], getAtom.o_operands[2]);
  EXPECT_EQ(refCas.o_operands[3], getAtom.o_operands[3]);
  EXPECT_EQ(refCas.o_operands[4], getAtom.o_operands[4]);
  EXPECT_EQ(refCas.atomicOperation, getAtom.atomicOperation);
  EXPECT_EQ(refCas.storageClass, getAtom.storageClass);
  EXPECT_EQ(refCas.memorySemantic, getAtom.memorySemantic);

  context->get_code(52, &getAtom);
  // BrigInstAtomic
  EXPECT_EQ(refMax.size, getAtom.size);
  EXPECT_EQ(refMax.kind, getAtom.kind);
  EXPECT_EQ(refMax.opcode, getAtom.opcode);
  EXPECT_EQ(refMax.type, getAtom.type);
  EXPECT_EQ(refMax.packing, getAtom.packing);

  EXPECT_EQ(refMax.o_operands[0], getAtom.o_operands[0]);
  EXPECT_EQ(refMax.o_operands[1], getAtom.o_operands[1]);
  EXPECT_EQ(refMax.o_operands[2], getAtom.o_operands[2]);
  EXPECT_EQ(refMax.o_operands[3], getAtom.o_operands[3]);
  EXPECT_EQ(refMax.o_operands[4], getAtom.o_operands[4]);
  EXPECT_EQ(refMax.atomicOperation, getAtom.atomicOperation);
  EXPECT_EQ(refMax.storageClass, getAtom.storageClass);
  EXPECT_EQ(refMax.memorySemantic, getAtom.memorySemantic);

  context->get_operand(24, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  // TODO(Chuang): set Brigb64 to getImm.type.
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(23, getImm.bits.u);

  context->get_operand(48, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  // TODO(Chuang): set Brigb64 to getImm.type.
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(12, getImm.bits.u);

  context->get_operand(88, &getImm);
  // BrigOperandImmed
  EXPECT_EQ(24, getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb64, getImm.type);
  EXPECT_EQ(23.0, getImm.bits.d);


  context->get_operand(8, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);


  context->get_operand(72, &getAddr);
  // BrigOperandAddress
  EXPECT_EQ(12, getAddr.size);
  EXPECT_EQ(BrigEOperandAddress, getAddr.kind);
  EXPECT_EQ(Brigb64, getAddr.type);
  EXPECT_EQ(0, getAddr.reserved);
  EXPECT_EQ(0, getAddr.directive);


  delete lexer;
}

TEST(CodegenTest, Cvt_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("cvt_f32_f64 $s1, $d1;\n");
  input.append("cvt_down_f32_f32 $s1,$s2;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstCvt cvtRef1 = {
    40,                    // size
    BrigEInstCvt,         // kind
    BrigCvt,               // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {8, 20, 0, 0, 0},     // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},  // aluModifier
    Brigf64,                   // stype
    0                    // reserved
  };

  BrigInstCvt cvtRef2 = {
    40,                    // size
    BrigEInstCvt,         // kind
    BrigCvt,               // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {8, 32, 0, 0, 0},     // o_operands[5]
    {1, 3, 0, 0, 0, 0, 0},     // aluModifier
    Brigf32,                   // stype
    0                    // reserved
  };


  const BrigoOffset32_t oStartOffset = context->get_operand_offset();
  const BrigcOffset32_t cStartOffset = context->get_code_offset();
  BrigInstCvt getCvt;
  BrigOperandReg getReg;
  unsigned int* pAluModRef,* pAluModGet;
  pAluModRef = pAluModGet = NULL;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Cvt(context));
  EXPECT_EQ(0, Cvt(context));

  context->get_code(8, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  context->get_code(48, &getCvt);

  EXPECT_EQ(cvtRef2.size, getCvt.size);
  EXPECT_EQ(cvtRef2.kind, getCvt.kind);
  EXPECT_EQ(cvtRef2.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef2.type, getCvt.type);
  EXPECT_EQ(cvtRef2.packing, getCvt.packing);
  EXPECT_EQ(cvtRef2.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef2.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef2.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef2.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef2.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef2.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef2.stype, getCvt.stype);
  EXPECT_EQ(cvtRef2.reserved, getCvt.reserved);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  /*************************************** Case 3 ************************************/
  context->clear_context();
  input.assign("cvt_upi_u32_f32 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigu32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf32;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));
  cvtRef1.aluModifier.floatOrInt = 0;
  cvtRef1.aluModifier.rounding = 2;

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);
  /*************************************** Case 4 ************************************/
  context->clear_context();
  input.assign("cvt_neari_f32_f32 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigf32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf32;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));
  // TODO(Chuang): the value of aluModifier is zero.
  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  /*************************************** Case 5 ************************************/
  context->clear_context();
  input.assign("cvt_u32_f32 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigu32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf32;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);


  /*************************************** Case 6 ************************************/
  context->clear_context();
  input.assign("cvt_f16_f32 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigf16;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf32;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  /*************************************** Case 7 ************************************/
  context->clear_context();
  input.assign("cvt_s32_u8 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigs32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigu8;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  /*************************************** Case 8 ************************************/
  context->clear_context();
  input.assign("cvt_f32_f16 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigf32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf16;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  /*************************************** Case 9 ************************************/
  context->clear_context();
  input.assign("cvt_ftz_up_f32_f32 $s1, $s2;\n");
  lexer->set_source_string(input);
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Cvt(context));

  context->get_code(cStartOffset, &getCvt);
  cvtRef1.size = sizeof(cvtRef1);     
  cvtRef1.kind = BrigEInstCvt;
  cvtRef1.opcode = BrigCvt;
  cvtRef1.type = Brigf32;
  cvtRef1.packing = BrigNoPacking;
  cvtRef1.o_operands[0] = oStartOffset;
  cvtRef1.o_operands[1] = sizeof(getReg) + oStartOffset;
  cvtRef1.o_operands[2] = 0;
  cvtRef1.o_operands[3] = 0;
  cvtRef1.o_operands[4] = 0;
  cvtRef1.stype = Brigf32;
  cvtRef1.reserved = 0;
  memset(&cvtRef1.aluModifier, 0, sizeof(cvtRef1.aluModifier));
  cvtRef1.aluModifier.floatOrInt = 1;
  cvtRef1.aluModifier.rounding = 2;
  cvtRef1.aluModifier.ftz = 1;

  context->get_code(cStartOffset, &getCvt);

  EXPECT_EQ(cvtRef1.size, getCvt.size);
  EXPECT_EQ(cvtRef1.kind, getCvt.kind);
  EXPECT_EQ(cvtRef1.opcode, getCvt.opcode);
  EXPECT_EQ(cvtRef1.type, getCvt.type);
  EXPECT_EQ(cvtRef1.packing, getCvt.packing);
  EXPECT_EQ(cvtRef1.o_operands[0], getCvt.o_operands[0]);
  EXPECT_EQ(cvtRef1.o_operands[1], getCvt.o_operands[1]);
  EXPECT_EQ(cvtRef1.o_operands[2], getCvt.o_operands[2]);
  EXPECT_EQ(cvtRef1.o_operands[3], getCvt.o_operands[3]);
  EXPECT_EQ(cvtRef1.o_operands[4], getCvt.o_operands[4]);

  pAluModRef = reinterpret_cast<unsigned int*>(&cvtRef1.aluModifier);
  pAluModGet = reinterpret_cast<unsigned int*>(&getCvt.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  EXPECT_EQ(cvtRef1.stype, getCvt.stype);
  EXPECT_EQ(cvtRef1.reserved, getCvt.reserved);

  delete lexer;
}


TEST(CodegenTest,  Instruction5_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase f2u4Ref = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigF2u4,               // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {8, 20, 32, 44, 32}       // o_operands[5]
  };

  std::string input("f2u4_u32 $s1, $s2, $s3, $s9, $s3;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction5(context));

  BrigOperandReg getReg;
  BrigInstBase  getF2u4;

  context->get_code(8, &getF2u4);

  // BrigInstBase
  EXPECT_EQ(f2u4Ref.size, getF2u4.size);
  EXPECT_EQ(f2u4Ref.kind, getF2u4.kind);
  EXPECT_EQ(f2u4Ref.opcode, getF2u4.opcode);
  EXPECT_EQ(f2u4Ref.type, getF2u4.type);
  EXPECT_EQ(f2u4Ref.packing, getF2u4.packing);
  EXPECT_EQ(f2u4Ref.o_operands[0], getF2u4.o_operands[0]);
  EXPECT_EQ(f2u4Ref.o_operands[1], getF2u4.o_operands[1]);
  EXPECT_EQ(f2u4Ref.o_operands[2], getF2u4.o_operands[2]);
  EXPECT_EQ(f2u4Ref.o_operands[3], getF2u4.o_operands[3]);
  EXPECT_EQ(f2u4Ref.o_operands[4], getF2u4.o_operands[4]);


  context->get_operand(8, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  context->get_operand(20, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  context->get_operand(32, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  context->get_operand(44, &getReg);
  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  delete lexer;
}
TEST(CodegenTest,  Instruction4_Fma_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstMod fmaF32Ref = {
    sizeof(BrigInstMod),                    // size
    BrigEInstMod,         // kind
    BrigFma,               // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},      // o_operands[5]
    {1, 2, 0, 1, 0, 0 ,0}
  };

  BrigInstBase fmaF64Ref = {
    sizeof(BrigInstBase),                    // size
    BrigEInstBase,         // kind
    BrigFma,               // opcode
    Brigf64,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  std::string input("fma_ftz_up_f32 $s3,1.0f,$s1,23f;\n");
  input.append("fma_f64 $d3,1.0,$d0, $d3;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4FmaPart2(context));
  EXPECT_EQ(0, Instruction4FmaPart2(context));

  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigInstBase  getFma;
  BrigInstMod getMod;

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  fmaF32Ref.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);
  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  curOpOffset += curOpOffset & 0x7;
  fmaF32Ref.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(1.0f, getImm.bits.f);

  fmaF32Ref.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  curOpOffset += curOpOffset & 0x7;

  fmaF32Ref.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(23.0f, getImm.bits.f);

  fmaF64Ref.o_operands[0] = curOpOffset;
  fmaF64Ref.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  curOpOffset += curOpOffset & 0x7;
  fmaF64Ref.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb64, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(1.0, getImm.bits.d);

  fmaF64Ref.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  context->get_code(curCodeOffset, &getMod);
  curCodeOffset += sizeof(BrigInstMod);

  // BrigInstMod
  EXPECT_EQ(fmaF32Ref.size, getMod.size);
  EXPECT_EQ(fmaF32Ref.kind, getMod.kind);
  EXPECT_EQ(fmaF32Ref.opcode, getMod.opcode);
  EXPECT_EQ(fmaF32Ref.type, getMod.type);
  EXPECT_EQ(fmaF32Ref.packing, getMod.packing);
  EXPECT_EQ(fmaF32Ref.o_operands[0], getMod.o_operands[0]);
  EXPECT_EQ(fmaF32Ref.o_operands[1], getMod.o_operands[1]);
  EXPECT_EQ(fmaF32Ref.o_operands[2], getMod.o_operands[2]);
  EXPECT_EQ(fmaF32Ref.o_operands[3], getMod.o_operands[3]);
  EXPECT_EQ(fmaF32Ref.o_operands[4], getMod.o_operands[4]);
  // EXPECT_EQ(cmpNeRef.aluModifier, get.aluModifier);
  unsigned int *pAluModRef = reinterpret_cast<unsigned int*>(&fmaF32Ref.aluModifier);
  unsigned int *pAluModGet = reinterpret_cast<unsigned int*>(&getMod.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  context->get_code(curCodeOffset, &getFma);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase
  EXPECT_EQ(fmaF64Ref.size, getFma.size);
  EXPECT_EQ(fmaF64Ref.kind, getFma.kind);
  EXPECT_EQ(fmaF64Ref.opcode, getFma.opcode);
  EXPECT_EQ(fmaF64Ref.type, getFma.type);
  EXPECT_EQ(fmaF64Ref.packing, getFma.packing);
  EXPECT_EQ(fmaF64Ref.o_operands[0], getFma.o_operands[0]);
  EXPECT_EQ(fmaF64Ref.o_operands[1], getFma.o_operands[1]);
  EXPECT_EQ(fmaF64Ref.o_operands[2], getFma.o_operands[2]);
  EXPECT_EQ(fmaF64Ref.o_operands[3], getFma.o_operands[3]);
  EXPECT_EQ(fmaF64Ref.o_operands[4], getFma.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest,  Instruction4_Cmov_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  // Note: Cmov without ftz and rounding.
  BrigInstBase cmovB32Ref = {
    sizeof(BrigInstBase),                    // size
    BrigEInstBase,         // kind
    BrigCmov,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  BrigInstBase cmovU8x4Ref = {
    sizeof(BrigInstBase),                    // size
    BrigEInstBase,         // kind
    BrigCmov,               // opcode
    Brigu8x4,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  std::string input("cmov_b32 $s1, $c0, $s3, $s2;\n");
  input.append("cmov_u8x4 $s1, $s0, $s1, $s2;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4CmovPart5(context));
  EXPECT_EQ(0, Instruction4CmovPart5(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  BrigOperandReg getReg;
  BrigInstBase  getCmov;

  cmovB32Ref.o_operands[0] = curOpOffset;
  cmovU8x4Ref.o_operands[0] = curOpOffset;
  cmovU8x4Ref.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  cmovB32Ref.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb1, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  cmovB32Ref.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  cmovB32Ref.o_operands[3] = curOpOffset;
  cmovU8x4Ref.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  cmovU8x4Ref.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  context->get_code(curCodeOffset, &getCmov);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase
  EXPECT_EQ(cmovB32Ref.size, getCmov.size);
  EXPECT_EQ(cmovB32Ref.kind, getCmov.kind);
  EXPECT_EQ(cmovB32Ref.opcode, getCmov.opcode);
  EXPECT_EQ(cmovB32Ref.type, getCmov.type);
  EXPECT_EQ(cmovB32Ref.packing, getCmov.packing);
  EXPECT_EQ(cmovB32Ref.o_operands[0], getCmov.o_operands[0]);
  EXPECT_EQ(cmovB32Ref.o_operands[1], getCmov.o_operands[1]);
  EXPECT_EQ(cmovB32Ref.o_operands[2], getCmov.o_operands[2]);
  EXPECT_EQ(cmovB32Ref.o_operands[3], getCmov.o_operands[3]);
  EXPECT_EQ(cmovB32Ref.o_operands[4], getCmov.o_operands[4]);

  context->get_code(curCodeOffset, &getCmov);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase
  EXPECT_EQ(cmovU8x4Ref.size, getCmov.size);
  EXPECT_EQ(cmovU8x4Ref.kind, getCmov.kind);
  EXPECT_EQ(cmovU8x4Ref.opcode, getCmov.opcode);
  EXPECT_EQ(cmovU8x4Ref.type, getCmov.type);
  EXPECT_EQ(cmovU8x4Ref.packing, getCmov.packing);
  EXPECT_EQ(cmovU8x4Ref.o_operands[0], getCmov.o_operands[0]);
  EXPECT_EQ(cmovU8x4Ref.o_operands[1], getCmov.o_operands[1]);
  EXPECT_EQ(cmovU8x4Ref.o_operands[2], getCmov.o_operands[2]);
  EXPECT_EQ(cmovU8x4Ref.o_operands[3], getCmov.o_operands[3]);
  EXPECT_EQ(cmovU8x4Ref.o_operands[4], getCmov.o_operands[4]);

  delete lexer;
}


TEST(CodegenTest,  Instruction4_Shuffle_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  // Note: Shuffle without ftz and rounding.
  BrigInstBase shuffleRef = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,          // kind
    BrigShuffle,               // opcode
    Brigu8x4,               // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0}         // o_operands[5]
  };

  std::string input("shuffle_u8x4 $s10, $s12, $s12, 0x55;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4ShufflePart6(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  BrigOperandReg getReg;
  BrigInstBase  getShuffle;
  BrigOperandImmed getImm;

  shuffleRef.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  shuffleRef.o_operands[1] = curOpOffset;
  shuffleRef.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(13, getReg.name);
  context->get_code(curCodeOffset, &getShuffle);
  curCodeOffset += sizeof(BrigInstBase);

  curOpOffset += curOpOffset & 0x7;
  shuffleRef.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0x55, getImm.bits.u);


  context->get_code(curCodeOffset, &getShuffle);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase
  EXPECT_EQ(shuffleRef.size, getShuffle.size);
  EXPECT_EQ(shuffleRef.kind, getShuffle.kind);
  EXPECT_EQ(shuffleRef.opcode, getShuffle.opcode);
  EXPECT_EQ(shuffleRef.type, getShuffle.type);
  EXPECT_EQ(shuffleRef.packing, getShuffle.packing);
  EXPECT_EQ(shuffleRef.o_operands[0], getShuffle.o_operands[0]);
  EXPECT_EQ(shuffleRef.o_operands[1], getShuffle.o_operands[1]);
  EXPECT_EQ(shuffleRef.o_operands[2], getShuffle.o_operands[2]);
  EXPECT_EQ(shuffleRef.o_operands[3], getShuffle.o_operands[3]);
  EXPECT_EQ(shuffleRef.o_operands[4], getShuffle.o_operands[4]);

  delete lexer;
}


TEST(CodegenTest,  Instruction4_Mad_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstMod madMod = {
    sizeof(BrigInstMod),   // size
    BrigEInstMod,         // kind
    BrigMad,               // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    {1, 3, 0, 1, 0, 0, 0}  // aluModifier
  };

  BrigInstBase madBase = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,          // kind
    BrigMad,               // opcode
    Brigs64,               // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0}         // o_operands[5]
  };


  std::string input("mad_ftz_down_f32 $s3,1.0f,$s1,23f;\n");
  input.append("mad_s64 $d1, $d2, $d3, $d4;\n");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4MadPart3(context));
  EXPECT_EQ(0, Instruction4MadPart3(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  BrigOperandReg getReg;
  BrigInstBase getBase;
  BrigInstMod getMod;
  BrigOperandImmed getImm;

  madMod.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  curOpOffset += curOpOffset & 0x7;
  madMod.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(1.0f, getImm.bits.f);

  madMod.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  curOpOffset += curOpOffset & 0x7;
  madMod.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  // BrigOperandImmed
  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(23.0f, getImm.bits.f);

  madBase.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  madBase.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  madBase.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);

  madBase.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  // BrigOperandReg
  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(28, getReg.name);


  context->get_code(curCodeOffset, &getMod);
  curCodeOffset += sizeof(BrigInstMod);

  // BrigInstMod
  EXPECT_EQ(madMod.size, getMod.size);
  EXPECT_EQ(madMod.kind, getMod.kind);
  EXPECT_EQ(madMod.opcode, getMod.opcode);
  EXPECT_EQ(madMod.type, getMod.type);
  EXPECT_EQ(madMod.packing, getMod.packing);
  EXPECT_EQ(madMod.o_operands[0], getMod.o_operands[0]);
  EXPECT_EQ(madMod.o_operands[1], getMod.o_operands[1]);
  EXPECT_EQ(madMod.o_operands[2], getMod.o_operands[2]);
  EXPECT_EQ(madMod.o_operands[3], getMod.o_operands[3]);
  EXPECT_EQ(madMod.o_operands[4], getMod.o_operands[4]);
  // EXPECT_EQ(cmpNeRef.aluModifier, get.aluModifier);
  unsigned int *pAluModRef = reinterpret_cast<unsigned int*>(&madMod.aluModifier);
  unsigned int *pAluModGet = reinterpret_cast<unsigned int*>(&getMod.aluModifier);
  EXPECT_EQ(*pAluModRef, *pAluModGet);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase
  EXPECT_EQ(madBase.size, getBase.size);
  EXPECT_EQ(madBase.kind, getBase.kind);
  EXPECT_EQ(madBase.opcode, getBase.opcode);
  EXPECT_EQ(madBase.type, getBase.type);
  EXPECT_EQ(madBase.packing, getBase.packing);

  EXPECT_EQ(madBase.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(madBase.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(madBase.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(madBase.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(madBase.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}


TEST(CodegenTest,  Instruction4_BitStringOperation_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  // Note: Bit String Operation without ftz and rounding.
  BrigInstBase extrInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigExtract,           // opcode
    Brigu64,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase inseInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigInsert,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase bitsInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigBitSelect,         // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };


  std::string input("extract_u64 $d1, $d1, $s1, $s2;\n");
  input.append("insert_b32 $s1, $s1, 2, 3;\n");
  input.append("bitselect_b32 $s1, $s1, 2, 3;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4BitStringOperationPart4(context));
  EXPECT_EQ(0, Instruction4BitStringOperationPart4(context));
  EXPECT_EQ(0, Instruction4BitStringOperationPart4(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  BrigOperandReg getReg;
  BrigInstBase getBase;
  BrigOperandImmed getImm;

  // BrigOperandReg D1
  extrInst.o_operands[0] = curOpOffset;
  extrInst.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigOperandReg S1
  extrInst.o_operands[2] = curOpOffset;
  inseInst.o_operands[0] = curOpOffset;
  inseInst.o_operands[1] = curOpOffset;
  bitsInst.o_operands[0] = curOpOffset;
  bitsInst.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  // BrigOperandReg S2
  extrInst.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  // BrigOperandImmed b32 2
  curOpOffset += curOpOffset & 0x7;
  inseInst.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(2, getImm.bits.u);

  // BrigOperandImmed b32 3
  curOpOffset += curOpOffset & 0x7;
  inseInst.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(3, getImm.bits.u);

  // BrigOperandImmed b32 2
  curOpOffset += curOpOffset & 0x7;
  bitsInst.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(2, getImm.bits.u);

  // BrigOperandImmed b32 3
  curOpOffset += curOpOffset & 0x7;
  bitsInst.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(3, getImm.bits.u);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase extract
  EXPECT_EQ(extrInst.size, getBase.size);
  EXPECT_EQ(extrInst.kind, getBase.kind);
  EXPECT_EQ(extrInst.opcode, getBase.opcode);
  EXPECT_EQ(extrInst.type, getBase.type);
  EXPECT_EQ(extrInst.packing, getBase.packing);
  EXPECT_EQ(extrInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(extrInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(extrInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(extrInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(extrInst.o_operands[4], getBase.o_operands[4]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase insert
  EXPECT_EQ(inseInst.size, getBase.size);
  EXPECT_EQ(inseInst.kind, getBase.kind);
  EXPECT_EQ(inseInst.opcode, getBase.opcode);
  EXPECT_EQ(inseInst.type, getBase.type);
  EXPECT_EQ(inseInst.packing, getBase.packing);
  EXPECT_EQ(inseInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(inseInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(inseInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(inseInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(inseInst.o_operands[4], getBase.o_operands[4]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase bitselect
  EXPECT_EQ(bitsInst.size, getBase.size);
  EXPECT_EQ(bitsInst.kind, getBase.kind);
  EXPECT_EQ(bitsInst.opcode, getBase.opcode);
  EXPECT_EQ(bitsInst.type, getBase.type);
  EXPECT_EQ(bitsInst.packing, getBase.packing);
  EXPECT_EQ(bitsInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(bitsInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(bitsInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(bitsInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(bitsInst.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}


TEST(CodegenTest,  Instruction4_MultiMediaOperation_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  // Note: Bit String Operation without ftz and rounding.


  BrigInstBase bitAlignInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigBitAlign,         // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase byteAlignInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigByteAlign,         // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase sadInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigSad4Hi,           // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase lerpInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigLerp,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };


  std::string input("bitalign_b32 $s5, $s0, $s1, $s2;\n");
  input.append("bytealign_b32 $s5, $s0, $s1, $s2;\n");
  input.append("sad4hi_b32 $s5, $s0, $s1, $s6;\n");
  input.append("lerp_b32 $s5, $s0, $s1, $s2;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction4MultiMediaOperationPart1(context));
  EXPECT_EQ(0, Instruction4MultiMediaOperationPart1(context));
  EXPECT_EQ(0, Instruction4MultiMediaOperationPart1(context));
  EXPECT_EQ(0, Instruction4MultiMediaOperationPart1(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;

  BrigOperandReg getReg;
  BrigInstBase getBase;

  // BrigOperandReg S5
  byteAlignInst.o_operands[0] = curOpOffset;
  bitAlignInst.o_operands[0] = curOpOffset;
  lerpInst.o_operands[0] = curOpOffset;
  sadInst.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);

  // BrigOperandReg S0
  byteAlignInst.o_operands[1] = curOpOffset;
  bitAlignInst.o_operands[1] = curOpOffset;
  lerpInst.o_operands[1] = curOpOffset;
  sadInst.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(12, getReg.name);

  // BrigOperandReg S1
  byteAlignInst.o_operands[2] = curOpOffset;
  bitAlignInst.o_operands[2] = curOpOffset;
  lerpInst.o_operands[2] = curOpOffset;
  sadInst.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(16, getReg.name);

  // BrigOperandReg S2
  byteAlignInst.o_operands[3] = curOpOffset;
  bitAlignInst.o_operands[3] = curOpOffset;
  lerpInst.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(20, getReg.name);

  // BrigOperandReg S6
  sadInst.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(24, getReg.name);


  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase bitalign
  EXPECT_EQ(bitAlignInst.size, getBase.size);
  EXPECT_EQ(bitAlignInst.kind, getBase.kind);
  EXPECT_EQ(bitAlignInst.opcode, getBase.opcode);
  EXPECT_EQ(bitAlignInst.type, getBase.type);
  EXPECT_EQ(bitAlignInst.packing, getBase.packing);
  EXPECT_EQ(bitAlignInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(bitAlignInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(bitAlignInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(bitAlignInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(bitAlignInst.o_operands[4], getBase.o_operands[4]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase bytealign
  EXPECT_EQ(byteAlignInst.size, getBase.size);
  EXPECT_EQ(byteAlignInst.kind, getBase.kind);
  EXPECT_EQ(byteAlignInst.opcode, getBase.opcode);
  EXPECT_EQ(byteAlignInst.type, getBase.type);
  EXPECT_EQ(byteAlignInst.packing, getBase.packing);
  EXPECT_EQ(byteAlignInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(byteAlignInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(byteAlignInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(byteAlignInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(byteAlignInst.o_operands[4], getBase.o_operands[4]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase sad4hi
  EXPECT_EQ(sadInst.size, getBase.size);
  EXPECT_EQ(sadInst.kind, getBase.kind);
  EXPECT_EQ(sadInst.opcode, getBase.opcode);
  EXPECT_EQ(sadInst.type, getBase.type);
  EXPECT_EQ(sadInst.packing, getBase.packing);
  EXPECT_EQ(sadInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(sadInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(sadInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(sadInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(sadInst.o_operands[4], getBase.o_operands[4]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase lerp
  EXPECT_EQ(lerpInst.size, getBase.size);
  EXPECT_EQ(lerpInst.kind, getBase.kind);
  EXPECT_EQ(lerpInst.opcode, getBase.opcode);
  EXPECT_EQ(lerpInst.type, getBase.type);
  EXPECT_EQ(lerpInst.packing, getBase.packing);
  EXPECT_EQ(lerpInst.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(lerpInst.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(lerpInst.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(lerpInst.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(lerpInst.o_operands[4], getBase.o_operands[4]);

  delete lexer;
}

TEST(CodegenTest,ExtensionCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("extension \"\\device\\amd.hsa\";");

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  size_t str_len = strlen("\"\\device\\amd.hsa\"") + 1;
  EXPECT_EQ(0,Extension(context));

  BrigDirectiveExtension ref = {
    sizeof(BrigDirectiveExtension),
    BrigEDirectiveExtension,
    context->get_code_offset(),
    context->get_string_offset() - str_len
  };
  BrigDirectiveExtension get;
  BrigdOffset32_t d_offset = context->get_directive_offset()
           - sizeof(BrigDirectiveExtension);
  context->get_directive(d_offset,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.s_name,get.s_name);

  delete lexer;
}

TEST(CodegenTest,PragmaCodegen){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("pragma \"once\";");

  Lexer *lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0,Pragma(context));

  BrigDirectivePragma ref = {
    sizeof(BrigDirectivePragma),
    BrigEDirectivePragma,
    context->get_code_offset(),
    context->get_string_offset() - (strlen("\"once\"") + 1)
  };
  BrigDirectivePragma get;
  BrigdOffset32_t d_offset = context->get_directive_offset()
                       -sizeof(BrigDirectivePragma);
  context->get_directive(d_offset,&get);

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.s_name,get.s_name);

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

  std::string input("signature &test(arg_u32)(arg_f32) :fbar(2) ;");
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
  ref->fbarCount = 2;
  ref->reserved = 0;
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
  EXPECT_EQ(ref->fbarCount,get->fbarCount);
  EXPECT_EQ(ref->reserved,get->reserved);
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

TEST(CodegenTest, MulCodeGen) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("mul_u32 $s1 ,$s2, $s3;\n");
  input.append("mul_hi_u32 $s1, $s2, $s9;\n");

  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Mul(context));
  EXPECT_EQ(0, Mul(context));

  BrigcOffset32_t ref2_c_offset = context->get_code_offset() - sizeof(BrigInstBase);
  BrigcOffset32_t ref1_c_offset = ref2_c_offset - sizeof(BrigInstBase);

  BrigoOffset32_t reg9_o_offset = context->get_operand_offset() - sizeof(BrigOperandReg);
  BrigoOffset32_t reg3_o_offset = reg9_o_offset - sizeof(BrigOperandReg);
  BrigoOffset32_t reg2_o_offset = reg3_o_offset - sizeof(BrigOperandReg);
  BrigoOffset32_t reg1_o_offset = reg2_o_offset - sizeof(BrigOperandReg);

  BrigInstBase ref1 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigMul,               // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {                      // o_operands[5]
      reg1_o_offset,
      reg2_o_offset,
      reg3_o_offset,
      0,
      0
    }
  };
  BrigInstBase ref2 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigMul,               // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {                      // o_operands[5]
      reg1_o_offset,
      reg2_o_offset,
      reg9_o_offset,
      0,
      0
    }
  };
  BrigsOffset32_t reg9_s_offset = context->get_string_offset()
          - (strlen("$s9") + 1);
  BrigsOffset32_t reg3_s_offset = reg9_s_offset - (strlen("$s3") + 1);
  BrigsOffset32_t reg2_s_offset = reg3_s_offset - (strlen("$s2") + 1);
  BrigsOffset32_t reg1_s_offset = reg2_s_offset - (strlen("$s1") + 1);

  BrigOperandReg reg1 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    reg1_s_offset
  };

  BrigOperandReg reg2 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    reg2_s_offset
  };
  BrigOperandReg reg3 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    reg3_s_offset
  };

  BrigOperandReg reg9 = {
    sizeof(BrigOperandReg),
    BrigEOperandReg,
    Brigb32,
    0,
    reg9_s_offset
  };
  BrigInstBase getBase;
  BrigOperandReg getReg;

  context->get_operand(reg9_o_offset, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg9.size, getReg.size);
  EXPECT_EQ(reg9.kind, getReg.kind);
  EXPECT_EQ(reg9.type, getReg.type);
  EXPECT_EQ(reg9.reserved, getReg.reserved);
  EXPECT_EQ(reg9.name, getReg.name);

  context->get_operand(reg3_o_offset, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg3.size, getReg.size);
  EXPECT_EQ(reg3.kind, getReg.kind);
  EXPECT_EQ(reg3.type, getReg.type);
  EXPECT_EQ(reg3.reserved, getReg.reserved);
  EXPECT_EQ(reg3.name, getReg.name);

  context->get_operand(reg2_o_offset, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg2.size, getReg.size);
  EXPECT_EQ(reg2.kind, getReg.kind);
  EXPECT_EQ(reg2.type, getReg.type);
  EXPECT_EQ(reg2.reserved, getReg.reserved);
  EXPECT_EQ(reg2.name, getReg.name);

  context->get_operand(reg1_o_offset, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg1.size, getReg.size);
  EXPECT_EQ(reg1.kind, getReg.kind);
  EXPECT_EQ(reg1.type, getReg.type);
  EXPECT_EQ(reg1.reserved, getReg.reserved);
  EXPECT_EQ(reg1.name, getReg.name);

  context->get_code(ref1_c_offset, &getBase);
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

  context->get_code(ref2_c_offset, &getBase);
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
}

TEST(CodegenTest, Call_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigInstBase callInst1 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigCall,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstMod callInst2 = {
    sizeof(BrigInstMod),  // size
    BrigEInstMod,         // kind
    BrigCall,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    {0, 0, 0, 0, 0, 1, 0}  // aluModifier
  };

  BrigInstBase callInst3 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigCall,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase callInst4 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigCall,              // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };


  std::string input("call_width(64) $s1 (%in) [&foo, &bar];\n");
  input.append("call_width(all)_fbar $s1 (%out)(%in) &sigFunc;\n");
  input.append("call &foo(%out)(%in);\n");
  input.append("call &bar(%in);\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  context->func_map["&foo"] = 0xf7;
  context->func_map["&bar"] = 0xf8;
  context->symbol_map["%in"] = 0xf9;
  context->symbol_map["%out"] = 0xfa;

  EXPECT_EQ(0, Call(context));
  EXPECT_EQ(0, Call(context));
  EXPECT_EQ(0, Call(context));
  EXPECT_EQ(0, Call(context));

  BrigoOffset32_t curOpOffset = 8;
  BrigcOffset32_t curCodeOffset = 8;
  BrigoOffset32_t inOpRefOffset = 0;
  BrigoOffset32_t outOpRefOffset = 0;
  BrigoOffset32_t fooOpRefOffset = 0;
  BrigoOffset32_t barOpRefOffset = 0;
  BrigoOffset32_t sigOpRefOffset = 0;

  BrigOperandReg getReg;
  BrigInstBase getBase;
  BrigInstMod getMod;
  BrigOperandArgumentList getArgList;
  BrigOperandFunctionRef getFunRef;
  BrigOperandArgumentRef getArgRef;
  BrigOperandImmed getImm;

  // BrigOperandImmed b32 64

  curOpOffset += curOpOffset & 0x7;
  callInst1.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(64, getImm.bits.u);

  callInst1.o_operands[1] = curOpOffset;
  
  // BrigOperandReg S1
  callInst1.o_operands[2] = curOpOffset;
  callInst2.o_operands[2] = curOpOffset;

  context->get_operand(curOpOffset, &getReg);
  curOpOffset += sizeof(BrigOperandReg);

  EXPECT_EQ(sizeof(BrigOperandReg), getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(8, getReg.name);
  // BrigOperandArgumentRef input Argument %in

  inOpRefOffset = curOpOffset;
  context->get_operand(curOpOffset, &getArgRef);
  curOpOffset += sizeof(BrigOperandArgumentRef);

  EXPECT_EQ(sizeof(BrigOperandArgumentRef), getArgRef.size);
  EXPECT_EQ(BrigEOperandArgumentRef, getArgRef.kind);
  EXPECT_EQ(0xf9, getArgRef.arg);

  // BrigOperandArgumentList input Argument List
  callInst1.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);
  callInst1.o_operands[1] = curOpOffset;
  
  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(inOpRefOffset, getArgList.o_args[0]);

  // BrigOperandFunctionRef func &foo Argument

  
  curOpOffset += sizeof(BrigOperandArgumentList);
  callInst3.o_operands[2] = curOpOffset;
  fooOpRefOffset = curOpOffset;
  context->get_operand(curOpOffset, &getFunRef);
  curOpOffset += sizeof(BrigOperandFunctionRef);

  EXPECT_EQ(sizeof(BrigOperandFunctionRef), getFunRef.size);
  EXPECT_EQ(BrigEOperandFunctionRef, getFunRef.kind);
  EXPECT_EQ(0xf7, getFunRef.fn);

  // BrigOperandFunctionRef func &bar Argument

  barOpRefOffset = curOpOffset;
  callInst4.o_operands[2] = curOpOffset;
  context->get_operand(curOpOffset, &getFunRef);
  curOpOffset += sizeof(BrigOperandFunctionRef);

  EXPECT_EQ(sizeof(BrigOperandFunctionRef), getFunRef.size);
  EXPECT_EQ(BrigEOperandFunctionRef, getFunRef.kind);
  EXPECT_EQ(0xf8, getFunRef.fn);


  // BrigOperandArgumentList func List
  callInst1.o_operands[4] = curOpOffset;

  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);
  BrigoOffset32_t o_args_1 = 0;
  context->get_operand(curOpOffset, &o_args_1);
  curOpOffset += sizeof(BrigoOffset32_t);

  EXPECT_EQ(sizeof(BrigOperandArgumentList) + sizeof(BrigoOffset32_t), getArgList.size);
  EXPECT_EQ(BrigEOperandFunctionList, getArgList.kind);
  EXPECT_EQ(2, getArgList.elementCount);
  EXPECT_EQ(fooOpRefOffset, getArgList.o_args[0]);
  EXPECT_EQ(barOpRefOffset, o_args_1);



  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);
  
  // BrigInstBase Call 1
  EXPECT_EQ(callInst1.size, getBase.size);
  EXPECT_EQ(callInst1.kind, getBase.kind);
  EXPECT_EQ(callInst1.opcode, getBase.opcode);
  EXPECT_EQ(callInst1.type, getBase.type);
  EXPECT_EQ(callInst1.packing, getBase.packing);
  EXPECT_EQ(callInst1.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(callInst1.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(callInst1.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(callInst1.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(callInst1.o_operands[4], getBase.o_operands[4]);

  // BrigOperandArgumentRef output Argument %out
  outOpRefOffset = curOpOffset;
  context->get_operand(curOpOffset, &getArgRef);
  curOpOffset += sizeof(BrigOperandArgumentRef);

  EXPECT_EQ(sizeof(BrigOperandArgumentRef), getArgRef.size);
  EXPECT_EQ(BrigEOperandArgumentRef, getArgRef.kind);
  EXPECT_EQ(0xfa, getArgRef.arg);

  // BrigOperandArgumentList output Argument List
  callInst2.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(outOpRefOffset, getArgList.o_args[0]);

  // BrigOperandArgumentList input Argument List
  callInst2.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(inOpRefOffset, getArgList.o_args[0]);

  // BrigOperandArgumentRef signature function &signFun
  sigOpRefOffset = curOpOffset;
  context->get_operand(curOpOffset, &getArgRef);
  curOpOffset += sizeof(BrigOperandArgumentRef);

  EXPECT_EQ(sizeof(BrigOperandArgumentRef), getArgRef.size);
  EXPECT_EQ(BrigEOperandArgumentRef, getArgRef.kind);
  EXPECT_EQ(0, getArgRef.arg);

  // BrigOperandArgumentList &sigFun signature
  callInst2.o_operands[4] = curOpOffset;

  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandFunctionList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(sigOpRefOffset, getArgList.o_args[0]);

  context->get_code(curCodeOffset, &getMod);
  curCodeOffset += sizeof(BrigInstMod);

  // BrigInstBase Call 2
  EXPECT_EQ(callInst2.size, getMod.size);
  EXPECT_EQ(callInst2.kind, getMod.kind);
  EXPECT_EQ(callInst2.opcode, getMod.opcode);
  EXPECT_EQ(callInst2.type, getMod.type);
  EXPECT_EQ(callInst2.packing, getMod.packing);
  EXPECT_EQ(callInst2.o_operands[0], getMod.o_operands[0]);
  EXPECT_EQ(callInst2.o_operands[1], getMod.o_operands[1]);
  EXPECT_EQ(callInst2.o_operands[2], getMod.o_operands[2]);
  EXPECT_EQ(callInst2.o_operands[3], getMod.o_operands[3]);
  EXPECT_EQ(callInst2.o_operands[4], getMod.o_operands[4]);

  unsigned int *pAluModRef = reinterpret_cast<unsigned int*>(&callInst2.aluModifier);
  unsigned int *pAluModGet = reinterpret_cast<unsigned int*>(&getMod.aluModifier);

  EXPECT_EQ(*pAluModRef, *pAluModGet);

  // BrigOperandImmed b32 0

  curOpOffset += curOpOffset & 0x7;
  callInst3.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  callInst3.o_operands[4] = 0;

  // BrigOperandArgumentList output Argument List
  callInst3.o_operands[1] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(outOpRefOffset, getArgList.o_args[0]);


  // BrigOperandArgumentList input Argument List
  callInst3.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);

  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(inOpRefOffset, getArgList.o_args[0]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase Call 3
  EXPECT_EQ(callInst3.size, getBase.size);
  EXPECT_EQ(callInst3.kind, getBase.kind);
  EXPECT_EQ(callInst3.opcode, getBase.opcode);
  EXPECT_EQ(callInst3.type, getBase.type);
  EXPECT_EQ(callInst3.packing, getBase.packing);
  EXPECT_EQ(callInst3.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(callInst3.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(callInst3.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(callInst3.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(callInst3.o_operands[4], getBase.o_operands[4]);

  // BrigOperandImmed b32 0

  curOpOffset += curOpOffset & 0x7;
  callInst4.o_operands[0] = curOpOffset;
  context->get_operand(curOpOffset, &getImm);
  curOpOffset += sizeof(BrigOperandImmed);

  EXPECT_EQ(sizeof(BrigOperandImmed), getImm.size);
  EXPECT_EQ(BrigEOperandImmed, getImm.kind);
  EXPECT_EQ(Brigb32, getImm.type);
  EXPECT_EQ(0, getImm.reserved);
  EXPECT_EQ(0, getImm.bits.u);

  
  callInst4.o_operands[4] = 0;

  // BrigOperandArgumentList input Argument List
  callInst4.o_operands[3] = curOpOffset;
  context->get_operand(curOpOffset, &getArgList);
  curOpOffset += sizeof(BrigOperandArgumentList);
  callInst4.o_operands[1] = curOpOffset;
  
  EXPECT_EQ(sizeof(BrigOperandArgumentList), getArgList.size);
  EXPECT_EQ(BrigEOperandArgumentList, getArgList.kind);
  EXPECT_EQ(1, getArgList.elementCount);
  EXPECT_EQ(inOpRefOffset, getArgList.o_args[0]);

  context->get_code(curCodeOffset, &getBase);
  curCodeOffset += sizeof(BrigInstBase);

  // BrigInstBase Call 4
  EXPECT_EQ(callInst4.size, getBase.size);
  EXPECT_EQ(callInst4.kind, getBase.kind);
  EXPECT_EQ(callInst4.opcode, getBase.opcode);
  EXPECT_EQ(callInst4.type, getBase.type);
  EXPECT_EQ(callInst4.packing, getBase.packing);
  EXPECT_EQ(callInst4.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(callInst4.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(callInst4.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(callInst4.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(callInst4.o_operands[4], getBase.o_operands[4]);


  delete lexer;
}


TEST(CodegenTest, Kernel_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("kernel &_kernel( \n");
  input.append("kernarg_u32 %arg0, \n");
  input.append("kernarg_u32 %arg1):fbar(2) \n");
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

TEST(CodegenTest, Instruction0_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("nop;\n");

  Lexer* lexer = new Lexer(input);

  BrigInstBase ref = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigNop,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  BrigInstBase get;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction0(context));

  context->get_code(8, &get);
  // BrigInstBase Nop
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

TEST(CodegenTest, WAVESIZE_CodeGen_SimpleTest) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("11\n");
  input.append("WAVESIZE\n");
  input.append("WAVESIZE\n");

  Lexer* lexer = new Lexer(input);

  BrigOperandWaveSz get;

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, BaseOperand(context));
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, BaseOperand(context));
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, BaseOperand(context));
  EXPECT_EQ(36, context->get_operand_offset());

  EXPECT_EQ(32, context->operand_map["WAVESIZE"]);
  context->get_operand(context->operand_map["WAVESIZE"], &get);

  EXPECT_EQ(4, get.size);
  EXPECT_EQ(BrigEOperandWaveSz, get.kind);


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
  EXPECT_EQ(calleeFunRef.fbarCount, funGet.fbarCount);
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
  EXPECT_EQ(callerFunRef.fbarCount, funGet.fbarCount);
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
  EXPECT_EQ(s0Reg.name, regGet.name);

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
  EXPECT_EQ(s1Reg.name, regGet.name);

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
