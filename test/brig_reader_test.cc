//===- brig_reader_test.cc ------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_engine.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_reader.h"
#include "brig_runtime.h"
#include "hsailasm_wrapper.h"
#include "brig_runtime_internal.h"

#include "llvm/ADT/SmallString.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/system_error.h"
#include "gtest/gtest.h"

#include <cstdarg>

#define STR(X) #X
#define XSTR(X) STR(X)

using hsa::brig::BrigReader;

hsa::brig::BrigProgram TestHSAIL(const std::string &source) {


  int result_fd;
  llvm::SmallString<128> resultPath;
  llvm::error_code ec =
    llvm::sys::fs::createUniqueFile("emptyBrig-%%%%%.o", result_fd, resultPath);
  close(result_fd);
  EXPECT_TRUE(!ec);
  if (ec) return NULL;

  std::string errMsg;
  bool isValidHSA = hsa::brig::HsailAsm::assembleHSAILString(source.c_str(),
                                                             resultPath.c_str(),
                                                             &errMsg);
  EXPECT_TRUE(isValidHSA);
  if (!isValidHSA) {
    llvm::errs() << "Assembly failed: " << errMsg << "\n";
    return NULL;
  }

  BrigReader *reader =
    BrigReader::createBrigReader(resultPath.c_str());
  EXPECT_TRUE(reader);
  if (!reader) return NULL;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(mod.isValid());
  if (!mod.isValid()) return NULL;

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
    "version 0:96:$full:$small;\n"
    "\n"
    "//==========================================================\n"
    "// Function: __Get_ncos\n"
    "//\n"
    "// Inputs: arg_val0 - List of values to compute the ncos on\n"
    "//\n"
    "// Outputs: arg_val1 - Results\n"
    "//\n"
    "//==========================================================\n"
    "\n"
    "kernel &__Get_ncos(kernarg_u32 %arg_val0, kernarg_u32 %arg_val1)\n"
    "{\n"
    "	// Use workitemabsid_u32 to get the buffer offset\n"
    "	workitemabsid_u32 $s1, 0;\n"
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
    "   ncos_f32 $s2, $s0;\n"
    "\n"
    "	// Store Dispatch ID on Kernel Argument\n"
    "	st_global_u32	$s2, [$s3] ;\n"
    "\n"
    "	ret;\n"
    "};\n"
    );

  EXPECT_TRUE(BP);
  if (!BP) return;

  float *arg_val0 = new float;
  float *arg_val1 = new float;

  *arg_val0 = M_PI;
  *arg_val1 = 0.0f;

  void *args[] = { &arg_val0, &arg_val1 };
  llvm::Function *fun = BP.getFunction("__Get_ncos");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_FLOAT_EQ(M_PI, *arg_val0);
  EXPECT_FLOAT_EQ(-1.0f, *arg_val1);

  delete arg_val1;
  delete arg_val0;
}

TEST(BrigKernelTest, Fib) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "function &fib (arg_s32 %r) (arg_s32 %n)\n"
    "{\n"
    "  ld_arg_s32 $s1, [%n];\n"
    "  cmp_lt_b1_s32 $c1, $s1, 3;  // if n < 3 go to return\n"
    "  cbr $c1, @return;\n"
    "  private_s32 %p;  // allocate a private variable\n"
    "                  // to hold the partial result\n"
    "  {\n"
    "    arg_s32 %nm2;\n"
    "    arg_s32 %res;\n"
    "    sub_s32 $s2, $s1, 2;  // compute fib (n-2)\n"
    "    st_arg_s32 $s2, [%nm2];\n"
    "    call &fib (%res)(%nm2);\n"
    "    ld_arg_s32 $s2, [%res];\n"
    "  }\n"
    "  st_private_s32 $s2, [%p];  // save the result in p\n"
    "  {\n"
    "    arg_s32 %nm2;\n"
    "    arg_s32 %res;\n"
    "    sub_s32 $s2, $s1, 1;  // compute fib (n-1)\n"
    "    st_arg_s32 $s2, [%nm2];\n"
    "    call &fib (%res)(%nm2);\n"
    "    ld_arg_u32 $s2, [%res];\n"
    "  }\n"
    "  ld_private_u32 $s3, [%p];  // add in the saved result\n"
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
  if (!BP) return;

  int *r = new int;
  int *n = new int;
  void *args[] = { &r, n };
  llvm::Function *fun = BP.getFunction("fibKernel");


  int fib1 = 1;
  int fib2 = 0;
  for (int i = 1; i < 25; ++i) {
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
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__OpenCL_vec_copy_kernel(\n"
    "        kernarg_u32 %arg_val0, \n"
    "        kernarg_u32 %arg_val1, \n"
    "        kernarg_u32 %arg_val2)\n"
    "{\n"
    "@__OpenCL_vec_copy_kernel_entry:\n"
    "        ld_kernarg_u32  $s0, [%arg_val2] ;\n"
    "        workitemabsid_u32   $s1, 0 ;\n"
    "        cmp_ge_b1_u32   $c0, $s1, $s0 ;\n"
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
  if (!BP) return;

  const unsigned arraySize = 16;
  float *arg_val0 = new float[arraySize];
  float *arg_val1 = new float[arraySize];
  for (unsigned i = 0; i < arraySize; ++i) {
    arg_val0[i] = M_PI;
    arg_val1[i] = 0;
  }

  uint32_t arg_val2 = arraySize;

  void *args[] = { &arg_val0, &arg_val1, &arg_val2 };
  llvm::Function *fun = BP.getFunction("__OpenCL_vec_copy_kernel");
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

TEST(BrigAllUpTest, AMDAllUp) {
  llvm::OwningPtr<llvm::MemoryBuffer> file;
  const char filename[] = XSTR(TEST_PATH) "/hsail_tests_p.hsail";
  llvm::error_code ec = llvm::MemoryBuffer::getFile(filename, file);
  EXPECT_TRUE(!ec);

  hsa::brig::BrigProgram BP = TestHSAIL(file->getBufferStart());
  EXPECT_TRUE(BP);
}

static const char InstTest[] =
  "version 0:96:$full:$small;\n"
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
  "        workitemabsid_u32 $s1, 0 ;\n"
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
  "        shl_u32 $s1, $s1, %u ;\n"
  "        shl_u32 $s0, $s0, $s1 ;\n"
  "        shl_u32 $s2, $s2, $s1 ;\n"
  "        shl_u32 $s3, $s3, $s1 ;\n"
  "        shl_u32 $s4, $s4, $s1 ;\n"
  "        ld_global_f%u $%c2, [$s2] ;\n"
  "        ld_global_f%u $%c3, [$s3] ;\n"
  "        ld_global_f%u $%c4, [$s4] ;\n"
  "        %s %s;\n"
  "        st_global_f%u $%c2, [$s0] ;\n"
  "        brn @BB0_1 ;\n"
  "};\n";

static const char *nary32[] = { "$s2",
                                "$s2, $s2",
                                "$s2, $s2, $s3",
                                "$s2, $s2, $s3, $s4" };

static const char *nary64[] = { "$d2",
                                "$d2, $d2",
                                "$d2, $d2, $d3",
                                "$d2, $d2, $d3, $d4" };

static char *asnprintf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  size_t size = vsnprintf(NULL, 0, format, ap) + 1;
  va_end(ap);
  char *buffer = new char[size];
  va_start(ap, format);
  vsnprintf(buffer, size, format, ap);
  va_end(ap);
  return buffer;
}

static hsa::brig::BrigProgram makeTest(unsigned args, const char *inst,
                                       unsigned bits) {
  unsigned logBytes = bits == 32 ? 2 : 3;
  char c = bits == 32 ? 's' : 'd';
  const char **nary = bits == 32 ? nary32 : nary64;
  char *buffer =
    asnprintf(InstTest,
              logBytes,
              bits, c,
              bits, c,
              bits, c,
              inst, nary[args],
              bits, c);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;
  return BP;
}

template<class T, size_t N>
static void testInst(const char *inst, const T(&testVec)[N],
                     unsigned blockDim = 1, unsigned threadDim = 1) {
  hsa::brig::BrigProgram BP = makeTest(N - 1, inst, sizeof(T) * 8);
  EXPECT_TRUE(BP);
  if (!BP) return;

  unsigned arraySize = 16;
  T *input1 = new T[arraySize];
  T *input2 = new T[arraySize];
  T *input3 = new T[arraySize];
  T *output = new T[arraySize];
  for (unsigned i = 0; i < arraySize; ++i) {
    input1[i] = N > 1 ? testVec[1] : T(0);
    input2[i] = N > 2 ? testVec[2] : T(0);
    input3[i] = N > 3 ? testVec[3] : T(0);
    output[i] = 0;
  }

  void *args[] = { &input1, &input2, &input3, &output, &arraySize};
  llvm::Function *fun = BP.getFunction("__OpenCL_vec_test_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args, blockDim, threadDim);

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

TEST(BrigInstTest, VectorCeil) {
  {
    const float testVec[] = { 2.0, 1.2 };
    testInst("ceil_f32", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.2L };
    testInst("ceil_f64", testVec);
  }
  {
    const float testVec[] = { -1.0 , -1.8 };
    testInst("ceil_f32", testVec);
  }
  {
    const double testVec[] = { -1.0L , -1.8L };
    testInst("ceil_f64", testVec);
  }
}

TEST(BrigInstTest, VectorFloor) {
  {
    const float testVec[] = { 1.0, 1.8 };
    testInst("floor_f32", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.8L };
    testInst("floor_f64", testVec);
  }
  {
    const float testVec[] = { -2.0 , -1.2 };
    testInst("floor_f32", testVec);
  }
  {
    const double testVec[] = { -2.0L , -1.2L };
    testInst("floor_f64", testVec);
  }
}

TEST(BrigInstTest, VectorTrunc) {
  {
    const float testVec[] = { 1.0, 1.7 };
    testInst("trunc_f32", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.7L };
    testInst("trunc_f64", testVec);
  }
  {
    const float testVec[] = { -1.0 , -1.7 };
    testInst("trunc_f32", testVec);
  }
  {
    const double testVec[] = { -1.0L , -1.7L };
    testInst("trunc_f64", testVec);
  }
}

TEST(BrigInstTest, VectorRint) {
  {
    const float testVec[] = { 2.0, 1.7 };
    testInst("rint_f32", testVec);
  }
  {
    const float testVec[] = { 1.0, 1.2 };
    testInst("rint_f32", testVec);
  }
  {
    const float testVec[] = { 2.0, 1.5 };
    testInst("rint_f32", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.7L };
    testInst("rint_f64", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.2L };
    testInst("rint_f64", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.5L };
    testInst("rint_f64", testVec);
  }
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

TEST(BrigInstTest, VectorMulHi) {
  {
    const uint32_t testVec[] = { 0x1, 0x40000000, 4};
    testInst("mulhi_u32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x1, 0x4000000000000000LL, 4 };
    testInst("mulhi_u64", testVec);
  }
  {
    const int32_t testVec[] = { -0x1, 0x40000000, -4 };
    testInst("mulhi_s32", testVec);
  }
  {
    const int64_t testVec[] = {-0x3, 0x6000000000000000LL, -8 };
    testInst("mulhi_s64", testVec);
  }
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

TEST(BrigInstTest, Ftz) {
  {
    const float testVec[] = { 2 * 1.0E-45f, 1.0E-45f, 1.0E-45f };
    testInst("add_f32", testVec);
  }

  {
    const float testVec[] = { 0.0, 1.0E-45f, 1.0E-45f };
    testInst("add_ftz_f32", testVec);
  }

  {
    const float testVec[] = { 1.191E-38f - 1.190E-38f,
                              1.191E-38f, -1.190E-38f };
    testInst("add_f32", testVec);
  }

  {
    const float testVec[] = { 0,
                              1.191E-38f, -1.190E-38f };
    testInst("add_ftz_f32", testVec);
  }
}

TEST(BrigInstTest, Round) {
  union { uint32_t u32; float f32; } result;
  {
    result.u32 = 0x3EAAAAAB;
    const float testVec[] = { result.f32, 1.0f, 3.0 };
    testInst("div_f32", testVec);
  }
  {
    result.u32 = 0xBEAAAAAB;
    const float testVec[] = { result.f32, -1.0f, 3.0 };
    testInst("div_f32", testVec);
  }
  {
    result.u32 = 0x3EAAAAAB;
    const float testVec[] = { result.f32, 1.0f, 3.0 };
    testInst("div_near_f32", testVec);
  }
  {
    result.u32 = 0xBEAAAAAB;
    const float testVec[] = { result.f32, -1.0f, 3.0 };
    testInst("div_near_f32", testVec);
  }
  {
    result.u32 = 0x3EAAAAAA;
    const float testVec[] = { result.f32, 1.0f, 3.0 };
    testInst("div_zero_f32", testVec);
  }
  {
    result.u32 = 0xBEAAAAAA;
    const float testVec[] = { result.f32, -1.0f, 3.0 };
    testInst("div_zero_f32", testVec);
  }
  {
    result.u32 = 0x3EAAAAAB;
    const float testVec[] = { result.f32, 1.0f, 3.0 };
    testInst("div_up_f32", testVec);
  }
  {
    result.u32 = 0xBEAAAAAA;
    const float testVec[] = { result.f32, -1.0f, 3.0 };
    testInst("div_up_f32", testVec);
  }
  {
    result.u32 = 0x3EAAAAAA;
    const float testVec[] = { result.f32, 1.0f, 3.0 };
    testInst("div_down_f32", testVec);
  }
  {
    result.u32 = 0xBEAAAAAB;
    const float testVec[] = { result.f32, -1.0f, 3.0 };
    testInst("div_down_f32", testVec);
  }
}

TEST(BrigWriterTest, VectorArith) {
  {
    const uint32_t testVec[] = { 0x01030507, 0x03070b0f, 0x02040608 };
    testInst("sub_pp_s8x4", testVec);
    testInst("sub_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x01010101, 0, 0xFFFFFFFF };
    testInst("sub_pp_s8x4", testVec);
    testInst("sub_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x00010003, 0x00030007, 0x00020004 };
    testInst("sub_pp_s16x2", testVec);
    testInst("sub_pp_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0x00010001, 0, 0xFFFFFFFF };
    testInst("sub_pp_s16x2", testVec);
    testInst("sub_pp_u16x2", testVec);
  }
  {
    const uint64_t testVec[] = { 0x01030507090b0d0fULL,
                                 0x03070b0f13171b1fULL,
                                 0x020406080a0c0e10ULL };
    testInst("sub_pp_s8x8", testVec);
    testInst("sub_pp_u8x8", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0101010101010101ULL,
                                 0ULL,
                                 0xFFFFFFFFFFFFFFFFULL };
    testInst("sub_pp_s8x8", testVec);
    testInst("sub_pp_u8x8", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0001000300050007ULL,
                                 0x00030007000b000fULL,
                                 0x0002000400060008ULL };
    testInst("sub_pp_s16x4", testVec);
    testInst("sub_pp_u16x4", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0001000100010001ULL,
                                 0ULL,
                                 0xFFFFFFFFFFFFFFFFULL };
    testInst("sub_pp_s16x4", testVec);
    testInst("sub_pp_u16x4", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0000000100000003ULL,
                                 0x0000000300000007ULL,
                                 0x0000000200000004ULL };
    testInst("sub_pp_s32x2", testVec);
    testInst("sub_pp_u32x2", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0000000100000001ULL,
                                 0ULL,
                                 0xFFFFFFFFFFFFFFFFULL };
    testInst("sub_pp_s32x2", testVec);
    testInst("sub_pp_u32x2", testVec);
  }
  {
    const uint64_t testVec[] = { 0x3f8000003f800000ULL,
                                 0x402df854402df854ULL,
                                 0x3ebc5ab23ebc5ab2ULL };
    testInst("mul_pp_f32x2", testVec);
  }
}

TEST(BrigWriterTest, GlobalArray) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
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
  if (!BP) return;
}

static const char SubwordsInst[] =
  "version 0:96:$full:$small;\n"
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
  char *buffer = asnprintf(SubwordsInst, type, value);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T;
  *arg_val0 = 0;

  void *args[] = { &arg_val0 };
  llvm::Function *fun = BP.getFunction("__OpenCL_subwords_kernel");
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
    "version 0:96:$full:$small;\n"
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
  if (!BP) return;

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
  llvm::Function *fun = BP.getFunction("__run");

  for (int i = 0; i < 4; i++) {
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
    "version 0:96:$full:$small;\n"
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
    "      workitemabsid_u32     $s1, 0;\n"
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
  if (!BP) return;

  unsigned arraySize = 16;
  float *arg_val0 = new float[arraySize];
  float *arg_val1 = new float[arraySize];
  float *arg_val2 = new float[arraySize];
  for (unsigned i = 0; i < arraySize; ++i) {
    arg_val0[i] = 1;
    arg_val1[i] = 2;
    arg_val2[i] = 0;
  }

  void *args[] = { &arg_val0, &arg_val1, &arg_val2, &arraySize };
  llvm::Function *fun = BP.getFunction("__OpenCL_vec_add_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_FLOAT_EQ(3, arg_val2[0]);

  delete[] arg_val0;
  delete[] arg_val1;
  delete[] arg_val2;
}

TEST(BrigKernelTest, SExtZExt) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;"
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
  if (!BP) return;

  int8_t *input = new int8_t;
  int8_t *sext = new int8_t;
  int8_t *zext = new int8_t;
  *input = '\xff';
  *sext = 7;
  *zext = 7;

  void *args[] = { &input, &sext, &zext };
  llvm::Function *fun = BP.getFunction("SextZext");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(-1, *sext);
  EXPECT_EQ( 0, *zext);

  delete input;
  delete sext;
  delete zext;
}

TEST(BrigKernelTest, SubWordArray) {

  if (sizeof(void *) == 4) return;

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;"
    ""
    "global_u8 &array[16];"
    ""
    "kernel &getArrayPtr(kernarg_u64 %ptr)"
    "{"
    "  ld_kernarg_u64 $d0, [%ptr];"
    "  lda_global_u64 $d1, [&array];"
    "  st_u64 $d1, [$d0];"
    "};"
    ""
    "kernel &SubWordArray(kernarg_u64 %size)"
    "{"
    "  ld_kernarg_u64 $d1, [%size];"         // n = size;
    "  cmp_eq_b1_u64 $c0, $d1, 0;"
    "  cbr $c0, @sumLoopExit;"       // if (n == 0) goto sumLoopExit;
    "  mov_b64 $d2, 0;"              // sum = 0;
    "@sumLoopHeader:"
    "  sub_u64 $d1, $d1, 1;"         // --n;
    "  ld_global_u8 $d3, [&array][$d1];"
    "  add_u64 $d2, $d2, $d3;"       // sum += array[n];
    "  cmp_ne_b1_u64 $c0, $d1, 0;"
    "  cbr $c0, @sumLoopHeader;"     // if (n != 0) goto sumLoopHeader;
    "@sumLoopExit:"
    "  ld_kernarg_u64 $d1, [%size];"         // n = size;
    "  mov_b64 $d3, 0;"              // i = 0;
    "  cmp_eq_b1_u64 $c0, $d3, $d1;"
    "  cbr $c0, @exit;"              // if (i == size) goto exit
    "@assignHeader:"
    "  st_global_u8 $d2, [&array][$d3];"
    "  add_u64 $d3, $d3, 1;"
    "  cmp_ne_b1_u64 $c0, $d3, $d1;"
    "  cbr $c0, @assignHeader;"      // if (i != size) goto assignHeader;
    "@exit:"
    "  ret;"
    "};"
    );

  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);

  size_t size = 16;
  char **array = new char *;
  {
    void *args[] = { &array };
    llvm::Function *fun = BP.getFunction("getArrayPtr");
    BE.launch(fun, args);
    EXPECT_TRUE(array);
  }

  for (unsigned i = 0; i < size; ++i) {
    (*array)[i] = (char) i;
  }

  {
    void *args[] = { &size };
    llvm::Function *fun = BP.getFunction("SubWordArray");
    BE.launch(fun, args);

    for (unsigned i = 0; i < size; ++i) {
      EXPECT_EQ((size * size - size) / 2, (*array)[i]);
    }
  }

  delete array;
}

TEST(BrigKernelTest, EmptyCB) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;"
    ""
    "kernel &EmptyCB()"
    "{"
    "@A:@B:@C:@D:@E:@F:@G:@H:@I:@J:@K:@L:@M:@N:@O:@P:@Q:@R:@S:@T:@U:@V:@W:@X:"
    "ret;"
    "};"
    );

  EXPECT_TRUE(BP);
  if (!BP) return;
  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("EmptyCB");
  llvm::ArrayRef<void *> args;
  BE.launch(fun, args);
}

TEST(BrigKernelTest, CRC32) {
  const unsigned arraySize = 16;
  char *a = new char[arraySize];
  for (unsigned i = 0; i < arraySize; ++i) {
    a[i] = 'a' + i;
  }
  unsigned len = 10;
  //CRC32 in C++
  uint   CRC32[256];
  unsigned   i,j;
  uint   crc;
  for (i = 0;i < 256;i++){
    crc = i;
    for (j = 0;j < 8;j++){
      if (crc & 1){
        crc = (crc >> 1) ^ 0xEDB88320;
      }else{
        crc = crc >> 1;
      }
    }
    CRC32[i] = crc;
  }
  uint ret = 0xFFFFFFFF;
  for (i = 0; i < len;i++){
    ret = CRC32[((ret & 0xFF) ^ a[i])] ^ (ret >> 8);
  }
  ret = ~ret;
  //CRC32 in C++ end. ret is CRC32 result.

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &CRC32Kernel(kernarg_u32 %r,\n"
    "                    kernarg_u32 %n_ptr,\n"
    "                    kernarg_u32 %n_len)\n"
    "{\n"
    "  ld_kernarg_u32 $s4, [%n_len]; \n"  // $s4 is for %n_len
    "  ld_kernarg_u32 $s3, [%n_ptr]; \n"  // $s3 is for %n_ptr
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
  if (!BP) return;

  unsigned *r = new unsigned;
  unsigned *l = new unsigned(len);
  void *args[] = { &r, &a, l};
  llvm::Function *fun = BP.getFunction("CRC32Kernel");
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
    "version 0:96:$full:$small;\n"
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
  if (!BP) return;

  unsigned size = 1000;
  int *r = new int[size];
  int *n = new int(size);

  llvm::Function *fun = BP.getFunction("__fizzbuzz");
  void *args[] = { &r, n };
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  for (unsigned i = 0; i < size; ++i) {
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
    "version 0:96:$full:$small;\n"
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
    "  cmp_le_b1_u32 $c0, $s7, $s3;\n"  //arr[j-1] > t goto endwhile
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
  if (!BP) return;

  const unsigned arraySize = 1024;
  unsigned *r = new unsigned[arraySize];

  for (unsigned i = 0; i < arraySize; i++) {
    r[i] = ((10653245 * i + 3325) % 2048);
  }

  unsigned *l = new unsigned(arraySize);
  void *args[] = { &r, l};
  llvm::Function *fun = BP.getFunction("insertionsortKernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  for (unsigned i = 0; i < arraySize - 1; ++i) {
    EXPECT_LE(r[i], r[i+1]);
  }

  delete[] r;
  delete l;
}

TEST(BrigKernelTest,  zeller) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &zeller (kernarg_s32 %r, kernarg_s32 %m,\n"
    "kernarg_s32 %d, kernarg_s32 %y)\n"
    "{\n"
    "  ld_kernarg_u32 $s1, [%m];\n"
    "  cmp_gt_b1_u32 $c1, $s1,12;\n"  // if n > 12 go to return
    "  cbr $c1, @break;\n"
    "  ld_kernarg_u32 $s2, [%d];\n"
    "  cmp_gt_b1_u32 $c2, $s2,31;\n"  // if n > 31 go to return
    "  cbr $c2, @break;\n"
    "  ld_kernarg_u32 $s3, [%y];\n"
    "  add_u32 $s1, $s1, 1;\n"   // (m+1)
    "  mul_u32 $s1, $s1, 26;\n"  // ((m+1)x26)
    "  div_u32 $s1, $s1, 10;\n"  // (((m+1)x26)/10)
    "  div_u32 $s4, $s3, 4;\n"   // (y/4)
    "  div_u32 $s5, $s3, 100;\n"  // (y/100)
    "  mul_u32 $s5, $s5, 6;\n"   // ((y/100)x6)
    "  div_u32 $s6, $s3, 400;\n"  // (y/400)
    "  add_u32 $s5, $s4, $s5;\n"  // ((y/4)+((y/100)x6))
    "  add_u32 $s6, $s6, $s5;\n"  // (((y/4)+((y/100)x6)+(y/400))
    "  add_u32 $s3, $s6, $s3;\n"  // (((y/4)+((y/100)x6)+(y/400)+y)
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
    "  st_global_s32 1234, [$s0];\n"  // if month or day is wrong, return 1234
    "  ret;\n"
    "};"
    );
  EXPECT_TRUE(BP);
  if (!BP) return;
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

  for (int i = 0; i < 4; i++) {
    hsa::brig::BrigEngine BE(BP);
    void *args[] = { &r_arg, m_arg+i, d_arg+i,y_arg+i };
    llvm::Function *fun = BP.getFunction("zeller");
    BE.launch(fun, args);
    EXPECT_EQ(*r_arg,temp[i]);
  }
  delete[] m_arg;
  delete[] d_arg;
  delete[] y_arg;
  delete r_arg;
}

TEST(BrigKernelTest, ThreadTest) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &threadTest(kernarg_s32 %r)\n"
    "{\n"
    "  ld_kernarg_u32 $s0, [%r];\n"
    "  workitemabsid_u32  $s1, 0;\n"
    "	 shl_u32	      $s2, $s1, 2;\n"
    "  add_u32        $s0, $s0, $s2;\n"
    "  st_global_s32  $s1, [$s0];\n"
    "  ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;
  const unsigned threads = 16;
  unsigned *tids = new unsigned[threads];
  for (unsigned i = 0; i < threads; ++i) {
    tids[i] = ~0;
  }

  hsa::brig::BrigEngine BE(BP);
  void *args[] = { &tids };
  llvm::Function *fun = BP.getFunction("threadTest");
  BE.launch(fun, args, 1, threads);

  for (unsigned i = 0; i < threads; ++i) {
    EXPECT_EQ(i, tids[i]);
  }
  delete[] tids;
}

TEST(BrigKernelTest, IndirectBranches) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &indirectBranchesKernel(kernarg_u32 %r,\n"
      "                               kernarg_u32 %n)\n"
      "{\n"
      "  ld_kernarg_u32 $s0, [%n];\n"
      "  and_b32 $s1, $s0, 0x1;\n"         //$s1 is for odd or even
      "  mov_b32 $s2, 0xF;\n"              //set 15 to $s2
      "  ldc_u32 $s4, @even;\n"
      "  ldc_u32 $s5, @ge;\n"
      "  cmp_ge_b1_u32 $c0, $s0, $s2;\n"   //if $s0 >= $s2 goto @ge
      "  cbr $c0, $s5;\n"
      "  mov_b32 $s2, 0xD;\n"              //else set 13 to $s2
      "  brn $s5;\n"
      "@ge:"
      "  cmp_eq_b1_u32 $c1, $s1, 0;\n"     //if %n is even number goto @even
      "  cbr $c1, $s4;\n"
      "@odd:"
      "  add_u32 $s2, $s2, 2;\n"
      "  brn @return;\n"
      "@even:"
      "  add_u32 $s2, $s2, 1;\n"
      "  brn @return;\n"
      "@return:"
      "  ld_kernarg_u32 $s1, [%r];\n"
      "  st_global_u32 $s2, [$s1];\n"
      "  ret;\n"
      "};"
    );
    EXPECT_TRUE(BP);
    if (!BP) return;

    unsigned *r = new unsigned;
    unsigned *n = new unsigned;
    void *args[] = { &r, n};
    llvm::Function *fun = BP.getFunction("indirectBranchesKernel");
    hsa::brig::BrigEngine BE(BP);

    {
      *r = 0;
      *n = 19;
      BE.launch(fun, args);
      EXPECT_EQ(17, *r);
    }
    {
      *r = 0;
      *n = 18;
      BE.launch(fun, args);
      EXPECT_EQ(16, *r);
    }
    {
      *r = 0;
      *n = 14;
      BE.launch(fun, args);
      EXPECT_EQ(14, *r);
    }
    {
      *r = 0;
      *n = 13;
      BE.launch(fun, args);
      EXPECT_EQ(15, *r);
    }

    delete r;
    delete n;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &indirectBranchesKernel(kernarg_u32 %r,\n"
      "                               kernarg_u32 %n)\n"
      "{\n"
      "  ld_kernarg_u32 $s0, [%n];\n"
      "  and_b32 $s3, $s0, 0x1;\n"        //$s3 is for odd or even
      "  mov_b32 $s2, 0xF;\n"             //set 15 to $s2

      "  ldc_u32 $s1, @odd;\n"
      "  cmp_eq_b1_u32 $c1, $s3, 0x1;\n"
      "  cbr $c1, @ldc_end;\n"
      "  ldc_u32 $s1, @even;\n"
      "@ldc_end:"

      "  cmp_ge_b1_u32 $c0, $s0, $s2;\n"
      "@tab: labeltargets @even, @odd;\n"
      "  cbr $c0, $s1, [@tab];\n"  //in the case of $s0 >= $s2
                                  //if %n is even goto @even else goto @odd
      "  mov_b32 $s2, 0xD;\n"     //or: set 13 to $s2
      "  brn $s1, [@tab];\n"      //if %n is even goto @even else goto @odd
      "@odd:"
      "  add_u32 $s2, $s2, 2;\n"
      "  brn @return;\n"
      "@even:"
      "  add_u32 $s2, $s2, 1;\n"
      "  brn @return;\n"
      "@return:"
      "  ld_kernarg_u32 $s1, [%r];\n"
      "  st_global_u32 $s2, [$s1];\n"
      "  ret;\n"
      "};"
    );
    EXPECT_TRUE(BP);
    if (!BP) return;

    unsigned *r = new unsigned;
    unsigned *n = new unsigned;
    void *args[] = { &r, n};
    llvm::Function *fun = BP.getFunction("indirectBranchesKernel");
    hsa::brig::BrigEngine BE(BP);
    {
      *r = 0;
      *n = 19;
      BE.launch(fun, args);
      EXPECT_EQ(17, *r);
    }
    {
      *r = 0;
      *n = 18;
      BE.launch(fun, args);
      EXPECT_EQ(16, *r);
    }
    {
      *r = 0;
      *n = 14;
      BE.launch(fun, args);
      EXPECT_EQ(14, *r);
    }
    {
      *r = 0;
      *n = 13;
      BE.launch(fun, args);
      EXPECT_EQ(15, *r);
    }

    delete r;
    delete n;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &indirectBranchesKernel(kernarg_u32 %r,\n"
      "                               kernarg_u32 %n)\n"
      "{\n"
      "  ld_kernarg_u32 $s0, [%n];\n"
      "  and_b32 $s3, $s0, 0x1;\n"             //$s3 is for odd or even
      "  mov_b32 $s2, 0xF;\n"                  //set 15 to $s2

      "  ldc_u32 $s1, @odd;\n"
      "  cmp_eq_b1_u32 $c1, $s3, 0x1;\n"
      "  cbr $c1, @ldc_end;\n"
      "  ldc_u32 $s1, @even;\n"
      "@ldc_end:"

       "  brn @cmp;\n"
      "@odd:"
      "  add_u32 $s2, $s2, 2;\n"
      "  brn @return;\n"
      "@even:"
      "  add_u32 $s2, $s2, 1;\n"
      "  brn @return;\n"
      "@cmp:"

      "  cmp_ge_b1_u32 $c0, $s0, $s2;\n"
      "  global_u32 %tab[] =  {@even, @odd};\n"
      "  cbr $c0, $s1, [%tab];\n"  //in the case of $s0 >= $s2
                                  //if %n is even goto @even else goto @odd
      "  mov_b32 $s2, 0xD;\n"     //or: set 13 to $s2
      "  brn $s1, [%tab];\n"      //if %n is even goto @even else goto @odd

      "@return:"
      "  ld_kernarg_u32 $s1, [%r];\n"
      "  st_global_u32 $s2, [$s1];\n"
      "  ret;\n"
      "};"
    );
    EXPECT_TRUE(BP);
    if (!BP) return;

    unsigned *r = new unsigned;
    unsigned *n = new unsigned;
    void *args[] = { &r, n};
    llvm::Function *fun = BP.getFunction("indirectBranchesKernel");
    hsa::brig::BrigEngine BE(BP);

    {
      *r = 0;
      *n = 19;
      BE.launch(fun, args);
      EXPECT_EQ(17, *r);
    }
    {
      *r = 0;
      *n = 18;
      BE.launch(fun, args);
      EXPECT_EQ(16, *r);
    }
    {
      *r = 0;
      *n = 14;
      BE.launch(fun, args);
      EXPECT_EQ(14, *r);
    }
    {
      *r = 0;
      *n = 13;
      BE.launch(fun, args);
      EXPECT_EQ(15, *r);
    }

    delete r;
    delete n;
  }
}

TEST(BrigKernelTest, DISABLED_IndirectCall) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "function &foo (arg_u32 %r) (arg_u32 %n)\n"
    "{\n"
    "  ld_arg_u32 $s1, [%n];\n"     //add 1 to %n and set result to %r
    "  add_u32 $s1, $s1, 1;\n"
    "  st_arg_u32 $s1, [%r];\n"
    "  ret;\n"
    "};\n"
    "\n"
    "signature &barone_t (arg_u32 %r) (arg_u32 %n);\n"

    "function &bar (arg_u32 %r) (arg_u32 %n)\n"
    "{\n"
    "  ld_arg_u32 $s1, [%n];\n"     //add 2 to %n and set result to %r
    "  add_u32 $s1, $s1, 2;\n"
    "  st_arg_u32 $s1, [%r];\n"
    "  ret;\n"
    "};\n"
    "\n"

    "kernel &indirectCallKernel(kernarg_u32 %r_ptr, kernarg_u32 %n_ptr)\n"
    "{\n"
    "  ld_kernarg_u32 $s0, [%n_ptr];\n"

    "  ldc_u32 $s3, &foo;\n"
    "  {\n"
    "    arg_u32 %r;\n"
    "    arg_u32 %n;\n"
    "    st_arg_u32 $s0, [%n];\n"
    "    call $s3 (%r)(%n) [&foo, &bar];\n"
    "    ld_arg_u32 $s0, [%r];\n"
    "  }\n"

    "  ldc_u32 $s3, &bar;\n"
    "  {\n"
    "    arg_u32 %r;\n"
    "    arg_u32 %n;\n"
    "    st_arg_u32 $s0, [%n];\n"
    "    call $s3 (%r)(%n) &barone_t;\n"
    "    ld_arg_u32 $s0, [%r];\n"
    "  }\n"

    "@return:"
    "  ld_kernarg_u32 $s1, [%r_ptr];\n"
    "  st_global_u32 $s0, [$s1];\n"
    "  ret;\n"
    "};"
    );
  EXPECT_TRUE(BP);
  if (!BP) return;

  int *r = new int;
  int *n = new int(5);
  void *args[] = { &r, n };
  llvm::Function *fun = BP.getFunction("indirectCallKernel");

  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  EXPECT_EQ(8, *r);

  delete n;
  delete r;
}

