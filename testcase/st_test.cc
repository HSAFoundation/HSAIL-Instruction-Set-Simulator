// Copyright 2012 MulticoreWare Inc.

#include "st_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(OperationSt, BrigSt){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(st_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n>=18) context->set_machine(BrigESmall);
  EXPECT_EQ(0, St(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = st_pair[n].ref;

  EXPECT_EQ(ref.size, getLdSt.size);
  EXPECT_EQ(ref.kind, getLdSt.kind);
  EXPECT_EQ(ref.opcode, getLdSt.opcode);
  EXPECT_EQ(ref.packing, getLdSt.packing);
  EXPECT_EQ(ref.type, getLdSt.type);
  EXPECT_EQ(ref.o_operands[0], getLdSt.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getLdSt.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getLdSt.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getLdSt.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getLdSt.o_operands[4]);
  EXPECT_EQ(ref.storageClass, getLdSt.storageClass);
  EXPECT_EQ(ref.memorySemantic, getLdSt.memorySemantic);
  EXPECT_EQ(ref.equivClass, getLdSt.equivClass);

  context->get_operand(ref.o_operands[0], &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  if(n == 0){
    context->get_operand(ref.o_operands[1], &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);
  } 
  if(n == 1){
    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[1] - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - address_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 28){
    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  if(n == 29){
    context->get_operand(ref.o_operands[1] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getComp.reg);
    EXPECT_EQ(0, getComp.offset);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationSt,testing::Range(0,30));

TEST_P(OperationStIm, BrigSt){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(stim_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n>=18) context->set_machine(BrigESmall);
  EXPECT_EQ(0, St(context));

  BrigInstLdSt getLdSt;
  BrigOperandImmed getImm;
  BrigOperandReg getReg;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = stim_pair[n].ref;

  EXPECT_EQ(ref.size, getLdSt.size);
  EXPECT_EQ(ref.kind, getLdSt.kind);
  EXPECT_EQ(ref.opcode, getLdSt.opcode);
  EXPECT_EQ(ref.packing, getLdSt.packing);
  EXPECT_EQ(ref.type, getLdSt.type);
  EXPECT_EQ(ref.o_operands[0], getLdSt.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getLdSt.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getLdSt.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getLdSt.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getLdSt.o_operands[4]);
  EXPECT_EQ(ref.storageClass, getLdSt.storageClass);
  EXPECT_EQ(ref.memorySemantic, getLdSt.memorySemantic);
  EXPECT_EQ(ref.equivClass, getLdSt.equivClass);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb8, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1], &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1] - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - address_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 27){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getComp.reg);
    EXPECT_EQ(0, getComp.offset);
  }
  if(n == 28){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  if(n == 29){
    context->get_operand(ref.o_operands[0], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb8, getImm.type);
    EXPECT_EQ(1, getImm.bits.u);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationStIm,testing::Range(0, 30));

TEST_P(OperationStV2, BrigSt){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(stv2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n>=18) context->set_machine(BrigESmall);
  EXPECT_EQ(0, St(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = stv2_pair[n].ref;

  EXPECT_EQ(ref.size, getLdSt.size);
  EXPECT_EQ(ref.kind, getLdSt.kind);
  EXPECT_EQ(ref.opcode, getLdSt.opcode);
  EXPECT_EQ(ref.packing, getLdSt.packing);
  EXPECT_EQ(ref.type, getLdSt.type);
  EXPECT_EQ(ref.o_operands[0], getLdSt.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getLdSt.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getLdSt.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getLdSt.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getLdSt.o_operands[4]);
  EXPECT_EQ(ref.storageClass, getLdSt.storageClass);
  EXPECT_EQ(ref.memorySemantic, getLdSt.memorySemantic);
  EXPECT_EQ(ref.equivClass, getLdSt.equivClass);

  context->get_operand(ref.o_operands[0] - reg_size * 2, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  context->get_operand(ref.o_operands[0] - reg_size, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4, getReg.s_name);

  context->get_operand(ref.o_operands[0], &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(regv2_size, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(ref.o_operands[0] - reg_size * 2, getRegV2.regs[0]);
  EXPECT_EQ(ref.o_operands[0] - reg_size, getRegV2.regs[1]);

  if(n == 0){
    context->get_operand(ref.o_operands[1], &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[1] - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - address_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 3){
    context->get_operand(ref.o_operands[1] - reg_size - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getComp.reg);
    EXPECT_EQ(-2, getComp.offset);
  }
  if(n == 28){
    context->get_operand(ref.o_operands[1] - reg_size - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getComp.reg);
    EXPECT_EQ(0, getComp.offset);
  }
  if(n == 29){
    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationStV2,testing::Range(0,30));

TEST_P(OperationStV4, BrigSt){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(stv4_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n>=18) context->set_machine(BrigESmall);
  EXPECT_EQ(0, St(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandRegV4 getRegV4;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = stv4_pair[n].ref;

  EXPECT_EQ(ref.size, getLdSt.size);
  EXPECT_EQ(ref.kind, getLdSt.kind);
  EXPECT_EQ(ref.opcode, getLdSt.opcode);
  EXPECT_EQ(ref.packing, getLdSt.packing);
  EXPECT_EQ(ref.type, getLdSt.type);
  EXPECT_EQ(ref.o_operands[0], getLdSt.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getLdSt.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getLdSt.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], getLdSt.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], getLdSt.o_operands[4]);
  EXPECT_EQ(ref.storageClass, getLdSt.storageClass);
  EXPECT_EQ(ref.memorySemantic, getLdSt.memorySemantic);
  EXPECT_EQ(ref.equivClass, getLdSt.equivClass);

  context->get_operand(ref.o_operands[0] - reg_size * 4, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  context->get_operand(ref.o_operands[0] - reg_size * 3, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4, getReg.s_name);

  context->get_operand(ref.o_operands[0] - reg_size * 2, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);

  context->get_operand(ref.o_operands[0] - reg_size, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4 * 3, getReg.s_name);

  context->get_operand(ref.o_operands[0], &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(ref.o_operands[0] - reg_size * 4, getRegV4.regs[0]);
  EXPECT_EQ(ref.o_operands[0] - reg_size * 3, getRegV4.regs[1]);
  EXPECT_EQ(ref.o_operands[0] - reg_size * 2, getRegV4.regs[2]);
  EXPECT_EQ(ref.o_operands[0] - reg_size, getRegV4.regs[3]);

  if(n == 0){
    context->get_operand(ref.o_operands[1], &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[1] - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(0, getAdd.directive);

    context->get_operand(ref.o_operands[1], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[1] - address_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 28){
    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);

    context->get_operand(ref.o_operands[1], &getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  if(n == 29){ 
    context->get_operand(ref.o_operands[1] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[1] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationStV4,testing::Range(0,30));


}  // namespace brig
}  // namespace hsa
