//===- brig2llvm.cc -------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_llvm.h"

#include "brig.h"
#include "brig_control_block.h"
#include "brig_function.h"
#include "brig_inst_helper.h"
#include "brig_module.h"
#include "brig_scope.h"
#include "brig_symbol.h"

#include "llvm/DIBuilder.h"
#include "llvm/DebugInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdio>

#include <fcntl.h>
#include <unistd.h>

namespace hsa {
namespace brig {

static bool isI386(void) {
#ifdef __i386__
  return true;
#else
  return false;
#endif
}

static bool isARM(void) {
#ifdef __arm__
  return true;
#else
  return false;
#endif
}

SymbolInfo::SymbolInfo(std::string name, const BrigSymbol &S, bool isGlobal) :
    name(name),
    dim(S.getArrayDim()),
    type(S.getType()),
    seg(S.getStorageClass()),
    isGlobal(isGlobal) {}

llvm::Function *BrigProgram::getFunction(llvm::StringRef name) const {
  llvm::Twine mangledName = "kernel." + name;
  return M->getFunction(mangledName.str());
}

unsigned BrigProgram::getNumParams(llvm::StringRef name) const {
  return M->getFunction(name)->arg_size();
}

void BrigProgram::delModule(llvm::Module *M) {
  if (!M) return;
  llvm::LLVMContext *C = &M->getContext();
  delete M;
  delete C;
}

static llvm::StructType *createSOAType(llvm::LLVMContext &C,
                                       llvm::Type *t,
                                       std::string name,
                                       int nr) {
  // [nr x t]
  llvm::ArrayType *tx8 = llvm::ArrayType::get(t, nr);
  std::vector<llvm::Type *> tv(1, tx8);
  // name = {[nr x t]}
  llvm::StructType *soa_type = llvm::StructType::create(C, tv, name, false);
  return soa_type;
}

static void insertGPUStateTy(llvm::LLVMContext &C) {
  llvm::StructType *c_reg_type =
    createSOAType(C, llvm::Type::getInt1Ty(C), "c_regs", 8);
  llvm::StructType *s_reg_type =
    createSOAType(C, llvm::Type::getInt32Ty(C), "s_regs", 128);
  llvm::StructType *d_reg_type =
    createSOAType(C, llvm::Type::getInt64Ty(C), "d_regs", 64);
  llvm::StructType *q_reg_type =
    createSOAType(C, llvm::Type::getIntNTy(C, 128), "q_regs", 32);
  llvm::Type *tv1[] = { c_reg_type,
                        s_reg_type,
                        d_reg_type,
                        q_reg_type };
  llvm::StructType::create(C, tv1, std::string("struct.regs"), false);
}

// Create a debug entry corresponding to a particular register
static void insertGPURegDebugInfo(llvm::BasicBlock &entry,
                                  llvm::DIBuilder &DB,
                                  llvm::Value *regs,
                                  llvm::DIType type,
                                  unsigned offset,
                                  char prefix,
                                  unsigned num) {

  char regName[16];
  snprintf(regName, sizeof(regName), "hsa$%c%u", prefix, num);

  llvm::DIFile file = DB.createFile("-", "");

  llvm::Type *int64Ty = llvm::Type::getInt64Ty(entry.getContext());
  llvm::Value *addr[] = {
    llvm::ConstantInt::get(int64Ty, llvm::DIBuilder::OpPlus),
    llvm::ConstantInt::get(int64Ty, offset)
  };

  llvm::DIVariable reg =
    DB.createComplexVariable(llvm::dwarf::DW_TAG_auto_variable,
                             file, regName, file, 0,
                             type, addr);

  DB.insertDeclare(regs, reg, &entry);
}

// Generate an llvm::DIType corresponding to a particular BrigType.
static llvm::DIType runOnTypeDebug(llvm::DIBuilder &DB, BrigType type) {

  char prefix =
    BrigInstHelper::isBitTy(type)      ? 'b' :
    BrigInstHelper::isFloatTy(type)    ? 'f' :
    BrigInstHelper::isUnsignedTy(type) ? 'u' :
    BrigInstHelper::isSignedTy(type)   ? 's' :
    'v';

  unsigned size = (unsigned) BrigInstHelper::getTypeSize(type);

  char typeName[16];
  snprintf(typeName, sizeof(typeName), "%c%u", prefix, size);

  unsigned encoding =
    BrigInstHelper::isBitTy(type)      ?
    size == 1 ? llvm::dwarf::DW_ATE_boolean : llvm::dwarf::DW_ATE_unsigned :
    BrigInstHelper::isFloatTy(type)    ? llvm::dwarf::DW_ATE_float :
    BrigInstHelper::isUnsignedTy(type) ? llvm::dwarf::DW_ATE_unsigned :
    BrigInstHelper::isSignedTy(type)   ? llvm::dwarf::DW_ATE_signed :
    llvm::dwarf::DW_ATE_lo_user;

  unsigned storageSize = std::max(size, 8U);
  return DB.createBasicType(typeName, storageSize, storageSize, encoding);
}

// Create debug entries corresponding to all HSA registers
static void insertGPUStateDebugInfo(llvm::BasicBlock &entry,
                                    llvm::DIBuilder &DB,
                                    llvm::Value *regs) {
  llvm::DIType b1Ty   = runOnTypeDebug(DB, BRIG_TYPE_B1);
  llvm::DIType b32Ty  = runOnTypeDebug(DB, BRIG_TYPE_B32);
  llvm::DIType f32Ty  = runOnTypeDebug(DB, BRIG_TYPE_F32);
  llvm::DIType b64Ty  = runOnTypeDebug(DB, BRIG_TYPE_B64);
  llvm::DIType f64Ty  = runOnTypeDebug(DB, BRIG_TYPE_F64);
  llvm::DIType b128Ty = runOnTypeDebug(DB, BRIG_TYPE_B128);

  llvm::DIFile file = DB.createFile("-", "");
  llvm::Value *sFields[] = {
    DB.createMemberType(file, "b32", file, 0, 32, 32, 0, 0, b32Ty),
    DB.createMemberType(file, "f32", file, 0, 32, 32, 0, 0, f32Ty)
  };
  llvm::Value *dFields[] = {
    DB.createMemberType(file, "b64", file, 0, 64, 32, 0, 0, b64Ty),
    DB.createMemberType(file, "f64", file, 0, 64, 32, 0, 0, f64Ty)
  };

  llvm::DIArray sRegArray = DB.getOrCreateArray(sFields);
  llvm::DIArray dRegArray = DB.getOrCreateArray(dFields);

  llvm::DIType sRegTy =
    DB.createUnionType(file, "sReg", file, 0, 32, 32, 0, sRegArray);
  llvm::DIType dRegTy =
    DB.createUnionType(file, "dReg", file, 0, 64, 32, 0, dRegArray);

  unsigned offset = 0;
  for (unsigned i = 0; i < 8; ++i) {
    insertGPURegDebugInfo(entry, DB, regs, b1Ty, offset, 'c', i);
    offset += 1;
  }

  for (unsigned i = 0; i < 128; ++i) {
    insertGPURegDebugInfo(entry, DB, regs, sRegTy, offset, 's', i);
    offset += 4;
  }

  for (unsigned i = 0; i < 64; ++i) {
    insertGPURegDebugInfo(entry, DB, regs, dRegTy, offset, 'd', i);
    offset += 8;
  }

  for (unsigned i = 0; i < 32; ++i) {
    insertGPURegDebugInfo(entry, DB, regs, b128Ty, offset, 'q', i);
    offset += 16;
  }
}

static void insertSetThreadInfo(llvm::LLVMContext &C, llvm::Module *M) {
  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  llvm::Type *args[] = { llvm::Type::getInt8PtrTy(C) };
  llvm::FunctionType *setThreadInfoTy =
    llvm::FunctionType::get(voidTy, args, false);
  M->getOrInsertFunction("__setThreadInfo", setThreadInfoTy);
}

static llvm::StringRef getStringRef(const BrigString *str) {
  llvm::StringRef ref((char *) str->bytes + 1, str->byteCount - 1);
  return ref;
}

static llvm::Type *runOnType(llvm::LLVMContext &C, BrigType type) {

  switch (type) {
    case BRIG_TYPE_NONE:
      return llvm::Type::getInt32Ty(C);
    case BRIG_TYPE_B1:
      return llvm::Type::getInt1Ty(C);
    case BRIG_TYPE_S8: case BRIG_TYPE_U8: case BRIG_TYPE_B8:
      return llvm::Type::getInt8Ty(C);
    case BRIG_TYPE_S16: case BRIG_TYPE_U16: case BRIG_TYPE_B16:
      return llvm::Type::getInt16Ty(C);
    case BRIG_TYPE_S32: case BRIG_TYPE_U32: case BRIG_TYPE_B32:
      return llvm::Type::getInt32Ty(C);
    case BRIG_TYPE_S64: case BRIG_TYPE_U64: case BRIG_TYPE_B64:
    case BRIG_TYPE_ROIMG: case BRIG_TYPE_RWIMG: case BRIG_TYPE_SAMP:
      return llvm::Type::getInt64Ty(C);
    case BRIG_TYPE_F16:
      return llvm::Type::getHalfTy(C);
    case BRIG_TYPE_F32:
      return llvm::Type::getFloatTy(C);
    case BRIG_TYPE_F64:
      return llvm::Type::getDoubleTy(C);
    case BRIG_TYPE_B128:
      return llvm::Type::getIntNTy(C, 128);
    default:
      if (BrigInstHelper::isVectorTy(type)) {

        llvm::Type *base = runOnType(C, BrigInstHelper::getElementTy(type));
        unsigned length = BrigInstHelper::getVectorLength(type);

        if (base->isIntegerTy() || isI386()) {
          unsigned bitWidth = base->getScalarSizeInBits() * length;
          return llvm::Type::getIntNTy(C, bitWidth);
        }

        return llvm::VectorType::get(base, length);
      } else {
        assert(false && "Unimplemented type");
      }
  }
}

static llvm::Type *runOnType(llvm::LLVMContext &C,
                             const BrigSymbol &S) {
  BrigType brigType = S.getType();
  llvm::Type *llvmType = runOnType(C, brigType);
  if (!S.isArray()) {
    return llvmType;
  } else if (S.isFlexArray()) {
    return llvmType->getPointerTo(0);
  } else {
    return llvm::ArrayType::get(llvmType, S.getArrayDim());
  }
}

static llvm::GlobalValue::LinkageTypes runOnLinkage(BrigLinkage8_t link) {
  if (link == BRIG_LINKAGE_EXTERN || link == BRIG_LINKAGE_NONE) {
    return llvm::GlobalValue::ExternalLinkage;
  } else if (link == BRIG_LINKAGE_STATIC) {
    return llvm::GlobalValue::InternalLinkage;
  }

  assert(false && "Invalid attribute");
}

static unsigned getRegField(const BrigString *name) {
  if (name->bytes[1] == 'c') return 0;
  if (name->bytes[1] == 's') return 1;
  if (name->bytes[1] == 'd') return 2;
  if (name->bytes[1] == 'q') return 3;

  assert(false && "Unknown reg type");
}

static unsigned getRegOffset(const BrigString *name) {
  const uint8_t *endptr;
  unsigned offset = (unsigned) nstrtol(name, &endptr, 10);
  assert(endptr == name->bytes + name->byteCount && "Invalid field");
  assert(offset < 128 && "Invalid offset");
  return offset;
}

struct FunScope;
static void insertEnterFn(llvm::BasicBlock &, const FunScope &);
static void insertDeclareVariable(llvm::BasicBlock &,
                                  const SymbolId,
                                  const bool,
                                  const FunScope &);

typedef std::map<SymbolId, llvm::Value *> SymbolMap;

struct ModScope {
  FunMap &funMap;
  SymbolMap &symbolMap;
  SymbolInfoMap &symbolInfoMap;
  llvm::DIContext *debugInfo;
  const HSADebugger *dbg;
  llvm::DIBuilder &DB;

