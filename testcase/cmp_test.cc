// Copyright 2012 MulticoreWare Inc.

#include "cmp_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestCmpEq, CmpEq) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_eq_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_eq_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 2 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.u);
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

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 23 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 24 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 26 : {
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
    case 27 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 31 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 32 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 35 : {
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
    case 36 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 39 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 40 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 41 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 42 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 43 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 44 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpEq, testing::Range(0,45));

TEST_P(TestCmpEqInvalid, CmpEqInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_eq_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpEqInvalid, testing::Range(0,56));

TEST_P(TestCmpNe, CmpNe) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_ne_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_ne_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 2 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 8 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(Brigb1, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.u);
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

      context->get_operand(ref.o_operands[1], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 23 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 24 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 26 : {
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
    case 27 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 31 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 32 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 35 : {
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
    case 36 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb1, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 39 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 40 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 41 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 42 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 43 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 44 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpNe, testing::Range(0,45));

TEST_P(TestCmpNeInvalid, CmpNeInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_ne_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpNeInvalid, testing::Range(0,56));

TEST_P(TestCmpLt, CmpLt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_lt_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_lt_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 3: {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 23 : {
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
    case 24 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 26 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 27 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 31 : {
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
    case 32 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 35 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 36 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 39 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpLt, testing::Range(0,40));

TEST_P(TestCmpLtInvalid, CmpLtInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_lt_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpLtInvalid, testing::Range(0,58));

TEST_P(TestCmpLe, CmpLe) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_le_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_le_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 3: {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 23 : {
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
    case 24 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 26 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 27 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 31 : {
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
    case 32 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 35 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 36 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 39 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpLe, testing::Range(0,40));

TEST_P(TestCmpLeInvalid, CmpLeInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_le_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpLeInvalid, testing::Range(0,58));

TEST_P(TestCmpGt, CmpGt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_gt_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_gt_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 3: {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 23 : {
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
    case 24 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 26 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 27 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 31 : {
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
    case 32 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 35 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 36 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 39 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpGt, testing::Range(0,40));

TEST_P(TestCmpGtInvalid, CmpGtInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_gt_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpGtInvalid, testing::Range(0,58));

TEST_P(TestCmpGe, CmpGe) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_ge_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_ge_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 2 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 3: {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 4 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 5 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 6 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 7 : {
      context->get_operand(ref.o_operands[0], &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb1, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
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
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 23 : {
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
    case 24 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 25 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 26 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 27 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 28 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 29 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 30 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 31 : {
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
    case 32 : {
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
      EXPECT_EQ(0, getImm2.bits.u);
      break;
    }
    case 33 : {
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 34 : {
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 35 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    case 36 : {
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
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 37 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 38 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 39 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpGe, testing::Range(0,40));

TEST_P(TestCmpGeInvalid, CmpGeInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_ge_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpGeInvalid, testing::Range(0,58));


TEST_P(TestCmpEqu, CmpEqu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_equ_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_equ_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpEqu, testing::Range(0,16));

TEST_P(TestCmpEquInvalid, CmpEquInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_equ_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpEquInvalid, testing::Range(0,51));

TEST_P(TestCmpNeu, CmpNeu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_neu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_neu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpNeu, testing::Range(0,16));

TEST_P(TestCmpNeuInvalid, CmpNeuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_neu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpNeuInvalid, testing::Range(0,51));

TEST_P(TestCmpLeu, CmpLeu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_leu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_leu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpLeu, testing::Range(0,16));

TEST_P(TestCmpLeuInvalid, CmpLeuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_leu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpLeuInvalid, testing::Range(0,51));

TEST_P(TestCmpLtu, CmpLtu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_ltu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_ltu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpLtu, testing::Range(0,16));

TEST_P(TestCmpLtuInvalid, CmpLtuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_ltu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpLtuInvalid, testing::Range(0,51));

TEST_P(TestCmpGtu, CmpGtu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_gtu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_gtu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpGtu, testing::Range(0,16));

TEST_P(TestCmpGtuInvalid, CmpGtuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_gtu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpGtuInvalid, testing::Range(0,51));

TEST_P(TestCmpGeu, CmpGeu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_geu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_geu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpGeu, testing::Range(0,16));

TEST_P(TestCmpGeuInvalid, CmpGeuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_geu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpGeuInvalid, testing::Range(0,51));

TEST_P(TestCmpNum, CmpNum) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_num_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_num_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpNum, testing::Range(0,16));

TEST_P(TestCmpNumInvalid, CmpNumInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_num_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpNumInvalid, testing::Range(0,51));

TEST_P(TestCmpNan, CmpNan) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_nan_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_nan_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.l[0]);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpNan, testing::Range(0,16));

TEST_P(TestCmpNanInvalid, CmpNanInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_nan_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpNanInvalid, testing::Range(0,51));

TEST_P(TestCmpSeq, CmpSeq) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_seq_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_seq_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSeq, testing::Range(0,16));

TEST_P(TestCmpSeqInvalid, CmpSeqInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_seq_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSeqInvalid, testing::Range(0,51));

TEST_P(TestCmpSne, CmpSne) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sne_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sne_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSne, testing::Range(0,16));

TEST_P(TestCmpSneInvalid, CmpSneInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sne_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSneInvalid, testing::Range(0,51));

TEST_P(TestCmpSlt, CmpSlt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_slt_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_slt_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSlt, testing::Range(0,16));

TEST_P(TestCmpSltInvalid, CmpSltInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_slt_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSltInvalid, testing::Range(0,51));

TEST_P(TestCmpSle, CmpSle) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sle_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sle_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSle, testing::Range(0,16));

TEST_P(TestCmpSleInvalid, CmpSleInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sle_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSleInvalid, testing::Range(0,51));

TEST_P(TestCmpSgt, CmpSgt) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sgt_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sgt_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSgt, testing::Range(0,16));

TEST_P(TestCmpSgtInvalid, CmpSgtInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sgt_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSgtInvalid, testing::Range(0,51));

TEST_P(TestCmpSge, CmpSge) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sge_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sge_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSge, testing::Range(0,16));

TEST_P(TestCmpSgeInvalid, CmpSgeInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sge_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSgeInvalid, testing::Range(0,51));

TEST_P(TestCmpSequ, CmpSequ) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sequ_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sequ_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSequ, testing::Range(0,16));

TEST_P(TestCmpSequInvalid, CmpSequInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sequ_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSequInvalid, testing::Range(0,51));

TEST_P(TestCmpSneu, CmpSneu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sneu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sneu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSneu, testing::Range(0,16));

TEST_P(TestCmpSneuInvalid, CmpSneuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sneu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSneuInvalid, testing::Range(0,51));

TEST_P(TestCmpSleu, CmpSleu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sleu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sleu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSleu, testing::Range(0,16));

TEST_P(TestCmpSleuInvalid, CmpSleuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sleu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSleuInvalid, testing::Range(0,51));

TEST_P(TestCmpSltu, CmpSltu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sltu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sltu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSltu, testing::Range(0,16));

TEST_P(TestCmpSltuInvalid, CmpSltuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sltu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSltuInvalid, testing::Range(0,51));

TEST_P(TestCmpSgtu, CmpSgtu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sgtu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sgtu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSgtu, testing::Range(0,16));

TEST_P(TestCmpSgtuInvalid, CmpSgtuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sgtu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSgtuInvalid, testing::Range(0,51));

TEST_P(TestCmpSgeu, CmpSgeu) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_sgeu_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_sgeu_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSgeu, testing::Range(0,16));

TEST_P(TestCmpSgeuInvalid, CmpSgeuInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_sgeu_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSgeuInvalid, testing::Range(0,51));

TEST_P(TestCmpSnum, CmpSnum) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_snum_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_snum_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSnum, testing::Range(0,16));

TEST_P(TestCmpSnumInvalid, CmpSnumInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_snum_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSnumInvalid, testing::Range(0,51));

TEST_P(TestCmpSnan, CmpSnan) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_snan_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_snan_pair[n].ref;

  BrigInstCmp get;
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
      EXPECT_EQ(Brigb1, getReg1.type);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.u);
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
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(12, getReg3.s_name);
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

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
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
      EXPECT_EQ(12, getReg3.s_name);
      break;
    }
    case 13 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);
      break;
    }
    case 15 : {
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
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.l[0]);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.l[0]);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpSnan, testing::Range(0,16));

TEST_P(TestCmpSnanInvalid, CmpSnanInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_cmp_snan_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestCmpSnanInvalid, testing::Range(0,51));

TEST_P(TestCmpF16, CmpF16) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(cmp_f16_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = cmp_f16_pair[n].ref;

  BrigInstCmp get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm2;
  BrigOperandWaveSz getWave1;

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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[1], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(0, getImm2.bits.f);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
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
    case 22 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 23 : {
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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 24 : {
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
    case 25 : {
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
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    case 26 : {
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
      EXPECT_EQ(Brigb16, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(0, getImm2.bits.f);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestCmpF16, testing::Range(0,27));

TEST_P(TestPackedCmpInteger, PackedCmpInteger) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(packedcmp_integer_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = packedcmp_integer_pair[n].ref;

  BrigInstCmp get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.reserved, get.reserved);

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
    case 3 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(1, getImm2.bits.l[0]);
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
      EXPECT_EQ(1, getImm2.bits.l[0]);
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
    case 13 : {
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
    case 14 : {
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
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.l[0]);
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
      EXPECT_EQ(1, getImm2.bits.u);
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
    case 20 : {
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
      break;
    }
    case 21 : {
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
    case 22 : {
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
      break;
    }
    case 23 : {
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
    case 24 : {
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
    case 25 : {
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
    case 26 : {
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
    case 27 : {
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
    case 28 : {
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
    case 29 : {
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
      break;
    }
    case 30 : {
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
    case 31 : {
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
    case 32 : {
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
    case 33 : {
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
      break;
    }
    case 34 : {
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
    case 35 : {
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
      break;
    }
    case 36 : {
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
    case 37 : {
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
    case 38 : {
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
    case 39 : {
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
    case 40 : {
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
    case 41 : {
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
    case 42 : {
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
      break;
    }
    case 43 : {
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
    case 44 : {
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
    case 45 : {
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
    case 46 : {
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
      break;
    }
    case 47 : {
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
    case 48 : {
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
      break;
    }
    case 49 : {
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
    case 50 : {
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
    case 51 : {
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
    case 52 : {
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
    case 53 : {
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
    case 54 : {
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
    case 55 : {
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
      break;
    }
    case 56 : {
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
    case 57 : {
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
    case 58 : {
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
    case 59 : {
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
      break;
    }
    case 60 : {
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
    case 61 : {
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
      break;
    }
    case 62 : {
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
    case 63 : {
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
    case 64 : {
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
    case 65 : {
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
    case 66 : {
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
    case 67 : {
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
    case 68 : {
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
      break;
    }
    case 69 : {
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
    case 70 : {
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
    case 71 : {
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
    case 72 : {
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
      break;
    }
    case 73 : {
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
    case 74 : {
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
      break;
    }
    case 75 : {
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
    case 76 : {
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
    case 77 : {
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
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestPackedCmpInteger,
                           testing::Range(0,78));

TEST_P(TestPackedCmpIntegerInvalid, PackedCmpIntegerInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_packedcmp_integer_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestPackedCmpIntegerInvalid,
                           testing::Range(0,60));

TEST_P(TestPackedCmpFloats, PackedCmpFloats) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(packedcmp_floats_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = packedcmp_floats_pair[n].ref;

  BrigInstCmp get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.reserved, get.reserved);

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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(1, getImm2.bits.d);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
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
    case 22 : {
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
    case 23 : {
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
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestPackedCmpFloats,
                           testing::Range(0,24));

TEST_P(TestPackedCmpFloatsInvalid, PackedCmpFloatsInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_packedcmp_floats_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestPackedCmpFloatsInvalid,
                           testing::Range(0,80));

TEST_P(TestPackedCmpSnan, PackedCmpSnan) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(packedcmp_snan_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Cmp(context));

  BrigInstCmp ref = packedcmp_snan_pair[n].ref;

  BrigInstCmp get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.comparisonOperator, get.comparisonOperator);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.reserved, get.reserved);

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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(1, getImm2.bits.d);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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
      break;
    }
    case 15 : {
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

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.d);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);
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

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);
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
      EXPECT_EQ(1, getImm1.bits.d);

      context->get_operand(ref.o_operands[2], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
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
    case 22 : {
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
    case 23 : {
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
      break;
    }
    case 24 : {
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
    case 25 : {
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
    case 26 : {
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
      break;
    }
    case 27 : {
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
    case 28 : {
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
    case 29 : {
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
      break;
    }
    case 30 : {
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
    case 31 : {
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
    case 32 : {
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
      break;
    }
    case 33 : {
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
    case 34 : {
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
    case 35 : {
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
      break;
    }
    case 36 : {
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
    case 37 : {
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
    case 38 : {
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
      break;
    }
    case 39 : {
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
    case 40 : {
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
    case 41 : {
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
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestPackedCmpSnan,
                           testing::Range(0,42));

TEST_P(TestPackedCmpSnanInvalid, PackedCmpSnanInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_packedcmp_snan_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Cmp(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest,
                           TestPackedCmpSnanInvalid,
                           testing::Range(0,140));


}  // namespace brig
}  // namespace hsa
