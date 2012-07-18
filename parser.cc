#include "parser.h"
#include "tokens.h"
#include "build/lexer.h"
#include <string>

int parse(std::string input) {
  yy_scan_string((char*) input.c_str());
  int token = yylex();   // get first token
  int result;

  terminal_type term = get_token_type(token);
  switch (term) {
    case QUERY_OP:
      // have a query
      // check syntax
      result = query(token);    
      return result;
    default:
      return 1;
  }
}

terminal_type get_token_type(int token)
{
  switch (token)
  {
    /* DataTypeId */
    case _U32:
    case _S32:
    case _S64:
    case _U64:
    case _B1:
    case _B32:
    case _F64:
    case _F32:
    case _B64:
    case _B8:
    case _B16:
    case _S8:
    case _S16:
    case _U8:
    case _U16:
    case _F16:
    case _B128:
    case _U8X4:
    case _S8X4:
    case _U16X2:
    case _S16X2:
    case _F16X2:
    case _F32X2:
    case _U8X8:
    case _S8X8:
    case _U16X4:
    case _S16X4:
    case _F16X4:
    case _U8X16:
    case _S8X16:
    case _U16X8:
    case _S16X8:
    case _F16X8:
    case _F32X4:
    case _S32X4:
    case _U32X4:
    case _F64X2:
    case _S64X2:
    case _U64X2:
      printf("dataTypeId\n");
      return DATA_TYPE_ID;

    /* queryOp */
    case QUERY_ORDER:
    case QUERY_DATA:
    case QUERY_ARRAY:
    case QUERY_WIDTH:
    case QUERY_DEPTH:
    case QUERY_NORMALIZED:
    case QUERY_FILTERING:
      printf("queryOp\n");
      return QUERY_OP;
   
    /* Register */
    case TOKEN_CREGISTER:
    case TOKEN_DREGISTER:
    case TOKEN_SREGISTER:
    case TOKEN_QREGISTER:
	  printf("Register\n");
	  return REGISTER;


 
     default:
	return UNKNOWN_TERM;  // unknown
  }
}

int query(int queryOp)
{
  // next token should be a dataTypeId  
  if (get_token_type(yylex()) != DATA_TYPE_ID)
    return 1;
  
  // next token should be an operand
  if (operand(yylex()))
    return 1; 		  // invalid operand
  

  return 0;
  
}


int operand(int first_token) {
  if (!identifier(first_token))   // an identifier
    return 0;      
  else if (!baseOperand(first_token)) // a base operand
    return 0;


  return 1;

}

int identifier(int first_token) {
  if (first_token == TOKEN_GLOBAL_IDENTIFIER)
    return 0;
  else if (first_token == TOKEN_LOCAL_IDENTIFIER)
    return 0;
  else if (get_token_type(first_token)==REGISTER)
    return 0;



  return 1;
}

int baseOperand(int first_token) {
  if (first_token == TOKEN_DOUBLE_CONSTANT) 
  {
    printf("double constant\n");
    return 0;
  }
  else if (first_token == TOKEN_SINGLE_CONSTANT)
  {
	printf("single constant\n");
    return 0;
  }
  else if (first_token == TOKEN_INTEGER_CONSTANT)
  {
	printf("integer constant\n");
    return 0;
  }
  else if (first_token == TOKEN_WAVESIZE)
  {
 	printf("TOKEN_WAVESIZE\n");
    return 0;
  } 
  else if (first_token == '-')
  {
     if (yylex() == TOKEN_INTEGER_CONSTANT)
		printf("integer constant\n");
       return 0;
  }

  
}