  ModScope(FunMap &funMap,
           SymbolMap &symbolMap,
           SymbolInfoMap &symbolInfoMap,
           llvm::DIContext *debugInfo,
           const HSADebugger *dbg,
           llvm::DIBuilder &DB) :
    funMap(funMap), symbolMap(symbolMap),
    symbolInfoMap(symbolInfoMap),
    debugInfo(debugInfo), dbg(dbg),
    DB(DB) {}
};

struct FunScope {
  typedef std::map<uint32_t, llvm::BasicBlock *> CBMap;
  typedef CBMap::const_iterator CBIt;

 private:
  const ModScope &parent;
  llvm::DISubprogram sub;
  const size_t id;

  struct BlockScope {
    llvm::DIScope scope;
    const uint32_t start;
    const uint32_t end;
    BlockScope(llvm::DIScope scope, uint32_t start, uint32_t end) :
      scope(scope), start(start), end(end) {}
  };

  typedef std::map<uint32_t, BlockScope> ScopeMap;
  ScopeMap instScopeMap;
  ScopeMap dirScopeMap;

 public:
  CBMap cbMap;
  llvm::Value *regs;

  FunScope(ModScope &parent,
           const BrigFunction &brigFun,
           llvm::Function *llvmFun) :
    parent(parent), id(brigFun.getOffset()) {

    // Create all basic blocks
    llvm::LLVMContext &C = llvmFun->getContext();
    const BrigControlBlock E = brigFun.end();
    for (BrigControlBlock cb = brigFun.begin(); cb != E; ++cb) {

      const char *name = cb.getName();
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(C, name, llvmFun);

      typedef std::pair<uint32_t, llvm::BasicBlock *> Pair;
      bool unique = cbMap.insert(Pair(cb.getOffset(), bb)).second;
      assert(unique && "Labels should be unique in functions");
    }

    llvm::BasicBlock &entry = llvmFun->getEntryBlock();
    if (hasDebugInfo()) {

      // Add DWARF function debug information
      llvm::DILineInfo info = getLineInfo(brigFun.getCCode());
      llvm::DIFile file = getDIFile(info);

      std::vector<llvm::Value *> args;
      args.push_back(NULL);
      for (BrigSymbol brigArg = brigFun.arg_begin(),
             E = brigFun.arg_end(); brigArg != E; ++brigArg) {
        args.push_back(runOnTypeDebug(parent.DB, brigArg.getType()));
      }

      llvm::DICompositeType debugFunTy =
        parent.DB.createSubroutineType(file, parent.DB.getOrCreateArray(args));
      sub = parent.DB.createFunction(file,
                                     llvmFun->getName(),
                                     llvmFun->getName(),
                                     file,
                                     info.getLine()-1,
                                     debugFunTy,
                                     false,
                                     true,
                                     info.getLine(),
                                     llvm::DIDescriptor::FlagPrototyped,
                                     false,
                                     llvmFun);

      // Add DWARF argument debug information
      unsigned argNo = 0;
      for (BrigSymbol brigArg = brigFun.arg_begin(),
             E = brigFun.arg_end(); brigArg != E; ++brigArg) {
        llvm::Value *arg = parent.symbolMap[brigArg.getAddr()];
        llvm::Value *stackArg =
          new llvm::AllocaInst(arg->getType(), "", &entry);
        new llvm::StoreInst(arg, stackArg, &entry);
        insertDebugDeclareLocal(entry, brigArg,
                                llvm::dwarf::DW_TAG_arg_variable,
                                sub, stackArg, ++argNo);
      }

      // Build tables mapping from code and directive offsets to debugging
      // scopes
      for (BrigScope scope = brigFun.scope_begin(), E = brigFun.scope_end();
          scope != E; ++scope) {

        uint32_t codeStart = scope.getCodeScopeStart();
        uint32_t codeEnd = scope.getCodeScopeEnd();
        assert(codeStart < codeEnd && "Invalid scope");

        llvm::DILineInfo info = getLineInfo(codeStart);
        uint32_t line = info.getLine();
        uint32_t column = info.getColumn();
        llvm::DIScope LB =
          parent.DB.createLexicalBlock(sub, file, line, column);

        BlockScope instBS(LB, codeStart, codeEnd);
        instScopeMap.insert(std::make_pair(codeStart, instBS));

        uint32_t dirStart = scope.getDirScopeStart();
        uint32_t dirEnd = scope.getDirScopeEnd();
        assert(dirStart < dirEnd && "Invalid scope");

        BlockScope dirBS(LB, dirStart, dirEnd);
        dirScopeMap.insert(std::make_pair(dirStart, dirBS));
      }
    }

    // Create registers
    llvm::Module *M = llvmFun->getParent();
    llvm::Type *regsType = M->getTypeByName("struct.regs");
    regs = new llvm::AllocaInst(regsType, "gpu_reg_p", &entry);

    // Use memset to remove a source of non-deterministic behavior. The HSA PRM
    // does not require that registers are initialized to zero.
    llvm::IRBuilder<> builder(&entry);
    llvm::DataLayout DL(M);
    llvm::Value *zero = llvm::ConstantInt::get(C, llvm::APInt(8, 0));
    builder.CreateMemSet(regs, zero,
                         DL.getTypeAllocSize(regsType),
                         DL.getPrefTypeAlignment(regsType));

    // Add callback function debug information
    insertEnterFn(entry, *this);

    // Add callback argument debug information
    for (BrigSymbol brigArg = brigFun.arg_begin(),
           E = brigFun.arg_end(); brigArg != E; ++brigArg) {
      insertDeclareVariable(entry, brigArg.getAddr(), false, *this);
    }

    // Add callback local debug information
    for (BrigSymbol local = brigFun.local_begin(),
           E = brigFun.local_end(); local != E; ++local) {
      llvm::StringRef name = getStringRef(local.getName());
      llvm::Type *type = runOnType(C, local);
      llvm::Value *v = new llvm::AllocaInst(type, name, &entry);
      parent.symbolMap[local.getAddr()] = v;
      parent.symbolInfoMap[local.getAddr()] =
        SymbolInfo(name.str(), local, false);
      insertDeclareVariable(entry, local.getAddr(), false, *this);
    }

    if (hasDebugInfo()) {

      // Add DWARF register debug information
      insertGPUStateDebugInfo(entry, parent.DB, regs);

      // Add DWARF local variable debug information
      for (BrigSymbol local = brigFun.local_begin(),
             E = brigFun.local_end(); local != E; ++local) {
        insertDebugDeclareLocal(entry, local, llvm::dwarf::DW_TAG_auto_variable,
                                sub);
      }
    }
  }

