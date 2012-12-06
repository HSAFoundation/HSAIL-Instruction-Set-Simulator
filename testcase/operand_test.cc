// Copyright 2012 MulticoreWare Inc.

#include "operand_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

// ------------------ BASIC LEXER TESTS -----------------
TEST(LexTest, CRegisterInvalidInput) {
  std::string input("$c20");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_CREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$c");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$c8");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_CREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, SRegisterInvalidInput) {
  std::string input("$s17");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$s");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$c0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_SREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, DRegisterInvalidInput) {
  std::string input("$d8");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$d");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$c0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$q0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_DREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, QRegisterInvalidInput) {
  std::string input("$q20");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_QREGISTER, lexer->get_next_token());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());

  input.assign("$q");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$q8");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$@0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$s0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$d0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  input.assign("$f0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_QREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, LabelInvalidInput) {
  std::string input("@123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@ _GO_TO_THIS");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("@@");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LABEL, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, LocalIdInvalidInput) {
  std::string input("%123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("% _Test_id_123");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("%test@123");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, GlobalIdInvalidInput) {
  std::string input("&123");
  Lexer* lexer = new Lexer(input);

  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&123_go");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&$");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("& _Test_id_123");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("&test@123");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  input.assign("$C0");
  lexer->set_source_string(input);
  EXPECT_NE(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST_P(IntegerConstant, IntegerConstants) {
  int n = GetParam();
  Lexer* lexer = new Lexer(integerconstant_pair[n].str);

  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(integerconstant_pair[n].ref, context->token_value.int_val);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, IntegerConstant, testing::Range(0, 21));

TEST_P(CRegisterValidInput, CRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_cregister[n]);
  EXPECT_EQ(TOKEN_CREGISTER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, CRegisterValidInput, testing::Range(0, 8));

TEST_P(SRegisterValidInput, SRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_sregister[n]);
  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, SRegisterValidInput, testing::Range(0, 16));

TEST_P(DRegisterValidInput, DRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_dregister[n]);
  EXPECT_EQ(TOKEN_DREGISTER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, DRegisterValidInput, testing::Range(0, 8));

TEST_P(QRegisterValidInput, QRegister) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_qregister[n]);
  EXPECT_EQ(TOKEN_QREGISTER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, QRegisterValidInput, testing::Range(0, 8));

TEST_P(LabelValidInput, Label) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_label[n]);
  EXPECT_EQ(TOKEN_LABEL, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, LabelValidInput, testing::Range(0, 9));

TEST_P(LocalIdValidInput, LocalId) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_localid[n]);
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, LocalIdValidInput, testing::Range(0, 9));

TEST_P(GlobalIdValidInput, GlobalId) {
  int n = GetParam();
  Lexer* lexer = new Lexer(input_globalid[n]);
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(LexTest, GlobalIdValidInput, testing::Range(0, 9));

// ------------------ BASIC CODEGEN TESTS -----------------
TEST(CodegenTest, IntegerOperandCodeGen) {
  /* Integer */
  context->set_error_reporter(main_reporter);
  context->clear_context();

  BrigOperandImmed ref = {
    sizeof(ref),        // size
    BrigEOperandImmed,  // kind
    Brigb32,            // type
    0,                  // reserved
    { 0 }
  };

  std::string input("0");
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.bits.u = 0;
  BrigOperandImmed get;
  context->get_operand<BrigOperandImmed>(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("00");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 0;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("0x00");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 0;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("012");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 10;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("0x12");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 18;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("0xAb");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 171;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("0X0ff");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 255;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("0xFF");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = 255;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  /* Negative Integer */

  input.assign("-1");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = (unsigned int) (-1);
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("-1234567");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = (unsigned int) (-1234567);
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);
  context->clear_context();

  input.assign("-2147483648");
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  ref.type = Brigb32;
  ref.bits.u = (unsigned int) (-2147483648);
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  delete lexer;
}

TEST_P(RegisterOperand, Register) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(registeroperand_pair[n].str);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  BrigOperandReg ref = registeroperand_pair[n].ref;

  BrigOperandReg get;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.s_name, get.s_name);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           RegisterOperand,
                           testing::Range(0, 32));

TEST_P(RegOperandInvalidIn, Register) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(invalidinput_registercodegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Operand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                        RegOperandInvalidIn,
                        testing::Range(0, 6));

TEST_P(IntegerOperand, Integer) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(integer_codegen_pair[n].str);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  BrigOperandImmed get;
  context->get_operand(operand_offset, &get);

  BrigOperandImmed ref = integer_codegen_pair[n].ref;
  if (n == 5){
    ref.bits.u = (unsigned int)(-99);
  }else if (n == 6){
    ref.bits.u = (unsigned int)(-1234567);
  }else if (n == 7){
    ref.bits.u = (unsigned int)(-2147483648);
  }else if (n == 11){
    ref.bits.u = (unsigned int)(-10);
  }else if (n ==14){
    ref.bits.u = (unsigned int)(-18);
  }
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.bits.u, get.bits.u);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                         IntegerOperand,
                         testing::Range(0, 13));

TEST_P(IntOperandInvalidIn, Integer) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(invalidinput_integercodegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Operand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                           IntOperandInvalidIn,
                           testing::Range(0, 2));


