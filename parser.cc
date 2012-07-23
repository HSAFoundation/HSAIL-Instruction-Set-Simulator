/* Copyright 2012 <MulticorewareInc> */

#include "./parser.h"

#include <string>

#include "./tokens.h"
#include "./build/lexer.h"

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

    /* IntRounding */
  case _UPI:
  case _DOWNI:
  case _ZEROI:
  case _NEARI:
    return INT_ROUNDING;

    /* FloatRounding */
  case _UP:
  case _DOWN:
  case _ZERO:
  case _NEAR:
    return FLOAT_ROUNDING;
    /* Packing */
  case _PP:
  case _PS:
  case _SP:
  case _SS:
  case __S:
  case __P:
  case _PP_SAT:
  case _PS_SAT:
  case _SP_SAT:
  case _SS_SAT:
  case _S_SAT:
  case _P_SAT:
    return PACKING;

  case _SMALL:
  case _LARGE:
  case _FULL:
  case _REDUCED:
  case _SFTZ:
  case _NOSFTZ:
    return TARGET;

    /* Instruction2Opcode */
  case ABS:
  case NEG:
  case NOT:
  case POPCOUNT:
  case FIRSTBIT:
  case LASTBIT:
  case BITREV:
  case MOVS_LO:
  case MOVS_HI:
  case FBAR_INITSIZE:
  case FBAR_INIT:
  case FBAR_RELEASECF:
  case COUNT:
  case MASK:
    return INSTRUCTION2_OPCODE;

    /* Instruction2Opcode NoDT */
  case UNPACK3:
  case UNPACK2:
  case UNPACK1:
  case UNPACK0:
  case ALLOCA:
  case WORKITEMID:
  case WORKITEMAID:
  case WORKGROUPSIZE:
  case NDRANGESIZE:
  case NDRANGEGROUPS:
    return INSTRUCTION2_OPCODE_NODT;

    /* Instruction2OpcodeFTZ */
  case SQRT:
  case FRACT:
  case FCOS:
  case FSIN:
  case FLOG2:
  case FEXP2:
  case FSQRT:
  case FRSQRT:
  case FRCP:
    return INSTRUCTION2_OPCODE_FTZ;

    /* Instruction3Opcode */
  case ADD:
  case CARRY:
  case BORROW:
  case DIV:
  case REM:
  case SUB:
  case SHL:
  case SHR:
  case AND:
  case XOR:
  case OR:
  case UNPACKLO:
  case UNPACKHI:
  case MOVD_LO:
  case MOVD_HI:
  case COPYSIGN:
  case CLASS:
  case SEND:
  case RECEIVE:
    return INSTRUCTION3_OPCODE;
  case MAX:
  case MIN:
    return INSTRUCTION3_OPCODE_FTZ;
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
          if (yylex() == ';')
            return 0;
          else
            printf("Missing ';' in query\n");
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
  if (first_token == TOKEN_GLOBAL_IDENTIFIER) {
    return 0;
  } else if (first_token == TOKEN_LOCAL_IDENTIFIER) {
    return 0;
  } else if (GetTokenType(first_token) == REGISTER) {
    return 0;
  }

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
          }  // while
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
          }  // while
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
    if ((next == TOKEN_GLOBAL_IDENTIFIER) ||
        (next == TOKEN_LOCAL_IDENTIFIER)) {
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

int CallTargets(int first_token) {
  // assumed first_token is '['
  int next;
  while (1) {
    next = yylex();
    if (!Identifier(next)) {
      next = yylex();
      if (next == ']')
        return 0;
      else if (next == ',') {
      } else {
        return 1;
      }
    } else {
      return 1;
    }
  }
};

int RoundingMode(int first_token, bool* is_ftz, int* last_token) {
  *is_ftz = false;
  *last_token = first_token;
  int next;

  if (first_token == _FTZ) {
    next = yylex();
    *last_token = next;

    if (GetTokenType(next) == FLOAT_ROUNDING) {
      // next is floatRounding
    } else {
      *is_ftz = true;
    }
    return 0;
  } else if (GetTokenType(first_token) == INT_ROUNDING) {
    return 0;
  } else if (GetTokenType(first_token) == FLOAT_ROUNDING) {
    return 0;
  } else {
    return 1;
  }
}

int Instruction2(int first_token) {
  // First token must be an Instruction2Opcode
  int next = yylex();
  // to get last token returned by RoundingMode in case _ftz
  int temp = 0;
  bool is_ftz = false;

  if (GetTokenType(first_token) == INSTRUCTION2_OPCODE) {
    if (!RoundingMode(next, &is_ftz, &temp)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }

    // check whether there is a Packing
    if (GetTokenType(next) == PACKING)
      // there is packing
      next = yylex();

    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      if (!Operand(yylex())) {
        if (yylex() == ',') {
          if (!Operand(yylex())) {
            if (yylex() == ';') {
              return 0;
            }
          }
        }
      }
    }
    return 1;
  } else if (GetTokenType(first_token) == INSTRUCTION2_OPCODE_NODT) {
    if (!RoundingMode(next, &is_ftz, &temp)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }


    // check the operands
    if (!Operand(next)) {
      if (yylex() == ',') {
        if (!Operand(yylex())) {
          if (yylex() == ';') {
            return 0;
          }
        }
      }
    }
    return 1;
  } else if (GetTokenType(first_token) == INSTRUCTION2_OPCODE_FTZ) {
    // Optional FTZ
    if (next == _FTZ) {
      // has a _ftz
      next = yylex();
    }


    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      if (!Operand(yylex())) {
        if (yylex() == ',') {
          if (!Operand(yylex())) {
            if (yylex() == ';') {
              return 0;
            }
          }
        }
      }
    }
    return 1;
  } else {
    return 1;
  }
}

