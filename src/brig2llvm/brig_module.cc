//===- brig_module.cc -----------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_module.h"
#include "brig_inst_helper.h"
#include "llvm/Support/raw_ostream.h"
#include <cstring>
#include <set>

namespace hsa {
namespace brig {

#define check(X, Y) check(X, Y, __FILE__, __LINE__, #X)

template<class Message>
bool (BrigModule::check)(bool test, const Message &msg,
                         const char *filename, unsigned lineno,
                         const char *cause) const {
  if (!test && out_)
     (*out_) << filename << ":" << lineno << " " << msg
            << " (" << cause << ")\n";
  return test;
}

bool BrigModule::validate(void) const {
  bool valid = true;
  valid &= validateDirectives();
  valid &= validateCode();
  valid &= validateOperands();
  valid &= validateStrings();
  valid &= validateDebug();
  if (valid) valid &= validateCCode();
  if (valid) valid &= validateInstructions();
  return valid;
}

bool BrigModule::validateDirectives(void) const {
  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

  if (!validateSectionSize(S_.directives, S_.directivesSize))
    return false;

  if (!validate(it)) return false;
  if (!check(it != E, "Empty directive section")) return false;

  // 20.8.22: The BrigDirectiveVersion directive must be the first directive
  // in the .directives section.
  const BrigDirectiveVersion *bdv = dyn_cast<BrigDirectiveVersion>(it);
  if (!check(bdv, "Missing BrigDirectiveVersion")) return false;
  if (!validate(bdv)) return false;

#define caseBrig(Y, X)                                 \
  case Brig ## X::Y ## Kind:                          \
    if (!validate(cast<Brig ## X>(it))) return false;  \
    break

  for (; it != E; ++it) {
    if (!validate(it)) return false;
    switch (it->kind) {
      case BRIG_DIRECTIVE_FUNCTION:
      case BRIG_DIRECTIVE_KERNEL:
        if (!validate(cast<BrigDirectiveExecutable>(it))) return false;
        break;
      case BRIG_DIRECTIVE_VARIABLE:
      case BRIG_DIRECTIVE_IMAGE:
      case BRIG_DIRECTIVE_SAMPLER:
        if (!validate(cast<BrigDirectiveSymbol>(it))) return false;
        break;
      caseBrig(Dir, DirectiveLabel);
      caseBrig(Dir, DirectiveVersion);
      caseBrig(Dir, DirectiveSignature);
      caseBrig(Dir, DirectiveFile);
      caseBrig(Dir, DirectiveComment);
      caseBrig(Dir, DirectiveLoc);
      caseBrig(Dir, DirectiveImageInit);
      caseBrig(Dir, DirectiveSamplerInit);
      caseBrig(Dir, DirectiveVariableInit);
      caseBrig(Dir, DirectiveLabelInit);
      caseBrig(Dir, DirectiveControl);
      caseBrig(Dir, DirectivePragma);
      caseBrig(Dir, DirectiveExtension);
      caseBrig(Dir, DirectiveArgScopeStart);
      caseBrig(Dir, DirectiveArgScopeEnd);
      caseBrig(Dir, DirectiveBlockStart);
      caseBrig(Dir, DirectiveBlockNumeric);
      caseBrig(Dir, DirectiveBlockString);
      caseBrig(Dir, DirectiveBlockEnd);
      caseBrig(Dir, DirectiveLabelTargets);
      caseBrig(Dir, DirectiveFbarrier);
    default:
      check(false, "Unrecognized directive");
      return false;
    }
  }
  return true;
}

bool BrigModule::validateCode(void) const {
  inst_iterator it = S_.code_begin();
  const inst_iterator E = S_.code_end();

  if (!validateSectionSize(S_.code, S_.codeSize))
    return false;

  for (; it != E; it++) {
    if (!validate(it)) return false;
    switch (it->kind) {
      caseBrig(Inst, InstAddr);
      caseBrig(Inst, InstAtomic);
      caseBrig(Inst, InstAtomicImage);
      caseBrig(Inst, InstBar);
      caseBrig(Inst, InstBasic);
      caseBrig(Inst, InstBr);
      caseBrig(Inst, InstCmp);
      caseBrig(Inst, InstCvt);
      caseBrig(Inst, InstFbar);
      caseBrig(Inst, InstImage);
      caseBrig(Inst, InstMem);
      caseBrig(Inst, InstMod);
      caseBrig(Inst, InstSeg);
      caseBrig(Inst, InstSourceType);
      default:
        check(false, "Unrecognized code");
        return false;
    }
  }

  return true;
}

bool BrigModule::validateOperands(void) const {
  oper_iterator it = S_.oper_begin();
  const oper_iterator E = S_.oper_end();

  if (!validateSectionSize(S_.operands, S_.operandsSize))
    return false;

  for (; it < E; ++it) {
    if (!validate(it)) return false;
    switch (it->kind) {
      caseBrig(Oper, OperandAddress);
      caseBrig(Oper, OperandArgumentList);
      caseBrig(Oper, OperandFunctionList);
      caseBrig(Oper, OperandFunctionRef);
      caseBrig(Oper, OperandImmed);
      caseBrig(Oper, OperandLabelRef);
      caseBrig(Oper, OperandReg);
      caseBrig(Oper, OperandRegVector);
      caseBrig(Oper, OperandWavesize);
      caseBrig(Oper, OperandFbarrierRef);
      default:
        check(false, "Unrecognized operands");
        return false;
    }
  }

#undef caseBrig

  return true;
}

bool BrigModule::validateInstructions(void) const {
  inst_iterator it = S_.code_begin();
  const inst_iterator E = S_.code_end();

  if (!validateSectionSize(S_.code, S_.codeSize))
    return false;

#define caseInst(X, Y)                           \
  case BRIG_OPCODE_ ## Y:                       \
    if (!validate ## X(it)) return false;        \
    break

  for (; it != E; it++) {
    switch (it->opcode) {
      caseInst(Abs, ABS);
      caseInst(Add, ADD);
      caseInst(Borrow, BORROW);
      caseInst(Carry, CARRY);
      caseInst(CopySign, COPYSIGN);
      caseInst(Div, DIV);
      caseInst(Fma, FMA);
      caseInst(Fract, FRACT);
      caseInst(Mad, MAD);
      caseInst(Max, MAX);
      caseInst(Min, MIN);
      caseInst(Mul, MUL);
      caseInst(MulHi, MULHI);
      caseInst(Neg, NEG);
      caseInst(Rem, REM);
      caseInst(Sqrt, SQRT);
      caseInst(Sub, SUB);
      caseInst(Mad24, MAD24);
      caseInst(Mad24Hi, MAD24HI);
      caseInst(Mul24, MUL24);
      caseInst(Mul24Hi, MUL24HI);
      caseInst(Shl, SHL);
      caseInst(Shr, SHR);
      caseInst(And, AND);
      caseInst(Not, NOT);
      caseInst(Or, OR);
      caseInst(PopCount, POPCOUNT);
      caseInst(Xor, XOR);
      caseInst(BitRev, BITREV);
      caseInst(BitSelect, BITSELECT);
      caseInst(FirstBit, FIRSTBIT);
      caseInst(LastBit, LASTBIT);
      caseInst(Lda, LDA);
      caseInst(Ldc, LDC);
      caseInst(Mov, MOV);
      caseInst(Shuffle, SHUFFLE);
      caseInst(UnpackHi, UNPACKHI);
      caseInst(UnpackLo, UNPACKLO);
      caseInst(Cmov, CMOV);
      caseInst(Class, CLASS);
      caseInst(Ncos, NCOS);
      caseInst(Nexp2, NEXP2);
      caseInst(Nlog2, NLOG2);
      caseInst(Nrcp, NRCP);
      caseInst(Nsqrt, NSQRT);
      caseInst(Nrsqrt, NRSQRT);
      caseInst(Nsin, NSIN);
      caseInst(BitAlign, BITALIGN);
      caseInst(ByteAlign, BYTEALIGN);
      caseInst(Segmentp, SEGMENTP);
      caseInst(FtoS, FTOS);
      caseInst(StoF, STOF);
      caseInst(Cmp, CMP);
      caseInst(Cvt, CVT);
      caseInst(Ld, LD);
      caseInst(St, ST);
      caseInst(Atomic, ATOMIC);
      caseInst(AtomicNoRet, ATOMICNORET);
      caseInst(RdImage, RDIMAGE);
      caseInst(LdImage, LDIMAGE);
      caseInst(StImage, STIMAGE);
      caseInst(AtomicImage, ATOMICIMAGE);
      caseInst(Cbr, CBR);
      caseInst(Brn, BRN);
      caseInst(Barrier, BARRIER);
      caseInst(Sync, SYNC);
      caseInst(Call, CALL);
      caseInst(Ret, RET);
      caseInst(SysCall, SYSCALL);
      caseInst(Alloca, ALLOCA);
      caseInst(Clock, CLOCK);
      caseInst(CurrentWorkGroupSize, CURRENTWORKGROUPSIZE);
      caseInst(DebugTrap, DEBUGTRAP);
      caseInst(DispatchId, DISPATCHID);
      caseInst(LaneId, LANEID);
      caseInst(GridGroups, GRIDGROUPS);
      caseInst(GridSize, GRIDSIZE);
      caseInst(Nop, NOP);
      caseInst(NullPtr, NULLPTR);
      caseInst(Qid, QID);
      caseInst(WorkGroupId, WORKGROUPID);
      caseInst(WorkGroupSize, WORKGROUPSIZE);
      caseInst(WorkItemAbsId, WORKITEMABSID);
      caseInst(WorkItemId, WORKITEMID);
      caseInst(Combine, COMBINE);
      caseInst(BitExtract, BITEXTRACT);
      caseInst(Sad, SAD);
      caseInst(Ceil, CEIL);
      caseInst(Floor, FLOOR);
      caseInst(Rint, RINT);
      caseInst(Trunc, TRUNC);
      caseInst(BitMask, BITMASK);
      caseInst(BitMask, BITINSERT);
      caseInst(Expand, EXPAND);
      caseInst(NFma, NFMA);
      caseInst(Lerp, LERP);
      caseInst(Sadhi, SADHI);
      caseInst(PackCvt, PACKCVT);
      caseInst(UnpackCvt, UNPACKCVT);
      caseInst(AtomicNoRetImage, ATOMICIMAGENORET);
      caseInst(QueryImageArray, QUERYIMAGEARRAY);
      caseInst(QueryImageDepth, QUERYIMAGEDEPTH);
      caseInst(QueryImageFormat, QUERYIMAGEFORMAT);
      caseInst(QueryImageHeight, QUERYIMAGEHEIGHT);
      caseInst(QueryImageOrder, QUERYIMAGEORDER);
      caseInst(QueryImageWidth, QUERYIMAGEWIDTH);
      caseInst(QuerySamplerCoord, QUERYSAMPLERCOORD);
      caseInst(QuerySamplerFilter, QUERYSAMPLERFILTER);
      caseInst(ArriveFbar, ARRIVEFBAR);
      caseInst(InitFbar, INITFBAR);
      caseInst(JoinFbar, JOINFBAR);
      caseInst(LeaveFbar, LEAVEFBAR);
      caseInst(ReleaseFbar, RELEASEFBAR);
      caseInst(WaitFbar, WAITFBAR);
      caseInst(Ldf, LDF);
      caseInst(MaskLane, MASKLANE);
      caseInst(CountLane, COUNTLANE);
      caseInst(CountUpLane, COUNTUPLANE);
      caseInst(SendLane, SENDLANE);
      caseInst(ReceiveLane, RECEIVELANE);
      caseInst(CuId, CUID);
      caseInst(Dim, DIM);
      caseInst(GetDetectExcept, GETDETECTEXCEPT);
      caseInst(MaxCuId, MAXCUID);
      caseInst(MaxWaveId, MAXWAVEID);
      caseInst(WaveId, WAVEID);
      caseInst(WorkItemFlatAbsId, WORKITEMFLATABSID);
      caseInst(WorkItemFlatId, WORKITEMFLATID);
      caseInst(ClearDetectExcept, CLEARDETECTEXCEPT);
      caseInst(SetDetectExcept, SETDETECTEXCEPT);
      caseInst(DispatchPtr, DISPATCHPTR);
      caseInst(QPtr, QPTR);
    default:
      check(false, "Unrecognized opcode");
      return false;
    }
  }

#undef caseInst

  return true;
}

bool BrigModule::validateStrings(void) const {
  bool valid = true;

  if (!validateSectionSize(S_.strings, S_.stringsSize))
    return false;

  std::set<std::string> stringSet;

  const char *curr = S_.strings + 4;
  size_t maxLen = S_.stringsSize - 4;

  while (maxLen) {
    const BrigString *str = (const BrigString *) curr;
    if (!check(str->byteCount + 4 <= maxLen,
               "String overflows string section"))
      return false;

    std::string s(reinterpret_cast<const char *>(str->bytes),
                  str->byteCount);
    valid &= check(stringSet.insert(s).second, "Duplicate string detected");

    size_t size = (str->byteCount + 7) / 4 * 4;
    maxLen -= size;
    curr += size;
  }

  return valid;
}

bool BrigModule::validateDebug(void) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validate(const BrigDirectiveExecutable *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);

  if (dir->kind == BRIG_DIRECTIVE_KERNEL)
    valid &= check(!(dir->outArgCount),
                   "Kernel must not have any output parameters");

  if (dir->inArgCount) {
    dir_iterator firstInParam1(dir);

    for (int i = 0; i < dir->outArgCount + 1; ++i) {
      ++firstInParam1;
      if (!validate(firstInParam1)) return false;
    }

    const dir_iterator firstInParam2(S_.directives + dir->firstInArg);
    if (!validate(firstInParam2)) return false;
    valid &= check(firstInParam1 == firstInParam2, "firstInArg is wrong");
  }

  const unsigned paramCount = dir->inArgCount + dir->outArgCount;
  dir_iterator argIt = dir_iterator(dir) + 1;
  for (unsigned i = 0; i < paramCount; ++i, ++argIt) {
    if (!validate(argIt)) return false;
    const BrigDirectiveSymbol *bds = dyn_cast<BrigDirectiveSymbol>(argIt);
    if (!check(bds, "Too few argument symbols")) return false;
    if (!validate(bds)) return false;
    if (dir->kind == BRIG_DIRECTIVE_FUNCTION)
      valid &= check(bds->segment == BRIG_SEGMENT_ARG,
                     "Argument not in arg space");
    if (dir->kind == BRIG_DIRECTIVE_KERNEL)
      valid &= check(bds->segment == BRIG_SEGMENT_KERNARG,
                     "Argument not in kernarg space");
  }

  const dir_iterator firstScopedDir(S_.directives +
                                    dir->firstScopedDirective);
  if (!validOrEnd(firstScopedDir)) return false;
  valid &= check(argIt <= firstScopedDir,
                 "The first scoped directive is too early");
  valid &= check(dir->firstScopedDirective <= dir->nextTopLevelDirective,
                 "The next directive is before the first scoped directive");
  const BrigExecutableModifier8_t mod = dir->modifier;

  if (mod & BRIG_EXECUTABLE_DECLARATION) {   // a declaration
    valid &= check(dir->firstScopedDirective == dir->nextTopLevelDirective,
                   "firstScopedDirective must be the same as"
                   "nextTopLevelDirective for function declaration");
    valid &= check(dir->instCount == 0,
                   "Function declaration must have zero instCount");
  }

  if (dir->inArgCount) {
    dir_iterator firstInParam1(dir);
    for (int i = 0; i < dir->outArgCount + 1; ++i) {
      ++firstInParam1;
      if (!validate(firstInParam1)) return false;
    }

    const dir_iterator firstInParam2(S_.directives + dir->firstInArg);
    if (!validate(firstInParam2)) return false;
    valid &= check(firstInParam1 == firstInParam2, "d_firstInParam is wrong");
  }

  for (int i = 0; i < 3; i++) {
    valid &= check(dir->reserved[i] == 0,
                   "reserved field of BrigDirectiveExecutable must be zero");
  }

  return valid;
}

bool BrigModule::validateSectionSize(const char *section,
                                     unsigned expectedSize) const {
  if (!check(expectedSize >= 4, "Section too small"))
    return false;

  unsigned size;
  memcpy(&size, section, sizeof(size));
  return check(size == expectedSize, "Inconsistent section size");
}

template<class T>
static bool updateCCode(BrigCodeOffset32_t &c_code, const T *dir) {
  if (c_code > dir->code) return false;
  c_code = dir->code;
  return true;
}

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigDirectiveSymbol *) { return true; }

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigDirectiveImage *) { return true; }

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigDirectiveSampler *) { return true; }

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigBlockNumeric *) { return true; }

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigBlockString *) { return true; }

