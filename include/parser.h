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
int RIW_Operand(Context* context);
int BodyStatementNested(Context* context);
int ArgStatement(Context* context);
int ArgStatements(Context* context);

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

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_H_