int Instruction3(int first_token) {
  // First token must be an Instruction3Opcode
  int next = yylex();
  // to get last token returned by RoundingMode in case _ftz
  int temp = 0;
  bool is_ftz = false;

  if (GetTokenType(first_token) == INSTRUCTION3_OPCODE) {
    if (!RoundingMode(next, &is_ftz, &temp)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }

    // check whether there is a Packing
    if (GetTokenType(next) == PACKING)
      // there is packing
      next = yylex();

    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      if (!Operand(yylex())) {
        if (yylex() == ',') {
          if (!Operand(yylex())) {
            if (yylex() == ',') {
              if (!Operand(yylex())) {
                if (yylex() == ';')
                  return 0;
              }  // 3rd operand
            }  // 2nd comma
          }  // 2nd operand
        }  // 1st comma
      }  // 1st operand
    }  // DATA_TYPE_ID
    return 1;
  } else if (GetTokenType(first_token) == INSTRUCTION3_OPCODE_FTZ) {
    // Optional FTZ
    if (next == _FTZ) {
      // has a _ftz
      next = yylex();
    }

    // check whether there is a Packing
    if (GetTokenType(next) == PACKING)
      // there is packing
      next = yylex();

    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      if (!Operand(yylex())) {
        if (yylex() == ',') {
          if (!Operand(yylex())) {
            if (yylex() == ',') {
              if (!Operand(yylex())) {
                if (yylex() == ';')
                  return 0;
              }  // 3rd operand
            }  // 2nd comma
          }  // 2nd operand
        }  // 1st comma
      }  // 1st operand
    }  // DATA_TYPE_ID
    return 1;
  } else {
    return 1;
  }
  return 1;
}

int Version(int first_token) {
  // first token must be version keyword
  // check for major
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    if (yylex() == ':') {
      // check for minor
      if (yylex() == TOKEN_INTEGER_CONSTANT) {
        int next = yylex();
        if (next == ';') {
          return 0;
        } else if (next == ':') {
          // check for target
          next = yylex();
          while (next != ';') {
            if (GetTokenType(next) == TARGET) {
              next = yylex();
              if (next == ',')
                next = yylex();      // next target
              else if (next != ';')
                return 1;
            } else {
              return 1;
            }
          }
          return 0;
        }
      }
    }
  }
  return 1;
};

int Alignment(int first_token) {
  // first token must be "align" keyword
  if (yylex() == TOKEN_INTEGER_CONSTANT)
    return 0;
  else
    return 1;
}

