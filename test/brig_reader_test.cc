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
#include "llvm/Support/MemoryBuffer.h"
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

hsa::brig::BrigProgram TestHSAIL(const std::string &source) {

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

  hsa::brig::BrigProgram BP = hsa::brig::GenLLVM::getLLVMModule(mod);
  EXPECT_TRUE(BP);

  delete reader;

  bool existed;
  llvm::sys::fs::remove(resultPath.c_str(), existed);
  EXPECT_TRUE(existed);

  return BP;
}

TEST(BrigKernelTest, Cosine) {
  hsa::brig::BrigProgram BP = TestHSAIL(
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
    "	ld_f32 $s0, [$s0];\n"
    "   fcos_f32 $s2, $s0;\n"
    "\n"
    "	// Store Dispatch ID on Kernel Argument\n"
    "	st_global_u32	$s2, [$s3] ;\n"
    "\n"
    "	ret;\n"
    "};\n"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;

  float *arg_val0 = new float;
  float *arg_val1 = new float;

  *arg_val0 = M_PI;
  *arg_val1 = 0.0f;

  void *args[] = { &arg_val0, &arg_val1 };
  llvm::Function *fun = BP->getFunction("__Get_fcos");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_FLOAT_EQ(M_PI, *arg_val0);
  EXPECT_FLOAT_EQ(-1.0f, *arg_val1);

  delete arg_val1;
  delete arg_val0;
}

TEST(BrigKernelTest, Fib) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "function &fib (arg_s32 %r) (arg_s32 %n)\n"
    "{\n"
    "  ld_arg_s32 $s1, [%n];\n"
    "  cmp_lt_b1_s32 $c1, $s1, 3; // if n < 3 go to return\n"
    "  cbr $c1, @return;\n"
    "  private_s32 %p; // allocate a private variable\n"
    "                  // to hold the partial result\n"
    "  {\n"
    "    arg_s32 %nm2;\n"
    "    arg_s32 %res;\n"
    "    sub_s32 $s2, $s1, 2; // compute fib (n-2)\n"
    "    st_arg_s32 $s2, [%nm2];\n"
    "    call &fib (%res)(%nm2);\n"
    "    ld_arg_s32 $s2, [%res];\n"
    "  }\n"
    "  st_private_s32 $s2, [%p]; // save the result in p\n"
    "  {\n"
    "    arg_s32 %nm2;\n"
    "    arg_s32 %res;\n"
    "    sub_s32 $s2, $s1, 1; // compute fib (n-1)\n"
    "    st_arg_s32 $s2, [%nm2];\n"
    "    call &fib (%res)(%nm2);\n"
    "    ld_arg_u32 $s2, [%res];\n"
    "  }\n"
    "  ld_private_u32 $s3, [%p]; // add in the saved result\n"
    "  add_u32 $s2, $s2, $s3;\n"
    "  st_arg_s32 $s2, [%r];\n"
    "  ret;\n"
    "@return:"
    "  st_arg_s32 1, [%r];\n"
    "  ret;\n"
    "};\n"
    "\n"
    "kernel &fibKernel(kernarg_s32 %r_ptr, kernarg_s32 %n_ptr)\n"
    "{\n"
    "  {\n"
    "    arg_s32 %r;\n"
    "    arg_s32 %n;\n"
    "    ld_kernarg_s32 $s0, [%n_ptr];\n"
    "    st_arg_s32 $s0, [%n];\n"
    "    call &fib(%r)(%n);\n"
    "    ld_arg_s32 $s0, [%r];\n"
    "    ld_kernarg_s32 $s1, [%r_ptr];\n"
    "	   st_global_s32 $s0, [$s1];\n"
    "  }\n"
    "  ret;\n"
    "};"
    );
  EXPECT_TRUE(BP);
  if(!BP) return;

  int *r = new int;
  int *n = new int;
  void *args[] = { &r, n };
  llvm::Function *fun = BP->getFunction("fibKernel");


  int fib1 = 1;
  int fib2 = 0;
  for(int i = 1; i < 25; ++i) {
    *n = i;
    hsa::brig::BrigEngine BE(BP);
    BE.launch(fun, args);
    EXPECT_EQ(fib1 + fib2, *r);
    fib1 = fib2;
    fib2 = *r;
  }

  delete n;
  delete r;
}