TEST(BrigWriterTest, GlobalInitialization) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$small;\n"
  "\n"
  "global_u32 &n = 9;\n"
  "kernel &__OpenCL_Global_Initializer_kernel(\n"
  "        kernarg_u32 %r)\n"
  "{\n"
  "@__OpenCL_Global_Initializer_kernel_entry:\n"
  "        ld_kernarg_u32 $s2, [%r];\n"
  "        ld_global_u32 $s1, [&n];\n"
  "        st_global_u32  $s1, [$s2];\n"
  "        ret;\n"
  "};\n"
    );

  EXPECT_TRUE(BP);
  if (!BP) return;
}

static const char InstTestCvt[] =
  "version 0:96:$full:$small;\n"
  "\n"
  "kernel &__Cvt_rounding_mode_test_kernel(\n"
  "        kernarg_u32 %%result, \n"
  "        kernarg%s %%input)\n"
  "{\n"
  "        ld_kernarg_u32 $s3, [%%result] ;\n"
  "        ld_kernarg%s $%c1, [%%input] ;\n"
  "        %s%s%s %s;\n"
  "@return:\n"
  "        st_global%s $%c0, [$s3];\n"
  "        ret;\n"
  "};\n";

static const char *cvtOperands[] = { "$s0, $s1",
                                     "$s0, $d1",
                                     "$d0, $s1",
                                     "$d0, $d1"};

static hsa::brig::BrigProgram makeTestCvt(const char *inst,
                                          const char *destTypeLength,
                                          const char *srcTypeLength,
                                          unsigned destBits,
                                          unsigned srcBits) {
  char dc = destBits > 32 ? 'd' : 's';
  char sc = srcBits > 32 ? 'd' : 's';
  if (destBits < 32) destBits = 32;
  if (srcBits < 32) srcBits = 32;
  const char *srcTypeStoreLength =
    strcmp(srcTypeLength, "_b1") ? srcTypeLength : "_u8";
  const char *destTypeStoreLength =
    strcmp(destTypeLength, "_b1") ? destTypeLength : "_u8";
  unsigned args = ((destBits >> 5) & 0x2) | (srcBits >> 6);
  char *buffer =
    asnprintf(InstTestCvt,
              srcTypeStoreLength,
              srcTypeStoreLength, sc,
              inst, destTypeLength, srcTypeLength, cvtOperands[args],
              destTypeStoreLength, dc);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;
  return BP;
}

