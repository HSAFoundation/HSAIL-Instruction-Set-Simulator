#ifndef VALIDATE_BRIG
#define VALIDATE_BRIG

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "error_reporter.h"
#include "context.h"
#include "brig.h"
#include "brig_util.h"
#include "brig_buffer.h"

namespace hsa{
namespace brig{

#define OPERANDSBUFFER_OFFSET 8
#define CODEBUFFER_OFFSET 8
#define DIRECTIVESBUFFER_OFFSET 8

namespace validate_brig{

void validate(const BrigInstBase* ref, const BrigInstBase* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstMod* ref, const BrigInstMod* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstCvt* ref, const BrigInstCvt* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstBar* ref, const BrigInstBar* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstLdSt* ref, const BrigInstLdSt* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstCmp* ref, const BrigInstCmp* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstMem* ref, const BrigInstMem* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstAtomic* ref, const BrigInstAtomic* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstAtomicImage* ref, const BrigInstAtomicImage* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstImage* ref, const BrigInstImage* get,
      BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigInstSegp* ref, const BrigInstSegp* get,
      BrigSections* RefOutput, BrigSections* GetOutput);

void validate(const BrigOperandBase* ref, const BrigOperandBase* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandReg* ref, const BrigOperandReg* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandImmed* ref, const BrigOperandImmed* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandRegV2* ref, const BrigOperandRegV2* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandRegV4* ref, const BrigOperandRegV4* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandAddress* ref, const BrigOperandAddress* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandLabelRef* ref, const BrigOperandLabelRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandIndirect* ref, const BrigOperandIndirect* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandCompound* ref, const BrigOperandCompound* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandArgumentList* ref, const BrigOperandArgumentList* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandArgumentRef* ref, const BrigOperandArgumentRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandWaveSz* ref, const BrigOperandWaveSz* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandFunctionRef* ref, const BrigOperandFunctionRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigOperandOpaque* ref, const BrigOperandOpaque* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveBase* ref, const BrigDirectiveBase* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveComment* ref, const BrigDirectiveComment* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveControl* ref, const BrigDirectiveControl* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveExtension* ref, const BrigDirectiveExtension* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveFile* ref, const BrigDirectiveFile* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveFunction* ref, const BrigDirectiveFunction* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveImage* ref, const BrigDirectiveImage* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveInit* ref, const BrigDirectiveInit* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveLabel* ref, const BrigDirectiveLabel* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveLabelList* ref, const BrigDirectiveLabelList* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveLabelInit* ref, const BrigDirectiveLabelInit* get,
    BrigSections* RefOutput, BrigSections* GetOutput);

void validate(const BrigDirectiveKernel* ref, const BrigDirectiveKernel* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveLoc* ref, const BrigDirectiveLoc* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const  BrigDirectivePad* ref, const BrigDirectivePad* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectivePragma* ref, const BrigDirectivePragma* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveSignature* ref, const BrigDirectiveSignature* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveSampler* ref, const BrigDirectiveSampler* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveScope* ref, const BrigDirectiveScope* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveSymbol* ref, const BrigDirectiveSymbol* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigDirectiveVersion* ref, const BrigDirectiveVersion* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigBlockStart* ref, const BrigBlockStart* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigBlockString* ref, const BrigBlockString* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigBlockNumeric* ref, const BrigBlockNumeric* get,
    BrigSections* RefOutput, BrigSections* GetOutput);
void validate(const BrigBlockEnd* ref, const BrigBlockEnd* get,
    BrigSections* RefOutput, BrigSections* GetOutput);

#define caseOperBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refoper)), (cast<Brig ## X>(getoper)),  \
      RefOutput, GetOutput);   \
    break


#define caseDirBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refdir)), (cast<Brig ## X>(getdir)),  \
      RefOutput, GetOutput);   \
    break

#define caseInstBrig(X)                                   \
  case BrigE ## X:                                    \
    validate_brig::validate((cast<Brig ## X>(refinst)), (cast<Brig ## X>(getinst)),  \
      RefOutput, GetOutput);   \
    break


}
}
}
#endif