// parse declaration prefix
// since this function checks for one token lookahead
// if the last token is not consumed by this,
// it will notify the caller to recheck
int DeclPrefix(int first_token, bool* recheck_last_token, int* last_token) {
  int last_align_token;
  int next_token;
  *recheck_last_token = false;
  *last_token = 0;

  if (first_token == ALIGN) {
    if (!Alignment(first_token)) {
      next_token = yylex();  // need to go to next token
      *last_token = next_token;
      // first is alignment
      if (next_token == CONST) {
        // alignment const
        next_token = yylex();
        *last_token = next_token;

        if ((next_token == EXTERN)||(next_token == STATIC)) {
          // alignment const externOrStatic
          *recheck_last_token = false;
        } else {
          // alignment const
          *recheck_last_token = true;
        }
      } else if ((next_token == EXTERN)||(next_token == STATIC)) {
        // alignment externOrStatic
        next_token = yylex();
        *last_token = next_token;

        if (next_token == CONST) {
          // alignmnet externOrStatic const
        } else {
          // alignment externOrStatic
          *recheck_last_token = true;
        }
      } else {
        // alignment
        *recheck_last_token = true;
      }
    }
  } else if (first_token == CONST) {
    // first is const
    next_token = yylex();
    *last_token = next_token;
    if (next_token == ALIGN) {
      if (!Alignment(next_token)) {
        // const alignment
        next_token = yylex();
        *last_token = next_token;

        if ((next_token == EXTERN)||(next_token == STATIC)) {
          // const alignment externOrStatic
        } else {
          // const alignment
          *recheck_last_token = true;
        }
      }
    } else if ((next_token == EXTERN)||(next_token == STATIC)) {
      // const externOrStatic
      next_token = yylex();
      *last_token = next_token;

      if (next_token == ALIGN) {
        if (!Alignment(next_token)) {
          // const externOrStatic alignment
        } else {
          return 1;
        }
      } else {
        // const externOrStatic
        *recheck_last_token = true;
      }
    } else {  // const does not stand alone
      *recheck_last_token = true;
      return 1;
    }
  } else if ((first_token == EXTERN)||(first_token == STATIC)) {
    // externOrStatic first
    next_token = yylex();
    *last_token = next_token;
    if (next_token == ALIGN) {
      if (!Alignment(next_token)) {
        // externOrStatic alignment
        next_token = yylex();
        *last_token = next_token;

        if (next_token == CONST) {
          // externOrStatic alignment const
        } else {
          // externOrStatic alignment
          *recheck_last_token = true;
        }
      }
    } else if (next_token == CONST) {
      // externOrStatic const
      next_token = yylex();
      *last_token = next_token;

      if (next_token == ALIGN) {
        if (!Alignment(next_token)) {
          *last_token = next_token;
        } else {
          return 1;
        }
        // externOrStatic const alignment
      } else {
        *recheck_last_token = true;
      }
    } else {
      *recheck_last_token = true;
    }
  } else {
    *recheck_last_token = true;
    *last_token = first_token;
  }
  return 0;
}

int FBar(int first_token) {
  // first token must be _FBAR
  if (yylex() == '(' )
    if (yylex() == TOKEN_INTEGER_CONSTANT)
      if (yylex() == ')')
        return 0;

  return 1;
}


int ArrayDimensionSet(int first_token,
                      bool* rescan_last_token,
                      int* last_token) {
  // first token must be '['
  *rescan_last_token = false;
  int next_token = yylex();
  while (1) {
    if (next_token == ']') {
      next_token = yylex();  // check if there is more item
      if (next_token == '[') {  // more item
        next_token = yylex();
      } else {  // no more item
        *last_token = next_token;
        *rescan_last_token  = true;
        return 0;
      }
    } else if (next_token == TOKEN_INTEGER_CONSTANT) {
      next_token = yylex();  // scan next
    } else {
      printf("Missing closing bracket.\n");
      return 1;
    }
  }
}

int ArgumentDecl(int first_token, bool* rescan_last_token, int* last_token) {
  bool rescan_after_declPrefix;
  int last_token_of_declPrefix;
  int next;
  if (!DeclPrefix(first_token,
                  &rescan_after_declPrefix,
                  &last_token_of_declPrefix)) {
    if (!rescan_after_declPrefix) {
      next = yylex();
    } else {
      next = last_token_of_declPrefix;
    }

    next = yylex();  // skip over "arg"

    if ((GetTokenType(next) == DATA_TYPE_ID)||
        (next == _RWIMG) ||
        (next == _SAMP) ||
        (next == _ROIMG)) {
      next = yylex();
      if (next == TOKEN_LOCAL_IDENTIFIER) {
        // scan for arrayDimensions
        next = yylex();
        if (next == '[') {
          if (!ArrayDimensionSet(next, rescan_last_token, last_token)) {
            return 0;
          }
        } else {
          // no arrayDimensions
          *last_token = next;
          *rescan_last_token = true;
          return 0;
        }
      }
    }
  }
  return 1;
}

int ArgumentListBody(int first_token,
                     bool* rescan_last_token,
                     int* last_token) {
  *last_token = 0;
  *rescan_last_token = false;

  int prev_token = 0;
  bool rescan = false;
  while (1) {
    if (!ArgumentDecl(first_token, &rescan, &prev_token)) {
      if (!rescan)
        prev_token = yylex();
      if (prev_token == ',') {
        first_token = yylex();
      } else {
        *last_token = prev_token;
        *rescan_last_token = true;
        return 0;
      }
    } else {
      return 1;
    }
  }
}