  llvm::Value *lookupSymbol(SymbolId symbol) const {
    SymbolMap::const_iterator it = parent.symbolMap.find(symbol);
    return it != parent.symbolMap.end() ? it->second : NULL;
  }

  llvm::Value *lookupSymbol(const BrigSymbol &symbol) const {
    SymbolMap::const_iterator it = parent.symbolMap.find(symbol.getAddr());
    return it != parent.symbolMap.end() ? it->second : NULL;
  }

  SymbolInfoMap::const_iterator symbol_begin() const {
    return parent.symbolInfoMap.begin();
  }

  SymbolInfoMap::const_iterator symbol_end() const {
    return parent.symbolInfoMap.end();
  }

  llvm::Function *lookupFun(uint32_t addr) const {
    return parent.funMap.find(addr)->second;
  }

  const HSADebugger *getHSADebugger() const { return parent.dbg; }
  bool hasDebugInfo() const { return parent.debugInfo; }

  llvm::DILineInfo getLineInfo(size_t addr) const {
    llvm::DILineInfoSpecifier spec(
      llvm::DILineInfoSpecifier::FunctionName |
      llvm::DILineInfoSpecifier::FileLineInfo |
      llvm::DILineInfoSpecifier::AbsoluteFilePath);
    return parent.debugInfo->getLineInfoForAddress(addr, spec);
  }

  llvm::DIFile getDIFile(llvm::DILineInfo info) const {
    llvm::StringRef srcDir = llvm::sys::path::parent_path(info.getFileName());
    llvm::StringRef srcFile = llvm::sys::path::filename(info.getFileName());
    return parent.DB.createFile(srcFile, srcDir);
  }

  // Find the proper scope, if any, of a symbol or instruction based on its
  // directive or code offset respectively.
  const llvm::DIScope *getDebugScope(size_t addr, const ScopeMap &map) const {
    llvm::DILineInfo info = getLineInfo(addr);
    ScopeMap::const_iterator it = map.upper_bound(addr);

    if (it == map.begin()) return NULL;
    --it;
    if (addr >= it->second.end) return NULL;

    assert(addr >= it->second.start && "Wrong scope?");
    return &it->second.scope;
  }

  // Generate a DebugLoc for an instruction based on its code offset
  llvm::DebugLoc getDebugLoc(size_t addr) const {
    llvm::DILineInfo info = getLineInfo(addr);
    uint32_t line = info.getLine();
    uint32_t column = info.getColumn();

    if (const llvm::DIScope *scope = getDebugScope(addr, instScopeMap))
      return llvm::DebugLoc::get(line, column, *scope);

    llvm::DILexicalBlockFile LB =
      parent.DB.createLexicalBlockFile(sub, getDIFile(info));
    return llvm::DebugLoc::get(line, column, LB);
  }

  // Generate a DebugLoc for a symbol based on its directive offset
  llvm::DebugLoc getDebugLoc(BrigSymbol &local) const {
    llvm::DILineInfo info = getLineInfo(local.getCCode());
    uint32_t line = info.getLine();
    uint32_t column = info.getColumn();

    const llvm::DIScope *scope = getDebugScope(local.getAddr(), dirScopeMap);
    if (scope)
      return llvm::DebugLoc::get(line, column, *scope);

    llvm::DILexicalBlockFile LB =
      parent.DB.createLexicalBlockFile(sub, getDIFile(info));
    return llvm::DebugLoc::get(line, column, LB);
  }

  // Insert debuging information corresponding to a local variable or parameter
  void insertDebugDeclareLocal(llvm::BasicBlock &entry, BrigSymbol &local,
                               unsigned tag, llvm::DIDescriptor scope,
                               llvm::Value *arg = NULL, unsigned argNo = 0) {
    bool isArg = tag == llvm::dwarf::DW_TAG_arg_variable;
    assert(isArg == bool(arg));
    assert(isArg == bool(argNo));

    llvm::DIFile file = parent.DB.createFile("-", "");
    llvm::StringRef name = getStringRef(local.getName());

    // Arguments are passed by reference
    llvm::DIType dTy = runOnTypeDebug(parent.DB, local.getType());
    llvm::DIType dArgTy =
      parent.DB.createReferenceType(llvm::dwarf::DW_TAG_reference_type, dTy);

    llvm::DebugLoc loc = isArg ?
      llvm::DebugLoc::get(sub.getLineNumber(), 0, sub) : getDebugLoc(local);

    llvm::DIVariable dVar =
      parent.DB.createLocalVariable(tag, scope, name, file, loc.getLine(),
                                    isArg ? dArgTy : dTy, true, 0, argNo);

    // Use the stack address instead of the argument for parameters, since the
    // register holding the parameter may get killed later.
    llvm::Value *v = isArg ? arg : parent.symbolMap[local.getAddr()];

    // The DebugLoc of a gdbDeclare intrinsic controls the scope of the
    // declaration.
    llvm::Instruction *dbgInst = parent.DB.insertDeclare(v, dVar, &entry);
    dbgInst->setDebugLoc(loc);
  }

