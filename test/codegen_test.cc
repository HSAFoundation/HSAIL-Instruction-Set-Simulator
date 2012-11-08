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
  context->get_directive(context->get_bdf_offset(), &get);
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

/*
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
*/

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
  curDirOffset = context->get_bdf_offset();
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

}  // namespace brig
}  // namespace hsa