template<> bool updateCCode(BrigCodeOffset32_t &,
                            const BrigBlockEnd *) { return true; }

bool BrigModule::validateCCode(void) const {
  BrigCodeOffset32_t c_code = 0;
  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

#define caseBrig(X)                                     \
  case Brig ## X::DirKind:                              \
    if (!check(updateCCode(c_code, cast<Brig ## X>(it)), \
              "c_code out of order")) return false;     \
    break

  for (; it != E; ++it) {
    switch (it->kind) {
      caseBrig(DirectiveFunction);
      caseBrig(DirectiveKernel);
      caseBrig(DirectiveVariable);
      caseBrig(DirectiveImage);
      caseBrig(DirectiveSampler);
      caseBrig(DirectiveLabel);
      caseBrig(DirectiveVersion);
      caseBrig(DirectiveSignature);
      caseBrig(DirectiveFile);
      caseBrig(DirectiveComment);
      caseBrig(DirectiveLoc);
      caseBrig(DirectiveVariableInit);
      caseBrig(DirectiveLabelInit);
      caseBrig(DirectiveControl);
      caseBrig(DirectivePragma);
      caseBrig(DirectiveExtension);
      caseBrig(DirectiveArgScopeStart);
      caseBrig(DirectiveArgScopeEnd);
      caseBrig(DirectiveBlockStart);
      caseBrig(DirectiveBlockNumeric);
      caseBrig(DirectiveBlockString);
      caseBrig(DirectiveBlockEnd);
    }
  }
#undef caseBrig

  return true;
}

bool BrigModule::validate(const BrigDirectiveSymbol *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  valid &= check(dir->align == 0 || dir->align == 1 ||
                 dir->align == 2 || dir->align == 4 ||
                 dir->align == 8 || dir->align == 16,
                 "Invalid align field. Must be 0, 1, 2, 4, 8 or 16");
  uint64_t dim = ((uint64_t) dir->dimHi << 32) + dir->dimLo;
  BrigSymbolModifier8_t mod = dir->modifier;
  if (!(mod & BRIG_SYMBOL_ARRAY))
    valid &= check(dim == 0, "Dim must be zero for non-array symbol");

  if (mod & BRIG_SYMBOL_FLEX_ARRAY) {
    valid &= check(mod & BRIG_SYMBOL_ARRAY,
                   "A flexible array must be an array");

    valid &= check(dim == 0, "Flexible array must have dim set to zero");
  }

  // check if reserved is zero
  for (int i = 0; i < 3; i++) {
    valid &= check(dir->reserved[i] == 0,
                   "reserved field of BrigDirectiveSymbol must be zero");
    if (!valid) break;
  }
  if (dir->init) {
    // 4.23
    valid &= check(dir->segment == BRIG_SEGMENT_READONLY ||
                   dir->segment == BRIG_SEGMENT_GLOBAL,
                   "Only global and readonly spaces can be initialized");
  }

  if (dir->kind == BRIG_DIRECTIVE_VARIABLE) {
    valid &= check(dir->type != BRIG_TYPE_ROIMG &&
                   dir->type != BRIG_TYPE_RWIMG &&
                   dir->type != BRIG_TYPE_SAMP,
                   "Invalid type");

    if (dir->init) {
      const dir_iterator init(S_.directives + dir->init);
      if (!validate(init)) return false;
      const BrigDirectiveVariableInit *bdi =
        dyn_cast<BrigDirectiveVariableInit>(init);
      const BrigDirectiveLabelInit *bli =
        dyn_cast<BrigDirectiveLabelInit>(init);

      if (!check(bdi || bli, "Missing variable initializer")) return false;
      if (bdi && !validate(bdi)) return false;
      if (bli && !validate(bli)) return false;

      if (bdi) {
        if (!validate(bdi)) return false;
        uint32_t elementCount = bdi->elementCount;
        uint64_t dim = ((uint64_t) dir->dimHi << 32) + dir->dimLo;

        if (dim)
          valid &= check(elementCount == dim,
                       "Inconsistent array dimensions");
      }
      if (bli) {
        if (!validate(bli)) return false;
        uint16_t labelCount = bli->labelCount;
        uint64_t dim = ((uint64_t) dir->dimHi << 32) + dir->dimLo;

        if (dim)
          valid &= check(labelCount == dim,
                       "Inconsistent array dimensions");
      }
    }
  } else if (dir->kind == BRIG_DIRECTIVE_IMAGE) {
    valid &= check(dir->type == BRIG_TYPE_ROIMG ||
                   dir->type == BRIG_TYPE_RWIMG,
                   "Invalid type");
    if (dir->init) {
      const dir_iterator init(S_.directives + dir->init);
      if (!validate(init)) return false;
      const BrigDirectiveImageInit *bdi =
        dyn_cast<BrigDirectiveImageInit>(init);

      if (!check(bdi, "Missing image initializer")) return false;
      if (bdi && !validate(bdi)) return false;
    }
  } else if (dir->kind == BRIG_DIRECTIVE_SAMPLER) {
    if (dir->init) {
      const dir_iterator init(S_.directives + dir->init);
      if (!validate(init)) return false;
      const BrigDirectiveSamplerInit *bdi =
        dyn_cast<BrigDirectiveSamplerInit>(init);

      if (!check(bdi, "Missing sampler initializer")) return false;
      if (bdi && !validate(bdi)) return false;
    }
  } else {
    check(false, "Invalid BrigDirectiveSymbol");
    return false;
  }
  return valid;
}
bool BrigModule::validate(const BrigDirectiveLabel *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  return valid;
}

// 20.8.22
bool BrigModule::validate(const BrigDirectiveVersion *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= check(dir->machineModel == BRIG_MACHINE_LARGE ||
                 dir->machineModel == BRIG_MACHINE_SMALL,
                 "Invalid machine");
  valid &= check(dir->profile == BRIG_PROFILE_FULL ||
                 dir->profile == BRIG_PROFILE_BASE,
                 "Invalid profile");

  valid &= check(dir->brigMajor == BRIG_VERSION_BRIG_MAJOR &&
                 dir->brigMinor == BRIG_VERSION_BRIG_MINOR,
                 "Unsupported Brig version");
  valid &= check(!(dir->reserved),
                 "reserved field of BrigDirectiveVersion must be zero");
  const BrigDirectiveVersion *bdfv = getFirstVersionDirective();
  if (!check(bdfv, "Missing BrigDirectiveVersion")) return false;
  valid &= check(dir->hsailMajor == bdfv->hsailMajor,
                 "All BrigDirectiveVersions must have the same value");
  valid &= check(dir->hsailMinor == bdfv->hsailMinor,
                 "All BrigDirectiveVersions must have the same value");
  valid &= check(dir->brigMajor == bdfv->brigMajor,
                 "All BrigDirectiveVersions must have the same value");
  valid &= check(dir->brigMinor == bdfv->brigMinor,
                 "All BrigDirectiveVersions must have the same value");
  valid &= check(dir->machineModel == bdfv->machineModel,
                 "All BrigDirectiveVersions must have the same value");
  valid &= check(dir->profile == bdfv->profile,
                 "All BrigDirectiveVersions must have the same value");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveSignature *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  uint16_t numArgs = dir->inArgCount + dir->outArgCount;

  for (int i = 0; i < numArgs; i++) {
    valid &= check(dir->args[i].type <= BRIG_TYPE_F64X2,
                   "Invalid type");
    valid &= check(dir->args[i].align == 0 ||
                   dir->args[i].align == 1 ||
                   dir->args[i].align == 2 ||
                   dir->args[i].align == 4 ||
                   dir->args[i].align == 8 ||
                   dir->args[i].align == 16,
                   "Invalid align, must be 0, 1, 2, 4, 8 or 16");

    valid &= check((dir->args[i].modifier & BRIG_SYMBOL_LINKAGE) ==
                   BRIG_LINKAGE_NONE,
                   "Signature arguments must have BRIG_LINKAGE_NONE");

    valid &= check(!(dir->args[i].modifier & BRIG_SYMBOL_CONST),
                   "Signautre argument cannot be marked by BRIG_SYMBOL_CONST");

    uint64_t dim = ((uint64_t)dir->args[i].dimHi << 32) + dir->args[i].dimLo;
    if (dim)
      valid &= check((dir->args[i].modifier & BRIG_SYMBOL_ARRAY) &&
                     !(dir->args[i].modifier & BRIG_SYMBOL_FLEX_ARRAY),
                     "Non-array and flexible array arguments "
                     "must have dim = 0");

    if (dir->args[i].modifier & BRIG_SYMBOL_FLEX_ARRAY)
      valid &= check(i == numArgs-1,
                     "Only the last argument can be flexible");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveFile *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->filename);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveComment *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLoc *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  BrigStringOffset32_t filename = lookupFilename(dir->fileid);

  valid &= check(filename,
                 "There is no BrigDirectiveFile for this BrigDirectiveLoc");
  if (filename) {
    valid &= validateSName(filename);
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectiveVariableInit *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 8);
  valid &= validateCCode(dir->code);
  valid &= check(dir->type <= BRIG_TYPE_F64X2,
                 "Invalid data type");
  valid &= validateSName(dir->data);
  const BrigString *bs = reinterpret_cast<const BrigString*>(S_.strings +
                                                       dir->data);

  valid &= check(bs->byteCount <=
                 dir->elementCount*BrigInstHelper::getTypeSize(dir->type),
                 "Invalid size of data in BrigDirectiveVariableInit");

  valid &= check(dir->reserved == 0,
                 "reserved field must be zero");

  return valid;
}

bool BrigModule::validate(const BrigDirectiveImageInit *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 8);
  valid &= validateCCode(dir->code);

  if (dir->array > 1)
    valid &= check(dir->depth == 1,
                   "Depth must be 1 for 1D or 2D image");

  valid &= check(dir->order <= BRIG_ORDER_SBGRA,
                 "Invalid image order");

  valid &= check(dir->format <= BRIG_FORMAT_UNORM_INT24,
                 "Invalid image format");

  valid &= check(dir->reserved == 0,
                 "reserved field is not zero");

  return valid;
}

