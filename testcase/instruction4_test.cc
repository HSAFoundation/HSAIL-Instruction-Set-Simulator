// Copyright 2012 MulticoreWare Inc.

#include "instruction4_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(Instruction4MadTest, BrigMad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4mad_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4mad_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 18 : {
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 19 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 20 : {
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
    case 21 : {
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
      break;
    }
    case 22 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 23 : {
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
      EXPECT_EQ(1, getImm2.bits.l[0]);
      break;
    }
    case 24 : {
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

      context->get_operand(ref.o_operands[2], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
      break;
    }
    case 25 : {
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

      context->get_operand(ref.o_operands[2], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 26 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4MadTest,
                           testing::Range(0,27));

TEST_P(Instruction4MadInvalid, BrigMad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4MadInvalid,
                           testing::Range(0,4));

TEST_P(Instruction4MadFtzRound, BrigMadModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4madftzround_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstMod get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigInstMod ref = instruction4madftzround_pair[n].ref;

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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4MadFtzRound,
                           testing::Range(0,16));

TEST_P(Instruction4MadFtzRoundInvalid, BrigMadModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad_ftz_round_invalid[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4MadFtzRoundInvalid,
                           testing::Range(0,9));

TEST_P(Instruction4Extract, BrigExtract) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4extract_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4extract_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 4 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4Extract,
                        testing::Range(0,10));

TEST_P(Instruction4ExtractInvalid, BrigExtract) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_extract_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                        Instruction4ExtractInvalid,
                        testing::Range(0,23));

TEST_P(Instruction4Insert, BrigInsert) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4insert_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4insert_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 4 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 8 : {
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
      EXPECT_EQ(1, getImm2.bits.l[0]);
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
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4Insert,
                        testing::Range(0, 10));

TEST_P(Instruction4InsertInvalid, BrigInsert) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_insert_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4InsertInvalid,
                        testing::Range(0,22));

TEST_P(Instruction4Bitselect, Bitselect) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4bitselect_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4bitselect_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 4 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
   
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4Bitselect,
                        testing::Range(0,10));

TEST_P(Instruction4BitselectInvalid, Bitselect) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_bitselect_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                        Instruction4BitselectInvalid,
                        testing::Range(0,22));

TEST_P(Instruction4ShuffleTest, Shuffle) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4shuffle_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2, getImm3;

  BrigInstBase ref = instruction4shuffle_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.u);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.u);
      break;
    }
    case 4 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.l[0]);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0x55, getImm3.bits.l[0]);
      break;
    }
    case 8 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(2, getImm3.bits.d);
      break;
    }
    case 9 : {
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
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(2, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(2, getImm3.bits.d);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest5,
                        Instruction4ShuffleTest,
                        testing::Range(0,11));


TEST_P(Instruction4ShuffleInvalid, Shuffle) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_shuffle_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                        Instruction4ShuffleInvalid,
                        testing::Range(0,18));

TEST_P(Instruction4CmovTest, Cmov) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4cmov_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;

  BrigInstBase ref = instruction4cmov_pair[n].ref;
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
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
      break;
    }
    case 1 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb1, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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
      EXPECT_EQ(Brigb1, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb1, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(0, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(0, getImm3.bits.u);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(Brigb1, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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
      EXPECT_EQ(Brigb1, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 11 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.l[0]);
      break;
    }
    case 18 : {
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
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 19 : {
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
      break;
    }
    case 20 : {
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
      EXPECT_EQ(1.0, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4CmovTest,
                        testing::Range(0,21));

TEST_P(Instruction4CmovInvalid, Cmov) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmov_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4CmovInvalid,
                        testing::Range(0,18));

TEST_P(Instruction4FmaTest, BrigFma) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4fma_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstMod get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigInstMod ref = instruction4fma_pair[n].ref;

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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.f);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.f);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.f);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.d);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction4FmaTest,
                        testing::Range(0,16));

TEST_P(Instruction4FmaFtzRoundInvalid, Fma) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_fma_ftz_round_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                        Instruction4FmaFtzRoundInvalid,
                        testing::Range(0,9));

TEST_P(Instruction4BitAlign, BrigBitAlign) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4bitalign_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4bitalign_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(8, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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
      EXPECT_EQ(8, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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
      EXPECT_EQ(8, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4BitAlign,
                           testing::Range(0,7));

TEST_P(Instruction4BitAlignInvalid, BrigBitAlign) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_bitalign_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4BitAlignInvalid,
                           testing::Range(0,9));

TEST_P(Instruction4ByteAlign, BrigByteAlign) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4bytealign_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4bytealign_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4ByteAlign,
                           testing::Range(0,7));

TEST_P(Instruction4ByteAlignInvalid, BrigByteAlign) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_bytealign_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4ByteAlignInvalid,
                           testing::Range(0,9));

TEST_P(Instruction4Lerp, BrigLerp) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4lerp_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4lerp_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Lerp,
                           testing::Range(0,7));

TEST_P(Instruction4LerpInvalid, BrigLerp) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_lerp_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4LerpInvalid,
                           testing::Range(0,9));

TEST_P(Instruction4Sad, BrigSad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4sad_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4sad_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Sad,
                           testing::Range(0,7));

TEST_P(Instruction4SadInvalid, BrigSad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_sad_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4SadInvalid,
                           testing::Range(0,9));

TEST_P(Instruction4Sad2, BrigSad2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4sad2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4sad2_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Sad2,
                           testing::Range(0,7));

TEST_P(Instruction4Sad2Invalid, BrigSad2) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_sad2_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4Sad2Invalid,
                           testing::Range(0,9));

TEST_P(Instruction4Sad4, BrigSad4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4sad4_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4sad4_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Sad4,
                           testing::Range(0,7));

TEST_P(Instruction4Sad4Invalid, BrigSad4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_sad4_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4Sad4Invalid,
                           testing::Range(0,9));

TEST_P(Instruction4Sad4Hi, BrigSad4Hi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction4sad4hi_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = instruction4sad4hi_pair[n].ref;
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
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

      context->get_operand(ref.o_operands[3], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Sad4Hi,
                           testing::Range(0,7));

TEST_P(Instruction4Sad4HiInvalid, BrigSad4Hi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_sad4hi_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           Instruction4Sad4HiInvalid,
                           testing::Range(0,9));



}  // namespace brig
}  // namespace hsa
