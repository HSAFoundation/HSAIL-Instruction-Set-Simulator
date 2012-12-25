// Copyright 2012 MulticoreWare Inc.

#include "query_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestQuery,Query)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
  context->global_symbol_map["&RWImg"] = 40;
  context->global_symbol_map["%RWImg"] = 80;
 
  int n = GetParam();
  std::string input(query_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Query(context));

  BrigInstBase get;
  context->get_code(code_offset, &get);

  BrigInstBase ref = query_pair[n].ref;

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

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestQuery, testing::Range(0,8));

TEST_P(TestQueryInvalid, QueryInvalid) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_query_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Query(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(InvalidTest,
                        TestQueryInvalid,
                        testing::Range(0,3));


}  // namespace brig
}  // namespace hsa
