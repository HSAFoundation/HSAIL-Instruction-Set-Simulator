//===- fcos.cc ------------------------------------------------------------===//
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

// Use LLVM for portable IO
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

#define STR(X) #X
#define XSTR(X) STR(X)

bool *out;
const int32_t NUMELEMENTS = 80;

static void aryset(int r, int c, bool val) {
  out[r * NUMELEMENTS + c] = val;
}

static bool aryget(int r, int c) {
  return out[r * NUMELEMENTS + c];
}

int main(int argc, char **argv) {
  hsa::RuntimeApi *hsaRT = hsa::getRuntime();
  if (!hsaRT) return -1;

  uint32_t numDevices = hsaRT->getDeviceCount();
  if (!numDevices) return -1;

  hsa::vector<hsa::Device *> devices = hsaRT->getDevices();

  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> file =
    llvm::MemoryBuffer::getFile(XSTR(OBJ_PATH) "/atomicexch.o");
  if (file.getError()) return -1;

  hsa::Program *program =
    hsaRT->createProgram(const_cast<char *>((*file)->getBufferStart()),
                         (*file)->getBufferSize(),
                         &devices);
  if (!program) return -1;

  hsa::Kernel *kernel =
    program->compileKernel("&run", "");
  if (!kernel) return -1;

  hsa::Device *device = devices[0];
  hsa::Queue *queue = device->createQueue(1);
  if (!queue) return -1;

  out = (bool *) hsaRT->allocateGlobalMemory(sizeof(bool[NUMELEMENTS * NUMELEMENTS]),
												 sizeof(bool));
  int *atomic = (int *) hsaRT->allocateGlobalMemory(sizeof(int),
                                                   sizeof(int));
  if (!out) return -1;
  if (!atomic) return -1;

  hsa::KernelArg argOut = { out };
  hsa::KernelArg argAtomic = { atomic };
  hsa::KernelArg numElems;
  numElems.s32value = NUMELEMENTS;

  hsa::LaunchAttributes la;
  la.grid[0] = NUMELEMENTS - 1;
  la.grid[1] = 1;
  la.grid[2] = 1;
  la.group[0] = 1;
  la.group[1] = 1;
  la.group[2] = 1;

  // initialize outArray
  for (int r=0; r<NUMELEMENTS; r++) {
	for (int c=0; c<NUMELEMENTS; c++) {
	  aryset(r, c, false);
	}
  }
  // initialize atomic
  *atomic = 0;

  hsacommon::vector<hsa::Event *> deps;
  queue->dispatch(kernel, la, deps, 3, argAtomic, argOut, numElems);

  // make a fake link from the final object to the first object
  aryset(*atomic, 0, true);

  bool passed = true;
  for (int r=0; r<NUMELEMENTS; r++) {
	for (int c=0; c<NUMELEMENTS; c++) {
	  llvm::outs() << (aryget(r, c) ? '*' : '.');
	}
	llvm::outs() << "\n";
  }
  llvm::outs() << "\n";

  // check for 1 entry per row
  for (int r=0; r<NUMELEMENTS; r++) {
	int count = 0;
	for (int c=0; c<NUMELEMENTS; c++) {
	  if (aryget(r, c)) {
		count++;
	  }
	}
	if (count != 1) {
	  llvm::outs() << "count for row " << r << " = " << count << "\n";
	  passed = false;
	}
  }

  // check for 1 entry per col
  for (int c=0; c<NUMELEMENTS; c++) {
	int count = 0;
	for (int r=0; r<NUMELEMENTS; r++) {
	  if (aryget(r, c)) {
		count++;
	  }
	}
	if (count != 1) {
	  llvm::outs() << "count for col " << c << " = " << count << "\n";
	  passed = false;
	}
  }


  llvm::outs() << "\n" << (passed ? "PASSED" : "FAILED") << "\n";

  hsaRT->freeGlobalMemory(out);
  hsaRT->freeGlobalMemory(atomic);

  delete queue;
  delete kernel;
  delete program;
  delete hsaRT;
}
