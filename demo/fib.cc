//===- fib.cc -------------------------------------------------------------===//
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

int main(int argc, char **argv) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  if (!hsaRT) return -1;

  uint32_t numDevices = hsaRT->getDeviceCount();
  if (!numDevices) return -1;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  llvm::error_code ec =
    llvm::MemoryBuffer::getFile(XSTR(OBJ_PATH) "/fib.o", file);
  if (ec) return -1;

  hsa::Program *program =
    hsaRT->createProgram(const_cast<char *>(file->getBufferStart()),
                         file->getBufferSize(),
                         &devices);
  if (!program) return -1;

  hsa::Kernel *kernel =
    program->compileKernel("&fibKernel", "");
  if (!kernel) return -1;

  hsa::Device *device = devices[0];
  hsa::Queue *queue = device->createQueue(1);
  if (!queue) return -1;

  llvm::outs() << "Fib sequence: ";

  int *r = (int *) hsaRT->allocateGlobalMemory(sizeof(int),
                                               sizeof(int));
  if (!r) return -1;

  hsa::KernelArg argR = { r };

  hsa::LaunchAttributes la;
  la.grid[0] = 1;
  la.grid[1] = 1;
  la.grid[2] = 1;
  la.group[0] = 1;
  la.group[1] = 1;
  la.group[2] = 1;

  for (int i = 1; i < 20; ++i) {
    hsa::KernelArg argN;
    argN.s32value = i;
    hsacommon::vector<hsa::Event *> deps;
    queue->dispatch(kernel, la, deps, 2, argR, argN);
    llvm::outs() << *r << " ";
  }
  llvm::outs() << "\n";

  hsaRT->freeGlobalMemory(r);

  delete queue;
  delete kernel;
  delete program;
  delete hsaRT;
}
