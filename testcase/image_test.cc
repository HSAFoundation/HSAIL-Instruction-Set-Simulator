// Copyright 2012 MulticoreWare Inc.

#include "image_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestLdImage, LdImage) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageload_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 30;
  EXPECT_EQ(0, ImageLoad(context));

  BrigInstImage ref = imageload_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  switch (n) {
    case 0 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 1 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 2 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 3 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 4 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 5 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 6 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 7 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 8 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 10 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 11 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }

  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestLdImage, testing::Range(0,12));

TEST_P(TestLdImageInvalid, LdImageInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageload_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageLoad(context));

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestLdImageInvalid, testing::Range(0,15));

TEST_P(TestImageNoRetAdd, ImageNoRetAdd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_add_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_add_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetAdd, testing::Range(0,6));

TEST_P(TestImageNoRetSub, ImageNoRetSub) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_sub_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_sub_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(Codegen, TestImageNoRetSub, testing::Range(0,6));

TEST_P(TestImageNoRetOr, ImageNoRetOr) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_or_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_or_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetOr, testing::Range(0,6));

TEST_P(TestImageNoRetAnd, ImageNoRetAnd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_and_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_and_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetAnd, testing::Range(0,6));

TEST_P(TestImageNoRetXor, ImageNoRetXor) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_xor_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_xor_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetXor, testing::Range(0,6));

TEST_P(TestImageNoRetInc, ImageNoRetInc) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_inc_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_inc_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetInc, testing::Range(0,6));

TEST_P(TestImageNoRetDec, ImageNoRetDec) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_dec_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_dec_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetDec, testing::Range(0,6));

TEST_P(TestImageNoRetMax, ImageNoRetMax) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_max_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_max_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetMax, testing::Range(0,6));

TEST_P(TestImageNoRetMin, ImageNoRetMin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_min_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_min_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetMin, testing::Range(0,6));

TEST_P(TestImageNoRetCas, ImageNoRetCas) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagenoret_cas_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageNoRet(context));

  BrigInstAtomicImage ref = imagenoret_cas_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageNoRetCas, testing::Range(0,6));

TEST_P(TestRdImage1d, RdImage1d) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageread_1d_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 0xf7;
  context->global_symbol_map["%Samp3"] = 0xf1;
  EXPECT_EQ(0, ImageRead(context));

  BrigInstImage ref = imageread_1d_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4, getReg5;
  BrigOperandOpaque getImage1, getImage2;
  BrigOperandRegV4 getRegV4;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

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

  context->get_operand(ref.o_operands[1], &getImage1);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage1.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage1.kind);
  EXPECT_EQ(0xf7, getImage1.directive);
  EXPECT_EQ(0, getImage1.reg);
  EXPECT_EQ(0, getImage1.offset);

  context->get_operand(ref.o_operands[2], &getImage2);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage2.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage2.kind);
  EXPECT_EQ(0xf1, getImage2.directive);
  EXPECT_EQ(0, getImage2.reg);
  EXPECT_EQ(0, getImage2.offset);

  context->get_operand(ref.o_operands[3], &getReg5);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg5.size);
  EXPECT_EQ(BrigEOperandReg, getReg5.kind);
  EXPECT_EQ(Brigb32, getReg5.type);
  EXPECT_EQ(0, getReg5.reserved);
  EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestRdImage1d, testing::Range(0,6));

