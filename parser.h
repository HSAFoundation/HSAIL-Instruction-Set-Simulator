// Copyright 2012 MulticoreWare Inc.

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "./tokens.h"
#include "include/brig.h"
#include "include/brig_buffer.h"

namespace hsa {
namespace brig {
enum TerminalType { UNKNOWN_TERM = 0,
                    REGISTER,
                    DATA_TYPE_ID,
                    QUERY_OP,
                    INT_ROUNDING,
                    FLOAT_ROUNDING,
                    PACKING,
                    INSTRUCTION2_OPCODE,
                    INSTRUCTION2_OPCODE_NODT,
                    INSTRUCTION2_OPCODE_FTZ,
                    INSTRUCTION3_OPCODE,
                    INSTRUCTION3_OPCODE_FTZ,
                    TARGET,
                    INITIALIZABLE_ADDRESS,
                    UNINITIALIZABLE_ADDRESS
                  };

// scan the source code and add symbols to string buffer
void ScanString(int first_token, Context* context);
// classify token into different terminal type
TerminalType GetTokenType(int token);

int Query(int queryOp, Context* context);
int Operand(int first_token, Context* context);
int Identifier(int first_token, Context* context);
int BaseOperand(int first_token, Context* context);
int AddressableOperand(int first_token, Context* context);
int ArrayOperandList(int first_token, Context* context);
int RoundingMode(int first_token,
                 bool* is_ftz,
                 int* last_token,
                 Context* context);

int Instruction2(int first_token, Context* context);
int Instruction3(int first_token, Context* context);
int Version(int first_token, Context* context);
int Alignment(int first_token, Context* context);
int DeclPrefix(int first_token,
               bool* rescan_last_token,
               int* last_token,
               Context* context);
int FBar(int first_token, Context* context);
int ArrayDimensionSet(int first_token,
                      bool* rescan_last_token,
                      int* last_token,
                      Context* context);
int ArgumentDecl(int first_token,
                 bool* rescan_last_token,
                 int* last_token,
                 Context* context);
int ArgumentListBody(int first_token,
                     bool* rescan_last_token,
                     int* last_token,
                     Context* context);
int FunctionDefinition(int first_token,
                       bool* rescan_last_token,
                       int* last_token,
                       Context* context);
int FunctionDecl(int first_token, Context* context);
int Codeblock(int first_token, Context* context);
int Function(int first_token, Context* context);
int OptionalWidth(int first_token, Context* context);
int Program(int first_token, Context* context);
int Branch(int first_token, Context* context);
int CallTargets(int first_token, Context* context);
int CallArgs(int first_token, Context* context);
int Call(int first_token, Context* context);
int Initializer(int first_token,
                bool* rescan,
                int* last_token,
                Context* context);
int InitializableDecl(int first_token, Context* context);
int UninitializableDecl(int first_token, Context* context);
int ArgUninitializableDecl(int first_token, Context* context);
}  // namespace brig
}  // namespace hsa
#endif  // PARSER_H_

