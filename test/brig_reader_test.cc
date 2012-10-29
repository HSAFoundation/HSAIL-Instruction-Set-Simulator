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

TEST(BrigWriterTest, VectorArith) {
  const uint32_t testVec[] = { 0, 0x01010101, 0xFFFFFFFF };
  testInst("add_pp_s8x4", testVec);
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

TEST(BrigKernelTest, EuclideanGCD) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "\n"
    "kernel &__run(\n"
    "      kernarg_u32 %arg_val2,\n"
    "      kernarg_u32 %arg_val0,\n"
    "      kernarg_u32 %arg_val1)\n"
    "{\n"
    "      ld_kernarg_u32    $s0, [%arg_val0];\n"
    "      ld_kernarg_u32    $s1, [%arg_val1];\n"
    "      cmp_eq_b1_u32 $c1, $s0, 0;\n"          // if a == 0;
    "      cbr     $c1, @BB0_1_0;\n"              // return b;
    "@WHILE:"
    "      cmp_eq_b1_u32 $c2, $s1, 0;\n"          // while b != 0;
    "      cbr     $c2, @BB0_1_1;\n"
    "@BB0_2:"
    "      cmp_gt_b1_u32 $c3, $s0, $s1;\n"        // if (a > b)
    "      cbr     $c3, @BB1_1;\n"
    "      brn     @BB1_2;\n"
    "@BB1_1:"
    "      sub_u32 $s0, $s0, $s1;\n"              // a = a - b;
    "      brn     @WHILE;\n"
    "@BB1_2:"
    "      sub_u32 $s1, $s1, $s0;\n"              // else b = b - a;
    "      brn     @WHILE;\n"
    "@BB0_1_0:"
    "      ld_kernarg_u32   $s6, [%arg_val2];\n"
    "      st_global_u32   $s1, [$s6] ;\n"
    "ret;\n"
    "@BB0_1_1:"
    "      ld_kernarg_u32   $s6, [%arg_val2];\n"
    "      st_global_u32   $s0, [$s6] ;\n"
    "ret;\n"
    "};\n"
  );
  EXPECT_TRUE(BP);
  if(!BP) return;

  int *number1 = new int[4];
  int *number2 = new int[4];
  int *divisor = new int;
  number1[0] = 12;
  number1[1] = 4;
  number1[2] = 0;
  number1[3] = 3;

  number2[0] = 16;
  number2[1] = 0;
  number2[2] = 5;
  number2[3] = 7;

  int temp[4] = {4,4,5,1};

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP->getFunction("__run");

  for(int i = 0; i < 4; i++) {
    void *args[] = { &divisor, number1 + i, number2 + i };
    BE.launch(fun, args);
    EXPECT_EQ(*divisor, temp[i]);
  }
  delete[] number1;
  delete[] number2;
  delete divisor;
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