template<class R, class T>
static void testInstCvt(const char *inst, const char *destTypeLength,
                        const char *srcTypeLength, const R result,
                        const T input) {
  hsa::brig::BrigProgram BP = makeTestCvt(inst, destTypeLength, srcTypeLength,
                                          sizeof(R) * 8, sizeof(T) * 8);
  EXPECT_TRUE(BP);
  if (!BP) return;
  T *src = new T(input);
  R *dest = new R;
  void *args[] = {&dest, src};
  llvm::Function *fun = BP.getFunction("__Cvt_rounding_mode_test_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);
  EXPECT_EQ(result, *dest);

  delete dest;
  delete src;
}

TEST(BrigInstTest, CvtIToI) {
  union {
    uint64_t u64; int64_t s64;
    uint32_t u32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
  } result, input;

#define CvtII(DEST,SRC) do {                                          \
    input.SRC = ~SRC(0);                                              \
    result.DEST = (DEST) input.SRC;                                   \
    testInstCvt("cvt", "_" #DEST, "_" #SRC, result.DEST, input.SRC);  \
    input.SRC = SRC(1ULL << (sizeof(SRC) * 8 - 1));                   \
    result.DEST = (DEST) input.SRC;                                   \
    testInstCvt("cvt", "_" #DEST, "_" #SRC, result.DEST, input.SRC);  \
    input.SRC = ~input.SRC;                                           \
    result.DEST = (DEST) input.SRC;                                   \
    testInstCvt("cvt", "_" #DEST, "_" #SRC, result.DEST, input.SRC);  \
  } while (0)

#define CvtBB(DEST, SRC) do {                   \
    CvtII(s ## DEST, s ## SRC);                 \
    CvtII(s ## DEST, u ## SRC);                 \
    CvtII(u ## DEST, s ## SRC);                 \
    CvtII(u ## DEST, u ## SRC);                 \
  } while (0)

                CvtBB( 8, 16); CvtBB( 8, 32); CvtBB( 8, 64);
  CvtBB(16, 8);                CvtBB(16, 32); CvtBB(16, 64);
  CvtBB(32, 8); CvtBB(32, 16);                CvtBB(32, 64);
  CvtBB(64, 8); CvtBB(64, 16); CvtBB(64, 32);

#undef CvtBB
#undef CvtII
}

TEST(BrigInstTest, CvtRoundingMode) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
  } result, input;

#define CvtTest(INST, DTYPE, STYPE, RTYPE, RESULT, ITYPE, INPUT) do {   \
    result.RTYPE = RESULT;                                              \
    input.ITYPE = INPUT;                                                \
    testInstCvt(INST, "_"#DTYPE, "_"#STYPE, result.DTYPE, input.STYPE); \
  } while(0)

#define CvtTestBits(INST, STYPE, RESULT, INPUT) do {        \
    CvtTest(INST, s8,  STYPE, u8,  RESULT, STYPE, INPUT);   \
    CvtTest(INST, s16, STYPE, u16, RESULT, STYPE, INPUT);   \
    CvtTest(INST, s32, STYPE, u32, RESULT, STYPE, INPUT);   \
    CvtTest(INST, s64, STYPE, u64, RESULT, STYPE, INPUT);   \
    if (INPUT >= 0) {                                       \
      CvtTest(INST, u8,  STYPE, u8,  RESULT, STYPE, INPUT); \
      CvtTest(INST, u16, STYPE, u16, RESULT, STYPE, INPUT); \
      CvtTest(INST, u32, STYPE, u32, RESULT, STYPE, INPUT); \
      CvtTest(INST, u64, STYPE, u64, RESULT, STYPE, INPUT); \
    }                                                       \
  } while(0)

  // Float to Int Tests
  CvtTestBits("cvt_upi",   f32, 0x2,  1.6f);
  CvtTestBits("cvt_upi",   f32,  ~0, -1.6f);
  CvtTestBits("cvt_upi",   f64, 0x2,  1.6);
  CvtTestBits("cvt_upi",   f64,  ~0, -1.6);
  CvtTestBits("cvt_downi", f32, 0x1,  1.6f);
  CvtTestBits("cvt_downi", f32,  ~1, -1.6f);
  CvtTestBits("cvt_downi", f64, 0x1,  1.6);
  CvtTestBits("cvt_downi", f64,  ~1, -1.6);
  CvtTestBits("cvt_neari", f32, 0x2,  1.6f);
  CvtTestBits("cvt_neari", f32,  ~1, -1.6f);
  CvtTestBits("cvt_neari", f64, 0x2,  1.6);
  CvtTestBits("cvt_neari", f64,  ~1, -1.6);
  CvtTestBits("cvt_zeroi", f32, 0x1,  1.6f);
  CvtTestBits("cvt_zeroi", f32,  ~0, -1.6f);
  CvtTestBits("cvt_zeroi", f64, 0x1,  1.6);
  CvtTestBits("cvt_zeroi", f64,  ~0, -1.6);

  // Float to Float
  CvtTest("cvt_up",   f32, f64, u32, 0x3FCCCCCD, u64, 0x3FF999999999999ALL);
  CvtTest("cvt_up",   f32, f64, u32, 0xBFCCCCCC, u64, 0xBFF999999999999ALL);
  CvtTest("cvt_down", f32, f64, u32, 0x3FCCCCCC, u64, 0x3FF999999999999ALL);
  CvtTest("cvt_down", f32, f64, u32, 0xBFCCCCCD, u64, 0xBFF999999999999ALL);
  CvtTest("cvt_near", f32, f64, u32, 0x3FCCCCCD, u64, 0x3FF999999999999ALL);
  CvtTest("cvt_near", f32, f64, u32, 0xBFCCCCCD, u64, 0xBFF999999999999ALL);
  CvtTest("cvt_zero", f32, f64, u32, 0x3FCCCCCC, u64, 0x3FF999999999999ALL);
  CvtTest("cvt_zero", f32, f64, u32, 0xBFCCCCCC, u64, 0xBFF999999999999ALL);

  // Int to Float (lossy)
  const int32_t X = 3 + (1 << 25);
  const int64_t Y = 3ULL +(1ULL << 54);
  CvtTest("cvt_up", f32, u32, f32,  X + 1, u32,  X);
  CvtTest("cvt_up", f32, s32, f32,  X + 1, s32,  X);
  CvtTest("cvt_up", f32, s32, f32, -X + 3, s32, -X);
  CvtTest("cvt_up", f64, u64, f64,  Y + 1, u64,  Y);
  CvtTest("cvt_up", f64, s64, f64,  Y + 1, s64,  Y);
  CvtTest("cvt_up", f64, s64, f64, -Y + 3, s64, -Y);

  CvtTest("cvt_down", f32, u32, f32,  X - 3, u32,  X);
  CvtTest("cvt_down", f32, s32, f32,  X - 3, s32,  X);
  CvtTest("cvt_down", f32, s32, f32, -X - 1, s32, -X);
  CvtTest("cvt_down", f64, u64, f64,  Y - 3, u64,  Y);
  CvtTest("cvt_down", f64, s64, f64,  Y - 3, s64,  Y);
  CvtTest("cvt_down", f64, s64, f64, -Y - 1, s64, -Y);

  CvtTest("cvt_near", f32, u32, f32,  X + 1, u32,  X);
  CvtTest("cvt_near", f32, s32, f32,  X + 1, s32,  X);
  CvtTest("cvt_near", f32, s32, f32, -X - 1, s32, -X);
  CvtTest("cvt_near", f64, u64, f64,  Y + 1, u64,  Y);
  CvtTest("cvt_near", f64, s64, f64,  Y + 1, s64,  Y);
  CvtTest("cvt_near", f64, s64, f64, -Y - 1, s64, -Y);

  CvtTest("cvt_zero", f32, u32, f32,  X - 3, u32,  X);
  CvtTest("cvt_zero", f32, s32, f32,  X - 3, s32,  X);
  CvtTest("cvt_zero", f32, s32, f32, -X + 3, s32, -X);
  CvtTest("cvt_zero", f64, u64, f64,  Y - 3, u64,  Y);
  CvtTest("cvt_zero", f64, s64, f64,  Y - 3, s64,  Y);
  CvtTest("cvt_zero", f64, s64, f64, -Y + 3, s64, -Y);

  // Int to Float (lossless)
  const char *cvtFloatRndModes[] = {"cvt_up",
                                    "cvt_down",
                                    "cvt_near",
                                    "cvt_zero"};
  for (unsigned i = 0; i < 4; ++i){
    CvtTest(cvtFloatRndModes[i], f32, u8, f32,  0xFF, u64, 0xFF);
    CvtTest(cvtFloatRndModes[i], f32, s8, f32,  0x7F, u64, 0x7F);
    CvtTest(cvtFloatRndModes[i], f32, s8, f32, -0x80, u64, 0x80);

    CvtTest(cvtFloatRndModes[i], f32, u16, f32,  0xFFFF, u64, 0xFFFF);
    CvtTest(cvtFloatRndModes[i], f32, s16, f32,  0x7FFF, u64, 0x7FFF);
    CvtTest(cvtFloatRndModes[i], f32, s16, f32, -0x8000, u64, 0x8000);

    CvtTest(cvtFloatRndModes[i], f64, u8, f64,  0xFF, u64, 0xFF);
    CvtTest(cvtFloatRndModes[i], f64, s8, f64,  0x7F, u64, 0x7F);
    CvtTest(cvtFloatRndModes[i], f64, s8, f64, -0x80, u64, 0x80);

    CvtTest(cvtFloatRndModes[i], f64, u16, f64,  0xFFFF, u64, 0xFFFF);
    CvtTest(cvtFloatRndModes[i], f64, s16, f64,  0x7FFF, u64, 0x7FFF);
    CvtTest(cvtFloatRndModes[i], f64, s16, f64, -0x8000, u64, 0x8000);

    CvtTest(cvtFloatRndModes[i], f64, u32, f64,  0xFFFFFFFFLL, u64, 0xFFFFFFFFLL);
    CvtTest(cvtFloatRndModes[i], f64, s32, f64,  0x7FFFFFFFLL, u64, 0x7FFFFFFFLL);
    CvtTest(cvtFloatRndModes[i], f64, s32, f64, -0x80000000LL, u64, 0x80000000LL);
  }
}

template<class X> static X next(X x, X dir);
template<> f32 next(f32 x, f32 dir) { return nextafterf(x, dir); }
template<> f64 next(f64 x, f64 dir) { return nextafter(x, dir); }

template<class X, class R> static R after() {
  return next<R>(hsa::brig::Int<X>::Max, INFINITY);
}

template<class X, class R> static R before() {
  return next<R>(hsa::brig::Int<X>::Min, -INFINITY);
}

TEST(BrigInstTest, CvtSatRoundingMode) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
  } result, input;

  using hsa::brig::Int;
  CvtTest("cvt_upi_sat", u8, f32, u8,  0x2, f32, 1.6);

#define TestBound(INST, DTYPE, STYPE, POS, NEG) do {                \
    const DTYPE Max = hsa::brig::Int<DTYPE>::Max;                   \
    const DTYPE Min = hsa::brig::Int<DTYPE>::Min;                   \
    const STYPE After = after<DTYPE, STYPE>();                      \
    const STYPE Before = before<DTYPE, STYPE>();                    \
    CvtTest(INST, DTYPE, STYPE, DTYPE, 0,   STYPE, NAN);            \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Max, STYPE, After);          \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Max, STYPE, After + 64);     \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Max, STYPE, INFINITY);       \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Min, STYPE, Before);         \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Min, STYPE, Before - 64);    \
    CvtTest(INST, DTYPE, STYPE, DTYPE, Min, STYPE, -INFINITY);      \
    CvtTest(INST, DTYPE, STYPE, DTYPE, POS, STYPE, 1.6);            \
    if (hsa::brig::Int<DTYPE>::isSigned)                            \
      CvtTest(INST, DTYPE, STYPE, DTYPE, NEG, STYPE, -1.6);         \
    else                                                            \
      CvtTest(INST, DTYPE, STYPE, DTYPE, 0, STYPE, -1.6);           \
  } while(0)

#define TestBoundInst(INST, POS, NEG) do {      \
    TestBound(INST, u8,  f32, POS, NEG);        \
    TestBound(INST, s8,  f32, POS, NEG);        \
    TestBound(INST, u16, f32, POS, NEG);        \
    TestBound(INST, s16, f32, POS, NEG);        \
    TestBound(INST, u32, f32, POS, NEG);        \
    TestBound(INST, s32, f32, POS, NEG);        \
    TestBound(INST, u64, f32, POS, NEG);        \
    TestBound(INST, s64, f32, POS, NEG);        \
    TestBound(INST, u8,  f64, POS, NEG);        \
    TestBound(INST, s8,  f64, POS, NEG);        \
    TestBound(INST, u16, f64, POS, NEG);        \
    TestBound(INST, s16, f64, POS, NEG);        \
    TestBound(INST, u32, f64, POS, NEG);        \
    TestBound(INST, s32, f64, POS, NEG);        \
    TestBound(INST, u64, f64, POS, NEG);        \
    TestBound(INST, s64, f64, POS, NEG);        \
  } while(0)

  TestBoundInst("cvt_upi_sat",   2, -1);
  TestBoundInst("cvt_downi_sat", 1, -2);
  TestBoundInst("cvt_neari_sat", 2, -2);
  TestBoundInst("cvt_zeroi_sat", 1, -1);
}


TEST(BrigWriterTest, FlexArray) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "function &maxofN(arg_f32 %r)(arg_u32 %n, align 8 arg_u8 %last[])\n"
    "{\n"
    "ld_arg_u32 $s0, [%n];\n"
    "mov_b32 $s1, 0;\n"
    "mov_b32 $s3, 0;\n"
    "@loop:\n"
    "cmp_eq_b1_u32 $c1, $s0, 0;\n"
    "cbr $c1, @done;\n"
    "ld_arg_f32 $s4, [%last][$s3]; \n"
    "add_f32 $s1, $s1, $s4;\n"
    "add_u32 $s3, $s3, 4;\n"
    "sub_u32 $s0, $s0, 1;\n"
    "brn @loop;\n"
    "@done:\n"
    "st_arg_f32 $s1, [%r];\n"
    "ret;\n"
    "};\n"
   );
}

TEST(BrigWriterTest, GlobalInitialization_b64) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "global_b64 &n = 0;\n"
  "kernel &__OpenCL_Global_Initializer_kernel(\n"
  "        kernarg_b64 %r)\n"
  "{\n"
  "@__OpenCL_Global_Initializer_kernel_entry:\n"
  "        ld_kernarg_u64 $d2, [%r];\n"
  "        ld_global_u64 $d1, [&n];\n"
  "        st_global_u64  $d1, [$d2];\n"
  "        ret;\n"
  "};\n"
    );

  EXPECT_TRUE(BP);
  if (!BP) return;
}

static const char GlobalInitializerInst[] =
  "version 0:96:$full:$%s;\n"
  "\n"
  "global_%s &n = %s;\n"
  "kernel &__OpenCL_Global_Initializer_kernel(\n"
  "        kernarg_%s %%r)\n"
  "{\n"
  "@__OpenCL_Global_Initializer_kernel_entry:\n"
  "        ld_kernarg_%s $%c2, [%%r];\n"
  "        ld_global_u%u  $%c1, [&n];\n"
  "        st_global_u%u  $%c1, [$%c2];\n"
  "        ret;\n"
  "};\n";

static const char *model[] = { "small", "large" };
static const char *mType[] = { "u32", "u64" };
static const char mReg[] = { 's', 'd' };
template<class T>
static void testGlobalInitializer(const char *type,
                                  const T &result,
                                  const char *value,
                                  unsigned bits) {
for(unsigned i = 0; i < 2; ++i) {
  char reg = 0;
  if (bits == 8 || bits == 16 || bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(GlobalInitializerInst,
              model[i],
              type,
              value,
              mType[i],
              mType[i],
              mReg[i],
              bits,
              reg,
              bits,
              reg,
              mReg[i]);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T;
  *arg_val0 = 0;

  void *args[] = { &arg_val0 };
  llvm::Function *fun = BP.getFunction("__OpenCL_Global_Initializer_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(result, *arg_val0);

  delete arg_val0;
 }
}

TEST(BrigGlobalTest, GlobalInitializer) {

#define TestGlobal(BITS, RESULT) do {                             \
    typedef uint ## BITS ## _t RType;                             \
    const RType result = RType(RESULT ## ULL);                    \
    testGlobalInitializer("b" #BITS, result, #RESULT, BITS);      \
    testGlobalInitializer("s" #BITS, result, #RESULT, BITS);      \
    testGlobalInitializer("u" #BITS, result, #RESULT, BITS);      \
  } while(0)

  TestGlobal(8, 0x7F);
  TestGlobal(8, 0177);
  TestGlobal(8, 0x7E);
  TestGlobal(8, 0176);
  TestGlobal(8, 0x80);
  TestGlobal(8, 0200);
  TestGlobal(8, 0x81);
  TestGlobal(8, 0201);
  TestGlobal(8, 0xFF);
  TestGlobal(8, 0377);
  TestGlobal(8, 0xFE);
  TestGlobal(8, 0376);
  TestGlobal(8, 0x0);
  TestGlobal(8, 00);
  TestGlobal(8, 0x01);
  TestGlobal(8, 0001);
  TestGlobal(16, 0xFFFF);
  TestGlobal(16, 0177777);
  TestGlobal(16, 0xFFFE);
  TestGlobal(16, 0177776);
  TestGlobal(16, 0x7FFF);
  TestGlobal(16, 077777);
  TestGlobal(16, 0x7FFE);
  TestGlobal(16, 077776);
  TestGlobal(16, 0x8000);
  TestGlobal(16, 0100000);
  TestGlobal(16, 0x8001);
  TestGlobal(16, 0100001);
  TestGlobal(16, 0x0);
  TestGlobal(16, 00);
  TestGlobal(16, 0x0001);
  TestGlobal(16, 0000001);
  TestGlobal(32, 0xFFFFFFFF);
  TestGlobal(32, 037777777777);
  TestGlobal(32, 0xFFFFFFFE);
  TestGlobal(32, 037777777776);
  TestGlobal(32, 0x7FFFFFFF);
  TestGlobal(32, 017777777777);
  TestGlobal(32, 0x7FFFFFFE);
  TestGlobal(32, 017777777776);
  TestGlobal(32, 0x80000000);
  TestGlobal(32, 020000000000);
  TestGlobal(32, 0x80000001);
  TestGlobal(32, 020000000001);
  TestGlobal(32, 0x0);
  TestGlobal(32, 00);
  TestGlobal(32, 0x00000001);
  TestGlobal(32, 000000000001);
  TestGlobal(64, 0xFFFFFFFFFFFFFFFF);
  TestGlobal(64, 01777777777777777777777);
  TestGlobal(64, 0xFFFFFFFFFFFFFFFE);
  TestGlobal(64, 01777777777777777777776);
  TestGlobal(64, 0x7FFFFFFFFFFFFFFF);
  TestGlobal(64, 01777777777777777777777);
  TestGlobal(64, 0x7FFFFFFFFFFFFFFE);
  TestGlobal(64, 01777777777777777777776);
  TestGlobal(64, 0x8000000000000000);
  TestGlobal(64, 0x8000000000000001);
  TestGlobal(64, 0x0);
  TestGlobal(64, 00);
  TestGlobal(64, 0x0000000000000001);
  TestGlobal(64, 0000000000000000000001);
  {
    const uint32_t result = uint32_t(0x7f7fffff);
    const char *value = "3.4028234663852e+38f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x0);
    const char *value = "0e0f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x0);
    const char *value = "0.0f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xff7fffff);
    const char *value = "-3.4028234663852e+38f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x80000001);
    const char *value = "-1.401298e-45f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7f7ffffe);
    const char *value = "0f7f7ffffe";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x00000001);
    const char *value = "1.401298e-45f";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xff7ffffe);
    const char *value = "0fff7ffffe";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7f800000); // PosInf
    const char *value = "0f7f800000";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xff800000); // NegInf
    const char *value = "0fff800000";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xff800001); // SNaN
    const char *value = "0fff800001";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7f800001); // SNaN
    const char *value = "0f7f800001";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xff800003); // SNaN
    const char *value = "0fff800003";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7f800003); // SNaN
    const char *value = "0f7f800003";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7fC00000); // QNaN
    const char *value = "0f7fC00000";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x7fC00003); // QNaN
    const char *value = "0f7fC00003";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xffC00000); // QNaN
    const char *value = "0fffC00000";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0xffC00003); // QNaN
    const char *value = "0fffC00003";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x007fffff);
    const char *value = "0f007fffff";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x807fffff);
    const char *value = "0f807fffff";
    unsigned bits = 32;
    testGlobalInitializer("f32", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7fefffffffffffffLL);
    const char *value = "0d7fefffffffffffff";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x0LL);
    const char *value = "0d0000000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xffefffffffffffffLL);
    const char *value = "0dffefffffffffffff";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7feffffffffffffeLL);
    const char *value = "0d7feffffffffffffe";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x8000000000000001LL);
    const char *value = "0d8000000000000001";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x0000000000000001LL);
    const char *value = "0d0000000000000001";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xffeffffffffffffeLL);
    const char *value = "0dffeffffffffffffe";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7ff0000000000000ULL); // PosInf
    const char *value = "0d7ff0000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xfff0000000000000ULL); // NegInf
    const char *value = "0dfff0000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7fff800000000000ULL); // QNaN
    const char *value = "0d7fff800000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xfff8000000000000ULL); // QNaN
    const char *value = "0dfff8000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x000fffffffffffffULL); // denomalized
    const char *value = "0d000fffffffffffff";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x800fffffffffffffULL);  // denomalized
    const char *value = "0d800fffffffffffff";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7ff8000000000000ULL);  // QNaN
    const char *value = "0d7ff8000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xfff8000000000003ULL); // QNaN
    const char *value = "0dfff8000000000003";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7ff7000000000000ULL); // SNaN
    const char *value = "0d7ff7000000000000";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7ff0000000000001ULL); // SNaN
    const char *value = "0d7ff0000000000001";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xfff0000000000001ULL); // SNaN
    const char *value = "0dfff0000000000001";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0x7ff0000000000003ULL); // SNaN
    const char *value = "0d7ff0000000000003";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }
  {
    const uint64_t result = uint64_t(0xfff0000000000003ULL); // SNaN
    const char *value = "0dfff0000000000003";
    unsigned bits = 64;
    testGlobalInitializer("f64", result, value, bits);
  }

}