TEST_P(TestRdImage1dInvalid, RdImage1dInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageread_1d_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageRead(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestRdImage1dInvalid, testing::Range(0,20));

TEST_P(TestRdImage2d, RdImage2d) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageread_2d_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 0xf7;
  context->global_symbol_map["%Samp3"] = 0xf1;
  EXPECT_EQ(0, ImageRead(context));

  BrigInstImage ref = imageread_2d_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4, getReg5, getReg6;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage1, getImage2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

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

  context->get_operand(ref.o_operands[1], &getImage1);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage1.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage1.kind);
  EXPECT_EQ(0xf7, getImage1.directive);
  EXPECT_EQ(0, getImage1.reg);
  EXPECT_EQ(0, getImage1.offset);

  context->get_operand(ref.o_operands[2], &getImage2);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage2.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage2.kind);
  EXPECT_EQ(0xf1, getImage2.directive);
  EXPECT_EQ(0, getImage2.reg);
  EXPECT_EQ(0, getImage2.offset);

  context->get_operand(ref.o_operands[3] - reg_size * 2, &getReg5);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg5.size);
  EXPECT_EQ(BrigEOperandReg, getReg5.kind);
  EXPECT_EQ(Brigb32, getReg5.type);
  EXPECT_EQ(0, getReg5.reserved);
  EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);

  context->get_operand(ref.o_operands[3] - reg_size, &getReg6);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg6.size);
  EXPECT_EQ(BrigEOperandReg, getReg6.kind);
  EXPECT_EQ(Brigb32, getReg6.type);
  EXPECT_EQ(0, getReg6.reserved);
  EXPECT_EQ(string_offset + 4 * 5, getReg6.s_name);

  context->get_operand(ref.o_operands[3], &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(regv2_size, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(ref.o_operands[3] - reg_size * 2, getRegV2.regs[0]);
  EXPECT_EQ(ref.o_operands[3] - reg_size, getRegV2.regs[1]);
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestRdImage2d, testing::Range(0,6));

TEST_P(TestRdImage2dInvalid, RdImage2dInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageread_2d_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageRead(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestRdImage2dInvalid, testing::Range(0,20));

TEST_P(TestRdImage3d, RdImage3d) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageread_3d_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 0xf7;
  context->global_symbol_map["%Samp3"] = 0xf1;
  EXPECT_EQ(0, ImageRead(context));

  BrigInstImage ref = imageread_3d_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandReg getReg5, getReg6, getReg7, getReg8;
  BrigOperandRegV4 getRegV4_1, getRegV4_2;
  BrigOperandOpaque getImage1, getImage2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

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

  context->get_operand(ref.o_operands[0], &getRegV4_1);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4_1.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4_1.kind);
  EXPECT_EQ(Brigb32, getRegV4_1.type);
  EXPECT_EQ(0, getRegV4_1.reserved);
  EXPECT_EQ(operand_offset, getRegV4_1.regs[0]);
  EXPECT_EQ(operand_offset + reg_size, getRegV4_1.regs[1]);
  EXPECT_EQ(operand_offset + reg_size * 2, getRegV4_1.regs[2]);
  EXPECT_EQ(operand_offset + reg_size * 3, getRegV4_1.regs[3]);

  context->get_operand(ref.o_operands[1], &getImage1);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage1.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage1.kind);
  EXPECT_EQ(0xf7, getImage1.directive);
  EXPECT_EQ(0, getImage1.reg);
  EXPECT_EQ(0, getImage1.offset);

  context->get_operand(ref.o_operands[2], &getImage2);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage2.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage2.kind);
  EXPECT_EQ(0xf1, getImage2.directive);
  EXPECT_EQ(0, getImage2.reg);
  EXPECT_EQ(0, getImage2.offset);

  context->get_operand(ref.o_operands[3] - 4 * reg_size, &getReg5);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg5.size);
  EXPECT_EQ(BrigEOperandReg, getReg5.kind);
  EXPECT_EQ(Brigb32, getReg5.type);
  EXPECT_EQ(0, getReg5.reserved);
  EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);

  context->get_operand(ref.o_operands[3] - 3 * reg_size, &getReg6);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg6.size);
  EXPECT_EQ(BrigEOperandReg, getReg6.kind);
  EXPECT_EQ(Brigb32, getReg6.type);
  EXPECT_EQ(0, getReg6.reserved);
  EXPECT_EQ(string_offset + 4 * 5, getReg6.s_name);

  context->get_operand(ref.o_operands[3] - 2 * reg_size, &getReg7);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg7.size);
  EXPECT_EQ(BrigEOperandReg, getReg7.kind);
  EXPECT_EQ(Brigb32, getReg7.type);
  EXPECT_EQ(0, getReg7.reserved);
  EXPECT_EQ(string_offset + 4 * 6, getReg7.s_name);

  context->get_operand(ref.o_operands[3] - reg_size, &getReg8);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg8.size);
  EXPECT_EQ(BrigEOperandReg, getReg8.kind);
  EXPECT_EQ(Brigb32, getReg8.type);
  EXPECT_EQ(0, getReg8.reserved);
  EXPECT_EQ(string_offset + 4 * 7, getReg8.s_name);

  context->get_operand(ref.o_operands[3], &getRegV4_2);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4_2.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4_2.kind);
  EXPECT_EQ(Brigb32, getRegV4_2.type);
  EXPECT_EQ(0, getRegV4_2.reserved);
  EXPECT_EQ(ref.o_operands[3] - 4 * reg_size, getRegV4_2.regs[0]);
  EXPECT_EQ(ref.o_operands[3] - 3 * reg_size, getRegV4_2.regs[1]);
  EXPECT_EQ(ref.o_operands[3] - 2 * reg_size, getRegV4_2.regs[2]);
  EXPECT_EQ(ref.o_operands[3] - reg_size, getRegV4_2.regs[3]);
  delete lexer;

}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestRdImage3d, testing::Range(0,6));

