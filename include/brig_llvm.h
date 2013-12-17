//===- brig_llvm.h --------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_BRIG_LLVM_H_
#define INCLUDE_BRIG_LLVM_H_

#include "brig.h"
#include "bugs.h"

#include "llvm/DebugInfo/DIContext.h"
#include "llvm/IR/Function.h"

#include <tr1/memory>

#include <map>
#include <string>

namespace llvm {
class DIContext;
class Function;
class LLVMContext;
class Module;
class StringRef;
class StructType;
class Type;
}

namespace hsa {
namespace brig {

class BrigModule;
class BrigSymbol;

struct BrigRegState;

typedef intptr_t FunId;
typedef size_t SymbolId;

struct HSADebugger {

  virtual void updatePC(size_t pc) {}
  virtual void enterFn(BrigRegState *state, FunId id) {}
  virtual void leaveFn() {}
  virtual void declareLocal(void *addr, SymbolId id) {}
  virtual void declareGlobal(void *addr, SymbolId id) {}
  virtual ~HSADebugger() {}

  static void updatePC(HSADebugger *dbg, size_t pc) {
    dbg->updatePC(pc);
  }

  static void enterFn(HSADebugger *dbg, BrigRegState *state, FunId id) {
    dbg->enterFn(state, id);
  }

  static void leaveFn(HSADebugger *dbg) {
    dbg->leaveFn();
  }

  static void declareLocal(HSADebugger *dbg, void *addr, SymbolId id) {
    dbg->declareLocal(addr, id);
  }

  static void declareGlobal(HSADebugger *dbg, void *addr, SymbolId id) {
    dbg->declareGlobal(addr, id);
  }
};

struct SymbolInfo {
  std::string name;
  uint64_t dim;
  BrigType type;
  BrigSegment seg;
  bool isGlobal;

  SymbolInfo() {}
  SymbolInfo(std::string name, const BrigSymbol &S, bool isGlobal);
};

typedef std::map<SymbolId, SymbolInfo> SymbolInfoMap;
typedef std::map<FunId, llvm::Function *> FunMap;

struct BrigProgram {
  BrigProgram(llvm::Module *M) : M(M, delModule) {}
  BrigProgram(llvm::Module *M, llvm::DIContext *debugInfo,
              FunMap &funMap, SymbolInfoMap &symbolInfoMap) :
    M(M, delModule), debugInfo(debugInfo),
    funMap(funMap), symbolInfoMap(symbolInfoMap) {}

  operator bool () { return M; }
  operator llvm::Module *() { return M.get(); }
  bool operator!() { return !M; }

  llvm::Function *getFunction(llvm::StringRef name) const;

  llvm::DILineInfo getLineInfoForAddress(uint64_t pc) const {
    llvm::DILineInfoSpecifier spec(
      llvm::DILineInfoSpecifier::FunctionName |
      llvm::DILineInfoSpecifier::FileLineInfo |
      llvm::DILineInfoSpecifier::AbsoluteFilePath);
    return debugInfo->getLineInfoForAddress(pc, spec);
  }

  std::string getFunctionName(FunId id) const {
    return funMap.find(id)->second->getName();
  }

  std::string getSymbolName(SymbolId id) const {
    return symbolInfoMap.find(id)->second.name;
  }

  uint64_t getSymbolArrayDim(SymbolId id) const {
    return symbolInfoMap.find(id)->second.dim;
  }

  BrigType getSymbolType(SymbolId id) const {
    return symbolInfoMap.find(id)->second.type;
  }

  BrigSegment getSymbolSegment(SymbolId id) const {
    return symbolInfoMap.find(id)->second.seg;
  }

 private:
  static void delModule(llvm::Module *M);

  const std::tr1::shared_ptr<llvm::Module> M;
  const std::tr1::shared_ptr<llvm::DIContext> debugInfo;
  const FunMap funMap;
  const SymbolInfoMap symbolInfoMap;
};

class GenLLVM {
 public:
  static BrigProgram getLLVMModule(const BrigModule &M,
                                   HSADebugger *dbg = NULL);
  static std::string getLLVMString(const BrigModule &M,
                                   HSADebugger *dbg = NULL);
};

} // namespace brig
} // namespace hsa

#endif  // INCLUDE_BRIG_LLVM_H_
