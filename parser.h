// Copyright 2012 MulticoreWare Inc.

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "./tokens.h"

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
                    TARGET
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
int Version(int first_token);
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
#endif  // PARSER_H_

