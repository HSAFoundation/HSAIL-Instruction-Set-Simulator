// Copyright 2012 MulticoreWare Inc.

#include "mul_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(MulBase, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mulbase_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase ref = mulbase_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;


  context->get_code(code_offset, &get);
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

  switch (n%18) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 9 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 10 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 12 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 13 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 16 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulBase, testing::Range(0,36));

TEST_P(MulBaseInvalid, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_base_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulBaseInvalid, testing::Range(0,18));

TEST_P(MulControl, BrigMulControl) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mulcontrol_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase ref = mulcontrol_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;

  context->get_code(code_offset, &get);
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

  if (n<12) {
    context->get_operand(ref.o_operands[0], &getReg1);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);

    context->get_operand(ref.o_operands[2], &getReg3);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg3.size);
    EXPECT_EQ(BrigEOperandReg, getReg3.kind);
    EXPECT_EQ(Brigb32, getReg3.type);
    EXPECT_EQ(0, getReg3.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
  } else {
    context->get_operand(ref.o_operands[0], &getReg1);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(string_offset, getReg1.s_name);

    context->get_operand(ref.o_operands[1], &getReg2);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(string_offset + 4, getReg2.s_name);

    context->get_operand(ref.o_operands[2], &getReg3);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg3.size);
    EXPECT_EQ(BrigEOperandReg, getReg3.kind);
    EXPECT_EQ(Brigb64, getReg3.type);
    EXPECT_EQ(0, getReg3.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulControl, testing::Range(0,24));

TEST_P(MulControlInvalid, BrigMulControl) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_control_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulControlInvalid, testing::Range(0,16));

TEST_P(MulHis32Base, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mulhis32_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase ref = mulhis32_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;


  context->get_code(code_offset, &get);
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

  switch (n) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHis32Base, testing::Range(0,9));

TEST_P(MulHis32BaseInvalid, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_s32_base_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulHis32BaseInvalid, testing::Range(0,13));


TEST_P(MulHiu32Base, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mulhiu32_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase ref = mulhiu32_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;


  context->get_code(code_offset, &get);
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

  switch (n) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHiu32Base, testing::Range(0,9));

TEST_P(MulHiu32BaseInvalid, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_u32_base_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulHiu32BaseInvalid, testing::Range(0,13));

TEST_P(MulHiControl, BrigMulControl) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mulhicontrol_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase ref = mulhicontrol_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;

  context->get_code(code_offset, &get);
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


  context->get_operand(ref.o_operands[0], &getReg1);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb32, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(string_offset, getReg1.s_name);

  context->get_operand(ref.o_operands[1], &getReg2);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg2.size);
  EXPECT_EQ(BrigEOperandReg, getReg2.kind);
  EXPECT_EQ(Brigb32, getReg2.type);
  EXPECT_EQ(0, getReg2.reserved);
  EXPECT_EQ(string_offset + 4, getReg2.s_name);

  context->get_operand(ref.o_operands[2], &getReg3);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg3.size);
  EXPECT_EQ(BrigEOperandReg, getReg3.kind);
  EXPECT_EQ(Brigb32, getReg3.type);
  EXPECT_EQ(0, getReg3.reserved);
  EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHiControl, testing::Range(0,16));

TEST_P(MulHiControlInvalid, BrigMulControl) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_control_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulHiControlInvalid, testing::Range(0,16));

TEST_P(MulFtzRound, BrigMulModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(mulftzround_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstMod get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigInstMod ref = mulftzround_pair[n].ref;

  context->get_code(code_offset, &get);
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

  EXPECT_EQ(ref.aluModifier.floatOrInt, get.aluModifier.floatOrInt);
  EXPECT_EQ(ref.aluModifier.rounding, get.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.approx, get.aluModifier.approx);
  EXPECT_EQ(ref.aluModifier.fbar, get.aluModifier.fbar);
  EXPECT_EQ(ref.aluModifier.reserved, get.aluModifier.reserved);

  switch (n) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 9 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 10 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 11 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 12 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 13 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 14 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 15 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
      break;
    }
    case 16 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 17 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);
      break;
    }
    case 18 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb16, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 19 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb16, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);
      break;
    }
    case 20 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 21 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);
      break;
    }
    case 22 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb16, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 23 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImme
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb16, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulFtzRound, testing::Range(0,24));

TEST_P(MulFtzRoundInvalid, BrigMulRounding) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_ftz_round_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(InvalidTest, MulFtzRoundInvalid, testing::Range(0,16));

TEST_P(Mul24, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(mul24_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = mul24_pair[n].ref;
  context->get_code(code_offset, &get);

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

  switch (n) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 9 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 10 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);
      break;
    }
    case 11 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mul24, testing::Range(0,12));

TEST_P(Mul24Invalid, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul24_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(InvalidTest, Mul24Invalid, testing::Range(0,4));

TEST_P(Mad24Hi, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(mad24hi_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = mad24hi_pair[n].ref;
  context->get_code(8, &get);

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

  switch (n) {
    case 0 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 9 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 10 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 11 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 12 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4, getReg4.s_name);
      break;
    }
    case 13 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 14 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 15 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4, getReg4.s_name);
      break;
    }
    case 16 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 17 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 18 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 19 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 20 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
      break;
    }
    case 21 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4, getReg4.s_name);
      break;
    }
    case 22 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 23 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);
      break;
    }
    case 24 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4, getReg4.s_name);
      break;
    }
    case 25 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 26 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mad24Hi, testing::Range(0,27));

TEST_P(Mad24HiInvalid, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad24_hi_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(InvalidTest, Mad24HiInvalid, testing::Range(0,4));

}  // namespace brig
}  // namespace hsa
