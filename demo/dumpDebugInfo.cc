//===- dumpDebugInfo.cc ---------------------------------------------------===//
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
#include "brig_inst_helper.h"

#include "llvm/IR/Module.h"
#include "llvm/DIBuilder.h"
#include "llvm/DebugInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/FileSystem.h"

using namespace std;
int main(int argc, char **argv) {
  if (argc < 2) {
    llvm::errs() << "Missing input.\n";
    return 0;
  }

  hsa::brig::BrigReader *reader =
    hsa::brig::BrigReader::createBrigReader(argv[1]);
  if (!reader) {
    llvm::errs() << argv[0] << ": File not found: " << argv[1] << "\n";
    return 0;
  }

  hsa::brig::BrigModule mod(*reader);
  if (!mod.isValid()) {
    llvm::errs() << argv[0] << ": Input is invalid!\n";
    return 0;
  }

  hsa::brig::BrigInstHelper helper = mod.getInstHelper();
  for (hsa::brig::debug_iterator it = mod.debug_begin(),
       E = mod.debug_end(); it != E; ++it) {
    if (const BrigBlockNumeric *numeric =
        hsa::brig::dyn_cast<BrigBlockNumeric>(it)) {
      const BrigString *str = helper.getData(numeric);
      llvm::StringRef debugData((const char *) str->bytes, str->byteCount);
      llvm::MemoryBuffer *debugDataBuffer =
        llvm::MemoryBuffer::getMemBuffer(debugData, "", false);
      std::string err;
      std::string input = std::string(argv[1]);
      std::string filename = input.substr(0, input.length()-2);
      filename.append(".debuginfo");

      llvm::raw_fd_ostream fstream(filename.c_str(), err, 2);
      fstream << debugDataBuffer->getBuffer() ;
      return 0;
    }
  }

  return 0;
}
