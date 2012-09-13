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

/*****************************************************************************/
/*****************        TITLE     :BrigLaneId_test case           **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        laneid    dest:s reg;                     **********/
/*****************************************************************************/

class Instruction1opNoDTLaneid: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigDynWaveId_test case        **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        dynwaveid   dest:s reg;                   **********/
/*****************************************************************************/

class Instruction1opNoDTDynwaveid: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigDispatchId_test case       **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        dispatchid    dest:s reg;                 **********/
/*****************************************************************************/

class Instruction1opNoDTDispatchid: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigCU_test case               **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        cu    dest:s reg;                         **********/
/*****************************************************************************/

class Instruction1opNoDTCu: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigWorkDim_test case          **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        workdim     dest:s reg;                   **********/
/*****************************************************************************/

class Instruction1opNoDTWorkdim: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :Brigworkitemidflat_test case   **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        workitemid_flat      dest:s reg;          **********/
/*****************************************************************************/

class Instruction1opNoDTWorkitemidFlat: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :Brigworkitemaidflat_test case  **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        workitemaid_flat    dest:s reg;           **********/
/*****************************************************************************/

class Instruction1opNoDTWorkitemaidFlat: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigClock_test case            **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************        clock    dest:d reg;                      **********/
/*****************************************************************************/

class Instruction1opNoDTClock: public ::testing::TestWithParam<int>{
};

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

/*****************************************************************************/
/*****************        TITLE     :BrigDebugtrap_test case        **********/
/*****************        TIME      :2012-09-03                     **********/
/*****************        AUTHOR    :Jiang Yan                      **********/
/*****************      debugtrap  src:s reg,Immed value,(wavesize) **********/
/*****************************************************************************/

class Instruction1opNoDTDebugtrap: public ::testing::TestWithParam<int>{
};

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

  context->get_code(0, &getBase);
  BrigInstBase ref = outputarray_instruction1opnodt_debugtrap[int(n/4)];

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest, 
                        Instruction1opNoDTDebugtrap, 
                        testing::Range(0,4));

}  // namespace brig
}  // namespace hsa
