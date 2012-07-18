#ifndef HSAIL2BRIG_PARSER_H_
#define HSAIL2BRIG_PARSER_H_

#include <string>
#include "tokens.h"

enum TerminalType { UNKNOWN_TERM = 0,
                    REGISTER,
                    DATA_TYPE_ID,
                    QUERY_OP
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

#endif  // HSAIL2BRIG_PARSER_H_
