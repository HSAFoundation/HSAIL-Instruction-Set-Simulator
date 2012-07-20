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
					FLOAT_ROUNDING		
                  };

// classify token into different terminal type
TerminalType GetTokenType(int token);

int Parse(std::string input);
int Query(int queryOp);
int Operand(int first_token);
int Identifier(int first_token);
int BaseOperand(int first_token);
int AddressableOperand(int first_token);
int ArrayOperandList(int first_token);
int RoundingMode(int first_token);
int Instruction2(int first_token);
#endif  // PARSER_H_
