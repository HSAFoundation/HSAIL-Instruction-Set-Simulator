// Copyright 2012 MulticoreWare Inc.

#include "instruction2_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestInstruction2Abs, Abs) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2abs_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2abs_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.u);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.l[0]);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Abs, testing::Range(0, 6));

TEST_P(TestInstruction2PackAbs, PackAbs) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2packabs_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2packabs_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n < 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2PackAbs, testing::Range(0, 10));

TEST_P(TestInstruction2Neg, Neg) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2neg_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2neg_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.u);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.l[0]);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Neg, testing::Range(0, 6));

TEST_P(TestInstruction2PackNeg, PackNeg) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2packneg_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2packneg_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n < 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2PackNeg, testing::Range(0, 10));

TEST_P(TestInstruction2Not, Not) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2not_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2not_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb1, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb1, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb1, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb1, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0, getImmed1.bits.u);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(17, getImmed1.bits.u);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(17, getImmed1.bits.l[0]);
  } else if (n == 6) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb1, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 7) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 8) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Not, testing::Range(0, 9));

TEST_P(TestInstruction2Popcount, Popcount) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2popcount_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2popcount_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(65537, getImmed1.bits.u);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(65537, getImmed1.bits.l[0]);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Popcount, testing::Range(0, 6));

TEST_P(TestInstruction2Firstbit, Firstbit) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2firstbit_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2firstbit_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.u);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.l[0]);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Firstbit, testing::Range(0, 6));

TEST_P(TestInstruction2Lastbit, Lastbit) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2lastbit_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2lastbit_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.u);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.l[0]);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Lastbit, testing::Range(0, 6));

TEST_P(TestInstruction2Bitrev, Bitrev) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2bitrev_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2bitrev_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.u);
  } else if (n == 3) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.l[0]);
  } else if (n == 4) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  } else if (n == 5) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz1);
    EXPECT_EQ(wavesz_size, getWaveSz1.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Bitrev, testing::Range(0, 6));

TEST_P(TestInstruction2MovLo, Movlo) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2movslo_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2movslo_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2MovLo, testing::Range(0, 1));

TEST_P(TestInstruction2MovsLoInvalid, MovsLo) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2_movslo_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2MovsLoInvalid,
                           testing::Range(0,5));

TEST_P(TestInstruction2Movhi, Movhi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2movshi_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2movshi_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(code_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Movhi, testing::Range(0, 1));

TEST_P(TestInstruction2MovsHiInvalid, MovsHi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2_movshi_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2MovsHiInvalid,
                           testing::Range(0,5));


TEST_P(TestInstruction2Count, Count) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2count_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2count_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if( n==1 ) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb1, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if( n==2 ) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(100, getImmed1.bits.u);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Count, testing::Range(0, 3));

TEST_P(TestInstruction2Mask, TestMask) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2mask_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2mask_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n == 0) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if(n == 1) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb1, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if(n == 2) {
    context->get_operand(operand_offset, &getReg1);
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(100, getImmed1.bits.l[0]);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestInstruction2Mask, testing::Range(0, 3));

TEST_P(TestInstruction2FTZsqrt, Sqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_sqrt_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_sqrt_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if(n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.f);
  } else if(n == 4){
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1.0, getImmed1.bits.d);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZsqrt,
                           testing::Range(0, 5));

TEST_P(TestInstruction2FTZModSqrt, Sqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modsqrt_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modsqrt_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

  if (n < 14) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 21) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 28) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb16, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.f);
  } else if (n < 35) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(1, getImmed1.bits.d);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModSqrt,
                           testing::Range(0, 42));

TEST_P(TestInstruction2FtzSqrtInvalid, Sqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_sqrt_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2FtzSqrtInvalid,
                           testing::Range(0,23));


TEST_P(TestInstruction2FTZfsin, Fsin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_fsin_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_fsin_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZfsin,
                           testing::Range(0,2));

TEST_P(TestInstruction2FTZModfsin, Fsin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modsin_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modsin_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModfsin,
                           testing::Range(0,2));

TEST_P(TestInstruction2FtzfsinInvalid, Fsin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_fsin_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2FtzfsinInvalid,
                           testing::Range(0,16));


TEST_P(TestInstruction2FTZfcos, Fcos) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_fcos_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_fcos_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZfcos,
                           testing::Range(0,2));

TEST_P(TestInstruction2FTZModfcos, Fcos) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modfcos_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modfcos_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);
  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModfcos,
                           testing::Range(0,2));

TEST_P(TestInstruction2FtzfcosInvalid, Fcos) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_fcos_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2FtzfcosInvalid,
                           testing::Range(0,16));


TEST_P(TestInstruction2FTZfexp2, Fexp2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_fexp2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_fexp2_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZfexp2,
                           testing::Range(0,2));

TEST_P(TestInstruction2FTZModfexp2, Fexp2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modexp2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modexp2_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModfexp2,
                           testing::Range(0,2));

