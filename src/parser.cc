/* Copyright 2012 <MulticorewareInc> */

#include "parser.h"
#include <string>
#include "tokens.h"
#include "lexer.h"
#include "error_reporter_interface.h"

extern int int_val;
extern char* string_val;
extern float float_val;
extern double double_val;

namespace hsa {
namespace brig {

TerminalType GetTokenType(unsigned int token) {
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
    /* initializable address */
  case READONLY:
  case GLOBAL:
    return INITIALIZABLE_ADDRESS;
  case PRIVATE:
  case GROUP:
  case SPILL:
    return UNINITIALIZABLE_ADDRESS;
  default:
    return UNKNOWN_TERM;  // unknown
  }
}

BrigDataType GetDataType(int token) {
  switch (token) {
    /* DataTypeId */
  case _U32:
    return Brigu32;
    break;
  case _S32:
    return Brigs32;
    break;
  case _S64:
    return Brigs64;
    break;
  case _U64:
    return Brigu64;
    break;
  case _B1:
    return Brigb1;
    break;
  case _B32:
    return Brigb32;
    break;
  case _F64:
    return Brigf64;
    break;
  case _F32:
    return Brigf32;
    break;
  case _B64:
    return Brigb64;
    break;
  case _B8:
    return Brigb8;
    break;
  case _B16:
    return Brigb16;
    break;
  case _S8:
    return Brigs8;
    break;
  case _S16:
    return Brigs16;
    break;
  case _U8:
    return Brigu8;
    break;
  case _U16:
    return Brigu16;
    break;
  case _F16:
    return Brigf16;
    break;
  case _B128:
    return Brigb128;
    break;
  case _U8X4:
    return Brigu8x4;
    break;
  case _S8X4:
    return Brigs8x4;
    break;
  case _U16X2:
    return Brigu16x2;
    break;
  case _S16X2:
    return Brigs16x2;
    break;
  case _F16X2:
    return Brigf16x2;
    break;
  case _F32X2:
    return Brigf32x2;
    break;
  case _U8X8:
    return Brigu8x8;
    break;
  case _S8X8:
    return Brigs8x8;
    break;
  case _U16X4:
    return Brigu16x4;
    break;
  case _S16X4:
    return Brigs16x4;
    break;
  case _F16X4:
    return Brigf16x4;
    break;
  case _U8X16:
    return Brigu8x16;
    break;
  case _S8X16:
    return Brigs8x16;
    break;
  case _U16X8:
    return Brigu16x8;
    break;
  case _S16X8:
    return Brigs16x8;
    break;
  case _F16X8:
    return Brigf16x8;
    break;
  case _F32X4:
    return Brigf32x4;
    break;
  case _S32X4:
    return Brigs32x4;
    break;
  case _U32X4:
    return Brigu32x4;
    break;
  case _F64X2:
    return Brigf64x2;
    break;
  case _S64X2:
    return Brigs64x2;
    break;
  case _U64X2:
    return Brigu64x2;
    break;
  case _ROIMG:
    return BrigROImg;
    break;
  case _RWIMG:
    return BrigRWImg;
    break;
  case _SAMP:
    return BrigSamp;
    break;
}
}

BrigOpcode GetOpCode(int token) {
  switch (token) {
    /* opcode */
  case ABS:
    return BrigAbs;
    break;
  case ADD:
    return BrigAdd;
    break;
  case CARRY:
    return BrigCarry;
    break;
  case BORROW:
    return BrigBorrow;
    break;
// TBD, need to add complete list of all operations.  
  }
}

BrigPacking GetPacking(int token) {
  switch (token) {
   /* packing type */
  case _PP:
    return BrigPackPP;
    break;
  case _PS:
    return BrigPackPS;
    break;
  case _SP:
    return BrigPackSP;
    break;
  case _SS:
    return BrigPackSS;
    break;
  case __S:
    return BrigPackS;
    break;
  case __P:
    return BrigPackP;
    break;
  case _PP_SAT:
    return BrigPackPPsat;
    break;
  case _PS_SAT:
    return BrigPackPSsat;
    break;
  case _SP_SAT:
    return BrigPackSPsat;
    break;
  case _SS_SAT:
    return BrigPackSSsat;
    break;
  case _S_SAT:
    return BrigPackSsat;
    break;
  case _P_SAT:
    return BrigPackPsat;
    break;
  }
}
 

int Query(unsigned int QueryOp, Context* context) {
  // next token should be a dataTypeId
  if (GetTokenType(yylex()) == DATA_TYPE_ID) {
    // next token should be an Operand
    if (!Operand(yylex(), context)) {
      // next should be a comma
      if (yylex() == ',') {
        // then finally an addressable Operand
        if (!AddressableOperand(yylex(), context)) {
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

int Operand(unsigned int first_token, Context* context) {
  if (!Identifier(first_token, context)) {  // an Identifier
    if (GetTokenType(first_token) == REGISTER) {
      BrigOperandReg bor;
      bor.size = 12;
      bor.kind = BrigEOperandReg;
      switch (first_token) {
        case TOKEN_CREGISTER:
          bor.type = Brigb1;
          break;
        case TOKEN_DREGISTER:
          bor.type = Brigb64;
          break;
        case TOKEN_SREGISTER:
          bor.type = Brigb32;
          break;
        case TOKEN_QREGISTER:
          bor.type = Brigb128;
          break;
      }

      bor.reserved = 0;
      std::string name(string_val);
      bor.name = context->add_symbol(name);
      if(!context->operand_map.count(name)) {
        context->operand_map[name] = context->get_operand_offset();
        context->append_operand(&bor);
      }
    }
    return 0;
  } else if (!BaseOperand(first_token, context)) {    // a base Operand
    return 0;
  }
  return 1;
}

int Identifier(unsigned int first_token, Context* context) {
  if (first_token == TOKEN_GLOBAL_IDENTIFIER) {
    return 0;
  } else if (first_token == TOKEN_LOCAL_IDENTIFIER) {
    return 0;
  } else if (GetTokenType(first_token) == REGISTER) {
    return 0;
  }

  return 1;
}

int BaseOperand(unsigned int first_token, Context* context) {
  int next;
  if (first_token == TOKEN_DOUBLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb64,            // type
      0                   // reserved
    };
    boi.bits.d = double_val;
    context->append_operand(&boi);

    return 0;
  } else if (first_token == TOKEN_SINGLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
    };
    boi.bits.f = float_val;
    context->append_operand(&boi);
    return 0;
  } else if (first_token == TOKEN_INTEGER_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
    };
    // TODO(Huy): check context for operation type and decide the type
    boi.bits.u = int_val;
    context->append_operand(&boi);

    return 0;
  } else if (first_token == TOKEN_WAVESIZE) {
    return 0;
  } else if (first_token == '-') {
    if (yylex() == TOKEN_INTEGER_CONSTANT) {
      BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
      };
      // TODO(Huy): check context for operation type and decide the type
      boi.bits.u = -int_val;
      context->append_operand(&boi);

     return 0;
    }
  } else if (GetTokenType(first_token) == DATA_TYPE_ID) {
    // scan next token
    if (yylex() == '(') {   // should be '('
      // check if we have a decimal list single or float list single
      next = yylex();
      if (next == TOKEN_INTEGER_CONSTANT) {
        BrigOperandImmed boi = {
        sizeof(boi),        // size
        BrigEOperandImmed,  // kind
        Brigb32,            // type
        0                   // reserved
        };
        // TODO(Huy): check context for operation type and decide the type
        boi.bits.u = -int_val;
        context->append_operand(&boi);

        next = yylex();
        if (next == ')') {
          return 0;
        } else {
          while (next == ',') {
            next = yylex();
            if (next == TOKEN_INTEGER_CONSTANT) {
              BrigOperandImmed boi = {
                sizeof(boi),        // size
                BrigEOperandImmed,  // kind
                Brigb32,            // type
                0                   // reserved
              };
  // TODO(Huy): check context for operation type and decide the type
              boi.bits.u = int_val;
              context->append_operand(&boi);

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
        BrigOperandImmed boi = {
        sizeof(boi),        // size
        BrigEOperandImmed,  // kind
        Brigb64,            // type
        0                   // reserved
        };
        boi.bits.d = double_val;
        context->append_operand(&boi);

        next = yylex();
        if (next == ')') {
          return 0;
        } else {
          while (next == ',') {
            next = yylex();
            if (next == TOKEN_DOUBLE_CONSTANT) {
              BrigOperandImmed boi = {
                sizeof(boi),        // size
                BrigEOperandImmed,  // kind
                Brigb64,            // type
                0                   // reserved
              };
              boi.bits.d = double_val;
              context->append_operand(&boi);
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

int AddressableOperand(unsigned int first_token, Context* context) {
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

int ArrayOperandList(unsigned int first_token, Context* context) {
  // assumed first_token is '('
  int next;
  while (1) {
    next = yylex();
    if (!Identifier(next, context)) {
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

int CallTargets(unsigned int first_token, Context* context) {
  // assumed first_token is '['
  int next;
  while (1) {
    next = yylex();
    if (!Identifier(next, context)) {
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
}

int CallArgs(unsigned int first_token, Context* context) {
  // assumed first_token is '('
  int next;
  while (1) {
    next = yylex();
    if (next == ')') {
      return 0;
    } else if (!Operand(next, context)) {
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

int RoundingMode(unsigned int first_token,
                 bool* is_ftz,
                 unsigned int* last_token,
                 Context* context) {
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

int Instruction2(unsigned int first_token, Context* context) {
  // First token must be an Instruction2Opcode
  unsigned int next = yylex();
  // to get last token returned by RoundingMode in case _ftz
  unsigned int temp = 0;
  bool is_ftz = false;

  //default value.
  BrigInstBase inst_op = {
    32,
    BrigEInstBase,
    GetOpCode(first_token),
    0,
    BrigNoPacking,
    {0,0,0,0,0}
  };

  if (GetTokenType(first_token) == INSTRUCTION2_OPCODE) {
    if (!RoundingMode(next, &is_ftz, &temp, context)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }

    // check whether there is a Packing
    if (GetTokenType(next) == PACKING) {
      // there is packing
      inst_op.packing = GetPacking(next);
      next = yylex();
    }

    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = GetDataType(next);
      next = yylex();
      std::string oper_str = string_val;
      if (!Operand(next, context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (yylex() == ',') {
          next = yylex();
          oper_str = string_val;
          if (!Operand(next, context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (yylex() == ';') {
              context->append_code<BrigInstBase>(&inst_op); 
              // if the rule is valid, just write to the .code section,
              // may need to edit others, worry that later.
              return 0;
            }
          }
        }
      }
    }
    return 1;
  } else if (GetTokenType(first_token) == INSTRUCTION2_OPCODE_NODT) {
    if (!RoundingMode(next, &is_ftz, &temp, context)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }


    // check the operands
    if (!Operand(next, context)) {
      if (yylex() == ',') {
        if (!Operand(yylex(), context)) {
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
      inst_op.type = GetDataType(next);
      next = yylex();
      std::string oper_str = string_val;
      if (!Operand(next, context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (yylex() == ',') {
          next = yylex();
          oper_str = string_val;
          if (!Operand(next, context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (yylex() == ';') {
              context->append_code<BrigInstBase>(&inst_op); 
              // if the rule is valid, just write to the .code section,
              // may need to edit others, worry that later.
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

int Instruction3(unsigned int first_token, Context* context) {
  // First token must be an Instruction3Opcode
  unsigned int next = yylex();
  // to get last token returned by RoundingMode in case _ftz
  unsigned int temp = 0;
  bool is_ftz = false;

  //default value.
  BrigInstBase inst_op = {
    32,
    BrigEInstBase,
    GetOpCode(first_token),
    0,
    BrigNoPacking,
    {0,0,0,0,0}
  };

  if (GetTokenType(first_token) == INSTRUCTION3_OPCODE) {
    if (!RoundingMode(next, &is_ftz, &temp, context)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }

    // check whether there is a Packing
    if (GetTokenType(next) == PACKING) {
      // there is packing
      inst_op.packing = GetPacking(next);
      next = yylex();
    }

    // now we must have a dataTypeId
    if (GetTokenType(next) == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = GetDataType(next);
      next = yylex();
      std::string oper_str = string_val;

      if (!Operand(next, context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (yylex() == ',') {
          next = yylex();
          oper_str = string_val;
          if (!Operand(next, context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (yylex() == ',') {
              next = yylex();
              oper_str = string_val;
              if (!Operand(next, context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (yylex() == ';')
                  context->append_code<BrigInstBase>(&inst_op); 
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
      inst_op.type = GetDataType(next);
      next = yylex();
      std::string oper_str = string_val;

      if (!Operand(next, context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (yylex() == ',') {
          next = yylex();
          oper_str = string_val;
          if (!Operand(next, context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (yylex() == ',') {
              next = yylex();
              oper_str = string_val;
              if (!Operand(next, context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (yylex() == ';')
                  context->append_code<BrigInstBase>(&inst_op); 
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

int Version(unsigned int first_token, Context* context) {
  // first token must be version keyword

  if (context == NULL) {
    printf("INVALID CONTEXT\n");
    return 1;
  }

  ErrorReporterInterface* err_reporter = context->get_error_reporter();

  BrigDirectiveVersion bdv;
  bdv.kind = BrigEDirectiveVersion;
  bdv.size = 20;
  bdv.reserved = 0;

  // set default values
  bdv.machine = BrigESmall;
  bdv.profile = BrigEFull;
  bdv.ftz = BrigENosftz;
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    bdv.major = int_val;
    // printf("Major = %d\n",int_val);
    if (yylex() == ':') {
      // check for minor
      if (yylex() == TOKEN_INTEGER_CONSTANT) {
        bdv.minor = int_val;
        // printf("Minor = %d\n",int_val);
        int next = yylex();
        if (next == ';') {
        } else if (next == ':') {
          // check for target
          next = yylex();
          while (next != ';') {
            if (GetTokenType(next) == TARGET) {
              switch (next) {
                case _SMALL:
                  bdv.machine = BrigESmall;
                  break;
                case _LARGE:
                  bdv.machine = BrigELarge;
                  break;
                case _FULL:
                  bdv.profile = BrigEFull;
                  break;
                case _REDUCED:
                  bdv.profile = BrigEReduced;
                  break;               
                case _SFTZ:
                  bdv.ftz = BrigESftz;
                  break;
                case _NOSFTZ:
                  bdv.ftz = BrigENosftz;
                  break;    
              }

              next = yylex();
              if (next == ',') {
                next = yylex();      // next target
              } else {
                if (next != ';') {
                  err_reporter->report_error(
                    ErrorReporterInterface::MISSING_SEMICOLON,
                    yylineno);
                  return 1;
                }
              }
            } else {
              err_reporter->report_error(
                ErrorReporterInterface::INVALID_TARGET,
                yylineno);
              return 1;
            }
          }
        } else {
          err_reporter->report_error(
            ErrorReporterInterface::MISSING_SEMICOLON,
            yylineno);
          return 1;
        }
        context->append_directive(&bdv);
        err_reporter->report_error(ErrorReporterInterface::OK, yylineno);
        return 0;
      }
      err_reporter->report_error(
        ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
        yylineno);
    }
    err_reporter->report_error(ErrorReporterInterface::MISSING_COLON, yylineno);
  }
  err_reporter->report_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
                             yylineno);
  return 1;
};

int Alignment(unsigned int first_token, Context* context) {
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
int DeclPrefix(unsigned int first_token,
               bool* recheck_last_token,
               unsigned int* last_token,
               Context* context) {
  unsigned int last_align_token;
  unsigned int next_token;
  *recheck_last_token = false;
  *last_token = 0;

  if (first_token == ALIGN) {
    if (!Alignment(first_token, context)) {
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
      if (!Alignment(next_token, context)) {
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
        if (!Alignment(next_token, context)) {
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
      if (!Alignment(next_token, context)) {
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
        if (!Alignment(next_token, context)) {
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

int FBar(unsigned int first_token, Context* context) {
  // first token must be _FBAR
  if (yylex() == '(' )
    if (yylex() == TOKEN_INTEGER_CONSTANT)
      if (yylex() == ')')
        return 0;

  return 1;
}


int ArrayDimensionSet(unsigned int first_token,
                      bool* rescan_last_token,
                      unsigned int* last_token,
                      Context* context) {
  // first token must be '['
  *rescan_last_token = false;
  unsigned int next_token = yylex();
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

int ArgumentDecl(unsigned int first_token,
                 bool* rescan_last_token,
                 unsigned int* last_token,
                 Context* context) {
  bool rescan_after_declPrefix;
  unsigned int last_token_of_declPrefix;
  unsigned int next;
  if (!DeclPrefix(first_token,
                  &rescan_after_declPrefix,
                  &last_token_of_declPrefix,
                  context)) {
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
      BrigDataType16_t data_type = GetDataType(next);
      next = yylex();
      if (next == TOKEN_LOCAL_IDENTIFIER) {
        // should have a meaning for DATA_TYPE_ID.
        // for argument, we need to set a BrigDirectiveSymbol
        // and write the corresponding string into .string section.


        std::string arg_name = string_val;
        int arg_name_offset = context->add_symbol(arg_name);
        // scan for arrayDimensions
        next = yylex();
        if (next == '[') {
          if (!ArrayDimensionSet(next,
                                 rescan_last_token,
                                 last_token,
                                 context)) {
            return 0;
          }
        } else {
          // no arrayDimensions
          BrigdOffset32_t dsize = context->get_directive_offset();
          BrigDirectiveSymbol sym_decl = {
          context->get_code_offset(),
          BrigArgSpace,
          BrigNone,
          0,
          0,
          0,
          arg_name_offset,
          data_type,
          1,
          0,               // d_init = 0 for arg
          0                // reserved
          };
          // append the DirectiveSymbol to .directive section.
          context->append_directive_symbol<BrigDirectiveSymbol>(&sym_decl);

          // update the current DirectiveFunction.
          // 1. update the directive offset.
          BrigDirectiveFunction bdf;
          context->get_directive<BrigDirectiveFunction>(
                    context->current_bdf_offset,
                    &bdf);
          if (bdf.d_firstScopedDirective == bdf.d_nextDirective) {
            bdf.d_nextDirective += 36;
            bdf.d_firstScopedDirective = bdf.d_nextDirective;
          } else {
            bdf.d_nextDirective += 36;
          }
          // std::cout << bdf.d_nextDirective << std::endl;
          // update param count
          if (context->arg_output) {
            bdf.outParamCount++;
          } else {
            if (!bdf.inParamCount)
              bdf.d_firstInParam = dsize;
            bdf.inParamCount++;
          }
          unsigned char * bdf_charp =
            reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp,
                                          context->current_bdf_offset,
                                          40);

          context->get_directive<BrigDirectiveFunction>(
                    context->current_bdf_offset,
                    &bdf);
          // std::cout << bdf.size << std::endl;


          *last_token = next;
          *rescan_last_token = true;
          return 0;
        }
      }
    }
  }
  return 1;
}

int ArgumentListBody(unsigned int first_token,
                     bool* rescan_last_token,
                     unsigned int* last_token,
                     Context* context) {
  *last_token = 0;
  *rescan_last_token = false;

  unsigned int prev_token = 0;
  bool rescan = false;
  while (1) {
    if (!ArgumentDecl(first_token, &rescan, &prev_token, context)) {
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

int FunctionDefinition(unsigned int first_token,
                       bool* rescan_last_token,
                       unsigned int* last_token,
                       Context* context) {
  *last_token = 0;
  * rescan_last_token = false;

  unsigned int token_to_scan;
  bool rescan;

  if (!DeclPrefix(first_token, &rescan, &token_to_scan, context)) {
    if (!rescan)
      token_to_scan = yylex();

    if (token_to_scan == FUNCTION) {
      // add default struct (Miao)

      context->current_bdf_offset = context->get_directive_offset();
      BrigdOffset32_t bdf_offset = context->current_bdf_offset;

      BrigDirectiveFunction bdf = {
      40,                      // size
      BrigEDirectiveFunction,  // kind
      context->get_code_offset(),
      0,
      0,
      bdf_offset+40,          // d_firstScopedDirective
      0,
      bdf_offset+40,          // d_nextDirective
      BrigNone,
      0,
      0,
      0,
      };

      context->append_directive(&bdf);

      // update it when necessary.
      // the later functions should have a entry point of bdf
      // just update it in time.
      //

      if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
        // should have meaning of Global_Identifier,
        // and check if there is existing global identifier
        // if there is, just use the current string,
        // if not write into string.

        std::string func_name = string_val;
        BrigsOffset32_t check_result = context->add_symbol(func_name);
        
        // add the func_name to the func_map.
        context->func_map[func_name] = context->current_bdf_offset;

        unsigned char* value = reinterpret_cast<unsigned char*>(&check_result);
        context->update_directive_bytes(value,
                                        context->current_bdf_offset + 8,
                                        bdf.size);

        /* Debug */
        // BrigDirectiveFunction get;
        // context->get_d<BrigDirectiveFunction>(bdf_offset, &get);
        // std::cout << get.s_name << std::endl;

        // check return argument list
        if (yylex() == '(') {
          context->arg_output = true;
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan,
                                       context)) {
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
          context->arg_output = false;
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan,
                                       context)) {
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
          if (!FBar(token_to_scan, context)) {
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

int FunctionDecl(unsigned int first_token, Context* context) {
  unsigned int token_to_scan;
  bool rescan;

  if (!DeclPrefix(first_token, &rescan, &token_to_scan, context)) {
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
                                       &token_to_scan,
                                       context)) {
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
                                       &token_to_scan,
                                       context)) {
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
          if (!FBar(token_to_scan, context)) {
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

int ArgBlock(unsigned int first_token, Context* context) {
  // first token should be {
  bool rescan = false;
  unsigned int last_token;
  unsigned int next_token = 0;
  // printf("In arg scope\n");
  while (1) {
    next_token = yylex();
    if ((GetTokenType(next_token) == INSTRUCTION2_OPCODE) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_NODT) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(next_token, context)) {
      } else {
        return 1;
      }
    } else if ((GetTokenType(next_token) == INSTRUCTION3_OPCODE) ||
               (GetTokenType(next_token) == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(next_token, context)) {
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == QUERY_OP) {  // Query Operation
      if (!Query(next_token, context)) {
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
      if (!Branch(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == TOKEN_LABEL) {  // label
      // need to check the lable_map, and update when necessary.
      // std::string label_name = string_val;
      // context->lable_o_map[label_name] = context->get_operand_offset(); // set the lable_map
      // add operand in .operand;
      // check if any entry with the same key in label_c_map;
      // update the corresponding instructions.
      // 
            // if no correlated instructions, then later instructions can directly use the label.
      if (yylex() == ':') {
      } else {
        return 1;
      }
    } else if (next_token == CALL) {  // call (only inside argblock
      if (!Call(next_token, context)) {
      } else {
        return 1;
      }
    } else if ((next_token == ALIGN) ||
               (next_token == CONST) ||
               (next_token == STATIC) ||
               (next_token == EXTERN)) {
      if (!DeclPrefix(first_token, &rescan, &last_token, context)) {
        if (!rescan)
          next_token = yylex();
        else
          next_token = last_token;

        if (GetTokenType(next_token) == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(next_token, context)) {
          }
        } else if (GetTokenType(next_token) == UNINITIALIZABLE_ADDRESS) {
          // uninitializable decl
          if (!UninitializableDecl(next_token, context)) {
          }
        } else if (next_token == ARG) {
          // arg uninitializable decl
          if (!ArgUninitializableDecl(next_token, context)) {
          }
        }
      }
    } else if (GetTokenType(next_token) == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == UNINITIALIZABLE_ADDRESS) {
      // printf("An unintializable address\n");
      if (!UninitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == ARG) {
      if (!ArgUninitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == '{') {
      printf("Argument scope cannot be nested\n");
      return 1;
    } else if (next_token == '}') {
      return 0;
    } else {
      break;
    }
  }
  return 1;
}

int Codeblock(unsigned int first_token, Context* context) {
  // first token should be '{'
  bool rescan = false;
  unsigned int last_token;
  unsigned int next_token = 0;

  while (1) {
    next_token = yylex();
    if ((GetTokenType(next_token) == INSTRUCTION2_OPCODE) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_NODT) ||
        (GetTokenType(next_token) == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(next_token, context)) {
        //update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive<BrigDirectiveFunction>(
                  context->current_bdf_offset,
                  &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
      
      } else {
        return 1;
      }
    } else if ((GetTokenType(next_token) == INSTRUCTION3_OPCODE) ||
               (GetTokenType(next_token) == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(next_token, context)) {
        //update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive<BrigDirectiveFunction>(
                  context->current_bdf_offset,
                  &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);      
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == QUERY_OP) {  // Query Operation
      if (!Query(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == RET) {  // ret operation
      if (yylex() == ';') {
      BrigcOffset32_t csize = context->get_code_offset();
      BrigInstBase op_ret = {
        32,
        BrigEInstBase,
        BrigRet,
        Brigf32,
        BrigNoPacking,
        {0, 0, 0, 0, 0}
        };
      // write to .code section
      context->append_code<BrigInstBase>(&op_ret);
      BrigDirectiveFunction bdf;
      context->get_directive<BrigDirectiveFunction>(
                context->current_bdf_offset,
                &bdf);
      bdf.operationCount++;

      unsigned char * bdf_charp =
        reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp,
                                      context->current_bdf_offset,
                                      bdf.size);

      } else {
        printf("Missing ';' at the end of ret operation\n");
        return 1;
      }
    } else if ((next_token == BRN) ||
               (next_token == CBR)) {
      if (!Branch(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == TOKEN_LABEL) {  // label
      if (yylex() == ':') {
      } else {
        return 1;
      }
    } else if (next_token == '{') {  // argument scope -> inner codeblock
      if (!ArgBlock(next_token, context)) {
        // printf("Out of arg scope \n");
      } else {
        return 1;
      }
    } else if ((next_token == ALIGN) ||
               (next_token == CONST) ||
               (next_token == STATIC) ||
               (next_token == EXTERN)) {
      if (!DeclPrefix(first_token, &rescan, &last_token, context)) {
        if (!rescan)
          next_token = yylex();
        else
          next_token = last_token;

        if (GetTokenType(next_token) == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(next_token, context)) {
          } else {
            return 1;
          }
        } else if (GetTokenType(first_token) == UNINITIALIZABLE_ADDRESS) {
          // uninitializable decl
          if (!UninitializableDecl(first_token, context)) {
          } else {
            return 1;
          }
        } else {
          return 1;
        }
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (GetTokenType(next_token) == UNINITIALIZABLE_ADDRESS) {
      // printf("An unintializable address\n");
      if (!UninitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == '}') {
      return 0;
    } else {
      break;
    }
  }
  return 1;
}

int Function(unsigned int first_token, Context* context) {
  bool rescan = false;
  unsigned int last_token = 0;
  if (!FunctionDefinition(first_token, &rescan, &last_token, context)) {
    if (!rescan)
      last_token = yylex();
    if (!Codeblock(last_token, context)) {
      if (yylex() == ';')
        return 0;
      else
        printf("Missing ';'\n");
    }
  }
  return 1;
}

int Program(unsigned int first_token, Context* context) {
  int result;
  unsigned int last_token;
  bool rescan;

  if (first_token == VERSION) {
    if (!Version(first_token, context)) {
      // parse topLevelStatement
      first_token = yylex();
      while (first_token) {
        if ( (first_token == ALIGN) ||
             (first_token == CONST) ||
             (first_token == EXTERN) ||
             (first_token == STATIC) ) {
          result = DeclPrefix(first_token, &rescan, &last_token, context);
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
                                           &first_token,
                                           context)) {
                if (!rescan)
                  first_token = yylex();

                if (first_token == ')')
                  first_token = yylex();
                else
                  return 1;  // missing closing )
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
                                           &first_token,
                                           context)) {
                if (!rescan)
                  first_token = yylex();
                if (first_token == ')')
                  first_token = yylex();
                else
                  return 1;  // missing closing )
              } else {
                return 1;
              }
            } else {  // missing '('
              return 1;
            }            // if found '(' - argList

            // check for optional FBar
            if (first_token == _FBAR) {
              if (!FBar(first_token, context)) {
                first_token = yylex();
              } else {
                return 1;
              }
            }

            if (first_token == ';') {  // this is a functionDecl
              first_token = yylex();
              continue;
            } else if (first_token == '{') {
              // so this must be a functionDefinition
              if (!Codeblock(first_token, context)) {
                // check codeblock of function
                first_token = yylex();
                if (first_token == ';') {
                  first_token = yylex();
                  continue;
                } else {
                  return 1;
                }
              } else {
                printf("Error in function's codeblock\n");
                return 1;
              }
            }
          }       // if found TOKEN_GLOBAL_ID
        } else if (GetTokenType(first_token) == INITIALIZABLE_ADDRESS) {
          // global initializable
          // this is an initializable declaration
          if (!InitializableDecl(first_token, context)) {
            first_token = yylex();
          } else {
            return 1;
          }
        } else  {
          return 1;  // currently only support functions
        }
      }    // while (first_token)
      return 0;
    }   // if (!Version)
  } else {
    printf("Program must start with version statement.\n");
  }
  return 1;
}

int OptionalWidth(unsigned int first_token, Context* context) {
  // first token must be _WIDTH
  unsigned int next_token = yylex();
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

int Branch(unsigned int first_token, Context* context) {
  unsigned int op = first_token;  // CBR or BRN
  unsigned int current_token = yylex();


  // check for optionalWidth
  if (current_token == _WIDTH) {
    if (!OptionalWidth(current_token, context)) {
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
    if (!Operand(current_token, context)) {
      if (yylex() == ',') {
        current_token = yylex();
        if (current_token == TOKEN_LABEL) {
          current_token = yylex();  // should be ';'
        } else if (!Identifier(current_token, context)) {
          current_token = yylex();  // should be ';'

        } else if (!Operand(current_token, context)) {
          if (yylex() == ',') {
            current_token = yylex();
            if (current_token == TOKEN_LABEL) {  // LABEL
              current_token = yylex();  // should be ';'
            } else if (current_token == '[') {
              current_token = yylex();
              if (!Identifier(current_token, context)) {
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
    } else if (!Identifier(current_token, context)) {
      current_token = yylex();

      if (current_token == ';')
        return 0;
      else if (current_token == ',') {
        if (yylex() == '[') {
          current_token = yylex();
          if (current_token == TOKEN_LABEL) {
            current_token = yylex();    // should be ']'
          } else if (!Identifier(current_token, context)) {
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

int Call(unsigned int first_token, Context* context) {
  // first token is "call"
  unsigned int next = yylex();

  // optional width
  if (next == _WIDTH) {
    if (!OptionalWidth(next, context)) {
      next = yylex();
    } else {
      printf("Error in optionalwidth\n");
      return 1;
    }
  }

  if (!Operand(next, context)) {
    next = yylex();
    // check for twoCallArgs
    if (next == '(') {
      if (!CallArgs(next, context))
        next = yylex();
      else
        return 1;
    }

    if (next == '(') {
      if (!CallArgs(next, context))
        next = yylex();
      else
        return 1;
    }

    // check for CallTarget
    if (next == ';') {
      return 0;
    } else if (next == '[') {
      if (!CallTargets(next, context)) {
        if (yylex() == ';')
           return 0;
      }
      return 1;
    }
  }
  return 1;
}

int Initializer(unsigned int first_token,
                bool* rescan,
                unsigned int* last_token,
                Context* context) {
  // first token should be '='
  *rescan = false;
  *last_token =0;
  unsigned int next = yylex();

  if (next == TOKEN_LABEL) {
    printf("Label initializers must be inside '{' and '}'\n");
    return 1;
  } else if (next == '{') {
    next = yylex();
  }

  // check type of initializer
  if (next == TOKEN_LABEL) {
    // label initializer
    while (1) {
      next = yylex();
      if (next == ',') {
        next = yylex();
        if (next == TOKEN_LABEL) {
          continue;
        } else {
          return 1;
        }
      } else {
        *last_token = next;
        *rescan = true;
        break;
      }
    }  // while(1)

  } else if (next == TOKEN_INTEGER_CONSTANT) {
    // decimal initializer
    while (1) {
      next = yylex();
      if (next == ',') {
        next = yylex();
        if (next == TOKEN_INTEGER_CONSTANT) {
          continue;
        } else {
          return 1;
        }
      } else {
        *last_token = next;
        *rescan = true;
        break;
      }
    }  // while(1)
  } else if (next == TOKEN_SINGLE_CONSTANT) {
    // single initializer
    while (1) {
      next = yylex();
      if (next == ',') {
        next = yylex();
        if (next == TOKEN_SINGLE_CONSTANT) {
          continue;
        } else {
          return 1;
        }
      } else {
        *last_token = next;
        *rescan = true;
        break;
      }
    }  // while(1)
  } else if (next == TOKEN_DOUBLE_CONSTANT) {
    // double initializer
    while (1) {
      next = yylex();
      if (next == ',') {
        next = yylex();
        if (next == TOKEN_DOUBLE_CONSTANT) {
          continue;
        } else {
          return 1;
        }
      } else {
        *last_token = next;
        *rescan = true;
        break;
      }
    }  // while(1)
  } else {
    return 1;
  }
  if (!*rescan)
    next = yylex();
  if (next == '}') {
    *rescan = false;
  } else {
    *rescan = true;
    *last_token = next;
  }


  return 0;
}

int InitializableDecl(unsigned int first_token, Context* context) {
  // first_token is READONLY or GLOBAL
  bool rescan;
  unsigned int last_token;
  unsigned int next = yylex();

  if (GetTokenType(next) == DATA_TYPE_ID) {
    next = yylex();
    if (!Identifier(next, context)) {
      // scan for arrayDimensions
      next = yylex();
      if (next == '[') {
        if (!ArrayDimensionSet(next, &rescan, &last_token, context)) {
          if (!rescan)
            next = yylex();
          else
            next = last_token;
        }
      }
      if (!Initializer(next, &rescan, &last_token, context)) {
        if (rescan)
          next = last_token;
        else
          next = yylex();

        if (next == ';')
          return 0;
        else
          printf("Missing ';' at the end of statement\n");
      }
    }
  }
  return 1;
};

int UninitializableDecl(unsigned int first_token, Context* context) {
  // first_token is PRIVATE, GROUP or SPILL
  bool rescan;
  unsigned int last_token;
  unsigned int next = yylex();
  if (GetTokenType(next) == DATA_TYPE_ID) {
    // printf("DataTypeId\n");
    next = yylex();
    if (!Identifier(next, context)) {
      // scan for arrayDimensions
      next = yylex();
      if (next == '[') {
        if (!ArrayDimensionSet(next, &rescan, &last_token, context)) {
          if (!rescan)
            next = yylex();
          else
            next = last_token;
        }
      }

      if (next == ';')
        return 0;
      else
        printf("Missing ';' at the end of statement\n");
    }
  }
  return 1;
}

int ArgUninitializableDecl(unsigned int first_token, Context* context) {
  // first token is ARG
  bool rescan;
  unsigned int last_token;
  unsigned int next = yylex();
  if (GetTokenType(next) == DATA_TYPE_ID) {
    // printf("DataTypeId\n");
    next = yylex();
    if (!Identifier(next, context)) {
      // scan for arrayDimensions
      next = yylex();
      if (next == '[') {
        if (!ArrayDimensionSet(next, &rescan, &last_token, context)) {
          if (!rescan)
            next = yylex();
          else
            next = last_token;
        }
      }

      if (next == ';')
        return 0;
      else
        printf("Missing ';' at the end of statement\n");
    }
  }
  return 1;
}

int FileDecl(unsigned int first_token, Context* context) {
  return 1;
}

}  // namespace brig
}  // namespace hsa
