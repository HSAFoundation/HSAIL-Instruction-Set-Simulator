#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_function.h"
#include "brig_symbol.h"
#include "brig_control_block.h"
#include "brig_inst_helper.h"

#include "llvm/Attributes.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Instructions.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"

namespace hsa{
namespace brig{

void BrigProgram::delModule(llvm::Module *M) {
  if(!M) return;
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
  llvm::StructType *pc_reg_type =
    createSOAType(C, llvm::Type::getIntNTy(C, 32), "pc_regs", 3);
  llvm::Type *tv1[] = { c_reg_type,
                        s_reg_type,
                        d_reg_type,
                        q_reg_type,
                        pc_reg_type };
  llvm::StructType::create(C, tv1, std::string("struct.regs"), false);
}

static void insertSetThreadInfo(llvm::LLVMContext &C, llvm::Module *M) {
  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  llvm::Type *args[] = { llvm::Type::getInt8PtrTy(C) };
  llvm::FunctionType *setThreadInfoTy =
    llvm::FunctionType::get(voidTy, args, false);
  M->getOrInsertFunction("__setThreadInfo", setThreadInfoTy);
}

static llvm::Type *getElementTy(llvm::LLVMContext &C, BrigDataType type) {

  assert(BrigInstHelper::isVectorTy(type) &&
         "Cannot get element of non-vector types");

  const bool isFloat = BrigInstHelper::isFloatTy(type);

  if(type >= Brigu8x4 && type <= Brigs8x16) {
    return llvm::Type::getInt8Ty(C);
  } else if(type >= Brigu16x2 && type <= Brigf16x8) {
    if(isFloat) return llvm::Type::getHalfTy(C);
    else return llvm::Type::getInt16Ty(C);
  } else if(type >= Brigu32x2 && type <= Brigf32x4) {
    if(isFloat) return llvm::Type::getFloatTy(C);
    else return llvm::Type::getInt32Ty(C);
  } else if(type >= Brigu64x2 && type <= Brigf64x2) {
    if(isFloat) return llvm::Type::getDoubleTy(C);
    else return llvm::Type::getInt64Ty(C);
  } else {
    assert(false && "Unknown type");
  }
}

static llvm::Type *runOnType(llvm::LLVMContext &C, BrigDataType type) {
  if(type == Brigb1) {
    return llvm::Type::getInt1Ty(C);
  } else if(type == Brigs8  || type == Brigu8  || type == Brigb8)  {
    return llvm::Type::getInt8Ty(C);
  } else if(type == Brigs16 || type == Brigu16 || type == Brigb16) {
    return llvm::Type::getInt16Ty(C);
  } else if(type == Brigs32 || type == Brigu32 || type == Brigb32) {
    return llvm::Type::getInt32Ty(C);
  } else if(type == Brigs64 || type == Brigu64 || type == Brigb64) {
    return llvm::Type::getInt64Ty(C);
  } else if(type == Brigf32) {
    return llvm::Type::getFloatTy(C);
  } else if(type == Brigf64) {
    return llvm::Type::getDoubleTy(C);
  } else if(type == Brigb128) {
    return llvm::Type::getIntNTy(C, 128);
  } else if(BrigInstHelper::isVectorTy(type)) {

    llvm::Type *base = getElementTy(C, type);
    unsigned length = BrigInstHelper::getVectorLength(type);

    if(base->isIntegerTy() || sizeof(void *) == 4) {
      unsigned bitWidth = base->getScalarSizeInBits() * length;
      return llvm::Type::getIntNTy(C, bitWidth);
    }

    return llvm::VectorType::get(base, length);
  } else {
    assert(false && "Unimplemented type");
  }
}

static llvm::Type *runOnType(llvm::LLVMContext &C,
                             const BrigSymbol &S) {
  BrigDataType brigType = S.getType();
  llvm::Type *llvmType = runOnType(C, brigType);
  if(!S.isArray()) {
    return llvmType;
  } else if(S.isFlexArray()) {
    return llvmType->getPointerTo(0);
  } else {
    return llvm::ArrayType::get(llvmType, S.getArrayDim());
  }
}

static llvm::GlobalValue::LinkageTypes runOnLinkage(BrigAttribute link) {
  if(link == BrigExtern || link == BrigNone) {
    return llvm::GlobalValue::ExternalLinkage;
  } else if(link == BrigStatic) {
    return llvm::GlobalValue::InternalLinkage;
  }

  assert(false && "Invalid attribute");
}

static unsigned getRegField(const char *name) {
  if(name[1] == 'c') return 0;
  if(name[1] == 's') return 1;
  if(name[1] == 'd') return 2;
  if(name[1] == 'q') return 3;

  assert(false && "Unknown reg type");
}

static unsigned getRegOffset(const char *name) {
  char *endptr;
  unsigned offset = (unsigned) strtol(name + 2, &endptr, 10);
  assert(!*endptr && "Invalid field");
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

  static const FunState Create(const FunMap &funMap,
                               SymbolMap &symbolMap,
                               const BrigFunction &brigFun,
                               llvm::Function *llvmFun) {
    return FunState(funMap, symbolMap, brigFun, llvmFun);
  }

  template<class T>
  llvm::Value *lookupSymbol(const T *symbol) const {
    (void) symbol->s;
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
           const BrigFunction &brigFun, llvm::Function *llvmFun) :
    funMap(funMap),
    symbolMap(symbolMap) {

    llvm::LLVMContext &C = llvmFun->getContext();
    const BrigControlBlock E = brigFun.end();
    for(BrigControlBlock cb = brigFun.begin(); cb != E; ++cb) {

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

    for(BrigSymbol local = brigFun.local_begin(),
          E = brigFun.local_end(); local != E; ++local) {
      const char *name = local.getName();
      llvm::Type *type = runOnType(C, local);
      symbolMap[local.getAddr()] = new llvm::AllocaInst(type, name, &entry);
    }
  }
};

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   const FunState &state) {

  llvm::LLVMContext &C = B.getContext();

  if(const BrigOperandReg *regOp = dyn_cast<BrigOperandReg>(op)) {
    const char *name = helper.getName(regOp);
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

  assert(false && "Unimplemented operand");
}

static bool hasAddr(const BrigOperandBase *op) {
  BrigOperandKinds kind = BrigOperandKinds(op->kind);
  switch(kind) {
  case BrigEOperandAddress:
  case BrigEOperandCompound:
  case BrigEOperandFunctionRef:
  case BrigEOperandImmed:
  case BrigEOperandIndirect:
  case BrigEOperandLabelRef:
  case BrigEOperandRegV2:
  case BrigEOperandRegV4:
  case BrigEOperandWaveSz:
    return false;
  case BrigEOperandReg:
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

  const size_t numElements = sizeof(op->regs) / sizeof(*op->regs);
  llvm::Type *elementTy = runOnType(C, BrigDataType(op->type));
  llvm::Type *vecTy = llvm::VectorType::get(elementTy, numElements);

  llvm::Value *vec = llvm::UndefValue::get(vecTy);
  for(unsigned i = 0; i < numElements; ++i) {
    const BrigOperandBase *reg = helper.getReg(op, i);
    llvm::Value *element = getOperand(B, reg, helper, state);
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

  if(hasAddr(op)) {
    llvm::Value *valAddr = getOperandAddr(B, op, helper, state);
    llvm::Value *val = new llvm::LoadInst(valAddr, "", false, &B);
    return val;
  }

  if(const BrigOperandLabelRef *label = dyn_cast<BrigOperandLabelRef>(op)) {
    llvm::Type *int32Ty = llvm::Type::getInt32Ty(C);
    return llvm::ConstantInt::get(int32Ty, label->labeldirective);
  }

  if(const BrigOperandFunctionRef *func = dyn_cast<BrigOperandFunctionRef>(op))
    return state.funMap.find(func->fn)->second;

  if(const BrigOperandImmed *immedOp = dyn_cast<BrigOperandImmed>(op)) {
    llvm::Type *type = runOnType(C, BrigDataType(immedOp->type));
    if(type->isIntegerTy(128)) {
      llvm::APInt bigInt(128, immedOp->bits.l);
      return llvm::ConstantInt::get(type, bigInt);
    } else if(type->isIntegerTy(64)) {
      return llvm::ConstantInt::get(type, immedOp->bits.l[0]);
    } else if(type->isIntegerTy(32)) {
      return llvm::ConstantInt::get(type, immedOp->bits.u);
    } else if(type->isIntegerTy(16)) {
      return llvm::ConstantInt::get(type, immedOp->bits.h);
    } else if(type->isIntegerTy(8)) {
      return llvm::ConstantInt::get(type, immedOp->bits.c);
    } else if(type->isIntegerTy(1)) {
      return llvm::ConstantInt::get(type, immedOp->bits.c);
    }
    assert(false && "Illegal immediate type");
  }

  if(const BrigOperandAddress *adderOp = dyn_cast<BrigOperandAddress>(op)) {
    const BrigDirectiveSymbol *symbol =
      cast<BrigDirectiveSymbol>(helper.getDirective(adderOp->directive));
    llvm::Value *address = state.lookupSymbol(symbol);
    assert(address && "Symbol not found");
    return address;
  }

  if(const BrigOperandIndirect *indOp = dyn_cast<BrigOperandIndirect>(op)) {
    llvm::Type *type = runOnType(C, BrigDataType(indOp->type));
    const BrigOperandBase *reg = helper.getReg(indOp);
    llvm::Value *base = reg ?
      getOperand(B, reg, helper, state) :
      llvm::ConstantInt::get(type, 0ULL);
    llvm::Value *offset = llvm::ConstantInt::get(type, indOp->offset);
    return llvm::BinaryOperator::Create(llvm::BinaryOperator::Add,
                                        base, offset, "", &B);
  }

  if(const BrigOperandCompound *comOp = dyn_cast<BrigOperandCompound>(op)) {
    llvm::Type *type = runOnType(C, BrigDataType(comOp->type));
    const BrigOperandBase *addrOp = helper.getOperand(comOp->name);
    llvm::Value *addr = getOperand(B, addrOp, helper, state);
    if(!comOp->reg && !comOp->offset)
      return addr;

    llvm::Value *adderInt = new llvm::PtrToIntInst(addr, type, "", &B);

    const BrigOperandBase *reg = helper.getReg(comOp);
    llvm::Value *base = reg ?
      getOperand(B, reg, helper, state) :
      llvm::ConstantInt::get(type, 0ULL);

    llvm::Value *offset = llvm::ConstantInt::get(type, comOp->offset);

    llvm::Value *index =
      llvm::BinaryOperator::Create(llvm::BinaryOperator::Add,
                                   base, offset, "", &B);
    return llvm::BinaryOperator::Create(llvm::BinaryOperator::Add,
                                        adderInt, index, "", &B);
  }

  if(const BrigOperandRegV2 *v2Op = dyn_cast<BrigOperandRegV2>(op))
    return getVectorOperand(B, v2Op, helper, state);

  if(const BrigOperandRegV4 *v4Op = dyn_cast<BrigOperandRegV4>(op))
    return getVectorOperand(B, v4Op, helper, state);

  if(isa<BrigOperandWaveSz>(op)) {
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

  llvm::Type *destType = runOnType(C, BrigDataType(inst->type));

  if(opnum == 0) return destType;

  if((inst->opcode == BrigLd          && opnum == 2) ||
     (inst->opcode == BrigSt          && opnum == 1) ||
     (inst->opcode == BrigAtomic      && opnum == 1))
    return destType->getPointerTo();

  if(const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst))
    return runOnType(C, BrigDataType(cmp->sourceType));

  if(const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst))
    return runOnType(C, BrigDataType(cvt->stype));

  if((inst->opcode == BrigShr && opnum == 2) ||
     (inst->opcode == BrigShl && opnum == 2))
    return runOnType(C, Brigu32);

  if((inst->opcode == BrigShuffle && opnum == 3) ||
     (inst->opcode == BrigMovdHi  && opnum == 1) ||
     (inst->opcode == BrigMovdLo  && opnum == 1) ||
     (inst->opcode == BrigMovsHi  && opnum == 1) ||
     (inst->opcode == BrigMovdLo  && opnum == 1))
    return runOnType(C, Brigu64);

  return destType;
}

static llvm::Value *decodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  unsigned opnum,
                                  const inst_iterator inst) {

  llvm::LLVMContext &C = B.getContext();
  llvm::Type *destTy = getOperandTy(C, inst, opnum);

  if(destTy == value->getType())
    return value;

  if(destTy->isFloatingPointTy()) {
    const unsigned typeSize = value->getType()->getPrimitiveSizeInBits();
    const unsigned destTySize = destTy->getPrimitiveSizeInBits();

    if(typeSize == destTySize) {
      return new llvm::BitCastInst(value, destTy, "", &B);

    } else if(typeSize == 64) {
      llvm::Type *doubleTy = llvm::Type::getDoubleTy(C);
      llvm::Value *result = new llvm::BitCastInst(value, doubleTy, "", &B);
      return new llvm::FPTruncInst(result, destTy, "", &B);

    } else if(typeSize == 32) {
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

  if(encodedTy == destTy)
    return value;

  if(encodedTy->isFloatingPointTy() || encodedTy->isVectorTy()) {

    llvm::LLVMContext &C = B.getContext();
    const unsigned encodedTySize = encodedTy->getPrimitiveSizeInBits();
    const unsigned destTySize = destTy->getPrimitiveSizeInBits();

    if(encodedTySize == destTySize) {
      return new llvm::BitCastInst(value, destTy, "", &B);

    } else if(encodedTySize == 64 && destTySize == 32) {
      llvm::Type *floatTy = llvm::Type::getFloatTy(C);
      llvm::Value *result = new llvm::FPTruncInst(value, floatTy, "", &B);
      return encodePacking(B, result, destTy, inst, helper);

    } else if(encodedTySize == 32 && destTySize == 64) {
      llvm::Type *doubleTy = llvm::Type::getDoubleTy(C);
      llvm::Value *result = new llvm::FPExtInst(value, doubleTy, "", &B);
      return encodePacking(B, result, destTy, inst, helper);

    } else {
      assert(false && "Unimplemented");
    }
  }

  bool isSigned = helper.isSignedTy(BrigDataType(inst->type));
  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(value, isSigned, destTy, isSigned);
  return llvm::CastInst::Create(castOp, value, destTy, "", &B);
}

static bool isSRet(const inst_iterator inst) {
  return sizeof(void *) == 4 &&
    BrigInstHelper::hasDest(inst) &&
    BrigInstHelper::isVectorTy(BrigDataType(inst->type));
}

static llvm::Function *getInstFun(const inst_iterator inst,
                                  const std::vector<llvm::Value *> &sources,
                                  llvm::Module *M) {

  bool sret = isSRet(inst);
  bool isStore = inst->opcode == BrigSt;
  llvm::LLVMContext &C = M->getContext();

  llvm::Type *result = sret || isStore ?
    llvm::Type::getVoidTy(C) :
    runOnType(C, BrigDataType(inst->type));
  std::vector<llvm::Type *> params;
  for(unsigned i = 0; i < sources.size(); ++i)
    params.push_back(sources[i]->getType());

  llvm::FunctionType *instFunTy =
    llvm::FunctionType::get(result, params, false);

  std::string name = BrigInstHelper::getInstName(inst);
  llvm::Function *instFun =
    llvm::cast<llvm::Function>(M->getOrInsertFunction(name, instFunTy));

  if(sret) {
    llvm::AttrBuilder AB;
    AB.addAttribute(llvm::Attributes::StructRet);
    AB.addAttribute(llvm::Attributes::NoAlias);
    AB.addAttribute(llvm::Attributes::NoCapture);
    llvm::Attributes attrs = llvm::Attributes::get(C, AB);
    instFun->addAttribute(1, attrs);
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
  const BrigAluModifier *aluMod = BrigInstHelper::getAluModifier(inst);
  assert(aluMod->floatOrInt && "Integer rounding illegal on float arithmetic");
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

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunState &state) {

  unsigned operand = 0;

  bool ftz = BrigInstHelper::isFtz(inst);
  if(ftz) insertEnableFtz(B);

  bool rounding = BrigInstHelper::hasRoundingMode(inst);
  if(rounding) insertSetRoundingMode(B, inst);

  bool sret = isSRet(inst);

  // Skip the width parameter for loads.
  if(inst->opcode == BrigLd) ++operand;

  std::vector<llvm::Value *> sources;

  llvm::Value *destAddr = NULL;
  if(BrigInstHelper::hasDest(inst)) {
    const BrigOperandBase *brigDest = helper.getOperand(inst, operand++);
    destAddr = getOperandAddr(B, brigDest, helper, state);
    if(sret) sources.push_back(destAddr);
  }

  for(; operand < 5 && inst->o_operands[operand]; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);
    llvm::Value *srcRaw = getOperand(B, brigSrc, helper, state);
    llvm::Value *srcVal = decodePacking(B, srcRaw, operand, inst);
    sources.push_back(srcVal);
  }

  llvm::Module *M = B.getParent()->getParent();
  llvm::Function *instFun = getInstFun(inst, sources, M);
  llvm::Value *resultRaw = llvm::CallInst::Create(instFun, sources, "", &B);

  if(destAddr && !sret) {
    llvm::PointerType *destPtrTy =
      llvm::cast<llvm::PointerType>(destAddr->getType());
    llvm::Type *destTy = destPtrTy->getElementType();
    llvm::Value *resultVal = encodePacking(B, resultRaw, destTy, inst, helper);
    new llvm::StoreInst(resultVal, destAddr, &B);
  }

  if(rounding) insertRestoreRoundingMode(B);

  if(ftz) insertDisableFtz(B);
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

  if(inst->opcode == BrigCbr) {
    const BrigOperandBase *pred = helper.getOperand(inst, 1);
    llvm::Value *predVal = getOperand(B, pred, helper, state);
    llvm::BranchInst::Create(targetBB, NULL, predVal, &B);
    return;
  }

  if(inst->opcode == BrigBrn) {
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
  if(inst->opcode == BrigCbr) {
    launchBB = llvm::BasicBlock::Create(C, B.getName() + ".launch", F);
    const BrigOperandBase *pred = helper.getOperand(inst, 1);
    llvm::Value *predVal = getOperand(B, pred, helper, state);
    llvm::BranchInst::Create(launchBB, NULL, predVal, &B);
  }

  llvm::IntegerType *labelTy = llvm::cast<llvm::IntegerType>(targetBB->getType());
  const FunState::CBMap &cbMap = state.cbMap;
  llvm::SwitchInst *launchInst =
    llvm::SwitchInst::Create(targetBB, &B, cbMap.size(), launchBB);
  for(FunState::CBIt cb = cbMap.begin(), E = cbMap.end(); cb != E; ++cb) {
    llvm::BasicBlock *dest = cb->second;
    llvm::ConstantInt *label = llvm::ConstantInt::get(labelTy, cb->first);
    if(dest != &F->getEntryBlock())
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
  for(unsigned i = 0; i < args.size(); ++i) {
    params.push_back(args[i]->getType());
  }

  llvm::Type *funTy = llvm::FunctionType::get(result, params, false);
  llvm::Type *funPtrTy = funTy->getPointerTo(0);
  if(funPtrTy == rawFun->getType()) return rawFun;

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
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 1));
  for(unsigned i = 0; i < oArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(oArgList, i);
    args.push_back(state.lookupSymbol(symbol));
  }

  const BrigOperandArgumentList *iArgList =
    cast<BrigOperandArgumentList>(helper.getOperand(inst, 3));
  for(unsigned i = 0; i < iArgList->elementCount; ++i) {
    const BrigSymbol symbol = helper.getArgument(iArgList, i);
    args.push_back(state.lookupSymbol(symbol));
  }

  const BrigOperandBase *brigFun = helper.getOperand(inst, 2);
  llvm::Value *rawFun = getOperand(B, brigFun, helper, state);
  llvm::Value *fun = decodeFunPacking(B, rawFun, args);
  llvm::CallInst::Create(fun, args, "", &B);
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             const FunState &state) {
  llvm::LLVMContext &C = B.getContext();

  if(inst->opcode == BrigRet) {
    llvm::ReturnInst::Create(C, &B);
  } else if(helper.isDirectBranchInst(inst)) {
    runOnDirectBranchInst(B, inst, helper, state);
  } else if(helper.isIndirectBranchInst(inst)) {
    runOnIndirectBranchInst(B, inst, helper, state);
  } else if(inst->opcode == BrigCall) {
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

  for(inst_iterator inst = CB.begin(), E = CB.end(); inst != E; ++inst) {
    runOnInstruction(*bb, inst, helper, state);

    // Create a new basic block after every conditional branch to handle the
    // fall-through path
    llvm::BranchInst *branch = llvm::dyn_cast<llvm::BranchInst>(&bb->back());
    if(branch && branch->isConditional()) {
      bb = llvm::BasicBlock::Create(C, bb->getName() + ".succ", &F);
      branch->setSuccessor(1, bb);
    }
  }

  // Fall through to the next control block
  if(bb->empty() || !llvm::isa<llvm::TerminatorInst>(&bb->back())) {
    ++it;
    if(it != state.cbMap.end()) llvm::BranchInst::Create(it->second, bb);
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
static void makeKernelTrampoline(llvm::Function *fun, const char *name) {

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
  for(unsigned i = 0; i < fun->arg_size(); ++i) {
    llvm::Type *paramTy = funTy->getParamType(i);
    trampParams.push_back(getParameter(bb, argArray, paramTy, i + 1));
  }

  llvm::CallInst::Create(fun, trampParams, "", bb);
  llvm::ReturnInst::Create(C, bb);
}

static void runOnFunction(llvm::Module &M, const BrigFunction &F,
                          FunMap &funMap, SymbolMap &symbolMap) {

  llvm::LLVMContext &C = M.getContext();

  llvm::Type *voidTy = llvm::Type::getVoidTy(C);
  std::vector<llvm::Type *> argVec;
  for(BrigSymbol arg = F.arg_begin(), E = F.arg_end(); arg != E; ++arg) {
    argVec.push_back(runOnType(C, arg)->getPointerTo(0));
  }
  llvm::ArrayRef<llvm::Type *> args(argVec);
  llvm::FunctionType *funTy = llvm::FunctionType::get(voidTy, args, false);

  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(F.getLinkage());
  llvm::Twine name(F.getName());

  if(F.isKernel())
    name = "kernel." + name;

  llvm::Function *fun = llvm::Function::Create(funTy, linkage, name, &M);
  funMap[F.getOffset()] = fun;

  BrigSymbol brigArg = F.arg_begin();
  llvm::Function::arg_iterator llvmArg = fun->arg_begin();
  llvm::Function::arg_iterator E = fun->arg_end();
  for(; llvmArg != E; ++brigArg, ++llvmArg) {
    llvmArg->setName(brigArg.getName());
    symbolMap[brigArg.getAddr()] = llvmArg;
  }

  if(F.isDeclaration()) return;

  const FunState state = FunState::Create(funMap, symbolMap, F, fun);
  for(BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, state);
  }

  if(F.isKernel())
    makeKernelTrampoline(fun, F.getName());
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
  uint32_t size = std::max(1U, S.getArrayDim());
  if(S.isArray()) {
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
  llvm::Twine name(S.getName());

  llvm::Constant *init = NULL;
  if(S.hasInitializer()) {
    llvm::Type *elementTy = llvm::isa<llvm::SequentialType>(type) ?
      type->getArrayElementType() : type;
    if(elementTy->isIntegerTy(1) || elementTy->isIntegerTy(8)) {
      init = runOnInitializer<uint8_t>(C, elementTy, S);
    } else if(elementTy->isIntegerTy(16)) {
      init = runOnInitializer<uint16_t>(C, elementTy, S);
    } else if(elementTy->isIntegerTy(32)) {
      init = runOnInitializer<uint32_t>(C, elementTy, S);
    } else if(elementTy->isIntegerTy(64)) {
      init = runOnInitializer<uint64_t>(C, elementTy, S);
    } else if(elementTy->isFloatTy()) {
      init = runOnInitializer<float>(C, elementTy, S);
    } else if(elementTy->isDoubleTy()) {
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

BrigProgram GenLLVM::getLLVMModule(const BrigModule &M) {

  if(!M.isValid()) return NULL;

  llvm::LLVMContext *C = new llvm::LLVMContext();
  llvm::Module *mod = new llvm::Module("BRIG", *C);

  insertGPUStateTy(*C);
  insertSetThreadInfo(*C, mod);

  SymbolMap symbolMap;
  for(BrigSymbol symbol = M.global_begin(),
        E = M.global_end(); symbol != E; ++symbol) {
    runOnGlobal(*mod, symbol, symbolMap);
  }

  FunMap funMap;
  for(BrigFunction fun = M.begin(), E = M.end(); fun != E; ++fun) {
    runOnFunction(*mod, fun, funMap, symbolMap);
  }

  return BrigProgram(mod);
}

std::string GenLLVM::getLLVMString(const BrigModule &M) {
  BrigProgram BP = getLLVMModule(M);
  if(!BP) return "";

  std::string output;
  llvm::raw_string_ostream ros(output);
  BP->print(ros, NULL);

  return output;
}

}
}
