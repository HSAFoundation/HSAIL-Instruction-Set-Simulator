//===- brig2llvm.cc -------------------------------------------------------===//
//
//                     The HSA Simultor
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
#include "brig_symbol.h"

#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <fcntl.h>
#include <unistd.h>

namespace hsa{
namespace brig{

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

  switch(type) {
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

typedef std::map<uint32_t, llvm::Function *> FunMap;
typedef std::map<const void *, llvm::Value *> SymbolMap;

struct FunState {
  typedef std::map<uint32_t, llvm::BasicBlock *> CBMap ;
  typedef CBMap::const_iterator CBIt;

  CBMap cbMap;
  const FunMap &funMap;
  llvm::Value *regs;
  const Callback callback;
  CallbackData cbd;

  static const FunState Create(const FunMap &funMap,
                               SymbolMap &symbolMap,
                               const BrigFunction &brigFun,
                               llvm::Function *llvmFun,
                               Callback callback, CallbackData cbd) {
    return FunState(funMap, symbolMap, brigFun, llvmFun, callback, cbd);
  }

  llvm::Value *lookupSymbol(const BrigDirectiveSymbol *symbol) const {
    SymbolMap::const_iterator it = symbolMap.find(symbol);
    return it != symbolMap.end() ? it->second : NULL;
  }

  llvm::Value *lookupSymbol(const BrigSymbol &symbol) const {
    SymbolMap::const_iterator it = symbolMap.find(symbol.getAddr());
    return it != symbolMap.end() ? it->second : NULL;
  }

 private:
  SymbolMap &symbolMap;

  FunState(const FunMap &funMap, SymbolMap &symbolMap,
           const BrigFunction &brigFun, llvm::Function *llvmFun,
           Callback callback, CallbackData cbd) :
    funMap(funMap),
    callback(callback),
    cbd(cbd),
    symbolMap(symbolMap) {

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

    for (BrigSymbol local = brigFun.local_begin(),
          E = brigFun.local_end(); local != E; ++local) {
      llvm::StringRef name = getStringRef(local.getName());
      llvm::Type *type = runOnType(C, local);
      symbolMap[local.getAddr()] = new llvm::AllocaInst(type, name, &entry);
    }
  }
};

static llvm::Value *getRegAddr(llvm::BasicBlock &B,
                               const BrigString *name,
                               const BrigInstHelper &helper,
                               const FunState &state) {

  llvm::LLVMContext &C = B.getContext();

  unsigned field = getRegField(name);
  unsigned offset = getRegOffset(name);

  llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
  llvm::Value *array[] = { llvm::ConstantInt::get(int32Ty, 0),
                           llvm::ConstantInt::get(int32Ty, field),
                           llvm::ConstantInt::get(int32Ty, 0),
                           llvm::ConstantInt::get(int32Ty, offset) };
  llvm::ArrayRef<llvm::Value *> arrayRef(array, 4);

  return llvm::GetElementPtrInst::Create(state.regs, arrayRef, "", &B);
}

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   const FunState &state) {
  if (const BrigOperandReg *regOp = dyn_cast<BrigOperandReg>(op)) {
    return getRegAddr(B, helper.getRegName(regOp), helper, state);
  }

  assert(false && "Unimplemented operand");
}

static bool hasAddr(const BrigOperandBase *op) {
  BrigOperandKinds kind = BrigOperandKinds(op->kind);
  switch(kind) {
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
                               const FunState &state);

template<class T>
static llvm::Value *getVectorOperand(llvm::BasicBlock &B,
                                     const T *op,
                                     const BrigInstHelper &helper,
                                     const FunState &state) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);

  const size_t numElements = op->regCount;
  llvm::Type *elementTy = runOnType(C, BrigType(op->type));
  llvm::Type *vecTy = llvm::VectorType::get(elementTy, numElements);

  llvm::Value *vec = llvm::UndefValue::get(vecTy);
  for (unsigned i = 0; i < numElements; ++i) {
    llvm::Value *regAddr =
      getRegAddr(B, helper.getRegName(op, i), helper, state);
    llvm::Value *element = new llvm::LoadInst(regAddr, "", false, &B);
    llvm::Value *idx = llvm::ConstantInt::get(int32Ty, i);
    vec = llvm::InsertElementInst::Create(vec, element, idx, "", &B);
  }