bool BrigModule::validate(const BrigDirectiveSamplerInit *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 8);

  valid &= check(dir->boundaryU <= BRIG_BOUNDARY_BORDER,
                 "Invalid sampler boundary value");
  valid &= check(dir->boundaryV <= BRIG_BOUNDARY_BORDER,
                 "Invalid sampler boundary value");
  valid &= check(dir->boundaryW <= BRIG_BOUNDARY_BORDER,
                 "Invalid sampler boundary value");

  valid &= check((dir->modifier & BRIG_SAMPLER_FILTER) <= BRIG_FILTER_LINEAR,
                 "Invalid sampler filter");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelTargets *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  for (unsigned i = 0; i < dir->labelCount; i++) {
    const dir_iterator init(S_.directives + dir->labels[i]);
    if (!validate(init)) return false;
    if (!check(isa<BrigDirectiveLabel>(init),
              "labels offset is wrong, must point to a BrigDirectiveLabel"))
      return false;
  }
  valid &= check(dir->reserved == 0,
                 "reserved field in BrigDirectiveLabelTargets must be zero");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveLabelInit *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  for (unsigned i = 0; i < dir->labelCount; i++) {
    const dir_iterator init(S_.directives + dir->labels[i]);
    if (!validate(init)) return false;
    if (!check(isa<BrigDirectiveLabel>(init),
              "labels offset is wrong, not a BrigDirectiveLabel"))
      return false;
  }
  valid &= check(dir->reserved == 0,
                 "reserved field in BrigDirectiveLabelInit must be zero");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveControl *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= check(dir->control <= BRIG_CONTROL_REQUIRENOPARTIALWORKGROUPS,
                 "Invalid control directive");
  valid &= check(dir->type <= BRIG_TYPE_F64X2,
                 "Invalid type");

  valid &= check(dir->reserved == 0,
                 "reserved in BrigDirectiveControl must be zero");

  for (int i = 0; i < dir->valueCount; i++) {
    valid &= check(dir->values[i],
                   "Missing a value");

    oper_iterator oper(S_.operands + dir->values[i]);

    valid &= check(isa<BrigOperandImmed>(oper) ||
                   isa<BrigOperandWavesize>(oper),
                   "Operand of BrigDirectiveControl must be "
                   "either immediate or wavesize");
  }
  return valid;
}

bool BrigModule::validate(const BrigDirectivePragma *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveExtension *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgScopeStart *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveArgScopeEnd *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockStart *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateSName(dir->name);
  const BrigString* bs = reinterpret_cast<const BrigString*>(S_.strings +
                                                             dir->name);
  std::string name(reinterpret_cast<const char*>(bs->bytes), bs->byteCount);
  valid &= check(0 == strcmp(name.c_str(), "debug") ||
                 0 == strcmp(name.c_str(), "rti"),
                 "Invalid name, should be either debug or rti");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockNumeric *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= check(0 == dir->size % 8,
                 "Invalid size, must be a multiple of 8");
  valid &= check(BRIG_TYPE_B8 == dir->type  ||
                 BRIG_TYPE_B16 == dir->type ||
                 BRIG_TYPE_B32 == dir->type ||
                 BRIG_TYPE_B64 == dir->type,
                 "Invalid type, must be b8, b16, b32, or b64");
  BrigType type = BrigType(dir->type);
  const BrigString* bs = reinterpret_cast<const BrigString*>(S_.strings +
                                                             dir->data);
  valid &= check(bs->byteCount ==
                 dir->elementCount*BrigInstHelper::getTypeSize(type),
                 "Number of data bytes is incorrect");
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockString *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateSName(dir->string);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveBlockEnd *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  return valid;
}

bool BrigModule::validate(const BrigDirectiveFbarrier *dir) const {
  bool valid = true;
  if (!validateSize(dir)) return false;
  valid &= validateAlignment(dir, 4);
  valid &= validateCCode(dir->code);
  valid &= validateSName(dir->name);
  return valid;
}

bool BrigModule::validOrEnd(const dir_iterator dir) const {
  // Exit early to avoid segmentation faults.
  dir_iterator firstValidDir(S_.directives);
  if (!check(firstValidDir <= dir, "dir before the directives section"))
    return false;

  dir_iterator E(S_.directives + S_.directivesSize);
  if (!check(dir <= E, "dir past the directives section"))
    return false;

  return true;
}

bool BrigModule::validate(const dir_iterator dir) const {
  // Exit early to avoid segmentation faults.
  if (!validOrEnd(dir)) return false;

  dir_iterator lastValidDir(S_.directives + S_.directivesSize -
                            sizeof(BrigDirectiveBase));
  if (!check(dir <= lastValidDir, "dir spans the directives section"))
    return false;

  dir_iterator E(S_.directives + S_.directivesSize);
  if (!check(dir + 1 <= E, "dir spans the directives section"))
    return false;

  return true;
}

bool BrigModule::validateCCode(BrigCodeOffset32_t c_code) const {
  bool valid = true;
  valid &= check(c_code + sizeof(BrigInstBase) > c_code,
                 "c_code overflows");
  valid &= check(!c_code || c_code <= S_.codeSize,
                 "c_code past the code section");
  return valid;
}

bool BrigModule::validateSName(BrigStringOffset32_t s_name) const {
  bool valid = true;

  valid &= check(s_name < S_.stringsSize,
                 "s_name past the strings section");

  // Do attempt the next test if s_name is past the end of the strings
  // section! It may cause a segmentation fault.
  if (!valid) return false;
  valid &= check(!(s_name % 4),
                 "BrigString must start on 4-byte boundary");
  const BrigString *bs = reinterpret_cast <const BrigString *>(S_.strings +
                                                               s_name);
  uint32_t numBytes = bs->byteCount;

  if (!check(s_name + numBytes <= S_.stringsSize,
             "s_name past the end of string section"))
    return false;

  for (uint32_t i = numBytes; i < ((numBytes + 3)/4)*4; i++) {
    valid &= check(!bs->bytes[i],
                   "Padding bytes in BrigString must be zero");
    if (!valid) break;
  }
  return valid;
}

static bool getRegType(char c, BrigType *type) {
  if (c == 'c') {
    *type = BRIG_TYPE_B1;
    return true;
  } else if (c == 's') {
    *type = BRIG_TYPE_B32;
    return true;
  } else if (c == 'd') {
    *type = BRIG_TYPE_B64;
    return true;
  } else if (c == 'q') {
    *type = BRIG_TYPE_B128;
    return true;
  } else {
    return false;
  }
}

bool BrigModule::validateRegName(BrigStringOffset32_t reg,
                                 BrigType targetType) const {
  bool valid = true;

  // Exit early to prevent out-of-bounds access
  if (!validateSName(reg))
    return false;

  const BrigString *name = (const BrigString *) (S_.strings + reg);

  // Exit early to prevent out-of-bounds access
  if (!check(name->bytes[0] == '$', "Register names must begin with '$'"))
    return false;

  // Exit early to prevent out-of-bounds access
  BrigType type;
  if (!check(getRegType(name->bytes[1], &type), "Invalid register type"))
    return false;

  // Exit early to prevent out-of-bounds access
  if (!check(isdigit(name->bytes[2]), "Register offset not a number"))
    return false;

  const uint8_t *endptr;
  int64_t regOffset = nstrtol(name, &endptr, 10);
  valid &= check(endptr == name->bytes + name->byteCount,
                 "Garbage after register offset");
  if (type == BRIG_TYPE_B1)
    check(0 <= regOffset && regOffset < 8, "Register offset out-of-bounds");
  else if (type == BRIG_TYPE_B32)
    check(0 <= regOffset && regOffset < 128, "Register offset out-of-bounds");
  else if (type == BRIG_TYPE_B64)
    check(0 <= regOffset && regOffset < 64, "Register offset out-of-bounds");
  else if (type == BRIG_TYPE_B128)
    check(0 <= regOffset && regOffset < 32, "Register offset out-of-bounds");

  valid &= check(targetType == type, "Register name does not match type");

  return valid;
}

bool BrigModule::validateAlignment(const void *dir, uint8_t alignment) const {
  bool valid = true;
  const char *dirOffset = reinterpret_cast<const char *>(dir);
  valid &= check((S_.directives - dirOffset) % alignment == 0,
                 "Improperly aligned directive");
  return valid;
}

template<typename T> bool BrigModule::validateSize(const T *brig) const {
  return check(brig->size >= sizeof(T), "Brig structure is too small");
}

bool BrigModule::validate(const BrigInstAddr *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(!(code->reserved[0] ||
                   code->reserved[1] ||
                   code->reserved[2]),
                 "reserved must be zero");
  return valid;
}

bool BrigModule::validate(const BrigInstAtomic *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_ATOMIC ||
                 code->opcode == BRIG_OPCODE_ATOMICNORET,
                 "Invalid opcode, should be either BrigAtomic or "
                 "BrigAtomicNoRet");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->atomicOperation <= BRIG_ATOMIC_SUB,
                 "Invalid atomicOperation");
  // PRM 6.5.1
  valid &= check(code->segment == BRIG_SEGMENT_GLOBAL ||
                 code->segment == BRIG_SEGMENT_GROUP ||
                 code->segment == BRIG_SEGMENT_FLAT,
                 "Invalid storage class, can be global, group, "
                 "or flat");
  valid &= check(code->memorySemantic == BRIG_SEMANTIC_REGULAR ||
                 code->memorySemantic == BRIG_SEMANTIC_ACQUIRE ||
                 code->memorySemantic == BRIG_SEMANTIC_ACQUIRE_RELEASE ||
                 code->memorySemantic == BRIG_SEMANTIC_PARTIAL_ACQUIRE_RELEASE,
                 "Invalid memorySemantic, can be regular, acquire, "
                 "acquire release, or partial acquire release");
  // validate type
  BrigType type = BrigType(code->type);
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Invalid type");
  switch (code->atomicOperation) {
    case BRIG_ATOMIC_AND:
    case BRIG_ATOMIC_OR:
    case BRIG_ATOMIC_XOR:
    case BRIG_ATOMIC_CAS:
    case BRIG_ATOMIC_EXCH:
      valid &= check(BrigInstHelper::isBitTy(type),
                     "Invalid type. Must be bit type for "
                     "atomic and, or, xor, cas, exch");
      break;
    case BRIG_ATOMIC_ADD:
    case BRIG_ATOMIC_SUB:
    case BRIG_ATOMIC_MAX:
    case BRIG_ATOMIC_MIN:
      valid &= check(BrigInstHelper::isUnsignedTy(type) ||
                     BrigInstHelper::isSignedTy(type),
                     "Invalid type. Must be integer type for "
                     "atomic add, sub, max or min");
      break;
    case BRIG_ATOMIC_INC:
    case BRIG_ATOMIC_DEC:
      valid &= check(BrigInstHelper::isUnsignedTy(type),
                     "Invalid type. Must be unsigned type for "
                     "atomic inc or dec");
      break;

    default:
      check(false, "Invalid operation");
      return false;
  }

  const BrigDirectiveVersion *bdv = getFirstVersionDirective();
  if (bdv->machineModel == BRIG_MACHINE_LARGE)
    valid &= check(BrigInstHelper::getTypeSize(type) == 32 ||
                   BrigInstHelper::getTypeSize(type) == 64,
                   "Invalid type length, must  be 32 or 64");

  if (bdv->machineModel == BRIG_MACHINE_SMALL)
    valid &= check(BrigInstHelper::getTypeSize(type) == 32,
                   "64 bit atomic operation is not allowed "
                   "on small machine model");
  return valid;
}