  size_t getFunId() const { return id; }
};

static llvm::Value *getRegAddr(llvm::BasicBlock &B,
                               const BrigString *name,
                               const BrigInstHelper &helper,
                               const FunScope &scope) {

  llvm::LLVMContext &C = B.getContext();

  unsigned field = getRegField(name);
  unsigned offset = getRegOffset(name);

  llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
  llvm::Value *array[] = { llvm::ConstantInt::get(int32Ty, 0),
                           llvm::ConstantInt::get(int32Ty, field),
                           llvm::ConstantInt::get(int32Ty, 0),
                           llvm::ConstantInt::get(int32Ty, offset) };
  llvm::ArrayRef<llvm::Value *> arrayRef(array, 4);

  return llvm::GetElementPtrInst::Create(scope.regs, arrayRef, "", &B);
}

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   const FunScope &scope) {
  if (const BrigOperandReg *regOp = dyn_cast<BrigOperandReg>(op)) {
    return getRegAddr(B, helper.getRegName(regOp), helper, scope);
  }

  assert(false && "Unimplemented operand");
}

static bool hasAddr(const BrigOperandBase *op) {
  BrigOperandKinds kind = BrigOperandKinds(op->kind);
  switch (kind) {
    case BRIG_OPERAND_ADDRESS:
    case BRIG_OPERAND_FUNCTION_REF:
    case BRIG_OPERAND_IMMED:
    case BRIG_OPERAND_LABEL_REF:
    case BRIG_OPERAND_REG_VECTOR:
    case BRIG_OPERAND_WAVESIZE:
      return false;
    case BRIG_OPERAND_REG:
      return true;
    default:
      assert(false && "Unimplemented");
  }
}

static llvm::Value *getOperand(llvm::BasicBlock &B,
                               const BrigOperandBase *op,
                               const BrigInstHelper &helper,
                               const FunScope &scope);

template<class T>
static llvm::Value *getVectorOperand(llvm::BasicBlock &B,
                                     const T *op,
                                     const BrigInstHelper &helper,
                                     const FunScope &scope) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);

  const size_t numElements = op->regCount;
  llvm::Type *elementTy = runOnType(C, BrigType(op->type));
  llvm::Type *vecTy = llvm::VectorType::get(elementTy, numElements);

  llvm::Value *vec = llvm::UndefValue::get(vecTy);
  for (unsigned i = 0; i < numElements; ++i) {
    llvm::Value *regAddr =
      getRegAddr(B, helper.getRegName(op, i), helper, scope);
    llvm::Value *element = new llvm::LoadInst(regAddr, "", true, &B);
    llvm::Value *idx = llvm::ConstantInt::get(int32Ty, i);
    vec = llvm::InsertElementInst::Create(vec, element, idx, "", &B);
  }

  return vec;
}

static llvm::Value *getOperand(llvm::BasicBlock &B,
                               const BrigOperandBase *op,
                               const BrigInstHelper &helper,
                               const FunScope &scope) {

  llvm::LLVMContext &C = B.getContext();

  if (hasAddr(op)) {
    llvm::Value *valAddr = getOperandAddr(B, op, helper, scope);
    llvm::Value *val = new llvm::LoadInst(valAddr, "", true, &B);
    return val;
  }

  if (const BrigOperandLabelRef *label = dyn_cast<BrigOperandLabelRef>(op)) {
    llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
    return llvm::ConstantInt::get(int32Ty, label->ref);
  }

  if (const BrigOperandFunctionRef *func = dyn_cast<BrigOperandFunctionRef>(op))
    return scope.lookupFun(func->ref);

  if (const BrigOperandImmed *immedOp = dyn_cast<BrigOperandImmed>(op)) {
    llvm::Type *type = runOnType(C, BrigType(immedOp->type));
    if (type->isIntegerTy(128)) {
      llvm::ArrayRef<uint64_t> bigIntArray((uint64_t *) immedOp->bytes, 2);
      llvm::APInt bigInt(128, bigIntArray);
      return llvm::ConstantInt::get(type, bigInt);
    } else if (type->isIntegerTy(64)) {
      uint64_t result;
      memcpy(&result, immedOp->bytes, sizeof(result));
      return llvm::ConstantInt::get(type, result);
    } else if (type->isIntegerTy(32)) {
      uint32_t result;
      memcpy(&result, immedOp->bytes, sizeof(result));
      return llvm::ConstantInt::get(type, result);
    } else if (type->isIntegerTy(16)) {
      uint16_t result;
      memcpy(&result, immedOp->bytes, sizeof(result));
      return llvm::ConstantInt::get(type, result);
    } else if (type->isIntegerTy(8)) {
      return llvm::ConstantInt::get(type, *(uint8_t *)  immedOp->bytes);
    } else if (type->isIntegerTy(1)) {
      return llvm::ConstantInt::get(type, *(uint8_t *)  immedOp->bytes);
    }
    assert(false && "Illegal immediate type");
  }

  if (const BrigOperandAddress *adderOp = dyn_cast<BrigOperandAddress>(op)) {

    llvm::Type *type = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);
    llvm::Value *addr;
    if (adderOp->symbol) {
      addr = scope.lookupSymbol(adderOp->symbol);
    } else {
      addr = llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(C));
    }

    uint64_t offset = ((uint64_t) adderOp->offsetHi << 32) + adderOp->offsetLo;
    if (!adderOp->reg && !offset)
      return addr;

    llvm::Value *adderInt = new llvm::PtrToIntInst(addr, type, "", &B);

    llvm::Value *base;
    if (adderOp->reg) {
      llvm::Value *regAddr =
        getRegAddr(B, helper.getRegName(adderOp), helper, scope);
      llvm::Value *regValue = new llvm::LoadInst(regAddr, "", true, &B);

      if (regValue->getType() == type) {
        base = regValue;
      } else {
        llvm::Instruction::CastOps castOp =
          llvm::CastInst::getCastOpcode(regValue, false, type, false);
        base = llvm::CastInst::Create(castOp, regValue, type, "", &B);
      }
    } else {
      base = llvm::ConstantInt::get(type, 0ULL);
    }

    llvm::Value *offsetValue = llvm::ConstantInt::get(type, offset);

    llvm::Value *index =
      llvm::BinaryOperator::Create(llvm::BinaryOperator::Add,
                                   base, offsetValue, "", &B);
    return llvm::BinaryOperator::Create(llvm::BinaryOperator::Add,
                                        adderInt, index, "", &B);
  }

  if (const BrigOperandRegVector *vecOp = dyn_cast<BrigOperandRegVector>(op))
    return getVectorOperand(B, vecOp, helper, scope);

  if (isa<BrigOperandWavesize>(op)) {
    llvm::Module *M = B.getParent()->getParent();
    llvm::FunctionType *getWavefrontSizeTy =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(C), false);
    llvm::Constant *wavefrontSizeFn =
      M->getOrInsertFunction("getWavefrontSize", getWavefrontSizeTy);
    return llvm::CallInst::Create(wavefrontSizeFn, "", &B);
  }

  assert(false && "Unimplemented");
}

static llvm::Type *getOperandTy(llvm::LLVMContext &C,
                                const inst_iterator inst,
                                unsigned opnum) {

  llvm::Type *destType = runOnType(C, BrigType(inst->type));

  if (opnum == 0) return destType;

  if ((inst->opcode == BRIG_OPCODE_LD     && opnum == 1) ||
      (inst->opcode == BRIG_OPCODE_ST     && opnum == 1) ||
      (inst->opcode == BRIG_OPCODE_ATOMIC && opnum == 1))
    return destType->getPointerTo();

  if (const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst))
    return runOnType(C, BrigType(cmp->sourceType));

  if (const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst))
    return runOnType(C, BrigType(cvt->sourceType));

  if ((inst->opcode == BRIG_OPCODE_CLASS && opnum == 2)) {
    return runOnType(C, BRIG_TYPE_U32);
  }

  if (inst->opcode == BRIG_OPCODE_PACK && opnum == 1)
    return destType;

  if (const BrigInstSourceType *st = dyn_cast<BrigInstSourceType>(inst)) {
    if (st->opcode != BRIG_OPCODE_COMBINE)
      return runOnType(C, BrigType(st->sourceType));
  }

  if ((inst->opcode == BRIG_OPCODE_SHR && opnum == 2) ||
     (inst->opcode == BRIG_OPCODE_SHL && opnum == 2))
    return runOnType(C, BRIG_TYPE_U32);

  if ((inst->opcode == BRIG_OPCODE_SHUFFLE && opnum == 3))
    return runOnType(C, BRIG_TYPE_U64);

  if (inst->opcode == BRIG_OPCODE_BITMASK)
    return runOnType(C, BRIG_TYPE_B32);

  return destType;
}

