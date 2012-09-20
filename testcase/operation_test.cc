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
#include "operation_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(Instruction1opNoDTLaneid, BrigLaneId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_laneid[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_laneid[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTLaneid,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTDynwaveid, BrigDynWaveId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_dynwaveid[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_dynwaveid[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTDynwaveid,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTDispatchid, BrigDispatchId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_dispatchid[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_dispatchid[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTDispatchid,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTCu, BrigCU){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_cu[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_cu[n];
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTCu,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTWorkdim, BrigWorkDim){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_workdim[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_workdim[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTWorkdim,
                        testing::Range(0,1));


TEST_P(Instruction1opNoDTWorkitemidFlat, BrigWorkItemIdflat){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_workitemidflat[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_workitemidflat[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTWorkitemidFlat,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTWorkitemaidFlat, BrigWorkItemAIdflat){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_workitemaidflat[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_workitemaidflat[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTWorkitemaidFlat,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTClock, BrigClock){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_clock[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  context->get_operand(8, &getReg);
  BrigInstBase ref = outputarray_instruction1opnodt_clock[n];
  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  // BrigOperandReg
  EXPECT_EQ(12, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(0, getReg.name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTClock,
                        testing::Range(0,1));

TEST_P(Instruction1opNoDTDebugtrap, BrigDebugtrap){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction1opnodt_debugtrap[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction1opnodt_debugtrap[int(n/5)];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  if((n == 0)||(n == 1)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);
  }
  if(n == 2){
    context->get_operand(8, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(0, getImm.bits.u);
  }
  if(n == 3){
    context->get_operand(8, &getImm);
    //BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if(n == 4){
    context->get_operand(8, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction1opNoDTDebugtrap,
                        testing::Range(0,5));

TEST_P(MulBase, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_base[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = output_array_mul_base[int(n/3)];

  context->get_code(0, &get);
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
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  delete lexer;
  }
};
INSTANTIATE_TEST_CASE_P(CodegenTest, MulBase, testing::Range(0,36));


TEST_P(MulBaseFalse, BrigMulBase) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_base_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulBaseFalse, testing::Range(0,17));

TEST_P(MulFtzRound, BrigMulModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_ftz_round[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));


  if (n < 96) {
    BrigInstMod get;
    BrigOperandReg getReg1, getReg2, getReg3;
    BrigOperandImmed getImm1, getImm2;
    BrigInstMod ref = output_array_mul_ftz_round[int(n/2)];

    context->get_code(0, &get);
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
    EXPECT_EQ(ref.aluModifier.hi, get.aluModifier.hi);
    EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);
    EXPECT_EQ(ref.aluModifier.approx, get.aluModifier.approx);
    EXPECT_EQ(ref.aluModifier.fbar, get.aluModifier.fbar);
    EXPECT_EQ(ref.aluModifier.reserved, get.aluModifier.reserved);

    switch (n % 12) {
      case 0 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 1 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 2 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 3 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 4 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 5 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 6 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 7 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 8 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 9 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 10 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 11 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      default : {
        assert(false);
      }
    }
  } else {

    BrigInstBase get;
    BrigOperandReg getReg1, getReg2, getReg3;
    BrigOperandImmed getImm1, getImm2;
    BrigInstBase ref = output_array_mul_floating[int((n-96)/2)];

    context->get_code(0, &get);
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

    switch (n-96) {
      case 0 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 1 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 2 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 3 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 4 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 5 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 6 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 7 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 8 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 9 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      case 10 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);
        break;
      }
      case 11 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);
        break;
      }
      default : {
        assert(false);
      }
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulFtzRound, testing::Range(0,108));


TEST_P(MulFtzRoundFalse, BrigMulRounding) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_ftz_round_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulFtzRoundFalse, testing::Range(0,7));


TEST_P(SegpSegmentpb1, BrigSegmentp){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_segp_segmentpb1[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;

  context->get_code(0, &getMem);

  BrigInstMem ref = outputarray_segp_segmentpb1[int(n/3)];

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

  if((n == 0)||(n == 3)||(n == 6)||(n == 9)||(n == 12)||(n == 15)||(n == 18)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 1)||(n == 4)||(n == 7)||(n == 10)||(n == 13)||(n == 16)||(n == 19)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
if((n == 2)||(n == 5)||(n == 8)||(n == 11)||(n == 14)||(n == 17)||(n == 20)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb1, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest, SegpSegmentpb1, testing::Range(0,21));


TEST_P(SegpFtos, BrigFtoS)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_segp_ftos[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;

  context->get_code(0, &getMem);

  BrigInstMem ref = outputarray_segp_ftos[int(n/2)];

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

  if((n == 0)||(n == 2)||(n == 4)||(n == 6)||(n == 8)||(n == 10)||(n == 12)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 1)||(n == 3)||(n == 5)||(n == 7)||(n == 9)||(n == 11)||(n == 13)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if((n == 14)||(n == 16)||(n == 18)||(n == 20)||
     (n == 22)||(n == 24)||(n == 26)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 15)||(n == 17)||(n == 19)||(n == 21)||
     (n == 23)||(n == 25)||(n == 27)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest, SegpFtos, testing::Range(0,28));


TEST_P(SegpStof, BrigStoF){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_segp_stof[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Segp(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;

  context->get_code(0, &getMem);

  BrigInstMem ref = outputarray_segp_stof[int(n/2)];

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

  if((n == 0)||(n == 2)||(n == 4)||(n == 6)||(n == 8)||(n == 10)||(n == 12)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 1)||(n == 3)||(n == 5)||(n == 7)||(n == 9)||(n == 11)||(n == 13)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }
  if((n == 14)||(n == 16)||(n == 18)||(n == 20)||
     (n == 22)||(n == 24)||(n == 26)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20,&getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 15)||(n == 17)||(n == 19)||(n == 21)||
     (n == 23)||(n == 25)||(n == 27)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(24, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest, SegpStof, testing::Range(0,28));

TEST_P(MulControl, BrigMulPacket) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_control[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;

  BrigInstBase ref = output_array_mul_control[n];
  context->get_code(0, &get);

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

  if (n<36) {
    context->get_operand(8, &getReg1);
    // BrigOperandReg
    EXPECT_EQ(12, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb32, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(0, getReg1.name);

    context->get_operand(20, &getReg2);
    // BrigOperandReg
    EXPECT_EQ(12, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb32, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(4, getReg2.name);

    context->get_operand(32, &getReg3);
    // BrigOperandReg
    EXPECT_EQ(12, getReg3.size);
    EXPECT_EQ(BrigEOperandReg, getReg3.kind);
    EXPECT_EQ(Brigb32, getReg3.type);
    EXPECT_EQ(0, getReg3.reserved);
    EXPECT_EQ(8, getReg3.name);
  } else {
    context->get_operand(8, &getReg1);
    // BrigOperandReg
    EXPECT_EQ(12, getReg1.size);
    EXPECT_EQ(BrigEOperandReg, getReg1.kind);
    EXPECT_EQ(Brigb64, getReg1.type);
    EXPECT_EQ(0, getReg1.reserved);
    EXPECT_EQ(0, getReg1.name);

    context->get_operand(20, &getReg2);
    // BrigOperandReg
    EXPECT_EQ(12, getReg2.size);
    EXPECT_EQ(BrigEOperandReg, getReg2.kind);
    EXPECT_EQ(Brigb64, getReg2.type);
    EXPECT_EQ(0, getReg2.reserved);
    EXPECT_EQ(4, getReg2.name);

    context->get_operand(32, &getReg3);
    // BrigOperandReg
    EXPECT_EQ(12, getReg3.size);
    EXPECT_EQ(BrigEOperandReg, getReg3.kind);
    EXPECT_EQ(Brigb64, getReg3.type);
    EXPECT_EQ(0, getReg3.reserved);
    EXPECT_EQ(8, getReg3.name);
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, MulControl, testing::Range(0,76));

TEST_P(MulControlFalse, BrigMulPacket) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_control_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulControlFalse, testing::Range(0,180));
TEST_P(MulHis32Base, BrigMulBase)
{

  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_hi_s32_base[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = output_array_mul_hi_s32_base[int(n/3)];
  context->get_code(0, &get);

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
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(Codegen, MulHis32Base, testing::Range(0,9));

TEST_P(MulHis32BaseFalse, BrigMulHi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_s32_base_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHis32BaseFalse, testing::Range(0,5));

TEST_P(MulHiu32Base, BrigMulBase)
{

  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_hi_u32_base[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = output_array_mul_hi_u32_base[int(n/3)];
  context->get_code(0, &get);

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
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHiu32Base, testing::Range(0,9));

TEST_P(MulHiu32BaseFalse, BrigMulHi) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_u32_base_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHiu32BaseFalse, testing::Range(0,5));

TEST_P(MulHiControl, BrigMulPacket) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul_hi_control[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;

  BrigInstBase ref = output_array_mul_hi_control[n];
  context->get_code(0, &get);

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

  context->get_operand(8, &getReg1);
  // BrigOperandReg
  EXPECT_EQ(12, getReg1.size);
  EXPECT_EQ(BrigEOperandReg, getReg1.kind);
  EXPECT_EQ(Brigb32, getReg1.type);
  EXPECT_EQ(0, getReg1.reserved);
  EXPECT_EQ(0, getReg1.name);

  context->get_operand(20, &getReg2);
  // BrigOperandReg
  EXPECT_EQ(12, getReg2.size);
  EXPECT_EQ(BrigEOperandReg, getReg2.kind);
  EXPECT_EQ(Brigb32, getReg2.type);
  EXPECT_EQ(0, getReg2.reserved);
  EXPECT_EQ(4, getReg2.name);

  context->get_operand(32, &getReg3);
  // BrigOperandReg
  EXPECT_EQ(12, getReg3.size);
  EXPECT_EQ(BrigEOperandReg, getReg3.kind);
  EXPECT_EQ(Brigb32, getReg3.type);
  EXPECT_EQ(0, getReg3.reserved);
  EXPECT_EQ(8, getReg3.name);
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(Codegen, MulHiControl, testing::Range(0,16));

TEST_P(MulHiControlFalse, BrigMulPacket) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul_hi_control_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, MulHiControlFalse, testing::Range(0,16));

TEST_P(Mul24, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul24[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = output_array_mul24[int(n/3)];
  context->get_code(0, &get);

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
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, Mul24, testing::Range(0,36));

TEST_P(Mul24False, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul24_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mul24False, testing::Range(0,4));

TEST_P(Mul24Hi, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mul24_hi[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3;
  BrigOperandImmed getImm1, getImm2;
  BrigOperandWaveSz getWave1, getWave2;

  BrigInstBase ref = output_array_mul24_hi[int(n/3)];
  context->get_code(0, &get);

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
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, Mul24Hi, testing::Range(0,36));

TEST_P(Mul24HiFalse, BrigMul24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mul24_hi_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mul24HiFalse, testing::Range(0,4));

TEST_P(Mad24, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mad24[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = output_array_mad24[int(n/3)];
  context->get_code(0, &get);

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

  switch (n % 54) {
    case 0 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 18 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 19 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 20 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 21 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 22 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 23 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 24 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 25 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 26 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 27 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 28 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 29 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 30 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 31 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 32 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 33 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 34 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 35 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 36 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 37 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 38 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 39 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 40 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 41 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 42 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 43 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 44 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 45 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 46 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 47 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 48 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 49 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 50 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 51 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 52 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 53 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, Mad24, testing::Range(0,108));

TEST_P(Mad24False, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad24_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mad24False, testing::Range(0,4));

TEST_P(Mad24Hi, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mad24_hi[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Mul(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = output_array_mad24_hi[int(n/3)];
  context->get_code(0, &get);

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

  switch (n % 54) {
    case 0 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 18 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 19 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 20 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 21 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 22 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 23 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 24 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 25 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 26 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 27 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 28 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 29 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 30 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 31 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 32 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 33 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 34 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 35 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 36 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 37 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 38 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 39 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 40 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 41 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 42 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 43 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 44 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 45 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 46 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 47 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 48 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 49 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 50 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 51 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 52 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 53 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest, Mad24Hi, testing::Range(0,108));

TEST_P(Mad24HiFalse, BrigMad24) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad24_hi_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Mul(context));
}
INSTANTIATE_TEST_CASE_P(CodegenTest, Mad24HiFalse, testing::Range(0,4));



TEST_P(Instruction3OpAddInt, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_add_int[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  //BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_add_int[int(n/2)];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  if((n == 0)||(n == 2)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 1)||(n == 3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 4)||(n == 6)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 5)||(n == 7)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 8)||(n == 10)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 9)||(n == 11)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 12)||(n == 14)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);
  }
  if((n == 13)||(n == 15)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpAddInt,
                        testing::Range(0,16));


TEST_P(Instruction3OpAddPackedint, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_add_packed_int[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_add_packed_int[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  if((n == 0)||(n == 1)||(n == 2)||(n ==3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 4)||(n == 5)||(n == 6)||(n ==7)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpAddPackedint,
                        testing::Range(0,8/*10*/));


TEST_P(Instruction3OpAddFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_add_float[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  //BrigOperandImmed getImm;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_add_float[int(n/2)];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  if((n == 0)||(n == 1)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 2)||(n == 3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 4)||(n == 5)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpAddFloat,
                        testing::Range(0,6));


TEST_P(Instruction3OpAddFtzRoundFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_add_ftz_round_float[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  BrigOperandReg getReg;
  //BrigOperandImmed getImm;
  context->get_code(0, &getMod);
  BrigInstMod ref = outputarray_instruction3op_add_ftz_round_float[int(n/1)];

  EXPECT_EQ(ref.size, getMod.size);
  EXPECT_EQ(ref.kind, getMod.kind);
  EXPECT_EQ(ref.opcode, getMod.opcode);
  EXPECT_EQ(ref.packing, getMod.packing);
  EXPECT_EQ(ref.type, getMod.type);
  EXPECT_EQ(ref.o_operands[0], getMod.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getMod.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getMod.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getMod.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getMod.o_operands[4]);

  EXPECT_EQ(ref.aluModifier.ftz, getMod.aluModifier.ftz);
  EXPECT_EQ(ref.aluModifier.rounding, getMod.aluModifier.rounding);
  EXPECT_EQ(ref.aluModifier.floatOrInt, getMod.aluModifier.floatOrInt);

  if((n == 0)||(n == 1)||(n == 3)||(n == 4)||(n == 6)||(n == 7)||(n == 9)||
     (n == 10)||(n == 11)||(n == 12)||(n == 15)||(n == 16)||(n == 18)||
     (n == 19)||(n == 21)||(n == 22)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 2)||(n == 5)||(n == 8)||(n == 13)||(n == 14)||(n == 17)||(n == 20)||
     (n == 23)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpAddFtzRoundFloat,
                        testing::Range(0,24/*96*/));


TEST_P(Instruction3OpAddPackedFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_add_packed_float[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_add_packed_float[n];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

 if(n == 0){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 1)||(n == 2)||(n == 3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpAddPackedFloat,
                        testing::Range(0,4));


TEST_P(Instruction3OpCarry, BrigCarry){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_carry[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  //BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_carry[int(n/2/*3*/)];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

  if((n == 0)||(n == 2)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 1)||(n == 3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 4)||(n == 6)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 5)||(n == 7)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 8)||(n == 10)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 9)||(n == 11)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 12)||(n == 14)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);
  }
  if((n == 13)||(n == 15)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpCarry,
                        testing::Range(0,16/*36*/));


TEST_P(Instruction3OpBorrow, BrigBorrow){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(inputarray_instruction3op_borrow[n]);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  //BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction3op_borrow[int(n/2/*3*/)];

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getBase.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getBase.o_operands[4]);

    if((n == 0)||(n == 2)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 1)||(n == 3)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 4)||(n == 6)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(8, getReg.name);
  }
  if((n == 5)||(n == 7)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);

    context->get_operand(32, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 8)||(n == 10)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(4, getReg.name);
  }
  if((n == 9)||(n == 11)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 12)||(n == 14)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);
  }
  if((n == 13)||(n == 15)){
    context->get_operand(8, &getReg);
    // BrigOperandReg
    EXPECT_EQ(12, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(0, getReg.name);

    context->get_operand(20, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);

    context->get_operand(24, &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(4, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,
                        Instruction3OpBorrow,
                        testing::Range(0,16/*36*/));

TEST_P(Instruction4Mad, BrigMad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mad[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));

  BrigInstBase get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandImmed getImm1, getImm2, getImm3;
  BrigOperandWaveSz getWave1, getWave2, getWave3;

  BrigInstBase ref = output_array_mad[int(n/3)];
  context->get_code(0, &get);

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

  switch (n % 54) {
    case 0 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 1 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 2 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 3 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 4 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 5 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 6 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 7 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 8 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb32, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 9 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 10 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 11 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 12 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 13 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 14 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 15 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 16 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 17 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb32, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 18 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 19 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 20 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb32, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 21 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 22 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 23 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb32, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 24 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb32, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 25 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb32, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 26 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb32, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 27 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(12, getReg4.name);
      break;
    }
    case 28 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 29 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(8, getReg3.name);

      context->get_operand(44, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 30 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 31 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 32 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 33 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 34 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 35 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getReg2);
      // BrigOperandReg
      EXPECT_EQ(12, getReg2.size);
      EXPECT_EQ(BrigEOperandReg, getReg2.kind);
      EXPECT_EQ(Brigb64, getReg2.type);
      EXPECT_EQ(0, getReg2.reserved);
      EXPECT_EQ(4, getReg2.name);

      context->get_operand(32, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 36 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 37 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 38 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(56, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 39 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 40 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 41 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(68, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 42 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

     context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 43 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 44 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getImm1);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm1.size);
      EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
      EXPECT_EQ(Brigb64, getImm1.type);
      EXPECT_EQ(0, getImm1.reserved);
      EXPECT_EQ(1, getImm1.bits.u);

      context->get_operand(44, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 45 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(8, getReg4.name);
      break;
    }
    case 46 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 47 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getReg3);
      // BrigOperandReg
      EXPECT_EQ(12, getReg3.size);
      EXPECT_EQ(BrigEOperandReg, getReg3.kind);
      EXPECT_EQ(Brigb64, getReg3.type);
      EXPECT_EQ(0, getReg3.reserved);
      EXPECT_EQ(4, getReg3.name);

      context->get_operand(36, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 48 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 49 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 50 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getImm2);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm2.size);
      EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
      EXPECT_EQ(Brigb64, getImm2.type);
      EXPECT_EQ(0, getImm2.reserved);
      EXPECT_EQ(1, getImm2.bits.u);

      context->get_operand(48, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    case 51 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getReg4);
      // BrigOperandReg
      EXPECT_EQ(12, getReg4.size);
      EXPECT_EQ(BrigEOperandReg, getReg4.kind);
      EXPECT_EQ(Brigb64, getReg4.type);
      EXPECT_EQ(0, getReg4.reserved);
      EXPECT_EQ(4, getReg4.name);
      break;
    }
    case 52 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getImm3);
      // BrigOperandImmed
      EXPECT_EQ(24, getImm3.size);
      EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
      EXPECT_EQ(Brigb64, getImm3.type);
      EXPECT_EQ(0, getImm3.reserved);
      EXPECT_EQ(1, getImm3.bits.u);
      break;
    }
    case 53 : {
      context->get_operand(8, &getReg1);
      // BrigOperandReg
      EXPECT_EQ(12, getReg1.size);
      EXPECT_EQ(BrigEOperandReg, getReg1.kind);
      EXPECT_EQ(Brigb64, getReg1.type);
      EXPECT_EQ(0, getReg1.reserved);
      EXPECT_EQ(0, getReg1.name);

      context->get_operand(20, &getWave1);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave1.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave1.kind);

      context->get_operand(24, &getWave2);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave2.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave2.kind);

      context->get_operand(28, &getWave3);
      // BrigOperandWaveSz
      EXPECT_EQ(4, getWave3.size);
      EXPECT_EQ(BrigEOperandWaveSz, getWave3.kind);
      break;
    }
    default : {
      assert(false);
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4Mad,
                           testing::Range(0,108));

TEST_P(Instruction4MadFalse, BrigMad) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4MadFalse,
                           testing::Range(0,4));

TEST_P(Instruction4MadFtzRound, BrigMadModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(input_array_mad_ftz_round[n]);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Instruction4(context));


  if (n < 192) {
    BrigInstMod get;
    BrigOperandReg getReg1, getReg2, getReg3, getReg4;
    BrigOperandImmed getImm1, getImm2, getImm3;
    BrigOperandWaveSz getWave1, getWave2, getWave3;
    BrigInstMod ref = output_array_mad_ftz_round[int(n/2)];

    context->get_code(0, &get);
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
    EXPECT_EQ(ref.aluModifier.hi, get.aluModifier.hi);
    EXPECT_EQ(ref.aluModifier.ftz, get.aluModifier.ftz);
    EXPECT_EQ(ref.aluModifier.approx, get.aluModifier.approx);
    EXPECT_EQ(ref.aluModifier.fbar, get.aluModifier.fbar);
    EXPECT_EQ(ref.aluModifier.reserved, get.aluModifier.reserved);

    switch (n % 24) {
      case 0 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 1 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 2 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 3 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 4 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 5 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 6 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 7 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 8 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 9 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 10 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 11 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 12 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 13 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 14 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 15 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 16 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 17 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 18 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 19 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 20 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 21 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 22 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 23 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      default : {
        assert(false);
      }
    }
  } else {
    BrigInstBase get;
    BrigOperandReg getReg1, getReg2, getReg3, getReg4;
    BrigOperandImmed getImm1, getImm2, getImm3;
    BrigOperandWaveSz getWave1, getWave2, getWave3;
    BrigInstBase ref = output_array_mad_floating[int((n-192)/2)];

    context->get_code(0, &get);
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

    switch (n-192) {
      case 0 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 1 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 2 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 3 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 4 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 5 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 6 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 7 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 8 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 9 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 10 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 11 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb32, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 12 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 13 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb32, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 14 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb32, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 15 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb32, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb32, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb32, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb32, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 16 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(12, getReg4.name);
        break;
      }
      case 17 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(8, getReg3.name);

        context->get_operand(44, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 18 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 19 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getReg2);
        // BrigOperandReg
        EXPECT_EQ(12, getReg2.size);
        EXPECT_EQ(BrigEOperandReg, getReg2.kind);
        EXPECT_EQ(Brigb64, getReg2.type);
        EXPECT_EQ(0, getReg2.reserved);
        EXPECT_EQ(4, getReg2.name);

        context->get_operand(32, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 20 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(8, getReg4.name);
        break;
      }
      case 21 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getReg3);
        // BrigOperandReg
        EXPECT_EQ(12, getReg3.size);
        EXPECT_EQ(BrigEOperandReg, getReg3.kind);
        EXPECT_EQ(Brigb64, getReg3.type);
        EXPECT_EQ(0, getReg3.reserved);
        EXPECT_EQ(4, getReg3.name);

        context->get_operand(56, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      case 22 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getReg4);
        // BrigOperandReg
        EXPECT_EQ(12, getReg4.size);
        EXPECT_EQ(BrigEOperandReg, getReg4.kind);
        EXPECT_EQ(Brigb64, getReg4.type);
        EXPECT_EQ(0, getReg4.reserved);
        EXPECT_EQ(4, getReg4.name);
        break;
      }
      case 23 : {
        context->get_operand(8, &getReg1);
        // BrigOperandReg
        EXPECT_EQ(12, getReg1.size);
        EXPECT_EQ(BrigEOperandReg, getReg1.kind);
        EXPECT_EQ(Brigb64, getReg1.type);
        EXPECT_EQ(0, getReg1.reserved);
        EXPECT_EQ(0, getReg1.name);

        context->get_operand(20, &getImm1);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm1.size);
        EXPECT_EQ(BrigEOperandImmed, getImm1.kind);
        EXPECT_EQ(Brigb64, getImm1.type);
        EXPECT_EQ(0, getImm1.reserved);
        EXPECT_EQ(1, getImm1.bits.u);

        context->get_operand(44, &getImm2);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm2.size);
        EXPECT_EQ(BrigEOperandImmed, getImm2.kind);
        EXPECT_EQ(Brigb64, getImm2.type);
        EXPECT_EQ(0, getImm2.reserved);
        EXPECT_EQ(1, getImm2.bits.u);

        context->get_operand(68, &getImm3);
        // BrigOperandImmed
        EXPECT_EQ(24, getImm3.size);
        EXPECT_EQ(BrigEOperandImmed, getImm3.kind);
        EXPECT_EQ(Brigb64, getImm3.type);
        EXPECT_EQ(0, getImm3.reserved);
        EXPECT_EQ(1, getImm3.bits.u);
        break;
      }
      default : {
        assert(false);
      }
    }
  }
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4MadFtzRound,
                           testing::Range(0,216));

TEST_P(Instruction4MadFtzRoundFalse, BrigMadModifier) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_mad_ftz_round_false[n]);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Instruction4(context));
  delete lexer;
};
INSTANTIATE_TEST_CASE_P(CodegenTest,
                           Instruction4MadFtzRoundFalse,
                           testing::Range(0,9));

}  // namespace brig
}  // namespace hsa
