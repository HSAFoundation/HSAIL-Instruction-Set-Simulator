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


}  // namespace brig
}  // namespace hsa
