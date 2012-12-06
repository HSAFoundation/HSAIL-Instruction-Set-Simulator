// Copyright 2012 MulticoreWare Inc.

#include "test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST(CodegenTest, Instruction0Op_CodeGen_nop) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("nop; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  EXPECT_EQ(0, Instruction0(context));

  BrigInstBase ref = {
    brig_inst_base_size,
    BrigEInstBase,
    BrigNop,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  BrigInstBase get;
  context->get_code(code_offset, &get);
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

}  // namespace brig
}  // namespace hsa