static const char GlobalArrayInst[] =
  "version 0:96:$full:$large;\n"
  "\n"
  "global_%s &array[] = { %s };\n"
  "\n"
  "kernel &__OpenCL_Global_Array_kernel(\n"
  "        kernarg_u32 %%arg_val0, kernarg_u32 %%n, kernarg_u64 %%bitsLen)\n"
  "{\n"
  "        ld_kernarg_u32 $s3, [%%n];\n"
  "        ld_kernarg_u64 $d5, [%%bitsLen];\n"
  "        div_u64 $d5, $d5, 8;\n"
  "        xor_b32 $s2, $s2, $s2;\n"
  "        mov_b64 $d7, 0;\n"
  "        ld_kernarg_u64 $d0, [%%arg_val0];\n"
  "@store:\n"
  "        ld_global_u%u $%c4, [&array][$d7];\n"
  "        st_global_u%u $%c4, [$d0];\n"
  "        add_u64 $d7, $d7, $d5;\n"
  "        add_u64 $d0, $d0, $d5;\n"
  "@loop_check:\n"
  "        add_u32 $s2, $s2, 1;\n"
  "        cmp_lt_b1_u32 $c1, $s2, $s3;\n"
  "        cbr $c1, @store;\n"
  "        ret ;\n"
  "} ;\n";

template<class T>
static void testGlobalArray(const char *type,
                            const T *result,
                            const char *value,
                            unsigned bits,
                            unsigned arraySz) {
  char reg = 0;
  if (bits == 8 || bits == 16 || bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(GlobalArrayInst,
              type,
              value,
              bits,
              reg,
              bits,
              reg);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T[arraySz];
  for (unsigned i = 0; i < arraySz; i++) {
    arg_val0[i] = 0;
  }
  unsigned *n = new unsigned(arraySz);
  uint64_t *bitsLen = new uint64_t(bits);

  void *args[] = { &arg_val0, n, bitsLen };
  llvm::Function *fun = BP.getFunction("__OpenCL_Global_Array_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  for (unsigned i = 0; i < arraySz; i++) {
    if (!strcmp(type, "f32"))
      EXPECT_FLOAT_EQ(result[i], arg_val0[i]);
    if (!strcmp(type, "f64"))
      EXPECT_DOUBLE_EQ(result[i], arg_val0[i]);
  }

  delete[] arg_val0;
  delete bitsLen;
  delete n;
}

TEST(BrigGlobalTest, GlobalArray) {
  {
    float result[5] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f };
    const char *value = "1.1f, 2.2f, 3.3f, 4.4f, 5.5f";
    const unsigned bits = 32;
    const unsigned arraySz = 5;
    testGlobalArray("f32", result, value, bits, arraySz);
  }
  {
    float result[4] = { 12.345, 12.345, 12.345, 12.345 };
    union { float f32; uint32_t u32; } inputData = { 12.345 };
    const char input[] = "%ff, %ef, 0f%lx, %af";
    char *buffer =
      asnprintf(input,
                inputData.f32, inputData.f32,
                inputData.u32, inputData.f32);
    const char *value = buffer;
    const unsigned bits = 32;
    const unsigned arraySz = 4;
    testGlobalArray("f32", result, value, bits, arraySz);
    delete[] buffer;
  }
  {
    double result[4] = { 12.345, 12.345, 12.345, 12.345 };
    union { double f64; uint64_t u64; } inputData = { 12.345 };
    const char input[] = "%fl, %el, 0d%llx, %al";
    char *buffer =
      asnprintf(input,
                inputData.f64, inputData.f64,
                inputData.u64, inputData.f64);
    const char *value = buffer;
    const unsigned bits = 64;
    const unsigned arraySz = 4;
    testGlobalArray("f64", result, value, bits, arraySz);
    delete[] buffer;
  }
}

TEST(BrigInstTest, WaveSize) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "kernel &waveTest(kernarg_u64 %r)\n"
      "{\n"
      "  ld_kernarg_u64 $d0, [%r];\n"
      "  add_u32    $s1, WAVESIZE, 0;\n"
      "  st_global_u32  $s1, [$d0];\n"
      "  ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("waveTest");
    unsigned *waveSize = new unsigned(0);
    void *args[] = { &waveSize };
    BE.launch(fun, args);
    EXPECT_EQ(1, *waveSize);
    delete waveSize;
  }
}

TEST(BrigKernelTest, MultipleVersionStatements) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$small;\n"
  "\n"
  "kernel &__Kernel_in_First_Scope(\n"
  "        kernarg_u32 %r)\n"
  "{\n"
  "        ret;\n"
  "};\n"
  "version 0:96:$full:$small;\n"
  "\n"
  "kernel &__Kernel_in_Second_Scope(\n"
  "        kernarg_u32 %r)\n"
  "{\n"
  "        ret;\n"
  "};\n"
  "version 0:96:$full:$small;\n"
  "\n"
  "kernel &__Kernel_in_Third_Scope(\n"
  "        kernarg_u32 %r)\n"
  "{\n"
  "        ret;\n"
  "};\n");

  EXPECT_TRUE(BP);
  if (!BP) return;
}
TEST(BrigKernelTest, Ftz2) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &ftzTest(kernarg_f32 %out)\n"
    "{\n"
    " add_ftz_f32 $s1, 0f007FFFFF, 0;\n"
    " ld_kernarg_f32 $s0, [%out];\n"
    " st_f32 $s1, [$s0];\n"
    " ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("ftzTest");
  float *arg1 = new float(7.0f);
  void *args[] = { &arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0.0f, *arg1);
  delete arg1;
}

TEST(BrigGlobalTest, Align16) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "align 16 readonly_s32 &x[] = {12, 13, 14, -13};"
    );
  EXPECT_TRUE(BP);
}

TEST(BrigKernelTest, Example6) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;"
    "function &callee(arg_f32 %output)(arg_f32 %input) {"
    "  ld_arg_f32 $s0, [%input];"
    "  st_arg_f32 $s0, [%output];"
    "  ret;"
    "};"
    ""
    "function &caller()() {"
    "  {"
    "    arg_f32 %an_input;"
    "    st_arg_f32 $s1, [%an_input ];"
    "    arg_f32 %an_output;"
    "    call &callee (%an_output)(%an_input);"
    "    ld_arg_f32 $s0, [%an_output];"
    "  }"
    "};"
    );
  EXPECT_TRUE(BP);
}

TEST(BrigInstTest, CombineV2_b32) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &regV2(kernarg_u32 %r, kernarg_u32 %x, kernarg_u32 %y)\n"
    "{\n"
    "  ld_kernarg_u32 $s5, [%r];\n"
    "  ld_kernarg_u32 $s0, [%x];\n"
    "  ld_kernarg_u32 $s1, [%y];\n"
    "  combine_v2_b64_b32 $d1, ($s0, $s1);\n"
    "  st_u64 $d1, [$s5];\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("regV2");
  unsigned *x = new unsigned(0x12345678);
  unsigned *y = new unsigned(0x9ABCDEF0);
  uint32_t *output = new uint32_t[2];
  memset(output, 0, sizeof(uint32_t[2]));
  void *args[] = { &output, x, y };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345678, output[0]);
  EXPECT_EQ(0x9ABCDEF0, output[1]);

  delete x;
  delete y;
  delete[] output;
}

TEST(BrigInstTest, CombineV2_b64) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &regV2(kernarg_u32 %r, kernarg_u64 %x, kernarg_u64 %y)\n"
    "{\n"
    "  ld_kernarg_u32 $s5, [%r];\n"
    "  ld_kernarg_u64 $d0, [%x];\n"
    "  ld_kernarg_u64 $d1, [%y];\n"
    "  combine_v2_b128_b64 $q0, ($d0, $d1);\n"
    "  st_b128 $q0, [$s5];\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("regV2");
  uint64_t *x = new uint64_t(0x1234567887654321ULL);
  uint64_t *y = new uint64_t(0x9ABCDEF00FEDCBA9ULL);
  uint64_t *output = new uint64_t[2];
  memset(output, 0, sizeof(uint64_t[2]));
  void *args[] = { &output, x, y };
  BE.launch(fun, args);
  EXPECT_EQ(0x1234567887654321ULL, output[0]);
  EXPECT_EQ(0x9ABCDEF00FEDCBA9ULL, output[1]);

  delete x;
  delete y;
  delete[] output;
}

TEST(BrigInstTest, CombineV4) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &regV4(kernarg_u32 %r, kernarg_u32 %w, kernarg_u32 %x,\n"
    "                              kernarg_u32 %y, kernarg_u32 %z)\n"
    "{\n"
    "  ld_kernarg_u32 $s0, [%r];\n"
    "  ld_kernarg_u32 $s1, [%w];\n"
    "  ld_kernarg_u32 $s2, [%x];\n"
    "  ld_kernarg_u32 $s3, [%y];\n"
    "  ld_kernarg_u32 $s4, [%z];\n"
    "  combine_v4_b128_b32 $q0, ($s1, $s2, $s3, $s4);\n"
    "  st_b128 $q0, [$s0];\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("regV4");
  unsigned *w = new unsigned(0x01020304);
  unsigned *x = new unsigned(0x05060708);
  unsigned *y = new unsigned(0x090A0B0C);
  unsigned *z = new unsigned(0x0D0E0F00);
  uint32_t *output = new uint32_t[4];
  memset(output, 0, sizeof(uint32_t[4]));
  void *args[] = { &output, w, x, y, z };
  BE.launch(fun, args);
  EXPECT_EQ(0x01020304, output[0]);
  EXPECT_EQ(0x05060708, output[1]);
  EXPECT_EQ(0x090A0B0C, output[2]);
  EXPECT_EQ(0x0D0E0F00, output[3]);

  delete w;
  delete x;
  delete y;
  delete z;
  delete[] output;
}

TEST(BrigInstTest, Testb128) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "kernel &MovB128(kernarg_u32 %r)\n"
    "{\n"
    "  ld_kernarg_u32 $s0, [%r];\n"
    "  mov_b128 $q1, _u32x4(1, 2, 3, 4);\n"
    "  st_b128 $q1, [$s0];\n"
    "  ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("MovB128");
  uint64_t *output = new uint64_t[2];
  memset(output, 0, sizeof(uint64_t) * 2);
  void *args[] = { &output };
  BE.launch(fun, args);
  EXPECT_EQ(0x0000000100000002LL, output[1]);
  EXPECT_EQ(0x0000000300000004LL, output[0]);

  delete[] output;
}

static const char Packed[] =
  "version 0:96:$full:$small;\n"
  "\n"
  "kernel &packed_kernel(\n"
  "        kernarg_u32 %%r, kernarg_u%u %%input)\n"
  "{\n"
  "  ld_kernarg_u32 $s4, [%%r];\n"
  "  ld_kernarg_u%u $%c0, [%%input];\n"
  "  ld_kernarg_u%u $%c1, [%%input];\n"
  "  shuffle_%s $%c2, $%c0, $%c1, 0x11;\n"
  "  st_u%u $%c2, [$s4];\n"
  "  ret;\n"
  "};\n";

template<class T>
static void testPacked(const char *type,
                       const T &result,
                       T *input,
                       unsigned bits) {
  char reg = 0;
  if (bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(Packed,
              bits,
              bits,
              reg,
              bits,
              reg,
              type,
              reg,
              reg,
              reg,
              bits,
              reg);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T(0);

  void *args[] = { &arg_val0, input };
  llvm::Function *fun = BP.getFunction("packed_kernel");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(result, *arg_val0);
  delete arg_val0;
}

TEST(BrigPacked, testPacked) {
  {
    const uint32_t result = uint32_t(0x1000100);
    uint32_t *input = new uint32_t(0x1);
    unsigned bits = 32;
    testPacked("u8x4", result, input, bits);
    delete input;
  }
  {
    const uint32_t result = uint32_t(0x1000100);
    uint32_t *input = new uint32_t(0x1);
    unsigned bits = 32;
    testPacked("s8x4", result, input, bits);
    delete input;
  }
  {
    const uint32_t result = uint32_t(0x10000);
    uint32_t *input = new uint32_t(0x1);
    unsigned bits = 32;
    testPacked("s16x2", result, input, bits);
    delete input;
  }
  {
    const uint32_t result = uint32_t(0x10000);
    uint32_t *input = new uint32_t(0x1);
    unsigned bits = 32;
    testPacked("u16x2", result, input, bits);
    delete input;
  }
  {
    const uint64_t result = uint64_t(0x101010101010000LL);
    uint64_t *input = new uint64_t(0x1);
    unsigned bits = 64;
    testPacked("s8x8", result, input, bits);
    delete input;
  }
  {
    const uint64_t result = uint64_t(0x100000000LL);
    uint64_t *input = new uint64_t(0x1);
    unsigned bits = 64;
    testPacked("f32x2", result, input, bits);
    delete input;
  }
  {
    const uint64_t result = uint64_t(0x101010101010000LL);
    uint64_t *input = new uint64_t(0x1);
    unsigned bits = 64;
    testPacked("u8x8", result, input, bits);
    delete input;
  }
}

static const char packedConstants[] =
  "version 0:96:$full:$small;\n"
  "kernel &packedConstants(kernarg_u32 %%r)\n"
  "{\n"
  "  ld_kernarg_u32 $s0, [%r];\n"
  "  mov_b%u $%c2, 0;\n"
  "  add_pp_%s $%c3, $%c2, %s;\n"
  "  st_u%u $%c3, [$s0];\n"
  "  ret;\n"
  "};\n";
template<class T>
static void testPackedConstants(unsigned bits,
                                const char *value1,
                                const T &result,
                                const char *type) {
  char reg = 0;
  if (bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(packedConstants,
              bits,
              reg,
              type,
              reg,
              reg,
              value1,
              bits,
              reg);

  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("packedConstants");
  T *output = new T;
  *output = 0;
  void *args[] = { &output };
  BE.launch(fun, args);
  EXPECT_EQ(result, *output);

  delete output;
}

TEST(BrigPacked, testPackedConstants) {
  {
    unsigned bits = 32;
    uint32_t result = uint32_t(0xffe90038);
    testPackedConstants(bits, "_s16x2(-23, 56)", result, "s16x2");
  }
  {
    unsigned bits = 32;
    uint32_t result = uint32_t(0x170038);
    testPackedConstants(bits, "_u16x2(23, 56)", result, "u16x2");
  }
  {
    unsigned bits = 64;
    uint64_t result = uint64_t(0x1700380022000aLL);
    testPackedConstants(bits, "_s16x4(23, 56, 34,10)", result, "s16x4");
  }
  {
    unsigned bits = 64;
    uint64_t result = uint64_t(0x1000000010000LL);
    testPackedConstants(bits, "_u16x4(1, 0, 1, 0)", result, "u16x4");
  }
  {
    unsigned bits = 32;
    uint32_t result = uint32_t(0x1738220a);
    testPackedConstants(bits, "_s8x4(23, 56, 34, 10)", result, "s8x4");
  }
  {
    unsigned bits = 32;
    uint32_t result = uint32_t(0x1000100);
    testPackedConstants(bits, "_u8x4(1, 0, 1, 0)", result, "u8x4");
  }
  {
    unsigned bits = 64;
    uint64_t result = uint64_t(0x1738220a00000000LL);
    testPackedConstants(bits, "_s8x8(23, 56, 34, 10, 0, 0, 0, 0)",
                        result, "s8x8");
  }
  {
    unsigned bits = 64;
    uint64_t result = uint64_t(0x400000003f800000LL);
    testPackedConstants(bits, "_f32x2(0d4000000000000000, 0d3ff0000000000000)",
                        result, "f32x2");
  }
}

TEST(BrigKernelTest, VariadicFunction) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;"
    "function &maxofN(arg_f32 %r)(arg_u32 %n, align 8 arg_f32 %last[])"
    "{\n"
      "ld_arg_u32 $s0, [%n];\n"  // s0 holds the number to add
      "mov_b32 $s1, 0;\n"       // s1 holds the sum
      "mov_b32 $s3, 0;\n"       // s3 is the offset into last
      "@loop:\n"
      "cmp_eq_b1_u32 $c1, $s0, 0;\n"      // see if the count is zero
      "cbr $c1, @done;\n"                // if it is, jump to done
      "ld_arg_u32 $s4, [%last][$s3];\n"  // load a value
      "add_f32 $s1, $s1, $s4;\n"        // add the value
      "add_u32 $s3, $s3, 4;\n"         // advance the offset to the next element
      "sub_u32 $s0, $s0, 1;\n"          // decrement the count
      "brn @loop;\n"
      "@done:\n"
      "st_arg_u32 $s1, [%r];\n"
      "ret;\n"
    "};\n"
    "kernel &adder(kernarg_u64 %r)\n"
    "{\n"          // here is an example caller passing in 4 32-bit floats
    "  {\n"
    "    ld_kernarg_u64 $d2, [%r];\n"
    "    align 8 arg_f32 %n[4];\n"
    "    st_arg_f32 1.2f, [%n][0];\n"
    "    st_arg_f32 2.4f, [%n][4];\n"
    "    st_arg_f32 3.6f, [%n][8];\n"
    "    st_arg_f32 6.1f, [%n][12];\n"
    "    arg_u32 %count;\n"
    "    st_arg_u32 4, [%count];\n"
    "    arg_f32 %sum;\n"
    "    call &maxofN(%sum)(%count, %n);\n"
    "    ld_arg_f32 $s0, [%sum];\n"
    "    st_u32 $s0, [$d2];\n"
    " }\n"                              // ... %s0 holds the sum
    "};\n"
    );
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("adder");
  float *output = new float;
  *output = 0;

  void *args[] = { &output };
  BE.launch(fun, args);
  EXPECT_EQ(1.2f + 2.4f + 3.6f +6.1f, *output);

  delete output;
}

TEST(BrigInstTest, VectorPopcount) {
  {
    const uint32_t testVec[] = { 0x0, 0x00000000 };
    testInst("popcount_u32_b32", testVec);
  }
}

TEST(BrigInstTest, VectorFirstBit) {
  {
    const uint32_t testVec[] = { 1, 0x7fffffff };
    testInst("firstbit_u32_u32", testVec);
    testInst("firstbit_u32_s32", testVec);
  }
}

TEST(BrigInstTest, VectorLastBit) {
  {
    const uint32_t testVec[] = { 0, 0x7fffffff };
    testInst("lastbit_u32_u32", testVec);
    testInst("lastbit_u32_s32", testVec);
  }
}

TEST(BrigInstTest, BitSelect) {
  {
    const uint32_t testVec[] = { 0xffffffff, 0xffff0000,
                                 0xffff0000, 0x0000ffff };
    testInst("bitselect_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 0xffffffffeaeabcbcULL,
                                 0xffff0000f0f0f0f0ULL,
                                 0xffff0000eeeebbbbULL,
                                 0x0000ffffaaaaccccULL };
    testInst("bitselect_b64", testVec);
  }
}

