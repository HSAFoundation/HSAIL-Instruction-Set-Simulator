// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "mock_error_reporter.h"
#include "parser_wrapper.h"

using ::testing::_;

namespace hsa {
namespace brig {

ErrorReporter* main_reporter = ErrorReporter::get_instance();
Context* context = new Context(main_reporter);

TEST(CodegenTest, Instrustion3Op_CodeGen) {
  main_reporter->show_error(true);
  // test error reporter
  Context* context1 = new Context(main_reporter);
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
  EXPECT_EQ(0, Instruction3(yylex(), context1));

  BrigInstBase get;
  context1->get_code<BrigInstBase>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  main_reporter->show_error(false);
  delete context1;
}


TEST(CodegenTest, Instrustion2Op_CodeGen) {
  main_reporter->show_error(true);
  // test error reporter
  Context* context1 = new Context(main_reporter);
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
  EXPECT_EQ(0, Instruction2(yylex(), context1));

  BrigInstBase get;
  context1->get_code<BrigInstBase>(
              0,
              &get);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);

  main_reporter->show_error(false);
  delete context1;
}

TEST(CodegenTest, SimplestFunction_CodeGen) {
  main_reporter->show_error(true);
  // test error reporter
  Context* context1 = new Context(main_reporter);
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
//  ref.major = 1;
//  ref.minor = 0;

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context1));

  input.assign("function &return_true(arg_f32 %ret_val)(){ret;};");

  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Function(yylex(), context1));

  // test the .directive section size
  BrigdOffset32_t dsize = context1->get_directive_offset();
  EXPECT_EQ(96, dsize);

  // test the offset to the .string section
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


  // test the .string size
  BrigsOffset32_t size = context1->get_string_offset();
  EXPECT_EQ(22, size);

  // find the string.
  std::string func_name("&return_true");
  int str_offset = context1->lookup_symbol(func_name);
  EXPECT_EQ(0, str_offset);

  BrigcOffset32_t csize = context1->get_code_offset();
  EXPECT_EQ(32, csize);

  main_reporter->show_error(false);
  delete context1;
}

TEST(CodegenTest, AlignmentCheck) {
  // Try the situation in PRM 20.2 (pg. 226)

  // use a new context object to ensure the problem happen
  // since if at beginning the offset is a multiple of 4 but not a multiple of 8
  // then appending a 4-byte aligned item will lead to a multiple-of-8 offset

  Context* context1 = new Context();

  // First append a 4-byte aligned item BrigBlockStart
  uint32_t old_offset;
  uint32_t curr_offset = context1->get_directive_offset();

  BrigBlockStart bbs = {
    12,                        // size
    BrigEDirectiveBlockStart,  // kind
    0,                         // c_code
    0                          // s_name;
  };

  context1->append_directive(&bbs);    // append_directiveirective
  old_offset = curr_offset;
  curr_offset = context1->get_directive_offset();

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

  context1->append_directive(&bbn);
  old_offset = curr_offset;
  curr_offset = context1->get_directive_offset();

  EXPECT_EQ(BrigEAlignment_8, Context::alignment_check(bbn));
  // this is a 8-byte aligned item and has a size of multiple of 8.
  // so the offset after appending this item should be a multiple of 8.
  EXPECT_EQ(0, curr_offset%8);

  delete context1;
}

TEST(CodegenTest, VersionCodeGen) {
  MockErrorReporter mock_rpt;
  mock_rpt.DelegateToFake();

  Context* test_context = new Context(&mock_rpt);

  // reference struct
  BrigDirectiveVersion ref = {
    sizeof(ref),
    BrigEDirectiveVersion,
    0,            // unknown c_code
    1,            // major
    0,            // minor
    BrigESmall,   // machine
    BrigEFull,    // profile
    BrigENosftz,  // ftz
    0             // reserved
    };

  std::string input("version 1:0;");
  ref.major = 1;
  ref.minor = 0;

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_CALL(mock_rpt, report_error(ErrorReporter::OK, yylineno));
  EXPECT_EQ(0, Version(yylex(), test_context));
  // after append BrigDirectiveVersion
  uint32_t curr_d_offset = test_context->get_directive_offset();

  // get structure back
  BrigDirectiveVersion get;
  test_context->get_directive<BrigDirectiveVersion>(
                  curr_d_offset-sizeof(get),
                  &get);
  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

  mock_rpt.show_all_error();
      /* TEST 2 */
  input.assign("version 2:0:$large;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));

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
  input.assign("version 2:0:$large, $reduced, $sftz;");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context));

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
}

TEST(CodegenTest, RegisterOperandCodeGen) {
  std::string name;
  std::string input("$d7");  // register


  Parser* parser = new Parser(context);
  parser->set_source_string(input);

  // scan symbols
  parser->scan_symbols();

  // rescan
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Operand(yylex(), context));

  // reference struct
  BrigOperandReg ref = {
    sizeof(ref),      // size
    BrigEOperandReg,  // kind
    Brigb64,          // type
    0                // reserved
  };
  name.assign("$d7");
  ref.name = context->lookup_symbol(name);

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
  EXPECT_EQ(0, Operand(yylex(), context));

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
  EXPECT_EQ(0, Operand(yylex(), context));

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
  EXPECT_EQ(0, Operand(yylex(), context));

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
  EXPECT_EQ(0, Operand(yylex(), context));

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
  EXPECT_EQ(0, Operand(yylex(), context));

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
  EXPECT_EQ(0, Operand(yylex(), context));

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

TEST(CodegenTest, FunctionDeclaration) {
  std::string input("function &get_global_id(arg_u32 %ret_val)");
  input.append("(arg_u32 %arg_val0);");
}

}  // namespace brig
}  // namespace hsa
