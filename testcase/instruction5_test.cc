// Copyright 2012 MulticoreWare Inc.

#include "instruction5_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

//Function RoundUp8 : Returns the smallest multiple of 8.
unsigned RoundUp8 (int bytes){
  return (((bytes) + kAlign - 1 ) & ~(kAlign - 1 ));
}

TEST_P(Instruction05Opf2u4, BrigF2u4) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction5opf2u4_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction5(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4, getReg5;
  BrigOperandImmed getImmed1, getImmed2, getImmed3, getImmed4;
  BrigOperandWaveSz getWaveSz1;
  context->get_code(code_offset, &get);
  BrigInstBase ref = instruction5opf2u4_pair[n].ref;

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

switch (n % 14) {
    case 0 : {
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
      break;
    }
    case 1 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

      context->get_operand(ref.o_operands[4], &getReg5);
      EXPECT_EQ(reg_size, getReg5.size);
      EXPECT_EQ(BrigEOperandReg, getReg5.kind);
      EXPECT_EQ(Brigb32, getReg5.type);
      EXPECT_EQ(0, getReg5.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);
      break;
    }
    case 2 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);
      break;
    }
    case 3 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

      context->get_operand(ref.o_operands[4], &getImmed1);
      EXPECT_EQ(immed_size, getImmed1.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
      EXPECT_EQ(Brigb32, getImmed1.type);
      EXPECT_EQ(0, getImmed1.reserved);
      EXPECT_EQ(1, getImmed1.bits.f);
      break;
    }
    case 4 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImmed1);
      EXPECT_EQ(immed_size, getImmed1.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
      EXPECT_EQ(Brigb32, getImmed1.type);
      EXPECT_EQ(0, getImmed1.reserved);
      EXPECT_EQ(1, getImmed1.bits.f);

      context->get_operand(ref.o_operands[4], &getImmed2);
      EXPECT_EQ(immed_size, getImmed2.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed2.kind);
      EXPECT_EQ(Brigb32, getImmed2.type);
      EXPECT_EQ(0, getImmed2.reserved);
      EXPECT_EQ(1, getImmed2.bits.f);
      break;
    }
    case 5 : {
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

      context->get_operand(ref.o_operands[2], &getImmed1);
      EXPECT_EQ(immed_size, getImmed1.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
      EXPECT_EQ(Brigb32, getImmed1.type);
      EXPECT_EQ(0, getImmed1.reserved);
      EXPECT_EQ(1, getImmed1.bits.f);

      context->get_operand(ref.o_operands[3], &getImmed2);
      EXPECT_EQ(immed_size, getImmed2.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed2.kind);
      EXPECT_EQ(Brigb32, getImmed2.type);
      EXPECT_EQ(0, getImmed2.reserved);
      EXPECT_EQ(1, getImmed2.bits.f);

      context->get_operand(ref.o_operands[4], &getImmed3);
      EXPECT_EQ(immed_size, getImmed3.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed3.kind);
      EXPECT_EQ(Brigb32, getImmed3.type);
      EXPECT_EQ(0, getImmed3.reserved);
      EXPECT_EQ(1, getImmed3.bits.f);
      break;
    }
    case 6 : {
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
      EXPECT_EQ(1, getImmed1.bits.f);

      context->get_operand(ref.o_operands[2], &getImmed2);
      EXPECT_EQ(immed_size, getImmed2.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed2.kind);
      EXPECT_EQ(Brigb32, getImmed2.type);
      EXPECT_EQ(0, getImmed2.reserved);
      EXPECT_EQ(1, getImmed2.bits.f);

      context->get_operand(ref.o_operands[3], &getImmed3);
      EXPECT_EQ(immed_size, getImmed3.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed3.kind);
      EXPECT_EQ(Brigb32, getImmed3.type);
      EXPECT_EQ(0, getImmed3.reserved);
      EXPECT_EQ(1, getImmed3.bits.f);

      context->get_operand(ref.o_operands[4], &getImmed4);
      EXPECT_EQ(immed_size, getImmed4.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed4.kind);
      EXPECT_EQ(Brigb32, getImmed4.type);
      EXPECT_EQ(0, getImmed4.reserved);
      EXPECT_EQ(1, getImmed4.bits.f);
      break;
    }
    case 7 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getReg4);
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

      context->get_operand(ref.o_operands[4], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 8 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[2], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 10 : {
      context->get_operand(operand_offset, &getReg1);
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(ref.o_operands[1], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 11 : {
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

      context->get_operand(ref.o_operands[2], &getReg3);
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[3], &getImmed4);
      EXPECT_EQ(immed_size, getImmed4.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed4.kind);
      EXPECT_EQ(Brigb32, getImmed4.type);
      EXPECT_EQ(0, getImmed4.reserved);
      EXPECT_EQ(1, getImmed4.bits.f);

      context->get_operand(ref.o_operands[4], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 12 : {
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

      context->get_operand(ref.o_operands[2], &getImmed3);
      EXPECT_EQ(immed_size, getImmed3.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed3.kind);
      EXPECT_EQ(Brigb32, getImmed3.type);
      EXPECT_EQ(0, getImmed3.reserved);
      EXPECT_EQ(1, getImmed3.bits.f);

      context->get_operand(ref.o_operands[3], &getImmed4);
      EXPECT_EQ(immed_size, getImmed4.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed4.kind);
      EXPECT_EQ(Brigb32, getImmed4.type);
      EXPECT_EQ(0, getImmed4.reserved);
      EXPECT_EQ(1, getImmed4.bits.f);

      context->get_operand(ref.o_operands[4], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }
    case 13 : {
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

      context->get_operand(ref.o_operands[2], &getImmed1);
      EXPECT_EQ(immed_size, getImmed1.size);
      EXPECT_EQ(BrigEOperandImmed, getImmed1.kind);
      EXPECT_EQ(Brigb32, getImmed1.type);
      EXPECT_EQ(0, getImmed1.reserved);
      EXPECT_EQ(1, getImmed1.bits.f);

      context->get_operand(ref.o_operands[3], &getWaveSz1);
      EXPECT_EQ(wavesz_size, getWaveSz1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWaveSz1.kind);
      break;
    }

  delete lexer;
}
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction05Opf2u4,
                           testing::Range(0, 14));

}  // namespace brig
}  // namespace hsa