bool BrigModule::validate(const BrigInstAtomicImage *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_ATOMICIMAGE ||
                 code->opcode == BRIG_OPCODE_ATOMICIMAGENORET,
                 "Invalid opcode, must be either "
                 "BRIG_OPCODE_ATOMIC_IMAGE or BRIG_OPCODE_ATOMIC_IMAGENORET");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                     "operands past the operands section");
    }
  }
  valid &= check(code->imageType == BRIG_TYPE_RWIMG,
                 "Invalid image type, must be BRIG_TYPE_RWIMG");
  valid &= check(code->coordType == BRIG_TYPE_U32,
                 "Invalid coordType, must be BRIG_TYPE_U32");
  valid &= check(code->atomicOperation <= BRIG_ATOMIC_SUB,
                 "Invalid atomicOperation");
  valid &= check(code->geometry <= BRIG_GEOMETRY_2DA,
                "Invalid geometry");
  valid &= check(!(code->reserved),
                 "Reserved must be zero");
  return valid;
}

bool BrigModule::validate(const BrigInstBar *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_BARRIER ||
                 code->opcode == BRIG_OPCODE_SYNC,
                 "Invalid opcode, must be either "
                 "BRIG_OPCODE_BARRIER or BRIG_OPCODE_SYNC");
  valid &= check(code->type <= BRIG_TYPE_F64X2, "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                     "operands past the operands section");
    }
  }
  valid &= check(code->memoryFence <= BRIG_FENCE_PARTIAL_BOTH,
                 "Invalid memoryFence");
  valid &= check(code->width <= BRIG_WIDTH_ALL,
                 "Invalid width");
  valid &= check(!(code->reserved),
                 "Reserved must be zero");
  return valid;
}

bool BrigModule::validate(const BrigInstBasic *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode < BRIG_OPCODE_INVALID,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                     "operands past the operands section");
    }
  }
  return valid;
}

bool BrigModule::validate(const BrigInstBr *code) const {
  return true;
}

bool BrigModule::validate(const BrigInstCmp *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_CMP,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  valid &= check(code->pack <= BRIG_PACK_PSAT,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->compare <= BRIG_COMPARE_SGTU,
                 "Invalid comparisonOperator");
  valid &= check(code->sourceType <= BRIG_TYPE_F64X2,
                 "Invalid sourceType");
  return valid;
}

bool BrigModule::validate(const BrigInstFbar *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  return valid;
}

bool BrigModule::validate(const BrigInstImage *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_LDIMAGE ||
                 code->opcode == BRIG_OPCODE_STIMAGE ||
                 code->opcode == BRIG_OPCODE_RDIMAGE,
                 "Invalid opcode");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->geometry <= BRIG_GEOMETRY_2DA,
                 "Invalid type of image geometry");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  return valid;
}

bool BrigModule::validate(const BrigInstCvt *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode == BRIG_OPCODE_CVT,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->sourceType <= BRIG_TYPE_F64X2,
                 "Invalid stype");
  return valid;
}

bool BrigModule::validate(const BrigInstMem *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode < BRIG_OPCODE_INVALID,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->segment == BRIG_SEGMENT_GLOBAL ||
                 code->segment == BRIG_SEGMENT_GROUP ||
                 code->segment == BRIG_SEGMENT_PRIVATE ||
                 code->segment == BRIG_SEGMENT_KERNARG ||
                 code->segment == BRIG_SEGMENT_READONLY ||
                 code->segment == BRIG_SEGMENT_SPILL ||
                 code->segment == BRIG_SEGMENT_ARG ||
                 code->segment == BRIG_SEGMENT_FLAT,
                 "Invalid storage class, can be global, group, "
                 "private, kernarg, readonly, spill, arg, or flat");
  return valid;
}

bool BrigModule::validate(const BrigInstMod *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode < BRIG_OPCODE_INVALID,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  valid &= check(code->pack <= BRIG_PACK_PSAT,
                 "Invalid packing control");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  return valid;
}

bool BrigModule::validate(const BrigInstSeg *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  valid &= check(code->opcode < BRIG_OPCODE_INVALID,
                 "Invalid opcode");
  valid &= check(code->type <= BRIG_TYPE_F64X2,
                 "Invalid type");
  for (unsigned i = 0; i < 5; ++i) {
    if (code->operands[i]) {
      valid &= check(code->operands[i] < S_.operandsSize,
                   "operands past the operands section");
    }
  }
  valid &= check(code->segment < BRIG_SEGMENT_INVALID,
                 "Invalid storage class");
  valid &= check(code->sourceType <= BRIG_TYPE_F64X2,
                 "Invalid type");
  return valid;
}

bool BrigModule::validate(const BrigInstSourceType *code) const {
  bool valid = true;
  if (!validateSize(code)) return false;
  return valid;
}

bool BrigModule::validate(const inst_iterator inst) const {
  // Exit early to avoid segmentation faults.
  inst_iterator firstValidCode(S_.code);
  if (!check(firstValidCode <= inst, "inst before the code section"))
    return false;

  inst_iterator E(S_.code + S_.codeSize);
  if (!check(inst <= E, "inst past the code section"))
    return false;

  inst_iterator lastValidCode(S_.code + S_.codeSize -
                            sizeof(BrigInstBase));
  if (!check(inst <= lastValidCode, "inst spans the code section"))
    return false;

  if (!check(inst + 1 <= E, "inst spans the code section"))
    return false;

  return true;
}

bool BrigModule::validate(const BrigOperandAddress *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  valid &= check(operand->type == BRIG_TYPE_B32 ||
                 operand->type == BRIG_TYPE_B64, "Invalid datatype, should be "
                 "BRIG_TYPE_B32 and BRIG_TYPE_B64");
  if (operand->symbol) {
    dir_iterator dir(S_.directives + operand->symbol);
    if (!validate(dir)) return false;
    valid &= check(isa<BrigDirectiveSymbol>(dir),
                   "Invalid directive, should point to a BrigDirectiveSymbol");
  }
  return valid;
}

bool BrigModule::validate(const BrigOperandArgumentList *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  size_t dirSize =
    sizeof(BrigOperandArgumentList) +
    sizeof(operand->elements[0]) *
    (std::max((uint16_t) 1, operand->elementCount) - 1);
  valid &= check(operand->size >= dirSize, "Invalid size");

  for (unsigned i = 0; i < operand->elementCount; ++i) {
    dir_iterator arg(S_.directives + operand->elements[i]);
    if (!validate(arg)) return false;
  }
  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionList *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  unsigned funRefCount = 0;
  unsigned sigRefCount = 0;

  if (operand->elementCount) {
    valid &= check(sizeof(BrigOperandArgumentList) +
                   sizeof(operand->elements[1]) * (operand->elementCount - 1)
                   <= operand->size, "Invalid size");
  }

  for (unsigned i = 0; i < operand->elementCount; ++i) {
    oper_iterator arg(S_.operands + operand->elements[i]);
    if (!validate(arg)) return false;
    if (const BrigOperandFunctionRef *funRef =
       dyn_cast<BrigOperandFunctionRef>(arg)) {
      if (!validate(funRef)) return false;
      dir_iterator fun(S_.directives + funRef->ref);
      if (!validate(fun)) return false;

      if (isa<BrigDirectiveFunction>(fun)) {
        ++funRefCount;
      } else {
        ++sigRefCount;
      }
    }
  }
  valid &= check(funRefCount == operand->elementCount ||
                 sigRefCount == operand->elementCount,
                 "element of o_args should be all functions or "
                 "all signatures");
  valid &= check(sigRefCount < 2, "Too many function signatures");

  return valid;
}

bool BrigModule::validate(const BrigOperandFunctionRef *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  dir_iterator fnDir(S_.directives + operand->ref);
  if (!validate(fnDir)) return false;
  valid &= check(isa<BrigDirectiveFunction>(fnDir) ||
                 isa<BrigDirectiveSignature>(fnDir),
                 "Invalid directive, should point to a "
                 "BrigDirectiveFunction or BrigDirectiveSignature");
  return valid;
}

bool BrigModule::validate(const BrigOperandImmed *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  if (!check(BRIG_TYPE_B1 == operand->type  ||
            BRIG_TYPE_B8 == operand->type  ||
            BRIG_TYPE_B16 == operand->type ||
            BRIG_TYPE_B32 == operand->type ||
            BRIG_TYPE_B64 == operand->type ||
            BRIG_TYPE_B128 == operand->type,
            "Invalid type, must be b1, b8, b16, b32, b64 or b128"))
    return false;
  BrigType type = BrigType(operand->type);
  size_t immediateSize = sizeof(BrigOperandImmed) - sizeof(uint8_t) +
    (BrigInstHelper::getTypeSize(type) / 8 + 3) / 4;
  valid &= check(immediateSize <= operand->size,
                 "Operand size too small for immediate");
  return valid;
}

bool BrigModule::validate(const BrigOperandLabelRef *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  dir_iterator directiveDir(S_.directives + operand->ref);
  if (!validate(directiveDir)) return false;
  valid &= check(isa<BrigDirectiveLabel>(directiveDir) ||
                 isa<BrigDirectiveLabelTargets>(directiveDir),
                 "Invalid directive, should point "
                 "to a BrigDirectiveLabel or BrigDirectiveLabelTargets");
  return valid;
}

bool BrigModule::validate(const BrigOperandReg *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  valid &= validateRegName(operand->reg, BrigType(operand->type));
  return valid;
}

bool BrigModule::validate(const BrigOperandRegVector *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  if (!check(operand->regCount >= 2 && operand->regCount <= 4,
            "Register vectors must have size 2 or 4"))
    return false;
  for (int i = 0; i < operand->regCount; ++i) {
    valid &= validateRegName(operand->regs[i], BrigType(operand->type));
  }
  valid &= check(operand->type == BRIG_TYPE_B1 ||
                 operand->type == BRIG_TYPE_B32 ||
                 operand->type == BRIG_TYPE_B64,
                 "Invalid date type");
  return valid;
}

bool BrigModule::validate(const BrigOperandWavesize *operand) const {
  if (!validateSize(operand)) return false;
  return true;
}

bool BrigModule::validate(const BrigOperandFbarrierRef *operand) const {
  bool valid = true;
  if (!validateSize(operand)) return false;
  return valid;
}

bool BrigModule::validate(const oper_iterator operand) const {
  // Exit early to avoid segmentation faults.
  oper_iterator firstValidOperand(S_.operands);
  if (!check(firstValidOperand <= operand,
     "operand before the operand section"))
    return false;

  oper_iterator E(S_.operands + S_.operandsSize);
  if (!check(operand <= E, "operand past the operand section"))
    return false;

  oper_iterator lastValidOperand(S_.operands + S_.operandsSize -
                                 sizeof(BrigOperandBase));
  if (!check(operand <= lastValidOperand,
     "operand spans the operand section"))
    return false;

  if (!check(operand + 1 <= E, "operand spans the operand section"))
    return false;

  return true;
}

static unsigned getNumOperands(const inst_iterator inst) {
  for (unsigned i = 0; i < 5; ++i) {
    if (!inst->operands[i])
      return i;
  }
  return 5;
}

static const BrigType16_t *getType(const oper_iterator oper) {
  if (const BrigOperandAddress *address = dyn_cast<BrigOperandAddress>(oper))
    return &address->type;

  if (const BrigOperandImmed *immed = dyn_cast<BrigOperandImmed>(oper))
    return &immed->type;

  if (const BrigOperandReg *reg = dyn_cast<BrigOperandReg>(oper))
    return &reg->type;

  if (const BrigOperandRegVector *regV = dyn_cast<BrigOperandRegVector>(oper))
    return &regV->type;

  return NULL;
}

static bool isCompatibleSrc(BrigType type, const oper_iterator oper) {
  if (isa<BrigOperandWavesize>(oper))
    return
      BrigInstHelper::isSignedTy(type)   ||
      BrigInstHelper::isUnsignedTy(type) ||
      BrigInstHelper::isBitTy(type);

  const BrigType16_t *srcTy = getType(oper);
  if (!srcTy) return false;

  if (type == BRIG_TYPE_F16 && isa<BrigOperandReg>(oper))
    return *srcTy == BRIG_TYPE_B32;

  const size_t instSize = BrigInstHelper::getTypeSize(type);
  const size_t srcSize = BrigInstHelper::getTypeSize(*srcTy);
  return instSize == srcSize;
}

bool BrigModule::isCompatibleAddrSize(const  BrigSegment8_t sClass,
                                      const BrigType16_t type) const {
  bool valid = true;
  size_t size = BrigInstHelper::getTypeSize(BrigType(type));
  switch (sClass) {
    case BRIG_SEGMENT_GROUP:
    case BRIG_SEGMENT_PRIVATE:
    case BRIG_SEGMENT_SPILL:
    case BRIG_SEGMENT_ARG:
      // FIXME: Disable check since AMD's assembler currently does this wrong.
      // Look at Brigantine::setOperand in HSAILBrigantine.cpp line 824, which
      // currently sets all address sizes to B64 for large targets.

      // valid &= check(32 == size, "Invalid type");
      break;
    default:
      const BrigDirectiveVersion *bdv = getFirstVersionDirective();
      if (!check(bdv, "Missing version?")) return false;
      valid &= check((32 == size && bdv->machineModel == BRIG_MACHINE_SMALL) ||
                     (64 == size && bdv->machineModel == BRIG_MACHINE_LARGE),
                     "Invalid type");
      break;
  }
  return valid;
}

