#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hsa.h"

#include <cmath>

#include "gtest/gtest.h"

TEST(HSARuntimeTest, VectorCopy) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  EXPECT_TRUE(hsaRT);
  if(!hsaRT) return;

  uint32_t numDevices = hsaRT->getDeviceCount();
  EXPECT_LE(1U, numDevices);
  if(!numDevices) return;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();
  EXPECT_EQ(numDevices, devices.size());

  char buffer[] =
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
    "};\n";

  hsa::Program *program =
    hsaRT->createProgram(buffer, sizeof(buffer), &devices);
  EXPECT_TRUE(program);
  if(!program) return;

  hsa::Kernel *kernel =
    program->compileKernel("&__OpenCL_vec_copy_kernel", "");
  EXPECT_TRUE(kernel);
  if(!kernel) return;

  for(unsigned i = 0; i < numDevices; ++i) {
    hsa::Device *device = devices[i];
    hsa::Queue *queue = device->createQueue(1);
    EXPECT_TRUE(queue);
    if(!queue) return;

    const int32_t length = 16;
    float *a = (float *) hsaRT->allocateGlobalMemory(sizeof(float[length]),
                                                     sizeof(float));
    float *b = (float *) hsaRT->allocateGlobalMemory(sizeof(float[length]),
                                                     sizeof(float));

    for(int32_t j = 0; j < length; ++j) {
      a[j] = (float) (M_PI * (j + 1));
      b[j] = 0;
    }

    hsa::KernelArg argA = { a };
    hsa::KernelArg argB = { b };
    hsa::KernelArg argLength;
    argLength.s32value = length;

    EXPECT_TRUE(a);
    EXPECT_TRUE(b);
    if(!a || !b) return;

    hsa::LaunchAttributes la;
    la.grid[0] = 1;
    la.grid[1] = 1;
    la.grid[2] = 1;
    la.group[0] = 1;
    la.group[1] = 1;
    la.group[2] = 1;

    hsacommon::vector<hsa::Event *> deps;
    hsa::DispatchEvent* event =
      queue->dispatch(kernel, la, deps, 3, argA, argB, argLength);

		EXPECT_EQ(a[i], b[i]);

    hsaRT->freeGlobalMemory(a);
    hsaRT->freeGlobalMemory(b);
  }
}