TEST_P(TestRdImage3dInvalid, RdImage3dInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageread_3d_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageRead(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestRdImage3dInvalid, testing::Range(0,20));

TEST_P(TestRdImage1da, RdImage1da) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageread_1da_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 0xf7;
  context->global_symbol_map["%Samp3"] = 0xf1;
  EXPECT_EQ(0, ImageRead(context));

  BrigInstImage ref = imageread_1da_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandReg getReg5, getReg6;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage1, getImage2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

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

  context->get_operand(ref.o_operands[1], &getImage1);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage1.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage1.kind);
  EXPECT_EQ(0xf7, getImage1.directive);
  EXPECT_EQ(0, getImage1.reg);
  EXPECT_EQ(0, getImage1.offset);

  context->get_operand(ref.o_operands[2], &getImage2);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage2.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage2.kind);
  EXPECT_EQ(0xf1, getImage2.directive);
  EXPECT_EQ(0, getImage2.reg);
  EXPECT_EQ(0, getImage2.offset);

  context->get_operand(ref.o_operands[3] - reg_size * 2, &getReg5);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg5.size);
  EXPECT_EQ(BrigEOperandReg, getReg5.kind);
  EXPECT_EQ(Brigb32, getReg5.type);
  EXPECT_EQ(0, getReg5.reserved);
  EXPECT_EQ(string_offset + 4 * 4, getReg5.s_name);

  context->get_operand(ref.o_operands[3] - reg_size, &getReg6);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg6.size);
  EXPECT_EQ(BrigEOperandReg, getReg6.kind);
  EXPECT_EQ(Brigb32, getReg6.type);
  EXPECT_EQ(0, getReg6.reserved);
  EXPECT_EQ(string_offset + 4 * 5, getReg6.s_name);

  context->get_operand(ref.o_operands[3], &getRegV2);
  // BrigOperandRegV2
  EXPECT_EQ(regv2_size, getRegV2.size);
  EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
  EXPECT_EQ(Brigb32, getRegV2.type);
  EXPECT_EQ(0, getRegV2.reserved);
  EXPECT_EQ(ref.o_operands[3] - 2 * reg_size, getRegV2.regs[0]);
  EXPECT_EQ(ref.o_operands[3] - reg_size, getRegV2.regs[1]);
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestRdImage1da, testing::Range(0,6));

