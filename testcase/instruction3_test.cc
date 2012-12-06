// Copyright 2012 MulticoreWare Inc.

#include "instruction3_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(Instruction3OpAddInt, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_add_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  BrigOperandWaveSz getWaveSz;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3_add_pair[n].ref;

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

  if((n == 0)||(n == 3)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
 if((n == 1)||(n == 4)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 2)||(n == 5)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getWaveSz);  
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 6)||(n == 9)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if((n == 7)||(n == 10)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 8)||(n == 11)){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getWaveSz);  
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 12)||(n == 15)){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if((n == 13)||(n == 16)){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 14)||(n == 17)){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getWaveSz);  
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 18)||(n == 21)){
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
    EXPECT_EQ(0x1703a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if((n == 19)||(n == 22)){
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
    EXPECT_EQ(0x1703a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 20)||(n == 23)){
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
    EXPECT_EQ(0x1703a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 24)||(n == 27)){
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

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if((n == 25)||(n == 28)){
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

    context->get_operand(ref.o_operands[2], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 26)||(n == 29)){
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

    context->get_operand(ref.o_operands[2], &getWaveSz);  
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }
  if((n == 30)||(n == 33)){
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

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if((n == 31)||(n == 34)){
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

    context->get_operand(ref.o_operands[2], &getImm);  
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x4356789, getImm.bits.u);
  }
  if((n == 32)||(n == 35)){
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

    context->get_operand(ref.o_operands[2], &getWaveSz);
    //BrigOperandWaveSz
    EXPECT_EQ(wavesz_size, getWaveSz.size);
    EXPECT_EQ(BrigEOperandWaveSz, getWaveSz.kind);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAddInt, 
                        testing::Range(0,42));


TEST_P(Instruction3OpAddPackedint, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_addpacked_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3_addpacked_pair[n].ref;

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

  if(n<=3){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n>=4){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAddPackedint, 
                        testing::Range(0,10));



TEST_P(Instruction3OpAddPackedFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_add_packed_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_add_packed_float_pair[n].ref;

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

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n>=1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAddPackedFloat, 
                        testing::Range(0,4));

TEST_P(Instruction3OpCarry, BrigCarry){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_carry_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_carry_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpCarry, 
                        testing::Range(0,8));

TEST_P(Instruction3OpBorrow, BrigBorrow){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_borrow_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_borrow_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpBorrow, 
                        testing::Range(0,8));

TEST_P(Instruction3OpDivInt, BrigDiv){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_div_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_div_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpDivInt, 
                        testing::Range(0,10));

TEST_P(Instruction3OpRem, BrigRem){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_rem_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_rem_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction3OpRem,
                        testing::Range(0,8));

TEST_P(Instruction3OpSubInt, BrigSub){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_sub_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_sub_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpSubInt,
                        testing::Range(0,11));

TEST_P(Instruction3OpSubPackedInt, BrigSub){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_subpacked_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_subpacked_pair[n].ref;

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

  if(n<=3){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n>=4){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpSubPackedInt, 
                        testing::Range(0,10));

TEST_P(Instruction3OpSubPackedFloat, BrigSub){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_sub_packed_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_sub_packed_float_pair[n].ref;

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

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n>=1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpSubPackedFloat, 
                        testing::Range(0,4));

TEST_P(Instruction3OpShl, BrigShl){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_shl_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_shl_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction3OpShl,
                        testing::Range(0,8));

TEST_P(Instruction3OpShlPacked, BrigShl){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_shl_packed_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_shl_packed_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpShlPacked, 
                        testing::Range(0,16));

TEST_P(Instruction3OpShr, BrigShr){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_shr_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_shr_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpShr, 
                        testing::Range(0,8));

TEST_P(Instruction3OpShrPacked, BrigShr){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_shr_packed_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_shr_packed_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpShrPacked, 
                        testing::Range(0,16));

TEST_P(Instruction3OpAnd, BrigAnd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_and_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_and_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAnd, 
                        testing::Range(0,9));

TEST_P(Instruction3OpOr, BrigOr){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_or_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_or_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,Instruction3OpOr,testing::Range(0,9));

TEST_P(Instruction3OpXor, BrigXor){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_xor_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_xor_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpXor, 
                        testing::Range(0,9));

TEST_P(Instruction3OpUnpacklo, BrigUnpacklo){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_unpacklo_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_unpacklo_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpUnpacklo, 
                        testing::Range(0,22));

TEST_P(Instruction3OpUnpackhi, BrigUnpackhi){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_unpackhi_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_unpackhi_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpUnpackhi, 
                        testing::Range(0,22));

TEST_P(Instruction3OpMovdlob64, BrigMovdlo){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_movdlo_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_movdlo_pair[n].ref;

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
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);  

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if(n == 1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x1703a, getImm.bits.u);
  }
  if(n == 2){
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
    EXPECT_EQ(0x38002a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);
  }
  if(n == 3){
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
    EXPECT_EQ(0x38002a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x1703a, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpMovdlob64, 
                        testing::Range(0,4));

TEST_P(Instruction3OpMovdhib64, BrigMovdhi){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op__movdhi_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op__movdhi_pair[n].ref;

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
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset, getReg.s_name);  

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if(n == 1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x1703a, getImm.bits.u);
  }
  if(n == 2){
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
    EXPECT_EQ(0x38002a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);
  }
  if(n == 3){
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
    EXPECT_EQ(0x38002a, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(0x1703a, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpMovdhib64, 
                        testing::Range(0,4));

TEST_P(Instruction3OpCopySignFloat, BrigCopysign){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_copysign_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3_copysign_pair[n].ref;

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

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if(n == 1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(7.2f, getImm.bits.f);
  }
  if(n == 2){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n == 3){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(7.2f, getImm.bits.f);
  }
  if(n == 4){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);
  }
  if(n == 5){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(7.2, getImm.bits.d);
  }
  if(n == 6){
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
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(1.0f, getImm.bits.f);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);
  }
  if(n == 7){
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
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(1.0f, getImm.bits.f);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb16, getImm.type);
    EXPECT_EQ(7.2f, getImm.bits.f);
  }
  if(n == 8){
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
    EXPECT_EQ(1.0f, getImm.bits.f);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);
  }
  if(n == 9){
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
    EXPECT_EQ(1.0f, getImm.bits.f);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(7.2f, getImm.bits.f);
  }
  if(n == 10){
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
    EXPECT_EQ(1.0, getImm.bits.d);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb64, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);
  }
  if(n == 11){
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
    EXPECT_EQ(1.0, getImm.bits.d);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb64, getImm.type);
    EXPECT_EQ(7.2, getImm.bits.d);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpCopySignFloat, 
                        testing::Range(0,12));

TEST_P(Instruction3OpClassFloat, BrigClass){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_classfloat_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3_classfloat_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                        Instruction3OpClassFloat,
                        testing::Range(0,8));

TEST_P(Instruction3OpSend, BrigSend){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_send_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3_send_pair[n].ref;

  EXPECT_EQ(ref.size, getBase.size);
  EXPECT_EQ(ref.kind, getBase.kind);
  EXPECT_EQ(ref.opcode, getBase.opcode);
  EXPECT_EQ(ref.packing, getBase.packing);
  EXPECT_EQ(ref.type, getBase.type);
  EXPECT_EQ(ref.o_operands[0], getBase.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], getBase.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], getBase.o_operands[2]);

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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if(n == 1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(3, getImm.bits.u);
  }
  if(n == 2){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if(n == 3){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(3, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpSend, 
                        testing::Range(0,4));

TEST_P(Instruction3OpReceive, BrigReceive){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_receive_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstBase getBase;
  BrigOperandReg getReg;
  BrigOperandImmed getImm;
  context->get_code(code_offset, &getBase);
  BrigInstBase ref = instruction3op_receive_pair[n].ref;

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

    context->get_operand(ref.o_operands[1], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4 * 2, getReg.s_name);  
  }
  if(n == 1){
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
    EXPECT_EQ(string_offset + 4, getReg.s_name);  

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(3, getImm.bits.u);
  }
  if(n == 2){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getReg);
    // BrigOperandReg
    EXPECT_EQ(reg_size, getReg.size);
    EXPECT_EQ(BrigEOperandReg, getReg.kind);
    EXPECT_EQ(Brigb32, getReg.type);
    EXPECT_EQ(0, getReg.reserved);
    EXPECT_EQ(string_offset + 4, getReg.s_name);  
  }
  if(n == 3){
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
    EXPECT_EQ(0x100011, getImm.bits.u);

    context->get_operand(ref.o_operands[2], &getImm);
    // BrigOperandImmed
    EXPECT_EQ(immed_size, getImm.size);
    EXPECT_EQ(BrigEOperandImmed, getImm.kind);
    EXPECT_EQ(Brigb32, getImm.type);
    EXPECT_EQ(3, getImm.bits.u);
  }

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpReceive, 
                        testing::Range(0,4));

