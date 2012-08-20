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
  EXPECT_EQ(144, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(32, csize);

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
  0,                         // c_code
  3,                        // storag class kernarg
  BrigNone ,                // attribut
  0,                        // reserved
  0,                        // symbolModifier
  0,                        // dim
  6,                        // s_name
  Brigf32,                  // type
  1,                        // align
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
  0,                         // c_code
  2,                        // storag class kernarg
  BrigNone ,                // attribut
  0,                        // reserved
  0,                        // symbolModifier
  0,                        // dim
  9,                        // s_name
  Brigu32,                  // type
  1,                        // align
  0,                        // d_init
  0,                         // reserved
  };

  context->get_directive(104, &get_sym);
  EXPECT_EQ(private_var.size, get_sym.size);
  EXPECT_EQ(private_var.kind, get_sym.kind);
  EXPECT_EQ(private_var.s.storageClass, get_sym.s.storageClass);
  EXPECT_EQ(private_var.s.s_name, get_sym.s.s_name);

  delete lexer;
};

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
  EXPECT_EQ(320, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(56+4, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(61, ssize);

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
  EXPECT_EQ(32, arg_scope.c_code);

  context->get_directive(312, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(24, cbr_op.o_operands[2]);
  EXPECT_EQ(44, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(24, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(192+40, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(232+40, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(32, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16, arg_l.o_args[0]);

  context->get_operand(44, &arg_l);
  EXPECT_EQ(16, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8, arg_l.o_args[0]);

  BrigoOffset32_t arg_test = 0;
  context->get_operand(56, &arg_test);
  EXPECT_EQ(8, arg_test);

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
  EXPECT_EQ(276+4, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(56, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(52, ssize);

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
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32, arg_scope.c_code);

  context->get_directive(272, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(24, cbr_op.o_operands[2]);
  EXPECT_EQ(44, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(24, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context->get_operand(8, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(188+4, arg_o.arg);

  context->get_operand(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(228+4, arg_o.arg);

  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context->get_operand(32, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16, arg_l.o_args[0]);

  context->get_operand(44, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8, arg_l.o_args[0]);

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
  EXPECT_EQ(116, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(16, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(16, ssize);

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
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32, arg_scope.c_code);

  context->get_directive(108, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(0, cbr_op.o_operands[1]);
  EXPECT_EQ(8, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand(8, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

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
  EXPECT_EQ(124+4, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(164, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(84, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(51, ssize);

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
  EXPECT_EQ(12, label1.size);
  EXPECT_EQ(100, label1.c_code);
  EXPECT_EQ(27, label1.s_name);

  context->get_directive(116, &label1);
  EXPECT_EQ(132, label1.c_code);
  EXPECT_EQ(41, label1.s_name);

  // test BrigCbr
  BrigInstBase cbr_op;
  context->get_code(0, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCbr, cbr_op.opcode);
  EXPECT_EQ(Brigb1, cbr_op.type);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(8, cbr_op.o_operands[1]);
  EXPECT_EQ(44, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigBrn
  BrigInstBar br_op;
  context->get_code(64, &br_op);
  EXPECT_EQ(36, br_op.size);
  EXPECT_EQ(BrigBrn, br_op.opcode);
  EXPECT_EQ(0, br_op.o_operands[0]);
  EXPECT_EQ(76, br_op.o_operands[1]);
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
  EXPECT_EQ(100+4, dsize);

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
  EXPECT_EQ(31, size);

  // test .code section.
  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(64, csize);

  BrigInstBase get_c;
  context->get_code(32, &get_c);
  EXPECT_EQ(BrigAdd, get_c.opcode);
  EXPECT_EQ(BrigPackPPsat, get_c.packing);
  EXPECT_EQ(Brigu16x2, get_c.type);
  EXPECT_EQ(8, get_c.o_operands[0]);
  EXPECT_EQ(32, get_c.o_operands[1]);
  EXPECT_EQ(44, get_c.o_operands[2]);

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
  EXPECT_EQ(100+4, dsize);

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
  EXPECT_EQ(22, size);

  // find the string.
  std::string func_name("&return_true");
  int str_offset = context->lookup_symbol(func_name);
  EXPECT_EQ(0, str_offset);

  BrigcOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(32, csize);

  delete lexer;
}

TEST(CodegenTest, AlignmentCheck) {
  // Try the situation in PRM 20.2 (pg. 226)

  // use a new context object to ensure the problem happen
  // since if at beginning the offset is a multiple of 4 but not a multiple of 8
  // then appending a 4-byte aligned item will lead to a multiple-of-8 offset
  context->clear_context();

  // First append a 4-byte aligned item BrigBlockStart
  uint32_t old_offset;
  uint32_t curr_offset = context->get_directive_offset();

  BrigBlockStart bbs = {
    12,                        // size
    BrigEDirectiveBlockStart,  // kind
    0,                         // c_code
    0                          // s_name;
  };

  context->append_directive(&bbs);    // append_directiveirective
  old_offset = curr_offset;
  curr_offset = context->get_directive_offset();

  EXPECT_EQ(0, curr_offset%4);
  EXPECT_EQ(BrigEAlignment_4, Context::alignment_check(bbs));

  // Next append a 8-byte aligned item  such as BrigBlockNumeric
  BrigBlockNumeric bbn = {
    16,                          // size
    BrigEDirectiveBlockNumeric,  // kind
    Brigb64,                     // type
    1,                           // elementCount
    1,                           // u64
  };

  context->append_directive(&bbn);
  old_offset = curr_offset;
  curr_offset = context->get_directive_offset();

  EXPECT_EQ(BrigEAlignment_8, Context::alignment_check(bbn));
  // this is a 8-byte aligned item and has a size of multiple of 8.
  // so the offset after appending this item should be a multiple of 8.
  EXPECT_EQ(0, curr_offset%8);
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
    0                // reserved
  };

  name.assign("$d7");
  ref.name = context->lookup_symbol(name);
  EXPECT_EQ(0, ref.name);

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
    0                   // reserved
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
  int loc = strBuf->lookup(input);
  EXPECT_EQ(0, loc);

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
  int lookup_sym2 = context->lookup_symbol(symbol);

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
  int loc = strBuf->lookup(input);
  EXPECT_EQ(offset, loc);

  delete strBuf;
};

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
};


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

}  // namespace brig
}  // namespace hsa