TEST(BrigKernelTest, CRC32) {
  const unsigned arraySize = 16;
  char *a = new char[arraySize];
  for(unsigned i = 0; i < arraySize; ++i) {
    a[i] = 'a' + i;
  }
  unsigned len = 10;
  //CRC32 in C++
  uint   CRC32[256];
  unsigned   i,j;
  uint   crc;
  for(i = 0;i < 256;i++){
    crc = i;
    for(j = 0;j < 8;j++){
      if(crc & 1){
        crc = (crc >> 1) ^ 0xEDB88320;
      }else{
        crc = crc >> 1;
      }
    }
    CRC32[i] = crc;
  }
  uint ret = 0xFFFFFFFF;
  for(i = 0; i < len;i++){
    ret = CRC32[((ret & 0xFF) ^ a[i])] ^ (ret >> 8);
  }
  ret = ~ret;
  //CRC32 in C++ end. ret is CRC32 result.

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "\n"
    "kernel &CRC32Kernel(kernarg_u32 %r,\n"
    "                    kernarg_u32 %n_ptr,\n"
    "                    kernarg_u32 %n_len)\n"
    "{\n"
    "  ld_kernarg_u32 $s4, [%n_len]; \n" // $s4 is for %n_len
    "  ld_kernarg_u32 $s3, [%n_ptr]; \n" // $s3 is for %n_ptr
    "  sub_u32 $s4, $s4, 1;\n"
    "  xor_b32 $s1, $s1, $s1;\n"         // $s1 is for i. i = 0
    "  mov_b32 $s0, 0xFFFFFFFF;\n"       // $s0 is for retVal

    "@loop_i:"
    "  cmp_lt_b1_u32 $c1, $s4, $s1;\n"   // if i > len  go to return
    "  cbr $c1, @return;\n"

    "  and_b32 $s5, $s0, 0xFF;\n"        // $s5 is for CRC32 array index
    "  ld_u32 $s6, [$s3];\n"
    "  add_u32 $s3, $s3, 1;\n"           // n_ptr + 1
    "  and_b32 $s6, $s6, 0xFF;\n"
    "  xor_b32 $s5, $s5, $s6;\n"         //$s5 now is sub index of CRC array

       // get value of CRC32 array $s5 index. result store in $s5
    "  add_u32 $s2, 0, 0;\n"             // $s2 is for j. j = 0
    "@loop_j:"
    "  and_b32 $s7, $s5, 0x1;\n"         // CRC32 array index
    "  cmp_eq_b1_u32 $c2, $s7, 0x1;\n"
    "  shr_u32 $s5, $s5, 1;\n"
    "  cbr $c2, @loop_j_if;\n"
    "  brn @loop_j_endif;\n"
    "@loop_j_if:"
    "  xor_b32 $s5, $s5, 0xEDB88320;\n"
    "@loop_j_endif:"
    "  add_u32 $s2, $s2, 1;\n"           // ++j
    "  cmp_lt_b1_u32 $c3, $s2, 8;\n"     // j start at 1; if j < 9 go to @loop_j
    "  cbr $c3, @loop_j;\n"

    "  shr_u32  $s6, $s0, 8;\n"          // $s6 now is ret >> 8
    "  xor_b32 $s0, $s5, $s6;\n"         // $s0 is ret

    "  add_u32 $s1, $s1, 1;\n"           // ++i
    "  brn @loop_i;\n"

    "@return:"
    "  xor_b32 $s0, $s0, 0xFFFFFFFF;\n"  // ~ret
    "  ld_kernarg_u32 $s1, [%r];\n"
    "  st_global_u32 $s0, [$s1];\n"
    "  ret;\n"
    "};"
    );
  EXPECT_TRUE(BP);
  if(!BP) return;

  unsigned *r = new unsigned;
  unsigned *l = new unsigned(len);
  void *args[] = { &r, &a, l};
  llvm::Function *fun = BP->getFunction("CRC32Kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(ret, *r);
  EXPECT_EQ(0x3981703A, *r);

  delete r;
  delete[] a;
  delete l;
}