TEST_P(TestRdImage1daInvalid, RdImage1daInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageread_1da_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageRead(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestRdImage1daInvalid, testing::Range(0,20));

TEST_P(TestRdImage2da, RdImage2da) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageread_2da_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 0xf7;
  context->global_symbol_map["%Samp3"] = 0xf1;
  EXPECT_EQ(0, ImageRead(context));

  BrigInstImage ref = imageread_2da_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg1, getReg2, getReg3, getReg4;
  BrigOperandReg getReg5, getReg6, getReg7, getReg8;
  BrigOperandRegV4 getRegV4_1, getRegV4_2;
  BrigOperandOpaque getImage1, getImage2;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

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

  context->get_operand(ref.o_operands[0], &getRegV4_1);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4_1.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4_1.kind);
  EXPECT_EQ(Brigb32, getRegV4_1.type);
  EXPECT_EQ(0, getRegV4_1.reserved);
  EXPECT_EQ(operand_offset, getRegV4_1.regs[0]);
  EXPECT_EQ(operand_offset + reg_size, getRegV4_1.regs[1]);
  EXPECT_EQ(operand_offset + reg_size * 2, getRegV4_1.regs[2]);
  EXPECT_EQ(operand_offset + reg_size * 3, getRegV4_1.regs[3]);

  context->get_operand(ref.o_operands[1], &getImage1);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage1.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage1.kind);
  EXPECT_EQ(0xf7, getImage1.directive);
  EXPECT_EQ(0, getImage1.reg);
  EXPECT_EQ(0, getImage1.offset);

  context->get_operand(ref.o_operands[2], &getImage2);
  // BrigOperandOpaque
  EXPECT_EQ(opaque_size, getImage2.size);
  EXPECT_EQ(BrigEOperandOpaque, getImage2.kind);
  EXPECT_EQ(0xf1, getImage2.directive);
  EXPECT_EQ(0, getImage2.reg);
  EXPECT_EQ(0, getImage2.offset);

  context->get_operand(ref.o_operands[3] - 4 * reg_size, &getReg5);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg5.size);
  EXPECT_EQ(BrigEOperandReg, getReg5.kind);
  EXPECT_EQ(Brigb32, getReg5.type);
  EXPECT_EQ(0, getReg5.reserved);
  EXPECT_EQ(operand_offset + 4 * 4, getReg5.s_name);

  context->get_operand(ref.o_operands[3] - 3 * reg_size, &getReg6);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg6.size);
  EXPECT_EQ(BrigEOperandReg, getReg6.kind);
  EXPECT_EQ(Brigb32, getReg6.type);
  EXPECT_EQ(0, getReg6.reserved);
  EXPECT_EQ(operand_offset + 4 * 5, getReg6.s_name);

  context->get_operand(ref.o_operands[3] - 2 * reg_size, &getReg7);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg7.size);
  EXPECT_EQ(BrigEOperandReg, getReg7.kind);
  EXPECT_EQ(Brigb32, getReg7.type);
  EXPECT_EQ(0, getReg7.reserved);
  EXPECT_EQ(operand_offset + 4 * 6, getReg7.s_name);

  context->get_operand(ref.o_operands[3] - reg_size, &getReg8);
  // BrigOperandReg
  EXPECT_EQ(reg_size, getReg8.size);
  EXPECT_EQ(BrigEOperandReg, getReg8.kind);
  EXPECT_EQ(Brigb32, getReg8.type);
  EXPECT_EQ(0, getReg8.reserved);
  EXPECT_EQ(operand_offset + 4 * 7, getReg8.s_name);

  context->get_operand(ref.o_operands[3], &getRegV4_2);
  // BrigOperandRegV4
  EXPECT_EQ(regv4_size, getRegV4_2.size);
  EXPECT_EQ(BrigEOperandRegV4, getRegV4_2.kind);
  EXPECT_EQ(Brigb32, getRegV4_2.type);
  EXPECT_EQ(0, getRegV4_2.reserved);
  EXPECT_EQ(ref.o_operands[3] - 4 * reg_size, getRegV4_2.regs[0]);
  EXPECT_EQ(ref.o_operands[3] - 3 * reg_size, getRegV4_2.regs[1]);
  EXPECT_EQ(ref.o_operands[3] - 2 * reg_size, getRegV4_2.regs[2]);
  EXPECT_EQ(ref.o_operands[3] - reg_size, getRegV4_2.regs[3]);
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestRdImage2da, testing::Range(0,6));

TEST_P(TestRdImage2daInvalid, RdImage2daInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imageread_2da_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageRead(context));
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestRdImage2daInvalid, testing::Range(0,21));

TEST_P(TestImageRetAdd, ImageRetAdd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_add_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_add_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetAdd, testing::Range(0,6));

TEST_P(TestImageRetSub, ImageRetSub) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_sub_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_sub_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetSub, testing::Range(0,6));

TEST_P(TestImageRetOr, ImageRetOr) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_or_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_or_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetOr, testing::Range(0,6));

