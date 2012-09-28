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

  char *buffer;

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
