//===- brigDebug.cc -------------------------------------------------------===//
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

template<typename T, size_t N>
static int getArraySize(T (&array)[N]) { return N; }

class DemoDebugger : public hsa::brig::HSADebugger {

 public:
  virtual void updatePC(size_t pc) {
    stack.back().showRegChanges();
    llvm::DILineInfo info = BP->getLineInfoForAddress(pc);
    std::cout << info.getFunctionName() << " at "
              << info.getFileName() << ":" << info.getLine() << "\t";
    printSourceLine(std::cout, info);
    std::cout << "\n";
  }

  virtual void enterFn(hsa::brig::BrigRegState *regs, hsa::brig::FunId id) {
    stack.push_back(StackFrame(id, regs));
    std::cout << "Entering: " << BP->getFunctionName(id) << "\n";
  }

  virtual void leaveFn() {
    std::cout << "Leaving: " << BP->getFunctionName(stack.back().id) << "\n";
    stack.pop_back();
  }

  virtual void declareLocal(void *addr, hsa::brig::SymbolId id) {
    stack.back().locals.push_back(Symbol(id, addr));
    std::cout << BP->getSymbolName(id) << " @ " << addr << "\n";
  }

  virtual void declareGlobal(void *addr, hsa::brig::SymbolId id) {
    globals.push_back(Symbol(id, addr));
    std::cout << BP->getSymbolName(id) << " @ " << addr << "\n";
  }

  void setBrigProgram(hsa::brig::BrigProgram *BP) {
    this->BP = BP;
  }

 private:
  typedef std::pair<hsa::brig::SymbolId, void *> Symbol;
  typedef std::vector<Symbol> SymbolList;

  SymbolList globals;
  hsa::brig::BrigProgram *BP;

  struct StackFrame {
    hsa::brig::BrigRegState prevRegState;
    hsa::brig::BrigRegState *regs;
    hsa::brig::FunId id;
    SymbolList locals;

    StackFrame(hsa::brig::FunId id, hsa::brig::BrigRegState *regs) :
      prevRegState(*regs), regs(regs), id(id) {}

    void showRegChanges() {
      for (int i = 0; i < getArraySize(regs->c); ++i) {
        if (regs->c[i] != prevRegState.c[i]) {
          std::cout << "$c" << i << "=" << regs->c[i] << "\n";
        }
      }
      for (int i = 0; i < getArraySize(regs->s); ++i) {
        if (regs->s[i] != prevRegState.s[i]) {
          std::cout << "$s" << i << "=" << regs->s[i]
                    << ",  FP: " << *(float *)(regs->s + i) << "\n";
        }
      }
      for (int i = 0; i < getArraySize(regs->d); ++i) {
        if (regs->d[i] != prevRegState.d[i]) {
          std::cout << "$d" << i << "=" << regs->d[i]
                    << ",  FP: " << *(double *)(regs->d + i) << "\n";
        }
      }
      std::cout << "\n";
      prevRegState = *regs;
    }
  };

  std::vector<StackFrame> stack;
};

int main(int argc, char **argv) {

  hsa::brig::BrigReader *reader =
    hsa::brig::BrigReader::createBrigReader(XSTR(OBJ_PATH) "/fib.o");
  if (!reader) {
    std::cerr << argv[0] << ": File not found: " << argv[1] << "\n";
    return 0;
  }

  hsa::brig::BrigModule mod(*reader);
  if (!mod.isValid()) {
    std::cerr << argv[0] << ": Input is invalid!\n";
    return 0;
  }


  DemoDebugger dbg;
  hsa::brig::BrigProgram BP =
    hsa::brig::GenLLVM::getLLVMModule(mod, &dbg);
  if (!BP) {
    std::cerr << argv[0] << ": Translation failure\n";
    return 0;
  }
  dbg.setBrigProgram(&BP);

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
