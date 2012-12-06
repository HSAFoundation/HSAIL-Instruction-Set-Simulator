// Copyright 2012 MulticoreWare Inc.

#include "syscall_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestSyscall,Syscall)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(syscall_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, SysCall(context));

  BrigInstBase get;
  context->get_code(code_offset, &get);
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3, getImm4;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = syscall_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

 switch(n){
    case 0:{
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
      EXPECT_EQ(3, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[3], &getReg3);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(string_offset + 4 * 2, getReg3.s_name);

      context->get_operand(ref.o_operands[4], &getReg4);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(string_offset + 4 * 3, getReg4.s_name);

      break;
    }
    case 1:{
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
      EXPECT_EQ(3, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(4, getImm2.bits.u);

      context->get_operand(ref.o_operands[3], &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(5, getImm3.bits.u);

      context->get_operand(ref.o_operands[4], &getImm4);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm4.size);
      EXPECT_EQ(BrigEOperandImmed, getImm4.kind);
      EXPECT_EQ(Brigb32, getImm4.type);
      EXPECT_EQ(0, getImm4.reserved);
      EXPECT_EQ(6, getImm4.bits.u);

      break;
    }
    case 2:{
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
      EXPECT_EQ(3, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(ref.o_operands[3], &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(ref.o_operands[4], &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);

      break;
    }
    case 3:{
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
      EXPECT_EQ(3, getImm1.bits.u);

      context->get_operand(ref.o_operands[2], &getReg2);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(string_offset + 4, getReg2.s_name);

      context->get_operand(ref.o_operands[3], &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(immed_size, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(5, getImm2.bits.u);

      context->get_operand(ref.o_operands[4], &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(wavesz_size, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      break;
    }
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestSyscall, testing::Range(0,4));

TEST_P(TestSyscallInvalid, SyscallInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_syscall_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, SysCall(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestSyscallInvalid, testing::Range(0,5));

}  // namespace brig
}  // namespace hsa
