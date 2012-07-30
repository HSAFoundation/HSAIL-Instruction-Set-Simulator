// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "lexer_wrapper.h"
#include "parser_wrapper.h"

namespace hsa {
namespace brig {
Context* context = new Context();

TEST(CodegenTest, SimplestFunction_CodeGen) {

  Context* context1 = new Context();
  BrigDirectiveFunction ref = {
    40, //size
    BrigEDirectiveFunction, //kind
    0,
    0,
    0,
    40,  // d_firstScopedDirective
    0,
    40,  // d_nextDirective
    BrigNone,
    0,
    0,
    0,
  };

  std::string input("version 1:0;");
//  ref.major = 1;
//  ref.minor = 0;

  // current directive offset value
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0, Version(yylex(), context1));


  input.assign("function &return_true()(){ret;};");

  // test the rule
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  EXPECT_EQ(0,Function(yylex(),context1));

  // test the .directive section size
  BrigdOffset32_t dsize = context1->get_directive_offset();
  EXPECT_EQ(60,dsize);

  // test the offset to the .string section
  BrigDirectiveFunction get;
  context1->get_directive<BrigDirectiveFunction>(dsize-sizeof(get), &get);
  EXPECT_EQ(ref.s_name, get.s_name);

  // test the .string size
  BrigsOffset32_t size = context1->get_string_offset();
  EXPECT_EQ(13,size);

  // find the string.
  std::string func_name("&return_true");
  int str_offset = context1->lookup_symbol(func_name);
  EXPECT_EQ(0, str_offset);

  BrigcOffset32_t csize = context1->get_code_offset();
  EXPECT_EQ(32, csize);

  delete context1;
}


TEST(CodegenTest, AlignmentCheck) {
  // Try the situation in PRM 20.2 (pg. 226)

  // use a new context object to ensure the problem happen
  // since if at beginning the offset is a multiple of 4 but not a multiple of 8
  // then appending a 4-byte aligned item will lead to a multiple-of-8 offset

  Context* context1 = new Context();

  // First append a 4-byte aligned item BrigBlockStart
  int old_offset;
  int curr_offset = context1->get_directive_offset();

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
  EXPECT_EQ(0, Version(yylex(), context));

  // after append BrigDirectiveVersion
  int curr_d_offset = context->get_directive_offset();

  // get structure back
  BrigDirectiveVersion get;
  context->get_directive<BrigDirectiveVersion>(curr_d_offset-sizeof(get), &get);
  // compare two structs
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.major, get.major);
  EXPECT_EQ(ref.minor, get.minor);
  EXPECT_EQ(ref.machine, get.machine);
  EXPECT_EQ(ref.profile, get.profile);
  EXPECT_EQ(ref.ftz, get.ftz);

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

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  // scan for strings first
  ScanString(yylex(), context);
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
  int curr_o_offset = context->get_operand_offset();
  context->get_operand<BrigOperandReg>(curr_o_offset-sizeof(get), &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.name, get.name);

  // second register
  input.assign("$q7");
  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  // scan for strings first
  ScanString(yylex(), context);
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
  int curr_o_offset = context->get_operand_offset();
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

  int offset = strBuf->size();
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
  int offset = context->get_string_offset();

  // add symbol
  int sym1_offset = context->add_symbol(symbol);
  EXPECT_EQ(offset, sym1_offset);

  offset = context->get_string_offset();
  symbol.assign("%symbol2");
  int sym2_offset = context->add_symbol(symbol);
  EXPECT_EQ(offset, sym2_offset);

  // try to add symbol 1 again
  symbol.assign("&symbol1");
  int sym1b_offset = context->add_symbol(symbol);
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

  int offset = strBuf->size();
  input.assign("test_string1");
  strBuf->append(input);


  // lookup second string
  input.assign("test_string1");
  int loc = strBuf->lookup(input);
  EXPECT_EQ(offset, loc);
};

TEST(CodegenTest, StringScanTest) {
  std::string input("version 1:0:$large;\n");
  input.append("global_f32 &x = 2;\n");
  input.append("function &test()() {\n");
  input.append("{arg_u32 %z;}\n");
  input.append(" }; \n");

  std::cout << input << std::endl;

  yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
  context->clear_all_buffers();
  // scan for strings first
  ScanString(yylex(), context);
  // Print out string buffer content:
  int index = 0;
  std::string temp;
  std::cout << "Buffer content: " << std::endl;
  while (index < context->get_string_offset()) {
    temp = context->get_string(index);
    std::cout << "Index " << index << ": " << temp << std::endl;
    index+=temp.length()+1;
  }
};

TEST(CodegenTest, TestLexWrapper) {
  Lexer* lexer = new Lexer();
  std::string input("$c1");
  lexer->set_source_string(input);

  int token = lexer->get_next_token();

  EXPECT_GE(token, 0);
  EXPECT_EQ(token, TOKEN_CREGISTER);

  std::string token_str = lexer->get_string_value();
  EXPECT_STREQ("$c1", token_str.c_str());
};

TEST(CodegenTest, GetIntValFromLexer) {
  std::string input("10");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, lexer->get_int_value());
}

TEST(CodegenTest, GetFloatValFromLexer) {
  std::string input("10.0f");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.0, lexer->get_float_value());
}

TEST(CodegenTest, GetDoubleValFromLexer) {
  std::string input("10.0l");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_DOUBLE_CONSTANT);
  EXPECT_EQ(10.0, lexer->get_double_value());
}

TEST(CodegenTest, ResetLexer) {
  std::string input("$c1 10 10.5f");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, lexer->get_int_value());

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.5, lexer->get_float_value());

  // restart from begin
  lexer->restart();
  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);
}

TEST(CodegenTest, ScanSymbolsWithParser) {
  std::string input("version 1:0:$large;\n");
  input.append("global_f32 &x = 2;\n");
  input.append("function &test()() {\n");
  input.append("{arg_u32 %z;}\n");
  input.append(" }; \n");
  
  Parser* parser = new Parser(input);
  parser->scan_symbols();
  
  // read symbols
  Context* context = parser->get_context();
  
  // Print out string buffer content:
  int index = 0;
  std::string temp;
  std::cout << "Buffer content: " << std::endl;
  while (index < context->get_string_offset()) {
    temp = context->get_string(index);
    std::cout << "Index " << index << ": " << temp << std::endl;
    index+=temp.length()+1;
  }
}

}  // namespace brig
}  // namespace hsa
