// Copyright 2012 MulticoreWare Inc.

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <string>
#include "context.h"

namespace hsa {
namespace brig {

// scan the source code and add symbols to string buffer
void ScanString(unsigned int first_token, Context* context);
// classify token into different terminal type
int Query(unsigned int queryOp, Context* context);
int Operand(unsigned int first_token, Context* context);
int Identifier(unsigned int first_token, Context* context);
int BaseOperand(unsigned int first_token, Context* context);
int AddressableOperand(unsigned int first_token, Context* context);
int ArrayOperandList(unsigned int first_token, Context* context);
int RoundingMode(unsigned int first_token,
                 bool* is_ftz,
                 unsigned int* last_token,
                 Context* context);

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

