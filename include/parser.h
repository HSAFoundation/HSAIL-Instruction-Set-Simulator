// Copyright 2012 MulticoreWare Inc.

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <string>
#include "context.h"
namespace hsa {
namespace brig {

int Query(Context* context);
int Operand(Context* context);
int Identifier(Context* context);
int BaseOperand(Context* context);
int AddressableOperand(Context* context);
int ArrayOperandList(Context* context);
int RoundingMode(Context* context);
int FunctionSignature(Context* context);
int Instruction2(Context* context);
int Instruction3(Context* context);
int Version(Context* context);
int Alignment(Context* context);
int SignatureArgumentList(Context *context);
int Label(Context* context);
int LabelTargets(Context* context);
int Instruction4(Context* context);
int DeclPrefix(Context* context);
int FBar(Context* context);
int ArrayDimensionSet(Context* context);
int ArgumentDecl(Context* context);
int ArgumentListBody(Context* context);
int FunctionDefinition(Context* context);
int FunctionDecl(Context* context);
int Codeblock(Context* context);
int Function(Context* context);
int OptionalWidth(Context* context);
int Program(Context* context);
int Branch(Context* context);
int CallTargets(Context* context);
int CallArgs(Context* context);
int Call(Context* context);
int Initializer(Context* context);
int InitializableDecl(Context* context);
int InitializableDeclPart2(Context* context, BrigStorageClass32_t storage_class);
int UninitializableDecl(Context* context);
int ArgUninitializableDecl(Context* context);
int VectorToken(Context *context);
int SignatureType(Context *context);
int FileDecl(Context* context);
int SysCall(Context* context);
int ArgBlock(Context* context);
int Extension(Context* context);
int KernelArgumentDecl(Context *context);
int KernelArgumentListBody(Context *context);
int Kernel(Context *context);
int OperandList(Context* context);
int Cmp(Context* context);
int GlobalPrivateDecl(Context* context);
int OffsetAddressableOperand(Context* context, BrigoOffset32_t addrOpOffset);
int MemoryOperand(Context* context);
int Ldc(Context* context);
int Instruction5(Context* context);
int CvtModifier1(Context* context);
int Mov(Context* context);
int Atom(Context* context);
int Instruction0(Context* context);
int Instruction1(Context* context);
int Operation(Context* context);
int Segp(Context* context);

int BodyStatementNested(Context* context);
int ArgStatement(Context* context);
int ArgStatements(Context* context);
int BodyStatement(Context* context);
int BodyStatements(Context* context);
int GlobalGroupDecl(Context* context);

/* MUL */
int Mul(Context* context);
int MulInst(Context* context);
int Mul24Inst(Context* context);
int Mad24Inst(Context* context);
int Ld(Context* context);
int LdModifier(Context* context, BrigInstLdSt* pLdSt_op, int* pVec_size);
int St(Context* context);
int Lda(Context* context);
int ImageRet(Context* context);
int ImageNoRet(Context* context);
int Cvt(Context* context);
int ImageLoad(Context* context);
int ImageStore(Context* context);

int SingleListSingle(Context *context);
int GlobalImageDecl(Context *context);
int GlobalImageDeclPart2(Context *context);
int ImageInitializer(Context *context);
int ImageInit(Context *context);
int GlobalReadOnlyImageDecl(Context *context);
int GlobalReadOnlyImageDeclPart2(Context *context);
int Ret(Context *context);
int ImageRead(Context *context);
int Sync(Context *context);
int Bar(Context *context);
int AtomicNoRet(Context *context);
int AtomModifiers(Context *context);
int Location(Context* context);
int Control(Context* context);
int Pragma(Context* context);
int Block(Context* context);
int DecimalListSingle(Context* context);
int FloatListSingle(Context* context);
int GlobalSymbolDecl(Context* context);
int Directive(Context* context);
int LabelList(Context* context);

int SobInit(Context *context);
int SobInitializer(Context *context);
int GlobalSamplerDecl(Context *context);
int GlobalSamplerDeclPart2(Context *context);
int GlobalDecl(Context *context);
int GlobalInitializablePart2(Context* context);
int GlobalInitializable(Context* context);

int SequenceOfPrograms(Context *context);
int PairAddressableOperand(Context *context);
int LdaMod(Context *context);
int OffsetAddressableOperand(Context* context);
int LdModifier(Context* context);

int TopLevelStatement(Context *context);
int TopLevelStatements(Context *context);

int Optacqreg(Context* context);

int ArrayOperand(Context* context);
int QueryOp(Context* context);
int ComparisonId(Context* context);
int CodeBlockEnd(Context* context);
int Comment(Context* context);

//Declarations of Helper Functions 
int GlobalImageDeclPart2(Context *context);
int GlobalReadOnlyImageDeclPart2(Context *context);
int GlobalInitializablePart2(Context* context);
int GlobalSamplerDeclPart2(Context *context);
int InitializableDeclPart2(Context* context, BrigStorageClass32_t storage_class);
int OffsetAddressableOperandPart2(Context* context, BrigoOffset32_t addrOpOffset, BrigoOffset32_t* pRetOpOffset);
int LdModifierPart2(Context* context, BrigInstLdSt* pLdSt_op, int* pVec_size);
int AtomModifiersPart2(Context* context, BrigStorageClass32_t* pStorageClass, 
                       BrigMemorySemantic32_t* pMemorySemantic);
int OptacqregPart2(Context* context, BrigMemorySemantic32_t* memSemantic);
int ArrayOperandListPart2(Context* context, BrigoOffset32_t* pRetOpOffset);

int ArrayOperandPart2(Context* context, BrigoOffset32_t* pOperand);
int QueryOpPart2(Context* context, BrigDataType16_t* pDataType, BrigOpcode32_t* pOpcode);
int AddressableOperandPart2(Context* context, BrigoOffset32_t* pRetOpOffset, bool IsImageOrSampler);
int ComparisonIdPart2(Context* context, BrigCompareOperation32_t* pCmpOperation);

int Instruction4MultiMediaOperationPart1(Context* context);
int Instruction4FmaPart2(Context* context);
int Instruction4MadPart3(Context* context);
int Instruction4BitStringOperationPart4(Context* context);
int Instruction4CmovPart5(Context* context);
int Instruction4ShufflePart6(Context* context);
int FunctionDefinitionPart2(Context* context);
int MemoryOperandPart2(Context* context, BrigoOffset32_t* pRetOpOffset);
int OperandPart2(Context* context, BrigoOffset32_t* pRetOpOffset);
int LabelTargetsPart2(Context* context);

int Instruction1Part1OpcodeDT(Context* context);
int Instruction1Part2OpcodeNoDT(Context* context);
int Instruction1Part3Clock(Context* context);

int Instruction2Part1OpcodeDT(Context* context);
int Instruction2Part2OpcodeNoDT(Context* context);
int Instruction2Part3OpcodeFtz(Context* context);

int BranchPart1Cbr(Context* context);
int BranchPart2Brn(Context* context);

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_H_