TEST_P(TestImageRetAnd, ImageRetAnd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_and_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_and_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetAnd, testing::Range(0,6));

TEST_P(TestImageRetXor, ImageRetXor) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_xor_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_xor_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetXor, testing::Range(0,6));

TEST_P(TestImageRetInc, ImageRetInc) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_inc_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_inc_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetInc, testing::Range(0,6));

TEST_P(TestImageRetDec, ImageRetDec) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_dec_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_dec_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetDec, testing::Range(0,6));

TEST_P(TestImageRetMax, ImageRetMax) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_max_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_max_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetMax, testing::Range(0,6));

TEST_P(TestImageRetMin, ImageRetMin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_min_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_min_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetMin, testing::Range(0,6));

TEST_P(TestImageRetCas, ImageRetCas) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imageret_cas_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, ImageRet(context));

  BrigInstAtomicImage ref = imageret_cas_pair[n].ref;
  BrigInstAtomicImage get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageRetCas, testing::Range(0,6));

TEST_P(TestImageStore, ImageStore) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(imagestore_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  context->global_symbol_map["%RWImg3"] = 30;
  EXPECT_EQ(0, ImageStore(context));

  BrigInstImage ref = imagestore_pair[n].ref;

  BrigInstImage get;
  BrigOperandReg getReg;
  BrigOperandRegV2 getRegV2;
  BrigOperandRegV4 getRegV4;
  BrigOperandOpaque getImage;

  context->get_code(code_offset, &get);
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.sourceType, get.sourceType);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.geom, get.geom);

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);

  switch (n) {
    case 0 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 1 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 2 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 3 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 4 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 5 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 6 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 7 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV2);
      // BrigOperandRegV2
      EXPECT_EQ(regv2_size, getRegV2.size);
      EXPECT_EQ(BrigEOperandRegV2, getRegV2.kind);
      EXPECT_EQ(Brigb32, getRegV2.type);
      EXPECT_EQ(0, getRegV2.reserved);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV2.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV2.regs[1]);
      break;
    }
    case 8 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 9 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getRegV4);
      // BrigOperandRegV4
      EXPECT_EQ(regv4_size, getRegV4.size);
      EXPECT_EQ(BrigEOperandRegV4, getRegV4.kind);
      EXPECT_EQ(Brigb32, getRegV4.type);
      EXPECT_EQ(0, getRegV4.reserved);
      EXPECT_EQ(ref.o_operands[2] - 4*reg_size, getRegV4.regs[0]);
      EXPECT_EQ(ref.o_operands[2] - 3*reg_size, getRegV4.regs[1]);
      EXPECT_EQ(ref.o_operands[2] - 2*reg_size, getRegV4.regs[2]);
      EXPECT_EQ(ref.o_operands[2] - reg_size, getRegV4.regs[3]);
      break;
    }
    case 10 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaquee
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
    case 11 : {
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

      context->get_operand(ref.o_operands[1], &getImage);
      // BrigOperandOpaque
      EXPECT_EQ(opaque_size, getImage.size);
      EXPECT_EQ(BrigEOperandOpaque, getImage.kind);
      EXPECT_EQ(30, getImage.directive);
      EXPECT_EQ(0, getImage.reg);
      EXPECT_EQ(0, getImage.offset);

      context->get_operand(ref.o_operands[2], &getReg);
      // BrigOperandReg
      EXPECT_EQ(reg_size, getReg.size);
      EXPECT_EQ(BrigEOperandReg, getReg.kind);
      EXPECT_EQ(Brigb32, getReg.type);
      EXPECT_EQ(0, getReg.reserved);
      EXPECT_EQ(string_offset + 4 * 4, getReg.s_name);
      break;
    }
  }
  delete lexer;
}
INSTANTIATE_TEST_CASE_P(CodegenTest, TestImageStore, testing::Range(0,12));

TEST_P(TestImageStoreInvalid, ImageStore) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_array_imagestore_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, ImageStore(context));

  delete lexer;
}
INSTANTIATE_TEST_CASE_P(InvalidTest, TestImageStoreInvalid, testing::Range(0,59));


}  // namespace brig
}  // namespace hsa