static llvm::Value *decodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  unsigned opnum,
                                  const inst_iterator inst) {
  llvm::LLVMContext &C = B.getContext();
  llvm::Type *destTy = getOperandTy(C, inst, opnum);

  if (destTy == value->getType())
    return value;

  if (destTy->isFloatingPointTy()) {
    const unsigned typeSize = value->getType()->getPrimitiveSizeInBits();
    const unsigned destTySize = destTy->getPrimitiveSizeInBits();

    if (typeSize == destTySize) {
      return new llvm::BitCastInst(value, destTy, "", &B);

    } else if (destTySize == 16) {
      llvm::Type *int16Ty = llvm::Type::getInt16Ty(C);
      llvm::Value *result = new llvm::TruncInst(value, int16Ty, "", &B);
      return new llvm::BitCastInst(result, destTy, "", &B);

    } else if (typeSize == 64) {
      llvm::Type *doubleTy = llvm::Type::getDoubleTy(C);
      llvm::Value *result = new llvm::BitCastInst(value, doubleTy, "", &B);
      return new llvm::FPTruncInst(result, destTy, "", &B);

    } else if (typeSize == 32) {
      llvm::Type *floatTy = llvm::Type::getFloatTy(C);
      llvm::Value *result = new llvm::BitCastInst(value, floatTy, "", &B);
      return new llvm::FPExtInst(result, destTy, "", &B);

    } else {
      assert(false && "Unimplemented");
    }
  }

  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(value, false, destTy, false);
  return llvm::CastInst::Create(castOp, value, destTy, "", &B);
}

static llvm::Value *encodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  llvm::Type *destTy,
                                  const inst_iterator inst,
                                  const BrigInstHelper &helper) {

  llvm::Type *encodedTy = value->getType();

  if (encodedTy == destTy)
    return value;

  if (encodedTy->isFloatingPointTy() || encodedTy->isVectorTy()) {

    llvm::LLVMContext &C = B.getContext();
    const unsigned encodedTySize = encodedTy->getPrimitiveSizeInBits();
    const unsigned destTySize = destTy->getPrimitiveSizeInBits();

    if (encodedTySize == destTySize) {
      return new llvm::BitCastInst(value, destTy, "", &B);

    } else if (encodedTySize == 64 && destTySize == 32) {
      llvm::Type *floatTy = llvm::Type::getFloatTy(C);
      llvm::Value *result = new llvm::FPTruncInst(value, floatTy, "", &B);
      return encodePacking(B, result, destTy, inst, helper);

    } else if (encodedTySize == 32 && destTySize == 64) {
      llvm::Type *doubleTy = llvm::Type::getDoubleTy(C);
      llvm::Value *result = new llvm::FPExtInst(value, doubleTy, "", &B);
      return encodePacking(B, result, destTy, inst, helper);

    } else if (encodedTySize == 16 && destTySize == 32) {
      llvm::Type *int16Ty = llvm::Type::getInt16Ty(C);
      llvm::Value *result = new llvm::BitCastInst(value, int16Ty, "", &B);
      return encodePacking(B, result, destTy, inst, helper);

    } else {
      assert(false && "Unimplemented");
    }
  }

  bool isSigned = helper.isSignedTy(BrigType(inst->type));
  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(value, isSigned, destTy, isSigned);
  return llvm::CastInst::Create(castOp, value, destTy, "", &B);
}

static bool isSRet(const inst_iterator inst) {
  if (isI386())
    return
      BrigInstHelper::hasDest(inst) &&
      BrigInstHelper::isVectorTy(BrigType(inst->type));

  if (isARM()) {
    BrigType type = BrigType(inst->type);
    if (BrigInstHelper::hasDest(inst))
      return
        type == BRIG_TYPE_U8X8  || type == BRIG_TYPE_S8X8  ||
        type == BRIG_TYPE_U16X4 || type == BRIG_TYPE_S16X4 ||
        type == BRIG_TYPE_U32X2 || type == BRIG_TYPE_S32X2 ||
        type == BRIG_TYPE_B128;

    return false;
  }

  return false;
}

static llvm::Function *getInstFun(const inst_iterator inst,
                                  const std::vector<llvm::Value *> &sources,
                                  llvm::Module *M) {

  bool sret = isSRet(inst);
  bool isStore = inst->opcode == BRIG_OPCODE_ST;
  llvm::LLVMContext &C = M->getContext();

  llvm::Type *result = sret || isStore ?
    llvm::Type::getVoidTy(C) :
    runOnType(C, BrigType(inst->type));
  std::vector<llvm::Type *> params;
  for (unsigned i = 0; i < sources.size(); ++i)
    params.push_back(sources[i]->getType());

  llvm::FunctionType *instFunTy =
    llvm::FunctionType::get(result, params, false);

  std::string name = BrigInstHelper::getInstName(inst);
  llvm::Function *instFun =
    llvm::cast<llvm::Function>(M->getOrInsertFunction(name, instFunTy));

  if (sret) {
    llvm::AttrBuilder AB;
    AB.addAttribute(llvm::Attribute::StructRet);
    AB.addAttribute(llvm::Attribute::NoAlias);
    AB.addAttribute(llvm::Attribute::NoCapture);
    llvm::AttributeSet attrs =
      llvm::AttributeSet::get(C, 1, AB);
    instFun->setAttributes(attrs);
  }

  return instFun;
}

static void insertEnableFtz(llvm::BasicBlock &B) {
  llvm::Module *M = B.getParent()->getParent();
  llvm::LLVMContext &C = M->getContext();
  llvm::FunctionType *enableFtzTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), false);
  llvm::Constant *enableFtz =
    M->getOrInsertFunction("enableFtzMode", enableFtzTy);
  llvm::CallInst::Create(enableFtz, "", &B);
}

static void insertDisableFtz(llvm::BasicBlock &B) {
  llvm::Module *M = B.getParent()->getParent();
  llvm::LLVMContext &C = M->getContext();
  llvm::FunctionType *disableFtzTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), false);
  llvm::Constant *disableFtz =
    M->getOrInsertFunction("disableFtzMode", disableFtzTy);
  llvm::CallInst::Create(disableFtz, "", &B);
}

static void insertSetRoundingMode(llvm::BasicBlock &B,
                                  const inst_iterator inst) {
  llvm::Module *M = B.getParent()->getParent();
  llvm::LLVMContext &C = M->getContext();
  llvm::FunctionType *setRoundTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), false);
  const BrigAluModifier16_t *aluMod = BrigInstHelper::getAluModifier(inst);
  const char *roundName = BrigInstHelper::getRoundingName(*aluMod);
  std::string setRoundName = std::string("setRoundingMode_") + roundName;
  llvm::Constant *setRoundMode =
    M->getOrInsertFunction(setRoundName, setRoundTy);
  llvm::CallInst::Create(setRoundMode, "", &B);
}

static void insertRestoreRoundingMode(llvm::BasicBlock &B) {
  llvm::Module *M = B.getParent()->getParent();
  llvm::LLVMContext &C = M->getContext();
  llvm::FunctionType *restoreRoundTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), false);
  llvm::Constant *restoreRound =
    M->getOrInsertFunction("setRoundingMode_near", restoreRoundTy);
  llvm::CallInst::Create(restoreRound, "", &B);
}

static void insertUpdatePC(llvm::BasicBlock &B,
                           inst_iterator inst,
                           const BrigInstHelper &helper,
                           const FunScope &scope) {

  if (!scope.getHSADebugger()) return;

  llvm::LLVMContext &C = B.getContext();

  llvm::Type *intPtrTy = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);

  llvm::Type *argsTy[] = { intPtrTy, intPtrTy };
  llvm::FunctionType *callbackTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), argsTy, false);

  void (*updatePC)(HSADebugger *, size_t) = HSADebugger::updatePC;
  llvm::Constant *cbIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) updatePC);
  llvm::Value *cbFPValue =
    llvm::ConstantExpr::getIntToPtr(cbIntValue, callbackTy->getPointerTo());

  llvm::Value *cbdIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) scope.getHSADebugger());

  llvm::Value *pcValue =
    llvm::ConstantInt::get(intPtrTy, helper.getAddr(inst));

  llvm::Value *args[] = { cbdIntValue, pcValue };

  llvm::CallInst::Create(cbFPValue, args, "", &B);
}

