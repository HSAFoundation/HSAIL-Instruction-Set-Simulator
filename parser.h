#ifndef _PARSER_H_
#define _PARSER_H_
#include "tokens.h"
#include <string>

enum terminal_type { UNKNOWN_TERM = 0,              // terminals in hsail grammars
		     REGISTER,
		     DATA_TYPE_ID,
		     QUERY_OP
 		   };


int parse(std::string input); 

terminal_type get_token_type(int token);  // classify token into different terminal types

int query(int queryOp) ;
int operand(int first_token);
int identifier(int first_token);
int baseOperand(int first_token);
int addressableOperand(int first_token);
#endif