TEST(BrigInstTest, VectorNsqrt) {
  {
    const float testVec[] = { 2.0f, 4.0f };
    testInst("nsqrt_f32", testVec);
  }
}

static const char St[] =
    "version 0:96:$full:$large;\n"
    "global_%s &n;\n"
    "global_%s &m = %s;\n"
    "kernel &testSt(kernarg_u64 %r)\n"
    "{\n"
    "  ld_kernarg_u64 $d0, [%r];\n"
    "  ld_global_%s $%c2, [&m];\n"
    "  st_global_%s $%c2, [&n];\n"
    "  ld_global_%s $%c1, [&n];\n"
    "  st_global_%s $%c1, [$d0];\n"
    "  ret;\n"
    "};\n";

template<class T>
static void testSt(const char *type1,
                   const char *type2,
                   const T &result,
                   const char *value,
                   unsigned bits) {
  char reg = 0;
  if (bits == 8 || bits == 16 || bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(St,
              type2,
              type1,
              value,
              type1,
              reg,
              type2,
              reg,
              type2,
              reg,
              type2,
              reg);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T;
  *arg_val0 = 0;

  void *args[] = { &arg_val0 };
  llvm::Function *fun = BP.getFunction("testSt");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(result, *arg_val0);
  delete arg_val0;
}

TEST(BrigKernelTest, testSt) {
  {
    const int16_t result = int16_t(0xff81);
    const char *value = "0x81";
    unsigned bits = 16;
    testSt("s8", "s16",  result, value, bits);
  }
  {
    const int16_t result = int16_t(0x0001);
    const char *value = "0x01";
    unsigned bits = 16;
    testSt("s8", "s16",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0xffffff81);
    const char *value = "0x81";
    unsigned bits = 32;
    testSt("s8", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0x00000001);
    const char *value = "0x01";
    unsigned bits = 32;
    testSt("s8", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0xffff8001);
    const char *value = "0x8001";
    unsigned bits = 32;
    testSt("s16", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0x00000001);
    const char *value = "0x0001";
    unsigned bits = 32;
    testSt("s16", "s32",  result, value, bits);
  }
  {
    const uint16_t result = uint16_t(0x007f);
    const char *value = "0x7f";
    unsigned bits = 16;
    testSt("u8", "u16",  result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x0000007f);
    const char *value = "0x7f";
    unsigned bits = 32;
    testSt("u8", "u32",  result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x00007fff);
    const char *value = "0x7fff";
    unsigned bits = 32;
    testSt("u16", "u32",  result, value, bits);
  }
}

TEST(BrigKernelTest, Atomic) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__instruction2_test_kernel(\n"
    "        kernarg_s32 %result, \n"
    "        kernarg_s64 %input1, kernarg_s32 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_s64 $d2, [%input2] ;\n"
    "        atomic_and_b32 $s3, [$d2], $s1;\n"
    "        ld_kernarg_s64 $d2, [%result] ;\n"
    "        st_s32 $s3, [$d2] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction2_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345678, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(BrigKernelTest, AtomicNoRet) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__instruction2_test_kernel(\n"
    "        kernarg_s32 %result, \n"
    "        kernarg_s32 %input1, kernarg_s32 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_s64 $d2, [%input2] ;\n"
    "        atomicnoret_and_b32 [$d2], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d2] ;\n"
    "        ld_kernarg_s64 $d2, [%result] ;\n"
    "        st_s32 $s2, [$d2] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction2_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345678, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(BrigInstTest, Barrier) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &BarrierTest()\n"
    "{\n"
    "        barrier;\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("BarrierTest");
  void *args[] = { 0 };
  for (unsigned blocks = 1; blocks < 16; ++blocks)
    for (unsigned threads = 1; threads < 16; ++threads)
      BE.launch(fun, args, blocks, threads);
}

TEST(BrigInstTest, Sync) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &SyncTest()\n"
    "{\n"
    "        sync;\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("SyncTest");
  void *args[] = { 0 };
  BE.launch(fun, args);
}

static const char Ld[] =
    "version 0:96:$full:$large;\n"
    "global_%s &n = %s;\n"
    "kernel &testLd(kernarg_u64 %r)\n"
    "{\n"
    "  ld_kernarg_u64 $d0, [%r];\n"
    "  ld_global_%s $%c1, [&n];\n"
    "  st_global_%s $%c1, [$d0];\n"
    "  ret;\n"
    "};\n";


template<class T>
static void testLd(const char *type1,
                   const char *type2,
                   const T &result,
                   const char *value,
                   unsigned bits) {
  char reg = 0;
  if (bits == 8 || bits == 16 || bits == 32)
    reg = 's';
  if (bits == 64)
    reg = 'd';
  char *buffer =
    asnprintf(Ld,
              type1,
              value,
              type1,
              reg,
              type2,
              reg);
  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  delete[] buffer;

  EXPECT_TRUE(BP);
  if (!BP) return;

  T *arg_val0 = new T;
  *arg_val0 = 0;

  void *args[] = { &arg_val0 };
  llvm::Function *fun = BP.getFunction("testLd");
  hsa::brig::BrigEngine BE(BP);
  BE.launch(fun, args);

  EXPECT_EQ(result, *arg_val0);

  delete arg_val0;
}

TEST(BrigKernelTest, testLd) {
  {
    const int16_t result = int16_t(0xff81);
    const char *value = "0x81";
    unsigned bits = 16;
    testLd("s8", "s16",  result, value, bits);
  }
  {
    const int16_t result = int16_t(0x0001);
    const char *value = "0x01";
    unsigned bits = 16;
    testLd("s8", "s16",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0xffffff81);
    const char *value = "0x81";
    unsigned bits = 32;
    testLd("s8", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0x00000001);
    const char *value = "0x01";
    unsigned bits = 32;
    testLd("s8", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0xffff8001);
    const char *value = "0x8001";
    unsigned bits = 32;
    testLd("s16", "s32",  result, value, bits);
  }
  {
    const int32_t result = int32_t(0x00000001);
    const char *value = "0x0001";
    unsigned bits = 32;
    testLd("s16", "s32",  result, value, bits);
  }
  {
    const uint16_t result = uint16_t(0x007f);
    const char *value = "0x7f";
    unsigned bits = 16;
    testLd("u8", "u16",  result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x0000007f);
    const char *value = "0x7f";
    unsigned bits = 32;
    testLd("u8", "u32",  result, value, bits);
  }
  {
    const uint32_t result = uint32_t(0x00007fff);
    const char *value = "0x7fff";
    unsigned bits = 32;
    testLd("u16", "u32",  result, value, bits);
  }
}

TEST(BrigInstTest, VectorBitalign) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitalign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 8;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB0A3A2A1, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitalign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 16;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t( 0 );
    uint32_t *arg1 = new uint32_t(0x55443322);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB1B0A3A2, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitalign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 24;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t( 0 );
    uint32_t *arg1 = new uint32_t(0x55443322);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB2B1B0A3, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(BrigInstTest, VectorBytealign) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bytealign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 1;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB0A3A2A1, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bytealign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 2;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t( 0 );
    uint32_t *arg1 = new uint32_t(0x55443322);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB1B0A3A2, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__instruction4_test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bytealign_b32 $s1, 0xA3A2A1A0, 0xB3B2B1B0, 3;\n"
      "        ld_kernarg_u64 $d0, [%result] ;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
    uint32_t *arg0 = new uint32_t( 0 );
    uint32_t *arg1 = new uint32_t(0x55443322);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xB2B1B0A3, *arg0);
    delete arg0;
    delete arg1;
  }
}


TEST(Instruction2Test, Abs) {
  {
    const int32_t testVec[] = { 1, 1 };
    testInst("abs_s32", testVec);
  }
  {
    const int32_t testVec[] = { 1, -1 };
    testInst("abs_s32", testVec);
  }
  {
    const int64_t testVec[] = { 3, -3 };
    testInst("abs_s64", testVec);
  }
  {
    //_s8x4(-1,0,-1,0)
    const uint32_t testVec[] = { 0x1000100, 0xff00ff00 };
    testInst("abs_p_s8x4", testVec);
  }
  {
    //_s8x8(-1,0,-1,0,-1,0,-1,0)
    const uint64_t testVec[] = {0x100010001000100LL, 0xff00ff00ff00ff00LL};
    testInst("abs_p_s8x8", testVec);
  }
  {
    //_s16x2(-1,-1)
    const uint32_t testVec[] = { 0x10001, 0xffffffff };
    testInst("abs_p_s16x2", testVec);
  }
  {
    //_s16x4(-1,0,-1,0)
    const uint64_t testVec[] = {0x1000000010000LL, 0xffff0000ffff0000LL};
    testInst("abs_p_s16x4", testVec);
  }
  {
    //_s32x2(-2,-2)
    const uint64_t testVec[] = { 0x200000002LL, 0xfffffffefffffffeLL };
    testInst("abs_p_s32x2", testVec);
  }
  {
    //_s8x4(-1,0,0,-1)
    const uint32_t testVec[] = { 0x01010101, 0xff0000ff };
    testInst("abs_s_s8x4", testVec);
  }
  {
    //_s8x4(-1,0,0,2)
    const uint32_t testVec[] = { 0x02020202, 0xff000002 };
    testInst("abs_s_s8x4", testVec);
  }
  {
    //_s8x8(-1,0,-1,0,-1,0,0,-1)
    const uint64_t testVec[] = {0x101010101010101LL, 0xff00ff00ff0000ffLL};
    testInst("abs_s_s8x8", testVec);
  }
  {
    //_s16x2(0,-1)
    const uint32_t testVec[] = { 0x10001, 0xffffffff };
    testInst("abs_s_s16x2", testVec);
  }
  {
    //_s16x4(-1,0,0,-1)
    const uint64_t testVec[] = {0x1000100010001LL, 0xffff00000000ffffLL};
    testInst("abs_s_s16x4", testVec);
  }
  {
    //_s32x2(-1,-2)
    const uint64_t testVec[] = { 0x200000002LL, 0xfffffffffffffffeLL };
    testInst("abs_s_s32x2", testVec);
  }
}

TEST(Instruction2Test, Neg) {
  {
    const int32_t testVec[] = { -1, 1 };
    testInst("neg_s32", testVec);
  }
  {
    const int32_t testVec[] = { 1, -1 };
    testInst("neg_s32", testVec);
  }
  {
    const int64_t testVec[] = { 3, -3 };
    testInst("neg_s64", testVec);
  }
  {
    //_s8x4(-1,0,-1,0)
    const uint32_t testVec[] = { 0x1000100, 0xff00ff00 };
    testInst("neg_p_s8x4", testVec);
  }
  {
    //_s8x8(-1,0,-1,0,-1,0,-1,0)
    const uint64_t testVec[] = {0x100010001000100LL, 0xff00ff00ff00ff00LL};
    testInst("neg_p_s8x8", testVec);
  }
  {
    //_s16x2(-1,-1)
    const uint32_t testVec[] = { 0x10001, 0xffffffff };
    testInst("neg_p_s16x2", testVec);
  }
  {
    //_s16x4(-1,0,-1,0)
    const uint64_t testVec[] = {0x1000000010000LL, 0xffff0000ffff0000LL};
    testInst("neg_p_s16x4", testVec);
  }
  {
    //_s32x2(-2,-2)
    const uint64_t testVec[] = { 0x200000002LL, 0xfffffffefffffffeLL };
    testInst("neg_p_s32x2", testVec);
  }
  {
    //_s8x4(-1,0,0,-1)
    const uint32_t testVec[] = { 0x01010101, 0xff0000ff };
    testInst("neg_s_s8x4", testVec);
  }
  {
    //_s8x8(-1,0,-1,0,-1,0,0,1)
    const uint64_t testVec[] = {0xffffffffffffffffLL, 0xff00ff00ff000001LL};
    testInst("neg_s_s8x8", testVec);
  }
  {
    //_s16x2(-1,1)
    const uint32_t testVec[] = { 0xffffffff, 0xffff0001 };
    testInst("neg_s_s16x2", testVec);
  }
  {
    //_s16x4(-1,0,-1,0)
    const uint64_t testVec[] = {0x0LL, 0xffff0000ffff0000LL};
    testInst("neg_s_s16x4", testVec);
  }
  {
    //_s32x2(-2,-3)
    const uint64_t testVec[] = { 0x300000003LL, 0xfffffffefffffffdLL };
    testInst("neg_s_s32x2", testVec);
  }
  {
    const float testVec[] = { 1.2f, -1.2f };
    testInst("neg_f32", testVec);
  }
  {
    const double testVec[] = { 1.2, -1.2 };
    testInst("neg_f64", testVec);
  }
}