  return vec;
}

static llvm::Value *getOperand(llvm::BasicBlock &B,
                               const BrigOperandBase *op,
                               const BrigInstHelper &helper,
                               const FunState &state) {

  llvm::LLVMContext &C = B.getContext();

  if (hasAddr(op)) {
    llvm::Value *valAddr = getOperandAddr(B, op, helper, state);
    llvm::Value *val = new llvm::LoadInst(valAddr, "", false, &B);
    return val;
  }

  if (const BrigOperandLabelRef *label = dyn_cast<BrigOperandLabelRef>(op)) {
    llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
    return llvm::ConstantInt::get(int32Ty, label->ref);
  }

  if (const BrigOperandFunctionRef *func = dyn_cast<BrigOperandFunctionRef>(op))
    return state.funMap.find(func->ref)->second;

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
      const BrigDirectiveSymbol *symbol =
        cast<BrigDirectiveSymbol>(helper.getDirective(adderOp->symbol));
      addr = state.lookupSymbol(symbol);
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
        getRegAddr(B, helper.getRegName(adderOp), helper, state);
      llvm::Value *regValue = new llvm::LoadInst(regAddr, "", false, &B);

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
    return getVectorOperand(B, vecOp, helper, state);

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

  if ((inst->opcode == BRIG_OPCODE_SHR && opnum == 2) ||
     (inst->opcode == BRIG_OPCODE_SHL && opnum == 2))
    return runOnType(C, BRIG_TYPE_U32);

  if ((inst->opcode == BRIG_OPCODE_SHUFFLE && opnum == 3))
    return runOnType(C, BRIG_TYPE_U64);

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
      llvm::AttributeSet::get(C, llvm::AttributeSet::FunctionIndex, AB);
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

static void insertDebugCallback(llvm::BasicBlock &B,
                                inst_iterator inst,
                                const BrigInstHelper &helper,
                                const FunState &state) {

  if (!state.callback) return;

  llvm::LLVMContext &C = B.getContext();

  llvm::Type *intPtrTy = llvm::Type::getIntNTy(C, sizeof(intptr_t) * 8);

  llvm::Type *argsTy[] = { state.regs->getType(), intPtrTy, intPtrTy };
  llvm::FunctionType *callbackTy =
    llvm::FunctionType::get(llvm::Type::getVoidTy(C), argsTy, false);

  llvm::Constant *cbIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) state.callback);
  llvm::Value *cbFPValue =
    llvm::ConstantExpr::getIntToPtr(cbIntValue, callbackTy->getPointerTo());

  llvm::Value *pcValue =
    llvm::ConstantInt::get(intPtrTy, helper.getAddr(inst));

  llvm::Value *cbdIntValue =
    llvm::ConstantInt::get(intPtrTy, (intptr_t) state.cbd);

  llvm::Value *args[] = { state.regs, pcValue, cbdIntValue };

  llvm::CallInst::Create(cbFPValue, args, "", &B);
}

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunState &state) {

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
    destAddr = getOperandAddr(B, brigDest, helper, state);
    if (sret) sources.push_back(destAddr);
  }

  for (; operand < 5 && inst->operands[operand]; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);
    llvm::Value *srcRaw = getOperand(B, brigSrc, helper, state);
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
                                  const FunState &state) {

  // The width of the branch is not necessary for a functional simulator.
  // Similarly, we can ignore the list of possible branch targets for an
  // indirect branch. In debug mode, we should check for and log branchs outside
  // the target width and branch divergence beyond the limit given by width.
  const BrigOperandBase *target = helper.getBranchTarget(inst);
  llvm::ConstantInt *cbNum =
    llvm::cast<llvm::ConstantInt>(getOperand(B, target, helper, state));
  llvm::BasicBlock *targetBB = state.cbMap.find(cbNum->getZExtValue())->second;

  if (inst->opcode == BRIG_OPCODE_CBR) {
    const BrigOperandBase *pred = helper.getOperand(inst, 0);
    llvm::Value *predVal = getOperand(B, pred, helper, state);
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
                                    const FunState &state) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Function *F = B.getParent();
  const BrigOperandBase *target = helper.getBranchTarget(inst);
  llvm::Value *targetBB = getOperand(B, target, helper, state);

  llvm::BasicBlock *launchBB = &B;
  if (inst->opcode == BRIG_OPCODE_CBR) {
    launchBB = llvm::BasicBlock::Create(C, B.getName() + ".launch", F);
    const BrigOperandBase *pred = helper.getOperand(inst, 0);
    llvm::Value *predVal = getOperand(B, pred, helper, state);
    llvm::BranchInst::Create(launchBB, NULL, predVal, &B);
  }

  llvm::IntegerType *labelTy =
    llvm::cast<llvm::IntegerType>(targetBB->getType());
  const FunState::CBMap &cbMap = state.cbMap;
  llvm::SwitchInst *launchInst =
    llvm::SwitchInst::Create(targetBB, &B, cbMap.size(), launchBB);
  for (FunState::CBIt cb = cbMap.begin(), E = cbMap.end(); cb != E; ++cb) {
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
                          const FunState &state) {

  std::vector<llvm::Value *> args;

  const BrigOperandArgumentList *oArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 0));
  for (unsigned i = 0; i < oArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(oArgList, i);
    args.push_back(state.lookupSymbol(symbol));
  }

  const BrigOperandArgumentList *iArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 2));
  for (unsigned i = 0; i < iArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(iArgList, i);
    args.push_back(state.lookupSymbol(symbol));
  }

  const BrigOperandBase *brigFun = helper.getOperand(inst, 1);
  llvm::Value *rawFun = getOperand(B, brigFun, helper, state);
  llvm::Value *fun = decodeFunPacking(B, rawFun, args);
  llvm::CallInst::Create(fun, args, "", &B);
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunState &state) {
  llvm::LLVMContext &C = B.getContext();

  insertDebugCallback(B, inst, helper, state);

  if (inst->opcode == BRIG_OPCODE_RET) {
    llvm::ReturnInst::Create(C, &B);
  } else if (helper.isDirectBranchInst(inst)) {
    runOnDirectBranchInst(B, inst, helper, state);
  } else if (helper.isIndirectBranchInst(inst)) {
    runOnIndirectBranchInst(B, inst, helper, state);
  } else if (inst->opcode == BRIG_OPCODE_CALL) {
    runOnCallInst(B, inst, helper, state);
  } else {
    runOnComplexInst(B, inst, helper, state);
  }
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB,
                    const FunState &state) {

  llvm::LLVMContext &C = F.getContext();
  BrigInstHelper helper = CB.getInstHelper();
  FunState::CBIt it = state.cbMap.find(CB.getOffset());
  assert(it != state.cbMap.end() && "Missing CB");
  llvm::BasicBlock *bb = it->second;

  for (inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst, helper, state);

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
    if (it != state.cbMap.end()) llvm::BranchInst::Create(it->second, bb);
    else llvm::ReturnInst::Create(C, bb);
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

  llvm::Value *load = new llvm::LoadInst(gep, "", bb);
  return new llvm::BitCastInst(load, paramTy, "", bb);
}


