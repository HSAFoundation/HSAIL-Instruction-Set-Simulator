// Copyright 2012 MulticoreWare Inc.

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <string>
#include "context.h"
namespace hsa {
namespace brig {

// classify token into different terminal type
int Query(Context* context);
int Operand(Context* context);
int Identifier(Context* context);
int BaseOperand(Context* context);
int AddressableOperand(Context* context);
int ArrayOperandList(unsigned int first_token, Context* context);
int RoundingMode(Context* context);

int Instruction2(unsigned int first_token, Context* context);
int Instruction3(unsigned int first_token, Context* context);
int Version(unsigned int first_token, Context* context);
int Alignment(unsigned int first_token, Context* context);
int DeclPrefix(unsigned int first_token,
               bool* rescan_last_token,
               unsigned int* last_token,
               Context* context);
int FBar(unsigned int first_token, Context* context);
int ArrayDimensionSet(unsigned int first_token,
                      bool* rescan_last_token,
                      unsigned int* last_token,
                      Context* context);
int ArgumentDecl(unsigned int first_token,
                 bool* rescan_last_token,
                 unsigned int* last_token,
                 Context* context);
int ArgumentListBody(unsigned int first_token,
                     bool* rescan_last_token,
                     unsigned int* last_token,
                     Context* context);
int FunctionDefinition(unsigned int first_token,
                       bool* rescan_last_token,
                       unsigned int* last_token,
                       Context* context);
int FunctionDecl(unsigned int first_token, Context* context);
int Codeblock(unsigned int first_token, Context* context);
int Function(unsigned int first_token, Context* context);
int OptionalWidth(unsigned int first_token, Context* context);
int Program(unsigned int first_token, Context* context);
int Branch(unsigned int first_token, Context* context);
int CallTargets(unsigned int first_token, Context* context);
int CallArgs(unsigned int first_token, Context* context);
int Call(unsigned int first_token, Context* context);
int Initializer(unsigned int first_token,
                bool* rescan,
                unsigned int* last_token,
                Context* context);
int InitializableDecl(unsigned int first_token, Context* context);
int UninitializableDecl(unsigned int first_token, Context* context);
int ArgUninitializableDecl(unsigned int first_token, Context* context);
int VectorToken(unsigned int first_token , Context *context);
int SignatureType(unsigned int first_token , Context *context);
int FileDecl(unsigned int first_token, Context* context);
int SysCall(unsigned int first_token, Context* context);


}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_H_