TEST_P(Instruction3OpAddFtzRoundFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_add_ftz_round_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3_add_ftz_round_float_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAddFtzRoundFloat, 
                        testing::Range(0,8));

TEST_P(Instruction3OpDivFtzRoundFloat, BrigDiv){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_div_ftz_round_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3op_div_ftz_round_float_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpDivFtzRoundFloat, 
                        testing::Range(0,8));

TEST_P(Instruction3OpSubFtzRoundFloat, BrigSub){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_sub_ftz_round_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3op_sub_ftz_round_float_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpSubFtzRoundFloat, 
                        testing::Range(0,8));

TEST_P(Instruction3OpAddFloat, BrigAdd){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3_addfloat_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3_addfloat_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                        Instruction3OpAddFloat, 
                        testing::Range(0,6));

TEST_P(Instruction3OpDivFloat, BrigDiv){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_div_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3op_div_float_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, 
                       Instruction3OpDivFloat, 
                       testing::Range(0,4));
TEST_P(Instruction3OpSubFloat, BrigSub){
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  Lexer* lexer = new Lexer(instruction3op_sub_float_pair[n].str);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction3(context));

  BrigInstMod getMod;
  context->get_code(code_offset, &getMod);
  BrigInstMod ref = instruction3op_sub_float_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(OperationTest, 
                        Instruction3OpSubFloat, 
                        testing::Range(0,3));


}  // namespace brig
}  // namespace hsa