static void insertEnterFn(llvm::BasicBlock &B, const FunScope &scope) {

  if (!scope.getHSADebugger()) return;

  llvm::LLVMContext &C = B.getContext();

  llvm::Type *intPtrTy = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);

  llvm::Type *argsTy[] = { intPtrTy, scope.regs->getType(), intPtrTy };
  llvm::FunctionType *callbackTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), argsTy, false);

  void (*enterFn)(HSADebugger *, BrigRegState *, FunId) = HSADebugger::enterFn;
  llvm::Constant *cbIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) enterFn);
  llvm::Value *cbFPValue =
    llvm::ConstantExpr::getIntToPtr(cbIntValue, callbackTy->getPointerTo());

  llvm::Value *cbdIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) scope.getHSADebugger());

  llvm::Value *idValue =
    llvm::ConstantInt::get(intPtrTy, scope.getFunId());

  llvm::Value *args[] = { cbdIntValue, scope.regs, idValue };

  llvm::CallInst::Create(cbFPValue, args, "", &B);
}

static void insertLeaveFn(llvm::BasicBlock &B, const FunScope &scope) {

  if (!scope.getHSADebugger()) return;

  llvm::LLVMContext &C = B.getContext();

  llvm::Type *intPtrTy = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);

  llvm::Type *argsTy[] = { intPtrTy };
  llvm::FunctionType *callbackTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), argsTy, false);

  void (*leaveFn)(HSADebugger *) = HSADebugger::leaveFn;
  llvm::Constant *cbIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) leaveFn);
  llvm::Value *cbFPValue =
    llvm::ConstantExpr::getIntToPtr(cbIntValue, callbackTy->getPointerTo());

  llvm::Value *cbdIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) scope.getHSADebugger());

  llvm::Value *args[] = { cbdIntValue };

  llvm::CallInst::Create(cbFPValue, args, "", &B);
}

static void insertDeclareVariable(llvm::BasicBlock &B,
                                  const SymbolId id,
                                  const bool isGlobal,
                                  const FunScope &scope) {

  if (!scope.getHSADebugger()) return;

  llvm::LLVMContext &C = B.getContext();

  llvm::Value *v = scope.lookupSymbol(id);

  llvm::Type *intPtrTy = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);
  llvm::Type *int8PtrTy = llvm::Type::getInt8PtrTy(C);

  llvm::Type *argsTy[] = { intPtrTy, int8PtrTy, intPtrTy };
  llvm::FunctionType *callbackTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), argsTy, false);

  void (*declareSymbol)(HSADebugger *, void *, SymbolId);
  if (isGlobal) declareSymbol = HSADebugger::declareGlobal;
  else declareSymbol = HSADebugger::declareLocal;

  llvm::Constant *cbIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) declareSymbol);
  llvm::Value *cbFPValue =
    llvm::ConstantExpr::getIntToPtr(cbIntValue, callbackTy->getPointerTo());

  llvm::Value *cbdIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) scope.getHSADebugger());

  llvm::Value *addr = new llvm::BitCastInst(v, int8PtrTy, "", &B);

  llvm::Value *idValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) id);

  llvm::Value *args[] = { cbdIntValue, addr, idValue };

  llvm::CallInst::Create(cbFPValue, args, "", &B);
}

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunScope &scope) {

  unsigned operand = 0;

  bool ftz = BrigInstHelper::isFtz(inst);
  if (ftz) insertEnableFtz(B);

  bool rounding = BrigInstHelper::hasRoundingMode(inst);
  if (rounding) insertSetRoundingMode(B, inst);

  bool sret = isSRet(inst);

  std::vector<llvm::Value *> sources;

  llvm::Value *destAddr = NULL;
  if (BrigInstHelper::hasDest(inst)) {
    const BrigOperandBase *brigDest = helper.getOperand(inst, operand++);
    destAddr = getOperandAddr(B, brigDest, helper, scope);
    if (sret) sources.push_back(destAddr);
  }

  for (; operand < 5 && inst->operands[operand]; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);
    llvm::Value *srcRaw = getOperand(B, brigSrc, helper, scope);
    llvm::Value *srcVal = decodePacking(B, srcRaw, operand, inst);
    sources.push_back(srcVal);
  }

  llvm::Module *M = B.getParent()->getParent();
  llvm::Function *instFun = getInstFun(inst, sources, M);
  llvm::Value *resultRaw = llvm::CallInst::Create(instFun, sources, "", &B);

  if (destAddr && !sret) {
    llvm::PointerType *destPtrTy =
      llvm::cast<llvm::PointerType>(destAddr->getType());
    llvm::Type *destTy = destPtrTy->getElementType();
    llvm::Value *resultVal = encodePacking(B, resultRaw, destTy, inst, helper);
    new llvm::StoreInst(resultVal, destAddr, &B);
  }

  if (rounding) insertRestoreRoundingMode(B);

  if (ftz) insertDisableFtz(B);
}

static void runOnDirectBranchInst(llvm::BasicBlock &B,
                                  const inst_iterator inst,
                                  const BrigInstHelper &helper,
                                  const FunScope &scope) {

  // The width of the branch is not necessary for a functional simulator.
  // Similarly, we can ignore the list of possible branch targets for an
  // indirect branch. In debug mode, we should check for and log branchs outside
  // the target width and branch divergence beyond the limit given by width.
  const BrigOperandBase *target = helper.getBranchTarget(inst);
  llvm::ConstantInt *cbNum =
    llvm::cast<llvm::ConstantInt>(getOperand(B, target, helper, scope));
  llvm::BasicBlock *targetBB = scope.cbMap.find(cbNum->getZExtValue())->second;

  if (inst->opcode == BRIG_OPCODE_CBR) {
    const BrigOperandBase *pred = helper.getOperand(inst, 0);
    llvm::Value *predVal = getOperand(B, pred, helper, scope);
    llvm::BranchInst::Create(targetBB, NULL, predVal, &B);
    return;
  }

  if (inst->opcode == BRIG_OPCODE_BRN) {
    llvm::BranchInst::Create(targetBB, &B);
    return;
  }

  assert(false && "Unknown branch opcode");
}

static void runOnIndirectBranchInst(llvm::BasicBlock &B,
                                    const inst_iterator inst,
                                    const BrigInstHelper &helper,
                                    const FunScope &scope) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Function *F = B.getParent();
  const BrigOperandBase *target = helper.getBranchTarget(inst);
  llvm::Value *targetBB = getOperand(B, target, helper, scope);

  llvm::BasicBlock *launchBB = &B;
  if (inst->opcode == BRIG_OPCODE_CBR) {
    launchBB = llvm::BasicBlock::Create(C, B.getName() + ".launch", F);
    const BrigOperandBase *pred = helper.getOperand(inst, 0);
    llvm::Value *predVal = getOperand(B, pred, helper, scope);
    llvm::BranchInst::Create(launchBB, NULL, predVal, &B);
  }

  llvm::IntegerType *labelTy =
    llvm::cast<llvm::IntegerType>(targetBB->getType());
  const FunScope::CBMap &cbMap = scope.cbMap;
  llvm::SwitchInst *launchInst =
    llvm::SwitchInst::Create(targetBB, &B, cbMap.size(), launchBB);
  for (FunScope::CBIt cb = cbMap.begin(), E = cbMap.end(); cb != E; ++cb) {
    llvm::BasicBlock *dest = cb->second;
    llvm::ConstantInt *label = llvm::ConstantInt::get(labelTy, cb->first);
    if (dest != &F->getEntryBlock())
      launchInst->addCase(label, dest);
  }
  launchInst->setDefaultDest(launchInst->getSuccessor(1));
}