int FunctionDefinition(int first_token,
                       bool* rescan_last_token,
                       int* last_token) {
  *last_token = 0;
  * rescan_last_token = false;

  int token_to_scan;
  bool rescan;

  if (!DeclPrefix(first_token, &rescan, &token_to_scan)) {
    if (!rescan)
      token_to_scan = yylex();

    if (token_to_scan == FUNCTION) {
      if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
        // check return argument list
        if (yylex() == '(') {
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan)) {
            if (!rescan)
              token_to_scan = yylex();

            if (token_to_scan == ')')
              token_to_scan = yylex();
            else
              return 1;
          } else {
            return 1;
          }
        } else {
          return 1;
        }
        // check argument list
        if (token_to_scan == '(') {
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan)) {
            if (!rescan)
              token_to_scan = yylex();
            if (token_to_scan == ')')
              token_to_scan = yylex();
            else
              return 1;
          } else {
            return 1;
          }
        } else {
          return 1;
        }
        // check for optional FBar
        if (token_to_scan == _FBAR) {
          if (!FBar(token_to_scan)) {
            return 0;
          }
        } else {
          *rescan_last_token = true;
          *last_token = token_to_scan;
          return 0;
        }
      }
    }
  }
  return 1;
}

int FunctionDecl(int first_token) {
  int token_to_scan;
  bool rescan;

  if (!DeclPrefix(first_token, &rescan, &token_to_scan)) {
    if (!rescan)
      token_to_scan = yylex();

    if (token_to_scan == FUNCTION) {
      if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
        // check return argument list
        if (yylex() == '(') {
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan)) {
            if (!rescan)
              token_to_scan = yylex();

            if (token_to_scan == ')')
              token_to_scan = yylex();
            else
              return 1;
          } else {
            return 1;
          }
        } else {
          return 1;
        }
        // check argument list
        if (token_to_scan == '(') {
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan)) {
            if (!rescan)
              token_to_scan = yylex();
            if (token_to_scan == ')')
              token_to_scan = yylex();
            else
              return 1;
          } else {
            return 1;
          }
        } else {
          return 1;
        }


        // check for optional FBar
        if (token_to_scan == _FBAR) {
          if (!FBar(token_to_scan)) {
            token_to_scan = yylex();
          }
        }
        if (token_to_scan == ';')
          return 0;
      }
    }
  }


  return 1;
}

int Codeblock(int first_token) {
  // first token should be '{'
  // currently only supports Instruction2 as body statement
  int next_token = 0;
  while (1) {
    next_token = yylex();
    if ((GetTokenType(next_token) == INSTRUCTION2_OPCODE) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_NODT) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(next_token)) {
      } else {
        return 1;
      }
    } else if ((GetTokenType(next_token) == INSTRUCTION3_OPCODE) ||
               (GetTokenType(next_token) == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(next_token)) {
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == QUERY_OP) {  // Query Operation
      if (!Query(next_token)) {
      } else {
        return 1;
      }
    } else if (next_token == RET) {  // ret operation
      if (yylex() == ';') {
      } else {
        printf("Missing ';' at the end of ret operation\n");
        return 1;
      }
    } else if ((next_token == BRN) ||
               (next_token == CBR)) {
      if (!Branch(next_token)) {
      } else {
        return 1;
      }
    } else if (next_token == TOKEN_LABEL) {  // label
      if (yylex() == ':') {
      } else {
        return 1;
      }
    } else if (next_token == '}') {
      next_token = yylex();
      if (next_token == ';')
        return 0;
    } else {
      break;
    }
  }
  return 1;
}

int Function(int first_token) {
  bool rescan = false;
  int last_token = 0;
  if (!FunctionDefinition(first_token, &rescan, &last_token)) {
    if (!rescan)
      last_token = yylex();
    if (!Codeblock(last_token))
      return 0;
  }
  return 1;
}