bool BrigModule::validateArithmeticInst(const inst_iterator inst,
                                        unsigned nary) const {
  bool valid = true;

  // Unary arithmetic requires signed types
  BrigType type = BrigType(inst->type);
  valid &= check(BrigInstHelper::isSignedTy(type) ||
                 BrigInstHelper::isUnsignedTy(type) ||
                 BrigInstHelper::isFloatTy(type) ||
                 BrigInstHelper::isBitTy(type),
                 "Invalid type");

  valid &= check(isa<BrigInstBasic>(inst) || isa<BrigInstMod>(inst),
                 "Incorrect instruction kind");

  if (!check(getNumOperands(inst) == nary + 1, "Incorrect number of operands"))
    return false;

  oper_iterator dest(S_.operands + inst->operands[0]);
  const BrigOperandReg *destReg = dyn_cast<BrigOperandReg>(dest);
  valid &= check(destReg, "Destination must be a register");

  for (unsigned i = 0; i < nary; ++i) {
    oper_iterator src(S_.operands + inst->operands[i + 1]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source must be a register, immediate, or wave size");

    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  }

  valid &= check(BrigInstHelper::getTypeSize(type) <= 128, "Illegal data type");

  valid &= check(BrigInstHelper::getTypeSize(type) <=
                 BrigInstHelper::getTypeSize(BrigType(destReg->type)),
                 "Destination register is too small");

  return valid;
}

bool BrigModule::validateShiftInst(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(BrigInstHelper::isSignedTy(type) ||
                 BrigInstHelper::isUnsignedTy(type),
                 "Type is only valid for signed and unsigned point types");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");

  oper_iterator src0(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0) ||
                 isa<BrigOperandWavesize>(src0),
                 "Source must be a register, immediate, or wave size");
  if (isa<BrigOperandReg>(src0) || isa<BrigOperandImmed>(src0))
    valid &= check(isCompatibleSrc(type, src0), "Incompatible source operand");

  oper_iterator src1(S_.operands + inst->operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1) ||
                 isa<BrigOperandWavesize>(src1),
                 "Source must be a register, immediate, or wave size");
  if (isa<BrigOperandReg>(src1) || isa<BrigOperandImmed>(src1))
    valid &= check(*getType(src1) == BRIG_TYPE_B32,
                   "Type of src1 should be BRIG_TYPE_B32");

  if (!BrigInstHelper::isVectorTy(type))
    valid &= check(BrigInstHelper::getTypeSize(type) == 32 ||
                   BrigInstHelper::getTypeSize(type) == 64,
                   "If regular form, length should be 32 or 64");

  return valid;
}

bool BrigModule::validateMovdInst(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == BRIG_TYPE_B64,
                 "Type of Movd should be BRIG_TYPE_B64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Movd cannot accept vector types");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B64,
                 "Destination should be a d register");
  oper_iterator src0(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src0), "Src0 should be register");
  valid &= check(*getType(src0) == BRIG_TYPE_B64,
                 "Src0 should be a d register");
  oper_iterator src1(S_.operands + inst->operands[2]);
  valid &= check(isa<BrigOperandReg>(src1), "Src0 should be register");
  valid &= check(*getType(src1) == BRIG_TYPE_B32,
                 "Src1 should be a s register");
  return valid;
}

bool BrigModule::validateMovsInst(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32,
                 "Type of Movs should be BRIG_TYPE_B32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Movs cannot accept vector types");
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "Destination should be a s register");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src), "Src0 should be register");
  valid &= check(*getType(src) == BRIG_TYPE_B64,
                 "Destination should be a d register");
  return valid;
}

bool BrigModule::validateUnpackInst(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;

  BrigType type = BrigType(inst->type);
  valid &= check(inst->type == BRIG_TYPE_U16X2 ||
                 inst->type == BRIG_TYPE_S16X2 ||
                 inst->type == BRIG_TYPE_F16X2 ||
                 inst->type == BRIG_TYPE_U16X4 ||
                 inst->type == BRIG_TYPE_S16X4 ||
                 inst->type == BRIG_TYPE_F16X4 ||
                 inst->type == BRIG_TYPE_U32X2 ||
                 inst->type == BRIG_TYPE_S32X2 ||
                 inst->type == BRIG_TYPE_F32X2 ||
                 inst->type == BRIG_TYPE_U8X8  ||
                 inst->type == BRIG_TYPE_S8X8  ||
                 inst->type == BRIG_TYPE_U8X4  ||
                 inst->type == BRIG_TYPE_S8X4,
                 "Length of Unpack should be 8x4, 8x8, 16x2, 16x4 or 32x2");

  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  oper_iterator src0(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0), "Src should be reg or immed");
  oper_iterator src1(S_.operands + inst->operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1), "Src should be reg or immed");

  for (int i = 0; i < 3; ++i) {
    oper_iterator reg(S_.operands + inst->operands[i]);
    valid &= check(isCompatibleSrc(type, reg), "Incompatible source operand");
  }

  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Unpack should accept vector types");

  return valid;
}

bool BrigModule::validateLdStImageInst(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstImage *image = dyn_cast<BrigInstImage>(inst);
  if (!check(image, "Invalid instruction kind")) return false;
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  BrigType destType = BrigType(image->type);
  valid &= check(destType == BRIG_TYPE_U32 ||
                 destType == BRIG_TYPE_F32 ||
                 destType == BRIG_TYPE_S32,
                 "Type of destination should be u32, f32 or s32");

  oper_iterator dest(S_.operands + image->operands[0]);
  valid &= check(isa<BrigOperandRegVector>(dest),
                 "Destination should be RegVector");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "Destination should be b32");

  oper_iterator src0(S_.operands + image->operands[1]);
  oper_iterator src1(S_.operands + image->operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandRegVector>(src1),
                 "Source should be reg, regV2 or regV4");
  valid &= check(*getType(src1) == BRIG_TYPE_B32,
                 "Source should be b register");
  valid &= check(!BrigInstHelper::isVectorTy(destType),
                 "LdSt can not accept vector types");
  return valid;
}

bool BrigModule::validateImageQueryInst(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;

  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_B32 || type == BRIG_TYPE_U32,
                 "Type should be b32 or u32");

  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");

  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Image can not accept vector types");
  return valid;
}

bool BrigModule::validateParaSynInst(const inst_iterator inst,
                                     unsigned nary) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 1 + nary, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_B64 ||
                 type == BRIG_TYPE_U32 ||
                 type == BRIG_TYPE_B32,
                 "Type should be b64, u32 or b32");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  for (unsigned i = 0; i < nary; ++i) {
    oper_iterator src(S_.operands + inst->operands[i + 1]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source shoule be register, immediate or waveSz");
    valid &= check(isCompatibleSrc(type, src), "Incompatible Source operand");
  }
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "ParaSynInst can not accept vector types");
  return valid;
}

bool BrigModule::validateFirstLastbitInst(const inst_iterator inst) const {
  bool valid = true;

  const BrigInstSourceType *stype = dyn_cast<BrigInstSourceType>(inst);
  if (!check(stype, "Incorrect instruction kind"))
    return false;

  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_U32, "Type should be u32");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32 ||
                 *getType(dest) == BRIG_TYPE_B64,
                 "Type of destination should be b32 or b64");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be register, immediate or wave size");

  BrigType sourceType = BrigType(stype->sourceType);
  valid &= check(isCompatibleSrc(sourceType, src),
                 "Incompatible source operand");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "First and Last cannot accept vector types");
  return valid;
}

bool BrigModule::validateAbs(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Abs is not valid for bit types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateAdd(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Add is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateBorrow(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Borrow is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Borrow cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateCarry(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Carry is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Carry cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateCopySign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "CopySign is only valid for floating point types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateDiv(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Div is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateFma(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Fma is only valid for floating point type");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Fma cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateFract(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Fract is only valid for floating point type");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateMad(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Mad is not valid for bit types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mad cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMax(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Max is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMin(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Min is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMul(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Mul is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMulHi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "MulHi is only valid for signed and unsigned types");
  if (!BrigInstHelper::isVectorTy(BrigType(inst->type))) {
    valid &= check(inst->type == BRIG_TYPE_U32 || inst->type == BRIG_TYPE_S32 ||
                   inst->type == BRIG_TYPE_U64 || inst->type == BRIG_TYPE_S64,
                   "MulHi should be u32, s32, u64, or s64");
  }
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateNeg(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Neg is only valid for signed and floating point types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateRem(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Rem must be BrigInstBase");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Rem is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Rem cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateSqrt(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Sqrt is only valid for floating point types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateSub(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!BrigInstHelper::isBitTy(BrigType(inst->type)),
                 "Sub is not valid for bit types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMad24(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Mad24 is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mad24 cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMad24Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Mad24Hi is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mad24Hi cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateMul24(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Mul24 is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mul24 cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateMul24Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(BrigInstHelper::isSignedTy(BrigType(inst->type)) ||
                 BrigInstHelper::isUnsignedTy(BrigType(inst->type)),
                 "Mul24Hi is only valid for signed and unsigned types");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mul24Hi cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateShl(const inst_iterator inst) const {
  return validateShiftInst(inst);
}

bool BrigModule::validateShr(const inst_iterator inst) const {
  return validateShiftInst(inst);
}

bool BrigModule::validateAnd(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B1 || inst->type == BRIG_TYPE_B32 ||
                 inst->type == BRIG_TYPE_B64,
                 "Type of And should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "And cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateNot(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B1 || inst->type == BRIG_TYPE_B32 ||
                 inst->type == BRIG_TYPE_B64,
                 "Type of Not should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Not cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateOr(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B1 || inst->type == BRIG_TYPE_B32 ||
                 inst->type == BRIG_TYPE_B64,
                 "Type of Or should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Or cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validatePopCount(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstSourceType *stype = dyn_cast<BrigInstSourceType>(inst);
  if (!check(stype, "Incorrect instruction kind"))
    return false;

  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of PopCount shoud be u32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "PopCount cannot accept vector types");
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;

  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "Type Destination of PopCount must be BRIG_TYPE_B32");

  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be reg, immediate or Wavesize");

  BrigType sourceType = BrigType(stype->sourceType);
  if (isa<BrigOperandReg>(src) || isa<BrigOperandImmed>(src))
    valid &= check(isCompatibleSrc(sourceType, src),
                   "Incompatible source operand");

  return valid;
}

bool BrigModule::validateXor(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B1 || inst->type == BRIG_TYPE_B32 ||
                 inst->type == BRIG_TYPE_B64,
                 "Type of Xor should be b1, b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Xor cannot accept vector types");
  valid &= validateArithmeticInst(inst, 2);
  return valid;
}

bool BrigModule::validateBitRev(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32 || inst->type == BRIG_TYPE_B64,
                 "Type of BitRev should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "BitRev cannot accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateBitSelect(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32 || inst->type == BRIG_TYPE_B64,
                 "Type of BitSelect should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "BitSelect cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateExtract(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32 || inst->type == BRIG_TYPE_B64,
                 "Type of Extract should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Extract cannot accept vector types");
  if (!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;

  BrigType type = BrigType(inst->type);
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");

  for (int i = 1; i < 4; ++i) {
    oper_iterator src(S_.operands + inst->operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source should be reg, immediate or Wavesize");
  }

  oper_iterator src0(S_.operands + inst->operands[1]);
  if (isa<BrigOperandReg>(src0) || isa<BrigOperandImmed>(src0))
    valid &= check(isCompatibleSrc(type, src0), "Incompatible source operand");

  oper_iterator src1(S_.operands + inst->operands[2]);
  if (isa<BrigOperandReg>(src1) || isa<BrigOperandImmed>(src1))
    valid &= check(*getType(src1) == BRIG_TYPE_B32, "Type src1 of Extract "
                   "should be b32");

  oper_iterator src2(S_.operands + inst->operands[3]);
  if (isa<BrigOperandReg>(src2) || isa<BrigOperandImmed>(src2))
    valid &= check(*getType(src2) == BRIG_TYPE_B32, "Type src2 of Extract "
                   "should be b32");
  return valid;
}

bool BrigModule::validateFirstBit(const inst_iterator inst) const {
  return validateFirstLastbitInst(inst);
}

bool BrigModule::validateInsert(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32 || inst->type == BRIG_TYPE_B64,
                 "Type of Insert should be b32 or b64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Insert cannot accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  return valid;
}

bool BrigModule::validateLastBit(const inst_iterator inst) const {
  return validateFirstLastbitInst(inst);
}

bool BrigModule::validateLda(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstAddr>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_U32 || type == BRIG_TYPE_U64,
                 "Length should be 32 or 64");
  const BrigInstAddr *mem = dyn_cast<BrigInstAddr>(inst);
  if (!check(mem, "Invalid instruction kind")) return false;
  valid &= check(isCompatibleAddrSize(mem->segment, type),
                 "Incompatible address size");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Lda cannot accept vector types");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be BrigOperandReg");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandAddress>(src),
                 "Src should be BrigOperandAddress");
  valid &= check(isCompatibleAddrSize(mem->segment, *getType(src)),
                 "Incompatible address size");
  return valid;
}

bool BrigModule::validateLdc(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(inst->type == BRIG_TYPE_U32 || inst->type == BRIG_TYPE_U64,
                 "Length should be 32 or 64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Ldc cannot accept vector types");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be BrigOperandReg");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandLabelRef>(src) ||
                 isa<BrigOperandFunctionRef>(src),
                 "Src should be LabelRef and FunctionRef");
  if (isa<BrigOperandFunctionRef>(src) || isa<BrigOperandLabelRef>(src)) {
    const BrigDirectiveVersion *bdv = getFirstVersionDirective();
    if (!check(bdv, "Missing version?")) return false;
    if (bdv->machineModel == BRIG_MACHINE_LARGE)
      valid &= check(*getType(dest) == BRIG_TYPE_B64,
                     "Type of dest should be b64 if machine model is large");
    if (bdv->machineModel == BRIG_MACHINE_SMALL)
      valid &= check(*getType(dest) == BRIG_TYPE_B32,
                     "Type of dest should be b32 if machine model is small");
  }
  return valid;
}

bool BrigModule::validateMov(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Mov cannot accept vector types");
  BrigType type = BrigType(inst->type);
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest) ||
                 isa<BrigOperandRegVector>(dest),
                 "Destination of Mov should be reg, regVector");

  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandRegVector>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Src of Mov should be reg, regVector, immediate or wavesize");

  if (isa<BrigOperandReg>(dest))
    valid &= check(isCompatibleSrc(type, dest),
                   "Incompatible destination operand");

  if (isa<BrigOperandReg>(src))
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");

  if (isa<BrigOperandRegVector>(dest))
    valid &= check(isa<BrigOperandReg>(src) &&
                   *getType(src) == BRIG_TYPE_B64 &&
                   *getType(dest) == BRIG_TYPE_B32,
                   "Src should point to d reg and type of dest should be b32");

  if (isa<BrigOperandRegVector>(src))
    valid &= check(isa<BrigOperandReg>(dest) &&
                   *getType(dest) == BRIG_TYPE_B64 &&
                   *getType(src) == BRIG_TYPE_B32,
                   "Dest should point to d reg and type of src should be b32");
  return valid;
}

