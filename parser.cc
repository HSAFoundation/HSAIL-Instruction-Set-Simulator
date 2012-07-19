/* Copyright 2012 <MulticorewareInc> */

#include "./parser.h"

#include <string>

#include "./tokens.h"
#include "./build/lexer.h"


int Parse(std::string input) {
    yy_scan_string(reinterpret_cast<const char*> (input.c_str()));
    int token = yylex();   // get first token
    int result;

    TerminalType term = GetTokenType(token);
    switch (term) {
    case QUERY_OP:
        // have a Query
        // check syntax
        result = Query(token);
        return result;
    default:
        return 1;
    }
}

TerminalType GetTokenType(int token) {
    switch (token) {
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
        return DATA_TYPE_ID;

        /* QueryOp */
    case QUERY_ORDER:
    case QUERY_DATA:
    case QUERY_ARRAY:
    case QUERY_WIDTH:
    case QUERY_DEPTH:
    case QUERY_HEIGHT:
    case QUERY_NORMALIZED:
    case QUERY_FILTERING:
        return QUERY_OP;

        /* Register */
    case TOKEN_CREGISTER:
    case TOKEN_DREGISTER:
    case TOKEN_SREGISTER:
    case TOKEN_QREGISTER:
        return REGISTER;

        default:
        return UNKNOWN_TERM;  // unknown
    }
}

int Query(int QueryOp) {
    // next token should be a dataTypeId
    if (GetTokenType(yylex()) == DATA_TYPE_ID) {
        // next token should be an Operand
        if (!Operand(yylex())) {
            // next should be a comma
            if (yylex() == ',') {
                // then finally an addressable Operand
                if (!AddressableOperand(yylex())) {
                    return 0;
                }
            }
        }
    }
    return 1;
}


int Operand(int first_token) {
    if (!Identifier(first_token))   // an Identifier
        return 0;
    else if (!BaseOperand(first_token))     // a base Operand
        return 0;

    return 1;
}

int Identifier(int first_token) {
    if (first_token == TOKEN_GLOBAL_IDENTIFIER)
        return 0;
    else if (first_token == TOKEN_LOCAL_IDENTIFIER)
        return 0;
    else if (GetTokenType(first_token) == REGISTER)
        return 0;

    return 1;
}

int BaseOperand(int first_token) {
    int next;
    if (first_token == TOKEN_DOUBLE_CONSTANT) {
        return 0;
    } else if (first_token == TOKEN_SINGLE_CONSTANT) {
        return 0;
    } else if (first_token == TOKEN_INTEGER_CONSTANT) {
        return 0;
    } else if (first_token == TOKEN_WAVESIZE) {
        return 0;
    } else if (first_token == '-') {
        if (yylex() == TOKEN_INTEGER_CONSTANT)
            return 0;
    } else if (GetTokenType(first_token) == DATA_TYPE_ID) {
        // scan next token
        if (yylex() == '(') {   // should be '('
            // check if we have a decimal list single or float list single
            next = yylex();
            if (next == TOKEN_INTEGER_CONSTANT) {
                next = yylex();
                if (next == ')') {
                    return 0;
                } else {
                    while (next == ',') {
                        next = yylex();
                        if (next == TOKEN_INTEGER_CONSTANT) {
                            next = yylex();
                            if (next == ')')
                                return 0;
                            else if (next != ',')
                                return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            } else if (next == TOKEN_DOUBLE_CONSTANT)   {
                next = yylex();
                if (next == ')') {
                    return 0;
                } else {
                    while (next == ',') {
                        next = yylex();
                        if (next == TOKEN_DOUBLE_CONSTANT) {
                            next = yylex();
                            if (next == ')')
                                return 0;
                            else if (next != ',')
                                return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        return 1;
    }
}

int AddressableOperand(int first_token) {
    int next;
    if (first_token == '[') {
        // next should be a non register
        next = yylex();
        if ( (next == TOKEN_GLOBAL_IDENTIFIER) \
            || (next == TOKEN_LOCAL_IDENTIFIER) ) {
            next = yylex();

            if (next == ']')
                return 0;
            else if (next == '<') {
                next = yylex();
                if (next == TOKEN_INTEGER_CONSTANT) {
                    if (yylex() == '>') {
                        if (yylex() == ']')
                            return 0;
                    }

                } else if (GetTokenType(next) == REGISTER) {
                    next = yylex();
                    if (next == '>') {
                        if (yylex() == ']')
                            return 0;

                    } else if ((next == '+') || (next == '-')) {
                        if (yylex() == TOKEN_INTEGER_CONSTANT) {
                            if (yylex() == '>') {
                                if (yylex() == ']')
                                    return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int ArrayOperandList(int first_token) {
    // assumed first_token is '('
    int next;
    while (1) {
        next = yylex();
        if (!Identifier(next)) {
            next = yylex();
            if (next == ')')
                return 0;
            else if (next == ',') {
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    }
}