static llvm::Value *decodeFunPacking(llvm::BasicBlock &B,
                                     llvm::Value *rawFun,
                                     const std::vector<llvm::Value *> &args) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Type *result = llvm::Type::getVoidTy(C);
  std::vector<llvm::Type *> params;
  for (unsigned i = 0; i < args.size(); ++i) {
    params.push_back(args[i]->getType());
  }

  llvm::Type *funTy = llvm::FunctionType::get(result, params, false);
  llvm::Type *funPtrTy = funTy->getPointerTo(0);
  if (funPtrTy == rawFun->getType()) return rawFun;

  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(rawFun, false, funPtrTy, false);

  return llvm::CastInst::Create(castOp, rawFun, funPtrTy, "", &B);
}

static void runOnCallInst(llvm::BasicBlock &B,
                          const inst_iterator inst,
                          const BrigInstHelper &helper,
                          const FunScope &scope) {

  std::vector<llvm::Value *> args;

  const BrigOperandArgumentList *oArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 0));
  for (unsigned i = 0; i < oArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(oArgList, i);
    args.push_back(scope.lookupSymbol(symbol));
  }

  const BrigOperandArgumentList *iArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 2));
  for (unsigned i = 0; i < iArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(iArgList, i);
    args.push_back(scope.lookupSymbol(symbol));
  }

  const BrigOperandBase *brigFun = helper.getOperand(inst, 1);
  llvm::Value *rawFun = getOperand(B, brigFun, helper, scope);
  llvm::Value *fun = decodeFunPacking(B, rawFun, args);
  llvm::CallInst::Create(fun, args, "", &B);
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunScope &scope) {
  llvm::LLVMContext &C = B.getContext();

  insertUpdatePC(B, inst, helper, scope);

  if (inst->opcode == BRIG_OPCODE_RET) {
    insertLeaveFn(B, scope);
    llvm::ReturnInst::Create(C, &B);
  } else if (helper.isDirectBranchInst(inst)) {
    runOnDirectBranchInst(B, inst, helper, scope);
  } else if (helper.isIndirectBranchInst(inst)) {
    runOnIndirectBranchInst(B, inst, helper, scope);
  } else if (inst->opcode == BRIG_OPCODE_CALL) {
    runOnCallInst(B, inst, helper, scope);
  } else {
    runOnComplexInst(B, inst, helper, scope);
  }
}

static void updateDebugInfo(llvm::BasicBlock &B,
                            const inst_iterator inst,
                            const BrigInstHelper &helper,
                            const FunScope &scope) {

  if (!scope.hasDebugInfo()) return;

  llvm::DebugLoc loc = scope.getDebugLoc(helper.getAddr(inst));

  typedef llvm::BasicBlock::iterator BBIt;
  // Don't add metadata to any instructions that may appear in the function
  // prolog. It may cause breakpoints to be set in the prolog.
  for (BBIt it = B.begin(), E = B.end(); it != E; ++it)
    if (!it->hasMetadata() &&
        !llvm::isa<llvm::AllocaInst>(it) &&
        !llvm::isa<llvm::StoreInst>(it) &&
        !llvm::isa<llvm::MemIntrinsic>(it))
      it->setDebugLoc(loc);

  // Update previous BBs if they have no debug information of their own.  For
  // example, HSA instruction-less BBs caused by labels at the beginning of
  // functions.
  if (llvm::BasicBlock *pred = B.getUniquePredecessor())
    if (!pred->begin()->hasMetadata())
      updateDebugInfo(*pred, inst, helper, scope);
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB,
                    const FunScope &scope) {

  llvm::LLVMContext &C = F.getContext();
  BrigInstHelper helper = CB.getInstHelper();
  FunScope::CBIt it = scope.cbMap.find(CB.getOffset());
  assert(it != scope.cbMap.end() && "Missing CB");
  llvm::BasicBlock *bb = it->second;

  for (inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst, helper, scope);
    updateDebugInfo(*bb, inst, helper, scope);

    // Create a new basic block after every conditional branch to handle the
    // fall-through path
    llvm::BranchInst *branch = llvm::dyn_cast<llvm::BranchInst>(&bb->back());
    if (branch && branch->isConditional()) {
      bb = llvm::BasicBlock::Create(C, bb->getName() + ".succ", &F);
      branch->setSuccessor(1, bb);
    }
  }

  // Fall through to the next control block
  if (bb->empty() || !llvm::isa<llvm::TerminatorInst>(&bb->back())) {
    ++it;
    if (it != scope.cbMap.end()) {
      llvm::BranchInst::Create(it->second, bb);
    } else {
      insertLeaveFn(*bb, scope);
      llvm::ReturnInst::Create(C, bb);
    }
  }
}

static llvm::Value *getParameter(llvm::BasicBlock *bb,
                                 llvm::Value *argArray,
                                 llvm::Type *paramTy,
                                 unsigned paramNo) {
  llvm::LLVMContext &C = bb->getContext();
  llvm::Value *offset =
    llvm::ConstantInt::get(llvm::Type::getInt32Ty(C), paramNo);
  llvm::Value *gep = llvm::GetElementPtrInst::Create(argArray, offset, "", bb);

  llvm::Value *load = new llvm::LoadInst(gep, "", true, bb);
  return new llvm::BitCastInst(load, paramTy, "", bb);
}


// We create a trampoline with the function signature:
// void fun(char **argv)
// The real function arguments are encoded in the argv array.
static void makeKernelTrampoline(llvm::Function *fun,
                                 const FunScope &scope) {

  llvm::LLVMContext &C = fun->getContext();
  llvm::Module *M = fun->getParent();

  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  llvm::Type *trampArgs[] = {
    llvm::Type::getInt8Ty(C)->getPointerTo()->getPointerTo()
  };
  llvm::FunctionType *trampFunTy =
    llvm::FunctionType::get(voidTy, trampArgs, false);
  llvm::GlobalValue::LinkageTypes linkage = fun->getLinkage();

  llvm::Twine name = "kernel." + fun->getName();
  llvm::Function *trampFun =
    llvm::Function::Create(trampFunTy, linkage, name, fun->getParent());
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(C, "", trampFun);

  llvm::Value *argArray = trampFun->arg_begin();
  llvm::Type *int8PtrTy = llvm::Type::getInt8PtrTy(C);
  llvm::Value *args[] = { getParameter(bb, argArray, int8PtrTy, 0) };
  llvm::Value *setThreadInfoFun = M->getFunction("__setThreadInfo");
  llvm::CallInst::Create(setThreadInfoFun, args, "", bb);

  llvm::FunctionType *funTy = fun->getFunctionType();
  std::vector<llvm::Value *> trampParams;
  for (unsigned i = 0; i < fun->arg_size(); ++i) {
    llvm::Type *paramTy = funTy->getParamType(i);
    trampParams.push_back(getParameter(bb, argArray, paramTy, i + 1));
  }

  typedef SymbolInfoMap::const_iterator SymbolInfoMapIt;
  for (SymbolInfoMapIt info = scope.symbol_begin(),
        E = scope.symbol_end(); info != E; ++info)
    if (info->second.isGlobal)
      insertDeclareVariable(*bb, info->first, true, scope);

  llvm::CallInst::Create(fun, trampParams, "", bb);
  llvm::ReturnInst::Create(C, bb);
}

static llvm::Function *createFunctionDecl(llvm::Module &M,
                                          const BrigFunction &F) {
  llvm::LLVMContext &C = M.getContext();

  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  std::vector<llvm::Type *> argVec;
  for (BrigSymbol arg = F.arg_begin(), E = F.arg_end(); arg != E; ++arg) {
    argVec.push_back(runOnType(C, arg)->getPointerTo(0));
  }
  llvm::ArrayRef<llvm::Type *> args(argVec);
  llvm::FunctionType *funTy = llvm::FunctionType::get(voidTy, args, false);

  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(F.getLinkage());
  llvm::StringRef nameRef = getStringRef(F.getName());
  llvm::Twine name(nameRef);

  if (llvm::Function *fun = M.getFunction(name.str()))
    return fun;

  return llvm::Function::Create(funTy, linkage, name, &M);
}

