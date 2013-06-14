//===- brigDebug.cc -------------------------------------------------------===//
//
//                     The HSA Simultor
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

#include "llvm/IR/Module.h"

#define STR(X) #X
#define XSTR(X) STR(X)

#include <iostream>

template<class T>
static void printSourceLine(T &out, llvm::DILineInfo &info) {

  const char *filename = info.getFileName();
  uint32_t line = info.getLine();
  uint32_t column = info.getColumn();

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  if (llvm::MemoryBuffer::getFile(filename, file)) {
    out << "<Missing file: " << filename << ">\n";
    return;
  }

  const char *start = file->getBufferStart();
  for (unsigned i = 1; start < file->getBufferEnd() && i < line; ++i) {
    while (start < file->getBufferEnd() && *start != '\n' && *start != '\r')
      ++start;
    ++start;
    if (start < file->getBufferEnd() && start[-1] != *start &&
       ( *start == '\n' || *start == '\r'))
      ++start;
  }

  start += (column - 1);

  const char *end = start;
  while (end < file->getBufferEnd() && *end != ';') ++end;
  while (end < file->getBufferEnd() && *end != '\n' && *end != '\r') ++end;

  std::string text(start, end - start);
  out << text;
}

static void callback(hsa::brig::BrigRegState *regs, size_t pc, void *data) {
  hsa::brig::BrigProgram *BP = (hsa::brig::BrigProgram *) data;
  llvm::DILineInfo info = BP->getLineInfoForAddress(pc);
  std::cout << info.getFunctionName() << " at "
            << info.getFileName() << ":" << info.getLine() << "\t";
  printSourceLine(std::cout, info);
  std::cout << "\n";
}

int main(int argc, char **argv) {

  hsa::brig::BrigReader *reader =
    hsa::brig::BrigReader::createBrigReader(XSTR(BIN_PATH) "/fib.o");
  if (!reader) {
    std::cerr << argv[0] << ": File not found: " << argv[1] << "\n";
    return 0;
  }

  hsa::brig::BrigModule mod(*reader);
  if (!mod.isValid()) {
    std::cerr << argv[0] << ": Input is invalid!\n";
    return 0;
  }

  hsa::brig::BrigProgram BP =
    hsa::brig::GenLLVM::getLLVMModule(mod, callback, &BP);
  if (!BP) {
    std::cerr << argv[0] << ": Translation failure\n";
    return 0;
  }

  hsa::brig::BrigEngine BE(BP);

  const char *funName = "fibKernel";
  llvm::Function *fun = BP->getFunction(funName);
  if (!fun) {
    std::cerr << argv[0] << ": Kernel &" << funName << " missing\n";
    return 0;
  }

  int *r = new int, *n = new int;
  void *args[] = { &r, n };

  std::cout << "Starting kernel: &" << funName << "\n\n";
  *n = 5;
  BE.launch(fun, args);
  std::cout << "\n\nResult = " << *r << "\n\n";

  delete r;
  delete n;

  return 0;
}