TEST(BrigKernelTest, VectorCopy) {
  hsa::brig::BrigProgram BP = TestHSAIL(
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
  EXPECT_TRUE(BP);
  if(!BP) return;

  const unsigned arraySize = 16;
  float *arg_val0 = new float[arraySize];
  float *arg_val1 = new float[arraySize];
  for(unsigned i = 0; i < arraySize; ++i) {
    arg_val0[i] = M_PI;
    arg_val1[i] = 0;
  }

  uint32_t arg_val2 = arraySize;

  void *args[] = { &arg_val0, &arg_val1, &arg_val2 };
  llvm::Function *fun = BP->getFunction("__OpenCL_vec_copy_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_FLOAT_EQ(M_PI, arg_val0[0]);
  EXPECT_FLOAT_EQ(M_PI, arg_val1[0]);

  delete[] arg_val1;
  delete[] arg_val0;
}

TEST(BrigAllUpTest, AllUp) {
  llvm::OwningPtr<llvm::MemoryBuffer> file;
  const char filename[] = XSTR(TEST_PATH) "/AllUp.hsail";
  llvm::error_code ec = llvm::MemoryBuffer::getFile(filename, file);
  EXPECT_TRUE(!ec);

  hsa::brig::BrigProgram BP = TestHSAIL(file->getBufferStart());
  EXPECT_TRUE(BP);
}

static const char InstTest[] =
  "version 1:0:$small;\n"
  "\n"
  "kernel &__OpenCL_vec_test_kernel(\n"
  "        kernarg_u32 %%arg_val0, \n"
  "        kernarg_u32 %%arg_val1, \n"
  "        kernarg_u32 %%arg_val2, \n"
  "        kernarg_u32 %%result, \n"
  "        kernarg_u32 %%array_size)\n"
  "{\n"
  "@__OpenCL_vec_add_kernel_entry:\n"
  "        ld_kernarg_u32 $s0, [%%array_size] ;\n"
  "        workitemaid $s1, 0 ;\n"
  "        cmp_lt_b1_u32 $c0, $s1, $s0 ;\n"
  "        ld_kernarg_u32 $s0, [%%result] ;\n"
  "        ld_kernarg_u32 $s2, [%%arg_val0] ;\n"
  "        ld_kernarg_u32 $s3, [%%arg_val1] ;\n"
  "        ld_kernarg_u32 $s4, [%%arg_val2] ;\n"
  "        cbr $c0, @BB0_2 ;\n"
  "        brn @BB0_1 ;\n"
  "@BB0_1:\n"
  "        ret ;\n"
  "@BB0_2:\n"
  "        shl_u32 $s1, $s1, 2 ;\n"
  "        add_u32 $s2, $s2, $s1 ;\n"
  "        ld_global_f32 $s2, [$s2] ;\n"
  "        add_u32 $s3, $s3, $s1 ;\n"
  "        ld_global_f32 $s3, [$s3] ;\n"
  "        add_u32 $s4, $s4, $s1 ;\n"
  "        ld_global_f32 $s4, [$s4] ;\n"
  "        %s %s;\n"
  "        add_u32 $s0, $s0, $s1 ;\n"
  "        st_global_f32 $s2, [$s0] ;\n"
  "        brn @BB0_1 ;\n"
  "};\n";

static const char *nary[] = { "$s2",
                              "$s2, $s2",
                              "$s2, $s2, $s3",
                              "$s2, $s2, $s3, $s4" };

static hsa::brig::BrigProgram makeTest(unsigned args, const char *inst) {
  size_t size = snprintf(NULL, 0, InstTest, inst, nary[args]);
  char *buffer = new char[size];
  snprintf(buffer, size, InstTest, inst, nary[args]);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;
  return BP;
}

template<class T, size_t N>
static void testInst(const char *inst, const T(&testVec)[N]) {
  hsa::brig::BrigProgram BP = makeTest(N - 1, inst);
  EXPECT_TRUE(BP);
  if(!BP) return;

  unsigned arraySize = 16;
  T *input1 = new T[arraySize];
  T *input2 = new T[arraySize];
  T *input3 = new T[arraySize];
  T *output = new T[arraySize];
  for(unsigned i = 0; i < arraySize; ++i) {
    input1[i] = N > 1 ? testVec[1] : T(0);
    input2[i] = N > 2 ? testVec[2] : T(0);
    input3[i] = N > 3 ? testVec[3] : T(0);
    output[i] = 0;
  }

  void *args[] = { &input1, &input2, &input3, &output, &arraySize};
  llvm::Function *fun = BP->getFunction("__OpenCL_vec_test_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(testVec[0], output[0]);

  delete[] input1;
  delete[] input2;
  delete[] input3;
  delete[] output;
}

TEST(BrigInstTest, VectorAbs) {
  {
    const int32_t testVec[] = { 1, 1 };
    testInst("abs_s32", testVec);
  }
  {
    const int32_t testVec[] = { 1, -1 };
    testInst("abs_s32", testVec);
  }
}

TEST(BrigInstTest, VectorNeg) {
  const int32_t testVec[] = { -1, 1 };
  testInst("neg_s32", testVec);
}

TEST(BrigInstTest, VectorAdd) {
  const float testVec[] = { M_PI, M_PI, 0 };
  testInst("add_f32", testVec);
}

TEST(BrigInstTest, VectorDiv) {
  const float testVec[] = { 2, 2, 1 };
  testInst("div_f32", testVec);
}

TEST(BrigInstTest, VectorMul) {
  const int32_t testVec[] = { 4, 2, 2 };
  testInst("mul_s32", testVec);
}

TEST(BrigInstTest, VectorSub) {
  const int32_t testVec[] = { 1, 2, 1 };
  testInst("sub_s32", testVec);
}

TEST(BrigInstTest, vectorAddS32) {
  const int32_t testVec[] = { 3, 3, 0 };
  testInst("add_s32", testVec);
}

TEST(BrigInstTest, vectorAddU32) {
  const uint32_t testVec[] = { 3, 3, 0 };
  testInst("add_u32", testVec);
}

TEST(BrigInstTest, VectorRem) {
  const int32_t testVec[] = { 1, 3, 2 };
  testInst("rem_s32", testVec);
}

TEST(BrigInstTest, VectorCarry) {
  const int32_t testVec[] = { 1, -2, 2 };
  testInst("carry_s32", testVec);
}

TEST(BrigInstTest, VectorBorrow) {
  const int32_t testVec[] = { 1, 0, 1 };
  testInst("borrow_s32", testVec);
}

TEST(BrigInstTest, VectorMad) {
  const int32_t testVec[] = { 6, 2, 2, 2 };
  testInst("mad_s32", testVec);
}

TEST(BrigWriterTest, GlobalArray) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "\n"
    "global_u32 &array[10] = {1,2,3,4,5,6,7,8};\n"
    "\n"
    "kernel &__OpenCL_global_array_kernel(\n"
    "        kernarg_u32 %arg_val0)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%arg_val0] ;\n"
    "        ret ;\n"
    "} ;\n"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;
}

static const char SubwordsInst[] =
  "version 1:0:$small;\n"
  "\n"
  "kernel &__OpenCL_subwords_kernel(\n"
  "        kernarg_u32 %%arg_val0)\n"
  "{\n"
  "@__OpenCL_subwords_kernel_entry:\n"
  "        ld_kernarg_u32 $s0, [%%arg_val0] ;\n"
  "        st_global_%s %s, [$s0] ;\n"
  "        ret ;\n"
  "} ;\n";

template<class T>
static void testSubwords(const char *type, const T &result, const char *value) {
  size_t size = snprintf(NULL, 0, SubwordsInst, type, value);
  char *buffer = new char[size];
  snprintf(buffer, size, SubwordsInst, type, value);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if(!BP) return;

  T *arg_val0 = new T;
  *arg_val0 = 0;

  void *args[] = { &arg_val0 };
  llvm::Function *fun = BP->getFunction("__OpenCL_subwords_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(result, *arg_val0);

  delete arg_val0;
}

TEST(BrigWriterTest, Subwords) {
  {
    const int8_t result = 123;
    const char *value = "123";
    testSubwords("s8", result, value);
  }
  {
    const int8_t result = int8_t(0xFF);
    const char *value = "0xFF";
    testSubwords("s8", result, value);
  }
  {
    const int16_t result = 123;
    const char *value = "123";
    testSubwords("s16", result, value);
  }
  {
    const int16_t result = int16_t(0xFFFF);
    const char *value = "0xFFFF";
    testSubwords("s16", result, value);
  }
  {
    const uint8_t result = 123;
    const char *value = "123";
    testSubwords("u8", result, value);
  }
  {
    const uint8_t result = 0xFF;
    const char *value = "0xFF";
    testSubwords("u8", result, value);
  }
  {
    const uint16_t result = 123;
    const char *value = "123";
    testSubwords("u16", result, value);
  }
  {
    const uint16_t result = 0xFFFF;
    const char *value = "0xFFFF";
    testSubwords("u16", result, value);
  }
}

TEST(BrigKernelTest, VectorAddArray) {

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "\n"
    "function &get_global_id(arg_u32 %ret_val) (arg_u32 %arg_val0);\n"
    "\n"
    "function &abort() ();\n"
    "\n"
    "kernel &__OpenCL_vec_add_kernel(\n"
    "      kernarg_u32 %arg_val0,\n"
    "      kernarg_u32 %arg_val1,\n"
    "      kernarg_u32 %arg_val2,\n"
    "      kernarg_u32 %arg_val3)\n"
    "{\n"
    "@__OpenCL_vec_add_kernel_entry:\n"
    "// BB#0:                                // %entry\n"
    "      ld_kernarg_u32    $s0, [%arg_val3];\n"
    "      workitemaid    $s1, 0;\n"
    "      cmp_lt_b1_u32     $c0, $s1, $s0;\n"
    "      ld_kernarg_u32    $s0, [%arg_val2];\n"
    "      ld_kernarg_u32    $s2, [%arg_val1];\n"
    "      ld_kernarg_u32    $s3, [%arg_val0];\n"
    "      cbr    $c0, @BB0_2;\n"
    "      brn    @BB0_1;                    // %if.end\n"
    "@BB0_1:\n"
    "      ret;\n"
    "@BB0_2:\n                               // %if.then\n"
    "      shl_u32     $s1, $s1, 2;\n"
    "      add_u32     $s2, $s2, $s1;\n"
    "      ld_global_f32    $s2, [$s2];\n"
    "      add_u32     $s3, $s3, $s1;\n"
    "      ld_global_f32    $s3, [$s3];\n"
    "      add_f32\n   $s2, $s3, $s2;\n"
    "      add_u32\n   $s0, $s0, $s1;\n"
    "      st_global_f32    $s2, [$s0];\n"
    "      brn    @BB0_1;\n"
    "};\n"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;

  unsigned arraySize = 16;
  float *arg_val0 = new float[arraySize];
  float *arg_val1 = new float[arraySize];
  float *arg_val2 = new float[arraySize];
  for(unsigned i = 0; i < arraySize; ++i) {
    arg_val0[i] = 1;
    arg_val1[i] = 2;
    arg_val2[i] = 0;
  }

  void *args[] = { &arg_val0, &arg_val1, &arg_val2, &arraySize };
  llvm::Function *fun = BP->getFunction("__OpenCL_vec_add_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_FLOAT_EQ(3, arg_val2[0]);

  delete[] arg_val0;
  delete[] arg_val1;
  delete[] arg_val2;
}

TEST(BrigKernelTest, SExtZExt) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;"
    ""
    "kernel &SextZext("
    "      kernarg_u32 %input,"
    "      kernarg_u32 %sext,"
    "      kernarg_u32 %zext)"
    "{"
    "  ld_kernarg_u32    $s0, [%input];"
    "  ld_kernarg_u32    $s1, [%sext];"
    "  ld_kernarg_u32    $s2, [%zext];"
    "  ld_s8 $s3, [$s0];"
    "  shr_s32 $s3, $s3, 24;"
    "  st_s8 $s3, [$s1];"
    "  ld_u8 $s3, [$s0];"
    "  shr_u32 $s3, $s3, 24;"
    "  st_u8 $s3, [$s2];"
    "  ret;\n"
    "};"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;

  char *input = new char;
  char *sext = new char;
  char *zext = new char;
  *input = '\xff';
  *sext = 7;
  *zext = 7;

  void *args[] = { &input, &sext, &zext };
  llvm::Function *fun = BP->getFunction("SextZext");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(-1, *sext);
  EXPECT_EQ( 0, *zext);

  delete input;
  delete sext;
  delete zext;
}

TEST(BrigKernelTest, SubWordArray) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$large;"
    ""
    "global_u8 %array[16];"
    ""
    "kernel &getArrayPtr(kernarg_u64 %ptr)"
    "{"
    "  ld_u64 $d0, [%ptr];"
    "  lda_u64 $d1, [%array];"
    "  st_u64 $d1, [$d0];"
    "};"
    ""
    "kernel &SubWordArray(kernarg_u64 %size)"
    "{"
    "  ld_u64 $d1, [%size];"         // n = size;
    "  cmp_eq_b1_u64 $c0, $d1, 0;"
    "  cbr $c0, @sumLoopExit;"       // if(n == 0) goto sumLoopExit;
    "  mov_b64 $d2, 0;"              // sum = 0;
    "@sumLoopHeader:"
    "  sub_u64 $d1, $d1, 1;"         // --n;
    "  ld_u8 $d3, [%array][$d1];"
    "  add_u64 $d2, $d2, $d3;"       // sum += array[n];
    "  cmp_ne_b1_u64 $c0, $d1, 0;"
    "  cbr $c0, @sumLoopHeader;"     // if(n != 0) goto sumLoopHeader;
    "@sumLoopExit:"
    "  ld_u64 $d1, [%size];"         // n = size;
    "  mov_b64 $d3, 0;"              // i = 0;
    "  cmp_eq_b1_u64 $c0, $d3, $d1;"
    "  cbr $c0, @exit;"              // if(i == size) goto exit
    "@assignHeader:"
    "  st_u8 $d2, [%array][$d3];"
    "  add_u64 $d3, $d3, 1;"
    "  cmp_ne_b1_u64 $c0, $d3, $d1;"
    "  cbr $c0, @assignHeader;"      // if(i != size) goto assignHeader;
    "@exit:"
    "  ret;"
    "};"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;

  hsa::brig::BrigEngine BE(BP);

  size_t size = 16;
  char **array = new char *;
  {
    void *args[] = { &array };
    llvm::Function *fun = BP->getFunction("getArrayPtr");
    BE.launch(fun, args);
    EXPECT_TRUE(array);
  }

  for(unsigned i = 0; i < size; ++i) {
    (*array)[i] = (char) i;
  }

  {
    void *args[] = { &size };
    llvm::Function *fun = BP->getFunction("SubWordArray");
    BE.launch(fun, args);

    for(unsigned i = 0; i < size; ++i) {
      EXPECT_EQ((size * size - size) / 2, (*array)[i]);
    }
  }

  delete array;
}

TEST(BrigKernelTest, EmptyCB) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$large;"
    ""
    "kernel &EmptyCB()"
    "{"
    "@A:@B:@C:@D:@E:@F:@G:@H:@I:@J:@K:@L:@M:@N:@O:@P:@Q:@R:@S:@T:@U:@V:@W:@X:"
    "ret;"
    "};"
    );

  EXPECT_TRUE(BP);
  if(!BP) return;
  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP->getFunction("EmptyCB");
  llvm::ArrayRef<void *> args;
  BE.launch(fun, args);
}

