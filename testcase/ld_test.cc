// Copyright 2012 MulticoreWare Inc.

#include "ld_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(OperationLd, BrigLd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(ld_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["&x"] = 40;
  context->local_symbol_map["%g"] = 80;

  if(n>=43) context->set_machine(BrigESmall);
  EXPECT_EQ(0, Ld(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandImmed getImmed;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = ld_pair[n].ref;

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

  context->get_operand(ref.o_operands[1], &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getImmed);
    EXPECT_EQ(immed_size, getImmed.size);
    EXPECT_EQ(BrigEOperandImmed, getImmed.kind);
    EXPECT_EQ(Brigb32, getImmed.type);
    EXPECT_EQ(0, getImmed.reserved);
    EXPECT_EQ(1, getImmed.bits.u);

    context->get_operand(ref.o_operands[2],&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);
  }  
  if(n == 1){
    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[2] - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - address_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 3){
    context->get_operand(ref.o_operands[2] - reg_size - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(10, getComp.offset);
  }
  if(n == 4){
    context->get_operand(ref.o_operands[2] - reg_size - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(-5, getComp.offset);
  }
  if(n == 5){
    context->get_operand(ref.o_operands[2] - reg_size - address_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(1, getComp.offset);
  }
  if(n == 78){
    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  if(n == 79){
    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationLd,testing::Range(0, 80));

TEST_P(OperationLdV2, BrigLd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(ldv2_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  context->local_symbol_map["%g"] = 80;
  context->global_symbol_map["&x"] = 40;

  if(n>=42) context->set_machine(BrigESmall);
  EXPECT_EQ(0, Ld(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = ldv2_pair[n].ref;

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

  context->get_operand(ref.o_operands[1] - reg_size * 2, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  context->get_operand(ref.o_operands[1] - reg_size, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4,  getReg.s_name);

  context->get_operand(ref.o_operands[1], &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(regv2_size, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(ref.o_operands[1] - reg_size * 2, getRegV2.regs[0]);
  EXPECT_EQ(ref.o_operands[1] - reg_size, getRegV2.regs[1]);

  if(n == 0){
    context->get_operand(ref.o_operands[2] - reg_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[2] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(1, getComp.offset);
  }
 if(n == 2){
    context->get_operand(ref.o_operands[2],&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);
  }
  if(n == 3){  
    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 78){ 
    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  if(n == 79){
    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2,  getReg.s_name);

    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(-5, getIndir.offset);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationLdV2,testing::Range(0,80));

TEST_P(OperationLdV4, BrigLd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(ldv4_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%g"] = 80;
  context->local_symbol_map["&x"] = 40;

  if(n>=43) context->set_machine(BrigESmall);
  EXPECT_EQ(0, Ld(context));

  BrigInstLdSt getLdSt;
  BrigOperandReg getReg;
  BrigOperandRegV4 getRegV4;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getLdSt);
  BrigInstLdSt ref = ldv4_pair[n].ref;

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

  context->get_operand(ref.o_operands[1] - reg_size * 4, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset, getReg.s_name);

  context->get_operand(ref.o_operands[1] - reg_size * 3, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4,  getReg.s_name);

  context->get_operand(ref.o_operands[1] - reg_size * 2, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4 * 2,  getReg.s_name);

  context->get_operand(ref.o_operands[1] - reg_size, &getReg);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg.size);
  EXPECT_EQ(BrigEOperandReg, getReg.kind);
  EXPECT_EQ(0, getReg.reserved);
  EXPECT_EQ(string_offset + 4 * 3,  getReg.s_name);

  context->get_operand(ref.o_operands[1], &getRegV4);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
  EXPECT_EQ(0, getRegV4.reserved);
  EXPECT_EQ(ref.o_operands[1] - reg_size * 4, getRegV4.regs[0]);
  EXPECT_EQ(ref.o_operands[1] - reg_size * 3, getRegV4.regs[1]);
  EXPECT_EQ(ref.o_operands[1] - reg_size * 2, getRegV4.regs[2]);
  EXPECT_EQ(ref.o_operands[1] - reg_size, getRegV4.regs[3]);

  if(n == 0){
    context->get_operand(ref.o_operands[2] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);

    context->get_operand(ref.o_operands[2], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(10, getComp.offset);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[2] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);

    context->get_operand(ref.o_operands[2], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(-5, getComp.offset);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[2] - reg_size - address_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);

    context->get_operand(ref.o_operands[2] - reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);

    context->get_operand(ref.o_operands[2], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size - address_size, getComp.name);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.reg);
    EXPECT_EQ(1, getComp.offset);
  }
  if(n == 3){
    context->get_operand(ref.o_operands[2],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 4){
    context->get_operand(ref.o_operands[2] - reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getReg.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[2], &getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[2] - reg_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationLdV4,testing::Range(0,80));

TEST_P(TestLdInvalid, LdInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_ld_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Ld(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestLdInvalid, testing::Range(0,12));


TEST_P(OperationLda, BrigLda)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;
  context->local_symbol_map["%g"] = 80;

  int n = GetParam();
  Lexer* lexer = new Lexer(lda_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n>=2 and n<=7) context->set_machine(BrigESmall);

  EXPECT_EQ(0, Lda(context));

  BrigInstMem getMem;
  BrigOperandReg getReg;
  BrigOperandAddress getAdd;
  BrigOperandIndirect getIndir;
  BrigOperandCompound getComp;
  context->get_code(code_offset, &getMem);
  BrigInstMem ref = lda_pair[n].ref;

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
  EXPECT_EQ(ref.storageClass, getMem.storageClass);

  if(n == 0){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  if(n == 3){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  if(n == 4){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb32, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(-5, getIndir.offset);
  }
  if(n == 5){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(0, getComp.offset);
  }
  if(n == 6){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(10, getComp.offset);
  }
  if(n == 7){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(reg_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb32, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb32, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(-5, getComp.offset);
  }
 if(n == 8){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 9){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 11){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(40, getAdd.directive);
  }
  if(n == 13){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0, getIndir.offset);
  }
  if(n == 14){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(0, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(0x6e, getIndir.offset);
  }
  if(n == 15){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(10, getIndir.offset);
  }
  if(n == 16){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getIndir);
    // BrigOperandIndirect
    EXPECT_EQ(indirect_size, getIndir.size);
    EXPECT_EQ(BrigEOperandIndirect, getIndir.kind);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getIndir.reg);
    EXPECT_EQ(Brigb64, getIndir.type);
    EXPECT_EQ(0, getIndir.reserved);
    EXPECT_EQ(-5, getIndir.offset);
  }
  if(n == 17){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(0, getComp.reg);
    EXPECT_EQ(4, getComp.offset);
  }
  if(n == 19){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(0, getComp.offset);
  }
  if(n == 20){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size,&getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(10, getComp.offset);
  }
  if(n == 21){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[0] + reg_size, &getAdd);
    // BrigOperandAddress
    EXPECT_EQ(address_size, getAdd.size);
    EXPECT_EQ(BrigEOperandAddress, getAdd.kind);
    EXPECT_EQ(Brigb64, getAdd.type);
    EXPECT_EQ(0, getAdd.reserved);
    EXPECT_EQ(80, getAdd.directive);

    context->get_operand(ref.o_operands[0] + reg_size + address_size, &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getComp);
    // BrigOperandCompound
    EXPECT_EQ(compound_size, getComp.size);
    EXPECT_EQ(BrigEOperandCompound, getComp.kind);
    EXPECT_EQ(Brigb64, getComp.type);
    EXPECT_EQ(0, getComp.reserved);
    EXPECT_EQ(ref.o_operands[0] + reg_size, getComp.name);
    EXPECT_EQ(ref.o_operands[0] + reg_size + address_size, getComp.reg);
    EXPECT_EQ(-5, getComp.offset);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,OperationLda,testing::Range(0,22));

TEST_P(OperationLdc, BrigLdc){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(ldc_pair[n].str);
  context->token_to_scan = lexer->get_next_token();
  if(n<2) context->set_machine(BrigESmall);

  EXPECT_EQ(0, Ldc(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandLabelRef getLabRef;
  BrigOperandFunctionRef getFun;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = ldc_pair[n].ref;

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
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getLabRef);
    // BrigOperandLabelRef
    EXPECT_EQ(labelref_size, getLabRef.size);
    EXPECT_EQ(BrigEOperandLabelRef, getLabRef.kind);
    EXPECT_EQ(0, getLabRef.labeldirective);
  }
  if(n == 1){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getFun);
    // BrigOperandFunctionRef
    EXPECT_EQ(functionref_size, getFun.size);
    EXPECT_EQ(BrigEOperandFunctionRef, getFun.kind);
    EXPECT_EQ(0, getFun.fn);
  }
  if(n == 2){
    context->get_operand(ref.o_operands[0], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);

    context->get_operand(ref.o_operands[1],&getFun);
    // BrigOperandFunctionRef
    EXPECT_EQ(functionref_size, getFun.size);
    EXPECT_EQ(BrigEOperandFunctionRef, getFun.kind);
    EXPECT_EQ(0, getFun.fn);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest,OperationLdc,testing::Range(0,3));

}  // namespace brig
}  // namespace hsa
