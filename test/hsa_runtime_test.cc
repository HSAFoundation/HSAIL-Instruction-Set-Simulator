//===- hsa_runtime_test.cc ------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hsa.h"

#include <cmath>

#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"

#include "gtest/gtest.h"

#define STR(X) #X
#define XSTR(X) STR(X)

TEST(HSARuntimeTest, VectorCopy) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  EXPECT_TRUE(hsaRT);
  if (!hsaRT) return;

  uint32_t numDevices = hsaRT->getDeviceCount();
  EXPECT_LE(1U, numDevices);
  if (!numDevices) return;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();
  EXPECT_EQ(numDevices, devices.size());

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  llvm::error_code ec =
    llvm::MemoryBuffer::getFile(XSTR(OBJ_PATH) "/VectorCopy.o", file);
  EXPECT_TRUE(!ec);
  if (ec) return;

  hsa::Program *program =
    hsaRT->createProgram(const_cast<char *>(file->getBufferStart()),
                         file->getBufferSize(),
                         &devices);
  EXPECT_TRUE(program);
  if (!program) return;

  hsa::Kernel *kernel =
    program->compileKernel("&__OpenCL_vec_copy_kernel", "");
  EXPECT_TRUE(kernel);
  if (!kernel) return;

  for (unsigned i = 0; i < numDevices; ++i) {
    hsa::Device *device = devices[i];
    hsa::Queue *queue = device->createQueue(1);
    EXPECT_TRUE(queue);
    if (!queue) return;

    const int32_t length = 16;
    float *a = (float *) hsaRT->allocateGlobalMemory(sizeof(float[length]),
                                                     sizeof(float));
    float *b = (float *) hsaRT->allocateGlobalMemory(sizeof(float[length]),
                                                     sizeof(float));

    for (int32_t j = 0; j < length; ++j) {
      a[j] = (float) (M_PI * (j + 1));
      b[j] = 0;
    }

    hsa::KernelArg argA = { a };
    hsa::KernelArg argB = { b };
    hsa::KernelArg argLength;
    argLength.s32value = length;

    EXPECT_TRUE(a);
    EXPECT_TRUE(b);
    if (!a || !b) return;

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
    (void) event;

		EXPECT_EQ(a[i], b[i]);

    hsaRT->freeGlobalMemory(a);
    hsaRT->freeGlobalMemory(b);
  }
}