bool BrigModule::validateMovdHi(const inst_iterator inst) const {
  return validateMovdInst(inst);
}

bool BrigModule::validateMovdLo(const inst_iterator inst) const {
  return validateMovdInst(inst);
}

bool BrigModule::validateMovsHi(const inst_iterator inst) const {
  return validateMovsInst(inst);
}

bool BrigModule::validateMovsLo(const inst_iterator inst) const {
  return validateMovsInst(inst);
}

bool BrigModule::validateShuffle(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(inst->type == BRIG_TYPE_U16X2 ||
                 inst->type == BRIG_TYPE_S16X2 ||
                 inst->type == BRIG_TYPE_F16X2 ||
                 inst->type == BRIG_TYPE_U16X4 ||
                 inst->type == BRIG_TYPE_S16X4 ||
                 inst->type == BRIG_TYPE_F16X4 ||
                 inst->type == BRIG_TYPE_U32X2 ||
                 inst->type == BRIG_TYPE_S32X2 ||
                 inst->type == BRIG_TYPE_F32X2 ||
                 inst->type == BRIG_TYPE_U8X8  ||
                 inst->type == BRIG_TYPE_S8X8  ||
                 inst->type == BRIG_TYPE_U8X4  ||
                 inst->type == BRIG_TYPE_S8X4,
                 "Length of Shuffle should be 8x4, 8x8, 16x2, 16x4 or 32x2");

  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  oper_iterator src0(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src0) ||
                 isa<BrigOperandImmed>(src0), "Src0 should be reg or immed");
  oper_iterator src1(S_.operands + inst->operands[2]);
  valid &= check(isa<BrigOperandReg>(src1) ||
                 isa<BrigOperandImmed>(src1), "Src1 should be reg or immed");
  oper_iterator src2(S_.operands + inst->operands[3]);
  valid &= check(isa<BrigOperandImmed>(src2), "Src2 should be immed");

  for (int i = 0; i < 4; ++i) {
    oper_iterator reg(S_.operands + inst->operands[i]);
    valid &= check(isCompatibleSrc(type, reg), "Incompatible operand");
  }

  valid &= check(BrigInstHelper::isVectorTy(type),
                 "Shuffle should accept vector types");
  return valid;
}

bool BrigModule::validateUnpackHi(const inst_iterator inst) const {
  return validateUnpackInst(inst);
}

bool BrigModule::validateUnpackLo(const inst_iterator inst) const {
  return validateUnpackInst(inst);
}

bool BrigModule::validateCmov(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBasic>(inst), "Incorrect instruction kind");
  if (!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;

  BrigType type = BrigType(inst->type);
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");

  oper_iterator src0(S_.operands + inst->operands[1]);

  oper_iterator src1(S_.operands + inst->operands[2]);
  valid &= check(isCompatibleSrc(type, src1), "Incompatible src1 operand");

  oper_iterator src2(S_.operands + inst->operands[3]);
  valid &= check(isCompatibleSrc(type, src2), "Incompatible src2 operand");

  for (int i = 1; i < 4; ++i) {
    oper_iterator src(S_.operands + inst->operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source should be reg, immdiate, or wavesize");
  }

  if (BrigInstHelper::isVectorTy(type)) {
    valid &= check(BrigInstHelper::isSignedTy(type)   ||
                   BrigInstHelper::isUnsignedTy(type) ||
                   BrigInstHelper::isFloatTy(type),
                   "Invalid type");
    valid &= check(isCompatibleSrc(type, src0), "Incompatible src0 operand");
  } else {
    valid &= check(inst->type == BRIG_TYPE_B1 || inst->type == BRIG_TYPE_B32 ||
                   inst->type == BRIG_TYPE_B64,
                   "Type should be b1, b32 and b64");
    valid &= check(*getType(src0) == BRIG_TYPE_B1, "Type of Src0 should be b1");
  }
  return valid;
}

bool BrigModule::validateClass(const inst_iterator inst) const {
  bool valid = true;

  const BrigInstSourceType *stype = dyn_cast<BrigInstSourceType>(inst);
  if (!check(stype, "Incorrect instruction kind"))
    return false;

  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;

  BrigType sourceType = BrigType(stype->sourceType);
  valid &= check(BrigInstHelper::isFloatTy(sourceType),
                 "Type of Class should be f16, f32, or f64");

  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest),
                 "Destination should be a register");
  valid &= check(*getType(dest) == BRIG_TYPE_B1,
                 "Destination should be a c reg");

  for (int i = 1; i < 3; ++i) {
    oper_iterator src(S_.operands + inst->operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source should be register or immediate");
  }

  oper_iterator src0(S_.operands + inst->operands[1]);
  valid &= check(isCompatibleSrc(sourceType, src0),
                 "Incompatible src0 operand");

  oper_iterator src1(S_.operands + inst->operands[2]);
  if (getType(src1))
    valid &= check(*getType(src1) == BRIG_TYPE_B32,
                   "Type of src1 should be BRIG_TYPE_B32");

  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Class can not accept the vector");
  return valid;
}

bool BrigModule::validateNcos(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_F32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Fcos can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNexp2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_F32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Fexp2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNlog2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_F32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Flog2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNrcp(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Type should be f16, f32, or f64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Frcp can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNsqrt(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Type should be f16, f32, or f64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Fsqrt can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNrsqrt(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(BrigInstHelper::isFloatTy(BrigType(inst->type)),
                 "Type should be f16, f32, or f64");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Frsqrt can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateNsin(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_F32, "Type should be f32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Fsin can not accept vector types");
  valid &= validateArithmeticInst(inst, 1);
  return valid;
}

bool BrigModule::validateBitAlign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32, "Type of BitAlign should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "BitAlign can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");

  return valid;
}

bool BrigModule::validateByteAlign(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32,
                 "Type of ByteAlign should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "ByteAlign can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");

  return valid;
}

bool BrigModule::validateF2u4(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_U32, "Type of F2u4 should be u32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "F2u4 can not accept vector types");
  valid &= validateArithmeticInst(inst, 4);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateLerp(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_U8X4, "Type of Lerp should be u8x4");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");

  return valid;
}

bool BrigModule::validateSad(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstSourceType>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_U32, "Type of Sad should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Sad can not accept vector types");
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateSad2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32, "Type of Sad2 should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Sad2 can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateSad4(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32, "Type of Sad4 should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Sad4 can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateSad4Hi(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_B32, "Type of Sad4Hi should be b32");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(inst->type)),
                 "Sad4Hi can not accept vector types");
  valid &= validateArithmeticInst(inst, 3);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateUnpack0(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_F32, "Type of Unpack0 should be f32");
  valid &= validateArithmeticInst(inst, 1);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateUnpack1(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_F32, "Type of Unpack1 should be f32");
  valid &= validateArithmeticInst(inst, 1);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateUnpack2(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_F32, "Type of Unpack2 should be f32");
  valid &= validateArithmeticInst(inst, 1);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateUnpack3(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(!isa<BrigInstMod>(inst), "Incorrect instruction kind");
  valid &= check(inst->type == BRIG_TYPE_F32, "Type of Unpack3 should be f32");
  valid &= validateArithmeticInst(inst, 1);
  if (!valid) return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "dest should be register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "dest should be a s register");
  return valid;
}

bool BrigModule::validateSegmentp(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstSeg *mem = dyn_cast<BrigInstSeg>(inst);
  if (!check(mem, "Invalid instruction kind")) return false;
  BrigType type = BrigType(mem->type);
  valid &= check(type == BRIG_TYPE_B1, "Type of Segmentp should be b1");
  valid &= check(mem->segment < BRIG_SEGMENT_INVALID,
                 "StorageClass should be global, "
                 "group, private, kernarg, readonly, spill, or arg");
  oper_iterator dest(S_.operands + mem->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + mem->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src),
                 "Source should be reg, immediate");
  valid &= check(isCompatibleAddrSize(mem->segment, *getType(src)),
                 "Incompatible address size");
  return valid;
}

bool BrigModule::validateFtoS(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstSeg *mem = dyn_cast<BrigInstSeg>(inst);
  if (!check(mem, "Invalid instruction kind")) return false;
  valid &= check(mem->type == BRIG_TYPE_U32 || mem->type == BRIG_TYPE_U64,
                 "Type of FtoS should be u32 or u64");
  BrigType type = BrigType(mem->type);
  oper_iterator dest(S_.operands + mem->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleAddrSize(mem->segment, *getType(dest)),
                 "Incompatible address size");
  oper_iterator src(S_.operands + mem->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src),
                 "Source should be reg, immediate");
  valid &= check(isCompatibleAddrSize(BrigSegment(BRIG_SEGMENT_FLAT),
                                      *getType(src)),
                 "Incompatible address size");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "FtoS can not accept vector types");
  return valid;
}

bool BrigModule::validateStoF(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  const BrigInstSeg *mem = dyn_cast<BrigInstSeg>(inst);
  if (!check(mem, "Invalid instruction kind")) return false;
  BrigType type = BrigType(mem->type);
  valid &= check(type == BRIG_TYPE_U32 || type == BRIG_TYPE_U64,
                 "Type of StoF should be u32 or u64");
  valid &= check(isCompatibleAddrSize(BrigSegment(BRIG_SEGMENT_FLAT), type),
                 "Incompatible address size");
  oper_iterator dest(S_.operands + mem->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + mem->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be reg, immediate or waveSz");
  valid &= check(isCompatibleAddrSize(mem->segment, *getType(dest)),
                 "Incompatible address size");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "StoF can not accept vector types");
  return valid;
}

bool BrigModule::validateCmp(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "Cmp may not have an modifier");
  const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst);
  if (!check(cmp, "Invalid instruction kind")) return false;
  BrigType type = BrigType(cmp->type);
  BrigType srcTy = BrigType(cmp->sourceType);
  oper_iterator dest(S_.operands + cmp->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  for (int i = 1; i < 3; ++i) {
    oper_iterator src(S_.operands + cmp->operands[i]);
    if (BrigInstHelper::isFloatTy(type)) {
      valid &= check(isa<BrigOperandReg>(src) ||
                     isa<BrigOperandImmed>(src) ||
                     isa<BrigOperandWavesize>(src),
                     "Source should be reg, immediate, or wavesize");
    } else {
      valid &= check(isa<BrigOperandReg>(src)   ||
                     isa<BrigOperandImmed>(src) ||
                     isa<BrigOperandWavesize>(src),
                     "Source should be reg, immediate or wavesize");
    }

    valid &= check(isCompatibleSrc(srcTy, src), "Incompatible source operand");
  }
  if (BrigInstHelper::isUnsignedTy(srcTy) || BrigInstHelper::isSignedTy(srcTy))
    valid &= check(cmp->compare <= BRIG_COMPARE_GE, "Invalid compare");

  if (BrigInstHelper::isFloatTy(srcTy))
    valid &= check(cmp->compare <= BRIG_COMPARE_SGTU, "Invalid compare");

  return valid;
}

