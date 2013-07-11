//===- barrierTest.cc -----------------------------------------------------===//
//
//                     The HSA Simulator
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

#define STR(X) #X
#define XSTR(X) STR(X)

#include <iostream>
#include <string>
#include <sstream>

using namespace std;


int main(int argc, char **argv) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  if (!hsaRT) return -1;

  uint32_t numDevices = hsaRT->getDeviceCount();
  if (!numDevices) return -1;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  llvm::error_code ec =
    llvm::MemoryBuffer::getFile(XSTR(OBJ_PATH) "/barrierTest.o", file);
  if (ec) return -1;

  hsa::Program *program =
    hsaRT->createProgram(const_cast<char *>(file->getBufferStart()),
                         file->getBufferSize(),
                         &devices);
  if (!program) return -1;

  hsa::Kernel *kernel =
    program->compileKernel("&run", "");
  if (!kernel) return -1;

  hsa::Device *device = devices[0];
  hsa::Queue *queue = device->createQueue(1);
  if (!queue) return -1;

  const int NUM = 60;

  int *outArray = (int *) hsaRT->allocateGlobalMemory(NUM * sizeof(int), sizeof(int));
  if (!outArray) return -1;
  int *locArray = (int *) hsaRT->allocateGlobalMemory(NUM * sizeof(int), sizeof(int));
  if (!locArray) return -1;

  hsa::KernelArg argOut;
  argOut.addr = outArray;
  hsa::KernelArg argLoc;
  argLoc.addr = locArray;

  uint32_t groupSizes[] = {1,2,4,5,6,10,12,15,20,30,60};
  int numGroupSizes = sizeof(groupSizes)/sizeof(uint32_t);
  int mismatches = 0;

  for (int i = 0; i < numGroupSizes; ++i) {
    uint32_t groupSize = groupSizes[i];
    hsa::LaunchAttributes la;
    la.grid[0] = NUM / groupSize;
    la.grid[1] = 1;
    la.grid[2] = 1;
    la.group[0] = groupSize;
    la.group[1] = 1;
    la.group[2] = 1;

    for (int k = 0; k < NUM; k++) {
      outArray[k] = -k;
    }

    hsacommon::vector<hsa::Event *> deps;
    queue->dispatch(kernel, la, deps, 2, argOut, argLoc);

    cout  << "Results with " << la.grid[0] << " blocks of size " << la.group[0] << endl;

    for (int k = 0; k < NUM; k++) {
      cout << outArray[k] << " ";
    }
    cout << endl;
    // compare with expected
    for (int k = 0; k < NUM; k++) {
      int sum = 0;
      uint32_t start = k - (k % groupSize);
      for (uint32_t i=start; i<(start + groupSize); i++) {
        sum += i;
      }
      int expected = sum;
      if (outArray[k] != expected) {
        cout << "mismatch at index " << k << ", expected " << expected << " but saw " << outArray[k] << endl;
        mismatches++;
      }
    }

  }
  cout << endl;

  cout << (mismatches ? "FAILED!!!" : "PASSED!!!") << endl;
  hsaRT->freeGlobalMemory(outArray);
  hsaRT->freeGlobalMemory(locArray);

  delete queue;
  delete kernel;
  delete program;
  delete hsaRT;
}
