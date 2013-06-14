//===- vectorCopy.cc ------------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Header files the HSA runtime implicitly depends on
#include <cstdio>
#include <cstdlib>
#include <cstring>

// The HSA runtime
#include "hsa.h"

// Use cmath for M_PI
#include <cmath>

// Use LLVM for portable IO
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
using namespace std;

#define STR(X) #X
#define XSTR(X) STR(X)

int main(int argc, char **argv) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  if (!hsaRT) return -1;

  uint32_t numDevices = hsaRT->getDeviceCount();
  if (!numDevices) return -1;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  llvm::error_code ec =
    llvm::MemoryBuffer::getFile(XSTR(BIN_PATH) "/VectorCopy.o", file);
  if (ec) return -1;

  hsa::Program *program =
    hsaRT->createProgram(const_cast<char *>(file->getBufferStart()),
                         file->getBufferSize(),
                         &devices);
  if (!program) return -1;

  hsa::Kernel *kernel =
    program->compileKernel("&__OpenCL_vec_copy_kernel", "");
  if (!kernel) return -1;

  for (unsigned i = 0; i < numDevices; ++i) {
    hsa::Device *device = devices[i];
    hsa::Queue *queue = device->createQueue(1);
    if (!queue) return -1;

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
    if (!a || !b) return -1;

    hsa::LaunchAttributes la;
    la.grid[0] = length;
    la.grid[1] = 1;
    la.grid[2] = 1;
    la.group[0] = 1;
    la.group[1] = 1;
    la.group[2] = 1;

    hsacommon::vector<hsa::Event *> deps;
    queue->dispatch(kernel, la, deps, 3, argA, argB, argLength);

    bool success = true;
    for (int k=0; k<length; k++) {
      if (a[k] != b[k]) {
        cout << "mismatch at index " << k << ", expected " << a[k] << ", saw " << b[k] << endl;
        success = false;
      }
    }
    if (success) cout << "Vector copy success for length " << length << endl;

    hsaRT->freeGlobalMemory(a);
    hsaRT->freeGlobalMemory(b);

    delete queue;
  }

  delete kernel;
  delete program;
  delete hsaRT;
}
