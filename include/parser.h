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
int SignatureArgumentList(Context *context) ;
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
int UninitializableDecl(Context* context);
int ArgUninitializableDecl(Context* context);
int VectorToken(Context *context);
int SignatureType(Context *context);
int FileDecl(Context* context);
int SysCall(Context* context);
int ArgBlock(Context* context);
int Extension(Context* context);
int KernelArgumentDecl(Context *context) ;
int KernelArgumentListBody(Context *context) ;
int Kernel(Context *context);
int OperandList(Context* context);
int Cmp(Context* context);
int GlobalPrivateDecl(Context* context);
int OffsetAddressableOperand(Context* context);
int MemoryOperand(Context* context);
int Ldc(Context* context);
int Instruction5(Context* context);
int CvtModifier1(Context* context);
int Mov(Context* context);
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_H_

