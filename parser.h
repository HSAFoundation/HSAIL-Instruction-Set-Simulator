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

// classify token into different terminal type
TerminalType GetTokenType(int token);

int Query(int queryOp);
int Operand(int first_token);
int Identifier(int first_token);
int BaseOperand(int first_token);
int AddressableOperand(int first_token);
int ArrayOperandList(int first_token);
int RoundingMode(int first_token, bool* is_ftz, int* last_token);
int Instruction2(int first_token);
int Instruction3(int first_token);
int Version(int first_token, Context* context);
int Alignment(int first_token);
int DeclPrefix(int first_token, bool* rescan_last_token, int* last_token);
int FBar(int first_token);
int ArrayDimensionSet(int first_token,
                      bool* rescan_last_token,
                      int* last_token);
int ArgumentDecl(int first_token, bool* rescan_last_token, int* last_token);
int ArgumentListBody(int first_token, bool* rescan_last_token, int* last_token);
int FunctionDefinition(int first_token,
                       bool* rescan_last_token,
                       int* last_token);
int FunctionDecl(int first_token);
int Codeblock(int first_token);
int Function(int first_token);
int OptionalWidth(int first_token);
int Program(int first_token, Context* context);
int Branch(int first_token);
int CallTargets(int first_token);
int CallArgs(int first_token);
int Call(int first_token);
int Initializer(int first_token,
                bool* rescan,
                int* last_token);
int InitializableDecl(int first_token);
int UninitializableDecl(int first_token);
int ArgUninitializableDecl(int first_token);
}  // namespace brig
}  // namespace hsa
#endif  // PARSER_H_

