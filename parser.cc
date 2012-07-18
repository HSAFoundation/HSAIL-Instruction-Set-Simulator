#include "parser.h"
#include "tokens.h"
#include "build/lexer.h"
#include <string>

extern int int_val;
extern float float_val;
extern double double_val;


int token_local_identifier()
{
    printf("LOCAL_IDENTIFIER \n");
    return 1;
}

int token_global_identifier()
{
    printf("GLOBAL_IDENTIFIER \n");
    return 1;
}

//rule for dataTypeID
int dataTypeId( int token ){
    if (token >= _U32 && token <= SYSCALL)
    {
        return 1;
    }

    printf("Unidentified dataTypeID in queryOp instruction.\n");
    return 0;
}

/* ---rule for addressableOperand---*/
int nonRegister( int token ){

    int result = 0;
    if (token == TOKEN_LOCAL_IDENTIFIER)
        {
            token_local_identifier();
            result = 1;
        }
            else if ( token == TOKEN_GLOBAL_IDENTIFIER )
            {
                token_global_identifier();
                result = 1;
            }
    
    return result;

    
}

int Register( int token ){
    
    int result = 0;
    if (token >= TOKEN_CREGISTER && token <= TOKEN_QREGISTER)
    {
       result = 1;
    }
    
    return result;

}

int addressableOperand(){

    char punctuation = yylex();
    if (punctuation != '[')
    {
        printf(" Unidentified Punctuation in addressableOperand. \n");
        return 0;
    }

    int token = yylex();
    int result = nonRegister(token);

    if ( result == 0 )
        return 0;

    punctuation = yylex();
    if (punctuation == '<' )
    {
        token = yylex();
        if(token == TOKEN_INTEGER_CONSTANT)
        {
            punctuation = yylex();
            if ( punctuation == '>')
            {    
                punctuation = yylex();
                if (punctuation == ']')
                    return 1;
            }
            return 0;
        }

        result = Register(token);
        if ( result == 0 )
            return 0;

        punctuation = yylex();
        if ( punctuation == '>')
        {    
            punctuation = yylex();
            if (punctuation == ']')
                return 1;
            else if ( punctuation == '+' || punctuation == '-' )
            {
                token = yylex();
                if ( token != TOKEN_INTEGER_CONSTANT)
                    return 0;

                punctuation = yylex();
                if ( punctuation == '>')
                {    
                    punctuation = yylex();
                    if (punctuation == ']')
                        return 1;
                    else return 0;
                }
                else return 0;
            }
        }
    }
    else if ( punctuation == ']')
        return 1;

    return 0;





}


/* ---rule for operand---*/
int floatListSingle(){
}

int decimalListSingle(){
}

int baseOperand( int token ){


}

int identifier( int token ){

    int result = 0;
    if (token == TOKEN_LOCAL_IDENTIFIER)
        {
            token_local_identifier();
            result = 1;
        }
            else if ( token == TOKEN_GLOBAL_IDENTIFIER )
            {
                token_global_identifier();
                result = 1;
            }
            else
            {
                result = Register(token);
               // if (result == 0)
               //     result = nonRegister(token);
            }
 
    return result;

}

int operand(){
    int token = yylex();
    std::string operandStr(yytext);

    int result = 0;
//    if ( (token >= TOKEN_CREGISTER && token <= TOKEN_QREGISTER) || token == TOKEN_LOCAL_IDENTIFIER || token == TOKEN_GLOBAL_IDENTIFIER)
    result = identifier( token );
    if ( result == 0)
        result = baseOperand(token);

    if (result == 0)
        printf("Unidentified operand in queryOp instruction.\n");


    return result;
        



}

int query(token_type queryOp)
{
    int result = 1;
    printf("Query Type: %s \n", yytext);
    
    int token = yylex();
    result = dataTypeId(token);
    if(!result)
        return 0;

    
    result = operand();
    if(!result)
        return 0;
    
    int punctuation = yylex();
    if (punctuation != ',')
        result = 0;
    if(!result)
        return 0;

    result = addressableOperand();
    if(!result)
        return 0;
    
    
    
//    token operand


    return 1;
    
  
}

int parser_main(std::string input)
{
  yy_scan_string((char*)input.c_str());
  int token = yylex();
  int result;
  switch (token)
  {
     /* queryOp */
     case QUERY_ORDER:
     case QUERY_DATA:
     case QUERY_ARRAY:
     case QUERY_WIDTH:
     case QUERY_DEPTH:
     case QUERY_NORMALIZED:
     case QUERY_FILTERING:
      	result=query((token_type)token);
	return result;
 
     default:
	return 0;    // no matching
  }

}






