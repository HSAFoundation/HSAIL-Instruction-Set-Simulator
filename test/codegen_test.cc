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
#include "gmock/gmock.h"
#include "mock_error_reporter.h"

using ::testing::_;
namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;


TEST(CodegenTest, CallwMultiArgs) {
  main_reporter->show_error(true);
  // test error reporter
  Context* context1 = new Context(main_reporter);
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,   // c_code
    32,   // s_name
    0,   // inParamCount
    208,  // d_firstScopedDirective
    1,   // operationCount
    296,  // d_nextDirective
    BrigNone,
    0,
    0,   // outParamCount
    0,
  };

  std::string input("version 1:0:$small;");

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context1));

  input.assign("function &callee(arg_f32 %output)(arg_f32 %input1, arg_f32 %input2) {ret;};\n");
  input.append("function &caller()(){{arg_f32 %an_input; arg_f32 %an_output ; call &callee(%an_output)(%an_input, %an_input); }};");
  //
  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Function(yylex(), context1));
  EXPECT_EQ(0, Function(yylex(), context1));

  // test the sizes of each section
  BrigdOffset32_t dsize = context1->get_directive_offset();
  EXPECT_EQ(296, dsize);
  BrigdOffset32_t csize = context1->get_code_offset();
  EXPECT_EQ(64, csize);
  BrigdOffset32_t osize = context1->get_operand_offset();
  EXPECT_EQ(56+4, osize);
  BrigdOffset32_t ssize = context1->get_string_offset();
  EXPECT_EQ(61, ssize);

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction get;
  context1->get_directive<BrigDirectiveFunction>(
              context1->current_bdf_offset,
              &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context1->get_directive<BrigDirectiveScope>(208, &arg_scope);
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32, arg_scope.c_code);

  context1->get_directive<BrigDirectiveScope>(288, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context1->get_code<BrigInstBase>(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(24, cbr_op.o_operands[2]);
  EXPECT_EQ(44, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context1->get_operand<BrigOperandFunctionRef>(24, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context1->get_operand<BrigOperandArgumentRef>(8, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(180+36, arg_o.arg);

  context1->get_operand<BrigOperandArgumentRef>(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(216+36, arg_o.arg);


  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context1->get_operand<BrigOperandArgumentList>(32, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16, arg_l.o_args[0]);

  context1->get_operand<BrigOperandArgumentList>(44, &arg_l);
  EXPECT_EQ(16, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8, arg_l.o_args[0]);

  BrigoOffset32_t arg_test = 0;
  context1->get_operand<BrigoOffset32_t>(56, &arg_test);
  EXPECT_EQ(8, arg_test);

  main_reporter->show_error(false);
  delete context1;
}

TEST(CodegenTest, Example6_CallwArgs) {
  main_reporter->show_error(true);
  // test error reporter
  Context* context1 = new Context(main_reporter);
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,   // c_code
    23,   // s_name
    0,   // inParamCount
    172,  // d_firstScopedDirective
    1,   // operationCount
    260,  // d_nextDirective
    BrigNone,
    0,
    0,   // outParamCount
    0,
  };

  std::string input("version 1:0:$small;");

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context1));

  input.assign("function &callee(arg_f32 %output)(arg_f32 %input) {ret;};\n");
  input.append("function &caller()(){{arg_f32 %an_input; arg_f32 %an_output ; call &callee(%an_output)(%an_input); }};");

  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Function(yylex(), context1));
  EXPECT_EQ(0, Function(yylex(), context1));

  // test the sizes of each section
  BrigdOffset32_t dsize = context1->get_directive_offset();
  EXPECT_EQ(260, dsize);
  BrigdOffset32_t csize = context1->get_code_offset();
  EXPECT_EQ(64, csize);
  BrigdOffset32_t osize = context1->get_operand_offset();
  EXPECT_EQ(56, osize);
  BrigdOffset32_t ssize = context1->get_string_offset();
  EXPECT_EQ(52, ssize);

  // test BrigDirectiveFunction, the caller function
  BrigDirectiveFunction get;
  context1->get_directive<BrigDirectiveFunction>(
              context1->current_bdf_offset,
              &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context1->get_directive<BrigDirectiveScope>(172, &arg_scope);
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32, arg_scope.c_code);

  context1->get_directive<BrigDirectiveScope>(252, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context1->get_code<BrigInstBase>(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(32, cbr_op.o_operands[1]);
  EXPECT_EQ(24, cbr_op.o_operands[2]);
  EXPECT_EQ(44, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context1->get_operand<BrigOperandFunctionRef>(24, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

  // test BrigOperandArgumentRef
  BrigOperandArgumentRef arg_o;
  context1->get_operand<BrigOperandArgumentRef>(8, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(180, arg_o.arg);

  context1->get_operand<BrigOperandArgumentRef>(16, &arg_o);
  EXPECT_EQ(8, arg_o.size);
  EXPECT_EQ(BrigEOperandArgumentRef, arg_o.kind);
  EXPECT_EQ(216, arg_o.arg);


  // test BrigOperandArgumentList
  BrigOperandArgumentList arg_l;
  context1->get_operand<BrigOperandArgumentList>(32, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(16, arg_l.o_args[0]);

  context1->get_operand<BrigOperandArgumentList>(44, &arg_l);
  EXPECT_EQ(12, arg_l.size);
  EXPECT_EQ(BrigEOperandArgumentList, arg_l.kind);
  EXPECT_EQ(8, arg_l.o_args[0]);

  main_reporter->show_error(false);
  delete context1;
}


TEST(CodegenTest, Example5_SimpleCall) {

  context->set_error_reporter(main_reporter);

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    32,   // c_code
    8,   // s_name
    0,   // inParamCount
    100,  // d_firstScopedDirective
    1,   // operationCount
    116,  // d_nextDirective
    BrigNone,
    0,
    0,   // outParamCount
    0,
  };

  std::string input("version 1:0:$small; \n");
  input.append("function &callee()(){ret;}; \n");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->clear_context();
  context->token_to_scan= yylex();

  EXPECT_EQ(0, Version(context));

  printf("Directive offset after Version: %d\n", context->get_directive_offset());
  EXPECT_EQ(0, Function(context));

  input.assign("function &caller()(){{call &callee; }};");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();

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
  BrigDirectiveFunction get;
  context->get_directive<BrigDirectiveFunction>(
              context->current_bdf_offset,
              &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveScope
  BrigDirectiveScope arg_scope;
  context->get_directive<BrigDirectiveScope>(100, &arg_scope);
  EXPECT_EQ(8, arg_scope.size);
  EXPECT_EQ(BrigEDirectiveArgStart, arg_scope.kind);
  EXPECT_EQ(32, arg_scope.c_code);

  context->get_directive<BrigDirectiveScope>(108, &arg_scope);
  EXPECT_EQ(BrigEDirectiveArgEnd, arg_scope.kind);
  EXPECT_EQ(64, arg_scope.c_code);

  // test BrigCall
  BrigInstBase cbr_op;
  context->get_code<BrigInstBase>(32, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCall, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(0, cbr_op.o_operands[1]);
  EXPECT_EQ(8, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigOperandFunctionRef
  BrigOperandFunctionRef func_o;
  context->get_operand<BrigOperandFunctionRef>(8, &func_o);
  EXPECT_EQ(8, func_o.size);
  EXPECT_EQ(BrigEOperandFunctionRef, func_o.kind);
  EXPECT_EQ(20, func_o.fn);

}

TEST(CodegenTest, Example4_Branch) {
  context->clear_context();
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    0,                        // c_code
    0,                        // s_name
    0,                        // inParamCount
    96,                       // d_firstScopedDirective
    5,                        // operationCount
    120,                      // d_nextDirective
    BrigNone,
    0,
    1,                        // outParamCount
    0,
  };
  std::string input("version 1:0:$small;\n");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Version(context));
  input.assign("function &branch_ops (arg_u8x4 %x)() \n");
  input.append("{cbr $c1, @then; \n");
  input.append("abs_p_s8x4 $s1, $s2; \n");
  input.append("brn @outof_IF; \n");
  input.append("@then: \n");
  input.append("add_pp_sat_u16x2 $s1, $s0, $s3; \n");
  input.append("@outof_IF: ret;};");

  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Function(context));

  // test the sizes of each section
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(120, dsize);
  BrigdOffset32_t csize = context->get_code_offset();
  EXPECT_EQ(164, csize);
  BrigdOffset32_t osize = context->get_operand_offset();
  EXPECT_EQ(84, osize);
  BrigdOffset32_t ssize = context->get_string_offset();
  EXPECT_EQ(51, ssize);

  // test BrigDirectiveFunction
  BrigDirectiveFunction get;
  context->get_directive<BrigDirectiveFunction>(
              context->current_bdf_offset,
              &get);
  EXPECT_EQ(ref.s_name, get.s_name);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.outParamCount, get.outParamCount);
  EXPECT_EQ(ref.inParamCount, get.inParamCount);
  EXPECT_EQ(ref.operationCount, get.operationCount);
  EXPECT_EQ(ref.d_nextDirective, get.d_nextDirective);
  EXPECT_EQ(ref.d_firstScopedDirective, get.d_firstScopedDirective);

  // test BrigDirectiveLabel
  BrigDirectiveLabel label1;
  context->get_directive<BrigDirectiveLabel>(96, &label1);
  EXPECT_EQ(12, label1.size);
  EXPECT_EQ(100, label1.c_code);
  EXPECT_EQ(27, label1.s_name);

  context->get_directive<BrigDirectiveLabel>(108, &label1);
  EXPECT_EQ(132, label1.c_code);
  EXPECT_EQ(41, label1.s_name);

  // test BrigCbr
  BrigInstBase cbr_op;
  context->get_code<BrigInstBase>(0, &cbr_op);
  EXPECT_EQ(32, cbr_op.size);
  EXPECT_EQ(BrigCbr, cbr_op.opcode);
  EXPECT_EQ(0, cbr_op.o_operands[0]);
  EXPECT_EQ(8, cbr_op.o_operands[1]);
  EXPECT_EQ(44, cbr_op.o_operands[2]);
  EXPECT_EQ(0, cbr_op.o_operands[3]);
  EXPECT_EQ(0, cbr_op.o_operands[4]);

  // test BrigBrn
  BrigInstBar br_op;
  context->get_code<BrigInstBar>(64, &br_op);
  EXPECT_EQ(36, br_op.size);
  EXPECT_EQ(BrigBrn, br_op.opcode);
  EXPECT_EQ(0, br_op.o_operands[0]);
  EXPECT_EQ(76, br_op.o_operands[1]);
  EXPECT_EQ(0, br_op.o_operands[2]);
  EXPECT_EQ(0, br_op.o_operands[3]);
  EXPECT_EQ(0, br_op.o_operands[4]);
}

TEST(CodegenTest, Example3_CodeGen) {
  context->clear_context();

  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    0,   // c_code
    0,   // s_name
    0,   // inParamCount
    96,  // d_firstScopedDirective
    2,   // operationCount
    96,  // d_nextDirective
    BrigStatic,
    0,
    1,   // outParamCount
    0,
  };

  std::string input("version 1:0:$small;");

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Version(context));

  input.assign("static function &packed_ops (extern arg_u8x4 %x)()");
  input.append("{abs_p_s8x4 $s1, $s2;add_pp_sat_u16x2 $s1, $s0, $s3;};");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Function(context));

  // test the .directive section size
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(96, dsize);

  // test the .directive section
  BrigDirectiveFunction get;
  context->get_directive<BrigDirectiveFunction>(
              context->current_bdf_offset,
              &get);
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
  context->get_code<BrigInstBase>(
              32,
              &get_c);
  EXPECT_EQ(BrigAdd, get_c.opcode);
  EXPECT_EQ(BrigPackPPsat, get_c.packing);
  EXPECT_EQ(Brigu16x2, get_c.type);
  EXPECT_EQ(8, get_c.o_operands[0]);
  EXPECT_EQ(32, get_c.o_operands[1]);
  EXPECT_EQ(44, get_c.o_operands[2]);
}

TEST(CodegenTest, Instrustion3Op_CodeGen) {
  context->clear_context();
  BrigInstBase ref = {
    32,
    BrigEInstBase,
    BrigAdd,
    Brigu16x2,
    BrigPackPPsat,
    {8, 20, 32, 0, 0}
  };

  std::string input("add_pp_sat_u16x2 $s1, $s0, $s3;");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase get;
  context->get_code<BrigInstBase>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
}

TEST(CodegenTest, Instrustion2Op_CodeGen) {
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

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  context->get_code<BrigInstBase>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
}

TEST(CodegenTest, Instrustion2Op_FTZ_CodeGen) {
  context->clear_context();
  std::string input("sqrt_s8x4 $s1, $s2;");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
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
  context->get_code<BrigInstBase>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
}

TEST(CodegenTest, Instrustion2Op_FTZ_With_Modifier_CodeGen) {
  context->clear_context();
  std::string input("sqrt_ftz_s8x4 $s1, $s2;");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Instruction2(context));

  BrigAluModifier bam;
  bam.ftz = 1;

  BrigInstMod ref = {
    sizeof(ref),      // size
    BrigEInstMod,     // kind
    BrigSqrt,          // opcode
    Brigs8x4,         // type
    BrigNoPacking,        // packing
    {8, 20, 0, 0, 0},  // operand
    bam
  };

  BrigInstMod get;
  context->get_code<BrigInstMod>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);
}