TEST_P(DoubleOperand, Double) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_double_codegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  BrigOperandImmed ref;
  ref.size = immed_size;
  ref.kind = BrigEOperandImmed;
  ref.type = Brigb64;
  ref.reserved = 0;

  BrigOperandImmed get;
  context->get_operand(operand_offset, &get);
     if (n < 13){
    ref.bits.d = 12.345;
  }else if (n < 16){
    ref.bits.d = -12.345;
  }else if (n < 18){
    ref.bits.d = 12345;
  }else if (n <21){
    ref.bits.d = 0;
  }else if (n == 21){
    ref.bits.d = 101;
  }else if (n == 22){
    ref.bits.d = 7;
  }else if (n == 23){
    ref.bits.d = 0.77;
  }else if (n == 24){
    ref.bits.d = 0.007;
  }else if (n < 27){
    ref.bits.d = 7;
  }else if (n == 27){
    ref.bits.d = 30;
  }else if (n == 28){
    ref.bits.d = 0;
  }else if (n == 29){
    ref.bits.d = 900;
  }
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.bits.d, get.bits.d);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           DoubleOperand,
                           testing::Range(0, 30));

TEST_P(DoubleOperandInvalid, Double) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(invalidinput_doublecodegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Operand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                        DoubleOperandInvalid,
                        testing::Range(0, 6));

TEST_P(SingleOperand, Single) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_single_codegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  BrigOperandImmed ref;
  ref.size = immed_size;
  ref.kind = BrigEOperandImmed;
  ref.type = Brigb32;
  ref.reserved = 0;

  BrigOperandImmed get;
  context->get_operand(operand_offset, &get);
     if (n < 7){
    ref.bits.f = 12.345;
  }else if (n < 11){
    ref.bits.f = -12.345;
  }else if (n == 11){
    ref.bits.f = 12345;
  }else if (n < 15 ){
    ref.bits.f = 0;
  }else if (n == 15){
    ref.bits.f = 101;
  }else if (n == 16){
    ref.bits.f = 7;
  }else if (n == 17){
    ref.bits.f = 0.77;
  }else if (n == 18){
    ref.bits.f = 0.007;
  }else if (n == 19){
    ref.bits.f = 7;
  }else if (n == 20){
    ref.bits.f = -7;
  }else if (n == 21){
    ref.bits.f = 30;
  }else if (n == 22){
    ref.bits.f = 0;
  }else if (n == 23){
    ref.bits.f = -900;
  }else if (n == 24){
    ref.bits.f = 9.5625;
  }else if (n == 25){
    ref.bits.f = 9.5625 * 2;
  }
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.bits.f, get.bits.f);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           SingleOperand,
                           testing::Range(0, 26));

TEST_P(SingleOperandInvalid, Single) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(invalidinput_singlecodegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Operand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                        SingleOperandInvalid,
                        testing::Range(0, 6));

TEST_P(WavesizeOperand, Wavesize) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(input_wavesize_codegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Operand(context));

  BrigOperandWaveSz ref ={
    wavesz_size,
    BrigEOperandWaveSz,
  };

  BrigOperandWaveSz get;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           WavesizeOperand,
                           testing::Range(0, 1));

TEST_P(WavesizeOperandInvalid, Double) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(invalidinput_wavesizecodegen[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Operand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                        WavesizeOperandInvalid,
                        testing::Range(0, 3));

TEST_P(TestAddressOperand, AddressOperand) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(address_operand_pair[n].str);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, MemoryOperand(context));

  BrigOperandAddress ref = address_operand_pair[n].ref;
  BrigOperandAddress get;
  context->get_operand(operand_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.directive, get.directive);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                         TestAddressOperand,
                         testing::Range(0, 2));

TEST_P(TestIndirectOperand, IndirectOperand) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(indirect_operand_pair[n].str);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  if(n>=3 and n<=4) context->set_machine(BrigESmall);
  EXPECT_EQ(0, MemoryOperand(context));

  BrigOperandIndirect ref = indirect_operand_pair[n].ref;
  BrigOperandIndirect get;
  if (n < 3){
    context->get_operand(operand_offset, &get);
  }else {
    context->get_operand(operand_offset + reg_size, &get);
  };
  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.reg, get.reg);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.offset, get.offset);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                         TestIndirectOperand,
                         testing::Range(0, 6));

TEST_P(TestCompoundOperand, CompoundOperand) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(compound_operand_pair[n].str);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();

  if(n>=3 and n<=4) context->set_machine(BrigESmall);
  EXPECT_EQ(0, MemoryOperand(context));

  BrigOperandCompound ref = compound_operand_pair[n].ref;
  BrigOperandCompound get;
  if (n < 3){
    context->get_operand(operand_offset + address_size, &get);
  }else {
    context->get_operand(operand_offset + reg_size + address_size, &get);
  };

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.reserved, get.reserved);
  EXPECT_EQ(ref.name, get.name);
  EXPECT_EQ(ref.reg, get.reg);
  EXPECT_EQ(ref.offset, get.offset);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                         TestCompoundOperand,
                         testing::Range(0, 6));

TEST_P(TestMemoryInvalidInput, MemoryOperand) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_memory_invalid[n]);

  Lexer* lexer = new Lexer();
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, MemoryOperand(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest,
                           TestMemoryInvalidInput,
                           testing::Range(0, 2));

}  // namespace brig
}  // namespace hsa
