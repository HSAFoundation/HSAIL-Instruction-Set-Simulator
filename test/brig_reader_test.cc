#include "brig_engine.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_reader.h"
#include "brig_writer.h"
#include "error_reporter.h"
#include "parser_wrapper.h"

#include "llvm/Module.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/system_error.h"
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

  hsa::brig::BrigModule mod(*reader, NULL);
  EXPECT_TRUE(!mod.isValid());

  delete reader;
}

TEST(BrigWriterTest, EmptyBRIG) {

  int result_fd;
  llvm::SmallString<128> resultPath;
  llvm::error_code ec =
    llvm::sys::fs::unique_file("emptyBrig-%%%%%.o", result_fd, resultPath);
  EXPECT_TRUE(!ec);
  if(ec) return;

  llvm::raw_fd_ostream out(result_fd, true);

  hsa::brig::Buffer dirBuf;
  for(unsigned i = 0; i < 8; ++i) dirBuf.append_char(0);
  BrigDirectiveVersion bdv = {
    sizeof(bdv),
    BrigEDirectiveVersion,
    8,
    1,
    0,
    BrigELarge,
    BrigEFull,
    BrigENosftz,
    0
  };
  dirBuf.append(&bdv);
  llvm::StringRef directives(&dirBuf.get()[0], dirBuf.size());

  hsa::brig::Buffer codeBuf;
  for(unsigned i = 0; i < 8; ++i) codeBuf.append_char(0);
  llvm::StringRef code(&codeBuf.get()[0], codeBuf.size());

  hsa::brig::Buffer operBuf;
  for(unsigned i = 0; i < 8; ++i) operBuf.append_char(0);
  llvm::StringRef operands(&operBuf.get()[0], operBuf.size());

  hsa::brig::Buffer debugBuf;
  for(unsigned i = 0; i < 8; ++i) debugBuf.append_char(0);
  llvm::StringRef debug(&debugBuf.get()[0], debugBuf.size());

  hsa::brig::StringBuffer stringsBuf;
  for(unsigned i = 0; i < 8; ++i) stringsBuf.append_char(0);
  llvm::StringRef strings(&stringsBuf.get()[0], stringsBuf.size());

  bool result =
    BrigWriter::write(out, directives, code, operands, debug, strings);
  EXPECT_TRUE(result);
  if(!result) return;
  out.close();

  BrigReader *reader =
    BrigReader::createBrigReader(resultPath.c_str());
  EXPECT_TRUE(reader);
  if(!reader) return;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(mod.isValid());

  delete reader;

  bool existed;
  llvm::sys::fs::remove(resultPath.c_str(), existed);
  EXPECT_TRUE(existed);
}

llvm::Module *TestHSAIL(const std::string &source) {

  Parser parser(source);
  Context *context = parser.get_context();
  context->set_error_reporter(ErrorReporter::get_instance());
  int res = parser.parse();
  EXPECT_TRUE(!res);
  if(res) return NULL;

  BrigcOffset32_t codeSize = context->get_code_offset();
  BrigdOffset32_t dirSize  = context->get_directive_offset();
  BrigoOffset32_t operSize = context->get_operand_offset();
  BrigsOffset32_t strSize  = context->get_string_offset();

  llvm::OwningArrayPtr<char> code(new char[codeSize]);
  llvm::OwningArrayPtr<char> directives(new char[dirSize]);
  llvm::OwningArrayPtr<char> operands(new char[operSize]);
  llvm::OwningArrayPtr<char> strings(new char[strSize]);

  context->get_code_bytes(code.get(), 0, codeSize);
  context->get_directive_bytes(directives.get(), 0, dirSize);
  context->get_operand_bytes(operands.get(), 0, operSize);
  context->get_string_bytes(strings.get(), 0, strSize);

  int result_fd;
  llvm::SmallString<128> resultPath;
  llvm::error_code ec =
    llvm::sys::fs::unique_file("emptyBrig-%%%%%.o", result_fd, resultPath);
  EXPECT_TRUE(!ec);
  if(ec) return NULL;

  llvm::raw_fd_ostream out(result_fd, true);

  bool result =
    BrigWriter::write(out,
                      llvm::StringRef(directives.get(), dirSize),
                      llvm::StringRef(code.get(), codeSize),
                      llvm::StringRef(operands.get(), operSize),
                      llvm::StringRef("", 0),
                      llvm::StringRef(strings.get(), strSize));
  EXPECT_TRUE(result);
  if(!result) return NULL;
  out.close();

  BrigReader *reader =
    BrigReader::createBrigReader(resultPath.c_str());
  EXPECT_TRUE(reader);
  if(!reader) return NULL;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(mod.isValid());
  if(!mod.isValid()) return NULL;

  hsa::brig::GenLLVM codegen(*reader);
  codegen();

  delete reader;

  bool existed;
  llvm::sys::fs::remove(resultPath.c_str(), existed);
  EXPECT_TRUE(existed);

  return codegen.getModule();
}

TEST(BrigWriterTest, VectorCopy) {
  llvm::Module *mod = TestHSAIL(
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
  EXPECT_TRUE(mod);
  if(!mod) return;

  const unsigned arraySize = 16;
  float *arg_val0 = new float[arraySize];
  float *arg_val1 = new float[arraySize];
  for(unsigned i = 0; i < arraySize; ++i) {
    arg_val0[i] = M_PI;
    arg_val1[i] = 0;
  }

  uint32_t arg_val2 = arraySize;

  void *args[] = { &arg_val0, &arg_val1, &arg_val2 };
  llvm::Function *fun = mod->getFunction("__OpenCL_vec_copy_kernel");
  hsa::brig::launchBrig(mod, fun, args);

  EXPECT_FLOAT_EQ(M_PI, arg_val0[0]);
  EXPECT_FLOAT_EQ(M_PI, arg_val1[0]);

  delete[] arg_val1;
  delete[] arg_val0;
}

TEST(BrigWriterTest, Cosine) {
  TestHSAIL(
    "version 1:0:$small;\n"
    "\n"
    "//==========================================================\n"
    "// Function: __Get_fcos\n"
    "//\n"
    "// Inputs: arg_val0 - List of values to compute the fcos on\n"
    "//\n"
    "// Outputs: arg_val1 - Results\n"
    "//\n"
    "//==========================================================\n"
    "\n"
    "kernel &__Get_fcos(kernarg_u32 %arg_val0, kernarg_u32 %arg_val1)\n"
    "{\n"
    "	// Use workitemaid to get the buffer offset\n"
    "	workitemaid $s1, 0;\n"
    "	shl_u32	 $s1, $s1, 2;  // Get offset by Multiplying by 4 because \n"
    "                        // that is the size of each value in the buffer\n"
    "	\n"
    "	// Compute Address of Input Value and store in $s0\n"
    "	ld_kernarg_u32	$s0, [%arg_val0];\n"
    "	add_u32	 $s0, $s0, $s1;  // Add offset to Address\n"
    "\n"
    "	// Compute Address of Output Value and store in $s3\n"
    "	ld_kernarg_u32	$s3, [%arg_val1];\n"
    "	add_u32	 $s3, $s3, $s1;  // Add offset to Address\n"
    "\n"
    "	// Execute Trig Opcode\n"
    "	fcos_f32 $s2, [$s0];\n"
    "\n"
    "	// Store Dispatch ID on Kernel Argument\n"
    "	st_global_u32	$s2, [$s3] ;\n"
    "\n"
    "	ret;\n"
    "};\n"
    );
}