TEST(CodegenTest, Instrustion2Op_with_Modifier_CodeGen) {
  context->clear_context();
  std::string input("abs_ftz_p_s8x4 $s1, $s2;");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Instruction2(context));

  BrigAluModifier bam;
  bam.ftz = 1;

  BrigInstMod ref = {
    sizeof(ref),      // size
    BrigEInstMod,     // kind
    BrigAbs,          // opcode
    Brigs8x4,         // type
    BrigPackP,        // packing
    {8, 20, 0, 0, 0},  // operand
    bam
  };

  BrigInstMod get;
  context->get_code<BrigInstMod>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);
}


TEST(CodegenTest, SimplestFunction_CodeGen) {
  context->clear_context();
  BrigDirectiveFunction ref = {
    40,                       // size
    BrigEDirectiveFunction,   // kind
    0,   // c_code
    0,   // s_name
    0,   // inParamCount
    96,  // d_firstScopedDirective
    1,   // operationCount
    96,  // d_nextDirective
    BrigNone,
    0,
    1,   // outParamCount
    0,
  };

  std::string input("version 1:0:$small;");

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Version(context));

  input.assign("function &return_true(arg_f32 %ret_val)(){ret;};");

  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Function(context));

  // test the .directive section size
  BrigdOffset32_t dsize = context->get_directive_offset();
  EXPECT_EQ(96, dsize);

  // test the offset to the .string section
  BrigDirectiveFunction get;
  context->get_directive<BrigDirectiveFunction>(
              context->current_bdf_offset,
              &get);
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
  // reference struct
  context->clear_context();
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

  std::string input("version 1:0;");
  ref.major = 1;
  ref.minor = 0;

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Version(context));

  // after append BrigDirectiveVersion
  uint32_t curr_d_offset = context->get_directive_offset();

  // get structure back
  BrigDirectiveVersion get;
  context->get_directive<BrigDirectiveVersion>(
                  curr_d_offset-sizeof(get),
                  &get);
  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

  context->clear_context();

  /* ---------- TEST 2 ---------*/
  input.assign("version 2:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
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

  context->clear_context();
      /* TEST 3, Multi Target */
  input.assign("version 2:0:$large, $reduced, $sftz;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Version(context));

  // reference struct
  ref.major = 2;
  ref.machine = BrigELarge;
  ref.profile = BrigEReduced;
  ref.ftz = BrigESftz;

  // get structure back
  curr_d_offset = context->get_directive_offset();
  context->get_directive<BrigDirectiveVersion>(curr_d_offset-sizeof(get), &get);

  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);
  context->clear_context();
}

