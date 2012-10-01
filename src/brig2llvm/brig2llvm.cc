#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "brig.h"
#include "brig_buffer.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_function.h"
#include "brig_symbol.h"
#include "brig_control_block.h"
#include "brig_inst_helper.h"
#include <iostream>
namespace hsa{
namespace brig{

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

static void gen_GPU_states(llvm::LLVMContext &C) {
  llvm::StructType *c_reg_type =
    createSOAType(C, llvm::Type::getInt1Ty(C), "c_regs", 8);
  llvm::StructType *s_reg_type =
    createSOAType(C, llvm::Type::getInt32Ty(C), "s_regs", 32);
  llvm::StructType *d_reg_type =
    createSOAType(C, llvm::Type::getInt64Ty(C), "d_regs", 32);
  llvm::StructType *q_reg_type =
    createSOAType(C, llvm::Type::getIntNTy(C, 128), "q_regs", 8);
  llvm::StructType *pc_reg_type =
    createSOAType(C, llvm::Type::getIntNTy(C, 32), "pc_regs", 3);
  llvm::Type *tv1[] = { c_reg_type,
                        s_reg_type,
                        d_reg_type,
                        q_reg_type,
                        pc_reg_type };
  llvm::StructType::create(C, tv1, std::string("struct.regs"), false);
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
    unsigned length = BrigInstHelper::getVectorLength(type);
    return llvm::VectorType::get(getElementTy(C, type), length);
  } else {
    assert(false && "Unimplemented type");
  }
}

static llvm::Type *runOnType(llvm::LLVMContext &C, const BrigSymbol &S) {

  if(S.isArray()) assert(false && "Array types unimplemented");
  if(S.isFlexArray()) assert(false && "Flex array types unimplemented");

  return runOnType(C, S.getType());
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
  assert(offset < 16 && "Invalid offset");
  return offset;
}

typedef std::map<uint32_t, llvm::Function *> FunMap;
typedef std::map<const void *, llvm::Value *> SymbolMap;

struct FunState {
  typedef std::map<uint32_t, llvm::BasicBlock *> CBMap ;
  typedef CBMap::iterator CBIt;

  const FunMap &funMap;
  llvm::Value *regs;
  CBMap cbMap;

