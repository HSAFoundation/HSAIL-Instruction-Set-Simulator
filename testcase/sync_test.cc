// Copyright 2012 MulticoreWare Inc.

#include "sync_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestSync,Sync)
{

  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(sync_pair[n].str);

  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Sync(context));

  BrigInstBar get;
  context->get_code(code_offset, &get);

  BrigInstBar ref = sync_pair[n].ref;
 

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.opcode, get.opcode);
  EXPECT_EQ(ref.packing, get.packing);
  EXPECT_EQ(ref.type, get.type);
  EXPECT_EQ(ref.o_operands[0], get.o_operands[0]);
  EXPECT_EQ(ref.o_operands[1], get.o_operands[1]);
  EXPECT_EQ(ref.o_operands[2], get.o_operands[2]);
  EXPECT_EQ(ref.o_operands[3], get.o_operands[3]);
  EXPECT_EQ(ref.o_operands[4], get.o_operands[4]);
  EXPECT_EQ(ref.syncFlags, get.syncFlags);

  delete lexer;

}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestSync, testing::Range(0,4));

TEST_P(TestSyncInvalid, SyncInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_sync_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Sync(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest, TestSyncInvalid, testing::Range(0,2));

}  // namespace brig
}  // namespace hsa