int Program(int first_token) {
  int result;
  int last_token;
  bool rescan;

  if (first_token == VERSION) {
    if (!Version(first_token)) {
      // parse topLevelStatement
      first_token = yylex();
      while (first_token) {
        if ( (first_token == ALIGN) ||
             (first_token == CONST) ||
             (first_token == EXTERN) ||
             (first_token == STATIC) ) {
          result = DeclPrefix(first_token, &rescan, &last_token);
          if (result)
            return 1;

          if (!rescan)
            first_token = yylex();
          else
            first_token = last_token;
        }

        // Found "function" keyword ------------------------
        if (first_token == FUNCTION) {
          // look at next token
          if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
            // check return argument list
            if (yylex() == '(') {
              first_token = yylex();

              if (first_token == ')') {   // empty argument list body
                first_token = yylex();
              } else if (!ArgumentListBody(first_token,
                                           &rescan,
                                           &first_token)) {
                if (!rescan)
                  first_token = yylex();

                if (first_token == ')')
                  first_token = yylex();
                else
                  return 1;
              } else {
                return 1;
              }
            } else {  // missing '('
              return 1;
            }          // if found '(' - returnArgList

            // check argument list
            if (first_token == '(') {
              first_token = yylex();

              if (first_token == ')') {   // empty argument list body
                first_token = yylex();
              } else if (!ArgumentListBody(first_token,
                                           &rescan,
                                           &first_token)) {
                if (!rescan)
                  first_token = yylex();
                if (first_token == ')')
                  first_token = yylex();
                else
                  return 1;
              } else {  // missing body
                return 1;
              }
            } else {  // missing '('
              return 1;
            }            // if found '(' - argList

            // check for optional FBar
            if (first_token == _FBAR) {
              if (!FBar(first_token)) {
                first_token = yylex();
              }
            }

            if (first_token == ';') {  // this is a functionDecl
              first_token = yylex();
              continue;
            } else if (first_token == '{') {
              // so this must be a functionDefinition
              if (!Codeblock(first_token)) {  // check codeblock of function
                first_token = yylex();
                continue;
              } else {
                printf("Error in function's codeblock\n");
                return 1;
              }
            }
          }       // if found TOKEN_GLOBAL_ID
        } else {  // if first_token == FUNCTION
          return 1;  // currently only support functions
        }
      }    // while (first_token)
      return 0;
    } else {
      return 1;
    }  // if (!Version)
  } else {
    printf("Program must start with version statement.\n");
    return 1;
  }
}

int OptionalWidth(int first_token) {
  // first token must be _WIDTH
  int next_token = yylex();
  if (next_token == '(') {
    next_token = yylex();
    if (next_token == ALL) {
      next_token = yylex();
    } else if (next_token == TOKEN_INTEGER_CONSTANT) {
      next_token = yylex();
    } else {
      return 1;
    }
    if (next_token == ')')
      return 0;
  }
  return 1;
}

int Branch(int first_token) {
  int op = first_token;  // CBR or BRN
  int current_token = yylex();


  // check for optionalWidth
  if (current_token == _WIDTH) {
    if (!OptionalWidth(current_token)) {
    } else {
      printf("Invalid optional width.\n");
      return 1;
    }
    current_token = yylex();
  }

  // check for optional _fbar modifier
  if (current_token == __FBAR)
    current_token = yylex();

  // parse operands
  if (op == CBR) {
    if (!Operand(current_token)) {
      if (yylex() == ',') {
        current_token = yylex();
        if (current_token == TOKEN_LABEL) {
          current_token = yylex();  // should be ';'
        } else if (!Identifier(current_token)) {
          current_token = yylex();  // should be ';'

        } else if (!Operand(current_token)) {
          if (yylex() == ',') {
            current_token = yylex();
            if (current_token == TOKEN_LABEL) {  // LABEL
              current_token = yylex();  // should be ';'
            } else if (current_token == '[') {
              current_token = yylex();
              if (!Identifier(current_token)) {
                current_token = yylex();  // should be ']'
              } else if (current_token == TOKEN_LABEL) {
                current_token = yylex();

                while (current_token != ']') {
                  if (current_token == ',') {
                    if (yylex() == TOKEN_LABEL)
                      current_token = yylex();  // scan next;
                    else
                      return 1;
                  } else {
                    printf("Missing ','\n");
                    return 1;
                  }
                }   // while
              }
              // current token should be ']'
              current_token = yylex();  // should be ';'
            }
          }  // yylex() = ','
        } else {
          return 1;
        }
        if (current_token == ';')
          return 0;
      }  // yylex = ','
    }  // first operand
  } else if (op == BRN) {
    if (current_token == TOKEN_LABEL) {
      if (yylex() == ';')
        return 0;
    } else if (!Identifier(current_token)) {
      current_token = yylex();

      if (current_token == ';')
        return 0;
      else if (current_token == ',') {
        if (yylex() == '[') {
          current_token = yylex();
          if (current_token == TOKEN_LABEL) {
            current_token = yylex();    // should be ']'
          } else if (!Identifier(current_token)) {
            current_token = yylex();    // should be ']'
          }
        }


        if (yylex() == ';')
          return 0;
      }
    }
  }
  return 1;
}