TEST(BrigKernelTest, FizzBuzz) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "kernel &__fizzbuzz (kernarg_s32 %r, kernarg_s32 %n)\n"
    "{\n"
       //$s0 ret, $s1 %n, $s2 i
    "  ld_kernarg_u32  $s1, [%n];\n"
    "  xor_b32 $s2, $s2, $s2;"
    "@loop:"
    "  mov_b32 $s4, $s2;\n"
    "  rem_s32 $s5, $s4, 15;\n"
    "  cmp_eq_b1_u32 $c1, $s5, 0;\n"
    "  cbr $c1, @FizzBuzz;\n"
    "  rem_s32 $s5, $s4, 3;\n"
    "  cmp_eq_b1_u32 $c1, $s5, 0;\n"
    "  cbr $c1, @Fizz;\n"
    "  rem_s32 $s5, $s4, 5;\n"
    "  cmp_eq_b1_u32 $c1, $s5, 0;\n"
    "  cbr $c1, @Buzz;\n"
    "  ld_kernarg_s32 $s5, [%r];\n"
    "  shl_u32 $s6, $s2, 2;"
    "  add_u32 $s5, $s5, $s6;"
    "  st_global_u32 0, [$s5];\n"
    "  brn @loop_check;"
    "@FizzBuzz:"
    "  ld_kernarg_s32  $s5, [%r];\n"
    "  shl_u32 $s6, $s2, 2;"
    "  add_u32 $s5, $s5, $s6;"
    "  st_global_u32 15, [$s5];\n"
    "  brn @loop_check;"
    "@Fizz:"
    "  ld_kernarg_s32  $s5, [%r];\n"
    "  shl_u32 $s6, $s2, 2;"
    "  add_u32 $s5, $s5, $s6;"
    "  st_global_u32 3, [$s5];\n"
    "  brn @loop_check;"
    "@Buzz:"
    "  ld_kernarg_s32  $s5, [%r];\n"
    "  shl_u32 $s6, $s2, 2;"
    "  add_u32 $s5, $s5, $s6;"
    "  st_global_u32 5, [$s5];\n"
    "  brn @loop_check;"
    "@loop_check:"
    "  add_u32 $s2, $s2, 1;"
    "  cmp_lt_b1_u32 $c1, $s2, $s1;\n"
    "  cbr $c1, @loop;\n"
    "  ret;\n"
    "};\n"
  );
  EXPECT_TRUE(BP);
  if(!BP) return;

  unsigned size = 1000;
  int *r = new int[size];
  int *n = new int(size);

  llvm::Function *fun = BP->getFunction("__fizzbuzz");
  void *args[] = { &r, n };
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  for(unsigned i = 0; i < size; ++i) {
    if (i % 15 == 0)
      EXPECT_EQ(15, r[i]);
    else {
      if (i % 3  == 0)
        EXPECT_EQ(3, r[i]);
      if (i % 5 == 0)
        EXPECT_EQ(5, r[i]);
    }
  }

  delete[] r;
  delete n;
}

TEST(BrigKernelTest, InsertionSorter) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "kernel &insertionsortKernel(kernarg_u32 %r, kernarg_u32 %l)\n"
    "{\n"
       //$s0 %l, $s1 %r, $s2 i, $s3 t %r[i], $s4 j, $s5 P_%r[j], $s6 P_%r[j-1]
    "  ld_kernarg_u32  $s1, [%r];\n"
    "  ld_kernarg_u32  $s0, [%l];\n"
    "  mov_b32 $s2, 1;\n"       //i = 0
    "@loop:"
    "  mov_b32 $s7, $s2;\n"
    "  shl_u32 $s7, $s7, 2;\n"
    "  add_u32 $s7, $s1, $s7;\n"       //$s3 now is addr of %r[i]
    "  ld_u32 $s3, [$s7];\n"           //int t = arr[i]
    "  mov_b32 $s4, $s2;\n"            //int j = i;

    "@loop_while:"
    "  cmp_le_b1_u32 $c0, $s4, 0;\n"   // if j <= 0 goto endwhile
    "  cbr $c0, @end_while;\n"
    "  sub_u32 $s6, $s4, 1;\n"
    "  shl_u32 $s6, $s6, 2;\n"
    "  add_u32 $s6, $s6, $s1;\n"
    "  ld_u32 $s7, [$s6];\n"           // $s7 now is %r[j-1]
    "  cmp_le_b1_u32 $c0, $s7, $s3;\n" //arr[j-1] > t goto endwhile
    "  cbr $c0, @end_while;\n"
    "  add_u32 $s5, $s6, 4;\n"         // $s5 now is addr of %r[j]
    "  st_global_u32 $s7, [$s5];\n"    // %r[j] = arr[j-1]
    "  sub_u32 $s4, $s4, 1;\n"         // --j
    "  brn @loop_while;\n"
    "@end_while:"
    "  shl_u32 $s6, $s4, 2;\n"
    "  add_u32 $s5, $s6, $s1;\n"       //init $s5
    "  st_global_u32 $s3, [$s5];\n"
    "@loop_check:"
    "  add_u32 $s2, $s2, 1;\n"
    "  cmp_lt_b1_u32 $c1, $s2, $s0;\n"
    "  cbr $c1, @loop;\n"
    "  ret;\n"
    "};\n"
  );
  EXPECT_TRUE(BP);
  if(!BP) return;

  const unsigned arraySize = 1024;
  unsigned *r = new unsigned[arraySize];

  for(unsigned i = 0; i < arraySize; i++) {
    r[i] = ((10653245 * i + 3325) % 2048);
  }

  unsigned *l = new unsigned(arraySize);
  void *args[] = { &r, l};
  llvm::Function *fun = BP->getFunction("insertionsortKernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  for(unsigned i = 0; i < arraySize - 1; ++i) {
    EXPECT_LE(r[i], r[i+1]);
  }

  delete[] r;
  delete l;
}