TEST(CodegenTest, RegisterOperandCodeGen) {
  std::string name;
  std::string input("$d7");  // register


  Parser* parser = new Parser(context);
  parser->set_source_string(input);
  parser->clear_context();
  // scan symbols
  parser->scan_symbols();

  // rescan
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
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
  context->get_operand<BrigOperandReg>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);

  // second register
  input.assign("$q7");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));

  // scan symbols
  parser->scan_symbols();

  // rescan
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Operand(context));

  name.assign("$q7");
  ref.name = context->lookup_symbol(name);
  ref.type = Brigb128;
  curr_o_offset = context->get_operand_offset();
  context->get_operand<BrigOperandReg>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);
}

TEST(CodegenTest, NumericValueOperandCodeGen) {
  /* Integer */
  std::string input("5");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
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
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.u = -5;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  // to overcome padding
  context->get_operand<BrigOperandImmed>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  /* float single */
  input.assign("5.0f");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.f = 5;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  // to overcome padding
  context->get_operand<BrigOperandImmed>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.f, get.bits.f);

  /* double */
  input.assign("5.0l");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.bits.d = 5;
  ref.type = Brigb64;
  // get structure from context and compare
  curr_o_offset = context->get_operand_offset();
  // to overcome padding
  context->get_operand<BrigOperandImmed>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.d, get.bits.d);

  /* Integer List */
  input.assign("_b32(5,6,8)");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, Operand(context));

  // reference struct
  ref.type = Brigb32;
  ref.bits.u = 8;
  // get last structure from context and compare
  curr_o_offset = context->get_operand_offset();
  // to overcome padding
  context->get_operand<BrigOperandImmed>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
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
};

