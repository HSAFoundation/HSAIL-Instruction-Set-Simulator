// Copyright 2012 MulticoreWare Inc.

#include "instruction1_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestInstruction1NullPtr, NullPtr){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_nullptr_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstMem get;
  context->get_code(code_offset, &get);
  BrigInstMem ref = instruction1_nullptr_pair[n].ref;

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
  EXPECT_EQ(ref.storageClass, get.storageClass);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1NullPtr,
                        testing::Range(0,9));

TEST_P(TestInstruction1LaneId, LaneId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_laneid_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_laneid_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1LaneId,
                        testing::Range(0,1));

TEST_P(TestInstruction1DynWaveId, DynWaveId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_dynwaveid_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_dynwaveid_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1DynWaveId,
                        testing::Range(0,1));

TEST_P(TestInstruction1QId, QId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_qid_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_qid_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1QId,
                        testing::Range(0,1));

TEST_P(TestInstruction1DispatchId, DispatchId){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_dispatchid_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_dispatchid_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1DispatchId,
                        testing::Range(0,1));

TEST_P(TestInstruction1Cu, Cu){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_cu_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_cu_pair[n].ref;
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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1Cu,
                        testing::Range(0,1));

TEST_P(TestInstruction1WorkDim, WorkDim){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_workdim_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_workdim_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1WorkDim,
                        testing::Range(0,1));


TEST_P(TestInstruction1WorkitemIdFlat, WorkitemIdFlat){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_workitemidflat_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_workitemidflat_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1WorkitemIdFlat,
                        testing::Range(0,1));

TEST_P(TestInstruction1WorkitemAbsidFlat, WorkitemAbsidFlat){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_workitemabsidflat_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_workitemabsidflat_pair[n].ref;

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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb32, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1WorkitemAbsidFlat,
                        testing::Range(0,1));

TEST_P(TestInstruction1Clock, Clock){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_clock_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  context->get_operand(operand_offset, &getReg);
  BrigInstBase ref = instruction1_clock_pair[n].ref;
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
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(Brigb64, getReg.type);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1Clock,
                        testing::Range(0,1));

TEST_P(TestInstruction1DebugTrap, DebugTrap){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction1_debugtrap_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction1(context));

  BrigInstBase getBase;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction1_debugtrap_pair[n].ref;

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
    context->get_operand(operand_offset, &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(0, getImm.bits.u);
  }
  if(n == 1){
    context->get_operand(operand_offset, &getImm);
    //BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(61, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        TestInstruction1DebugTrap,
                        testing::Range(0,2));

}  // namespace brig
}  // namespace hsa