// We create a trampoline with the function signature:
// void fun(char **argv)
// The real function arguments are encoded in the argv array.
static void makeKernelTrampoline(llvm::Function *fun, llvm::StringRef name) {

  llvm::LLVMContext &C = fun->getContext();
  llvm::Module *M = fun->getParent();

  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  llvm::Type *trampArgs[] = {
    llvm::Type::getInt8Ty(C)->getPointerTo()->getPointerTo()
  };
  llvm::FunctionType *trampFunTy =
    llvm::FunctionType::get(voidTy, trampArgs, false);
  llvm::GlobalValue::LinkageTypes linkage = fun->getLinkage();

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

  llvm::CallInst::Create(fun, trampParams, "", bb);
  llvm::ReturnInst::Create(C, bb);
}

static void runOnFunction(llvm::Module &M, const BrigFunction &F,
                          FunMap &funMap, SymbolMap &symbolMap,
                          Callback callback, CallbackData cbd) {

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

  if (F.isKernel())
    name = "kernel." + name;

  llvm::Function *fun = llvm::Function::Create(funTy, linkage, name, &M);
  funMap[F.getOffset()] = fun;

  BrigSymbol brigArg = F.arg_begin();
  llvm::Function::arg_iterator llvmArg = fun->arg_begin();
  llvm::Function::arg_iterator E = fun->arg_end();
  for (; llvmArg != E; ++brigArg, ++llvmArg) {
    llvmArg->setName(getStringRef(brigArg.getName()));
    symbolMap[brigArg.getAddr()] = llvmArg;
  }

  if (F.isDeclaration()) return;

  const FunState state =
    FunState::Create(funMap, symbolMap, F, fun, callback, cbd);
  for (BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, state);
  }

  if (F.isKernel())
    makeKernelTrampoline(fun, nameRef);
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
  return llvm::ConstantFP::get(C, llvm::APFloat(*array));
}

template<>
llvm::Constant *runOnInitializer<double>(llvm::LLVMContext &C,
                                         llvm::Type *type,
                                         const double *array) {
  return llvm::ConstantFP::get(C, llvm::APFloat(*array));
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
                        SymbolMap &symbolMap) {
  llvm::LLVMContext &C = M.getContext();
  llvm::Type *type = runOnType(C, S);
  bool isConst = S.isConst();
  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(S.getLinkage());
  llvm::Twine name(getStringRef(S.getName()));

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

  symbolMap[S.getAddr()] =
    new llvm::GlobalVariable(M, type, isConst, linkage, init, name);
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

      return dic;
    }
  }

  return NULL;
}

BrigProgram GenLLVM::getLLVMModule(const BrigModule &M,
                                   Callback callback,
                                   CallbackData cbd) {

  if (!M.isValid()) return NULL;

  llvm::LLVMContext *C = new llvm::LLVMContext();
  llvm::Module *mod = new llvm::Module("BRIG", *C);

  insertGPUStateTy(*C);
  insertSetThreadInfo(*C, mod);

  SymbolMap symbolMap;
  for (BrigSymbol symbol = M.global_begin(),
        E = M.global_end(); symbol != E; ++symbol) {
    runOnGlobal(*mod, symbol, symbolMap);
  }

  FunMap funMap;
  for (BrigFunction fun = M.begin(), E = M.end(); fun != E; ++fun) {
    runOnFunction(*mod, fun, funMap, symbolMap, callback, cbd);
  }

  llvm::DIContext *debugInfo = runOnDebugInfo(M);

  return BrigProgram(mod, debugInfo);
}

std::string GenLLVM::getLLVMString(const BrigModule &M,
                                   Callback callback,
                                   CallbackData cbd) {
  BrigProgram BP = getLLVMModule(M, callback, cbd);
  if (!BP) return "";

  std::string output;
  llvm::raw_string_ostream ros(output);
  BP->print(ros, NULL);

  return output;
}

}
}