TEST_P(TestInstruction2Ftzfexp2Invalid, Fexp2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_fexp2_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2Ftzfexp2Invalid,
                           testing::Range(0,16));


TEST_P(TestInstruction2FTZflog2, Flog2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_flog2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_flog2_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZflog2,
                           testing::Range(0,2));

TEST_P(TestInstruction2FTZModflog2, Flog2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modflog2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modflog2_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

  if (n == 0) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(0.3f, getImmed1.bits.f);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModflog2,
                           testing::Range(0,2));

TEST_P(TestInstruction2Ftzflog2Invalid, Flog2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_flog2_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestInstruction2Ftzflog2Invalid,
                           testing::Range(0,16));


TEST_P(TestInstruction2FTZFrcp, Frcp) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_frcp_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_frcp_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n < 1) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZFrcp,
                           testing::Range(0,2));

TEST_P(TestInstruction2FTZModFrcp, Frcp) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modfrcp_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modfrcp_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

  if (n < 1) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModFrcp,
                           testing::Range(0, 2));

TEST_P(TestInstruction2FtzfrcpInvalid, Frcp) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_frcp_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                        TestInstruction2FtzfrcpInvalid,
                        testing::Range(0,16));


TEST_P(TestInstruction2FTZfract, Fract) {
    context->set_error_reporter(main_reporter);
    context->clear_context();

    int n =GetParam();
    Lexer* lexer = new Lexer(instruction2opftz_fract_pair[n].str);
    context->token_to_scan = lexer->get_next_token();

    EXPECT_EQ(0, Instruction2(context));

    BrigInstBase get;
    BrigOperandReg getReg1, getReg2;
    BrigOperandImmed getImmed1;
    context->get_code(code_offset, &get);
    BrigInstBase ref = instruction2opftz_fract_pair[n].ref;

    EXPECT_EQ(ref.size, get.size);
    EXPECT_EQ(ref.kind, get.kind);
    EXPECT_EQ(ref.opcode, get.opcode);
    EXPECT_EQ(ref.packing, get.packing);
    EXPECT_EQ(ref.type, get.type);
    EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
    EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
    EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
    EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
    EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

   if (n < 2) {
     context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3){
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb16, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2f, getImmed1.bits.f);
  } else if (n == 4) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2f, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ(immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2, getImmed1.bits.d);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZfract,
                           testing::Range(0,6));

TEST_P(TestInstruction2FTZModfract, Fract) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modfract_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modfract_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);

   if (n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3){
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n == 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb16, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2f, getImmed1.bits.f);
  }  else if (n == 4) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2f, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(3.2, getImmed1.bits.d);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModfract,
                           testing::Range(0,6));

TEST_P(TestInstruction2FtzfractInvalid, Fract) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_fract_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FtzfractInvalid,
                           testing::Range(0,16));


TEST_P(TestInstruction2FTZfrsqrt, Frsqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_frsqrt_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction2opftz_frsqrt_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  if (n < 1) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
    } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.d);
    }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZfrsqrt,
                           testing::Range(0,4));

TEST_P(TestInstruction2FTZModfrsqrt, Frsqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction2opftz_modsfrqrt_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction2(context));

  BrigInstMod getmod;
  BrigOperandReg getReg1, getReg2;
  BrigOperandImmed getImmed1;
  context->get_code(code_offset, &getmod);
  BrigInstMod ref = instruction2opftz_modsfrqrt_pair[n].ref;

  EXPECT_EQ(ref.size, getmod.size);
  EXPECT_EQ(ref.kind, getmod.kind);
  EXPECT_EQ(ref.opcode, getmod.opcode);
  EXPECT_EQ(ref.packing, getmod.packing);
  EXPECT_EQ(ref.type, getmod.type);
  EXPECT_EQ(ref.o_operands[0], getmod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getmod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getmod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getmod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getmod.o_operands[4]);
  EXPECT_EQ(ref.aluModifier.valid, getmod.aluModifier.valid);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getmod.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, getmod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, getmod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, getmod.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, getmod.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, getmod.aluModifier.reserved);
  if (n < 1) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 2) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    EXPECT_EQ( reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);
  } else if (n < 3) {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb32, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.f);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    EXPECT_EQ( reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getImmed1);
    EXPECT_EQ( immed_size, getImmed1.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
    EXPECT_EQ(Brigb64, getImmed1.type);
    EXPECT_EQ(0, getImmed1.reserved);
    EXPECT_EQ(4, getImmed1.bits.d);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FTZModfrsqrt,
                           testing::Range(0,4));

TEST_P(TestInstruction2FtzfrsqrtInvalid, Frsqrt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_instruction2opftz_frsqrt_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction2(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestInstruction2FtzfrsqrtInvalid,
                           testing::Range(0,16));


}  // namespace brig
}  // namespace hsa