bool BrigModule::validatePackedCmp(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 3, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "PackedCmp may not have an modifier");
  const BrigInstCmp *cmp = dyn_cast<BrigInstCmp>(inst);
  if (!check(cmp, "Invalid instruction kind")) return false;
  BrigType type = BrigType(cmp->type);
  valid &= check(type == BRIG_TYPE_U8X4  || type == BRIG_TYPE_S8X4  ||
                 type == BRIG_TYPE_U8X8  || type == BRIG_TYPE_S8X8  ||
                 type == BRIG_TYPE_U16X2 || type == BRIG_TYPE_S16X2 ||
                 type == BRIG_TYPE_F16X2 ||
                 type == BRIG_TYPE_U16X4 || type == BRIG_TYPE_S16X4 ||
                 type == BRIG_TYPE_F16X4 ||
                 type == BRIG_TYPE_U32X2 || type == BRIG_TYPE_S32X2 ||
                 type == BRIG_TYPE_F32X2,
                 "Invalid type");
  oper_iterator dest(S_.operands + cmp->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination oper;and");
  for (int i = 1; i < 3; ++i) {
    oper_iterator src(S_.operands + cmp->operands[i]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed> (src),
                   "Source should be reg or immediate");
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  }

  if (BrigInstHelper::isUnsignedTy(type) || BrigInstHelper::isSignedTy(type))
    valid &= check(cmp->compare <= 5, "Invalid compare");

  if (BrigInstHelper::isFloatTy(type))
    valid &= check(cmp->compare <= 27, "Invalid compare");

  if (BrigInstHelper::isVectorTy(type)) {
    valid &= check(cmp->pack == BRIG_PACK_PP, "Packing should be pp");
  } else {
    valid &= check(cmp->pack == BRIG_PACK_NONE,
                   "Packing should be noPacking");
  }
  return valid;
}

bool BrigModule::validateCvt(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  valid &= check(BrigInstHelper::getAluModifier(inst),
                 "Cvt may not have an modifier");
  const BrigInstCvt *cvt = dyn_cast<BrigInstCvt>(inst);
  if (!check(cvt, "Invalid instruction kind")) return false;

  BrigType type = BrigType(cvt->type);
  valid &= check(BrigInstHelper::isBitTy(type)      ||
                 BrigInstHelper::isUnsignedTy(type) ||
                 BrigInstHelper::isSignedTy(type)   ||
                 BrigInstHelper::isFloatTy(type),
                 "Type should be unsigned, signed, float or bit");

  if (BrigInstHelper::isBitTy(type)) {
    valid &= check(BrigInstHelper::getTypeSize(type) == 1,
                   "Illegal data length");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(type) <= 64,
                   "Illegal data length");
  }

  BrigType srcTy = BrigType(cvt->sourceType);
  valid &= check(BrigInstHelper::isBitTy(srcTy) ||
                 BrigInstHelper::isUnsignedTy(srcTy) ||
                 BrigInstHelper::isSignedTy(srcTy) ||
                 BrigInstHelper::isFloatTy(srcTy),
                 "Type should be unsigned, signed float or bit");
  if (BrigInstHelper::isBitTy(srcTy)) {
    valid &= check(BrigInstHelper::getTypeSize(srcTy) == 1,
                   "Illegal data length");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(srcTy) <= 64,
                   "Illegal data length");
  }

  oper_iterator dest(S_.operands + cvt->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be reg");

  oper_iterator src(S_.operands + cvt->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be reg, immediate or wavesize");

  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Cvt can not accept vector types");

  return valid;
}

bool BrigModule::validateLd(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstMem>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest)   ||
                 isa<BrigOperandRegVector>(dest),
                 "Destination must be a register, regVector");
  BrigType type = BrigType(inst->type);
  const BrigType16_t *destTy = getType(dest);
  if (BrigInstHelper::isBitTy(type)) {
    valid &= check(BrigInstHelper::getTypeSize(type) ==
                   BrigInstHelper::getTypeSize(*destTy)
                   && 128 == BrigInstHelper::getTypeSize(type),
                   "Destination must be a q register if type is Bits");
  } else {
    valid &= check(BrigInstHelper::getTypeSize(type) <=
                   BrigInstHelper::getTypeSize(*destTy) &&
                   64 >= BrigInstHelper::getTypeSize(*destTy),
                   "Destination register is too small");
  }
  oper_iterator addr(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandAddress>(addr),
                 "address must be a BrigOperandAddress");
  const BrigInstMem *ldSt = dyn_cast<BrigInstMem>(inst);
  if (!check(ldSt, "Invalid instruction kind")) return false;
  valid &= check(isCompatibleAddrSize(ldSt->segment, *getType(addr)),
                 "Incompatible address size");

  return valid;
}

bool BrigModule::validateSt(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstMem>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest)   ||
                 isa<BrigOperandRegVector>(dest) ||
                 isa<BrigOperandImmed>(dest) ||
                 isa<BrigOperandWavesize>(dest),
                 "Destination must be a register, immediate, "
                 "registerVector, or wavesize");
  BrigType type = BrigType(inst->type);
  const BrigType16_t *destTy = getType(dest);
  if (BrigInstHelper::isBitTy(type)) {
    valid &= check(BrigInstHelper::getTypeSize(type) ==
                   BrigInstHelper::getTypeSize(*destTy)
                   && 128 == BrigInstHelper::getTypeSize(type),
                   "Destination must be a q register if type is b128");
  } else {
    if (destTy)
      valid &= check(BrigInstHelper::getTypeSize(type) <=
                     BrigInstHelper::getTypeSize(*destTy) &&
                     64 >= BrigInstHelper::getTypeSize(*destTy),
                     "Destination register is too small");
  }
  oper_iterator addr(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandAddress>(addr),
                 "address must be a BrigOperandAddress");
  const BrigInstMem *ldSt = dyn_cast<BrigInstMem>(inst);
  if (!check(ldSt, "Invalid instruction kind")) return false;
  valid &= check(isCompatibleAddrSize(ldSt->segment, *getType(addr)),
                 "Incompatible address size");
  return valid;
}

