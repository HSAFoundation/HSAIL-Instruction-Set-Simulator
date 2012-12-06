// Copyright 2012 MulticoreWare Inc.

#include "mov_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestMov, BrigMov) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(mov_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mov(context));

  BrigInstBase ref = mov_pair[n].ref;

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4, getReg5;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandImmed getImm1;

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
      context->get_operand(operand_offset, &getReg1);
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
      break;
    }
    case 1 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
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
      break;
    }
    case 2 : {
      context->get_operand(operand_offset, &getReg1);
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
      break;
    }
    case 3 : {
      context->get_operand(operand_offset, &getReg1);
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
      break;
    }
    case 4 : {
      context->get_operand(operand_offset, &getReg1);
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
      break;
    }
    case 5 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);
 
     context->get_operand(operand_offset + reg_size, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

     context->get_operand(operand_offset + reg_size * 2, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[1], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(operand_offset + reg_size, getRegV2.regs[0]);
      EXPECT_EQ(operand_offset + reg_size * 2, getRegV2.regs[1]);

      break;
    }
    case 6 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(operand_offset + reg_size, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

     context->get_operand(ref.o_operands[0], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(operand_offset, getRegV2.regs[0]);
      EXPECT_EQ(operand_offset + reg_size, getRegV2.regs[1]);

      context->get_operand(ref.o_operands[1], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg4.s_name);
      break;
    }
    case 7 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

     context->get_operand(operand_offset + reg_size, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4, getReg3.s_name);

     context->get_operand(ref.o_operands[0], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(operand_offset, getRegV2.regs[0]);
      EXPECT_EQ(operand_offset + reg_size, getRegV2.regs[1]);

      context->get_operand(ref.o_operands[1], &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.d);
      break;
    }
    case 8 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb128, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(operand_offset + reg_size, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(operand_offset + reg_size * 2, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(operand_offset + reg_size * 3, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

      context->get_operand(operand_offset + reg_size * 4, &getReg5);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg5.size);
      EXPECT_EQ(BrigEOperandReg, getReg5.kind);
      EXPECT_EQ(Brigb32, getReg5.type);
      EXPECT_EQ(0, getReg5.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);

     context->get_operand(ref.o_operands[1], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(operand_offset + reg_size, getRegV4.regs[0]);
      EXPECT_EQ(operand_offset + reg_size * 2, getRegV4.regs[1]);
      EXPECT_EQ(operand_offset + reg_size * 3, getRegV4.regs[2]);
      EXPECT_EQ(operand_offset + reg_size * 4, getRegV4.regs[3]);
      break;
    }
    case 9 : {
      context->get_operand(operand_offset, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(string_offset, getReg1.s_name);

      context->get_operand(operand_offset + reg_size, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(operand_offset + reg_size * 2, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(operand_offset + reg_size * 3, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

     context->get_operand(ref.o_operands[0], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(operand_offset, getRegV4.regs[0]);
      EXPECT_EQ(operand_offset + reg_size, getRegV4.regs[1]);
      EXPECT_EQ(operand_offset + reg_size * 2, getRegV4.regs[2]);
      EXPECT_EQ(operand_offset + reg_size * 3, getRegV4.regs[3]);

      context->get_operand(ref.o_operands[1], &getReg5);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg5.size);
      EXPECT_EQ(BrigEOperandReg, getReg5.kind);
      EXPECT_EQ(Brigb128, getReg5.type);
      EXPECT_EQ(0, getReg5.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);
      break;
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestMov, testing::Range(0,10));

TEST_P(TestMovInvalid, BrigMov) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mov_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mov(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestMovInvalid, testing::Range(0,29));

}  // namespace brig
}  // namespace hsa