TEST(Instruction2Test, Not) {
  {
    const uint32_t testVec[] = { 0x0, 0xffffffff };
    testInst("not_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 0xffffffffffffffffLL, 0x0LL };
    testInst("not_b64", testVec);
  }
}

TEST(Instruction2Test, Popcount) {
  {
    const uint32_t testVec[] = { 0x0, 0x00000000 };
    testInst("popcount_u32_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 24, 0x00ffffff };
    testInst("popcount_u32_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 31, 0x7fffffff };
    testInst("popcount_u32_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 25, 0x01ffffff };
    testInst("popcount_u32_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 32, 0xffffffff };
    testInst("popcount_u32_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 20, 0xffff0f00 };
    testInst("popcount_u32_b32", testVec);
  }

  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1)\n"
    "{\n"
    "        popcount_u32_b64 $s1, 0xffff0f00f0e00000;\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_u32 $s1, [$s0] ;\n"

    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(27, *arg0);
    delete arg0;
    delete arg1;
  }

}

TEST(Instruction2Test, Bitrev) {
  {
    const uint32_t testVec[] = { 0x1e6a2c48, 0x12345678 };
    testInst("bitrev_b32", testVec);
  }
  {
    const uint32_t testVec[] = { 0x12345678, 0x1e6a2c48 };
    testInst("bitrev_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x1234567812345678LL, 0x1e6a2c481e6a2c48LL };
    testInst("bitrev_b64", testVec);
  }
}

TEST(Instruction2Test, FirstBit) {
  {
    const uint32_t testVec[] = { 0, 0xffffffff };
    testInst("firstbit_u32_u32", testVec);
  }
  {
    const uint32_t testVec[] = { 1, 0x7fffffff };
    testInst("firstbit_u32_u32", testVec);
  }
  {
    const int32_t testVec[] = { -1, 0x0};
    testInst("firstbit_u32_u32", testVec);
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1)\n"
    "{\n"
    "        firstbit_u32_u64 $s1, 0x7ff0f00f0e00000;\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_u32 $s1, [$s0] ;\n"

    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(5, *arg0);
    delete arg0;
    delete arg1;
  }

  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1)\n"
    "{\n"
    "        firstbit_u32_s64 $s1, 0x7ff0f00f0e00000;\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_u32 $s1, [$s0] ;\n"

    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(5, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction2Test, LastBit) {
  {
    const uint32_t testVec[] = { 0, 0xffffffff };
    testInst("lastbit_u32_u32", testVec);
  }
  {
    const uint32_t testVec[] = { 0, 0x7fffffff };
    testInst("lastbit_u32_u32", testVec);
  }
  {
    const uint32_t testVec[] = { 0, 0x01ffffff };
    testInst("lastbit_u32_u32", testVec);
  }
  {
    const int32_t testVec[] = { -1, 0x0};
    testInst("lastbit_u32_u32", testVec);
  }

  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1)\n"
    "{\n"
    "        lastbit_u32_u64 $s1, 0x7fe0000000000;\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_u32 $s1, [$s0] ;\n"

    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(41, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction2Test, Class) {
  // f32
  { // PosInf
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f32 $c1, 0f7f800000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::PosInf);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosInf
  {  // NegInf
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f32 $c1, 0fff800000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::NegInf);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegInf
  {  // QNaN
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f32 $c1, 0f7fc00000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::QNan);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  } // QNaN
  { // SNaN - hard coded SNAN into the test source code to avoid
    // being changed into QNAN on 32-bit platforms
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f32 $c1, 0fff800001, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::SNan);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // SNaN
  {  // PosZero
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = +0.0f;
    uint32_t arg2 = uint32_t(hsa::brig::PosZero);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosZero
  {  // NegZero
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = -0.0f;
    uint32_t arg2 = uint32_t(hsa::brig::NegZero);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegZero
  {  // PosNorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = +1.0f;
    uint32_t arg2 = uint32_t(hsa::brig::PosNorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosNorm
  {  // NegNorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = -1.0f;
    uint32_t arg2 = uint32_t(hsa::brig::NegNorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegNorm
  {  // PosSubnorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = 1e-45f;
    uint32_t arg2 = uint32_t(hsa::brig::PosSubnorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosSubnorm
  {  // NegSubnorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f32  $s1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f32 $c1, $s1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f32 arg1 = -1e-45f;
    uint32_t arg2 = uint32_t(hsa::brig::NegSubnorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }

  // f64
  {  // PosInf
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f64 $c1, 0d7ff0000000000000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint64_t* arg0  = new uint64_t(0);
    uint32_t arg1 = uint32_t(hsa::brig::PosInf);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosInf
  {  // NegInf
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f64 $c1, 0dfff0000000000000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint64_t* arg0  = new uint64_t(0);
    uint32_t arg1 = uint32_t(hsa::brig::NegInf);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegInf
  {  // QNaN
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f64 $c1, 0d7ff8000000000000, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::QNan);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // QNaN
  { // SNaN - hard coded SNAN into the test source code to avoid
    // being changed into QNAN on 32-bit platforms
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_u32  $s1, [%input];\n"
    "        class_b1_f64 $c1, 0dfff0000000000001, $s1;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    uint32_t arg1 = uint32_t(hsa::brig::SNan);
    void *args[] = { &arg0, &arg1};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // SNaN
  {  // PosZero
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f64 arg1 = +0.0l;
    uint32_t arg2 = uint32_t(hsa::brig::PosZero);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosZero
  {  // NegZero
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f64 arg1 = -0.0l;
    uint32_t arg2 = uint32_t(hsa::brig::NegZero);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegZero
  {  // PosNorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f64 arg1 = +1.0l;
    uint32_t arg2 = uint32_t(hsa::brig::PosNorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosNorm
  {  // NegNorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    f64 arg1 = -1.0l;
    uint32_t arg2 = uint32_t(hsa::brig::NegNorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // NegNorm
  {  // PosSubnorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    double arg1 = 1e-323l;
    uint32_t arg2 = uint32_t(hsa::brig::PosSubnorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }  // PosSubnorm
  {  // NegSubnorm
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u32 %input1, \n"
    "        kernarg_u32 %input2)\n"
    "{\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        ld_kernarg_f64  $d1, [%input1];\n"
    "        ld_kernarg_u32  $s2, [%input2];\n"
    "        class_b1_f64 $c1, $d1, $s2;\n"
    "        cbr $c1, @BB;\n"
    "        ret;\n"
    "@BB:\n"
    "        ld_kernarg_u32 $s0, [%result] ;\n"
    "        st_global_u32 1, [$s0] ;\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t* arg0  = new uint32_t();
    double arg1 = -1e-323l;
    uint32_t arg2 = uint32_t(hsa::brig::NegSubnorm);
    void *args[] = { &arg0, &arg1, &arg2};
    BE.launch(fun, args);
    EXPECT_EQ(1, *arg0);
    delete arg0;
  }
}

TEST(Instruction2Test, Mov) {
  {
    const uint32_t testVec[] = { 0x7fffffff, 0x7fffffff };
    testInst("mov_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 0xffffffffffff0f00LL, 0xffffffffffff0f00LL };
    testInst("mov_b64", testVec);
  }
}

TEST(Instruction2Test, Ncos) {
  {
    const float testVec[] = { 1.0f, 0.0f };
    testInst("ncos_f32", testVec);
  }
}

TEST(Instruction2Test, Nsin) {
  {
    const float testVec[] = { 1.0f, M_PI/2 };
    testInst("nsin_f32", testVec);
  }
}

TEST(Instruction2Test, Nlog2) {
  {
    const float testVec[] = { 2.0f, 4.0f };
    testInst("nlog2_f32", testVec);
  }
}

TEST(Instruction2Test, Nexp2) {
  {
    const float testVec[] = { 16.0f, 4.0f };
    testInst("nexp2_f32", testVec);
  }
}

TEST(Instruction2Test, Nsqrt) {
  {
    const float testVec[] = { 2.0f, 4.0f };
    testInst("nsqrt_f32", testVec);
  }
  {
    const double testVec[] = { 2.0, 4.0 };
    testInst("nsqrt_f64", testVec);
  }
}

TEST(Instruction2Test, Copysign) {
  {
    const float testVec[] = { -456.5f, 456.5f, -0.22f };
    testInst("copysign_f32", testVec);
  }
}

TEST(Instruction2Test, Nrsqrt) {
  {
    const float testVec[] = { 0.5f, 4.0f };
    testInst("nrsqrt_f32", testVec);
  }
  {
    const double testVec[] = { 0.5, 4.0 };
    testInst("nrsqrt_f64", testVec);
  }
}

TEST(Instruction2Test, Nrcp) {
  {
    const float testVec[] = { 0.25f, 4.0f };
    testInst("nrcp_f32", testVec);
  }
  {
    const double testVec[] = { 0.25, 4.0 };
    testInst("nrcp_f64", testVec);
  }
}

TEST(Instruction2Test, Sqrt) {
  {
    const float testVec[] = { 2.0f, 4.0f };
    testInst("sqrt_f32", testVec);
  }
  {
    const float testVec[] = { 2.0f, 4.0f };
    testInst("sqrt_ftz_f32", testVec);
  }
  {
    const double testVec[] = { 2.0, 4.0 };
    testInst("sqrt_f64", testVec);
  }
  {
    const double testVec[] = { 2.0, 4.0 };
    testInst("sqrt_ftz_f64", testVec);
  }
}

TEST(Instruction2Test, Fract) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__instruction2_test_kernel(\n"
    "        kernarg_s32 %result, \n"
    "        kernarg_s32 %input)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input] ;\n"
    "        fract_f32 $s2, $s1 ;\n"
    "        ld_kernarg_u64 $d0, [%result] ;\n"
    "        st_s32 $s2, [$d0] ;\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction2_test_kernel");
  float *arg0 = new float(0.0f);
  float *arg1 = new float(4.6f);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_FLOAT_EQ(0.6f, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction2Test, Ceil) {
  {
    const float testVec[] = { 2.0, 1.2 };
    testInst("ceil_f32", testVec);
  }
  {
    const float testVec[] = { 1.0, 1e-45 };
    testInst("ceil_f32", testVec);
  }
  {
    const float testVec[] = { 0.0, 1e-45 };
    testInst("ceil_ftz_f32", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.2L };
    testInst("ceil_f64", testVec);
  }
  {
    const float testVec[] = { -1.0 , -1.8 };
    testInst("ceil_f32", testVec);
  }
  {
    const double testVec[] = { -1.0L , -1.8L };
    testInst("ceil_f64", testVec);
  }
}

TEST(Instruction2Test, Floor) {
  {
    const float testVec[] = { 1.0, 1.8 };
    testInst("floor_f32", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.8L };
    testInst("floor_f64", testVec);
  }
  {
    const float testVec[] = { -2.0 , -1.2 };
    testInst("floor_f32", testVec);
  }
  {
    const double testVec[] = { -2.0L , -1.2L };
    testInst("floor_f64", testVec);
  }
  {
    const float testVec[] = { -1.0,-1e-45 };
    testInst("floor_f32", testVec);
  }
  {
    const float testVec[] = { -0.0,-1e-45 };
    testInst("floor_ftz_f32", testVec);
  }
}

TEST(Instruction2Test, Trunc) {
  {
    const float testVec[] = { 1.0, 1.7 };
    testInst("trunc_f32", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.7L };
    testInst("trunc_f64", testVec);
  }
  {
    const float testVec[] = { -1.0 , -1.7 };
    testInst("trunc_f32", testVec);
  }
  {
    const double testVec[] = { -1.0L , -1.7L };
    testInst("trunc_f64", testVec);
  }
}

TEST(Instruction2Test, Rint) {
  {
    const float testVec[] = { 2.0, 1.7 };
    testInst("rint_f32", testVec);
  }
  {
    const float testVec[] = { 1.0, 1.2 };
    testInst("rint_f32", testVec);
  }
  {
    const float testVec[] = { 2.0, 1.5 };
    testInst("rint_f32", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.7L };
    testInst("rint_f64", testVec);
  }
  {
    const double testVec[] = { 1.0L, 1.2L };
    testInst("rint_f64", testVec);
  }
  {
    const double testVec[] = { 2.0L, 1.5L };
    testInst("rint_f64", testVec);
  }
}

TEST(Instruction3Test, Add) {
  {
    const uint32_t testVec[] = { 0xffffffff, 0x7fffffff, 0x80000000};
    testInst("add_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0x1, 0xfffffffe, 0x3};
    testInst("add_s32", testVec);
  }
  {
    const uint64_t testVec[] = { 0xfffffffffffffffeULL,
                                 0x7ffffffffffffffeULL,
                                 0x8000000000000000ULL };
    testInst("add_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0x1ULL, 0xfffffffffffffffeULL, 0x3ULL};
    testInst("add_u64", testVec);
  }
  {
    const uint32_t testVec[] = { 0xefffff0c, 0x7ffffffe, 0x7000000e};
    testInst("add_pp_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xefffffff, 0x7ffffffe, 0x7000000e};
    testInst("add_pp_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x7fffff0c, 0x7ffffffe, 0x7000000e};
    testInst("add_pp_sat_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xffe7ffff, 0x7007fffe, 0x700000e0};
    testInst("add_ps_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x50e7dfde, 0x7007fffe, 0x700000e0};
    testInst("add_ps_sat_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x6efefede, 0x7007fffe, 0x700000e0};
    testInst("add_sp_sat_s8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are not modified.

    const uint32_t testVec[] = { 0xdededede, 0x7007fffe, 0x700000e0};
    testInst("add_ss_sat_s8x4", testVec);
  }
}

TEST(Instruction3Test, Borrow) {
  {
    const uint32_t testVec[] = { 0x1, 0x7ffffffe, 0x80000000};
    testInst("borrow_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 000, 0xffffffff, 0xfffffffd};
    testInst("borrow_u32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x1,
                                 0x7fffffffffffffffLL,
                                 0x8000000000000000LL};
    testInst("borrow_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0x00LL, 0xfffffffffffffffeLL, 0x3LL};
    testInst("borrow_u64", testVec);
  }
}

TEST(Instruction3Test, Carry) {
  {
    const uint32_t testVec[] = { 0, 0x7ffffffe, 0x80000000};
    testInst("carry_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 1, 0xfffffffe, 0x3};
    testInst("carry_u32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0LL, 0x7ffffffffffffffeLL,
                                 0x8000000000000000LL};
    testInst("carry_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0x0LL, 0xfffffffffffffffdLL, 0x2LL};
    testInst("carry_u64", testVec);
  }
}

TEST(Instruction3Test, Div) {
  {
    const int32_t testVec[] = { -0x12abcdef, -0x380369cd, 0x3};
    testInst("div_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0x11111111, 0xffffffff, 0xf};
    testInst("div_u32", testVec);
  }
  {
    const int64_t testVec[] = { 0x1LL, 0x7fffffffffffffffLL,
                                0x7ffffffffffffffeLL};
    testInst("div_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0x10000020000LL, 0xfffffffffffffffeLL,
                                 0xfffffeLL};
    testInst("div_u64", testVec);
  }
}

TEST(Instruction3Test, Rem) {
  {
    const int32_t testVec[] = { 0x0, 0x380369cd, 0x3};
    testInst("rem_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0x3, 0xffffffff, 0xe};
    testInst("rem_u32", testVec);
  }
  {
    const int64_t testVec[] = { 0x2LL, 0x7fffffffffffffffLL,
                                0x7ffffffffffffffdLL};
    testInst("rem_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0xfffffffffffffffdLL,
                                 0xfffffffffffffffdLL,
                                 0xffffffffffffffffLL};
    testInst("rem_u64", testVec);
  }
}

TEST(Instruction3Test, Sub) {
  {
    const uint32_t testVec[] = { 0xffffffff, 0x7ffffffe, 0x7fffffff };
    testInst("sub_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfffffffe, 0xfffffffd, 0xffffffff };
    testInst("sub_u32", testVec);
  }
  // u8x4
  {
    const uint32_t testVec[] = { 0xa00fffe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xa000000, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_pp_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfbfcfdfe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ps_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x0, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ps_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xd0200fe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_sp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xd020000, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_sp_sat_u8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = { 0xfefefefe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ss_u8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = { 0x0, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ss_sat_u8x4", testVec);
  }
  // u16x2
  {
    const uint32_t testVec[] = { 0xa00fefe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_pp_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0xa000000, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_pp_sat_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfcfcfefe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ps_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0x0, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ps_sat_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0x0c02fefe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_sp_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0x0c020000, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_sp_sat_u16x2", testVec);
  }
    {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = { 0xfefefefe, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ss_u16x2", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = { 0x0, 0xfafbfcfd, 0xf0fbfdff };
    testInst("sub_ss_sat_u16x2", testVec);
  }
}

TEST(Instruction3Test, Shl) {
  {
    const int32_t testVec[] = { -506, -253, 33 };
    testInst("shl_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 506, 253, 33 };
    testInst("shl_u32", testVec);
  }
}

TEST(Instruction3Test, Shr) {
  {
    const int32_t testVec[] = { 126, 253, 33 };
    testInst("shr_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 126, 253, 33 };
    testInst("shr_u32", testVec);
  }
}

TEST(Instruction3Test, And) {
  {
    const uint32_t testVec[] = { 9, 13, 11 };
    testInst("and_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 74898LL, 224694LL, 74898LL };
    testInst("and_b64", testVec);
  }
}

TEST(Instruction3Test, Or) {
  {
    const uint32_t testVec[] = { 15, 13, 11 };
    testInst("or_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 224694LL, 224694LL, 74898LL };
    testInst("or_b64", testVec);
  }
}

TEST(Instruction3Test, Xor) {
  {
    const uint32_t testVec[] = { 6, 13, 11 };
    testInst("xor_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 149796LL, 224694LL, 74898LL };
    testInst("xor_b64", testVec);
  }
}

TEST(Instruction3Test, Unpackhi) {
  {
    const int32_t testVec[] = { 0x127e3474, 0x7e74feee, 0x12345678 };
    testInst("unpackhi_s8x4", testVec);
  }
  {
    const int64_t testVec[] = { 0x127e347456fe78eeLL,
                                0x7e74feee0a0afffeLL,
                                0x1234567812345678LL };
    testInst("unpackhi_s8x8", testVec);

  }
  {
    const int32_t testVec[] = { 0x12347e74, 0x7e74feee, 0x12345678};
    testInst("unpackhi_s16x2", testVec);
  }
  {
    const int64_t testVec[] = { 0x12347e745678feeeLL,
                                0x7e74feee0a0afffeLL,
                                0x1234567812345678LL };
    testInst("unpackhi_s16x4", testVec);

  }
}

TEST(Instruction3Test, Unpacklo) {
  {
    const int32_t testVec[] = { 0x56fe78ee, 0x7e74feee, 0x12345678 };
    testInst("unpacklo_s8x4", testVec);
  }
  {
    const int64_t testVec[] = { 0x120a340a56ff78feLL,
                                0x7e74feee0a0afffeLL,
                                0x1234567812345678LL };
    testInst("unpacklo_s8x8", testVec);

  }
  {
    const int32_t testVec[] = { 0x5678feee, 0x7e74feee,0x12345678 };
    testInst("unpacklo_s16x2", testVec);
  }
  {
    const int64_t testVec[] = { 0x12340a0a5678fffeLL,
                                0x7e74feee0a0afffeLL,
                                0x1234567812345678LL };
    testInst("unpacklo_s16x4", testVec);

  }
  {
    const int64_t testVec[] = { 0x123456780a0afffeLL,
                                0x7e74feee0a0afffeLL,
                                0x1234567812345678LL };
    testInst("unpacklo_s32x2", testVec);

  }
}

TEST(Instruction3Test, Max) {
  {
    const uint32_t testVec[] = { 0x7fffffff, 0x7fffffff, 0x80000000 };
    testInst("max_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0xffffffff, 0xfffffffe, 0xffffffff };
    testInst("max_u32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x7ffffffffffffffeLL,
                                 0x7ffffffffffffffeLL,
                                 0x8000000000000001LL };
    testInst("max_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0xffffffffffffffffLL,
                                 0xffffffffffffffffLL,
                                 0xfffffffffffffffeLL };
    testInst("max_u64", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfefdfbff, 0xfefcfbfe, 0xfafdfbff };
    testInst("max_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xffffffff, 0xfefcfbfe, 0xfafdfbff };
    testInst("max_ps_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfefefeff, 0xfefcfbfe, 0xfafdfbff };
    testInst("max_sp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xffffffff, 0xfefcfbfe, 0xfafdfbff };
    testInst("max_ss_u8x4", testVec);
  }
}

TEST(Instruction3Test, Min) {
  {
    const uint32_t testVec[] = { 0x80000002, 0x7fffffff, 0x80000002 };
    testInst("min_s32", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfffffffe, 0xfffffffe, 0xffffffff };
    testInst("min_u32", testVec);
  }
  {
    const uint64_t testVec[] = { 0x8000000000000001LL,
                                 0x7ffffffffffffffeLL,
                                 0x8000000000000001LL };
    testInst("min_s64", testVec);
  }
  {
    const uint64_t testVec[] = { 0xfffffffffffffffeLL,
                                 0xfffffffffffffffeLL,
                                 0xfffffffffffffffeLL };
    testInst("min_u64", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfafcfbfe, 0xfefcfbfe, 0xfafdfbff };
    testInst("min_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfefcfbfe, 0xfefcfbfe, 0xfafdfbff };
    testInst("min_ps_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfafdfbfe, 0xfefcfbfe, 0xfafdfbff };
    testInst("min_sp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfefefefe, 0xfefcfbfe, 0xfafdfbff };
    testInst("min_ss_u8x4", testVec);
  }
}

TEST(Instruction3Test, BitMask) {
  {
    const uint32_t testVec[] = {0x1e00, 9, 4};
    testInst("bitmask_b32", testVec);
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitmask_b64 $d1, 33, 4;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0ULL);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0x1e00000000ULL, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction3Test, Cmp) {
  { // Packed Cmp
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        cmp_lt_pp_u8x4_u8x4 $s1, 0x11223344, 0x44221155; \n "
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xff0000ff, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        cmp_lt_pp_u16x2_u16x2 $s1, 0x11223344, 0x44221155; \n "
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffff0000, *arg0);
    delete arg0;
    delete arg1;
  }
  { // Cmp_u32_b1 or s32_b1
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u32 $s0, [%result];\n"
      "        cmp_eq_u32_b1 $s1, 1, 1; \n "
      "        st_u32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffffffff, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u32 $s0, [%result];\n"
      "        cmp_eq_u32_b1 $s1, 1, 0; \n "
      "        st_u32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0x0, *arg0);
    delete arg0;
    delete arg1;
  }
  { // Cmp_f32_b1 or f64_b1
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_f32 $s0, [%result];\n"
      "        cmp_eq_f32_b1 $s1, 1, 1; \n "
      "        st_f32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(1.0, *arg0);
    delete arg0;
    delete arg1;
  }
  { // Cmp_f32_b1 or f64_b1
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_f32 $s0, [%result];\n"
      "        cmp_eq_f32_b1 $s1, 1, 0; \n "
      "        st_f32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0.0, *arg0);
    delete arg0;
    delete arg1;
  }
  { // Cmp_u32_u32
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u32 $s0, [%result];\n"
      "        cmp_lt_u32_u32 $s1, 0x12345678, 0x87654321; \n "
      "        st_u32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffffffff, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_u32 $s0, [%result];\n"
      "        cmp_gt_u32_u32 $s1, 0x12345678, 0x87654321; \n "
      "        st_u32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0x0, *arg0);
    delete arg0;
    delete arg1;
  }
  { // Cmp_f32_u32
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_f32 $s0, [%result];\n"
      "        cmp_lt_f32_u32 $s1, 0x12345678, 0x87654321; \n "
      "        st_f32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(1.0, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        ld_kernarg_f32 $s0, [%result];\n"
      "        cmp_gt_f32_u32 $s1, 0x12345678, 0x87654321; \n "
      "        st_f32 $s1, [$s0] ;\n"
      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0.0, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction3Test, Mul) {
  {
    const uint32_t testVec[] = {1000, 100, 10};
    testInst("mul_u32", testVec);
  }
  {
    const int32_t testVec[] = {-1000, 100, -10};
    testInst("mul_s32", testVec);
  }
  {
    const uint64_t testVec[] = {0x100000000ULL, 2, 0x80000000ULL};
    testInst("mul_u64", testVec);
  }
  {
    const uint64_t testVec[] = {0xFFFFFFFF00000000ULL, ~2ULL + 1,
                                0x80000000ULL};
    testInst("mul_s64", testVec);
  }
  // packed mul unsigned
  {
    const uint32_t testVec[] = {0x40606d1, 0x403021f, 0x102030f};
    testInst("mul_pp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x40606ff, 0x403021f, 0x102030f};
    testInst("mul_pp_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x1f3e5dd1, 0x403021f, 0x102030f};
    testInst("mul_sp_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x1f3e5dff, 0x403021f, 0x102030f};
    testInst("mul_sp_sat_u8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = {0xd1d1d1d1, 0x403021f, 0x102030f};
    testInst("mul_ss_u8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = {0xffffffff, 0x403021f, 0x102030f};
    testInst("mul_ss_sat_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x3c2d1ed1, 0x403021f, 0x102030f};
    testInst("mul_ps_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x3c2d1eff, 0x403021f, 0x102030f};
    testInst("mul_ps_sat_u8x4", testVec);
  }

  // packed mul signed
  {
    const uint32_t testVec[] = {0x4060600, 0x40302f0, 0x10203f0};
    testInst("mul_pp_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0x406067f, 0x40302f0, 0x10203f0};
    testInst("mul_pp_sat_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0xf0e0d000, 0x40302f0, 0x10203f0};
    testInst("mul_sp_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0xf0e0d07f, 0x40302f0, 0x10203f0};
    testInst("mul_sp_sat_s8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = {0x0,  0x40302f0, 0x10203f0};
    testInst("mul_ss_s8x4", testVec);
  }
  {
    // Note: PRM  4.12 Table 4-5
    // ss : the lower element of both sources is used.
    // the result is written into the lower element of the dest.
    // the other bits of the dest are NOT modified.

    const uint32_t testVec[] = {0x7f7f7f7f, 0x40302f0, 0x10203f0};
    testInst("mul_ss_sat_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0xc0d0e000, 0x40302f0, 0x10203f0};
    testInst("mul_ps_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0xc0d0e07f, 0x40302f0, 0x10203f0};
    testInst("mul_ps_sat_s8x4", testVec);
  }
}

TEST(Instruction3Test, Unpack) {
  {
    const uint32_t testVec[] = {0x11ff, 0x11ff00ff, 1 };
    testInst("unpack_u32_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = {0x0, 0x11ff00ff, 1 };
    testInst("unpack_u32_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = {0xff, 0x11ff00ff, 0 };
    testInst("unpack_u32_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = {0x11, 0x11ff00ff, 3 };
    testInst("unpack_u32_u8x4", testVec);
  }

  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u64 %input1)\n"
    "{\n"
    "        ld_kernarg_u64 $d2, [%input1];\n"
    "        unpack_u32_u32x2 $s1, $d2, 1;\n"
    "        ld_kernarg_u32 $s0, [%result];\n"
    "        st_u32 $s1, [$s0];\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint64_t *arg1 = new uint64_t(0xffff000076771111LL);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffff0000, *arg0);
    delete arg0;
    delete arg1;
  }

}

TEST(Instruction3Test, UnpackCvt) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "    unpackcvt_f32_u8x4 $s1, 0xff020003, 0;\n"
      "    ld_kernarg_u32 $s0, [%result];\n"
      "    st_f32 $s1, [$s0] ;\n"
      "    ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_FLOAT_EQ(3.0, *arg0);

    delete arg0;
    delete arg1;
  }

  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$small;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "    unpackcvt_f32_u8x4 $s1, 0xff020003, 3;\n"
      "    ld_kernarg_u32 $s0, [%result];\n"
      "    st_f32 $s1, [$s0] ;\n"
      "    ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    f32 *arg0 = new f32(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_FLOAT_EQ(255.0, *arg0);

    delete arg0;
    delete arg1;
  }
}


TEST(Instruction4Test, Fma) {
  {
    const float testVec[] = { 14.6, 1.1, 6, 8 };
    testInst("fma_f32", testVec);
  }
  {
    const double testVec[] = { 18.884888, 1.666, 6, 8.888888 };
    testInst("fma_f64", testVec);
  }
}

TEST(Instruction4Test, NFma) {
  {
    const float testVec[] = { 14.6, 1.1, 6, 8 };
    testInst("nfma_f32", testVec);
  }
  {
    const double testVec[] = { 18.884888, 1.666, 6, 8.888888 };
    testInst("nfma_f64", testVec);
  }
}

TEST(Instruction4Test, Bitselect) {
  {
    const uint32_t testVec[] = { 5, 6, 13, 7 };
    testInst("bitselect_b32", testVec);
  }
  {
    const uint64_t testVec[] = { 277LL, 546LL, 456LL, 789LL };
    testInst("bitselect_b64", testVec);
  }
}

TEST(Instruction4Test, Bitextract) {
  {
    const uint32_t testVec[] = { 1, 134, 7, 22 };
    testInst("bitextract_u32", testVec);
  }
  {
    const uint32_t testVec[] = { 4, 134, 5, 3 };
    testInst("bitextract_u32", testVec);
  }
  {
    const int32_t testVec[] = { -4, 134, 5, 3 };
    testInst("bitextract_s32", testVec);
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitextract_u64 $d1, 0x008600000000, 33, 7;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0x43ULL, *arg0);
    delete arg0;
    delete arg1;
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitextract_s64 $d1, 0x008600000000, 33, 7;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xFFFFFFFFFFFFFFC3ULL, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction4Test, Mad) {
  {
    const uint32_t testVec[] = { 9, 2, 2, 5};
    testInst("mad_u32", testVec);
  }
  {
    const int32_t testVec[] = { 1, 2, 2, -3};
    testInst("mad_s32", testVec);
  }
}

TEST(Instruction4Test, Shuffle) {
  {  // 8x4
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        shuffle_s8x4 $s1, 0x11223344, 0x55667788, 0x6C;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u32 $s1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0x77661144, *arg0);
    delete arg0;
    delete arg1;
  }
  { // 8x8
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        shuffle_u8x8 $d1, 0x1122334455667788, "
      "                          0xffeeddccbbaa0011, "
      "                          0xE31AA3;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xff11ee0033664455LL, *arg0);
    delete arg0;
    delete arg1;
  }
  { // 16x2
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        shuffle_u16x2 $s1, 0x11335577, "
      "                          0xffddbb00, "
      "                          0x2;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u32 $s1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffdd5577, *arg0);
    delete arg0;
    delete arg1;
  }
  { // 16x4
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        shuffle_u16x4 $d1, 0x1122334455667788, "
      "                          0xffeeddccbbaa0011, "
      "                          0x6C;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xbbaaddcc11227788LL, *arg0);
    delete arg0;
    delete arg1;
  }
  { // 32x2
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        shuffle_u32x2 $d1, 0x1122334455667788, "
      "                          0xffeeddccbbaa0011, "
      "                          0x2;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u64 $d1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffeeddcc55667788LL, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction4Test, Sad) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u32 %input1)\n"
  "{\n"
  "        sad_u32_u32 $s1, 0x22222222, 0x11111111, 0x1;\n"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u32 $s1, [$d0] ;\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  uint32_t *arg1 = new uint32_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0x11111112, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, Sad_u32_u16x2) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u32 %input1)\n"
  "{\n"
  "        sad_u32_u16x2 $s1, 0x22222222, 0x11111111, 0x1;\n"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u32 $s1, [$d0] ;\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  uint32_t *arg1 = new uint32_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0x2223, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, Sad_u32_u8x4) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u32 %input1)\n"
  "{\n"
  "        sad_u32_u8x4 $s1, 0x22232232, 0x11111111, 0x1;\n"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u32 $s1, [$d0] ;\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  uint32_t *arg1 = new uint32_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0x56, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, SadHi_u16x2_u8x4) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u32 %input1)\n"
  "{\n"
  "        sadhi_u16x2_u8x4 $s1, 0x22232232, 0x11111111, 0x1;\n"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u32 $s1, [$d0] ;\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  uint32_t *arg1 = new uint32_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0x550001, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, Cmov_32) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u32 %input1)\n"
  "{\n"
  "        cmov_b32 $s1, 0, 13, 7;\n"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u32 $s1, [$d0];\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  uint32_t *arg1 = new uint32_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(7, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, Cmov_64) {
  hsa::brig::BrigProgram BP = TestHSAIL(
  "version 0:96:$full:$large;\n"
  "\n"
  "kernel &__instruction4_test_kernel(\n"
  "        kernarg_u64 %result, \n"
  "        kernarg_u64 %input1)\n"
  "{\n"
  "        cmov_b64 $d1, 14473758, 0x5555666677778888, 14473758;"
  "        ld_kernarg_u64 $d0, [%result];\n"
  "        st_u64 $d1, [$d0];\n"

  "        ret;\n"
  "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction4_test_kernel");
  uint64_t *arg0 = new uint64_t(0);
  uint64_t *arg1 = new uint64_t(0);
  void *args[] = { &arg0, arg1 };
  BE.launch(fun, args);
  EXPECT_EQ(0x5555666677778888ULL, *arg0);
  delete arg0;
  delete arg1;
}

TEST(Instruction4Test, Lerp) {
  {
    const uint32_t testVec[] = { 0x7070807, 0x0E0C0A08, 0x020507, 0x01000100};
    testInst("lerp_u8x4", testVec);
  }
}

TEST(Instruction5Test, PackCvt) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__instruction5_test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_f32 %input1, \n"
    "        kernarg_f32 %input2, \n"
    "        kernarg_f32 %input3, \n"
    "        kernarg_f32 %input4) \n"
    "{\n"
    "        ld_kernarg_f32 $s1, [%input1];\n"
    "        ld_kernarg_f32 $s2, [%input2];\n"
    "        ld_kernarg_f32 $s3, [%input3];\n"
    "        ld_kernarg_f32 $s4, [%input4];\n"
    "        packcvt_u8x4_f32 $s9, $s1, $s2, $s3, $s4 ;\n"
    "        ld_kernarg_u32 $s0, [%result];\n"
    "        st_global_u32 $s9, [$s0] ;\n"
    "        ret;\n"
    "};\n");

  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__instruction5_test_kernel");
  uint32_t *arg0 = new uint32_t(0);
  f32 *arg1 = new f32(3.4);
  f32 *arg2 = new f32(-1.6);
  f32 *arg3 = new f32(1.6);
  f32 *arg4 = new f32(256.6);
  void *args[] = { &arg0, arg1, arg2, arg3, arg4 };
  BE.launch(fun, args);
  EXPECT_EQ(0xff020003, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
  delete arg3;
  delete arg4;
}

TEST(Instruction5Test, BitInsert) {
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &__test_kernel(\n"
      "        kernarg_u32 %result, \n"
      "        kernarg_u32 %input1)\n"
      "{\n"
      "        bitinsert_u32 $s1, 0xffffeefe,\n"
      "                           0x33,\n"
      "                           5, 7;\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        st_u32 $s1, [$d0] ;\n"

      "        ret;\n"
      "};\n");

    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction ("__test_kernel");
    uint32_t *arg0 = new uint32_t(0);
    uint32_t *arg1 = new uint32_t(0);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xffffe67e, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(Instruction4Test, Pack) {
  {
    const uint32_t testVec[] = {0xff1111ff, 0x11ff, 0xff11, 1 };
    testInst("pack_u16x2_u32", testVec);
  }
  {
    const uint32_t testVec[] = {0x22ff, 0x11ff, 0xff22, 1 };
    testInst("pack_u8x4_u32", testVec);
  }
  {
    const uint32_t testVec[] = {0x2211ff, 0x11ff, 0xff22, 2 };
    testInst("pack_u8x4_u32", testVec);
  }
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$small;\n"
    "\n"
    "kernel &__test_kernel(\n"
    "        kernarg_u32 %result, \n"
    "        kernarg_u64 %input1)\n"
    "{\n"
    "        ld_kernarg_u64 $d2, [%input1];\n"
    "        pack_u32x2_u32 $d1, $d2, 0xfe, 1;\n"
    "        ld_kernarg_u32 $s0, [%result];\n"
    "        st_u64 $d1, [$s0];\n"
    "        ret;\n"
    "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("__test_kernel");
    uint64_t *arg0 = new uint64_t(0);
    uint64_t *arg1 = new uint64_t(0xffff000076771111ULL);
    void *args[] = { &arg0, arg1 };
    BE.launch(fun, args);
    EXPECT_EQ(0xfe76771111ULL, *arg0);
    delete arg0;
    delete arg1;
  }
}

TEST(AtomTest, And) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_and_b32 $s3, [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345678, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Or) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_or_b32 $s3, [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0xffffffff, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Xor) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_xor_b32 $s3, [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0xedcba987, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Exch) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_exch_b32 $s3, [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0xffffffff, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Add) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_add_s32 $s3, [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345677, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Sub) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_sub_s32 $s3, [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(3);
  int32_t *arg2 = new int32_t(23);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(20, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Max) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_max_s32 $s3, [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(23);
  int32_t *arg2 = new int32_t(3);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(23, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomTest, Min) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atom_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomic_min_s32 $s3, [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atom_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(23);
  int32_t *arg2 = new int32_t(3);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(3, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, And) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_and_b32 [$d0], $s1;\n"
    "        ld_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345678, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Or) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s32 %result, \n"
    "        kernarg_s32 %input1, kernarg_s32 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_or_b32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0xffffffff, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Xor) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_xor_b32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0xedcba987, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Add) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_add_s32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(0xffffffff);
  int32_t *arg2 = new int32_t(0x12345678);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(0x12345677, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Sub) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_sub_s32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(23);
  int32_t *arg2 = new int32_t(3);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(-20, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Max) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_max_s32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(23);
  int32_t *arg2 = new int32_t(3);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(23, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(AtomicNoRetTest, Min) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &__atomicnoret_test_kernel(\n"
    "        kernarg_s64 %result, \n"
    "        kernarg_s32 %input1, kernarg_s64 %input2)\n"
    "{\n"
    "        ld_kernarg_s32 $s1, [%input1] ;\n"
    "        ld_kernarg_u64 $d0, [%input2];\n"
    "        atomicnoret_min_s32 [$d0], $s1;\n"
    "        ld_kernarg_s32 $s2, [$d0] ;\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        st_s32 $s2, [$d0] ;\n"

    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("__atomicnoret_test_kernel");
  int32_t *arg0 = new int32_t(0);
  int32_t *arg1 = new int32_t(3);
  int32_t *arg2 = new int32_t(23);
  void *args[] = { &arg0, arg1, &arg2 };
  BE.launch(fun, args);
  EXPECT_EQ(3, *arg0);
  delete arg0;
  delete arg1;
  delete arg2;
}

TEST(BrigInstTest, Clock) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &clock_test(kernarg_s64 %result)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        clock_u64 $d1;\n"
    "        st_u64 $d1, [$d0] ;\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("clock_test");

  uint64_t time = 0;
  uint64_t *result = new uint64_t(0);
  void *args[] = { &result };
  for(unsigned i = 0; i < 1024; ++i) {
    BE.launch(fun, args);
    EXPECT_LT(time, *result);
    time = *result;
  }
  delete result;
}

TEST(BrigInstTest, LaneId) {
  const uint32_t testVec[] = { 0 };
  testInst("laneid_u32", testVec);
}

TEST(BrigInstTest, MaxWaveId) {
  const uint32_t testVec[] = { 0 };
  testInst("maxwaveid_u32", testVec);
}

TEST(BrigInstTest, WaveId) {
  const uint32_t testVec[] = { 0 };
  testInst("waveid_u32", testVec);
}

TEST(BrigInstTest, CuId) {

  int32_t *maxCuId = new int32_t(-11);
  {
    hsa::brig::BrigProgram BP = TestHSAIL(
      "version 0:96:$full:$large;\n"
      "\n"
      "kernel &getMaxCuId(kernarg_s64 %result)\n"
      "{\n"
      "        ld_kernarg_u64 $d0, [%result];\n"
      "        maxcuid_u32 $s1;\n"
      "        st_u32 $s1, [$d0] ;\n"
      "        ret;\n"
      "};\n");
    EXPECT_TRUE(BP);
    if (!BP) return;

    hsa::brig::BrigEngine BE(BP);
    llvm::Function *fun = BP.getFunction("getMaxCuId");

    void *args[] = { &maxCuId };
    BE.launch(fun, args);
    EXPECT_GE(*maxCuId, 0);
  }

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &getCuId(kernarg_s64 %result)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%result];\n"
    "        cuid_u32 $s1;\n"
    "        st_u32 $s1, [$d0] ;\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("getCuId");

  uint32_t *result = new uint32_t(0);
  void *args[] = { &result };
  for(unsigned i = 0; i < 1024; ++i) {
    BE.launch(fun, args);
    EXPECT_LE(*result, *maxCuId);
  }

  delete result;
  delete maxCuId;
}

TEST(BrigInstTest, Dim) {
  const uint32_t testVec[] = { 1 };
  testInst("dim_u32", testVec);
}

TEST(BrigInstTest, WorkItemAndGroup) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &workItemAndGroupTest(kernarg_s64 %results)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%results];\n"
    "        workitemid_u32 $s0, 0;\n"
    "        workgroupid_u32 $s1, 0;\n"
    "        workgroupsize_u32 $s2, 0;\n"
    "        currentworkgroupsize_u32 $s6, 0;\n"
    "        cmp_ne_b1_u32 $c0, $s2, $s6;\n"
    "        cbr $c0, @return;\n"
    "        mul_u32 $s3, $s2, $s1;\n"
    "        add_u32 $s4, $s3, $s0;\n"
    "        mul_u32 $s5, $s4, 4;\n"
    "        cvt_u64_u32 $d1, $s5;\n"
    "        add_u64 $d2, $d0, $d1;\n"
    "        st_u32 0x31415926, [$d2];\n"
    "@return:\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("workItemAndGroupTest");

  enum { Dim = 32 };
  uint32_t *results = new uint32_t[Dim * Dim];
  for(unsigned i = 0; i < Dim * Dim; ++i) results[i] = 0;

  void *args[] = { &results };
  BE.launch(fun, args, Dim, Dim);
  for(unsigned i = 0; i < Dim; ++i) {
    for(unsigned j = 0; j < Dim; ++j) {
      EXPECT_EQ(0x31415926, results[i * Dim + j]);
    }
  }
  delete[] results;
}

TEST(BrigInstTest, WorkItemAbsId) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &workItemFlatAbsIdTest(kernarg_s64 %results)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%results];\n"
    "        workitemflatabsid_u32 $s0;\n"
    "        mul_u32 $s1, $s0, 4;\n"
    "        cvt_u64_u32 $d1, $s1;\n"
    "        add_u64 $d2, $d0, $d1;\n"
    "        workitemflatid_u32 $s2;\n"
    "        st_u32 $s2, [$d2];\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("workItemFlatAbsIdTest");

  enum { Dim = 32 };
  uint32_t *results = new uint32_t[Dim * Dim];
  for(unsigned i = 0; i < Dim * Dim; ++i) results[i] = 0;

  void *args[] = { &results };
  BE.launch(fun, args, Dim, Dim);
  for(unsigned i = 0; i < Dim; ++i) {
    for(unsigned j = 0; j < Dim; ++j) {
      EXPECT_EQ(j, results[i * Dim + j]);
    }
  }
  delete[] results;
}

TEST(BrigInstTest, GridSize) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &gridSizeTest(kernarg_s64 %results)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%results];\n"
    "        gridsize_u32 $s0, 0\n;"
    "        st_u32 $s0, [$d0];\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("gridSizeTest");

  uint32_t *result = new uint32_t(0);
  void *args[] = { &result };

  for(unsigned i = 1; i < 1024; ++i) {
    BE.launch(fun, args, i);
    EXPECT_EQ(i, *result);
  }

  delete result;
}

TEST(BrigInstTest, GridGroups) {
  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "kernel &gridGroupsTest(kernarg_s64 %results)\n"
    "{\n"
    "        ld_kernarg_u64 $d0, [%results];\n"
    "        gridgroups_u32 $s0, 0\n;"
    "        st_u32 $s0, [$d0];\n"
    "        ret;\n"
    "};\n");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("gridGroupsTest");

  uint32_t *result = new uint32_t(0);
  void *args[] = { &result };

  for(unsigned i = 1; i < 1024; ++i) {
    BE.launch(fun, args, i);
    EXPECT_EQ(i, *result);
  }

  delete result;
}

TEST(BrigInstTest, Nop) {
  const uint32_t testVec[] = { 0 };
  testInst("nop ; //", testVec);
}

TEST(BrigInstTest, NullPtr) {
  const uint32_t testVec32[] = { 0 };
  testInst("nullptr_u32", testVec32);
}

TEST(BrigInstTest, PackedCmov) {
  {
    const uint32_t testVec[] = { 0x12dc5601U, 0x1000100U, 0x12345678U,
                                 0xfedcba01U };
    testInst("cmov_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x12dc5601, 0x1000100, 0x12345678,
                                 0xfedcba01 };
    testInst("cmov_s8x4", testVec);
  }
}

TEST(BrigInstTest, PackedShr) {
  {
    const uint32_t testVec[] = { 0x3c1e0f07U, 0xf0783c1eU, 2 };
    testInst("shr_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfc1e0f07, 0xf0783c1e, 2 };
    testInst("shr_s8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0x1e0f0783U, 0xf0783c1eU, 3 };
    testInst("shr_u16x2", testVec);
  }
  {
    const uint32_t testVec[] = { 0xfe0f0783, 0xf0783c1e, 3 };
    testInst("shr_s16x2", testVec);
  }
}

TEST(BrigInstTest, PackedShl) {
  {
    const uint32_t testVec[] = { 0xc0e0f078U, 0xf0783c1eU, 2 };
    testInst("shl_u8x4", testVec);
  }
  {
    const uint32_t testVec[] = { 0xc1e0f078U, 0xf0783c1eU, 2 };
    testInst("shl_u16x2", testVec);
  }
}

TEST(BrigInstTest, QId) {
  const uint32_t testVec32[] = { 0 };
  testInst("qid_u32", testVec32);
}

TEST(BrigInstTest, QPtr) {
  const uint32_t testVec32[] = { 0 };
  testInst("qptr_global_u32", testVec32);
}

TEST(BrigInstTest, DispatchId) {
  const uint64_t testVec32[] = { 0 };
  testInst("dispatchid_u64", testVec32);
}

TEST(BrigInstTest, DispatchPtr) {
  const uint32_t testVec32[] = { 0 };
  testInst("dispatchptr_global_u32", testVec32);
}

TEST(DebugTest, Square) {
  const char filename[] = XSTR(OBJ_PATH) "/square.o";
  BrigReader *reader = BrigReader::createBrigReader(filename);
  EXPECT_TRUE(reader);
  if (!reader) return;

  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  EXPECT_TRUE(mod.isValid());
  if (!mod.isValid()) return;

  hsa::brig::BrigProgram BP = hsa::brig::GenLLVM::getLLVMModule(mod);
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("run");
  float *out = new float(0);
  float *in = new float(4);
  void *args[] = { &out, &in };
  BE.launch(fun, args);
  EXPECT_EQ(16.0, *out);
  delete in;
  delete out;
}

TEST(BrigKernelTest, SWA) {

  hsa::brig::BrigProgram BP = TestHSAIL(
    "version 0:96:$full:$large;\n"
    "\n"
    "function &swa (arg_f64 %_result) (arg_u64 %_this,  arg_f64 %_val);\n"
    "kernel &run(){\n"
    " {\n"
    "   arg_f64 %_outval;\n"
    "   arg_u64 %__this;\n"
    "   arg_f64 %_inval;\n"
    "   call &swa (%_outval) (%__this, %_inval);\n"
    " }\n"
    "};\n"
    "\n"
    "function &swa (arg_f64 %_result) (arg_u64 %_this, arg_f64 %_val) {\n"
    "  ret;\n"
    "};");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("run");
  void *args[] = { NULL };
  BE.launch(fun, args);
}

static bool testFPE(const char *str, u32 mask) {

  const char *buffer =
    asnprintf("version 0:96:$full:$large;\n"
              "\n"
              "kernel &fpeTest(kernarg_u64 %resultPtr){\n"
              "   cleardetectexcept_u32 31;\n"
              "   %s\n"
              "   getdetectexcept_u32 $s0;\n"
              "   ld_kernarg_u64 $d0, [%resultPtr];\n"
              "   st_global_u32 $s0, [$d0];\n"
              "};", str);

  hsa::brig::BrigProgram BP = TestHSAIL(buffer);
  EXPECT_TRUE(BP);
  if (!BP) return false;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("fpeTest");

  uint32_t *fpe = new uint32_t(0);
  void *args[] = { &fpe };

  BE.launch(fun, args);

  bool result = !((*fpe ^ mask) & mask);

  delete fpe;
  delete[] buffer;

  return result;
}

TEST(BrigFPE, Invalid) {

#define INF32  "0f7f800000"
#define INF64  "0d7ff0000000000000"
#define NINF32 "0fff800000"
#define NINF64  "0dfff0000000000000"
#define QNAN32 "0f7fc00001"
#define QNAN64 "0d7ff8000000000001"
#define SNAN32 "0f7f800001"
#define SNAN64 "0d7ff0000000000001"

  using hsa::brig::HSA_INVALID;

  // SNAN+0
  EXPECT_TRUE(testFPE("add_f32 $s2, " SNAN32 ", 0;", HSA_INVALID));
  EXPECT_TRUE(testFPE("add_f64 $d2, " SNAN64 ", 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("add_f32 $s2, " QNAN32 ", 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("add_f64 $d2, " QNAN64 ", 0;", HSA_INVALID));

  // 0*inf
  EXPECT_TRUE(testFPE("mul_f32 $s2, 0, " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("mul_f64 $d2, 0, " INF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("mul_f32 $s2, 0, 1;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("mul_f64 $d2, 0, 1;", HSA_INVALID));

  // inf*0
  EXPECT_TRUE(testFPE("mul_f32 $s2, " INF32 ", 0;", HSA_INVALID));
  EXPECT_TRUE(testFPE("mul_f64 $d2, " INF64 ", 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("mul_f32 $s2, 1, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("mul_f64 $d2, 1, 0;", HSA_INVALID));

  // fma(0, inf, 0)
  EXPECT_TRUE(testFPE("fma_f32 $s2, 0, " INF32 ", 0;", HSA_INVALID));
  EXPECT_TRUE(testFPE("fma_f64 $d2, 0, " INF64 ", 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f32 $s2, 0, 1, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f64 $d2, 0, 1, 0;", HSA_INVALID));

  // fma(inf, 0, 0)
  EXPECT_TRUE(testFPE("fma_f32 $s2, " INF32 ", 0, 0;", HSA_INVALID));
  EXPECT_TRUE(testFPE("fma_f64 $d2, " INF64 ", 0, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f32 $s2, 1, 0, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f64 $d2, 1, 0, 0;", HSA_INVALID));

  // INF + -INF
  EXPECT_TRUE(testFPE("add_f32 $s2, " INF32 ", " NINF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("add_f64 $d2, " INF64 ", " NINF64 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("sub_f32 $s2, " INF32 ", " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("sub_f64 $d2, " INF64 ", " INF64 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("fma_f32 $s2, " INF32 ", 1, " NINF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("fma_f64 $d2, " INF64 ", 1, " NINF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("add_f32 $s2, " INF32 ", " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("add_f64 $d2, " INF64 ", " INF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("sub_f32 $s2, " INF32 ", " NINF32 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("sub_f64 $d2, " INF64 ", " NINF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f32 $s2, " INF32 ", 1, " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("fma_f64 $d2, " INF64 ", 1, " INF64 ";", HSA_INVALID));

  // 0/0
  EXPECT_TRUE(testFPE("div_f32 $s2, 0, 0;", HSA_INVALID));
  EXPECT_TRUE(testFPE("div_f64 $d2, 0, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0, 1;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0, 1;", HSA_INVALID));

  // inf/inf
  EXPECT_TRUE(testFPE("div_f32 $s2, " INF32 ", " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("div_f64 $d2, " INF64 ", " INF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("div_f32 $s2, " INF32 ", 1;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("div_f64 $d2, " INF64 ", 1;", HSA_INVALID));

  // sqrt(-1)
  EXPECT_TRUE(testFPE("sqrt_f32 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("sqrt_f64 $d2, -1;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("sqrt_f32 $s2, 1;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("sqrt_f64 $d2, 1;", HSA_INVALID));

  // cvt(QNAN)
  EXPECT_TRUE(testFPE("cvt_u32_f32 $s2, " QNAN32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u64_f64 $d2, " QNAN64 ";", HSA_INVALID));

  // cvt(INF)
  EXPECT_TRUE(testFPE("cvt_u32_f32 $s2, " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u64_f64 $d2, " INF64 ";", HSA_INVALID));

  // cvt(BIG)
  EXPECT_TRUE(testFPE("cvt_u32_f32 $s2, 1E+128;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u64_f64 $d2, 1E+128;", HSA_INVALID));

  // cvt(SMALL)
  EXPECT_TRUE(!testFPE("cvt_u32_f32 $s2, 1E+9;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u64_f64 $d2, 1E+9;", HSA_INVALID));

  // cvt_u8_f32
  EXPECT_TRUE(testFPE("cvt_u8_f32 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u8_f32 $s2,  256;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u8_f32 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u8_f32 $s2, 255;", HSA_INVALID));

  // cvt_s8_f32
  EXPECT_TRUE(testFPE("cvt_s8_f32 $s2,  -129;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s8_f32 $s2,   128;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s8_f32 $s2, -128;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s8_f32 $s2,  127;", HSA_INVALID));

  // cvt_u16_f32
  EXPECT_TRUE(testFPE("cvt_u16_f32 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u16_f32 $s2,  65536;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u16_f32 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u16_f32 $s2, 65535;", HSA_INVALID));

  // cvt_s16_f32
  EXPECT_TRUE(testFPE("cvt_s16_f32 $s2,  -32769;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s16_f32 $s2,   32768;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s16_f32 $s2, -32768;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s16_f32 $s2,  32767;", HSA_INVALID));

  // cvt_u32_f32
  EXPECT_TRUE(testFPE("cvt_u32_f32 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u32_f32 $s2, 4294967296;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u32_f32 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u32_f32 $s2, 0f4f7fffff;", HSA_INVALID));

  // cvt_s32_f32
  EXPECT_TRUE(testFPE("cvt_s32_f32 $s2,   0fcf000001;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s32_f32 $s2,   2147483648;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s32_f32 $s2, -2147483648;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s32_f32 $s2,  0f8effffff;", HSA_INVALID));

  // cvt_u64_f32
  EXPECT_TRUE(testFPE("cvt_u64_f32 $d2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u64_f32 $d2,  0f5f800000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u64_f32 $d2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u64_f32 $d2, 0f5F7FFFFF;", HSA_INVALID));

  // cvt_s64_f32
  EXPECT_TRUE(testFPE("cvt_s64_f32 $d2,  0fdf000001;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s64_f32 $d2,  0f5f000000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s64_f32 $d2, 0fdf000000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s64_f32 $d2, 0f5effffff;", HSA_INVALID));

  // cvt_u8_f64
  EXPECT_TRUE(testFPE("cvt_u8_f64 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u8_f64 $s2,  256;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u8_f64 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u8_f64 $s2, 255;", HSA_INVALID));

  // cvt_s8_f64
  EXPECT_TRUE(testFPE("cvt_s8_f64 $s2,  -129;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s8_f64 $s2,   128;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s8_f64 $s2, -128;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s8_f64 $s2,  127;", HSA_INVALID));

  // cvt_u16_f64
  EXPECT_TRUE(testFPE("cvt_u16_f64 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u16_f64 $s2,  65536;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u16_f64 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u16_f64 $s2, 65535;", HSA_INVALID));

  // cvt_s16_f64
  EXPECT_TRUE(testFPE("cvt_s16_f64 $s2,  -32769;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s16_f64 $s2,   32768;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s16_f64 $s2, -32768;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s16_f64 $s2,  32767;", HSA_INVALID));

  // cvt_u32_f64
  EXPECT_TRUE(testFPE("cvt_u32_f64 $s2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u32_f64 $s2,  4294967296;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u32_f64 $s2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u32_f64 $s2, 4294967295;", HSA_INVALID));

  // cvt_s32_f64
  EXPECT_TRUE(testFPE("cvt_s32_f64 $s2,  -2147483649;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s32_f64 $s2,   2147483648;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s32_f64 $s2, -2147483648;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s32_f64 $s2,  2147483647;", HSA_INVALID));

  // cvt_u64_f64
  EXPECT_TRUE(testFPE("cvt_u64_f64 $d2, -1;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_u64_f64 $d2,  0d43f0000000000000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u64_f64 $d2, 0;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_u64_f64 $d2, 0d43efffffffffffff;", HSA_INVALID));

  // cvt_s64_f64
  EXPECT_TRUE(testFPE("cvt_s64_f64 $d2,  0dc3e0000000000001;", HSA_INVALID));
  EXPECT_TRUE(testFPE("cvt_s64_f64 $d2,  0d43e0000000000000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s64_f64 $d2, 0dc3e0000000000000;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_s64_f64 $d2, 0d43dfffffffffffff;", HSA_INVALID));

  // cvt_sat
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u32_f32 $s2, " QNAN32 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u64_f64 $d2, " QNAN64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u32_f32 $s2, " INF32 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u64_f64 $d2, " INF64 ";", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u32_f32 $s2, 1E+128;", HSA_INVALID));
  EXPECT_TRUE(!testFPE("cvt_downi_sat_u64_f64 $d2, 1E+128;", HSA_INVALID));

#undef INF32
#undef INF64
#undef NINF32
#undef NINF64
#undef QNAN32
#undef QNAN64
#undef SNAN32
#undef SNAN64
}

TEST(BrigFPE, DivisionByZero) {
  using hsa::brig::HSA_DIVBYZERO;
  EXPECT_TRUE(testFPE("div_f32 $s2, 1, 0;", HSA_DIVBYZERO));
  EXPECT_TRUE(testFPE("div_f64 $d2, 1, 0;", HSA_DIVBYZERO));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 1, 1;", HSA_DIVBYZERO));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 1, 1;", HSA_DIVBYZERO));
}

TEST(BrigFPE, UnderFlow) {
  using hsa::brig::HSA_UNDERFLOW;

  // Normal-Denormal Boundary
  EXPECT_TRUE(testFPE("div_f32 $s2, 0f00800001, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(testFPE("div_f64 $d2, 0d0010000000000001, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f00800000, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0010000000000000, 2;", HSA_UNDERFLOW));

  // Denormal
  EXPECT_TRUE(testFPE("div_f32 $s2, 0f00400001, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(testFPE("div_f64 $d2, 0d0008000000000001, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f00400000, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0008000000000000, 2;", HSA_UNDERFLOW));

  // Normal
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f01000001, 2;", HSA_UNDERFLOW));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0020000000000001, 2;", HSA_UNDERFLOW));
}

TEST(BrigFPE, OverFlow) {
  using hsa::brig::HSA_OVERFLOW;

  // Normal-Infinity Boundary
  EXPECT_TRUE(testFPE("mul_f32 $s2, 0f7f7fffff, 2;", HSA_OVERFLOW));
  EXPECT_TRUE(testFPE("mul_f64 $d2, 0d7fefffffffffffff, 2;", HSA_OVERFLOW));

  // Normal
  EXPECT_TRUE(!testFPE("mul_f32 $s2, 0f7effffff, 2;", HSA_OVERFLOW));
  EXPECT_TRUE(!testFPE("mul_f64 $d2, 0d7fdfffffffffffff, 2;", HSA_OVERFLOW));
}

TEST(BrigFPE, Inexact) {
  using hsa::brig::HSA_INEXACT;

  // Normal-Denormal Boundary
  EXPECT_TRUE(testFPE("div_f32 $s2, 0f00800001, 2;", HSA_INEXACT));
  EXPECT_TRUE(testFPE("div_f64 $d2, 0d0010000000000001, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f00800000, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0010000000000000, 2;", HSA_INEXACT));

  // Denormal
  EXPECT_TRUE(testFPE("div_f32 $s2, 0f00400001, 2;", HSA_INEXACT));
  EXPECT_TRUE(testFPE("div_f64 $d2, 0d0008000000000001, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f00400000, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0008000000000000, 2;", HSA_INEXACT));

  // Normal
  EXPECT_TRUE(!testFPE("div_f32 $s2, 0f01000001, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 0d0020000000000001, 2;", HSA_INEXACT));

  // Normal-Infinity Boundary
  EXPECT_TRUE(testFPE("mul_f32 $s2, 0f7f7fffff, 2;", HSA_INEXACT));
  EXPECT_TRUE(testFPE("mul_f64 $d2, 0d7fefffffffffffff, 2;", HSA_INEXACT));

  // Normal
  EXPECT_TRUE(!testFPE("mul_f32 $s2, 0f7effffff, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("mul_f64 $d2, 0d7fdfffffffffffff, 2;", HSA_INEXACT));

  // Rounding
  EXPECT_TRUE(testFPE("div_f32 $s2, 1, 3;", HSA_INEXACT));
  EXPECT_TRUE(testFPE("div_f64 $d2, 1, 3;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f32 $s2, 1, 2;", HSA_INEXACT));
  EXPECT_TRUE(!testFPE("div_f64 $d2, 1, 2;", HSA_INEXACT));
}

TEST(BrigFPE, Set) {
  using hsa::brig::HSA_ALL_EXCEPT;
  for(unsigned i = 0; i <= HSA_ALL_EXCEPT; ++i) {
    const char *buffer = asnprintf("setdetectexcept_u32 %i;", i);
    EXPECT_TRUE(testFPE(buffer, i));
    delete[] buffer;
  }
}

TEST(BrigFPE, Reset) {

  using hsa::brig::HSA_ALL_EXCEPT;

  EXPECT_TRUE(testFPE(
                "cvt_u8_f32 $s2, -1;"         // INVALID
                "div_f32 $s2, 1, 0;"          // DIVBYZERO
                "div_f32 $s2, 0f00800001, 2;" // UNDERFLOW
                "mul_f32 $s2, 0f7f7fffff, 2;" // OVERFLOW
                "div_f32 $s2, 1, 3;"          // INEXACT
                "add_f32 $s2, 0, 0;",         // No Exception
                HSA_ALL_EXCEPT));

  hsa::brig::BrigProgram BP =
    TestHSAIL("version 0:96:$full:$large;\n"
              "\n"
              "kernel &fpeTest(kernarg_u64 %resultPtr){\n"
              "   getdetectexcept_u32 $s0;\n"
              "   ld_kernarg_u64 $d0, [%resultPtr];\n"
              "   st_global_u32 $s0, [$d0];\n"
              "};");
  EXPECT_TRUE(BP);
  if (!BP) return;

  hsa::brig::BrigEngine BE(BP);
  llvm::Function *fun = BP.getFunction("fpeTest");

  uint32_t *fpe = new uint32_t(0);
  void *args[] = { &fpe };

  BE.launch(fun, args);
  EXPECT_EQ(0, *fpe);

  delete fpe;
}

TEST(BrigInstTest, CvtF32ToBool) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
    bool b1;
  } result, input;
   CvtTest("cvt", b1, f32, u8, 0, u32, 0); // +0.0f
   CvtTest("cvt", b1, f32, u8, 0, u32, 0x80000000); // -0.0f
   CvtTest("cvt", b1, f32, u8, 1, f32, NAN);
   CvtTest("cvt", b1, f32, u8, 1, f32, -1);
   CvtTest("cvt", b1, f32, u8, 1, f32, 1);
   CvtTest("cvt", b1, f32, u8, 1, f32, 1); // > +0.0f
   CvtTest("cvt", b1, f32, u8, 1, f32, 0x80000001); // < -0.0f
   CvtTest("cvt", b1, f32, u8, 1, f32, -INFINITY);
   CvtTest("cvt", b1, f32, u8, 1, f32, INFINITY);
}

TEST(BrigInstTest, CvtF64ToBool) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
    bool b1;
  } result, input;
  CvtTest("cvt", b1, f64, u8, 0, u64, 0); // +0.0
  CvtTest("cvt", b1, f64, u8, 0, u64, 0x8000000000000000ULL); // -0.0
  CvtTest("cvt", b1, f64, u8, 1, f64, NAN);
  CvtTest("cvt", b1, f64, u8, 1, f64, -1);
  CvtTest("cvt", b1, f64, u8, 1, f64, 1);
  CvtTest("cvt", b1, f64, u8, 1, u64, 1); // > +0.0
  CvtTest("cvt", b1, f64, u8, 1, u64, 0x8000000000000001ULL); // < -0.0
  CvtTest("cvt", b1, f64, u8, 1, f64, -INFINITY);
  CvtTest("cvt", b1, f64, u8, 1, f64, INFINITY);
}

TEST(BrigInstTest, CvtIntToBool) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
   } result, input;
#define IntToBoolTestValue(X, Y) do {                       \
    result.u8 = Y;                                          \
    input.X = Y;                                            \
    testInstCvt("cvt", "_b1", "_" #X, result.u8, input.X ); \
  } while(0)

#define IntToBoolTest(X) do {                   \
    IntToBoolTestValue(s ## X, 0);              \
    IntToBoolTestValue(s ## X, 1);              \
    IntToBoolTestValue(u ## X, 0);              \
    IntToBoolTestValue(u ## X, 1);              \
  } while(0)

  IntToBoolTest(8);
  IntToBoolTest(16);
  IntToBoolTest(32);
  IntToBoolTest(64);
#undef IntToBoolTest
#undef IntToBoolTestValue
}

TEST(BrigInstTest, BoolToFloat) {
  union {
    uint64_t u64; double f64; int64_t s64;
    uint32_t u32; float f32; int32_t s32;
    uint16_t u16; int16_t s16;
    uint8_t u8; int8_t s8;
   } result, input;
#define BoolToFloat(X, Y) do {                              \
    result.X = Y;                                           \
    input.u8 = Y;                                           \
    testInstCvt("cvt", "_" #X, "_b1", result.X, input.u8 ); \
  } while(0)

  BoolToFloat(f32, 0);
  BoolToFloat(f32, 1);
  BoolToFloat(f64, 0);
  BoolToFloat(f64, 1);
}