TEST(BrigKernelTest,  zeller) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 1:0:$small;\n"
    "kernel &zeller (kernarg_s32 %r, kernarg_s32 %m,\n"
    "kernarg_s32 %d, kernarg_s32 %y)\n"
    "{\n"
    "  ld_kernarg_u32 $s1, [%m];\n"
    "  cmp_gt_b1_u32 $c1, $s1,12;\n" // if n > 12 go to return
    "  cbr $c1, @break;\n"
    "  ld_kernarg_u32 $s2, [%d];\n"
    "  cmp_gt_b1_u32 $c2, $s2,31;\n" // if n > 31 go to return
    "  cbr $c2, @break;\n"
    "  ld_kernarg_u32 $s3, [%y];\n"
    "  add_u32 $s1, $s1, 1;\n"   // (m+1)
    "  mul_u32 $s1, $s1, 26;\n"  // ((m+1)x26)
    "  div_u32 $s1, $s1, 10;\n"  // (((m+1)x26)/10)
    "  div_u32 $s4, $s3, 4;\n"   // (y/4)
    "  div_u32 $s5, $s3, 100;\n" // (y/100)
    "  mul_u32 $s5, $s5, 6;\n"   // ((y/100)x6)
    "  div_u32 $s6, $s3, 400;\n" // (y/400)
    "  add_u32 $s5, $s4, $s5;\n" // ((y/4)+((y/100)x6))
    "  add_u32 $s6, $s6, $s5;\n" // (((y/4)+((y/100)x6)+(y/400))
    "  add_u32 $s3, $s6, $s3;\n" // (((y/4)+((y/100)x6)+(y/400)+y)
    // (((y/4)+((y/100)x6)+(y/400)+y+(((m+1)x26)/10))
    "  add_u32 $s1, $s1, $s3;\n"
    // (((y/4)+((y/100)x6)+(y/400)+y+(((m+1)x26)/10)+d)
    "  add_u32 $s1, $s1, $s2;\n"
    "  rem_s32 $s1, $s1, 7;\n"
    "  ld_kernarg_s32 $s0, [%r];\n"
    "  st_global_s32 $s1, [$s0];\n"
    "  ret;\n"
    "@break:"
    "  ld_kernarg_s32 $s0, [%r];\n"
    "  st_global_s32 1234, [$s0];\n" // if month or day is wrong, return 1234
    "  ret;\n"
    "};"
    );
  EXPECT_TRUE(BP);
  if(!BP) return;
  int *m_arg = new int[4];
  int *d_arg = new int[4];
  int *y_arg = new int[4];
  int *r_arg = new int;
  m_arg[0] = 10;
  m_arg[1] = 10;
  m_arg[2] = 4;
  m_arg[3] = 10;
  d_arg[0] = 14;
  d_arg[1] = 21;
  d_arg[2] = 25;
  d_arg[3] = 10;
  y_arg[0] = 1066;
  y_arg[1] = 1600;
  y_arg[2] = 1644;
  y_arg[3] = 2012;
  int temp[4] = {1,0,2,4};

  for(int i = 0; i < 4; i++) {
  hsa::brig::BrigEngine BE(BP);
  void *args[] = { &r_arg, m_arg+i, d_arg+i,y_arg+i };
    llvm::Function *fun = BP->getFunction("zeller");
    BE.launch(fun, args);
    EXPECT_EQ(*r_arg,temp[i]);
  }
  delete[] m_arg;
  delete[] d_arg;
  delete[] y_arg;
  delete r_arg;

}