  FunState(const FunMap &funMap, SymbolMap &symbolMap,
           const BrigFunction &brigFun, llvm::Function *llvmFun) :
    funMap(funMap), symbolMap(symbolMap) {

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

    for(BrigSymbol local = brigFun.local_begin(),
          E = brigFun.local_end(); local != E; ++local) {
      const char *name = local.getName();
      llvm::Type *type = runOnType(C, local);
      symbolMap[local.getAddr()] = new llvm::AllocaInst(type, name, &entry);
    }
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
};

static llvm::Value *getOperandAddr(llvm::BasicBlock &B,
                                   const BrigOperandBase *op,
                                   const BrigInstHelper &helper,
                                   FunState &state) {

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
  case BrigEOperandFunctionRef:
  case BrigEOperandImmed:
  case BrigEOperandIndirect:
  case BrigEOperandLabelRef:
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
                               FunState &state) {

  llvm::LLVMContext &C = B.getContext();

  if(hasAddr(op)) {
    llvm::Value *valAddr = getOperandAddr(B, op, helper, state);
    llvm::Value *val = new llvm::LoadInst(valAddr, "", false, &B);
    return val;
  }

  if(const BrigOperandLabelRef *label = dyn_cast<BrigOperandLabelRef>(op)) {
    return state.cbMap[label->labeldirective];
  }

  const BrigOperandFunctionRef *func = dyn_cast<BrigOperandFunctionRef>(op);
  if(func) {
    return state.funMap.find(func->fn)->second;
  }

  if(const BrigOperandImmed *immedOp = dyn_cast<BrigOperandImmed>(op)) {
    llvm::Type *type = runOnType(C, BrigDataType(immedOp->type));
    if(type->isIntegerTy(64)) {
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

  assert(false && "Unimplemented");
}

static llvm::Type *getOperandTy(llvm::LLVMContext &C,
                                const inst_iterator inst,
                                unsigned opnum) {

  llvm::Type *destType = runOnType(C, BrigDataType(inst->type));

  if(opnum == 0) return destType;

  if((inst->opcode == BrigLd          && opnum == 2) ||
     (inst->opcode == BrigSt          && opnum == 1) ||
     (inst->opcode == BrigAtomic      && opnum == 1) ||
     (inst->opcode == BrigAtomicNoRet && opnum == 0))
    return destType->getPointerTo();

  if(const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst))
    return runOnType(C, BrigDataType(cmp->sourceType));

  if(const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst))
    return runOnType(C, BrigDataType(cvt->stype));

  if((inst->opcode == BrigShr && opnum == 2) ||
     (inst->opcode == BrigShl && opnum == 2))
    return runOnType(C, Brigu32);

  if((inst->opcode == BrigShuffle && opnum == 3))
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

  if(destTy->isFloatingPointTy())
    return new llvm::BitCastInst(value, destTy, "", &B);

  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(value, false, destTy, false);
  return llvm::CastInst::Create(castOp, value, destTy, "", &B);
}

static llvm::Value *encodePacking(llvm::BasicBlock &B,
                                  llvm::Value *value,
                                  llvm::Type *destTy,
                                  const inst_iterator inst,
                                  const BrigInstHelper &helper) {

  llvm::Type *encodedType = value->getType();

  if(encodedType == destTy)
    return value;

  if(encodedType->isFloatingPointTy() || encodedType->isVectorTy())
    return new llvm::BitCastInst(value, destTy, "", &B);

  bool isSigned = helper.isSignedTy(BrigDataType(inst->type));
  llvm::Instruction::CastOps castOp =
    llvm::CastInst::getCastOpcode(value, isSigned, destTy, isSigned);
  return llvm::CastInst::Create(castOp, value, destTy, "", &B);
}

static
llvm::FunctionType *getInstFunType(const inst_iterator inst,
                                   const std::vector<llvm::Value *> &sources,
                                   llvm::LLVMContext &C) {

  llvm::Type *result = runOnType(C, BrigDataType(inst->type));
  std::vector<llvm::Type *> params;
  for(unsigned i = 0; i < sources.size(); ++i)
    params.push_back(sources[i]->getType());

  return llvm::FunctionType::get(result, params, false);
}

static void runOnComplexInst(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             FunState &state) {

  unsigned operand = 0;

  // Skip the width parameter for loads.
  if(inst->opcode == BrigLd) ++operand;

  const BrigOperandBase *brigDest = NULL;
  if(BrigInstHelper::hasDest(inst))
    brigDest = helper.getOperand(inst, operand++);

  std::vector<llvm::Value *> sources;
  for(; operand < 5 && inst->o_operands[operand]; ++operand) {
    const BrigOperandBase *brigSrc = helper.getOperand(inst, operand);
    llvm::Value *srcRaw = getOperand(B, brigSrc, helper, state);
    llvm::Value *srcVal = decodePacking(B, srcRaw, operand, inst);
    sources.push_back(srcVal);
  }

  llvm::Module *M = B.getParent()->getParent();
  std::string name = BrigInstHelper::getInstName(inst);
  llvm::FunctionType *funTy = getInstFunType(inst, sources, B.getContext());
  llvm::Function *instFun =
    llvm::cast<llvm::Function>(M->getOrInsertFunction(name, funTy));
  llvm::Value *resultRaw = llvm::CallInst::Create(instFun, sources, "", &B);

  if(brigDest) {
    llvm::Value *destAddr = getOperandAddr(B, brigDest, helper, state);
    llvm::PointerType *destPtrTy = cast<llvm::PointerType>(destAddr->getType());
    llvm::Type *destTy = destPtrTy->getElementType();
    llvm::Value *resultVal = encodePacking(B, resultRaw, destTy, inst, helper);
    new llvm::StoreInst(resultVal, destAddr, &B);
  }
}

static void runOnBranchInst(llvm::BasicBlock &B,
                            const inst_iterator inst,
                            const BrigInstHelper &helper,
                            FunState &state) {

  // The width of the branch is not necessary for a functional simulator.
  // Similarly, we can ignore the list of possible branch targets for an
  // indirect branch. In debug mode, we should check for and log branchs outside
  // the target width and branch divergence beyond the limit given by width.

  unsigned targetOpNum = inst->opcode == BrigBrn ? 1 : 2;

  const BrigOperandBase *target = helper.getOperand(inst, targetOpNum);
  llvm::BasicBlock *targetBB =
    llvm::cast<llvm::BasicBlock>(getOperand(B, target, helper, state));

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

static void runOnCallInst(llvm::BasicBlock &B,
                          const inst_iterator inst,
                          const BrigInstHelper &helper,
                          FunState &state) {

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
  llvm::Value *func = getOperand(B, brigFun, helper, state);
  llvm::CallInst::Create(func, args, "", &B);
}

static void runOnInstruction(llvm::BasicBlock &B,
                             const inst_iterator inst,
                             const BrigInstHelper &helper,
                             FunState &state) {
  llvm::LLVMContext &C = B.getContext();

  if(inst->opcode == BrigRet) {
    llvm::ReturnInst::Create(C, &B);
  } else if(helper.isBranchInst(inst)) {
    runOnBranchInst(B, inst, helper, state);
  } else if(inst->opcode == BrigCall) {
    runOnCallInst(B, inst, helper, state);
  } else {
    runOnComplexInst(B, inst, helper, state);
  }
}

static void runOnCB(llvm::Function &F, const BrigControlBlock &CB,
                    FunState &state) {

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
  if(!llvm::isa<llvm::TerminatorInst>(&bb->back())) {
    ++it;
    if(it != state.cbMap.end()) llvm::BranchInst::Create(it->second, bb);
    else llvm::ReturnInst::Create(C, bb);
  }
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

  FunState state(funMap, symbolMap, F, fun);
  for(BrigControlBlock cb = F.begin(), E = F.end(); cb != E; ++cb) {
    runOnCB(*fun, cb, state);
  }
}

static llvm::Type *getType(llvm::LLVMContext &C,
                           const BrigSymbol &S) {
  BrigDataType brigType = S.getType();
  llvm::Type *llvmType = runOnType(C, brigType);
  if(!S.isArray()) {
    return llvmType;
  } else {
    return llvm::ArrayType::get(llvmType, S.getArrayDim());
  }
}

template<class T>
static llvm::Constant *runOnInitializer(llvm::LLVMContext &C,
                                        const BrigSymbol &S) {
  const T *array = S.getInit<T>();
  uint32_t size = std::max(1U, S.getArrayDim());
  llvm::ArrayRef<T> arrayRef(array, size);
  return llvm::ConstantDataArray::get(C, arrayRef);
}

static void runOnGlobal(llvm::Module &M, const BrigSymbol &S,
                        SymbolMap &symbolMap) {
  llvm::LLVMContext &C = M.getContext();
  llvm::Type *type = getType(C, S);
  bool isConst = S.isConst();
  llvm::GlobalValue::LinkageTypes linkage = runOnLinkage(S.getLinkage());
  llvm::Twine name(S.getName());

  llvm::Constant *init = NULL;
  if(S.hasInitializer()) {
    llvm::Type *elementTy = type->getArrayElementType();
    if(elementTy->isIntegerTy(1) || elementTy->isIntegerTy(8)) {
      init = runOnInitializer<uint8_t>(C, S);
    } else if(elementTy->isIntegerTy(16)) {
      init = runOnInitializer<uint16_t>(C, S);
    } else if(elementTy->isIntegerTy(32)) {
      init = runOnInitializer<uint32_t>(C, S);
    } else if(elementTy->isIntegerTy(64)) {
      init = runOnInitializer<uint64_t>(C, S);
    } else if(elementTy->isFloatTy()) {
      init = runOnInitializer<float>(C, S);
    } else if(elementTy->isDoubleTy()) {
      init = runOnInitializer<double>(C, S);
    } else {
      assert(false && "Unimplemented");
    }
  }

  symbolMap[S.getAddr()] =
    new llvm::GlobalVariable(M, type, isConst, linkage, init, name);
}

GenLLVM::GenLLVM(const StringBuffer &strings,
                 const Buffer &directives,
                 const Buffer &code,
                 const Buffer &operands,
                 const Buffer &debug) :
  mod_(strings, directives, code, operands, debug, &llvm::errs()),
  brig_frontend_(NULL), output_() {
}

GenLLVM::GenLLVM(const BrigReader &reader) :
  mod_(reader, &llvm::errs()),
  brig_frontend_(NULL), output_() {
}

void GenLLVM::operator()(void) {

  C_ = new llvm::LLVMContext();
  brig_frontend_ = new llvm::Module("BRIG", *C_);

  assert(mod_.isValid());

  gen_GPU_states(*C_);

  SymbolMap symbolMap;
  for(BrigSymbol symbol = mod_.global_begin(),
        E = mod_.global_end(); symbol != E; ++symbol) {
    runOnGlobal(*brig_frontend_, symbol, symbolMap);
  }

  FunMap funMap;
  for(BrigFunction fun = mod_.begin(), E = mod_.end(); fun != E; ++fun) {
    runOnFunction(*brig_frontend_, fun, funMap, symbolMap);
  }

  llvm::verifyModule(*brig_frontend_);
}

const std::string &GenLLVM::str(void) {
  if(!output_.size()) {
    llvm::raw_string_ostream ros(output_);
    brig_frontend_->print(ros, NULL);
  }
  return output_;
}

}
}
