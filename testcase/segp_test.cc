// Copyright 2012 MulticoreWare Inc.

#include "segp_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(SegpSegmentpb1, BrigSegmentp){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(segpsegmentpb1_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(code_offset, &getMem);

  BrigInstSegp ref = segpsegmentpb1_pair[n].ref;

  EXPECT_EQ(ref.size, getMem.size);
  EXPECT_EQ(ref.kind, getMem.kind);
  EXPECT_EQ(ref.opcode, getMem.opcode);
  EXPECT_EQ(ref.packing, getMem.packing);
  EXPECT_EQ(ref.type, getMem.type);
  EXPECT_EQ(ref.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getMem.o_operands[4]);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb1, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);
  }
  if(n == 3){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, SegpSegmentpb1, testing::Range(0,28));

TEST_P(SegpFtos, BrigFtoS)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(segpftos_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(code_offset, &getMem);

  BrigInstSegp ref = segpftos_pair[n].ref;

  EXPECT_EQ(ref.size, getMem.size);
  EXPECT_EQ(ref.kind, getMem.kind);
  EXPECT_EQ(ref.opcode, getMem.opcode);
  EXPECT_EQ(ref.packing, getMem.packing);
  EXPECT_EQ(ref.type, getMem.type);
  EXPECT_EQ(ref.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getMem.o_operands[4]);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if(n == 21){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 22){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if(n == 23){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, SegpFtos, testing::Range(0,42));

TEST_P(SegpStof, BrigStoF){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(segpstof_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(code_offset, &getMem);

  BrigInstSegp ref = segpstof_pair[n].ref;

  EXPECT_EQ(ref.size, getMem.size);
  EXPECT_EQ(ref.kind, getMem.kind);
  EXPECT_EQ(ref.opcode, getMem.opcode);
  EXPECT_EQ(ref.packing, getMem.packing);
  EXPECT_EQ(ref.type, getMem.type);
  EXPECT_EQ(ref.o_operands[0], getMem.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getMem.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getMem.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getMem.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getMem.o_operands[4]);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if(n == 21){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(12, getReg.s_name);
  }
  if(n == 22){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if(n == 23){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, SegpStof, testing::Range(0,42));

}  // namespace brig
}  // namespace hsa