bool BrigModule::validateAtomicInst(const inst_iterator inst,
                                    bool isRet) const {
  bool valid = true;
  unsigned ret = isRet ? 1 : 0;
  const BrigInstAtomic *atomicInst = dyn_cast<BrigInstAtomic>(inst);
  if (!check(atomicInst, "Incorrect instruction kind"))
    return false;
  const unsigned numOperands = getNumOperands(inst);
  if (!check((2 + ret) == numOperands  ||
            ((3 + ret) == numOperands &&
             BRIG_ATOMIC_CAS == (atomicInst->atomicOperation)),
           "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  if (ret) {
    oper_iterator dest(S_.operands + inst->operands[0]);
    valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
    valid &= check(BrigInstHelper::isSignedTy(type)   ||
                   BrigInstHelper::isUnsignedTy(type) ||
                   BrigInstHelper::isBitTy(type),
                   "Invalid type");
    valid &= check(BrigInstHelper::getTypeSize(type) <= 64,
                   "Illegal data type");
    valid &= check(BrigInstHelper::getTypeSize(type) <=
                   BrigInstHelper::getTypeSize(*getType(dest)),
                   "Destination register is too small");
  }
  oper_iterator addr(S_.operands + inst->operands[ret]);
  valid &= check(isa<BrigOperandAddress>(addr),
                 "address must be a BrigOperandAddress, BrigOperandIndirect, "
                 " or BrigOperandCompound");
  valid &= check(isCompatibleAddrSize(atomicInst->segment, *getType(addr)),
                 "Incompatible address size");
  for (unsigned i = ret + 1; i < numOperands; ++i) {
    oper_iterator src(S_.operands + inst->operands[i]);
    valid &= check(isa<BrigOperandReg>(src)   ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source must be a register, immediate, or wave size");
    valid &= check(isCompatibleSrc(type, src), "Incompatible source operand");
  }
  return valid;
}

bool BrigModule::validateAtomic(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateAtomicInst(inst, true);
  return valid;
}

bool BrigModule::validateAtomicNoRet(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateAtomicInst(inst, false);
  return valid;
}

bool BrigModule::validateRdImage(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstImage *instRead = dyn_cast<BrigInstImage>(inst);
  if (!check(instRead, "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 4, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + instRead->operands[0]);
  valid &= check(isa<BrigOperandRegVector>(dest),
                 "Destination must be regVector");
  valid &= check(BrigInstHelper::getTypeSize(*getType(dest)) == 32,
                 "Illegal data type");

  oper_iterator src(S_.operands + instRead->operands[3]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandRegVector>(src),
                 "Src must be a register or regVector");
  valid &= check(BrigInstHelper::getTypeSize(*getType(src)) == 32,
                 "Illegal data type");
  valid &= check(!BrigInstHelper::isVectorTy(BrigType(instRead->type)),
                 "Image cannot accept vector types");
  BrigType type = BrigType(instRead->type);
  valid &= check(BrigInstHelper::getTypeSize(type) == 32, "Illegal data type");
  valid &= check(BrigInstHelper::isSignedTy(type)   ||
                 BrigInstHelper::isUnsignedTy(type) ||
                 BrigInstHelper::isFloatTy(type),
                 "Invalid type");
  return valid;
}

bool BrigModule::validateLdImage(const inst_iterator inst) const {
  return validateLdStImageInst(inst);
}

bool BrigModule::validateStImage(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstImage *image = dyn_cast<BrigInstImage>(inst);
  if (!check(image, "Invalid instruction kind")) return false;
  oper_iterator src0(S_.operands + image->operands[1]);
  valid &= validateLdStImageInst(inst);
  return valid;
}

bool BrigModule::validateAtomicImageInst(const inst_iterator inst,
                                         bool isRet) const {
  bool valid = true;
  const BrigInstAtomicImage *atoIm = dyn_cast<BrigInstAtomicImage>(inst);
  if (!check(atoIm, "Incorrect instruction kind"))
    return false;
  unsigned CAS = atoIm->atomicOperation == BRIG_ATOMIC_CAS ? 1 : 0;
  unsigned ret = isRet ? 1 : 0;
  valid &= check(3 + ret + CAS == getNumOperands(inst),
                 "Incorrect number of operands");
  BrigType type = BrigType(inst->type);
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Image cannot accept vector types");
  switch (type) {
    case BRIG_TYPE_U32:
    case BRIG_TYPE_S32:
    case BRIG_TYPE_U64:
    case BRIG_TYPE_S64:
      valid &= check(atoIm->atomicOperation == BRIG_ATOMIC_ADD ||
                     atoIm->atomicOperation == BRIG_ATOMIC_SUB ||
                     atoIm->atomicOperation == BRIG_ATOMIC_MIN ||
                     atoIm->atomicOperation == BRIG_ATOMIC_MAX ||
                     atoIm->atomicOperation == BRIG_ATOMIC_INC ||
                     atoIm->atomicOperation == BRIG_ATOMIC_DEC,
                     "Invalid type");
      break;
    case BRIG_TYPE_B32:
    case BRIG_TYPE_B64:
      valid &= check(atoIm->atomicOperation == BRIG_ATOMIC_AND ||
                     atoIm->atomicOperation == BRIG_ATOMIC_OR  ||
                     atoIm->atomicOperation == BRIG_ATOMIC_XOR ||
                     atoIm->atomicOperation == BRIG_ATOMIC_CAS ||
                     atoIm->atomicOperation == BRIG_ATOMIC_EXCH,
                     "Invalid type");
      break;
    default:
      valid &= check(false, "Invalid type");
      break;
  }
  if (isRet) {
    oper_iterator dest(S_.operands + inst->operands[0]);
    const BrigOperandReg *destReg = dyn_cast<BrigOperandReg>(dest);
    if (!check(destReg, "Destination must be register"))
      return false;
    if (inst->type == BRIG_TYPE_B32 || inst->type == BRIG_TYPE_B64)
      valid &= check(inst->type == destReg->type,
                     "Must be an s register for 32-bit types, "
                     " a d register for 64-bit types");
    valid &= check(BrigInstHelper::getTypeSize(type) <=
                   BrigInstHelper::getTypeSize(BrigType(destReg->type)),
                   "Destination register is too small");
  }

  oper_iterator image(S_.operands + inst->operands[ret]);
  oper_iterator regVector(S_.operands + inst->operands[1 + ret]);
  valid &= check(isa<BrigOperandReg>(regVector) ||
                 isa<BrigOperandRegVector>(regVector),
                 "regVector must be a register or regVector");
  oper_iterator src(S_.operands + inst->operands[2 + ret]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Src must be a register, immediate, or wave size");
  if (CAS) {
    oper_iterator src(S_.operands + inst->operands[3 + ret]);
    valid &= check(isa<BrigOperandReg>(src) ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Src must be a register, immediate, or wave size");
  }

  return valid;
}

bool BrigModule::validateAtomicImage(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateAtomicImageInst(inst, true);
  return valid;
}

bool BrigModule::validateAtomicNoRetImage(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateAtomicImageInst(inst, false);
  return valid;
}

bool BrigModule::validateQueryImageArray(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryArray should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageDepth(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryDepth should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQuerySamplerFilter(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryFiltering should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageFormat(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQuerySamplerCoord(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageHeight(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryHeight should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageNormalized(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryNormalized should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageOrder(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryOrder should be b32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateQueryImageWidth(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32,
                 "Type of QueryWidth should be u32");
  valid &= validateImageQueryInst(inst);
  return valid;
}

bool BrigModule::validateBranchInst(const inst_iterator inst,
                                    unsigned nary) const {
  bool valid = true;

  if (!check(isa<BrigInstBr>(inst),
            "Incorrect instruction kind"))
    return false;

  const unsigned numOperands = getNumOperands(inst);

  if (!check(numOperands == nary || numOperands == nary + 1,
            "Incorrect number of operands"))
    return false;

  if (nary == 2) {
    oper_iterator dest(S_.operands + inst->operands[0]);
    valid &= check(isa<BrigOperandReg>(dest),
                     "Destination must be a Reg");
    valid &= check(1 == BrigInstHelper::getTypeSize(*getType(dest)),
                     "Invalid type size");
  }

  if (numOperands == nary) {
    oper_iterator labelOrReg(S_.operands + inst->operands[nary - 1]);
    valid &= check(isa<BrigOperandLabelRef>(labelOrReg)   ||
                     isa<BrigOperandReg>(labelOrReg),
                     "Destination must be a LabelRef or Reg");
  }

  if (numOperands == nary + 1) {
    oper_iterator dest(S_.operands + inst->operands[nary - 1]);
    valid &= check(isa<BrigOperandReg>(dest),
                     "Destination must be a Reg");
    valid &= check(32 == BrigInstHelper::getTypeSize(*getType(dest)) ||
                   64 == BrigInstHelper::getTypeSize(*getType(dest)),
                     "Invalid type size");
    oper_iterator labelOrAddr(S_.operands + inst->operands[nary]);
    valid &= check(isa<BrigOperandLabelRef>(labelOrAddr)   ||
                     isa<BrigOperandAddress>(labelOrAddr),
                     "Destination must be a LabelRef or Address");
  }
  return valid;
}

bool BrigModule::validateCbr(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateBranchInst(inst, 2);
  return valid;
}

bool BrigModule::validateBrn(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateBranchInst(inst, 1);
  return valid;
}

bool BrigModule::validateBarrier(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 0, "Incorrect number of operands"))
    return false;
  const BrigInstBar *bib = dyn_cast<BrigInstBar>(inst);
  if (!check(bib, "Invalid instruction kind")) return false;
  BrigType type = BrigType(bib->type);
  valid &= check(type == BRIG_TYPE_NONE, "Type of dest should be none");
  valid &= check(bib->memoryFence <= BRIG_FENCE_PARTIAL_BOTH,
                 "SyncFlags should be global, group, or both");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Barrier can not accept vector types");
  return valid;
}

bool BrigModule::validateFbarArrive(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarInit(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B64, "Type should be b64");
  valid &= validateParaSynInst(inst, 1);
  return valid;
}

bool BrigModule::validateFbarRelease(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarSkip(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateFbarWait(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B64, "Type should be b64");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateSync(const inst_iterator inst) const {
  if (!check(getNumOperands(inst) == 0, "Incorrect number of operands"))
    return false;
  return true;
}

bool BrigModule::validateCountLane(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operand"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_U32, "Type should be u32");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be register, immediate or waveSz");
  if (getType(src))
    valid &= check(*getType(src) == BRIG_TYPE_B1 ||
                   *getType(src) == BRIG_TYPE_B32,
                   "Source should be c or s register");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Count can not accept vector types");
  return valid;
}

bool BrigModule::validateCountUpLane(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_U32, "Type should be u32");
  valid &= validateParaSynInst(inst, 0);
  return valid;
}

bool BrigModule::validateMaskLane(const inst_iterator inst) const {
  bool valid = true;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operand"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(type == BRIG_TYPE_B64, "Type should be b64");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination should be register");
  valid &= check(isCompatibleSrc(type, dest),
                 "Incompatible destination operand");
  oper_iterator src(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandReg>(src) ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source should be register, immediate or waveSz");
  if (getType(src))
    valid &= check(*getType(src) == BRIG_TYPE_B1 ||
                   *getType(src) == BRIG_TYPE_B32,
                   "Source should be c or s register");
  valid &= check(!BrigInstHelper::isVectorTy(type),
                 "Mask can not accept vector types");
  return valid;
}

bool BrigModule::validateSendLane(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B32, "Type should be b32");
  valid &= validateParaSynInst(inst, 2);
  return valid;
}

bool BrigModule::validateReceiveLane(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(inst->type == BRIG_TYPE_B32, "Type should be b32");
  valid &= validateParaSynInst(inst, 2);
  return valid;
}

bool BrigModule::validateCall(const inst_iterator inst) const {
  bool valid = true;
  valid &= check(isa<BrigInstBr>(inst),
                 "Incorrect instruction kind");
  const unsigned numOperands = getNumOperands(inst);
  if (!check(3 <= numOperands, "Incorrect number of operands"))
    return false;
  for (unsigned i = 0; i < 3; i += 2) {
    oper_iterator args(S_.operands + inst->operands[i]);
    valid &= check(isa<BrigOperandArgumentList>(args),
                   "args must be an argumentList");
  }
  oper_iterator func(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandFunctionRef>(func) ||
                 isa<BrigOperandReg>(func),
                 "functionRef must be a register or function reference");
  if (4 == numOperands) {
    oper_iterator funcs(S_.operands + inst->operands[3]);
    valid &= check(isa<BrigOperandFunctionList>(funcs),
                   "funcs must be a functionList");
  }
  return valid;
}

bool BrigModule::validateRet(const inst_iterator inst) const {
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 0, "Incorrect number of operands"))
    return false;
  return true;
}

bool BrigModule::validateSysCall(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 5, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(32 == BrigInstHelper::getTypeSize(*getType(dest)) ||
                 64 == BrigInstHelper::getTypeSize(*getType(dest)),
                 "Destination register type must be b32 or b64");
  oper_iterator number(S_.operands + inst->operands[1]);
  valid &= check(isa<BrigOperandImmed>(number) ||
                 isa<BrigOperandWavesize>(number),
                 "number must be an immediate or wavesize");

  if (getType(number))
    valid &= check(32 == BrigInstHelper::getTypeSize(*getType(number)) ||
                   64 == BrigInstHelper::getTypeSize(*getType(number)),
                   "immediate type must be b32 or b64");
  for (unsigned i = 2; i < 5; ++i) {
    oper_iterator src(S_.operands + inst->operands[i]);
    if (!isa<BrigOperandWavesize>(src))
      valid &= check(BrigInstHelper::getTypeSize(*getType(src)) == 32 ||
                     BrigInstHelper::getTypeSize(*getType(src)) == 64,
                     "Illegal data type");
    valid &= check(isa<BrigOperandReg>(src)   ||
                   isa<BrigOperandImmed>(src) ||
                   isa<BrigOperandWavesize>(src),
                   "Source must be a register, immediate, or wave size");
  }
  return valid;
}

bool BrigModule::validateAlloca(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstSeg>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 2, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(32 == BrigInstHelper::getTypeSize(*getType(dest)),
                 "Destination register type must be b32");

  oper_iterator src(S_.operands + inst->operands[1]);
  if (!isa<BrigOperandWavesize>(src))
    valid &= check(BrigInstHelper::getTypeSize(*getType(src)) == 32,
                   "Illegal data type");
  valid &= check(isa<BrigOperandReg>(src)   ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source must be a register, immediate, or wave size");

  return valid;
}

bool BrigModule::validateSpecialInst(const inst_iterator inst,
                                     unsigned nary) const {
  bool valid = true;
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == nary + 1, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(32 == BrigInstHelper::getTypeSize(*getType(dest)) ||
                 64 == BrigInstHelper::getTypeSize(*getType(dest)),
                 "Destination register type must be b32");
  if (1 == nary) {
    oper_iterator number(S_.operands + inst->operands[1]);
    valid &= check(32 == BrigInstHelper::getTypeSize(*getType(number)),
                   "Invalid type");
    const BrigOperandImmed *dimension = dyn_cast<BrigOperandImmed>(number);
    if (!check(dimension, "number must be a BrigOperandImmed"))
      return false;
  }
  return valid;
}

bool BrigModule::validateClock(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 1, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(64 == BrigInstHelper::getTypeSize(*getType(dest)),
                 "Destination register type must be b64");
  return valid;
}

bool BrigModule::validateCU(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateCurrentWorkGroupSize(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateDebugTrap(const inst_iterator inst) const {
  bool valid = true;
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 1, "Incorrect number of operands"))
    return false;
  oper_iterator src(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(src)   ||
                 isa<BrigOperandImmed>(src) ||
                 isa<BrigOperandWavesize>(src),
                 "Source must be a register, immediate, or wave size");
  return valid;
}

bool BrigModule::validateDispatchId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateDynWaveId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateLaneId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateMaxDynWaveId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateGridGroups(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateGridSize(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateNop(const inst_iterator inst) const {
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 0, "Incorrect number of operands"))
    return false;
  return true;
}

bool BrigModule::validateNullPtr(const inst_iterator inst) const {
  bool valid = true;
  const BrigInstSeg *mem = dyn_cast<BrigInstSeg>(inst);
  if (!check(mem, "Invalid instruction kind")) return false;
  if (!check(getNumOperands(inst) == 1, "Incorrect number of operands"))
    return false;
  BrigType type = BrigType(inst->type);
  valid &= check(BrigInstHelper::isUnsignedTy(type), "Invalid type");
  valid &= check(32 == BrigInstHelper::getTypeSize(type) ||
                 64 == BrigInstHelper::getTypeSize(type), "Illegal data type");
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");

  valid &= check(isCompatibleAddrSize(mem->segment, *getType(dest)),
                 "Incompatible address size");
  return valid;
}

bool BrigModule::validateQid(const inst_iterator inst) const {
  bool valid = true;
  // maybe BrigInstBase, or other.
  if (!check(isa<BrigInstBasic>(inst), "Incorrect instruction kind"))
    return false;
  if (!check(getNumOperands(inst) == 1, "Incorrect number of operands"))
    return false;
  oper_iterator dest(S_.operands + inst->operands[0]);
  valid &= check(isa<BrigOperandReg>(dest), "Destination must be a register");
  valid &= check(*getType(dest) == BRIG_TYPE_B32,
                 "Type of destination should be s register");
  return valid;
}

bool BrigModule::validateWorkDim(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateWorkGroupId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateWorkGroupSize(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateWorkItemAbsId(const inst_iterator inst) const {
  return true;
}

bool BrigModule::validateWorkItemAbsIdFlat(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateWorkItemId(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 1);
  return valid;
}

bool BrigModule::validateWorkItemIdFlat(const inst_iterator inst) const {
  bool valid = true;
  valid &= validateSpecialInst(inst, 0);
  return valid;
}

bool BrigModule::validateCombine(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateBitExtract(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateCeil(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateFloor(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateRint(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateTrunc(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateBitMask(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateExpand(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateNFma(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateSadhi(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validatePackCvt(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateUnpackCvt(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateArriveFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateInitFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateJoinFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateLeaveFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateReleaseFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateWaitFbar(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateLdf(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateCuId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateDim(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateGetDetectExcept(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateMaxCuId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateMaxWaveId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateWaveId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateWorkItemFlatAbsId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateWorkItemFlatId(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateClearDetectExcept(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateSetDetectExcept(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateDispatchPtr(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

bool BrigModule::validateQPtr(const inst_iterator inst) const {
  bool valid = true;
  return valid;
}

const BrigDirectiveVersion* BrigModule::getFirstVersionDirective() const {
  return dyn_cast<BrigDirectiveVersion>(S_.begin());
}

BrigStringOffset32_t BrigModule::lookupFilename(uint32_t fileid) const {
  dir_iterator it = S_.begin();
  const dir_iterator E = S_.end();

  while (it != E) {
    const BrigDirectiveFile *dir = dyn_cast<BrigDirectiveFile>(it);
    if (dir && dir->fileid == fileid) {
      return dir->filename;
    }
  }
  return 0;
}

}  // namespace brig
}  // namespace hsa