static void runOnFunction(llvm::Module &M, const BrigFunction &F,
                          ModScope &mScope) {

  llvm::Function *fun = mScope.funMap[F.getOffset()];
  fun->addFnAttr("no-frame-pointer-elim", "true");

  BrigSymbol brigArg = F.arg_begin();
  llvm::Function::arg_iterator llvmArg = fun->arg_begin();
  llvm::Function::arg_iterator E = fun->arg_end();
  for (; llvmArg != E; ++brigArg, ++llvmArg) {
    llvm::StringRef name = getStringRef(brigArg.getName());
    llvmArg->setName(name);
    mScope.symbolMap[brigArg.getAddr()] = llvmArg;
    mScope.symbolInfoMap[brigArg.getAddr()] =
      SymbolInfo(name.str(), brigArg, false);
  }

  if (F.isDeclaration()) return;

  const FunScope fScope(mScope, F, fun);
  for (BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, fScope);
  }

  if (F.isKernel()) makeKernelTrampoline(fun, fScope);
}

template<class T>
static llvm::Constant *runOnInitializer(llvm::LLVMContext &C,
                                        llvm::Type *type,
                                        const T *array) {
  return llvm::ConstantInt::get(type, *array);
}

template<>
llvm::Constant *runOnInitializer<float>(llvm::LLVMContext &C,
                                        llvm::Type *type,
                                        const float *array) {
  llvm::APInt API(32, *(const uint32_t *) array);
  llvm::APFloat APF(llvm::APFloat::IEEEsingle, API);
  return llvm::ConstantFP::get(C, APF);
}

template<>
llvm::Constant *runOnInitializer<double>(llvm::LLVMContext &C,
                                         llvm::Type *type,
                                         const double *array) {
  llvm::APInt API(64, *(const uint64_t *) array);
  llvm::APFloat APF(llvm::APFloat::IEEEdouble, API);
  return llvm::ConstantFP::get(C, APF);
}

template<class T>
static llvm::Constant *runOnInitializer(llvm::LLVMContext &C,
                                        llvm::Type *type,
                                        const BrigSymbol &S) {
  const T *array = S.getInit<T>();
  uint64_t size = std::max(uint64_t(1), S.getArrayDim());
  if (S.isArray()) {
    llvm::ArrayRef<T> arrayRef(array, size);
    return llvm::ConstantDataArray::get(C, arrayRef);
  }

  return runOnInitializer(C, type, array);
}

static void runOnGlobal(llvm::Module &M, const BrigSymbol &S,
                        SymbolMap &symbolMap,
                        SymbolInfoMap &symbolInfoMap,
                        llvm::DIBuilder &DB) {
  llvm::LLVMContext &C = M.getContext();
  llvm::Type *type = runOnType(C, S);
  bool isConst = S.isConst();
  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(S.getLinkage());
  llvm::StringRef name = getStringRef(S.getName());

  llvm::Constant *init = NULL;
  if (S.hasInitializer()) {
    llvm::Type *elementTy = llvm::isa<llvm::SequentialType>(type) ?
      type->getArrayElementType() : type;
    if (elementTy->isIntegerTy(1) || elementTy->isIntegerTy(8)) {
      init = runOnInitializer<uint8_t>(C, elementTy, S);
    } else if (elementTy->isIntegerTy(16)) {
      init = runOnInitializer<uint16_t>(C, elementTy, S);
    } else if (elementTy->isIntegerTy(32)) {
      init = runOnInitializer<uint32_t>(C, elementTy, S);
    } else if (elementTy->isIntegerTy(64)) {
      init = runOnInitializer<uint64_t>(C, elementTy, S);
    } else if (elementTy->isFloatTy()) {
      init = runOnInitializer<float>(C, elementTy, S);
    } else if (elementTy->isDoubleTy()) {
      init = runOnInitializer<double>(C, elementTy, S);
    } else {
      assert(false && "Unimplemented");
    }
  } else {
    init = llvm::Constant::getNullValue(type);
  }

  llvm::Value *global =
    new llvm::GlobalVariable(M, type, isConst, linkage, init, name);
  symbolMap[S.getAddr()] = global;
  symbolInfoMap[S.getAddr()] = SymbolInfo(name.str(), S, true);

  llvm::DIType debugTy = runOnTypeDebug(DB, S.getType());
  llvm::DIFile file = DB.createFile("-", "");
  DB.createGlobalVariable(name, file, 0, debugTy, true, global);
}

llvm::DIContext *runOnDebugInfo(const BrigModule &M) {

  BrigInstHelper helper = M.getInstHelper();

  for (debug_iterator it = M.debug_begin(),
        E = M.debug_end(); it != E; ++it) {
    if (const BrigBlockNumeric *numeric = dyn_cast<BrigBlockNumeric>(it)) {
      const BrigString *str = helper.getData(numeric);
      llvm::StringRef debugData((const char *) str->bytes, str->byteCount);
      llvm::MemoryBuffer *debugDataBuffer =
        llvm::MemoryBuffer::getMemBuffer(debugData, "", false);
      llvm::object::ObjectFile *objFile =
        llvm::object::ObjectFile::createObjectFile(debugDataBuffer);

      int errFID = dup(STDERR_FILENO);
      int nullFID = open("/dev/null", O_WRONLY);
      dup2(nullFID, STDERR_FILENO);

      llvm::DIContext *dic = llvm::DIContext::getDWARFContext(objFile);

      dup2(errFID, STDERR_FILENO);
      close(nullFID);
      close(errFID);

      delete objFile;

      return dic;
    }
  }

  return NULL;
}

BrigProgram GenLLVM::getLLVMModule(const BrigModule &M,
                                   HSADebugger *dbg) {

  if (!M.isValid()) return NULL;

  llvm::LLVMContext *C = new llvm::LLVMContext();
  llvm::Module *mod = new llvm::Module("BRIG", *C);

  // Call normalize to ensure the triple string is correctly formatted.
  // Incorrectly formatted triple strings will prevent LLVM from ensuring the
  // correct stack alignment. On i386 on Linux, the correct stack alignment is
  // 16-bytes, but the default LLVM stack alignment for i386 is only 8-bytes.
  // An insufficiently aligned stack will yield segmentation faults when
  // LLVM-generated code calls into brig_runtime.cc. Saving a xmm register to
  // the stack with the movaps instruction is particularly problematic. The
  // underlying issue will be fixed upstream in LLVM 3.4 by svn 186501 from
  // PR126636. Consequently, the normalization can be removed when the simulator
  // updates to LLVM 3.4.
  mod->setTargetTriple(llvm::Triple::normalize(llvm::sys::getProcessTriple()));

  llvm::DIContext *debugInfo = runOnDebugInfo(M);
  llvm::DIBuilder DB(*mod);
  DB.createCompileUnit(llvm::dwarf::DW_LANG_lo_user,
                       "-", "", "brig2llvm", true, "", 0);

  insertGPUStateTy(*C);
  insertSetThreadInfo(*C, mod);

  SymbolMap symbolMap;
  SymbolInfoMap symbolInfoMap;
  for (BrigSymbol symbol = M.global_begin(),
        E = M.global_end(); symbol != E; ++symbol) {
    runOnGlobal(*mod, symbol, symbolMap, symbolInfoMap, DB);
  }

  FunMap funMap;
  for (BrigFunction fun = M.begin(), E = M.end(); fun != E; ++fun) {
    funMap[fun.getOffset()] = createFunctionDecl(*mod, fun);
  }

  ModScope scope(funMap, symbolMap, symbolInfoMap, debugInfo, dbg, DB);
  for (BrigFunction fun = M.begin(), E = M.end(); fun != E; ++fun) {
    runOnFunction(*mod, fun, scope);
  }

  DB.finalize();

  return BrigProgram(mod, debugInfo, funMap, symbolInfoMap);
}

std::string GenLLVM::getLLVMString(const BrigModule &M,
                                   HSADebugger *dbg) {
  BrigProgram BP = getLLVMModule(M, dbg);
  if (!BP) return "";

  std::string output;
  llvm::raw_string_ostream ros(output);
  llvm::Module *mod = BP;
  mod->print(ros, NULL);

  return output;
}

}
}