TEST(CodegenTest, BrigOperandAddressGeneration) {
  std::string name;
  std::string input("[&test]");  // [name]

  Parser* parser = new Parser(context);
  parser->set_source_string(input);

  // scan symbols
  parser->scan_symbols();

  // rescan
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->token_to_scan = yylex();
  EXPECT_EQ(0, AddressableOperand(context));

  name.assign(input.c_str());
  BrigOperandAddress ref = {
    sizeof(ref),          // size
    BrigEOperandAddress,  // kind
    Brigb32,              // Data Type
    0,                    // reserved
    // this should be the offset of the BrigDirectiveSymbol struct
    0,  // directive
    0,  // offset -> ??
  };

  if (context->get_machine() == BrigELarge)
    ref.type = Brigb64;

    // get structure from context and compare
  BrigOperandAddress get;
  uint32_t curr_o_offset = context->get_operand_offset();
  context->get_operand<BrigOperandAddress>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.directive, get.directive);
};

TEST(ErrorReportingTest, UseMockErrorReporter) {
  // reference struct
  MockErrorReporter mer;

  // forward call to FakeErrorReporter
  // to record error history
  mer.DelegateToFake();
  Context* context1 = Context::get_instance();
  ErrorReporterInterface* old_rpt = context1->get_error_reporter();
  context1->set_error_reporter(&mer);
  context1->clear_context();
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

  std::string input("version 1:0;");
  ref.major = 1;
  ref.minor = 0;

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context1->token_to_scan = yylex();
  EXPECT_CALL(mer, report_error(ErrorReporterInterface::OK, _, _));
  EXPECT_EQ(0, Version(context1));

  mer.show_all_error();
  context1->set_error_reporter(old_rpt);
  context1->clear_context();

}
}  // namespace brig
}  // namespace hsa
