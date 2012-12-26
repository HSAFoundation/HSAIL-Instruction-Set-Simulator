// Copyright 2012 MulticoreWare Inc.

#include "atom_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestAtomicAnd, AtomicAnd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_and_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_and_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicAnd, testing::Range(0,6));

TEST_P(TestAtomicOr, AtomicOr) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_or_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_or_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicOr, testing::Range(0,6));

TEST_P(TestAtomicXor, AtomicXor) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_xor_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_xor_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicXor, testing::Range(0,6));

TEST_P(TestAtomicExch, AtomicExch) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_exch_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_exch_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicExch, testing::Range(0,6));

TEST_P(TestAtomicAdd, AtomicAdd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_add_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_add_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicAdd, testing::Range(0,6));

TEST_P(TestAtomicSub, AtomicSub) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_sub_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_sub_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicSub, testing::Range(0,6));

TEST_P(TestAtomicInc, AtomicInc) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_inc_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_inc_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicInc, testing::Range(0,6));

TEST_P(TestAtomicDec, AtomicDec) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_dec_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_dec_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicDec, testing::Range(0,6));

TEST_P(TestAtomicMax, AtomicMax) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_max_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_max_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicMax, testing::Range(0,6));

TEST_P(TestAtomicMin, AtomicMin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_min_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_min_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicMin, testing::Range(0,6));

TEST_P(TestAtomicCas, AtomicCas) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomic_cas_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Atom(context));

  BrigInstAtomic ref = atomic_cas_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicCas, testing::Range(0,6));

TEST_P(TestAtomicNoRetAnd, AtomicNoRetAnd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_and_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_and_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetAnd, testing::Range(0,6));

TEST_P(TestAtomicNoRetOr, AtomicNoRetOr) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_or_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_or_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetOr, testing::Range(0,6));

TEST_P(TestAtomicNoRetXor, AtomicNoRetXor) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_xor_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_xor_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetXor, testing::Range(0,6));

TEST_P(TestAtomicNoRetAdd, AtomicNoRetAdd) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_add_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_add_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetAdd, testing::Range(0,6));

TEST_P(TestAtomicNoRetSub, AtomicNoRetSub) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_sub_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_sub_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetSub, testing::Range(0,6));

TEST_P(TestAtomicNoRetInc, AtomicNoRetInc) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_inc_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_inc_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetInc, testing::Range(0,6));

TEST_P(TestAtomicNoRetDec, AtomicNoRetDec) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_dec_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_dec_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetDec, testing::Range(0,6));

TEST_P(TestAtomicNoRetMax, AtomicNoRetMax) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_max_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_max_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetMax, testing::Range(0,6));

TEST_P(TestAtomicNoRetMin, AtomicNoRetMin) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_min_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_min_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetMin, testing::Range(0,6));

TEST_P(TestAtomicNoRetCas, AtomicNoRetCas) {
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&x"] = 40;

  int n = GetParam();
  std::string input(atomicnoret_cas_pair[n].str);
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, AtomicNoRet(context));

  BrigInstAtomic ref = atomicnoret_cas_pair[n].ref;
  BrigInstAtomic get;
  context->get_code(code_offset, &get);

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.atomicOperation, get.atomicOperation);
  EXPECT_EQ(ref.storageClass, get.storageClass);
  EXPECT_EQ(ref.memorySemantic, get.memorySemantic); 

  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestAtomicNoRetCas, testing::Range(0,6));
}  // namespace brig
}  // namespace hsa
