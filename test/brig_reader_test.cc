#include "brig_reader.h"
#include "brig_writer.h"
#include "brig_module.h"
#include "error_reporter.h"
#include "parser_wrapper.h"

#include "gtest/gtest.h"

#define STR(X) #X
#define XSTR(X) STR(X)

using hsa::brig::BrigReader;
using hsa::brig::BrigWriter;
using hsa::brig::Buffer;
using hsa::brig::Context;
using hsa::brig::ErrorReporter;
using hsa::brig::Lexer;
using hsa::brig::Parser;

TEST(BrigReaderTest, VectorCopy) {
  BrigReader *reader =
    BrigReader::createBrigReader(XSTR(TEST_PATH) "/VectorCopy.bin");
  EXPECT_TRUE(reader);
  if(!reader) return;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(!mod.isValid());

  delete reader;
}

TEST(BrigWriterTest, VectorCopy) {

  std::string source(
    "version 1:0:$small;\n"
    "\n"
    "kernel &__OpenCL_vec_copy_kernel(\n"
    "        kernarg_u32 %arg_val0, \n"
    "        kernarg_u32 %arg_val1, \n"
    "        kernarg_u32 %arg_val2)\n"
    "{\n"
    "@__OpenCL_vec_copy_kernel_entry:\n"
    "        ld_kernarg_u32  $s0, [%arg_val2] ;\n"
    "        workitemaid     $s1, 0 ;\n"
    "        cmp_ge_b1_u32    $c0, $s1, $s0 ;\n"
    "        ld_kernarg_u32  $s0, [%arg_val1] ;\n"
    "        ld_kernarg_u32  $s2, [%arg_val0] ;\n"
    "        cbr     $c0, @BB0_2 ;\n"
    "\n"
    "        shl_u32  $s1, $s1, 2 ;\n"
    "        add_u32  $s0, $s0, $s1 ;\n"
    "        add_u32  $s1, $s2, $s1 ;\n"
    "        ld_global_f32   $s1, [$s1] ;\n"
    "        st_global_f32   $s1, [$s0] ;\n"
    "@BB0_2:\n"
    "        ret ;\n"
    "};\n"
    );

  Parser parser(source);
  Context *context = parser.get_context();
  context->set_error_reporter(ErrorReporter::get_instance());
  int res = parser.parse();
  EXPECT_TRUE(!res);
  BrigcOffset32_t codeSize = context->get_code_offset();
  BrigdOffset32_t dirSize  = context->get_directive_offset();
  BrigoOffset32_t operSize = context->get_operand_offset();
  BrigsOffset32_t strSize  = context->get_string_offset();
}