TEST(BrigKernelTest, FizzBuzz) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "kernel &__fizzbuzz (kernarg_s32 %r, kernarg_s32 %n)\n"
    "{\n"
    "@__fizzbuzz_entry:"
    "      ld_kernarg_u32   $s3, [%n];\n"
    "      div_f32     $s2, $s3, 3;\n"      
    "      mul_s32     $s1, $s2, 3;\n"      
    "      sub_s32     $s0, $s3, $s1;\n"    
    "      cmp_eq_b1_u32   $c0, $s0, 0;\n"  
    "      cbr         $c0, @exit;\n"       // end if
    "      div_f32     $s8, $s3, 5;\n"      
    "      mul_s32     $s7, $s8, 5;\n"      
    "      sub_s32     $s6, $s3, $s7;\n"    
    "      cmp_eq_b1_u32   $c1, $s6, 0;\n" 
    "      cbr         $c1, @exit;\n"       //end if
    "      ld_kernarg_s32 $s5, [%r];\n"
    "      st_global_s32  0, [$s5];\n"
    "      ret;\n"
    "@exit:"
    "      ld_kernarg_s32 $s4, [%r];\n"
    "      st_global_s32  $s3, [$s4];\n"
    "      ret;\n"
    "};\n"
  );
  EXPECT_TRUE(BP);
  if(!BP) return;

  unsigned size = 10;
  int *r = new int[size];
  int *n = new int[size];
  void *args[] = { &r, n };
  llvm::Function *fun = BP->getFunction("__fizzbuzz");

  for(unsigned i = 0; i < size; ++i) {
    n[i] = i + 1;
    hsa::brig::BrigEngine BE(BP);
    BE.launch(fun, args);
  }
 
  EXPECT_EQ(3, r[2]);

  delete[] r;
  delete[] n;
}
